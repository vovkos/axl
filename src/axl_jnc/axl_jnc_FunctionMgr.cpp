#include "pch.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_GcShadowStack.h"
#include "axl_jnc_Module.h"

// #define _AXL_JNC_NO_JIT
// #define _AXL_JNC_NO_PROLOGUE_GC_SAFE_POINT

namespace axl {
namespace jnc {

//.............................................................................

CFunctionMgr::CFunctionMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_pCurrentFunction = NULL;
	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));

	mt::CallOnce (RegisterGcShadowStack, 0);
}

void
CFunctionMgr::Clear ()
{
	m_LlvmFunctionMap.Clear ();
	m_FunctionList.Clear ();
	m_PropertyList.Clear ();
	m_PropertyTemplateList.Clear ();
	m_ScheduleLauncherFunctionList.Clear ();
	m_ThunkFunctionList.Clear ();
	m_ThunkPropertyList.Clear ();
	m_DataThunkPropertyList.Clear ();
	m_ThunkFunctionMap.Clear ();
	m_ThunkPropertyMap.Clear ();
	m_ScheduleLauncherFunctionMap.Clear ();
	m_EmissionContextStack.Clear ();
	m_ThisValue.Clear ();
	m_ScopeLevelValue.Clear ();
	m_VTablePtrPtrValue.Clear ();
	m_VTablePtrValue.Clear ();

	m_pCurrentFunction = NULL;
	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));
}

CFunction*
CFunctionMgr::CreateFunction (
	EFunction FunctionKind,
	CFunctionType* pType,
	uint_t Flags
	)
{
	CFunction* pFunction;
	switch (FunctionKind)
	{
	case EFunction_Thunk:
		pFunction = AXL_MEM_NEW (CThunkFunction);
		m_ThunkFunctionList.InsertTail ((CThunkFunction*) pFunction);
		break;

	case EFunction_ScheduleLauncher:
		pFunction = AXL_MEM_NEW (CScheduleLauncherFunction);
		m_ScheduleLauncherFunctionList.InsertTail ((CScheduleLauncherFunction*) pFunction);
		break;

	default:
		pFunction = AXL_MEM_NEW (CFunction);
		m_FunctionList.InsertTail (pFunction);
	}

	pFunction->m_pModule = m_pModule;
	pFunction->m_FunctionKind = FunctionKind;
	pFunction->m_pType = pType;
	pFunction->m_Flags = Flags;
	pFunction->m_TypeOverload.AddOverload (pType);
	return pFunction;
}

CFunction*
CFunctionMgr::CreateFunction (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CFunctionType* pType,
	uint_t Flags
	)
{
	CFunction* pFunction = CreateFunction (EFunction_Named, pType, Flags);
	pFunction->m_Name = Name;
	pFunction->m_QualifiedName = QualifiedName;
	pFunction->m_Tag = QualifiedName;

	return pFunction;
}

CProperty*
CFunctionMgr::CreateProperty (
	EProperty PropertyKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	uint_t Flags
	)
{
	CProperty* pProperty;

	switch (PropertyKind)
	{
	case EProperty_Thunk:
		pProperty = AXL_MEM_NEW (CThunkProperty);
		m_ThunkPropertyList.InsertTail ((CThunkProperty*) pProperty);
		break;

	case EProperty_DataThunk:
		pProperty = AXL_MEM_NEW (CDataThunkProperty);
		m_DataThunkPropertyList.InsertTail ((CDataThunkProperty*) pProperty);
		break;

	default:
		pProperty = AXL_MEM_NEW (CProperty);
		m_PropertyList.InsertTail (pProperty);
	}

	pProperty->m_pModule = m_pModule;
	pProperty->m_Name = Name;
	pProperty->m_QualifiedName = QualifiedName;
	pProperty->m_Tag = QualifiedName;
	pProperty->m_Flags = Flags;
	m_pModule->MarkForLayout (pProperty, true);
	return pProperty;
}

CProperty*
CFunctionMgr::CreateInternalProperty (const char* pTag)
{
	CProperty* pProperty = CreateProperty (rtl::CString (), rtl::CString ());
	pProperty->m_Tag = pTag;
	return pProperty;
}

CPropertyTemplate*
CFunctionMgr::CreatePropertyTemplate ()
{
	CPropertyTemplate* pPropertyTemplate = AXL_MEM_NEW (CPropertyTemplate);
	pPropertyTemplate->m_pModule = m_pModule;
	m_PropertyTemplateList.InsertTail (pPropertyTemplate);
	return pPropertyTemplate;
}

#pragma AXL_TODO ("get rid of emission context stack: postpone compilation of nested function")

void
CFunctionMgr::PushEmissionContext ()
{
	if (!m_pCurrentFunction)
		return;

	TEmissionContext* pContext = AXL_MEM_NEW (TEmissionContext);
	pContext->m_pCurrentFunction = m_pCurrentFunction;
	pContext->m_ThisValue = m_ThisValue;
	pContext->m_ScopeLevelValue = m_ScopeLevelValue;
	pContext->m_VTablePtrPtrValue = m_VTablePtrPtrValue;
	pContext->m_VTablePtrValue = m_VTablePtrValue;

	pContext->m_pCurrentNamespace = m_pModule->m_NamespaceMgr.m_pCurrentNamespace;
	pContext->m_pCurrentScope = m_pModule->m_NamespaceMgr.m_pCurrentScope;

	pContext->m_pCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	pContext->m_pReturnBlock = m_pModule->m_ControlFlowMgr.m_pReturnBlock;
	pContext->m_pSilentReturnBlock = m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock;
	pContext->m_pUnreachableBlock = m_pModule->m_ControlFlowMgr.m_pUnreachableBlock;
	pContext->m_ControlFlowMgrFlags = m_pModule->m_ControlFlowMgr.m_Flags;
	pContext->m_LlvmDebugLoc = m_pModule->m_LlvmIrBuilder.GetCurrentDebugLoc ();

	m_EmissionContextStack.InsertTail (pContext);

	m_pModule->m_NamespaceMgr.m_pCurrentNamespace = &m_pModule->m_NamespaceMgr.m_GlobalNamespace;
	m_pModule->m_NamespaceMgr.m_pCurrentScope = NULL;

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (NULL);
	m_pModule->m_ControlFlowMgr.m_pReturnBlock = NULL;
	m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock = NULL;
	m_pModule->m_ControlFlowMgr.m_pUnreachableBlock = NULL;
	m_pModule->m_ControlFlowMgr.m_Flags = 0;
	m_pModule->m_LlvmIrBuilder.ClearCurrentDebugLoc ();

	m_pModule->m_VariableMgr.DeallocateTlsVariableArray (m_pCurrentFunction->m_TlsVariableArray);

	m_pCurrentFunction = NULL;
	m_ThisValue.Clear ();
	m_ScopeLevelValue.Clear ();
	m_VTablePtrPtrValue.Clear ();
	m_VTablePtrValue.Clear ();
}

void
CFunctionMgr::PopEmissionContext ()
{
	ASSERT (m_pCurrentFunction);
	m_pModule->m_VariableMgr.DeallocateTlsVariableArray (m_pCurrentFunction->m_TlsVariableArray);

	if (m_EmissionContextStack.IsEmpty ())
	{
		m_pCurrentFunction = NULL;
		m_ThisValue.Clear ();
		m_ScopeLevelValue.Clear ();
		m_VTablePtrPtrValue.Clear ();
		m_VTablePtrValue.Clear ();
		return;
	}

	TEmissionContext* pContext = m_EmissionContextStack.RemoveTail ();
	m_pCurrentFunction = pContext->m_pCurrentFunction;
	m_ThisValue = pContext->m_ThisValue;
	m_ScopeLevelValue = pContext->m_ScopeLevelValue;
	m_VTablePtrPtrValue = pContext->m_VTablePtrPtrValue;
	m_VTablePtrValue = pContext->m_VTablePtrValue;

	m_pModule->m_NamespaceMgr.m_pCurrentNamespace = pContext->m_pCurrentNamespace;
	m_pModule->m_NamespaceMgr.m_pCurrentScope = pContext->m_pCurrentScope;

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pContext->m_pCurrentBlock);
	m_pModule->m_ControlFlowMgr.m_pReturnBlock = pContext->m_pReturnBlock;
	m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock = pContext->m_pSilentReturnBlock;
	m_pModule->m_ControlFlowMgr.m_pUnreachableBlock = pContext->m_pUnreachableBlock;
	m_pModule->m_ControlFlowMgr.m_Flags = pContext->m_ControlFlowMgrFlags;
	m_pModule->m_LlvmIrBuilder.SetCurrentDebugLoc (pContext->m_LlvmDebugLoc);

	AXL_MEM_DELETE (pContext);

	m_pModule->m_VariableMgr.RestoreTlsVariableArray (m_pCurrentFunction->m_TlsVariableArray);
}

void
CFunctionMgr::CutVTable ()
{
	CFunction* pFunction = m_pCurrentFunction;
	ASSERT (pFunction && pFunction->IsMember ());

	CValue ThisArgValue = pFunction->GetType ()->GetCallConv ()->GetThisArgValue (pFunction);
	ASSERT (ThisArgValue.GetType ()->GetTypeKind () == EType_ClassPtr);

	CClassType* pClassType = ((CClassPtrType*) ThisArgValue.GetType ())->GetTargetType ();

	CLlvmScopeComment Comment (&m_pModule->m_LlvmIrBuilder, "cut vtable ptr");

	int32_t LlvmIndexArray [] =
	{
		0, // TInterface**
		0, // TInterface*
		0, // vtable**
	};

	m_pModule->m_LlvmIrBuilder.CreateGep (ThisArgValue, LlvmIndexArray, countof (LlvmIndexArray), NULL, &m_VTablePtrPtrValue);
	m_pModule->m_LlvmIrBuilder.CreateLoad (m_VTablePtrPtrValue, NULL, &m_VTablePtrValue);
	m_pModule->m_LlvmIrBuilder.CreateStore (pClassType->GetVTablePtrValue (), m_VTablePtrPtrValue);
}

void
CFunctionMgr::RestoreVTable ()
{
	CLlvmScopeComment Comment (&m_pModule->m_LlvmIrBuilder, "restore vtable ptr");
	m_pModule->m_LlvmIrBuilder.CreateStore (m_VTablePtrValue, m_VTablePtrPtrValue);
}

bool
CFunctionMgr::FireOnChangeEvent ()
{
	CFunction* pFunction = m_pCurrentFunction;

	ASSERT (
		pFunction->m_FunctionKind == EFunction_Setter &&
		pFunction->m_pProperty &&
		pFunction->m_pProperty->GetType ()->GetFlags () & EPropertyTypeFlag_Bindable
		);

	CValue PropertyValue = pFunction->m_pProperty;

	if (pFunction->m_pThisType)
	{
		ASSERT (m_ThisValue);
		PropertyValue.InsertToClosureHead (m_ThisValue);
	}

	CValue OnChange;

	return
		m_pModule->m_OperatorMgr.GetPropertyOnChange (PropertyValue, &OnChange) &&
		m_pModule->m_OperatorMgr.MemberOperator (&OnChange, "call") &&
		m_pModule->m_OperatorMgr.CallOperator (OnChange);
}

bool
CFunctionMgr::Prologue (
	CFunction* pFunction,
	const CToken::CPos& Pos
	)
{
	bool Result;

	PushEmissionContext ();

	m_pCurrentFunction = pFunction;

	// create scope

	m_pModule->m_NamespaceMgr.OpenNamespace (pFunction->m_pParentNamespace);

	pFunction->m_pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos);

	// create entry block (gc roots come here)

	CBasicBlock* pEntryBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("function_entry");
	CBasicBlock* pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("function_body");

	pFunction->m_pEntryBlock = pEntryBlock;
	pEntryBlock->MarkEntry ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pEntryBlock);
	m_pModule->m_LlvmIrBuilder.ClearCurrentDebugLoc (); // no line info in entry block

#ifndef _AXL_JNC_NO_PROLOGUE_GC_SAFE_POINT
	CFunction* pGcSafePoint = GetStdFunction (EStdFunc_GcSafePoint);
	m_pModule->m_LlvmIrBuilder.CreateCall (pGcSafePoint, pGcSafePoint->GetType (), NULL);
#endif

	if (pFunction->m_FunctionKind == EFunction_ModuleConstructor)
	{
		bool Result = m_pModule->m_VariableMgr.AllocatePrimeStaticVariables ();
		if (!Result)
			return false;
	}

	m_pModule->m_ControlFlowMgr.Jump (pBodyBlock, pBodyBlock);
	m_pModule->m_ControlFlowMgr.m_pUnreachableBlock = NULL;
	m_pModule->m_ControlFlowMgr.m_Flags = 0; // clear jump flag

	CLlvmScopeComment Comment (&m_pModule->m_LlvmIrBuilder, "prologue");

	// save scope level

	if (pFunction->m_FunctionKind == EFunction_ModuleConstructor)
	{
		Result = m_pModule->m_VariableMgr.InitializeGlobalStaticVariables ();
		if (!Result)
			return false;
	}
	else // do not save / restore scope level in module constructor
	{
		CVariable* pVariable = m_pModule->m_VariableMgr.GetStdVariable (EStdVariable_ScopeLevel);
		m_pModule->m_LlvmIrBuilder.CreateLoad (pVariable, NULL, &m_ScopeLevelValue);
	}

	pFunction->GetType ()->GetCallConv ()->CreateArgVariables (pFunction);

	// 'this' arg

	if (pFunction->IsMember ())
	{
		CreateThisValue ();

		// cut vtable

		if (pFunction->NeedsVTablePtrCut ())
		{
			CutVTable ();
			m_pModule->m_ControlFlowMgr.m_pReturnBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("return_block");
		}
	}

	// return blocks for bindable setter

	if (pFunction->m_FunctionKind == EFunction_Setter && (pFunction->m_pProperty->GetType ()->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		m_pModule->m_ControlFlowMgr.m_pReturnBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("return_block");
		m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("silent_return_block");
	}

	return true;
}

void
CFunctionMgr::CreateThisValue ()
{
	CFunction* pFunction = m_pCurrentFunction;
	ASSERT (pFunction && pFunction->IsMember ());

	CValue ThisArgValue = pFunction->GetType ()->GetCallConv ()->GetThisArgValue (pFunction);

	if (pFunction->m_pThisArgType->Cmp (pFunction->m_pThisType) == 0)
	{
		if (pFunction->m_pThisType->GetTypeKind () != EType_DataPtr)
		{
			m_ThisValue = ThisArgValue;
		}
		else // make it thin
		{
			ASSERT (
				ThisArgValue.GetType ()->GetTypeKind () == EType_DataPtr &&
				((CDataPtrType*) ThisArgValue.GetType ())->GetPtrTypeKind () == EDataPtrType_Normal);

			CDataPtrType* pPtrType = ((CDataPtrType*) ThisArgValue.GetType ());

			CValue PtrValue;
			m_pModule->m_LlvmIrBuilder.CreateExtractValue (ThisArgValue, 0, NULL, &PtrValue);

			pPtrType = pPtrType->GetTargetType ()->GetDataPtrType (EDataPtrType_Thin, pPtrType->GetFlags ());
			m_ThisValue.SetThinDataPtr (PtrValue.GetLlvmValue (), pPtrType, ThisArgValue);
		}
	}
	else
	{
		ASSERT (pFunction->m_StorageKind == EStorage_Override && pFunction->m_ThisArgDelta < 0);

		CValue PtrValue;
		m_pModule->m_LlvmIrBuilder.CreateBitCast (ThisArgValue, m_pModule->GetSimpleType (EStdType_BytePtr), &PtrValue);
		m_pModule->m_LlvmIrBuilder.CreateGep (PtrValue, (int32_t) pFunction->m_ThisArgDelta, NULL, &PtrValue);
		m_pModule->m_LlvmIrBuilder.CreateBitCast (PtrValue, pFunction->m_pThisType, &m_ThisValue);
	}
}

bool
CFunctionMgr::Epilogue ()
{
	bool Result;

	CFunction* pFunction = m_pCurrentFunction;
	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();

	ASSERT (m_pCurrentFunction && pScope);

	if (m_pModule->m_ControlFlowMgr.m_pReturnBlock)
	{
		m_pModule->m_ControlFlowMgr.Follow (m_pModule->m_ControlFlowMgr.m_pReturnBlock);
		m_pModule->m_ControlFlowMgr.m_pReturnBlock = NULL;
	}

	if (pFunction->NeedsVTablePtrCut ())
		RestoreVTable ();

	if (pFunction->m_FunctionKind == EFunction_Setter &&
		(pFunction->m_pProperty->GetType ()->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		Result = FireOnChangeEvent ();
		if (!Result)
			return false;

		ASSERT (m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock);

		if (m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock->GetFlags () & EBasicBlockFlag_Jumped)
			m_pModule->m_ControlFlowMgr.Follow (m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock);

		m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock = NULL;
	}

	if (pFunction->m_FunctionKind == EFunction_Destructor)
	{
		ASSERT (pFunction->GetParentType ()->GetTypeKind () == EType_Class && m_ThisValue);

		CClassType* pClassType = (CClassType*) pFunction->GetParentType ();

		Result =
			pClassType->CallMemberPropertyDestructors (m_ThisValue) &&
			pClassType->CallMemberFieldDestructors (m_ThisValue) &&
			pClassType->CallBaseTypeDestructors (m_ThisValue);

		if (!Result)
			return false;
	}

	if (pFunction->m_FunctionKind == EFunction_ModuleDestructor)
		m_pModule->m_VariableMgr.m_StaticDestructList.RunDestructors ();

	Result = m_pModule->m_ControlFlowMgr.CheckReturn ();
	if (!Result)
		return false;

	try
	{
		llvm::verifyFunction (*pFunction->GetLlvmFunction (), llvm::ReturnStatusAction);
	}
	catch (err::CError Error)
	{
		err::SetFormatStringError (
			"LLVM verification fail for '%s': %s",
			pFunction->m_Tag.cc (),
			Error->GetDescription ().cc ()
			);

		return false;
	}

	m_pModule->m_NamespaceMgr.CloseScope ();
	m_pModule->m_NamespaceMgr.CloseNamespace ();

	PopEmissionContext ();
	return true;
}

void
CFunctionMgr::InternalPrologue (
	CFunction* pFunction,
	CValue* pArgValueArray,
	size_t ArgCount
	)
{
	PushEmissionContext ();

	m_pCurrentFunction = pFunction;

	m_pModule->m_NamespaceMgr.OpenInternalScope ();

	CBasicBlock* pEntryBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("function_entry");
	CBasicBlock* pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("function_body");

	pFunction->m_pEntryBlock = pEntryBlock;
	pEntryBlock->MarkEntry ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pEntryBlock);
	m_pModule->m_LlvmIrBuilder.ClearCurrentDebugLoc (); // no line info in entry block

#ifndef _AXL_JNC_NO_PROLOGUE_GC_SAFE_POINT
	CFunction* pGcSafePoint = GetStdFunction (EStdFunc_GcSafePoint);
	m_pModule->m_LlvmIrBuilder.CreateCall (pGcSafePoint, pGcSafePoint->GetType (), NULL);
#endif

	m_pModule->m_ControlFlowMgr.Jump (pBodyBlock, pBodyBlock);
	m_pModule->m_ControlFlowMgr.m_pUnreachableBlock = NULL;
	m_pModule->m_ControlFlowMgr.m_Flags = 0;

	if (pFunction->m_FunctionKind != EFunction_ModuleConstructor) // do not save / restore scope level in module constructor
	{
		CVariable* pVariable = m_pModule->m_VariableMgr.GetStdVariable (EStdVariable_ScopeLevel);
		m_pModule->m_LlvmIrBuilder.CreateLoad (pVariable, NULL, &m_ScopeLevelValue);
	}

	if (pFunction->IsMember ())
		CreateThisValue ();

	if (ArgCount)
	{
		llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin ();
		rtl::CArrayT <CFunctionArg*> ArgArray = pFunction->GetType ()->GetArgArray ();

		for (size_t i = 0; i < ArgCount; i++, LlvmArg++)
			pArgValueArray [i] = CValue (LlvmArg, ArgArray [i]->GetType ());
	}
}

void
CFunctionMgr::InternalEpilogue ()
{
	CFunction* pFunction = m_pCurrentFunction;

	CBasicBlock* pCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	if (!pCurrentBlock->HasTerminator ())
	{
		CType* pReturnType = pFunction->GetType ()->GetReturnType ();

		CValue ReturnValue;
		if (pReturnType->GetTypeKind () != EType_Void)
			ReturnValue = pReturnType->GetZeroValue ();

		m_pModule->m_ControlFlowMgr.Return (ReturnValue);
	}

	m_pModule->m_NamespaceMgr.CloseScope ();

	PopEmissionContext ();
}

CFunction*
CFunctionMgr::GetDirectThunkFunction (
	CFunction* pTargetFunction,
	CFunctionType* pThunkFunctionType,
	bool HasUnusedClosure
	)
{
	if (!HasUnusedClosure && pTargetFunction->m_pType->Cmp (pThunkFunctionType) == 0)
		return pTargetFunction;

	char SignatureChar = 'D';

	if (HasUnusedClosure)
	{
		SignatureChar = 'U';
		pThunkFunctionType = pThunkFunctionType->GetStdObjectMemberMethodType ();
	}

	rtl::CString Signature;
	Signature.Format (
		"%c%x.%s",
		SignatureChar,
		pTargetFunction,
		pThunkFunctionType->GetSignature ().cc ()
		);

	rtl::CStringHashTableMapIteratorT <CFunction*> Thunk = m_ThunkFunctionMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;

	CThunkFunction* pThunkFunction = (CThunkFunction*) CreateFunction (EFunction_Thunk, pThunkFunctionType);
	pThunkFunction->m_StorageKind = EStorage_Static;
	pThunkFunction->m_Tag = "directThunkFunction";
	pThunkFunction->m_Signature = Signature;
	pThunkFunction->m_pTargetFunction = pTargetFunction;

	Thunk->m_Value = pThunkFunction;

	m_pModule->MarkForCompile (pThunkFunction);
	return pThunkFunction;
}

CProperty*
CFunctionMgr::GetDirectThunkProperty (
	CProperty* pTargetProperty,
	CPropertyType* pThunkPropertyType,
	bool HasUnusedClosure
	)
{
	if (!HasUnusedClosure && pTargetProperty->m_pType->Cmp (pThunkPropertyType) == 0)
		return pTargetProperty;

	rtl::CString Signature;
	Signature.Format (
		"%c%x.%s",
		HasUnusedClosure ? 'U' : 'D',
		pTargetProperty,
		pThunkPropertyType->GetSignature ().cc ()
		);

	rtl::CStringHashTableMapIteratorT <CProperty*> Thunk = m_ThunkPropertyMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;

	CThunkProperty* pThunkProperty = (CThunkProperty*) CreateProperty (EProperty_Thunk, rtl::CString (), rtl::CString ());
	pThunkProperty->m_StorageKind = EStorage_Static;
	pThunkProperty->m_Signature = Signature;
	pThunkProperty->m_Tag = "g_directThunkProperty";

	bool Result = pThunkProperty->Create (pTargetProperty, pThunkPropertyType, HasUnusedClosure);
	if (!Result)
		return NULL;

	Thunk->m_Value = pThunkProperty;

	pThunkProperty->EnsureLayout ();
	return pThunkProperty;
}

CProperty*
CFunctionMgr::GetDirectDataThunkProperty (
	CVariable* pTargetVariable,
	CPropertyType* pThunkPropertyType,
	bool HasUnusedClosure
	)
{
	bool Result;

	rtl::CString Signature;
	Signature.Format (
		"%c%x.%s",
		HasUnusedClosure ? 'U' : 'D',
		pTargetVariable,
		pThunkPropertyType->GetSignature ().cc ()
		);

	rtl::CStringHashTableMapIteratorT <CProperty*> Thunk = m_ThunkPropertyMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;

	CDataThunkProperty* pThunkProperty = (CDataThunkProperty*) CreateProperty (EProperty_DataThunk, rtl::CString (), rtl::CString ());
	pThunkProperty->m_StorageKind = EStorage_Static;
	pThunkProperty->m_Signature = Signature;
	pThunkProperty->m_pTargetVariable = pTargetVariable;
	pThunkProperty->m_Tag = "g_directDataThunkProperty";

	if (HasUnusedClosure)
		pThunkPropertyType = pThunkPropertyType->GetStdObjectMemberPropertyType ();

	Result = pThunkProperty->Create (pThunkPropertyType);
	if (!Result)
		return NULL;

	Thunk->m_Value = pThunkProperty;

	pThunkProperty->EnsureLayout ();
	m_pModule->MarkForCompile (pThunkProperty);
	return pThunkProperty;
}

CFunction*
CFunctionMgr::GetScheduleLauncherFunction (
	CFunctionPtrType* pTargetFunctionPtrType,
	EClassPtrType SchedulerPtrTypeKind
	)
{
	rtl::CString Signature = pTargetFunctionPtrType->GetSignature ();
	if (SchedulerPtrTypeKind == EClassPtrType_Weak)
		Signature += ".w";

	rtl::CStringHashTableMapIteratorT <CFunction*> Thunk = m_ScheduleLauncherFunctionMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;

	CClassPtrType* pSchedulerPtrType = ((CClassType*) GetSimpleType (m_pModule, EStdType_Scheduler))->GetClassPtrType (SchedulerPtrTypeKind);

	rtl::CArrayT <CFunctionArg*> ArgArray  = pTargetFunctionPtrType->GetTargetType ()->GetArgArray ();
	ArgArray.Insert (0, pTargetFunctionPtrType->GetSimpleFunctionArg ());
	ArgArray.Insert (1, pSchedulerPtrType->GetSimpleFunctionArg ());

	CFunctionType* pLauncherType = m_pModule->m_TypeMgr.GetFunctionType (ArgArray);
	CScheduleLauncherFunction* pLauncherFunction = (CScheduleLauncherFunction*) CreateFunction (EFunction_ScheduleLauncher, pLauncherType);
	pLauncherFunction->m_StorageKind = EStorage_Static;
	pLauncherFunction->m_Tag = "scheduleLauncherFunction";
	pLauncherFunction->m_Signature = Signature;

	Thunk->m_Value = pLauncherFunction;

	m_pModule->MarkForCompile (pLauncherFunction);
	return pLauncherFunction;
}

bool
CFunctionMgr::InjectTlsPrologues ()
{
	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;
		if (pFunction->GetEntryBlock () && !pFunction->GetTlsVariableArray ().IsEmpty ())
			InjectTlsPrologue (pFunction);
	}

	rtl::CIteratorT <CThunkFunction> ThunkFunction = m_ThunkFunctionList.GetHead ();
	for (; ThunkFunction; ThunkFunction++)
	{
		CFunction* pFunction = *ThunkFunction;
		if (!pFunction->GetTlsVariableArray ().IsEmpty ())
			InjectTlsPrologue (pFunction);
	}

	rtl::CIteratorT <CScheduleLauncherFunction> ScheduleLauncherFunction = m_ScheduleLauncherFunctionList.GetHead ();
	for (; ScheduleLauncherFunction; ScheduleLauncherFunction++)
	{
		CFunction* pFunction = *ScheduleLauncherFunction;
		if (!pFunction->GetTlsVariableArray ().IsEmpty ())
			InjectTlsPrologue (pFunction);
	}

	return true;
}

void
CFunctionMgr::InjectTlsPrologue (CFunction* pFunction)
{
	CBasicBlock* pBlock = pFunction->GetEntryBlock ();
	ASSERT (pBlock);

	rtl::CArrayT <TTlsVariable> TlsVariableArray = pFunction->GetTlsVariableArray ();
	ASSERT (!TlsVariableArray.IsEmpty ());

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pBlock);

	llvm::BasicBlock::iterator LlvmAnchor = pBlock->GetLlvmBlock ()->begin ();

#ifndef _AXL_JNC_NO_PROLOGUE_GC_SAFE_POINT
	if (llvm::isa <llvm::CallInst> (LlvmAnchor)) // skip gc-safe-point
		LlvmAnchor++;
#endif

	m_pModule->m_LlvmIrBuilder.SetInsertPoint (LlvmAnchor);

	CFunction* pGetTls = GetStdFunction (EStdFunc_GetTls);
	CValue TlsValue;
	LlvmAnchor = m_pModule->m_LlvmIrBuilder.CreateCall (pGetTls, pGetTls->GetType (), &TlsValue);

	// tls variables used in this function

	size_t Count = TlsVariableArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = TlsVariableArray [i].m_pVariable->GetTlsField ();
		ASSERT (pField);

		CValue PtrValue;
		m_pModule->m_LlvmIrBuilder.CreateGep2 (TlsValue, pField->GetLlvmIndex (), NULL, &PtrValue);
		TlsVariableArray [i].m_pLlvmAlloca->replaceAllUsesWith (PtrValue.GetLlvmValue ());
	}

	// unfortunately, erasing could not be safely done inside the above loop (cause of InsertPoint)
	// so just have a dedicated loop for erasing alloca's

	Count = TlsVariableArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
		TlsVariableArray [i].m_pLlvmAlloca->eraseFromParent ();

	// skip all the gep's to get past tls prologue

	LlvmAnchor++;
	while (llvm::isa <llvm::GetElementPtrInst> (LlvmAnchor))
		LlvmAnchor++;

	pFunction->m_pLlvmPostTlsPrologueInst = LlvmAnchor;
}

class CJitEventListener: public llvm::JITEventListener
{
protected:
	CFunctionMgr* m_pFunctionMgr;

public:
	CJitEventListener (CFunctionMgr* pFunctionMgr)
	{
		m_pFunctionMgr = pFunctionMgr;
	}

	virtual
	void
	NotifyObjectEmitted (const llvm::ObjectImage& LLvmObjectImage)
	{
		// printf ("NotifyObjectEmitted\n");
	}

	virtual
	void
	NotifyFunctionEmitted (
		const llvm::Function& LlvmFunction,
		void* p,
		size_t Size,
		const EmittedFunctionDetails& Details
		)
	{
		CFunction* pFunction = m_pFunctionMgr->FindFunctionByLlvmFunction ((llvm::Function*) &LlvmFunction);
		if (pFunction)
		{
			pFunction->m_pfMachineCode = p;
			pFunction->m_MachineCodeSize = Size;
		}
	}
};

void
LlvmFatalErrorHandler (
	void* pContext,
	const std::string& ErrorString,
	bool ShouldGenerateCrashDump
	)
{
	throw err::CreateStringError (ErrorString.c_str ());
}

bool
CFunctionMgr::JitFunctions (llvm::ExecutionEngine* pExecutionEngine)
{
#ifdef _AXL_JNC_NO_JIT
	err::SetFormatStringError ("LLVM jitting is disabled");
	return false;
#endif

	CScopeThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	CJitEventListener JitEventListener (this);
	pExecutionEngine->RegisterJITEventListener (&JitEventListener);

	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;

		if (!pFunction->GetEntryBlock ())
			continue;

		try
		{
			void* pf = pExecutionEngine->getPointerToFunction (pFunction->GetLlvmFunction ());
			pFunction->m_pfMachineCode = pf;

			// ASSERT (pFunction->m_pfMachineCode == pf && pFunction->m_MachineCodeSize != 0);
		}
		catch (err::CError Error)
		{
			err::SetFormatStringError (
				"LLVM jitting fail for '%s': %s",
				pFunction->m_Tag.cc (),
				Error->GetDescription ().cc ()
				);

			pExecutionEngine->UnregisterJITEventListener (&JitEventListener);
			return false;
		}
	}

	// for MC jitter this should do all the job
	pExecutionEngine->finalizeObject ();

	pExecutionEngine->UnregisterJITEventListener (&JitEventListener);
	return true;
}

CFunction*
CFunctionMgr::GetStdFunction (EStdFunc Func)
{
	ASSERT ((size_t) Func < EStdFunc__Count);

	if (m_StdFunctionArray [Func])
		return m_StdFunctionArray [Func];

	CFunction* pFunction;

	switch (Func)
	{
	case EStdFunc_OnRuntimeError:
		pFunction = CreateOnRuntimeError ();
		break;

	case EStdFunc_CheckNullPtr:
		pFunction = CreateCheckNullPtr ();
		break;

	case EStdFunc_CheckScopeLevel:
		pFunction = CreateCheckScopeLevel ();
		break;

	case EStdFunc_CheckDataPtrRange:
		pFunction = CreateCheckDataPtrRange ();
		break;

	case EStdFunc_CheckClassPtrScopeLevel:
		pFunction = CreateCheckClassPtrScopeLevel ();
		break;

	case EStdFunc_DynamicCastClassPtr:
		pFunction = CreateDynamicCastClassPtr ();
		break;

	case EStdFunc_StrengthenClassPtr:
		pFunction = CreateStrengthenClassPtr ();
		break;

	case EStdFunc_GetDataPtrSpan:
		pFunction = CreateGetDataPtrSpan ();
		break;

	case EStdFunc_HeapAlloc:
		pFunction = CreateHeapAlloc ();
		break;

	case EStdFunc_HeapUAlloc:
		pFunction = CreateHeapUAlloc ();
		break;

	case EStdFunc_HeapUFree:
		pFunction = CreateHeapUFree ();
		break;

	case EStdFunc_HeapUFreeClassPtr:
		pFunction = CreateHeapUFreeClassPtr ();
		break;

	case EStdFunc_GcAddObject:
		pFunction = CreateGcAddObject ();
		break;

	case EStdFunc_MarkGcRoot:
		pFunction = CreateMarkGcRoot ();
		break;

	case EStdFunc_GetTls:
		pFunction = CreateGetTls ();
		break;

	case EStdFunc_GcSafePoint:
		pFunction = CreateGcSafePoint ();
		break;

	case EStdFunc_RunGc:
		pFunction = CreateRunGc ();
		break;

	case EStdFunc_RunGcWaitForDestructors:
		pFunction = CreateRunGcWaitForDestructors ();
		break;

	case EStdFunc_GetCurrentThreadId:
		pFunction = CreateGetCurrentThreadId ();
		break;

	case EStdFunc_CreateThread:
		pFunction = CreateCreateThread ();
		break;

	case EStdFunc_Sleep:
		pFunction = CreateSleep ();
		break;

	case EStdFunc_GetTimestamp:
		pFunction = CreateGetTimestamp ();
		break;

	case EStdFunc_StrLen:
		pFunction = CreateStrLen ();
		break;

	case EStdFunc_Rand:
		pFunction = CreateRand ();
		break;

	case EStdFunc_Printf:
		pFunction = CreatePrintf ();
		break;

	case EStdFunc_AppendFmtLiteral_a:
		pFunction = CreateAppendFmtLiteral_a ();
		break;

	case EStdFunc_AppendFmtLiteral_p:
		pFunction = CreateAppendFmtLiteral_p ();
		break;

	case EStdFunc_AppendFmtLiteral_i32:
		pFunction = CreateAppendFmtLiteral_i32 ();
		break;

	case EStdFunc_AppendFmtLiteral_ui32:
		pFunction = CreateAppendFmtLiteral_ui32 ();
		break;

	case EStdFunc_AppendFmtLiteral_i64:
		pFunction = CreateAppendFmtLiteral_i64 ();
		break;

	case EStdFunc_AppendFmtLiteral_ui64:
		pFunction = CreateAppendFmtLiteral_ui64 ();
		break;

	case EStdFunc_AppendFmtLiteral_f:
		pFunction = CreateAppendFmtLiteral_f ();
		break;

	case EStdFunc_SimpleMulticastCall:
		pFunction = ((CMulticastClassType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleMulticast))->GetMethod (EMulticastMethod_Call);
		break;

	default:
		ASSERT (false);
		pFunction = NULL;
	}

	m_StdFunctionArray [Func] = pFunction;
	return pFunction;
}

CFunction*
CFunctionMgr::CreateInternalFunction (
	const char* pTag,
	CFunctionType* pType,
	uint_t Flags
	)
{
	CFunction* pFunction = CreateFunction (EFunction_Internal, pType, Flags);
	pFunction->m_Tag = pTag;
	return pFunction;
}

// void
// jnc.OnRuntimeError (
//		int Error,
//		int8* pCodeAddr
//		);

CFunction*
CFunctionMgr::CreateOnRuntimeError ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.onRuntimeError", pType);
}

// void
// jnc.CheckNullPtr (
//		int8* p,
//		int Error
//		);

CFunction*
CFunctionMgr::CreateCheckNullPtr ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.checkNullPtr", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("iface_fail");
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("iface_success");

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmIrBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	m_pModule->m_LlvmIrBuilder.RuntimeError (ArgValue2);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);

	InternalEpilogue ();

	return pFunction;
}


// void
// jnc.CheckDataPtrScopeLevel (
//		size_t SrcScopeLevel
//		size_t DstScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCheckScopeLevel ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.checkScopeLevel", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_fail");
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_success");

	CValue CmpValue;
	m_pModule->m_LlvmIrBuilder.CreateGt_u (ArgValue1, ArgValue2, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_LlvmIrBuilder.RuntimeError (ERuntimeError_ScopeMismatch);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);

	InternalEpilogue ();

	return pFunction;
}

// void
// jnc.CheckDataPtrRange (
//		int8* p,
//		size_t Size,
//		int8* pRangeBegin,
//		int8* pRangeEnd
//		);

CFunction*
CFunctionMgr::CreateCheckDataPtrRange ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.checkDataPtrRange", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];
	CValue ArgValue3 = ArgValueArray [2];
	CValue ArgValue4 = ArgValueArray [3];

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("sptr_fail");
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("sptr_success");
	CBasicBlock* pCmp2Block = m_pModule->m_ControlFlowMgr.CreateBlock ("sptr_cmp2");
	CBasicBlock* pCmp3Block = m_pModule->m_ControlFlowMgr.CreateBlock ("sptr_cmp3");

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmIrBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pCmp2Block, pCmp2Block);

	m_pModule->m_LlvmIrBuilder.CreateLt_u (ArgValue1, ArgValue3, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pCmp3Block, pCmp3Block);

	CValue PtrEndValue;
	m_pModule->m_LlvmIrBuilder.CreateGep (ArgValue1, ArgValue2, NULL ,&PtrEndValue);
	m_pModule->m_LlvmIrBuilder.CreateGt_u (PtrEndValue, ArgValue4, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	m_pModule->m_LlvmIrBuilder.RuntimeError (ERuntimeError_DataPtrOutOfRange);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);

	InternalEpilogue ();

	return pFunction;
}

// void
// jnc.CheckClassPtrScopeLevel (
//		object* p,
//		size_t DstScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCheckClassPtrScopeLevel ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.checkClassPtrScopeLevel", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_nonull");
	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_fail");
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_success");

	CValue CmpValue;

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr)->GetZeroValue ();

	m_pModule->m_LlvmIrBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pSuccessBlock, pNoNullBlock, pNoNullBlock);

	static int32_t LlvmIndexArray [] =
	{
		0, // TInterface**
		0, // TInterface*
		1, // TObject**
	};

	CValue ObjPtrValue;
	m_pModule->m_LlvmIrBuilder.CreateGep (ArgValue1, LlvmIndexArray, countof (LlvmIndexArray), NULL, &ObjPtrValue); // TObject** ppObject
	m_pModule->m_LlvmIrBuilder.CreateLoad (ObjPtrValue, NULL, &ObjPtrValue);  // TObject* pObject

	CValue SrcScopeLevelValue;
	m_pModule->m_LlvmIrBuilder.CreateGep2 (ObjPtrValue, 1, NULL, &SrcScopeLevelValue);     // size_t* pScopeLevel
	m_pModule->m_LlvmIrBuilder.CreateLoad (SrcScopeLevelValue, NULL, &SrcScopeLevelValue); // size_t ScopeLevel

	m_pModule->m_LlvmIrBuilder.CreateGt_u (SrcScopeLevelValue, ArgValue2, &CmpValue); // SrcScopeLevel > DstScopeLevel
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_LlvmIrBuilder.RuntimeError (ERuntimeError_ScopeMismatch);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);

	InternalEpilogue ();

	return pFunction;
}

// object*
// jnc.DynamicCastClassPtr (
//		object* p,
//		int8* pType
//		);

CFunction*
CFunctionMgr::CreateDynamicCastClassPtr ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.dynamicCastClassPtr", pType);
}

// object*
// jnc.StrengthenClassPtr (weak object* p);

CFunction*
CFunctionMgr::CreateStrengthenClassPtr ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr);

	CType* ArgTypeArray [] =
	{
		((CClassType*) m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectClass))->GetClassPtrType (EClassPtrType_Weak)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.strengthenClassPtr", pType);
}

// size_t
// jnc.GetDataPtrSpan (jnc.DataPtr Ptr);

CFunction*
CFunctionMgr::CreateGetDataPtrSpan ()
{
	CType* pIntPtrType = GetSimpleType (m_pModule, EType_Int_p);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void)->GetDataPtrType (EDataPtrType_Normal, EPtrTypeFlag_Const)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pIntPtrType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction ("getDataPtrSpan", "jnc.getDataPtrSpan", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue = ArgValueArray [0];

	CValue PtrValue;
	CValue RangeEndValue;
	CValue SpanValue;
	m_pModule->m_LlvmIrBuilder.CreateExtractValue (ArgValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmIrBuilder.CreateExtractValue (ArgValue, 2, NULL, &RangeEndValue);
	m_pModule->m_LlvmIrBuilder.CreatePtrToInt (PtrValue, pIntPtrType, &PtrValue);
	m_pModule->m_LlvmIrBuilder.CreatePtrToInt (RangeEndValue, pIntPtrType, &RangeEndValue);
	m_pModule->m_LlvmIrBuilder.CreateSub_i (RangeEndValue, PtrValue, pIntPtrType, &SpanValue);
	m_pModule->m_LlvmIrBuilder.CreateRet (SpanValue);

	InternalEpilogue ();

	return pFunction;
}

// int8*
// jnc.HeapAlloc (size_t Size);

CFunction*
CFunctionMgr::CreateHeapAlloc ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.heapAlloc", pType);
}

// int8*
// jnc.HeapUAlloc (size_t Size);

CFunction*
CFunctionMgr::CreateHeapUAlloc ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.uheapAlloc", pType);
}

// void
// jnc.HeapUFree (int8* p);

CFunction*
CFunctionMgr::CreateHeapUFree ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.uheapFree", pType);
}

// void
// jnc.HeapUFreeClassPtr (object* p);

CFunction*
CFunctionMgr::CreateHeapUFreeClassPtr ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.uheapFreeClassPtr", pType);
}

// void
// jnc.GcAddObject (object.hdr* p);

CFunction*
CFunctionMgr::CreateGcAddObject ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectHdrPtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.gcAddObject", pType);
}

// void
// jnc.MarkGcRoot (
//		int8* p,
//		int8* pType
//		);

CFunction*
CFunctionMgr::CreateMarkGcRoot ()
{
	CType* pBytePtrType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
	CType* pBytePtrPtrType = pBytePtrType->GetDataPtrType_c ();

	CType* ArgTypeArray [] =
	{
		pBytePtrPtrType,
		pBytePtrType,
	};

	llvm::Type* LlvmArgTypeArray [2] =
	{
		pBytePtrType->GetLlvmType (),
		pBytePtrType->GetLlvmType (),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));

	CFunction* pFunction = CreateInternalFunction ("jnc.markGcRoot", pType);

	pFunction->m_pLlvmFunction = llvm::Intrinsic::getDeclaration (
		m_pModule->GetLlvmModule (),
		llvm::Intrinsic::gcroot
		);

	return pFunction;
}

// void
// jnc.GcSafePoint ();

CFunction*
CFunctionMgr::CreateGcSafePoint ()
{
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType ();
	return CreateInternalFunction ("jnc.gcSafePoint", pType);
}

// void
// jnc.RunGc ();

CFunction*
CFunctionMgr::CreateRunGc ()
{
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType ();
	return CreateFunction ("runGc", "jnc.runGc", pType);
}

// void
// jnc.RunGcWaitForDestructors ();

CFunction*
CFunctionMgr::CreateRunGcWaitForDestructors ()
{
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType ();
	return CreateFunction ("runGcWaitForDestructors", "jnc.runGcWaitForDestructors", pType);
}

// i64
// jnc.GetCurrentThreadId ();

CFunction*
CFunctionMgr::CreateGetCurrentThreadId ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64_u);
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, NULL, 0, 0);
	return CreateFunction ("getCurrentThreadId", "jnc.getCurrentThreadId", pType);
}

// i64
// jnc.CreateThread (function* pf ());

CFunction*
CFunctionMgr::CreateCreateThread ()
{
	CFunctionType* pFunctionType = ((CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction));
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64_u);
	CType* ArgTypeArray [] =
	{
		pFunctionType->GetFunctionPtrType (EFunctionPtrType_Normal, EPtrTypeFlag_Checked),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateFunction ("createThread", "jnc.createThread", pType);
}

// void
// jnc.Sleep (uint_t MsCount);

CFunction*
CFunctionMgr::CreateSleep ()
{
	CType* ArgTypeArray [] =
	{
		((CFunctionType*) m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32_u)),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (ArgTypeArray, countof (ArgTypeArray));
	return CreateFunction ("sleep", "jnc.sleep", pType);
}

// uint64_t
// jnc.getTimestamp ();

CFunction*
CFunctionMgr::CreateGetTimestamp ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64_u);
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, NULL, 0);
	return CreateFunction ("getTimestamp", "jnc.getTimestamp", pType);
}

// size_t
// strlen (nullable const char* p);

CFunction*
CFunctionMgr::CreateStrLen ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType (EDataPtrType_Normal, EPtrTypeFlag_Const),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction ("strlen", "strlen", pType);
	return pFunction;
}

// int
// rand ();

CFunction*
CFunctionMgr::CreateRand ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int);
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, NULL, 0);
	CFunction* pFunction = CreateFunction ("rand", "rand", pType);
	return pFunction;
}

// int
// printf ();

CFunction*
CFunctionMgr::CreatePrintf ()
{
	CCallConv* pCallConv = m_pModule->m_TypeMgr.GetCallConv (ECallConv_Cdecl);
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
	CType* pFormatType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType (EType_DataPtr, EDataPtrType_Thin, EPtrTypeFlag_Unsafe | EPtrTypeFlag_Const);

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (
		pCallConv,
		pReturnType,
		&pFormatType, 1,
		EFunctionTypeFlag_VarArg
		);

	CFunction* pFunction = CreateFunction ("printf", "printf", pType);
	return pFunction;
}

// jnc.TTls*
// jnc.GcTls ();

CFunction*
CFunctionMgr::CreateGetTls ()
{
	CType* pReturnType = m_pModule->m_VariableMgr.GetTlsStructType ()->GetDataPtrType (
		EDataPtrType_Thin,
		EPtrTypeFlag_Unsafe
		);

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, NULL, 0, 0);
	return CreateInternalFunction ("jnc.getTls", pType);
}

// size_t
// jnc.AppendFmtLiteral_a (
//		jnc.TFmtLiteral* pLiteral,
//		char* p,
//		size_t Length
//		);

CFunction*
CFunctionMgr::CreateAppendFmtLiteral_a ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_FmtLiteral)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.appendFmtLiteral_a", pType);
	return pFunction;
}

// size_t
// jnc.AppendFmtLiteral_p (
//		jnc.TFmtLiteral* pFmtLiteral,
//		char* pFmtSpecifier,
//		jnc.TPtr Ptr
//		);

CFunction*
CFunctionMgr::CreateAppendFmtLiteral_p ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_FmtLiteral)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType (),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.appendFmtLiteral_p", pType);
	return pFunction;
}

// size_t
// jnc.AppendFmtLiteral_i32 (
//		jnc.TFmtLiteral* pFmtLiteral,
//		char* pFmtSpecifier,
//		i32 i
//		);

CFunction*
CFunctionMgr::CreateAppendFmtLiteral_i32 ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_FmtLiteral)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.appendFmtLiteral_i32", pType);
	return pFunction;
}

// size_t
// jnc.AppendFmtLiteral_ui32 (
//		jnc.TFmtLiteral* pFmtLiteral,
//		char* pFmtSpecifier,
//		i32 i
//		);

CFunction*
CFunctionMgr::CreateAppendFmtLiteral_ui32 ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_FmtLiteral)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32_u),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.appendFmtLiteral_ui32", pType);
	return pFunction;
}

// size_t
// jnc.AppendFmtLiteral_i64 (
//		jnc.TFmtLiteral* pFmtLiteral,
//		char* pFmtSpecifier,
//		i64 i
//		);

CFunction*
CFunctionMgr::CreateAppendFmtLiteral_i64 ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_FmtLiteral)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.appendFmtLiteral_i64", pType);
	return pFunction;
}

// size_t
// jnc.AppendFmtLiteral_ui64 (
//		jnc.TFmtLiteral* pFmtLiteral,
//		char* pFmtSpecifier,
//		i64 i
//		);

CFunction*
CFunctionMgr::CreateAppendFmtLiteral_ui64 ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_FmtLiteral)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64_u),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.appendFmtLiteral_ui64", pType);
	return pFunction;
}

// size_t
// jnc.AppendFmtLiteral_f (
//		jnc.TFmtLiteral* pFmtLiteral,
//		char* pFmtSpecifier,
//		double f
//		);

CFunction*
CFunctionMgr::CreateAppendFmtLiteral_f ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_FmtLiteral)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char)->GetDataPtrType_c (),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.appendFmtLiteral_f", pType);
	return pFunction;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

