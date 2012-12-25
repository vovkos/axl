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
	m_pShortType = NULL;
	m_pClassType = NULL;
	m_pVTableType = NULL;
	m_VTableIndex = -1;
	m_pAnchorNamespace = NULL;
	m_pNamespace = NULL;
	m_pBlock = NULL;
	m_pScope = NULL;
	m_pScopeLevelVariable = NULL;
	m_pExternFunction = NULL;
	m_pLlvmFunction = NULL;
	m_pfn = NULL;
}

CClassType* 
CFunction::GetVTableClassType ()
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
CFunctionOverload::ChooseOverload (rtl::CBoxListT <CValue>* pArgList) const
{
	ASSERT (m_pFunction);

	CModule* pModule = m_pFunction->GetType ()->GetModule ();

	ECast BestCastKind = pModule->m_OperatorMgr.GetCallCastKind (m_pFunction->GetType (), pArgList);
	CFunction* pBestFunction = BestCastKind ? m_pFunction : NULL;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OverloadArray [i];
		ECast CastKind = pModule->m_OperatorMgr.GetCallCastKind (pFunction->GetType (), pArgList);

		if (!CastKind)
			continue;

		if (CastKind == BestCastKind)
		{
			err::SetFormatStringError (_T("ambiguous call to overloaded function"));
			return NULL;
		}

		if (CastKind > BestCastKind)
		{
			pBestFunction = pFunction;
			BestCastKind = CastKind;
		}
	}

	if (!pBestFunction)
	{
		err::SetFormatStringError (_T("none of the %d overloads accept the specified argument list"), Count + 1);
		return NULL;
	}

	return pBestFunction; 
}

CFunction*
CFunctionOverload::FindOverload (CFunctionType* pType) const
{
	if (pType->Cmp (m_pFunction->GetType ()) == 0)
		return m_pFunction;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OverloadArray [i];
		if (pType->Cmp (pFunction->GetType ()) == 0)
			return pFunction;
	}

	return NULL;
}

CFunction*
CFunctionOverload::FindShortOverload (CFunctionType* pType) const
{
	if (pType->Cmp (m_pFunction->GetShortType ()) == 0)
		return m_pFunction;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OverloadArray [i];
		if (pType->Cmp (pFunction->GetShortType ()) == 0)
			return pFunction;
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

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pExistingFunction = m_OverloadArray [i];
		
		if (pFunction->GetType ()->GetArgSignature ().Cmp (pExistingFunction->GetType ()->GetArgSignature ()) == 0)
		{
			err::SetFormatStringError (_T("illegal function overload: duplicate argument signature"));
			return false;
		}
	}

	m_OverloadArray.Append (pFunction);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
