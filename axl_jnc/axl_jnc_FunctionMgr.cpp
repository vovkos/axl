#include "stdafx.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Parser.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionMgr::CFunctionMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));

	m_pCurrentFunction = NULL;
}

void
CFunctionMgr::Clear ()
{
	m_FunctionList.Clear ();
	m_PropertyList.Clear ();
	m_GlobalFunctionList.Clear ();
	m_GlobalPropertyList.Clear ();

	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));

	m_pCurrentFunction = NULL;
}

CFunction*
CFunctionMgr::CreateFunction (
	EFunction FunctionKind,
	const rtl::CString& Tag,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CFunction* pFunction = AXL_MEM_NEW (CFunction);
	pFunction->m_FunctionKind = FunctionKind;
	pFunction->m_pModule = m_pModule;
	pFunction->m_pType = pType;
	pFunction->m_Tag = Tag;
	
	if (pArgList)
	{
		pFunction->m_ArgList.TakeOver (pArgList);
	}
	else
	{
		// create default argument names

		rtl::CArrayT <CType*> ArgTypeArray = pType->GetArgTypeArray ();
		size_t Count = ArgTypeArray.GetCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CFunctionFormalArg* pArg = AXL_MEM_NEW (CFunctionFormalArg);
			pArg->m_Name.Format (_T("a%d"), i + 1);
			pArg->m_pType = ArgTypeArray [i];
			pFunction->m_ArgList.InsertTail (pArg);
		}
	}

	m_FunctionList.InsertTail (pFunction);
	return pFunction;
}

CFunction*
CFunctionMgr::CreateGlobalFunction (
	const rtl::CString& Name,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CGlobalFunction* pGlobalFunction = AXL_MEM_NEW (CGlobalFunction);
	pGlobalFunction->m_Name = Name;
	m_GlobalFunctionList.InsertTail (pGlobalFunction);

	CFunction* pFunction = CreateFunction (EFunction_Global, pGlobalFunction->m_Name, pType, pArgList);
	pGlobalFunction->AddOverload (pFunction);
	return pFunction;
}

CProperty*
CFunctionMgr::CreateProperty (
	CPropertyType* pType,
	CFunction* pGetter,
	const CFunctionOverload& Setter
	)
{
	CProperty* pProperty = AXL_MEM_NEW (CProperty);
	pProperty->m_pModule = m_pModule;
	pProperty->m_pType = pType;
	pProperty->m_pGetter = pGetter;
	pProperty->m_Setter = Setter;
	m_PropertyList.InsertTail (pProperty);
	return pProperty;
}

CProperty*
CFunctionMgr::CreateProperty (CPropertyType* pPropertyType)
{
	CFunctionTypeOverload* pSetterType = pPropertyType->GetSetterType ();

	CFunction* pGetter = CreateFunction (EFunction_PropertyGetter, _T("getter"), pPropertyType->GetGetterType ());
			
	CFunctionOverload Setter;

	size_t Count = pSetterType->GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pSetterTypeOverload = pSetterType->GetType (i);
		CFunction* pSetterOverload = CreateFunction (EFunction_PropertySetter, _T("setter"), pSetterTypeOverload);
		Setter.AddOverload (pSetterOverload);
	}

	return CreateProperty (pPropertyType, pGetter, Setter);
}

CGlobalProperty*
CFunctionMgr::CreateGlobalProperty (
	const rtl::CString& Name,
	CPropertyType* pType,
	CFunction* pGetter,
	const CFunctionOverload& Setter
	)
{
	CProperty* pProperty = CreateProperty (pType, pGetter, Setter);
	return CreateGlobalProperty (Name, pProperty);
}

CGlobalProperty*
CFunctionMgr::CreateGlobalProperty (
	const rtl::CString& Name,
	CProperty* pProperty
	)
{
	CGlobalProperty* pGlobalProperty = AXL_MEM_NEW (CGlobalProperty);
	pGlobalProperty->m_Name = Name;
	pGlobalProperty->m_pProperty = pProperty;
	m_GlobalPropertyList.InsertTail (pGlobalProperty);
	return pGlobalProperty;
}

bool
CFunctionMgr::CompileFunctions ()
{
	bool Result;

	CSetCurrentThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	rtl::CIteratorT <jnc::CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		jnc::CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		// set namespace

		CNamespace* pNamespace = NULL;
		CProperty* pProperty = NULL;
		CClassType* pThisType = NULL;
		CClassType* pOriginType = NULL;

		EFunction FunctionKind = pFunction->GetFunctionKind ();
		switch (FunctionKind)
		{
		case EFunction_Global:
			pNamespace = pFunction->GetGlobalFunction ()->GetParentNamespace ();
			break;

		case EFunction_Method:
			pNamespace = pFunction->GetClassMethodMember()->GetParentNamespace ();
			pThisType = pFunction->GetClassMethodMember()->GetParentType ();
			pOriginType = pFunction->GetOriginClassType ();
			break;

		case EFunction_PropertyGetter:
		case EFunction_PropertySetter:
			pProperty = pFunction->GetProperty ();
			pNamespace = m_pModule->m_NamespaceMgr.GetGlobalNamespace ();
			break;

		default:
			ASSERT (false);
		}

		m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);

		// create entry block and scope

		m_pCurrentFunction = pFunction;
		pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));
		m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);
		CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (pFunction->GetBodyFirstToken ()->m_Pos);

		// store arguments

		llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

		CValue ThisValue;
		if (FunctionKind == EFunction_Method)
		{
			llvm::Value* pLlvmArg = LlvmArg;
			CValue ArgValue (pLlvmArg, pOriginType);			
			Result = CreateThisValue (ArgValue, pThisType, &ThisValue);
			if (!Result)
				return false;

			LlvmArg++;
		}

		rtl::CIteratorT <CFunctionFormalArg> Arg = pFunction->GetFirstArg ();
		for (; Arg; Arg++, LlvmArg++)
		{
			CFunctionFormalArg* pArg = *Arg;
			llvm::Value* pLlvmArg = LlvmArg;

			CVariable* pArgVariable = m_pModule->m_VariableMgr.CreateVariable (pArg->GetName (), pArg->GetType ());

			CValue ArgValue (pLlvmArg, pArg->GetType ());
			CValue ArgVariableValue (pArgVariable);

			m_pModule->m_LlvmBuilder.CreateStore (ArgValue, ArgVariableValue);
			pScope->AddItem (pArgVariable);
		}

		// parse body

		jnc::CParser Parser;
		Parser.m_Stage = jnc::CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;
		Parser.m_ThisValue = ThisValue;
		Parser.m_pThisType = pThisType;
		Parser.Create (jnc::ESymbol_compound_stmt, true); 
			
		rtl::CBoxIteratorT <jnc::CToken> Token = pFunction->GetBodyFirstToken ();
		for (; Token; Token++)
		{
			Result = Parser.ParseToken (&*Token);
			if (!Result)
			{
				err::PushSrcPosError (m_pModule->GetFilePath (), Token->m_Pos.m_Line, Token->m_Pos.m_Col);
				return false;
			}
		}

		m_pModule->m_NamespaceMgr.CloseScope (pFunction->GetBodyLastToken ()->m_Pos);

		pFunction->m_Ast = Parser.GetAst ();
		pFunction->m_pScope = pScope;

		try 
		{
			llvm::verifyFunction (*pFunction->GetLlvmFunction (), llvm::ReturnStatusAction);
		}
		catch (err::CError Error)
		{
			err::SetFormatStringError (
				_T("LLVM verification fail for '%s': %s"), 
				pFunction->GetTag (),
				Error->GetDescription ()
				);

			return false;
		}
	}

	m_pModule->m_NamespaceMgr.SetGlobalNamespace (); // just in case

	return true;
}

bool
CFunctionMgr::JitFunctions (llvm::ExecutionEngine* pExecutionEngine)
{
	CSetCurrentThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	rtl::CIteratorT <jnc::CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		jnc::CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		try 
		{
			pFunction->m_pfn = pExecutionEngine->getPointerToFunction (pFunction->GetLlvmFunction ());
		}
		catch (err::CError Error)
		{
			err::SetFormatStringError (
				_T("LLVM jitting fail for '%s': %s"), 
				pFunction->GetTag (),
				Error->GetDescription ()
				);

			return false;
		}

		ASSERT (pFunction->m_pfn);
	}

	return true;
}

CFunction*
CFunctionMgr::GetStdFunction (EStdFunc Func)
{
	ASSERT (Func >= 0 && Func < EStdFunc__Count);

	if (m_StdFunctionArray [Func])
		return m_StdFunctionArray [Func];

	CFunction* pFunction;

	switch (Func)
	{
	case EStdFunc_CreateSafePtrValidator:
		pFunction = CreateCreateSafePtrValidator ();
		break;

	case EStdFunc_CheckSafePtrRange:
		pFunction = CreateCheckSafePtrRange ();
		break;

	case EStdFunc_CheckSafePtrScope:
		pFunction = CreateCheckSafePtrScope ();
		break;

	case EStdFunc_OnInvalidSafePtr:
		pFunction = CreateOnInvalidSafePtr ();
		break;

	case EStdFunc_CheckInterfaceNull:
		pFunction = CreateCheckInterfaceNull ();
		break;

	case EStdFunc_CheckInterfaceScope:
		pFunction = CreateCheckInterfaceScope ();
		break;

	case EStdFunc_OnInvalidInterface:
		pFunction = CreateOnInvalidInterface ();
		break;

	case EStdFunc_DynamicCastInterface:
		pFunction = CreateDynamicCastInterface ();
		break;

	case EStdFunc_HeapAllocate:
		pFunction = CreateHeapAllocate ();
		break;

	default:
		ASSERT (false);
		pFunction = NULL;
	}

	m_StdFunctionArray [Func] = pFunction;
	return pFunction;
}

// jnc.sptrv
// jnc.CreateSafePtrValidator (
//		int8* pRegionBegin,
//		size_t Size,
//		size_t ScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCreateSafePtrValidator ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_SafePtrValidator);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray), 0);
	CFunction* pFunction = CreateFunction (EFunction_Global, _T("jnc.CreateSafePtrValidator"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);
	CValue ArgValue3 (LlvmArg++, ArgTypeArray [2]);

	CValue RegionEndValue;
	m_pModule->m_LlvmBuilder.CreateGep (ArgValue1, ArgValue2, NULL, &RegionEndValue);

	CValue ValidatorValue = pReturnType->GetUndefValue ();
	
	rtl::CString s = pReturnType->GetTypeString ();

	m_pModule->m_LlvmBuilder.CreateInsertValue (ValidatorValue, ArgValue1, 0, NULL, &ValidatorValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (ValidatorValue, RegionEndValue, 1, NULL, &ValidatorValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (ValidatorValue, ArgValue3, 2, pReturnType, &ValidatorValue);
	m_pModule->m_LlvmBuilder.CreateRet (ValidatorValue);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void 
// jnc.CheckSafePtrRange (
//		int8* p,
//		size_t Size,
//		jnc.sptrv Validator,
//		int Error
//		);

CFunction*
CFunctionMgr::CreateCheckSafePtrRange ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetStdType (EStdType_SafePtrValidator),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int)
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction (EFunction_Global, _T("jnc.CheckSafePtrRange"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);
	CValue ArgValue3 (LlvmArg++, ArgTypeArray [2]);
	CValue ArgValue4 (LlvmArg++, ArgTypeArray [3]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_success"));
	CBasicBlock* pCmp2Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_cmp2"));
	CBasicBlock* pCmp3Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_cmp3"));
	
	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pCmp2Block);	
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pCmp2Block);

	CValue RegionBeginValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue3, 0, NULL, &RegionBeginValue);
	m_pModule->m_LlvmBuilder.CreateLt_u (ArgValue1, RegionBeginValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pCmp3Block);	
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pCmp3Block);

	CValue PtrEndValue;
	CValue RegionEndValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue3, 1, NULL, &RegionEndValue);
	m_pModule->m_LlvmBuilder.CreateGep (ArgValue1, ArgValue2, NULL ,&PtrEndValue);
	m_pModule->m_LlvmBuilder.CreateGt_u (PtrEndValue, RegionEndValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFailBlock);

	CFunction* pOnInvalidSafePtr = GetStdFunction (EStdFunc_OnInvalidSafePtr);
	m_pModule->m_LlvmBuilder.CreateCall3 (pOnInvalidSafePtr, ArgValue1, ArgValue3, ArgValue4, NULL, NULL);
	m_pModule->m_LlvmBuilder.CreateBr (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSuccessBlock);

	m_pModule->m_LlvmBuilder.CreateRet ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void 
// jnc.CheckSafePtrScope (
//		int8* p,
//		jnc.sptrv Validator,
//		size_t DstScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCheckSafePtrScope ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_SafePtrValidator),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction (EFunction_Global, _T("jnc.CheckSafePtrScope"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);
	CValue ArgValue3 (LlvmArg++, ArgTypeArray [2]);

	CValue ScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue2, 2, NULL, &ScopeLevelValue);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_success"));
	
	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateLt_u (ArgValue3, ScopeLevelValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFailBlock);

	CValue ErrorValue (ERuntimeError_ScopeMismatch, EType_Int);
	CFunction* pOnInvalidSafePtr = GetStdFunction (EStdFunc_OnInvalidSafePtr);
	m_pModule->m_LlvmBuilder.CreateCall3 (pOnInvalidSafePtr, ArgValue1, ArgValue2, ErrorValue, NULL, NULL);
	m_pModule->m_LlvmBuilder.CreateBr (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSuccessBlock);

	m_pModule->m_LlvmBuilder.CreateRet ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void
// jnc.OnInvalidSafePtr (
//		int8* p,
//		jnc.sptrv Validator,
//		int Error
//		);

CFunction*
CFunctionMgr::CreateOnInvalidSafePtr ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_SafePtrValidator),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateFunction (EFunction_Global, _T("jnc.OnInvalidSafePtr"), pType);
}

// void 
// jnc.CheckInterfaceNull (int8* p);

CFunction*
CFunctionMgr::CreateCheckInterfaceNull ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction (EFunction_Global, _T("jnc.CheckInterfaceNull"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_success"));
	
	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pSuccessBlock);	
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFailBlock);

	CValue ErrorValue (ERuntimeError_NullInterface, EType_Int);
	CFunction* pOnInvalidInterface = GetStdFunction (EStdFunc_OnInvalidInterface);
	m_pModule->m_LlvmBuilder.CreateCall2 (pOnInvalidInterface, ArgValue1, ErrorValue, NULL, NULL);
	m_pModule->m_LlvmBuilder.CreateBr (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSuccessBlock);

	m_pModule->m_LlvmBuilder.CreateRet ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void 
// jnc.CheckInterfaceScope (
//		int8* p,
//		size_t ScopeLevel,
//		size_t DstScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCheckInterfaceScope ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction (EFunction_Global, _T("jnc.CheckInterfaceScope"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);
	CValue ArgValue3 (LlvmArg++, ArgTypeArray [2]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_success"));
	CBasicBlock* pCmp2Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_cmp2"));
	
	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pSuccessBlock, pCmp2Block);	
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pCmp2Block);

	m_pModule->m_LlvmBuilder.CreateLt_u (ArgValue3, ArgValue2, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFailBlock);

	CValue ErrorValue (ERuntimeError_ScopeMismatch, EType_Int);
	CFunction* pOnInvalidInterface = GetStdFunction (EStdFunc_OnInvalidInterface);
	m_pModule->m_LlvmBuilder.CreateCall2 (pOnInvalidInterface, ArgValue1, ErrorValue, NULL, NULL);
	m_pModule->m_LlvmBuilder.CreateBr (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSuccessBlock);

	m_pModule->m_LlvmBuilder.CreateRet ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void
// jnc.OnInvalidInterface (
//		int8* p,
//		int Error
//		);

CFunction*
CFunctionMgr::CreateOnInvalidInterface ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateFunction (EFunction_Global, _T("jnc.OnInvalidInterface"), pType);
}

// int8*
// jnc.DynamicCastInterface (
//		int8* p,
//		int8* pType
//		);

CFunction*
CFunctionMgr::CreateDynamicCastInterface ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateFunction (EFunction_Global, _T("jnc.DynamicCastInterface"), pType);
}

// int8*
// jnc.HeapAllocate (int8* pType);

CFunction*
CFunctionMgr::CreateHeapAllocate ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateFunction (EFunction_Global, _T("jnc.HeapAllocate"), pType);
}

CFunction* 
CFunctionMgr::CreateClassInitializer (CClassType* pClassType)
{
	CValue VTablePtrValue;
	bool Result = pClassType->GetVTablePtrValue (&VTablePtrValue);
	if (!Result)
		return NULL;

	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		pClassType->GetClassStructType ()->GetPointerType (EType_Pointer_u),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int)
	};

	rtl::CString Tag;
	Tag = pClassType->GetQualifiedName ();
	Tag.Append (_T(".$init"));

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction (EFunction_Global, Tag, pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);

	CValue TypeValue (&pClassType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));
	CValue ObjectPtrValue;
	CValue IfacePtrValue;
	CValue PtrValue;

	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 0, NULL, &ObjectPtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 1, NULL, &IfacePtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue2, PtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 1, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (TypeValue, PtrValue);

	InitializeInterface (pClassType, ObjectPtrValue, IfacePtrValue, VTablePtrValue);

	m_pModule->m_LlvmBuilder.CreateRet ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

bool
CFunctionMgr::InitializeInterface (
	CClassType* pClassType,
	const CValue& ObjectPtrValue,
	const CValue& IfacePtrValue,
	const CValue& VTablePtrValue
	)
{
	CValue IfaceHdrPtrValue;
	CValue PtrValue;

	// TInterfaceHdr

	m_pModule->m_LlvmBuilder.CreateGep2 (IfacePtrValue, 0, NULL, &IfaceHdrPtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (IfaceHdrPtrValue, 0, NULL, &PtrValue);

	rtl::CString s0 = GetLlvmTypeString (IfacePtrValue.GetLlvmValue ()->getType ());
	rtl::CString s1 = GetLlvmTypeString (IfaceHdrPtrValue.GetLlvmValue ()->getType ());
	rtl::CString s2 = GetLlvmTypeString (PtrValue.GetLlvmValue ()->getType ());
	rtl::CString s3 = GetLlvmTypeString (VTablePtrValue.GetLlvmValue ()->getType ());

	m_pModule->m_LlvmBuilder.CreateStore (VTablePtrValue, PtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (IfaceHdrPtrValue, 1, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ObjectPtrValue, PtrValue);

	// base types

	rtl::CIteratorT <CClassBaseType> BaseType = pClassType->GetFirstBaseType ();
	for (; BaseType; BaseType++)
	{
		CClassType* pBaseClassType = BaseType->GetType ();
		CValue BaseClassPtrValue;
		CValue BaseClassVTablePtrValue;

		CPointerType* pBaseClassVTablePtrType = pBaseClassType->GetVTableStructType ()->GetPointerType (EType_Pointer_u);

		m_pModule->m_LlvmBuilder.CreateGep2 (IfacePtrValue, BaseType->GetLlvmIndex (), NULL, &BaseClassPtrValue);
		m_pModule->m_LlvmBuilder.CreateGep2 (VTablePtrValue, BaseType->GetVTableIndex (), NULL, &BaseClassVTablePtrValue);
		m_pModule->m_LlvmBuilder.CreateBitCast (BaseClassVTablePtrValue, pBaseClassVTablePtrType, &BaseClassVTablePtrValue);

		InitializeInterface (pBaseClassType, ObjectPtrValue, BaseClassPtrValue, BaseClassVTablePtrValue);
	}
	
	return true;
}

bool
CFunctionMgr::CreateThisValue (
	const CValue& ArgValue,
	CClassType* pResultType,
	CValue* pResultValue
	)
{
	CClassType* pSrcType = (CClassType*) ArgValue.GetType ();
	ASSERT (pSrcType->IsClassType ());

	CValue PtrValue;
	CValue ScopeLevelValue;

	CPointerType* pSrcPointerType = pSrcType->GetInterfaceStructType ()->GetPointerType (EType_Pointer_u);
	CPointerType* pDstPointerType = pResultType->GetInterfaceStructType ()->GetPointerType (EType_Pointer_u);

	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue, 0, pSrcPointerType, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue, 1, NULL, &ScopeLevelValue);

	bool Result = m_pModule->m_OperatorMgr.CastOperator (&PtrValue, pDstPointerType);
	if (!Result)
		return false;

	CValue ZeroValue ((int64_t) 0, EType_SizeT);
	CValue OneValue (1, EType_SizeT);

	CBasicBlock* pCmpBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	CBasicBlock* pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("this_phi"));
	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("this_scope_new"));

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (ScopeLevelValue, ZeroValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pPhiBlock, pNoNullBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pNoNullBlock);
	m_pModule->m_LlvmBuilder.CreateBr (pPhiBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPhiBlock);

	CValue PhiValue;
	CValue ThisValue;
	m_pModule->m_LlvmBuilder.CreatePhi (OneValue, pNoNullBlock, ZeroValue, pCmpBlock, &PhiValue);
	m_pModule->m_LlvmBuilder.CreateInterface (PtrValue, PhiValue, pResultType, pResultValue);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

