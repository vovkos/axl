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
	const rtl::CString& Tag,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CFunction* pFunction = AXL_MEM_NEW (CFunction);
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

		size_t Count = pType->GetArgCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CFunctionFormalArg* pArg = AXL_MEM_NEW (CFunctionFormalArg);
			pArg->m_Name.Format (_T("a%d"), i + 1);
			pArg->m_pType = pType->GetArgType (i);
			pFunction->m_ArgList.InsertTail (pArg);
		}
	}

	m_FunctionList.InsertTail (pFunction);
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

	CFunction* pGetter = CreateFunction (_T("getter"), pPropertyType->GetGetterType ());
			
	CFunctionOverload Setter;

	size_t Count = pSetterType->GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pSetterTypeOverload = pSetterType->GetType (i);
		CFunction* pSetterOverload = CreateFunction (_T("setter"), pSetterTypeOverload);
		Setter.AddOverload (pSetterOverload);
	}

	return CreateProperty (pPropertyType, pGetter, Setter);
}

CGlobalFunction*
CFunctionMgr::CreateGlobalFunction (
	const rtl::CString& Name,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CFunction* pFunction = CreateFunction (Name, pType, pArgList);
	CGlobalFunction* pGlobalFunction = AXL_MEM_NEW (CGlobalFunction);
	pGlobalFunction->m_Name = Name;
	pGlobalFunction->m_pFunction = pFunction;
	pFunction->m_pOverload = pGlobalFunction;
	m_GlobalFunctionList.InsertTail (pGlobalFunction);
	return pGlobalFunction;
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

	rtl::CIteratorT <jnc::CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		jnc::CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		// set namespace

		CNamespace* pNamespace = NULL;

		EFunction FunctionKind = pFunction->GetFunctionKind ();
		switch (FunctionKind)
		{
		case EFunction_Global:
			{
			CGlobalFunction* pGlobalFunction = (CGlobalFunction*) pFunction->GetOverload ();
			pNamespace = pGlobalFunction->GetParentNamespace ();
			break;
			}

		default:
			ASSERT (false);
		}

		m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);

		// create entry block

		m_pCurrentFunction = pFunction;
		pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));
		m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

		// store arguments

		rtl::CIteratorT <CFunctionFormalArg> Arg = pFunction->GetFirstArg ();
		llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

		for (; Arg; Arg++, LlvmArg++)
		{
			CFunctionFormalArg* pArg = *Arg;
			CVariable* pArgVariable = m_pModule->m_VariableMgr.CreateVariable (pArg->GetName (), pArg->GetType ());
			m_pModule->m_LlvmBuilder.CreateStore (LlvmArg, pArgVariable->GetLlvmValue ());
			pNamespace->AddItem (pArgVariable);
		}

		// parse body

		jnc::CParser Parser;
		Parser.m_Stage = jnc::CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;
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

		// llvm::verifyFunction (pFunction->GetLlvmFunction (), llvm::PrintMessageAction);

		pFunction->m_Ast = Parser.GetAst ();
		jnc::CParser::CAstNode* pAstNode = pFunction->m_Ast->GetRoot ();

		ASSERT (pAstNode->m_Kind == jnc::ESymbol_compound_stmt);
		jnc::CParser::CCompoundStmt* pBody = (jnc::CParser::CCompoundStmt*) pAstNode;
		pFunction->m_pScope = pBody->m_pScope;
	}

	m_pModule->m_NamespaceMgr.SetGlobalNamespace (); // just in case

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
	case EStdFunc_CreateSafePtr:
		pFunction = GetCreateSafePtr ();
		break;

	case EStdFunc_CheckSafePtrRange:
		pFunction = GetCheckSafePtrRange ();
		break;

	case EStdFunc_CheckSafePtrScope:
		pFunction = GetCheckSafePtrScope ();
		break;

	case EStdFunc_OnInvalidSafePtr:
		pFunction = GetOnInvalidSafePtr ();
		break;

	case EStdFunc_LoadDynamicPtr:
		pFunction = GetLoadDynamicPtr ();
		break;

	case EStdFunc_StoreDynamicPtr:
		pFunction = GetStoreDynamicPtr ();
		break;

	default:
		ASSERT (false);
		pFunction = NULL;
	}

	m_StdFunctionArray [Func] = pFunction;
	return pFunction;
}


// jnc.sptr 
// jnc.CreateSafePtr (
//		int8* p,
//		int8* pRegionBegin,
//		size_t Size,
//		size_t ScopeLevel
//		);

CFunction*
CFunctionMgr::GetCreateSafePtr ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetSafePtrStructType ();

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetBytePtrType (),
		m_pModule->m_TypeMgr.GetBytePtrType (),
		m_pModule->m_TypeMgr.GetBasicType (EType_SizeT),
		m_pModule->m_TypeMgr.GetBasicType (EType_SizeT),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray), 0);
	CFunction* pFunction = CreateFunction (_T("jnc.CreateSafePtr"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	llvm::Value* pLlvmArg1 = LlvmArg++;
	llvm::Value* pLlvmArg2 = LlvmArg++;
	llvm::Value* pLlvmArg3 = LlvmArg++;
	llvm::Value* pLlvmArg4 = LlvmArg++;
	llvm::Value* pLlvmEnd = m_pModule->m_LlvmBuilder.CreateGEP (pLlvmArg2, pLlvmArg3, "sptr_end");

	llvm::Value* pLlvmSafePtr = pReturnType->GetLlvmUndefValue ();
	
	pLlvmSafePtr = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmSafePtr, pLlvmArg1, 0, "sptr_p");
	pLlvmSafePtr = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmSafePtr, pLlvmArg2, 1, "sptr_beg");
	pLlvmSafePtr = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmSafePtr, pLlvmEnd, 2, "sptr_end");
	pLlvmSafePtr = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmSafePtr, pLlvmArg4, 3, "sptr_scope");

	m_pModule->m_LlvmBuilder.CreateRet (pLlvmSafePtr);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void 
// jnc.CheckSafePtrRange (
//		jnc.sptr p,
//		size_t Size,
//		int Error
//		);

CFunction*
CFunctionMgr::GetCheckSafePtrRange ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetBasicType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetSafePtrStructType (),
		m_pModule->m_TypeMgr.GetBasicType (EType_SizeT),
		m_pModule->m_TypeMgr.GetBasicType (EType_Int)
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction (_T("jnc.CheckSafePtrRange"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	llvm::Value* pLlvmArg1 = LlvmArg++;
	llvm::Value* pLlvmArg2 = LlvmArg++;
	llvm::Value* pLlvmArg3 = LlvmArg++;

	llvm::Value* pLlvmPtr = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmArg1, 0, "sptr_p");
	llvm::Value* pLlvmRegionBegin = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmArg1, 1, "sptr_beg");
	llvm::Value* pLlvmRegionEnd = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmArg1, 2, "sptr_end");
	llvm::Value* pLlvmPtrEnd = m_pModule->m_LlvmBuilder.CreateGEP (pLlvmPtr, pLlvmArg2, "sptr_p_end");

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_success"));
	CBasicBlock* pCmp2Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_cmp2"));
	CBasicBlock* pCmp3Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_cmp3"));
	
	CValue Null (m_pModule->m_TypeMgr.GetBytePtrType (), NULL);

	llvm::Value* pCmp = m_pModule->m_LlvmBuilder.CreateICmpEQ (pLlvmPtr, Null.GetLlvmValue (), "eq");
	m_pModule->m_LlvmBuilder.CreateCondBr (pCmp, pFailBlock->GetLlvmBlock (), pCmp2Block->GetLlvmBlock ());
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pCmp2Block);

	pCmp = m_pModule->m_LlvmBuilder.CreateICmpULT (pLlvmPtr, pLlvmRegionBegin, "lt");
	m_pModule->m_LlvmBuilder.CreateCondBr (pCmp, pFailBlock->GetLlvmBlock (), pCmp3Block->GetLlvmBlock ());
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pCmp3Block);

	pCmp = m_pModule->m_LlvmBuilder.CreateICmpUGT (pLlvmPtrEnd, pLlvmRegionEnd, "gt");
	m_pModule->m_LlvmBuilder.CreateCondBr (pCmp, pFailBlock->GetLlvmBlock (), pSuccessBlock->GetLlvmBlock ());
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSuccessBlock);

	m_pModule->m_LlvmBuilder.CreateRetVoid ();

	CFunction* pOnInvalidSafePtr = GetStdFunction (EStdFunc_OnInvalidSafePtr);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFailBlock);
	m_pModule->m_LlvmBuilder.CreateCall2 (pOnInvalidSafePtr->GetLlvmFunction (), pLlvmArg1, pLlvmArg3);
	m_pModule->m_LlvmBuilder.CreateRetVoid ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void 
// jnc.CheckSafePtrScope (
//		jnc.sptr p,
//		size_t ScopeLevel
//		);

CFunction*
CFunctionMgr::GetCheckSafePtrScope ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetBasicType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetSafePtrStructType (),
		m_pModule->m_TypeMgr.GetBasicType (EType_SizeT)
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateFunction (_T("jnc.CheckSafePtrScope"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	llvm::Value* pLlvmArg1 = LlvmArg++;
	llvm::Value* pLlvmArg2 = LlvmArg++;

	llvm::Value* pLlvmPtrScope = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmArg1, 3, "sptr_scope");

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_success"));
	
	llvm::Value* pCmp = m_pModule->m_LlvmBuilder.CreateICmpULT (pLlvmArg2, pLlvmPtrScope, "lt");
	m_pModule->m_LlvmBuilder.CreateCondBr (pCmp, pFailBlock->GetLlvmBlock (), pSuccessBlock->GetLlvmBlock ());
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSuccessBlock);

	m_pModule->m_LlvmBuilder.CreateRetVoid ();

	CFunction* pOnInvalidSafePtr = GetStdFunction (EStdFunc_OnInvalidSafePtr);

	CValue ErrorValue;
	ErrorValue.SetConstInt32 (ESafePtrError_ScopeMismatch, EType_Int);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFailBlock);
	m_pModule->m_LlvmBuilder.CreateCall2 (pOnInvalidSafePtr->GetLlvmFunction (), pLlvmArg1, ErrorValue.GetLlvmValue ());
	m_pModule->m_LlvmBuilder.CreateRetVoid ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void
// jnc.OnInvalidSafePtr (
//		jnc.sptr p,
//		int Error
//		);

CFunction*
CFunctionMgr::GetOnInvalidSafePtr ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetBasicType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetSafePtrStructType (),
		m_pModule->m_TypeMgr.GetBasicType (EType_Int)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateFunction (_T("jnc.OnInvalidSafePtr"), pType);
}

// jnc.sptr 
// jnc.LoadDynamicPtr (jnc.dptr);

CFunction*
CFunctionMgr::GetLoadDynamicPtr ()
{
	return NULL;
}

// void
// jnc.StoreDynamicPtr (
//		int8* pSrc
//		int8* pSrcType
//		jnc.dptr pDst,
//		);

CFunction*
CFunctionMgr::GetStoreDynamicPtr ()
{
	return NULL;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
