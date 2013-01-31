#include "stdafx.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetFunctionKindString (EFunction FunctionKind)
{
	switch (FunctionKind)
	{
	case EFunction_Named:
		return _T("named-function");

	case EFunction_Getter:
		return _T("get");

	case EFunction_Setter:
		return _T("set");

	case EFunction_PreConstructor:
		return _T("preconstruct");

	case EFunction_Constructor:
		return _T("this");

	case EFunction_StaticConstructor:
		return _T("static this");

	case EFunction_Destructor:
		return _T("~this");

	case EFunction_CallOperator:
		return _T("call-operator");

	case EFunction_CastOperator:
		return _T("cast-operator");

	case EFunction_UnaryOperator:
		return _T("unary-operator");

	case EFunction_BinaryOperator:
		return _T("binary-operator");

	case EFunction_Internal:
		return _T("internal");

	case EFunction_Thunk:
		return _T("thunk");

	case EFunction_AutoEv:
		return _T("autoev");

	default:
		return _T("undefined-function-kind");
	};
}

//.............................................................................

CFunction::CFunction ()
{
	m_ItemKind = EModuleItem_Function;
	m_FunctionKind = EFunction_Undefined;
	m_pParentNamespace = NULL;
	m_pType = NULL;
	m_pShortType = NULL;
	m_pOrphanNamespace = NULL;
	m_pExternFunction = NULL;
	m_pClassType = NULL;
	m_pVirtualOriginClassType = NULL;
	m_pProperty = NULL;
	m_ClassVTableIndex = -1;
	m_PropertyVTableIndex = -1;
	m_pBlock = NULL;
	m_pScope = NULL;
	m_pScopeLevelVariable = NULL;
	m_pLlvmFunction = NULL;
	m_pfn = NULL;
}

rtl::CString
CFunction::CreateArgString ()
{
	if (m_ArgList.IsEmpty ())
	return 
		(m_pType->GetFlags () & EFunctionTypeFlag_VarArg) ? 
		(m_pType->GetFlags () & EFunctionTypeFlag_UnsafeVarArg) ? 
		_T("(unsafe ...)") : _T("(safe ...)") : _T("()");

	rtl::CIteratorT <CFunctionFormalArg> Arg = m_ArgList.GetHead ();

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

	if (!(m_pType->GetFlags () & EFunctionTypeFlag_VarArg))
		String.Append (_T(")"));
	else if (m_pType->GetFlags () & EFunctionTypeFlag_UnsafeVarArg)
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

	llvm::FunctionType* pLlvmType = (llvm::FunctionType*) m_pType->GetLlvmType ();
	m_pLlvmFunction = llvm::Function::Create (
		pLlvmType, 
		llvm::Function::ExternalLinkage, 
		(const tchar_t*) m_Tag, 
		m_pModule->m_pLlvmModule
		);

	return m_pLlvmFunction;
}

CFunction*
CFunction::FindOverload (CFunctionType* pType)
{
	if (pType->Cmp (m_pType) == 0)
		return this;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OverloadArray [i];
		if (pType->Cmp (pFunction->m_pType) == 0)
			return pFunction;
	}

	return NULL;
}

CFunction*
CFunction::FindShortOverload (CFunctionType* pType)
{
	if (pType->Cmp (m_pShortType) == 0)
		return this;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OverloadArray [i];
		if (pType->Cmp (pFunction->m_pShortType) == 0)
			return pFunction;
	}

	return NULL;
}

CFunction*
CFunction::ChooseOverload (const rtl::CBoxListT <CValue>* pArgList)
{
	if (m_OverloadArray.IsEmpty ())
		return this;

	ECast BestCastKind = m_pModule->m_OperatorMgr.GetArgCastKind (m_pType, pArgList);
	CFunction* pBestFunction = BestCastKind ? this : NULL;

	size_t Count = m_OverloadArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OverloadArray [i];
		ECast CastKind = m_pModule->m_OperatorMgr.GetArgCastKind (pFunction->GetType (), pArgList);
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

bool
CFunction::AddOverload (CFunction* pFunction)
{
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

CFunction*
GetItemUnnamedMethod (
	CModuleItem* pItem,
	EFunction FunctionKind
	)
{
	if (pItem->GetItemKind () == EModuleItem_Property)
	{
		CProperty* pProperty = (CProperty*) pItem;
		switch (FunctionKind)
		{
		case EFunction_Constructor:
			return pProperty->GetConstructor ();

		case EFunction_StaticConstructor:
			return pProperty->GetStaticConstructor ();

		case EFunction_Destructor:
			return pProperty->GetDestructor ();

		case EFunction_Getter:
			return pProperty->GetGetter ();

		case EFunction_Setter:
			return pProperty->GetSetter ();
		}
	}
	else if (pItem->GetItemKind () == EModuleItem_Type && ((CType*) pItem)->GetTypeKind () == EType_Class)
	{
		CClassType* pClassType = (CClassType*) pItem;
		switch (FunctionKind)
		{
		case EFunction_PreConstructor:
			return pClassType->GetPreConstructor ();

		case EFunction_Constructor:
			return pClassType->GetConstructor ();

		case EFunction_StaticConstructor:
			return pClassType->GetStaticConstructor ();

		case EFunction_Destructor:
			return pClassType->GetDestructor ();
		}
	}

	return NULL;
}

bool
CFunction::ResolveOrphan ()
{
	ASSERT (m_pOrphanNamespace);

	CModuleItem* pItem = m_pOrphanNamespace->FindItemTraverse (m_OrphanName);
	if (!pItem)
	{
		err::SetFormatStringError (_T("unresolved orphan function '%s'"), m_Tag);
		return false;
	}

	CFunction* pOriginFunction = NULL;

	EModuleItem ItemKind = pItem->GetItemKind ();
	if (m_FunctionKind == EFunction_Named)
	{
		if (ItemKind != EModuleItem_Function)
		{
			err::SetFormatStringError (_T("'%s' is not a function"), m_Tag);
			return false;
		}

		pOriginFunction = (CFunction*) pItem;
	}
	else
	{
		pOriginFunction = GetItemUnnamedMethod (pItem, m_FunctionKind);
		if (!pOriginFunction)
		{
			err::SetFormatStringError (_T("'%s' has no %d"), m_Tag, GetFunctionKindString (m_FunctionKind));
			return false;
		}
	}

	pOriginFunction = pOriginFunction->FindShortOverload (m_pType);
	if (!pOriginFunction)
	{
		err::SetFormatStringError (_T("'%s': overload not found"), m_Tag);
		return false;
	}

	if (pOriginFunction->HasBody () || pOriginFunction->m_pExternFunction)
	{
		err::SetFormatStringError (_T("'%s' already has a body"), m_Tag);
		return false;
	}

	ASSERT (pOriginFunction->m_FunctionKind == m_FunctionKind);
	pOriginFunction->m_pExternFunction = this;

	if (!m_StorageKind)
	{
		m_StorageKind = pOriginFunction->m_StorageKind;
	}
	else if (m_StorageKind != pOriginFunction->m_StorageKind)
	{
		err::SetFormatStringError (_T("storage specifier mismatch for orphan function '%s'"), m_Tag);
		return false;
	}

	m_AccessKind = pOriginFunction->m_AccessKind;
	m_pParentNamespace = pOriginFunction->m_pParentNamespace;
	m_pType = pOriginFunction->m_pType;
	m_pShortType = pOriginFunction->m_pShortType;
	m_Name = pOriginFunction->m_Name;
	m_QualifiedName = pOriginFunction->m_QualifiedName;
	m_Tag = pOriginFunction->m_Tag;
	m_pClassType = pOriginFunction->m_pClassType;
	m_pVirtualOriginClassType = pOriginFunction->m_pVirtualOriginClassType;
	m_pProperty = pOriginFunction->m_pProperty;

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
