#include "pch.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

//.............................................................................

const char*
GetFunctionKindString (EFunction FunctionKind)
{
	static const char* StringTable [EFunction__Count] = 
	{
		"undefined-function-kind",  // EFunction_Undefined,
		"named-function",           // EFunction_Named,
		"get",                      // EFunction_Getter,
		"set",                      // EFunction_Setter,
		"preconstruct",             // EFunction_PreConstructor,
		"this",                     // EFunction_Constructor,
		"~this",                    // EFunction_Destructor,
		"static this",              // EFunction_StaticConstructor,
		"static ~this",             // EFunction_StaticDestructor,
		"module this",              // EFunction_ModuleConstructor,
		"module ~this",             // EFunction_ModuleDestructor,
		"call-operator",            // EFunction_CallOperator,
		"cast-operator",            // EFunction_CastOperator,
		"unary-operator",           // EFunction_UnaryOperator,
		"binary-operator",          // EFunction_BinaryOperator,
		"autoev-starter",           // EFunction_AutoEvStarter,
		"autoev-stopper",           // EFunction_AutoEvStopper,
		"autoev-handler",           // EFunction_AutoEvHandler,
		"internal",                 // EFunction_Internal, 
		"thunk",                    // EFunction_Thunk,
		"schedule-launcher",        // EFunction_ScheduleLauncher,
	};

	return (size_t) FunctionKind < EFunction__Count ? 
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
		EFunctionKindFlag_NoOverloads,  // EFunction_Getter,
		0,                              // EFunction_Setter,
		EFunctionKindFlag_NoStorage |   // EFunction_PreConstructor,
		EFunctionKindFlag_NoOverloads |
		EFunctionKindFlag_NoArgs,       
		EFunctionKindFlag_NoStorage,    // EFunction_Constructor,
		EFunctionKindFlag_NoStorage |   // EFunction_Destructor,
		EFunctionKindFlag_NoOverloads |
		EFunctionKindFlag_NoArgs,              
		EFunctionKindFlag_NoStorage |   // EFunction_StaticConstructor,
		EFunctionKindFlag_NoOverloads |
		EFunctionKindFlag_NoArgs,              
		EFunctionKindFlag_NoStorage |   // EFunction_StaticDestructor,
		EFunctionKindFlag_NoOverloads |
		EFunctionKindFlag_NoArgs,              
		0,                              // EFunction_CallOperator,
		EFunctionKindFlag_NoOverloads | // EFunction_CastOperator,
		EFunctionKindFlag_NoArgs,       
		EFunctionKindFlag_NoOverloads | // EFunction_UnaryOperator,
		EFunctionKindFlag_NoArgs,       
		0,                              // EFunction_BinaryOperator,
		0,                              // EFunction_AutoEvStarter,
		0,                              // EFunction_AutoEvStopper,
		0,                              // EFunction_AutoEvHandler,
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
	m_pOrphanNamespace = NULL;
	m_pType = NULL;
	m_pExternFunction = NULL;
	m_pCastOpType = NULL;
	m_pThisArgType = NULL;
	m_pThisType = NULL;
	m_ThisArgDelta = 0;
	m_ThisArgTypeFlags = 0;
	m_pVirtualOriginClassType = NULL;
	m_pProperty = NULL;
	m_pAutoEv = NULL;
	m_ClassVTableIndex = -1;
	m_PropertyVTableIndex = -1;
	m_pBlock = NULL;
	m_pScope = NULL;
	m_pLlvmFunction = NULL;
	m_pf = NULL;
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
		(const char*) m_Tag, 
		m_pModule->m_pLlvmModule
		);

	return m_pLlvmFunction;
}

void
CFunction::ConvertToMemberMethod (CNamedType* pParentType)
{
	ASSERT (m_TypeOverload.GetOverloadCount () == 1);

	m_pParentNamespace = pParentType;
	m_pType = pParentType->GetMemberMethodType (m_pType, m_ThisArgTypeFlags);
	m_TypeOverload = m_pType;

	ASSERT (!m_pType->GetArgArray ().IsEmpty ());
	m_pThisArgType = m_pType->GetArgArray () [0]->GetType ();
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
	CModuleItem* pItem = m_pParentNamespace->FindItemTraverse (m_DeclaratorName);
	if (!pItem)
	{
		err::SetFormatStringError ("unresolved orphan function '%s'", m_Tag.cc ()); // thanks a lot gcc
		return false;
	}

	CFunction* pOriginFunction = NULL;

	EModuleItem ItemKind = pItem->GetItemKind ();
	if (m_FunctionKind == EFunction_Named)
	{
		if (ItemKind != EModuleItem_Function)
		{
			err::SetFormatStringError ("'%s' is not a function", m_Tag.cc ()); 
			return false;
		}

		pOriginFunction = (CFunction*) pItem;
	}
	else
	{
		pOriginFunction = GetItemUnnamedMethod (pItem, m_FunctionKind);
		if (!pOriginFunction)
		{
			err::SetFormatStringError ("'%s' has no %d", m_Tag.cc (), GetFunctionKindString (m_FunctionKind));
			return false;
		}
	}

	pOriginFunction = pOriginFunction->FindShortOverload (m_pType);
	if (!pOriginFunction)
	{
		err::SetFormatStringError ("'%s': overload not found", m_Tag.cc ()); 
		return false;
	}

	if (pOriginFunction->HasBody () || pOriginFunction->m_pExternFunction)
	{
		err::SetFormatStringError ("'%s' already has a body", m_Tag.cc ()); 
		return false;
	}

	if (pOriginFunction->m_StorageKind == EStorage_Abstract)
	{
		err::SetFormatStringError ("'%s' is abstract and hence cannot have a body", m_Tag.cc ());
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
		err::SetFormatStringError ("storage specifier mismatch for orphan function '%s'", m_Tag.cc ()); 
		return false;
	}

	MakeStub (pOriginFunction);
	return true;
}

void
CFunction::MakeStub (CFunction* pFunction)
{
	m_AccessKind = pFunction->m_AccessKind;
	m_pParentNamespace = pFunction->m_pParentNamespace;
	m_pType = pFunction->m_pType;
	m_Name = pFunction->m_Name;
	m_QualifiedName = pFunction->m_QualifiedName;
	m_Tag = pFunction->m_Tag;
	m_pThisArgType = pFunction->m_pThisArgType;
	m_pThisType = pFunction->m_pThisType;
	m_pVirtualOriginClassType = pFunction->m_pVirtualOriginClassType;
	m_pProperty = pFunction->m_pProperty;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
