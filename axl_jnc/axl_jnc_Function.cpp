#include "stdafx.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetPropertyAccessorString (EPropertyAccessor PropertyAccessor)
{
	switch (PropertyAccessor)
	{
	case EPropertyAccessor_Get:
		return _T("get");

	case EPropertyAccessor_Set:
		return _T("set");

	default:
		return _T("undefined-property-accessor");
	};
}

//.............................................................................

CFunction::CFunction ()
{
	m_ItemKind = EModuleItem_Function;
	m_FunctionKind = EFunction_Undefined;
	m_PropertyAccessorKind = EPropertyAccessor_Undefined;
	m_pType = NULL;
	m_pClosureType = NULL;
	m_pClassType = NULL;
	m_pVTableType = NULL;
	m_VTableIndex = -1;
	m_VTableIndexDelta = 0;
	m_pAnchorNamespace = NULL;
	m_pNamespace = NULL;
	m_pBlock = NULL;
	m_pScope = NULL;
	m_pExternFunction = NULL;
	m_pLlvmFunction = NULL;
	m_pfn = NULL;
}

CClassType* 
CFunction::GetOriginClassType ()
{
	EType TypeKind = m_pVTableType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Property:
		return ((CPropertyType*) m_pVTableType)->GetParentClassType ();

	case EType_Class:
	case EType_Interface:
		return (CClassType*) m_pVTableType;

	default:
		ASSERT (false);
		return NULL;
	}
}

rtl::CString
CFunction::CreateArgString ()
{
	if (m_ArgList.IsEmpty ())
	return 
		(m_pType->GetFlags () & EFunctionTypeFlag_IsVarArg) ? 
		(m_pType->GetFlags () & EFunctionTypeFlag_IsUnsafeVarArg) ? 
		_T("(unsafe ...)") : _T("(safe ...)") : _T("()");

	rtl::CIteratorT <jnc::CFunctionFormalArg> Arg = m_ArgList.GetHead ();

	rtl::CString String;
	String.Format (
		_T("(%s %s"), 
		Arg->GetType ()->GetTypeString (),
		Arg->GetName ()
		);

	for (Arg++; Arg; Arg++)
		String.AppendFormat (
			_T(", %s %s"), 
			Arg->GetType ()->GetTypeString (),
			Arg->GetName ()
			);

	if (!(m_pType->GetFlags () & EFunctionTypeFlag_IsVarArg))
		String.Append (_T(")"));
	else if (m_pType->GetFlags () & EFunctionTypeFlag_IsUnsafeVarArg)
		String.Append (_T(", unsafe ...)"));
	else
		String.Append (_T(", safe ...)"));

	return String;
}

llvm::Function* 
CFunction::GetLlvmFunction ()
{
	if (m_pLlvmFunction)
		return m_pLlvmFunction;

	if (m_pExternFunction)
		return m_pExternFunction->GetLlvmFunction ();

	llvm::FunctionType* pLlvmType = m_pType->GetLlvmType ();
	m_pLlvmFunction = llvm::Function::Create (
		pLlvmType, 
		llvm::Function::ExternalLinkage, 
		(const tchar_t*) m_Tag, 
		m_pModule->m_pLlvmModule
		);

	return m_pLlvmFunction;
}

//.............................................................................

CFunction*
CFunctionOverload::FindOverload (rtl::CBoxListT <CValue>* pArgList) const
{
	// TODO: implement 

	return m_pFunction; 
}

CFunction*
CFunctionOverload::FindOverload (
	CFunctionType* pType,
	bool IsClosure
	) const
{
	if (IsClosure)
	{
		if (pType->Cmp (m_pFunction->GetClosureType ()) == 0)
			return m_pFunction;

		size_t Count = m_OverloadArray.GetCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CFunction* pFunction = GetFunction (i);
			if (pType->Cmp (pFunction->GetClosureType ()) == 0)
				return pFunction;
		}
	}
	else
	{
		if (pType->Cmp (m_pFunction->GetType ()) == 0)
			return m_pFunction;

		size_t Count = m_OverloadArray.GetCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CFunction* pFunction = GetFunction (i);
			if (pType->Cmp (pFunction->GetType ()) == 0)
				return pFunction;
		}
	}

	return NULL;
}

bool
CFunctionOverload::AddOverload (CFunction* pFunction)
{
	if (!m_pFunction)
	{
		m_pFunction = pFunction;
		return true;
	}

	m_OverloadArray.Append (pFunction);

	// TODO: check that no overloads overlap

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
