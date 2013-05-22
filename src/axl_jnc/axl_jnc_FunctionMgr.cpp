#include "pch.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Parser.llk.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionMgr::CFunctionMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_pCurrentFunction = NULL;

	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));
}

void
CFunctionMgr::Clear ()
{
	m_FunctionList.Clear ();
	m_PropertyList.Clear ();
	m_PropertyTemplateList.Clear ();
	m_AutoEvList.Clear ();
	m_OrphanFunctionArray.Clear ();
	m_DefaultPreConstructorTypeArray.Clear ();
	m_ThunkList.Clear ();
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
	CFunction* pFunction = AXL_MEM_NEW (CFunction);
	pFunction->m_pModule = m_pModule;
	pFunction->m_FunctionKind = FunctionKind;
	pFunction->m_pType = pType;
	pFunction->m_TypeOverload.AddOverload (pType);
	m_FunctionList.InsertTail (pFunction);
	return pFunction;
}

CProperty*
CFunctionMgr::CreateProperty (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	CProperty* pProperty = AXL_MEM_NEW (CProperty);
	pProperty->m_pModule = m_pModule;
	pProperty->m_Name = Name;
	pProperty->m_QualifiedName = QualifiedName;
	pProperty->m_Tag = QualifiedName;
	m_PropertyList.InsertTail (pProperty);
	return pProperty;
}

CPropertyTemplate*
CFunctionMgr::CreatePropertyTemplate ()
{
	CPropertyTemplate* pPropertyTemplate = AXL_MEM_NEW (CPropertyTemplate);
	m_PropertyTemplateList.InsertTail (pPropertyTemplate);
	return pPropertyTemplate;
}

CAutoEv*
CFunctionMgr::CreateAutoEv (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	CAutoEv* pAutoEv = AXL_MEM_NEW (CAutoEv);
	pAutoEv->m_pModule = m_pModule;
	pAutoEv->m_Name = Name;
	pAutoEv->m_QualifiedName = QualifiedName;	
	pAutoEv->m_Tag = QualifiedName;
	m_AutoEvList.InsertTail (pAutoEv);
	return pAutoEv;
}

bool
CFunctionMgr::ResolveOrphanFunctions ()
{
	bool Result;

	size_t Count = m_OrphanFunctionArray.GetCount ();

	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OrphanFunctionArray [i];
		
		Result = pFunction->ResolveOrphan ();
		if (!Result)
			return false;
	}

	return true;
}

bool
CFunctionMgr::CalcPropertyLayouts ()
{
	bool Result;

	rtl::CIteratorT <CProperty> Property = m_PropertyList.GetHead ();
	for (; Property; Property++)
	{
		Result = Property->CalcLayout ();
		if (!Result)
			return false;
	}

	return true;
}

bool
CFunctionMgr::CalcAutoEvLayouts ()
{
	bool Result;

	rtl::CIteratorT <CAutoEv> AutoEv = m_AutoEvList.GetHead ();
	for (; AutoEv; AutoEv++)
	{
		Result = AutoEv->CalcLayout ();
		if (!Result)
			return false;
	}

	return true;
}

#ifdef _AXL_JNC_PARSER_H

void 
LlvmFatalErrorHandler (
	void* pContext,
	const std::string& ErrorString
	)
{
	throw err::CreateStringError (ErrorString.c_str ());
}

bool
CFunctionMgr::ScanAutoEvs ()
{
	bool Result;

	CScopeThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	rtl::CIteratorT <CAutoEv> AutoEv = m_AutoEvList.GetHead ();
	for (; AutoEv; AutoEv++)
	{
		CAutoEv* pAutoEv = *AutoEv;
		if (!pAutoEv->HasBody ())
		{
			err::SetFormatStringError ("autoev '%s' has no body", pAutoEv->m_Tag.cc ()); // thanks a lot gcc
			return false;
		}

		// scan

		CParser Parser;
		Parser.m_Stage = CParser::EStage_AutoEvScan;
		Parser.m_pModule = m_pModule;

		CFunction* pStarter = pAutoEv->GetStarter ();
		m_pCurrentFunction = pStarter;
		m_pModule->m_NamespaceMgr.OpenNamespace (pStarter->GetParentNamespace ());

		Result = Parser.ParseTokenList (ESymbol_autoev_body_0, *pAutoEv->GetBody (), false);
		if (!Result)
			return false;

		ASSERT (Parser.m_AutoEvBindSiteTotalCount);
		pAutoEv->m_BindSiteCount = Parser.m_AutoEvBindSiteTotalCount;

		m_pModule->m_NamespaceMgr.CloseNamespace ();
	}

	m_pCurrentFunction = NULL;
	return true;
}

bool
CFunctionMgr::CompileFunctions ()
{
	bool Result;

	CScopeThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	// (1) functions

	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		// prologue

		Result = Prologue (pFunction, pFunction->GetBody ()->GetHead ()->m_Pos);
		if (!Result)
			return false;

		// parse body

		ESymbol StartSymbol = 
			pFunction->GetFunctionKind () == EFunction_Constructor && 
			pFunction->GetParentType ()->GetTypeKind () == EType_Class ? 
			ESymbol_constructor_compound_stmt :
			ESymbol_compound_stmt;

		CParser Parser;
		Parser.m_pModule = m_pModule;
		Parser.m_Stage = CParser::EStage_Pass2;

		Result = Parser.ParseTokenList (StartSymbol, *pFunction->GetBody (), true);
		if (!Result)
			return false;

		pFunction->m_Ast = Parser.GetAst ();

		// epilogue

		Result = Epilogue (pFunction->GetBody ()->GetTail ()->m_Pos);
		if (!Result)
			return false;
	}

	// (2) auto-properties 

	rtl::CIteratorT <CProperty> Property = m_PropertyList.GetHead ();
	for (; Property; Property++)
	{
		CProperty* pProperty = *Property;

		Result = pProperty->CalcLayout ();
		if (!Result)
			return false;

		uint_t Flags = pProperty->GetType ()->GetFlags ();

		if (Flags & EPropertyTypeFlag_AutoGet)
		{
			Result = pProperty->CompileAutoGetter ();
			if (!Result)
				return false;
		}

		if (Flags & EPropertyTypeFlag_AutoSet)
		{
			Result = pProperty->CompileAutoSetter ();
			if (!Result)
				return false;
		}
	}

	// (3) autoevs

	rtl::CIteratorT <CAutoEv> AutoEv = m_AutoEvList.GetHead ();
	for (; AutoEv; AutoEv++)
	{
		CAutoEv* pAutoEv = *AutoEv;
		ASSERT (pAutoEv->HasBody ());

		Result = Prologue (pAutoEv->GetStarter (), pAutoEv->GetBody ()->GetHead ()->m_Pos);
		if (!Result)
			return false;

		Result = m_pModule->m_OperatorMgr.CallOperator (pAutoEv->GetStopper ());
		if (!Result)
			return false;

		CParser Parser;
		Parser.m_Stage = CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;

		Result = Parser.ParseTokenList (ESymbol_autoev_body, *pAutoEv->GetBody (), true);
		if (!Result)
			return false;

		pAutoEv->m_Ast = Parser.GetAst ();

		CValue StateValue;
		Result = 
			m_pModule->m_OperatorMgr.GetAutoEvField (pAutoEv, EAutoEvField_State, &StateValue) &&
			m_pModule->m_OperatorMgr.StoreDataRef (StateValue, CValue ((int64_t) 1, EType_Int_p)) &&
			Epilogue (pAutoEv->GetBody ()->GetTail ()->m_Pos);

		if (!Result)
			return false;

		Result = pAutoEv->CreateStopper ();
		if (!Result)
			return false;
	}

	// (4) default preconstructors for classes/structs/unions with initialized fields

	size_t Count = m_DefaultPreConstructorTypeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CDerivableType* pType = m_DefaultPreConstructorTypeArray [i];

		m_pModule->m_NamespaceMgr.OpenNamespace (pType);

		Result = CreateDefaultPreConstructor (pType);
		if (!Result)
			return false;

		m_pModule->m_NamespaceMgr.CloseNamespace ();
	}

	// (5) thunks

	rtl::CIteratorT <TThunk> Thunk = m_ThunkList.GetHead ();
	for (; Thunk; Thunk++)
	{
		Result = Thunk->m_pClosureType ?
			CompileClosureThunk (*Thunk) :
			CompileDirectThunk (*Thunk);

		if (!Result)
			return false;
	}

	return true;
}

#endif

bool
CFunctionMgr::CreateDefaultPreConstructor (CDerivableType* pType)
{
	bool Result;

	EType TypeKind = pType->GetTypeKind ();

	CFunction* pPreConstructor = TypeKind == EType_Class ? 
		((CClassType*) pType)->GetPreConstructor () :
		pType->GetConstructor ();

	ASSERT (pPreConstructor);

	InternalPrologue (pPreConstructor);

	switch (TypeKind)
	{
	case EType_Class:
		Result = ((CClassType*) pType)->GetIfaceStructType ()->InitializeFields ();
		break;

	case EType_Struct:
		Result = 
			((CStructType*) pType)->CallBaseTypeConstructors () &&
			((CStructType*) pType)->InitializeFields ();
		break;

	case EType_Union:
		Result = ((CUnionType*) pType)->InitializeField ();
		break;
	}

	if (!Result)
		return false;

	InternalEpilogue ();
	return true;
}

void
CFunctionMgr::SaveEmissionContext ()
{
	if (!m_pCurrentFunction)
		return;

	TEmissionContext* pContext = AXL_MEM_NEW (TEmissionContext);
	pContext->m_pCurrentFunction = m_pCurrentFunction;
	pContext->m_ThisValue = m_ThisValue;
	pContext->m_ThinThisValue = m_ThinThisValue;
	pContext->m_ScopeLevelValue = m_ScopeLevelValue;
	pContext->m_VTablePtrPtrValue = m_VTablePtrPtrValue;
	pContext->m_VTablePtrValue = m_VTablePtrValue;
	pContext->m_MemberNewField = m_MemberNewField;

	pContext->m_pCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	pContext->m_pReturnBlock = m_pModule->m_ControlFlowMgr.m_pReturnBlock;
	pContext->m_pSilentReturnBlock = m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock;
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
	m_ThinThisValue = pContext->m_ThinThisValue;
	m_ScopeLevelValue = pContext->m_ScopeLevelValue;
	m_VTablePtrPtrValue = pContext->m_VTablePtrPtrValue;
	m_VTablePtrValue = pContext->m_VTablePtrValue;
	m_MemberNewField = pContext->m_MemberNewField;

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pContext->m_pCurrentBlock);
	m_pModule->m_ControlFlowMgr.m_pReturnBlock = pContext->m_pReturnBlock;
	m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock = pContext->m_pSilentReturnBlock;
	m_pModule->m_ControlFlowMgr.m_Flags = pContext->m_ControlFlowMgrFlags;

	AXL_MEM_DELETE (pContext);
}

void
CFunctionMgr::ClearEmissionContext ()
{
	m_pCurrentFunction = NULL;
	m_ThisValue.Clear ();
	m_ThinThisValue.Clear ();
	m_ScopeLevelValue.Clear ();
	m_VTablePtrPtrValue.Clear ();
	m_VTablePtrValue.Clear ();
	m_MemberNewField = NULL;
}

void
CFunctionMgr::CutVTable (const CValue& ThisArgValue)
{
	ASSERT (ThisArgValue.GetType ()->GetTypeKind () == EType_ClassPtr);

	CClassType* pClassType = ((CClassPtrType*) ThisArgValue.GetType ())->GetTargetType ();

	m_pModule->m_LlvmBuilder.CreateComment ("cut vtable ptr");

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
	m_pModule->m_LlvmBuilder.CreateComment ("restore vtable ptr");
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
		CClosure* pClosure = PropertyValue.CreateClosure ();
		pClosure->GetArgList ()->InsertTail (m_ThisValue);
	}

	CValue OnChangeEventValue;
	return
		m_pModule->m_OperatorMgr.GetStdField (PropertyValue, EStdField_OnChange, &OnChangeEventValue) &&
		m_pModule->m_OperatorMgr.CallOperator (OnChangeEventValue);
}

void
CFunctionMgr::CreateThinThisValue ()
{	
	ASSERT (m_ThisValue && m_ThisValue.GetType ()->GetTypeKind () == EType_DataPtr);

	CDataPtrType* pPtrType = ((CDataPtrType*) m_ThisValue.GetType ());
	ASSERT (pPtrType->GetPtrTypeKind () == EDataPtrType_Normal);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (m_ThisValue, 0, NULL, &PtrValue);

	pPtrType = pPtrType->GetTargetType ()->GetDataPtrType (EDataPtrType_Thin, pPtrType->GetFlags ());
	m_ThinThisValue.SetThinDataPtr (PtrValue.GetLlvmValue (), pPtrType, m_ThisValue);
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

	// create entry block 

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("function_entry");
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);
	m_pModule->m_ControlFlowMgr.m_Flags = 0;

	// save scope level

	m_pModule->m_LlvmBuilder.CreateLoad (m_pModule->m_VariableMgr.GetScopeLevelVariable (), NULL, &m_ScopeLevelValue);
	
	// 'this' arg

	if (pFunction->IsMember ())
	{
		llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
		CValue ThisArgValue (LlvmArg, pFunction->m_pThisArgType);			
		CValue PtrValue;

		if (pFunction->m_pThisArgType->Cmp (pFunction->m_pThisType) == 0)
		{
			m_ThisValue = ThisArgValue;

			if (pFunction->m_pThisType->GetTypeKind () == EType_DataPtr)
				CreateThinThisValue ();
		}
		else
		{
			ASSERT (pFunction->m_StorageKind == EStorage_Override && pFunction->m_ThisArgDelta < 0);

			m_pModule->m_LlvmBuilder.CreateBitCast (ThisArgValue, CGetType (m_pModule, EStdType_BytePtr), &PtrValue);
			m_pModule->m_LlvmBuilder.CreateGep (PtrValue, (int32_t) pFunction->m_ThisArgDelta, NULL, &PtrValue);
			m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pFunction->m_pThisType, &m_ThisValue);
		}

		if (pFunction->NeedsVTablePtrCut ())
			CutVTable (ThisArgValue);
	}

	CNamedType* pParentType = pFunction->GetParentType ();

	EFunction FuncitonKind = pFunction->GetFunctionKind ();
	switch (FuncitonKind) 
	{
	case EFunction_PreConstructor:
		ASSERT (pParentType && pParentType->GetTypeKind () == EType_Class);
		m_MemberNewField = ((CClassType*) pParentType)->GetFirstMemberNewField ();
		Result = ((CClassType*) pParentType)->GetIfaceStructType ()->InitializeFields ();
		break;

	case EFunction_Constructor:
		ASSERT (pParentType);
		switch (pParentType->GetTypeKind ())
		{
		case EType_Struct:
			Result = ((CStructType*) pParentType)->InitializeFields ();
			break;

		case EType_Union:
			Result = ((CUnionType*) pParentType)->InitializeField ();
			break;

		default:
			Result = true;
		}

		break;

	case EFunction_ModuleConstructor:
		Result = m_pModule->m_VariableMgr.InitializeGlobalVariables ();
		break;

	case EFunction_AutoEvStarter:
		Result = CreateAutoEvArgFields ();
		break;

	default:
		Result = CreateShadowArgVariables ();
	}

	if (!Result)
		return false;

	if (pFunction->NeedsVTablePtrCut ())
	{
		m_pModule->m_ControlFlowMgr.m_pReturnBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("return_block");
	}
	else if (pFunction->m_FunctionKind == EFunction_Setter && (pFunction->m_pProperty->GetType ()->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		m_pModule->m_ControlFlowMgr.m_pReturnBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("return_block");
		m_pModule->m_ControlFlowMgr.m_pSilentReturnBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("silent_return_block");
	}

	return true;
}

bool
CFunctionMgr::CreateShadowArgVariables ()
{
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

		m_pModule->m_VariableMgr.AllocateVariable (pArgVariable);

		CValue ArgValue (pLlvmArg, pArg->GetType ());

		m_pModule->m_LlvmBuilder.CreateStore (ArgValue, pArgVariable);
		pFunction->m_pScope->AddItem (pArgVariable);
	}

	return true;
}

bool
CFunctionMgr::CreateAutoEvArgFields ()
{
	CFunction* pFunction = m_pCurrentFunction;
	ASSERT (pFunction && pFunction->m_pAutoEv);

	CValue AutoEvDataValue;

	bool Result = m_pModule->m_OperatorMgr.GetAutoEvData (pFunction->m_pAutoEv, &AutoEvDataValue);
	if (!Result)
		return false;

	rtl::CArrayT <CFunctionArg*> ArgArray = pFunction->GetType ()->GetArgArray ();
	size_t ArgCount = ArgArray.GetCount ();
	size_t i = 0;

	rtl::CIteratorT <CStructField> ArgField = pFunction->m_pAutoEv->GetFirstArgField ();
	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

	if (pFunction->IsMember ())
	{
		i++;
		LlvmArg++;
	}

	for (; i < ArgCount; i++, LlvmArg++, ArgField++)
	{
		CFunctionArg* pArg = ArgArray [i];
		llvm::Value* pLlvmArg = LlvmArg;
		CStructField* pArgField = *ArgField;

		if (!pArg->IsNamed ())
			continue;

		CValue ArgValue (pLlvmArg, pArg->GetType ());

		CValue StoreValue;
		Result = m_pModule->m_OperatorMgr.GetStructField (AutoEvDataValue, pArgField, NULL, &StoreValue);
		if (!Result)
			return false;
			
		m_pModule->m_LlvmBuilder.CreateStore (ArgValue, StoreValue);
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
			pClassType->StopAutoEvs (m_ThisValue) &&
			pClassType->CallMemberNewDestructors (m_ThisValue) &&
			pClassType->CallBaseDestructors (m_ThisValue);

		if (!Result)
			return false;
	}

	// ensure return

	CBasicBlock* pCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	if (!pCurrentBlock->HasReturn ()) 
	{
		CType* pReturnType = pFunction->GetType ()->GetReturnType ();
		if (pReturnType->GetTypeKind () == EType_Void)
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
		else if (pCurrentBlock->GetFlags () & EBasicBlockFlag_Jumped)
		{
			err::SetFormatStringError (
				"not all control paths in function '%s' return a value",
				pFunction->m_Tag.cc ()
				);
			return false;
		}
		else if (!pCurrentBlock->HasTerminator ())
		{	
			// make LLVM happy
			m_pModule->m_LlvmBuilder.CreateUnreachable (); 
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

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("function_entry");

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);
	m_pModule->m_ControlFlowMgr.m_Flags = 0;

	m_pModule->m_LlvmBuilder.CreateLoad (m_pModule->m_VariableMgr.GetScopeLevelVariable (), NULL, &m_ScopeLevelValue);

	rtl::CArrayT <CFunctionArg*> ArgArray = pFunction->GetType ()->GetArgArray ();
	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin ();

	if (pFunction->IsMember ())
	{
		ASSERT (pFunction->m_pThisArgType->Cmp (pFunction->m_pThisType) == 0);

		m_ThisValue = CValue (LlvmArg, pFunction->m_pThisType);
		
		if (pFunction->m_pThisType->GetTypeKind () == EType_DataPtr)
			CreateThinThisValue ();
	}

	for (size_t i = 0; i < ArgCount; i++, LlvmArg++)
		pArgValueArray [i] = CValue (LlvmArg, ArgArray [i]->GetType ());
}

void
CFunctionMgr::InternalEpilogue ()
{
	CFunction* pFunction = m_pCurrentFunction;

	CBasicBlock* pCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	if (!pCurrentBlock->HasReturn ()) 
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
	EThunk ThunkKind = EThunk_Direct;

	if (HasUnusedClosure)
	{
		SignatureChar = 'U';
		ThunkKind = EThunk_DirectUnusedClosure;
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
	
	CFunction* pThunkFunction = CreateFunction (EFunction_Thunk, pThunkFunctionType);
	pThunkFunction->m_StorageKind = EStorage_Static;
	pThunkFunction->m_Tag = "_direct_thunk_function";

	TThunk* pThunk = AXL_MEM_NEW (TThunk);
	pThunk->m_ThunkKind = ThunkKind;
	pThunk->m_Signature = Signature;
	pThunk->m_pTargetFunctionType = pTargetFunction->GetType ();
	pThunk->m_pTargetFunction = pTargetFunction;
	pThunk->m_pClosureType = NULL;
	pThunk->m_pThunkFunction = pThunkFunction;
	m_ThunkList.InsertTail (pThunk);

	Thunk->m_Value = pThunkFunction;
	return pThunkFunction;
}

CFunction*
CFunctionMgr::GetClosureThunkFunction (
	CFunctionType* pTargetFunctionType,
	CFunction* pTargetFunction,
	CClassType* pClosureType,
	const rtl::CArrayT <size_t>& ClosureMap,
	CFunctionType* pThunkFunctionType
	)
{
	rtl::CString Signature;
	Signature.Format ("C%s.%x.%s", 
		pTargetFunctionType->GetSignature ().cc (),
		pTargetFunction, 
		pThunkFunctionType->GetSignature ().cc ()
		);

	size_t Count = ClosureMap.GetCount ();
	for (size_t i = 0; i < Count; i++)
		Signature.AppendFormat (".%d", ClosureMap [i]);

	rtl::CStringHashTableMapIteratorAT <CFunction*> Thunk = m_ThunkFunctionMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;
	
	pThunkFunctionType = pClosureType->GetMemberMethodType (pThunkFunctionType);
	
	CFunction* pThunkFunction = CreateFunction (EFunction_Thunk, pThunkFunctionType);
	pThunkFunction->m_StorageKind = EStorage_Static;
	pThunkFunction->m_Tag = "_closure_thunk_function";

	TThunk* pThunk = AXL_MEM_NEW (TThunk);
	pThunk->m_ThunkKind = EThunk_Closure;
	pThunk->m_Signature = Signature;
	pThunk->m_pTargetFunctionType = pTargetFunctionType;
	pThunk->m_pTargetFunction = pTargetFunction;
	pThunk->m_pClosureType = pClosureType;
	pThunk->m_ClosureMap = ClosureMap;
	pThunk->m_pThunkFunction = pThunkFunction;
	m_ThunkList.InsertTail (pThunk);
	
	Thunk->m_Value = pThunkFunction;
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
	
	CProperty* pThunkProperty = CreateProperty (rtl::CString (), rtl::CString ());
	pThunkProperty->m_StorageKind = EStorage_Static;
	pThunkProperty->m_Tag = "_direct_thunk_property";
	pThunkProperty->m_pType = HasUnusedClosure ? 
		pThunkPropertyType->GetStdObjectMemberPropertyType () : 
		pThunkPropertyType;
	pThunkProperty->m_pGetter = GetDirectThunkFunction (
		pTargetProperty->m_pGetter, 
		pThunkPropertyType->GetGetterType (),
		HasUnusedClosure
		);

	CFunctionTypeOverload* pThunkSetterType = pThunkPropertyType->GetSetterType ();

	size_t SetterCount = pThunkSetterType->GetOverloadCount ();

	// all the checks should have been done at CheckCast ()

	ASSERT (SetterCount == 0 || pTargetProperty->m_pSetter); 
	
	for (size_t i = 0; i < SetterCount; i++)
	{
		CFunctionType* pThunkFunctionType = pThunkSetterType->GetOverload (i);

		CFunction* pTargetSetter = pTargetProperty->m_pSetter->ChooseSetterOverload (pThunkFunctionType);
		ASSERT (pTargetSetter);

		CFunction* pThunkFunction = GetDirectThunkFunction (
			pTargetSetter, 
			pThunkFunctionType,
			HasUnusedClosure
			);

		if (!pThunkProperty->m_pSetter)
		{
			pThunkProperty->m_pSetter = pThunkFunction;
		}
		else
		{
			bool Result = pThunkProperty->m_pSetter->AddOverload (pThunkFunction);
			ASSERT (Result);
		}
	}

	Thunk->m_Value = pThunkProperty;

	pThunkProperty->CalcLayout ();
	return pThunkProperty;
}

CProperty*
CFunctionMgr::GetClosureThunkProperty (
	CPropertyType* pTargetPropertyType,
	CProperty* pTargetProperty,
	CClassType* pClosureType,
	const rtl::CArrayT <size_t>& ClosureMap,
	CPropertyType* pThunkPropertyType
	)
{
	rtl::CString Signature;
	Signature.Format ("C%s.%x.%s", 
		pTargetPropertyType->GetSignature ().cc (),
		pTargetProperty, 
		pThunkPropertyType->GetSignature ().cc ()
		);

	size_t Count = ClosureMap.GetCount ();
	for (size_t i = 0; i < Count; i++)
		Signature.AppendFormat (".%d", ClosureMap [i]);

	rtl::CStringHashTableMapIteratorAT <CProperty*> Thunk = m_ThunkPropertyMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;

	CProperty* pThunkProperty = CreateProperty (rtl::CString (), rtl::CString ());
	pThunkProperty->m_StorageKind = EStorage_Static;
	pThunkProperty->m_Tag = "_closure_thunk_property";
	pThunkProperty->m_pType = pClosureType->GetMemberPropertyType (pThunkPropertyType);

	// i use ASSERT () cause all the checks should have been done at CheckCast ()

	if (pTargetProperty)
	{
		pThunkProperty->m_pGetter = GetClosureThunkFunction (
			pTargetProperty->m_pGetter->GetType (), 
			pTargetProperty->m_pGetter, 
			pClosureType,
			ClosureMap, 
			pThunkPropertyType->GetGetterType ()
			);

		CFunctionTypeOverload* pThunkSetterType = pThunkPropertyType->GetSetterType ();
		size_t SetterCount = pThunkSetterType->GetOverloadCount ();

		ASSERT (SetterCount == 0 || pTargetProperty->m_pSetter); 
	
		for (size_t i = 0; i < SetterCount; i++)
		{
			CFunctionType* pThunkFunctionType = pThunkSetterType->GetOverload (i);

			CFunction* pTargetSetter = pTargetProperty->m_pSetter->ChooseSetterOverload (pThunkFunctionType);
			ASSERT (pTargetSetter);

			CFunction* pThunkFunction = GetClosureThunkFunction (
				pTargetSetter->GetType (), 
				pTargetSetter,
				pClosureType,
				ClosureMap,
				pThunkFunctionType
				);

			if (!pThunkProperty->m_pSetter)
			{
				pThunkProperty->m_pSetter = pThunkFunction;
			}
			else
			{
				bool Result = pThunkProperty->m_pSetter->AddOverload (pThunkFunction);
				ASSERT (Result);
			}
		}
	}
	else
	{
		pThunkProperty->m_pGetter = GetClosureThunkFunction (
			pTargetPropertyType->GetGetterType (), 
			NULL, 
			pClosureType,
			ClosureMap, 
			pThunkPropertyType->GetGetterType ()
			);

		CFunctionTypeOverload* pTargetSetterType = pTargetPropertyType->GetSetterType ();
	
		CFunctionTypeOverload* pThunkSetterType = pThunkPropertyType->GetSetterType ();
		size_t SetterCount = pThunkSetterType->GetOverloadCount ();

		for (size_t i = 0; i < SetterCount; i++)
		{
			CFunctionType* pThunkFunctionType = pThunkSetterType->GetOverload (i);

			size_t j = pTargetSetterType->ChooseSetterOverload (pThunkFunctionType);
			ASSERT (j != -1);

			CFunctionType* pTargetFunctionType = pTargetSetterType->GetOverload (j);

			CFunction* pThunkFunction = GetClosureThunkFunction (
				pTargetFunctionType,
				NULL,
				pClosureType,
				ClosureMap,
				pThunkFunctionType
				);

			if (!pThunkProperty->m_pSetter)
			{
				pThunkProperty->m_pSetter = pThunkFunction;
			}
			else
			{
				bool Result = pThunkProperty->m_pSetter->AddOverload (pThunkFunction);
				ASSERT (Result);
			}
		}
	}

	Thunk->m_Value = pThunkProperty;

	pThunkProperty->CalcLayout ();
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
	
	CProperty* pThunkProperty = CreateProperty (rtl::CString (), rtl::CString ());
	pThunkProperty->m_StorageKind = EStorage_Static;
	pThunkProperty->m_Tag = "_direct_data_thunk_property";
	pThunkProperty->m_pType = HasUnusedClosure ? 
		pThunkPropertyType->GetStdObjectMemberPropertyType () : 
		pThunkPropertyType;

	CFunction* pGetter = CreateFunction (EFunction_Getter, pThunkProperty->m_pType->GetGetterType ());
	pGetter->m_StorageKind = EStorage_Static;
	pGetter->m_Tag = "_direct_data_thunk_getter";
		
	InternalPrologue (pGetter);

	Result = m_pModule->m_ControlFlowMgr.Return (pTargetVariable);
	ASSERT (Result);

	InternalEpilogue ();

	pThunkProperty->m_pGetter = pGetter;

	CFunctionTypeOverload* pThunkSetterType = pThunkProperty->m_pType->GetSetterType ();

	size_t SetterCount = pThunkSetterType->GetOverloadCount ();

	// all the checks should have been done at CheckCast ()

	for (size_t i = 0; i < SetterCount; i++)
	{
		CFunction* pSetter = CreateFunction (EFunction_Setter, pThunkSetterType->GetOverload (i));
		pSetter->m_StorageKind = EStorage_Static;
		pSetter->m_Tag = "_direct_data_thunk_setter";

		CValue SrcValue;

		if (HasUnusedClosure)
		{
			CValue ArgValueArray [2];
			InternalPrologue (pSetter, ArgValueArray, 2);
			SrcValue = ArgValueArray [1];
		}
		else
		{
			InternalPrologue (pSetter, &SrcValue, 1);
		}

		Result = m_pModule->m_OperatorMgr.StoreDataRef (pTargetVariable, SrcValue);
		ASSERT (Result);

		InternalEpilogue ();

		if (!pThunkProperty->m_pSetter)
		{
			pThunkProperty->m_pSetter = pSetter;
		}
		else
		{
			Result = pThunkProperty->m_pSetter->AddOverload (pSetter);
			ASSERT (Result);
		}
	}

	Thunk->m_Value = pThunkProperty;

	pThunkProperty->CalcLayout ();
	return pThunkProperty;
}

CProperty*
CFunctionMgr::GetClosureDataThunkProperty (
	CDataPtrType* pTargetDataPtrType,
	CPropertyType* pThunkPropertyType
	)
{
	ASSERT (pTargetDataPtrType->GetPtrTypeKind () != EDataPtrType_Thin);

	bool Result;

	rtl::CString Signature;
	Signature.Format ("C%s.%s", 
		pTargetDataPtrType->GetSignature ().cc (),
		pThunkPropertyType->GetSignature ().cc ()
		);

	rtl::CStringHashTableMapIteratorAT <CProperty*> Thunk = m_ThunkPropertyMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;

	CProperty* pThunkProperty = CreateProperty (rtl::CString (), rtl::CString ());
	pThunkProperty->m_StorageKind = EStorage_Static;
	pThunkProperty->m_Tag = "_closure_data_thunk_property";

	CType* pReturnType = pThunkPropertyType->GetGetterType ()->GetReturnType ();
	CFunctionArg* pThisArg = pTargetDataPtrType->GetSimpleFunctionArg ();

	rtl::CArrayT <CFunctionArg*> ArgArray = pThunkPropertyType->GetGetterType ()->GetArgArray ();
	ArgArray.Insert (0, pThisArg);

	CFunctionType* pGetterType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgArray);

	CFunction* pGetter = CreateFunction (EFunction_Getter, pGetterType);
	pGetter->m_StorageKind = EStorage_Static;
	pGetter->m_Tag = "_closure_data_thunk_getter";
		
	CValue DataValue;
	InternalPrologue (pGetter, &DataValue, 1);

	Result = 
		m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, &DataValue) &&
		m_pModule->m_ControlFlowMgr.Return (DataValue);

	ASSERT (Result);

	InternalEpilogue ();

	pThunkProperty->m_pGetter = pGetter;

	CFunctionTypeOverload* pThunkSetterType = pThunkPropertyType->GetSetterType ();
	size_t SetterCount = pThunkSetterType->GetOverloadCount ();

	for (size_t i = 0; i < SetterCount; i++)
	{
		CFunctionType* pThunkFunctionType = pThunkSetterType->GetOverload (i);

		ArgArray = pThunkFunctionType->GetArgArray ();
		ArgArray.Insert (0, pThisArg);

		CFunctionType* pSetterType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgArray);

		CFunction* pSetter = CreateFunction (EFunction_Setter, pSetterType);
		pSetter->m_StorageKind = EStorage_Static;
		pSetter->m_Tag = "_closure_data_thunk_setter";
		
		CValue ArgValueArray [2];
		InternalPrologue (pSetter, ArgValueArray, 2);

		Result = 
			m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, &ArgValueArray [0]) &&
			m_pModule->m_OperatorMgr.StoreDataRef (ArgValueArray [0], ArgValueArray [1]);

		ASSERT (Result);

		InternalEpilogue ();

		if (!pThunkProperty->m_pSetter)
		{
			pThunkProperty->m_pSetter = pSetter;
		}
		else
		{
			bool Result = pThunkProperty->m_pSetter->AddOverload (pSetter);
			ASSERT (Result);
		}
	}

	pThunkProperty->m_pType = m_pModule->m_TypeMgr.GetPropertyType (
		pGetterType, 
		pThunkProperty->m_pSetter ? *pThunkProperty->m_pSetter->GetTypeOverload () : NULL, 
		pThunkPropertyType->GetFlags ()
		);

	Thunk->m_Value = pThunkProperty;

	pThunkProperty->CalcLayout ();
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

bool
CFunctionMgr::CompileDirectThunk (TThunk* pThunk)
{
	ASSERT (pThunk->m_pTargetFunction);

	bool Result;

	rtl::CArrayT <CFunctionArg*> TargetArgArray = pThunk->m_pTargetFunction->GetType ()->GetArgArray ();
	rtl::CArrayT <CFunctionArg*> ThunkArgArray = pThunk->m_pThunkFunction->GetType ()->GetArgArray ();

	size_t TargetArgCount = TargetArgArray.GetCount ();
	size_t ThunkArgCount = ThunkArgArray.GetCount ();

	char Buffer [256];
	rtl::CArrayT <CValue> ArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgArray.SetCount (TargetArgCount);

	CFunction* pFunction = pThunk->m_pThunkFunction;

	InternalPrologue (pFunction);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

	// handle the first thunk argument (it's either NULL or actual interface)

	size_t i = 0;

	if (TargetArgCount == ThunkArgCount)
	{
		CValue IfaceValue (LlvmArg, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr));
		m_pModule->m_LlvmBuilder.CreateBitCast (IfaceValue, TargetArgArray [0]->GetType (), &IfaceValue);
		ArgArray [0] = IfaceValue;
		i++;
	}
	else
	{
		ASSERT (ThunkArgCount == TargetArgCount + 1);
		ThunkArgArray.Remove (0); 
		LlvmArg++;
	}

	for (; i < TargetArgCount; i++, LlvmArg++)
	{
		CValue ArgValue (LlvmArg, ThunkArgArray [i]->GetType ());
		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgArray [i]->GetType ());
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pThunk->m_pTargetFunction, 
		pThunk->m_pTargetFunction->GetType (),
		ArgArray,
		ArgArray.GetCount (),
		&ReturnValue
		);

	if (pFunction->GetType ()->GetReturnType ()->GetTypeKind () != EType_Void)
	{
		Result = m_pModule->m_ControlFlowMgr.Return (ReturnValue);
		if (!Result)
			return false;
	}

	InternalEpilogue ();
	return true;
}

bool
CFunctionMgr::CompileClosureThunk (TThunk* pThunk)
{
	ASSERT (pThunk->m_pClosureType);

	bool Result;

	rtl::CArrayT <CFunctionArg*> TargetArgArray = pThunk->m_pTargetFunctionType->GetArgArray ();
	rtl::CArrayT <CFunctionArg*> ThunkArgArray = pThunk->m_pThunkFunction->GetType ()->GetArgArray ();
	rtl::CArrayT <size_t> ClosureMap = pThunk->m_ClosureMap;

	size_t TargetArgCount = TargetArgArray.GetCount ();
	size_t ThunkArgCount = ThunkArgArray.GetCount ();
	size_t ClosureArgCount = pThunk->m_ClosureMap.GetCount ();

	char Buffer [256];
	rtl::CArrayT <CValue> ArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgArray.SetCount (TargetArgCount);

	CFunction* pFunction = pThunk->m_pThunkFunction;

	InternalPrologue (pFunction);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

	CValue ClosureValue (LlvmArg++, pThunk->m_pClosureType->GetClassPtrType ());

	CStructType* pClosureFieldStructType = pThunk->m_pClosureType->GetIfaceStructType ();
	ASSERT (pClosureFieldStructType);

	rtl::CIteratorT <CStructField> ClosureMember = pClosureFieldStructType->GetFieldList ().GetHead ();

	CValue PfnValue;

	if (pThunk->m_pTargetFunction)
	{
		PfnValue = pThunk->m_pTargetFunction;
	}
	else
	{
		Result = m_pModule->m_OperatorMgr.GetClassFieldValue (ClosureValue, *ClosureMember, &PfnValue);
		if (!Result)
			return false;

		ClosureMember++;
	}
	
	size_t i = 0;
	size_t iThunk = 1; // skip ClosureValue arg
	size_t iClosure = 0;

	for (; i < TargetArgCount && iClosure < ClosureArgCount; i++)
	{
		CValue ArgValue;

		if (i == pThunk->m_ClosureMap [iClosure])
		{		
			Result = m_pModule->m_OperatorMgr.GetClassFieldValue (ClosureValue, *ClosureMember, &ArgValue);

			if (!Result)
				return false;

			ClosureMember++;
			iClosure++;
		}
		else
		{
			ArgValue = CValue (LlvmArg, ThunkArgArray [iThunk]->GetType ());
			LlvmArg++;
			iThunk++;
		}

		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgArray [i]->GetType ());
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	for (; i < TargetArgCount; i++, iThunk, LlvmArg++)
	{
		CValue ArgValue (LlvmArg, ThunkArgArray [iThunk]->GetType ());

		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgArray [i]->GetType ());
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	CValue ReturnValue;

	m_pModule->m_LlvmBuilder.CreateCall (
		PfnValue, 
		pThunk->m_pTargetFunctionType,
		ArgArray,
		ArgArray.GetCount (),
		&ReturnValue
		);

	if (pFunction->GetType ()->GetReturnType ()->GetTypeKind () != EType_Void)
	{
		Result = m_pModule->m_ControlFlowMgr.Return (ReturnValue);
		if (!Result)
			return false;
	}

	InternalEpilogue ();
	return true;
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

	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;

		if (!pFunction->GetBlock ())
			continue;

		try 
		{
			pFunction->m_pf = pExecutionEngine->getPointerToFunction (pFunction->GetLlvmFunction ());
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

		ASSERT (pFunction->m_pf);
	}

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
		pFunction = CreateUHeapFree ();
		break;

	case EStdFunc_MulticastSet:
		pFunction = CreateMulticastSet (EFunctionPtrType_Normal, "jnc.MulticastSet");
		break;

	case EStdFunc_MulticastSet_w:
		pFunction = CreateMulticastSet (EFunctionPtrType_Weak, "jnc.MulticastSet_w");
		break;

	case EStdFunc_MulticastSet_t:
		pFunction = CreateMulticastSet (EFunctionPtrType_Thin, "jnc.MulticastSet_t");
		break;

	case EStdFunc_MulticastSet_u:
		pFunction = CreateMulticastSet (EFunctionPtrType_Unsafe, "jnc.MulticastSet_u");
		break;

	case EStdFunc_MulticastAdd:
		pFunction = CreateMulticastAdd (EFunctionPtrType_Normal, "jnc.MulticastAdd");
		break;

	case EStdFunc_MulticastAdd_w:
		pFunction = CreateMulticastAdd (EFunctionPtrType_Weak, "jnc.MulticastAdd_w");
		break;

	case EStdFunc_MulticastAdd_t:
		pFunction = CreateMulticastAdd (EFunctionPtrType_Thin, "jnc.MulticastAdd_t");
		break;

	case EStdFunc_MulticastAdd_u:
		pFunction = CreateMulticastAdd (EFunctionPtrType_Unsafe, "jnc.MulticastAdd_u");
		break;

	case EStdFunc_MulticastRemove:
		pFunction = CreateMulticastRemove (EFunctionPtrType_Normal, "jnc.MulticastRemove");
		break;

	case EStdFunc_MulticastRemove_w:
		pFunction = CreateMulticastRemove (EFunctionPtrType_Weak, "jnc.MulticastRemove_w");
		break;

	case EStdFunc_MulticastRemove_t:
		pFunction = CreateMulticastRemove (EFunctionPtrType_Thin, "jnc.MulticastRemove_t");
		break;

	case EStdFunc_MulticastRemove_u:
		pFunction = CreateMulticastRemove (EFunctionPtrType_Unsafe, "jnc.MulticastRemove_u");
		break;

	case EStdFunc_MulticastSnapshot:
		pFunction = CreateMulticastSnapshot (EFunctionPtrType_Normal, "jnc.MulticastSnapshot");
		break;

	case EStdFunc_MulticastSnapshot_w:
		pFunction = CreateMulticastSnapshot (EFunctionPtrType_Weak, "jnc.MulticastSnapshot_w");
		break;

	case EStdFunc_MulticastSnapshot_t:
		pFunction = CreateMulticastSnapshot (EFunctionPtrType_Thin, "jnc.MulticastSnapshot_t");
		break;

	case EStdFunc_MulticastSnapshot_u:
		pFunction = CreateMulticastSnapshot (EFunctionPtrType_Unsafe, "jnc.MulticastSnapshot_u");
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
//		int8* pRangeEnd,
//		int Error
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
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int)
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction ("jnc.CheckDataPtrRange", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];
	CValue ArgValue3 = ArgValueArray [2];
	CValue ArgValue4 = ArgValueArray [3];
	CValue ArgValue5 = ArgValueArray [4];

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

	m_pModule->m_LlvmBuilder.RuntimeError (ArgValue5);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);

	InternalEpilogue ();

	return pFunction;
}

// void 
// jnc.CheckClassPtrScopeLevel (
//		object p,
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

// object
// jnc.DynamicCastClassPtr (
//		object p,
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
// jnc.UHeapFreeClassPtr (object p);

CFunction*
CFunctionMgr::CreateUHeapFreeClassPtr ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction ("jnc.UHeapFree", pType);
}


// intptr
// jnc.MulticastSet (
//		multicast* pMulticast (), 
//		function* pf ()
//		);

CFunction*
CFunctionMgr::CreateMulticastSet (
	EFunctionPtrType PtrTypeKind,
	const char* pTag
	)
{
	CFunctionType* pFunctionType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionPtrType* pPtrType = pFunctionType->GetFunctionPtrType (PtrTypeKind);

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);

	CType* ArgTypeArray [] =
	{
		pPtrType->GetMulticastType ()->GetDataPtrType (EDataPtrType_Unsafe),
		pPtrType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (pTag, pType);
}

// intptr
// jnc.MulticastAdd (
//		multicast* pMulticast (), 
//		function* pf ()
//		);

CFunction*
CFunctionMgr::CreateMulticastAdd (
	EFunctionPtrType PtrTypeKind,
	const char* pTag
	)
{
	CFunctionType* pFunctionType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionPtrType* pPtrType = pFunctionType->GetFunctionPtrType (PtrTypeKind);

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);

	CType* ArgTypeArray [] =
	{
		pPtrType->GetMulticastType ()->GetDataPtrType (EDataPtrType_Unsafe),
		pPtrType
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (pTag, pType);
}

// function* pf ()
// jnc.MulticastRemove (
//		multicast* pMulticast (), 
//		intptr Handle
//		);

CFunction*
CFunctionMgr::CreateMulticastRemove (
	EFunctionPtrType PtrTypeKind,
	const char* pTag
	)
{
	CFunctionType* pFunctionType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionPtrType* pPtrType = pFunctionType->GetFunctionPtrType (PtrTypeKind);

	CType* pReturnType = pPtrType;

	CType* ArgTypeArray [] =
	{
		pPtrType->GetMulticastType ()->GetDataPtrType (EDataPtrType_Unsafe),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (pTag, pType);
}

// mcsnapshot ()
// jnc.MulticastSnapshot (multicast* pMulticast ());

CFunction*
CFunctionMgr::CreateMulticastSnapshot (
	EFunctionPtrType PtrTypeKind,
	const char* pTag
	)
{
	CFunctionType* pFunctionType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionPtrType* pPtrType = pFunctionType->GetFunctionPtrType (PtrTypeKind);

	CType* pReturnType = pPtrType->GetMcSnapshotType ();

	CType* ArgTypeArray [] =
	{
		pPtrType->GetMulticastType ()->GetDataPtrType (EDataPtrType_Unsafe),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (pTag, pType);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {