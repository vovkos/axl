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
	static const tchar_t* StringTable [EFunction__Count] = 
	{
		_T("undefined-function-kind"),  // EFunction_Undefined,
		_T("named-function"),           // EFunction_Named,
		_T("get"),                      // EFunction_Getter,
		_T("set"),                      // EFunction_Setter,
		_T("preconstruct"),             // EFunction_PreConstructor,
		_T("this"),                     // EFunction_Constructor,
		_T("static this"),              // EFunction_StaticConstructor,
		_T("~this"),                    // EFunction_Destructor,
		_T("call-operator"),            // EFunction_CallOperator,
		_T("cast-operator"),            // EFunction_CastOperator,
		_T("unary-operator"),           // EFunction_UnaryOperator,
		_T("binary-operator"),          // EFunction_BinaryOperator,
		_T("autoev"),                   // EFunction_AutoEv,
		_T("internal"),                 // EFunction_Internal, 
		_T("thunk"),                    // EFunction_Thunk,
	};

	return FunctionKind >= 0 && FunctionKind < EFunction__Count ? 
		StringTable [FunctionKind] : 
		StringTable [EFunction_Undefined];
}

//.............................................................................

int
GetFunctionKindFlags (EFunction FunctionKind)
{
	static int FlagTable [EFunction__Count] = 
	{
		0,                              // EFunction_Undefined,
		0,                              // EFunction_Named,
		EFunctionKindFlag_NoStorage |   // EFunction_Getter,
		EFunctionKindFlag_NoOverloads,                      		
		EFunctionKindFlag_NoStorage,    // EFunction_Setter,
		EFunctionKindFlag_NoStorage |   // EFunction_PreConstructor,
		EFunctionKindFlag_NoOverloads |
		EFunctionKindFlag_NoArgs,       
		EFunctionKindFlag_NoStorage,    // EFunction_Constructor,
		EFunctionKindFlag_NoStorage |   // EFunction_StaticConstructor,
		EFunctionKindFlag_NoOverloads |
		EFunctionKindFlag_NoArgs,              
		EFunctionKindFlag_NoStorage |   // EFunction_Destructor,
		EFunctionKindFlag_NoOverloads |
		EFunctionKindFlag_NoArgs,              
		0,                              // EFunction_CallOperator,
		EFunctionKindFlag_NoOverloads | // EFunction_CastOperator,
		EFunctionKindFlag_NoArgs,       
		EFunctionKindFlag_NoOverloads | // EFunction_UnaryOperator,
		EFunctionKindFlag_NoArgs,       
		0,                              // EFunction_BinaryOperator,
		0,                              // EFunction_AutoEv,
		0,                              // EFunction_Internal, 
		0,                              // EFunction_Thunk,
	};

	return FunctionKind >= 0 && FunctionKind < EFunction__Count ? FlagTable [FunctionKind] : 0;
}

//.............................................................................

CFunction::CFunction ()
{
	m_ItemKind = EModuleItem_Function;
	m_FunctionKind = EFunction_Undefined;
	m_pParentNamespace = NULL;
	m_pType = NULL;
	m_pOrphanNamespace = NULL;
	m_pExternFunction = NULL;
	m_pClassType = NULL;
	m_pThisArgType = NULL;
	m_pThisType = NULL;
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
	rtl::CString String = _T('(');

	rtl::CArrayT <CType*> ArgTypeArray = m_pType->GetArgTypeArray ();
	size_t ArgCount = ArgTypeArray.GetCount ();

	if (m_pClassType)
		String.AppendFormat (_T("%s this"), ArgTypeArray [0]->GetTypeString ());

	if (!m_ArgList.IsEmpty ())
	{
		rtl::CIteratorT <CFunctionFormalArg> Arg = m_ArgList.GetHead ();

		String.AppendFormat (
			m_pClassType ? _T(", %s %s") : _T("%s %s"), 
			Arg->GetType ()->GetTypeString (), 
			Arg->GetName ()
			);

		for (Arg++; Arg; Arg++)
			String.AppendFormat (_T(", %s %s"), Arg->GetType ()->GetTypeString (), Arg->GetName ());
	}
	else
	{
		if (ArgCount && !m_pClassType)
			String += ArgTypeArray [0]->GetTypeString ();

		for (size_t i = 1; i < ArgCount; i++)
			String.AppendFormat (_T(", %s"), ArgTypeArray [i]->GetTypeString ());
	}

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

void
CFunction::ConvertToMethodMember (
	CClassType* pClassType,
	int ThisArgTypeFlags
	)
{
	ASSERT (!m_pClassType);
	ASSERT (m_TypeOverload.GetOverloadCount () == 1);

	m_pClassType = pClassType;
	m_pType = pClassType->GetMethodMemberType (m_pType, ThisArgTypeFlags);
	m_TypeOverload = m_pType;

	ASSERT (!m_pType->GetArgTypeArray ().IsEmpty ());
	m_pThisArgType = (CClassPtrType*) m_pType->GetArgTypeArray () [0];
	m_pThisType = m_pThisArgType;
}

bool
CFunction::AddOverload (CFunction* pFunction)
{
	bool Result = m_TypeOverload.AddOverload (pFunction->m_pType);
	if (!Result)
		return false;

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
