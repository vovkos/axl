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
			llvm::Value* pLlvmArg = LlvmArg;

			CVariable* pArgVariable = m_pModule->m_VariableMgr.CreateVariable (pArg->GetName (), pArg->GetType ());

			CValue ArgValue (pLlvmArg, pArg->GetType ());
			CValue ArgVariableValue (pArgVariable);

			m_pModule->m_LlvmBuilder.CreateStore (ArgValue, ArgVariableValue);
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
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);
	CValue ArgValue3 (LlvmArg++, ArgTypeArray [2]);
	CValue ArgValue4 (LlvmArg++, ArgTypeArray [3]);

	CValue EndValue;
	m_pModule->m_LlvmBuilder.CreateGep (ArgValue2, ArgValue3, NULL, &EndValue);

	CValue SafePtrValue (pReturnType->GetLlvmUndefValue (), pReturnType);
	
	rtl::CString s = pReturnType->GetTypeString ();

	m_pModule->m_LlvmBuilder.CreateInsertValue (SafePtrValue, ArgValue1, 0, NULL, &SafePtrValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (SafePtrValue, ArgValue2, 1, NULL, &SafePtrValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (SafePtrValue, EndValue, 2, NULL, &SafePtrValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (SafePtrValue, ArgValue4, 3, pReturnType, &SafePtrValue);
	m_pModule->m_LlvmBuilder.CreateRet (SafePtrValue);

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
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);
	CValue ArgValue3 (LlvmArg++, ArgTypeArray [2]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_success"));
	CBasicBlock* pCmp2Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_cmp2"));
	CBasicBlock* pCmp3Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_cmp3"));
	
	CValue NullValue (m_pModule->m_TypeMgr.GetBytePtrType (), NULL);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue1, 0, NULL, &PtrValue);

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (PtrValue, NullValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pCmp2Block);	
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pCmp2Block);

	CValue RegionBeginValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue1, 1, NULL, &RegionBeginValue);
	m_pModule->m_LlvmBuilder.CreateLt_u (PtrValue, RegionBeginValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pCmp3Block);	
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pCmp3Block);

	CValue PtrEndValue;
	CValue RegionEndValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue1, 2, NULL, &RegionEndValue);
	m_pModule->m_LlvmBuilder.CreateGep (PtrValue, ArgValue2, NULL ,&PtrEndValue);
	m_pModule->m_LlvmBuilder.CreateGt_u (PtrEndValue, RegionEndValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFailBlock);

	CFunction* pOnInvalidSafePtr = GetStdFunction (EStdFunc_OnInvalidSafePtr);
	m_pModule->m_LlvmBuilder.CreateCall2 (pOnInvalidSafePtr, ArgValue1, ArgValue3, NULL, NULL);
	m_pModule->m_LlvmBuilder.CreateBr (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSuccessBlock);

	m_pModule->m_LlvmBuilder.CreateRet ();

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
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);

	CValue ScopeValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue1, 3, NULL, &ScopeValue);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_success"));
	
	CValue CmpValue;

	m_pModule->m_LlvmBuilder.CreateLt_u (ArgValue2, ScopeValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pFailBlock, pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFailBlock);

	CValue ErrorValue;
	ErrorValue.SetConstInt32 (ESafePtrError_ScopeMismatch, EType_Int);

	CFunction* pOnInvalidSafePtr = GetStdFunction (EStdFunc_OnInvalidSafePtr);
	m_pModule->m_LlvmBuilder.CreateCall2 (pOnInvalidSafePtr, ArgValue1, ErrorValue, NULL, NULL);
	m_pModule->m_LlvmBuilder.CreateBr (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSuccessBlock);

	m_pModule->m_LlvmBuilder.CreateRet ();

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

//.............................................................................

} // namespace axl {
} // namespace jnc {
