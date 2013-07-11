#include "pch.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_GcStrategy.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionMgr::CFunctionMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_pCurrentFunction = NULL;
	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));

	mt::CallOnce (RegisterGcStrategy, 0);
}

void
CFunctionMgr::Clear ()
{
	m_LlvmFunctionMap.Clear ();
	m_FunctionList.Clear ();
	m_PropertyList.Clear ();
	m_PropertyTemplateList.Clear ();
	m_ThunkFunctionList.Clear ();
	m_ThunkPropertyList.Clear ();
	m_DataThunkPropertyList.Clear ();
	m_ThunkFunctionMap.Clear ();
	m_ThunkPropertyMap.Clear ();
	m_ScheduleLauncherFunctionMap.Clear ();
	m_EmissionContextStack.Clear ();
	
	ClearEmissionContext ();

	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));
}

CFunction*
CFunctionMgr::CreateFunction (
	EFunction FunctionKind,
	CFunctionType* pType
	)
{
	CFunction* pFunction;
	switch (FunctionKind)
	{
	case EFunction_Thunk:
		pFunction = AXL_MEM_NEW (CThunkFunction);
		m_ThunkFunctionList.InsertTail ((CThunkFunction*) pFunction);
		break;

	default:
		pFunction = AXL_MEM_NEW (CFunction);
		m_FunctionList.InsertTail (pFunction);
	}
	
	pFunction->m_pModule = m_pModule;
	pFunction->m_FunctionKind = FunctionKind;
	pFunction->m_pType = pType;
	pFunction->m_TypeOverload.AddOverload (pType);
	return pFunction;
}

CProperty*
CFunctionMgr::CreateProperty (
	EProperty PropertyKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
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
	m_pModule->MarkForLayout (pProperty);
	return pProperty;
}

CProperty*
CFunctionMgr::CreateInternalProperty (
	const char* pTag,
	CPropertyType* pType 
	)
{
	CProperty* pProperty = CreateProperty (rtl::CString (), rtl::CString ());
	pProperty->m_Tag = pTag;
	pProperty->Create (pType);
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

void
CFunctionMgr::SaveEmissionContext ()
{
	if (!m_pCurrentFunction)
		return;

	TEmissionContext* pContext = AXL_MEM_NEW (TEmissionContext);
	pContext->m_pCurrentFunction = m_pCurrentFunction;
	pContext->m_ThisValue = m_ThisValue;
	pContext->m_ScopeLevelValue = m_ScopeLevelValue;
	pContext->m_VTablePtrPtrValue = m_VTablePtrPtrValue;
	pContext->m_VTablePtrValue = m_VTablePtrValue;
	
	pContext->m_pCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	pContext->m_pReturnBlock = m_pModule->m_ControlFlowMgr.m_pReturnBlock;
	pContext->m_pSilentReturnBlock = m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock;
	pContext->m_pUnreachableBlock = m_pModule->m_ControlFlowMgr.m_pUnreachableBlock;
	pContext->m_ControlFlowMgrFlags = m_pModule->m_ControlFlowMgr.m_Flags;

	m_EmissionContextStack.InsertTail (pContext);
}

void
CFunctionMgr::RestoreEmissionContext ()
{
	if (m_EmissionContextStack.IsEmpty ())
		return;

	TEmissionContext* pContext = m_EmissionContextStack.RemoveTail ();
	m_pCurrentFunction = pContext->m_pCurrentFunction;
	m_ThisValue = pContext->m_ThisValue;
	m_ScopeLevelValue = pContext->m_ScopeLevelValue;
	m_VTablePtrPtrValue = pContext->m_VTablePtrPtrValue;
	m_VTablePtrValue = pContext->m_VTablePtrValue;

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pContext->m_pCurrentBlock);
	m_pModule->m_ControlFlowMgr.m_pReturnBlock = pContext->m_pReturnBlock;
	m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock = pContext->m_pSilentReturnBlock;
	m_pModule->m_ControlFlowMgr.m_pUnreachableBlock = pContext->m_pUnreachableBlock;
	m_pModule->m_ControlFlowMgr.m_Flags = pContext->m_ControlFlowMgrFlags;

	AXL_MEM_DELETE (pContext);
}

void
CFunctionMgr::ClearEmissionContext ()
{
	m_pCurrentFunction = NULL;
	m_ThisValue.Clear ();
	m_ScopeLevelValue.Clear ();
	m_VTablePtrPtrValue.Clear ();
	m_VTablePtrValue.Clear ();
}

void
CFunctionMgr::CutVTable (const CValue& ThisArgValue)
{
	ASSERT (ThisArgValue.GetType ()->GetTypeKind () == EType_ClassPtr);

	CClassType* pClassType = ((CClassPtrType*) ThisArgValue.GetType ())->GetTargetType ();

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "cut vtable ptr");

	int32_t LlvmIndexArray [] = 
	{
		0, // TInterface**
		0, // TInterface* 
		0, // vtable** 
	};

	m_pModule->m_LlvmBuilder.CreateGep (ThisArgValue, LlvmIndexArray, countof (LlvmIndexArray), NULL, &m_VTablePtrPtrValue);
	m_pModule->m_LlvmBuilder.CreateLoad (m_VTablePtrPtrValue, NULL, &m_VTablePtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (pClassType->GetVTablePtrValue (), m_VTablePtrPtrValue);
}

void
CFunctionMgr::RestoreVTable ()
{
	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "restore vtable ptr");
	m_pModule->m_LlvmBuilder.CreateStore (m_VTablePtrValue, m_VTablePtrPtrValue);
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

	CValue OnChangeEventValue;

	return
		m_pModule->m_OperatorMgr.GetPropertyOnChange (PropertyValue, &OnChangeEventValue) &&
		m_pModule->m_OperatorMgr.MemberOperator (&OnChangeEventValue, "Call") &&
		m_pModule->m_OperatorMgr.CallOperator (OnChangeEventValue);
}

bool
CFunctionMgr::Prologue (
	CFunction* pFunction,
	const CToken::CPos& Pos
	)
{
	bool Result;

	SaveEmissionContext ();
	ClearEmissionContext ();

	m_pCurrentFunction = pFunction;

	// create scope
	
	m_pModule->m_NamespaceMgr.OpenNamespace (pFunction->m_pParentNamespace);
	pFunction->m_pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos);

	// create entry block (gc roots come here)

	CBasicBlock* pEntryBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("function_entry");
	CBasicBlock* pPrologueBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("prologue");

	pFunction->m_pEntryBlock = pEntryBlock;
	pEntryBlock->MarkEntry ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pEntryBlock);

	if (pFunction->m_FunctionKind == EFunction_ModuleConstructor)
	{
		bool Result = m_pModule->m_VariableMgr.AllocatePrimeGlobalVariables ();
		if (!Result)
			return false;
	}
	
	m_pModule->m_ControlFlowMgr.Jump (pPrologueBlock, pPrologueBlock);
	m_pModule->m_ControlFlowMgr.m_pUnreachableBlock = NULL;
	m_pModule->m_ControlFlowMgr.m_Flags = 0; // clear jump flag

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "prologue");

	// save scope level

	if (pFunction->m_FunctionKind == EFunction_ModuleConstructor)
	{
		Result = m_pModule->m_VariableMgr.InitializeGlobalVariables ();
		if (!Result)
			return false;
	}
	else // do not save / restore scope level in module constructor
	{
		CVariable* pVariable = m_pModule->m_VariableMgr.GetStdVariable (EStdVariable_ScopeLevel);
		m_pModule->m_LlvmBuilder.CreateLoad (pVariable, NULL, &m_ScopeLevelValue);
	}
	
	Result = CreateShadowArgVariables ();
	if (!Result)
		return false;

	// 'this' arg

	if (pFunction->IsMember ())
	{
		llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
		CValue ThisArgValue (LlvmArg, pFunction->m_pThisArgType);

		CreateThisValue (ThisArgValue);

		// cut vtable

		if (pFunction->NeedsVTablePtrCut ())
		{
			CutVTable (ThisArgValue);
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
CFunctionMgr::CreateThisValue (const CValue& ThisArgValue)
{
	CFunction* pFunction = m_pCurrentFunction;
	ASSERT (pFunction && pFunction->IsMember ());

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
			m_pModule->m_LlvmBuilder.CreateExtractValue (ThisArgValue, 0, NULL, &PtrValue);

			pPtrType = pPtrType->GetTargetType ()->GetDataPtrType (EDataPtrType_Thin, pPtrType->GetFlags ());
			m_ThisValue.SetThinDataPtr (PtrValue.GetLlvmValue (), pPtrType, ThisArgValue);
		}
	}
	else
	{
		ASSERT (pFunction->m_StorageKind == EStorage_Override && pFunction->m_ThisArgDelta < 0);

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateBitCast (ThisArgValue, m_pModule->GetSimpleType (EStdType_BytePtr), &PtrValue);
		m_pModule->m_LlvmBuilder.CreateGep (PtrValue, (int32_t) pFunction->m_ThisArgDelta, NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pFunction->m_pThisType, &m_ThisValue);
	}
}

bool
CFunctionMgr::CreateShadowArgVariables ()
{
	bool Result;

	CFunction* pFunction = m_pCurrentFunction;
	ASSERT (pFunction);

	rtl::CArrayT <CFunctionArg*> ArgArray = pFunction->GetType ()->GetArgArray ();
	size_t ArgCount = ArgArray.GetCount ();
	size_t i = 0;

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

	if (pFunction->IsMember ())
	{
		i++;
		LlvmArg++;
	}

	for (; i < ArgCount; i++, LlvmArg++)
	{
		CFunctionArg* pArg = ArgArray [i];

		llvm::Value* pLlvmArg = LlvmArg;

		if (!pArg->IsNamed ())
			continue;

		CVariable* pArgVariable = m_pModule->m_VariableMgr.CreateVariable (
			EStorage_Stack,
			pArg->GetName (), 
			pArg->GetName (), 
			pArg->GetType (), 
			pArg->GetPtrTypeFlags ()
			);

		CValue PtrValue;
		Result = m_pModule->m_OperatorMgr.Allocate (
			EStorage_Stack, 
			pArg->GetType (), 
			pArg->GetName (), 
			&PtrValue
			);

		if (!Result)
			return false;

		pArgVariable->m_pLlvmAllocValue = PtrValue.GetLlvmValue ();
		pArgVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();
			
		CValue ArgValue (pLlvmArg, pArg->GetType ());

		m_pModule->m_LlvmBuilder.CreateStore (ArgValue, pArgVariable);
		Result = pFunction->m_pScope->AddItem (pArgVariable);
		if (!Result)
			return false;
	}

	return true;
}

bool
CFunctionMgr::Epilogue (const CToken::CPos& Pos)
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
	{
		m_pModule->m_OperatorMgr.ProcessDestructArray (m_pModule->m_VariableMgr.GetGlobalDestructArray ());
		m_pModule->m_OperatorMgr.ProcessStaticDestructList (m_pModule->m_VariableMgr.GetStaticDestructList ());
	}

	// ensure return

	CBasicBlock* pCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	if (!pCurrentBlock->HasTerminator ()) 
	{
		CType* pReturnType = pFunction->GetType ()->GetReturnType ();

		if (!(pCurrentBlock->GetFlags () & EBasicBlockFlag_Jumped))
		{
			m_pModule->m_LlvmBuilder.CreateUnreachable (); // just to make LLVM happy
		}
		else if (pReturnType->GetTypeKind () == EType_Void)
		{
			m_pModule->m_ControlFlowMgr.Return ();
		}
		else if (!(m_pModule->m_ControlFlowMgr.GetFlags () & EControlFlowFlag_HasReturn))
		{
			err::SetFormatStringError (
				"function '%s' must return a '%s' value",
				pFunction->m_Tag.cc (),
				pReturnType->GetTypeString ().cc ()
				);
			return false;
		}
		else 
		{
			err::SetFormatStringError (
				"not all control paths in function '%s' return a value",
				pFunction->m_Tag.cc ()
				);
			return false;
		}
	}

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

	m_pModule->m_NamespaceMgr.CloseScope (Pos);
	m_pModule->m_NamespaceMgr.CloseNamespace ();

	RestoreEmissionContext ();
	return true;
}

void
CFunctionMgr::InternalPrologue (
	CFunction* pFunction,
	CValue* pArgValueArray,
	size_t ArgCount
	)
{
	SaveEmissionContext ();
	ClearEmissionContext ();
	
	m_pCurrentFunction = pFunction;

	CBasicBlock* pEntryBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("function_entry");
	CBasicBlock* pPrologueBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("prologue");

	pFunction->m_pEntryBlock = pEntryBlock;
	pEntryBlock->MarkEntry ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pEntryBlock);
	m_pModule->m_ControlFlowMgr.Jump (pPrologueBlock, pPrologueBlock);
	m_pModule->m_ControlFlowMgr.m_pUnreachableBlock = NULL;
	m_pModule->m_ControlFlowMgr.m_Flags = 0;

	if (pFunction->m_FunctionKind != EFunction_ModuleConstructor) // do not save / restore scope level in module constructor
	{
		CVariable* pVariable = m_pModule->m_VariableMgr.GetStdVariable (EStdVariable_ScopeLevel);
		m_pModule->m_LlvmBuilder.CreateLoad (pVariable, NULL, &m_ScopeLevelValue);
	}

	rtl::CArrayT <CFunctionArg*> ArgArray = pFunction->GetType ()->GetArgArray ();
	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin ();

	if (pFunction->IsMember ())
		CreateThisValue (CValue (LlvmArg, pFunction->m_pThisArgType));

	for (size_t i = 0; i < ArgCount; i++, LlvmArg++)
		pArgValueArray [i] = CValue (LlvmArg, ArgArray [i]->GetType ());
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

	RestoreEmissionContext ();
}

CFunction*
CFunctionMgr::GetDirectThunkFunction (
	CFunction* pTargetFunction,
	CFunctionType* pThunkFunctionType,
	bool HasUnusedClosure
	)
{
	ASSERT (HasUnusedClosure || pTargetFunction->m_pType->Cmp (pThunkFunctionType) != 0);

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

	rtl::CStringHashTableMapIteratorAT <CFunction*> Thunk = m_ThunkFunctionMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;
	
	CThunkFunction* pThunkFunction = (CThunkFunction*) CreateFunction (EFunction_Thunk, pThunkFunctionType);
	pThunkFunction->m_StorageKind = EStorage_Static;
	pThunkFunction->m_Tag = "_direct_thunk_function";
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
	ASSERT (HasUnusedClosure || pTargetProperty->m_pType->Cmp (pThunkPropertyType) != 0);

	rtl::CString Signature;
	Signature.Format (
		"%c%x.%s", 
		HasUnusedClosure ? 'U' : 'D',
		pTargetProperty, 
		pThunkPropertyType->GetSignature ().cc ()
		);

	rtl::CStringHashTableMapIteratorAT <CProperty*> Thunk = m_ThunkPropertyMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;
	
	CThunkProperty* pThunkProperty = (CThunkProperty*) CreateProperty (EProperty_Thunk, rtl::CString (), rtl::CString ());
	pThunkProperty->m_StorageKind = EStorage_Static;
	pThunkProperty->m_Signature = Signature;
	pThunkProperty->m_Tag = "_direct_thunk_property";

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

	rtl::CStringHashTableMapIteratorAT <CProperty*> Thunk = m_ThunkPropertyMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;
	
	CDataThunkProperty* pThunkProperty = (CDataThunkProperty*) CreateProperty (EProperty_DataThunk, rtl::CString (), rtl::CString ());
	pThunkProperty->m_StorageKind = EStorage_Static;
	pThunkProperty->m_Signature = Signature;
	pThunkProperty->m_pTargetVariable = pTargetVariable;
	pThunkProperty->m_Tag = "_direct_data_thunk_property";

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
	CFunction* pTargetFunction, // could be NULL
	CClassPtrType* pSchedulerType // could be weak
	)
{
	bool Result;

	rtl::CString Signature;
	Signature.Format (
		"%s.%x.%s", 
		pTargetFunctionPtrType->GetSignature ().cc (),
		pTargetFunction, 
		pSchedulerType->GetSignature ().cc ()
		);

	rtl::CStringHashTableMapIteratorAT <CFunction*> Thunk = m_ScheduleLauncherFunctionMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;

	size_t SchedulerIdx = 0;

	rtl::CArrayT <CFunctionArg*> ArgArray  = pTargetFunctionPtrType->GetTargetType ()->GetArgArray ();
	if (!pTargetFunction)
	{
		ArgArray.Insert (0, pTargetFunctionPtrType->GetSimpleFunctionArg ());
		ArgArray.Insert (1, pSchedulerType->GetSimpleFunctionArg ());
		SchedulerIdx = 1;
	}
	else
	{
		ArgArray.Insert (0, pSchedulerType->GetSimpleFunctionArg ());
		SchedulerIdx = 0;
	}

	CFunctionType* pLauncherType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgArray);
	CFunction* pLauncher = CreateFunction (EFunction_ScheduleLauncher, pLauncherType);
	pLauncher->m_Tag = "_schedule_launcher_function";
	
	size_t ArgCount = ArgArray.GetCount (); 

	char Buffer [256];
	rtl::CArrayT <CValue> ArgValueArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgValueArray.SetCount (ArgCount);

	InternalPrologue (pLauncher, ArgValueArray, ArgCount);

	CValue ScheduleValue;
	m_pModule->m_OperatorMgr.MemberOperator (ArgValueArray [SchedulerIdx], "Schedule", &ScheduleValue);

	CValue FunctionPtrValue;

	if (pTargetFunction)
		FunctionPtrValue = pTargetFunction;
	else
		FunctionPtrValue = ArgValueArray [0];

	if (ArgCount > SchedulerIdx + 1)
	{
		rtl::CBoxListT <CValue> ArgList;
		for (size_t i = SchedulerIdx + 1; i < ArgCount; i++)
			ArgList.InsertTail (ArgValueArray [i]);

		Result = m_pModule->m_OperatorMgr.ClosureOperator (&FunctionPtrValue, &ArgList);
		if (!Result)
			return NULL;
	}

	m_pModule->m_OperatorMgr.CallOperator (ScheduleValue, FunctionPtrValue);
	if (!Result)
		return NULL;
	
	InternalEpilogue ();

	Thunk->m_Value = pLauncher;
	return pLauncher;
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
	const std::string& ErrorString
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

	CJitEventListener JitEventListener (this);
		
	pExecutionEngine->RegisterJITEventListener (&JitEventListener);
	
	CScopeThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;

		if (!pFunction->GetEntryBlock ())
			continue;

		try 
		{		
			void* pf = pExecutionEngine->getPointerToFunction (pFunction->GetLlvmFunction ());
			ASSERT (pFunction->m_pfMachineCode == pf && pFunction->m_MachineCodeSize != 0);
		}
		catch (err::CError Error)
		{
			err::SetFormatStringError (
				"LLVM jitting fail for '%s': %s", 
				pFunction->m_Tag.cc (),
				Error->GetDescription ().cc ()
				);

			return false;
		}
	}

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

	case EStdFunc_HeapAlloc:
		pFunction = CreateHeapAlloc ();
		break;

	case EStdFunc_UHeapAlloc:
		pFunction = CreateUHeapAlloc ();
		break;

	case EStdFunc_UHeapFree:
		pFunction = CreateUHeapFree ();
		break;

	case EStdFunc_UHeapFreeClassPtr:
		pFunction = CreateUHeapFreeClassPtr ();
		break;

	case EStdFunc_GcAddObject:
		pFunction = CreateGcAddObject ();
		break;

	case EStdFunc_MarkGcRoot:
		pFunction = CreateMarkGcRoot ();
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
	CFunctionType* pType 
	)
{
	CFunction* pFunction = CreateFunction (EFunction_Internal, pType);
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

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.OnRuntimeError", pType);
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

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.CheckNullPtr", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("iface_fail");
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("iface_success");
	
	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	m_pModule->m_LlvmBuilder.RuntimeError (ArgValue2);

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
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.CheckScopeLevel", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_fail");
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_success");
	
	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateGt_u (ArgValue1, ArgValue2, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_LlvmBuilder.RuntimeError (ERuntimeError_ScopeMismatch);

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
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.CheckDataPtrRange", pType);

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
	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pCmp2Block, pCmp2Block);

	m_pModule->m_LlvmBuilder.CreateLt_u (ArgValue1, ArgValue3, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pCmp3Block, pCmp3Block);	

	CValue PtrEndValue;
	m_pModule->m_LlvmBuilder.CreateGep (ArgValue1, ArgValue2, NULL ,&PtrEndValue);
	m_pModule->m_LlvmBuilder.CreateGt_u (PtrEndValue, ArgValue4, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	m_pModule->m_LlvmBuilder.RuntimeError (ERuntimeError_DataPtrOutOfRange);

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
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.CheckClassPtrScopeLevel", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_nonull");
	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_fail");
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("scope_success");

	CValue CmpValue;

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr)->GetZeroValue ();

	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pSuccessBlock, pNoNullBlock, pNoNullBlock);

	static int32_t LlvmIndexArray [] = 
	{
		0, // TInterface**
		0, // TInterface* 
		1, // TObject** 
	};

	CValue ObjPtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (ArgValue1, LlvmIndexArray, countof (LlvmIndexArray), NULL, &ObjPtrValue); // TObject** ppObject
	m_pModule->m_LlvmBuilder.CreateLoad (ObjPtrValue, NULL, &ObjPtrValue);  // TObject* pObject
	
	CValue SrcScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, NULL, &SrcScopeLevelValue);     // size_t* pScopeLevel
	m_pModule->m_LlvmBuilder.CreateLoad (SrcScopeLevelValue, NULL, &SrcScopeLevelValue); // size_t ScopeLevel

	m_pModule->m_LlvmBuilder.CreateGt_u (SrcScopeLevelValue, ArgValue2, &CmpValue); // SrcScopeLevel > DstScopeLevel
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_LlvmBuilder.RuntimeError (ERuntimeError_ScopeMismatch);

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
	return CreateInternalFunction ("jnc.DynamicCastClassPtr", pType);
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
	return CreateInternalFunction ("jnc.StrengthenClassPtr", pType);
}

// int8*
// jnc.HeapAlloc (int8* pType);

CFunction*
CFunctionMgr::CreateHeapAlloc ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.HeapAlloc", pType);
}

// int8*
// jnc.UHeapAlloc (int8* pType);

CFunction*
CFunctionMgr::CreateUHeapAlloc ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.UHeapAlloc", pType);
}

// void
// jnc.UHeapFree (int8* p);

CFunction*
CFunctionMgr::CreateUHeapFree ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.UHeapFree", pType);
}

// void
// jnc.UHeapFreeClassPtr (object* p);

CFunction*
CFunctionMgr::CreateUHeapFreeClassPtr ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.UHeapFreeClassPtr", pType);
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

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.GcAddObject", pType);
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
	CType* pBytePtrPtrType = pBytePtrType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe);

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

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));

	CFunction* pFunction = CreateInternalFunction ("jnc.MarkGcRoot", pType);

	pFunction->m_pLlvmFunction = llvm::Intrinsic::getDeclaration (
		m_pModule->GetLlvmModule (),
		llvm::Intrinsic::gcroot
		);

	return pFunction;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
