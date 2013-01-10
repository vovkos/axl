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
	m_GlobalFunctionList.Clear ();
	m_GlobalAutoEvTypeArray.Clear ();
	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));
	m_pCurrentFunction = NULL;
}

CFunction*
CFunctionMgr::CreateAnonimousFunction (CFunctionType* pType)
{
	CFunction* pFunction = AXL_MEM_NEW (CFunction);
	pFunction->m_pModule = m_pModule;
	pFunction->m_pType = pType;
	m_FunctionList.InsertTail (pFunction);
	return pFunction;
}

CFunction*
CFunctionMgr::CreatePropertyAccessorFunction (
	EPropertyAccessor AccessorKind,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	CFunction* pFunction = CreateAnonimousFunction (pType);
	pFunction->m_pAnchorNamespace = pNamespace;
	pFunction->m_Tag = GetPropertyAccessorString (AccessorKind);
	pFunction->m_PropertyAccessorKind = AccessorKind;

	if (pNamespace->GetNamespaceKind () != ENamespace_Class)
		pFunction->m_FunctionKind = EFunction_Global;

	if (pArgList)
		pFunction->m_ArgList.TakeOver (pArgList);

	return pFunction;
}

CFunction*
CFunctionMgr::CreateFunction (
	const CQualifiedName& Name,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	CFunction* pFunction = AXL_MEM_NEW (CFunction);
	pFunction->m_pModule = m_pModule;
	pFunction->m_pType = pType;
	pFunction->m_pAnchorNamespace = pNamespace;
	pFunction->m_Name = Name;
	pFunction->m_Tag = pNamespace->CreateQualifiedName (Name);

	if (Name.IsSimple ())
		pFunction->m_pNamespace = pNamespace;

	if (pArgList)
		pFunction->m_ArgList.TakeOver (pArgList);

	m_FunctionList.InsertTail (pFunction);
	return pFunction;
}

CFunction*
CFunctionMgr::CreateInternalFunction (
	const rtl::CString& Name,
	CFunctionType* pType
	)
{
	CFunction* pFunction = CreateAnonimousFunction (pType);
	pFunction->m_FunctionKind = EFunction_Global;
	pFunction->m_Tag = Name;
	return pFunction;
}

CGlobalFunction*
CFunctionMgr::CreateGlobalFunction (
	const rtl::CString& Name,
	CFunction* pFunction
	)
{
	CGlobalFunction* pGlobalFunction = AXL_MEM_NEW (CGlobalFunction);
	pGlobalFunction->m_Name = Name;
	m_GlobalFunctionList.InsertTail (pGlobalFunction);

	pFunction->m_FunctionKind = EFunction_Global;
	pGlobalFunction->AddOverload (pFunction);
	return pGlobalFunction;
}

bool
CFunctionMgr::ResolveOrphanFunction (CFunction* pFunction)
{
	ASSERT (pFunction->m_FunctionKind == EFunction_Undefined);

	if (pFunction->m_pAnchorNamespace->GetNamespaceKind () == ENamespace_Class) 
	{		
		// should have been resolved during class layout
		err::SetFormatStringError (_T("unresolved orphan class member '%s'"), pFunction->m_Tag);
		return false;
	}

	CModuleItem* pItem = pFunction->m_pAnchorNamespace->FindItemTraverse (pFunction->m_Name, true);
	if (!pItem)
	{
		err::SetFormatStringError (_T("unresolved orphan function '%s'"), pFunction->m_Tag);
		return false;
	}

	EModuleItem ItemKind = pItem->GetItemKind ();
	CFunction* pExternFunction = NULL;

	if (pFunction->m_PropertyAccessorKind)
	{
		CPropertyType* pPropertyType = NULL;

		switch (ItemKind)
		{
		case EModuleItem_Property:
			pPropertyType = (CPropertyType*) pItem;
			pFunction->m_FunctionKind = EFunction_Global;
			pFunction->m_pNamespace = pPropertyType->GetParentNamespace ();
			break;

		case EModuleItem_ClassMember:			
			CClassMember* pMember = (CClassMember*) pItem;
			EClassMember MemberKind = pMember->GetMemberKind ();
			
			if (MemberKind == EClassMember_Property)
			{
				pPropertyType = ((CClassPropertyMember*) pItem)->GetType ();
				pFunction->m_FunctionKind = EFunction_Method;
				pFunction->m_pNamespace = pMember->GetParentType ();
			}
		};			

		if (pPropertyType)
			pExternFunction = pFunction->m_PropertyAccessorKind == EPropertyAccessor_Set ? 
				pPropertyType->GetSetter ()->FindOverload (pFunction->GetType ()) :
				pPropertyType->GetGetter ()->GetType ()->Cmp (pFunction->GetType ()) == 0 ?
				pPropertyType->GetGetter () : NULL;
	}
	else
	{
		CFunctionOverload* pFunctionOverload = NULL;
		CGlobalFunction* pGlobalFunction;

		switch (ItemKind)
		{
		case EModuleItem_GlobalFunction:
			pGlobalFunction = (CGlobalFunction*) pItem;
			pFunctionOverload = pGlobalFunction;
			pFunction->m_FunctionKind = EFunction_Global;
			pFunction->m_pNamespace = pGlobalFunction->GetParentNamespace ();
			break;

		case EModuleItem_ClassMember:			
			CClassMember* pMember = (CClassMember*) pItem;
			EClassMember MemberKind = pMember->GetMemberKind ();

			if (MemberKind == EClassMember_Method ||
				MemberKind == EClassMember_Constructor ||
				MemberKind == EClassMember_Destructor)
			{
				pFunctionOverload = (CClassMethodMember*) pMember;
				pFunction->m_FunctionKind = EFunction_Method;
				pFunction->m_pNamespace = pMember->GetParentType ();
			}
		};			

		if (pFunctionOverload)
			pExternFunction = pFunctionOverload->FindOverload (pFunction->GetType ());
	}

	if (!pExternFunction)
	{
		err::SetFormatStringError (_T("type mismatch resolving orphan function '%s'"), pFunction->m_Tag);
		return false;
	}

	if (pExternFunction->HasBody ())
	{
		err::SetFormatStringError (_T("'%s' already has body"), pExternFunction->m_Tag);
		return false;
	}

	pExternFunction->m_pExternFunction = pFunction;
	return true;
}

bool
CFunctionMgr::CompileGlobalAutoEv ()
{
	bool Result;

	size_t Count = m_GlobalAutoEvTypeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CClassType* pType = m_GlobalAutoEvTypeArray [i];

		CNamespace* pNamespace = pType->GetParentNamespace ();
		m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);

		CParser Parser;
		Parser.m_Stage = CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;
		// Parser.m_ThisValue = ThisValue; 
		
		Parser.Create (ESymbol_autoev_body, true); 
		
		CParser::CSymbolNode_autoev_body*  pSymbol = (CParser::CSymbolNode_autoev_body*) Parser.GetPredictionTop ();
		pSymbol->m_Arg.pType = pType;
			
		rtl::CBoxIteratorT <CToken> Token = pType->GetAutoEvBody ()->GetHead ();
		for (; Token; Token++)
		{
			Result = Parser.ParseToken (&*Token);
			if (!Result)
			{
				err::PushSrcPosError (m_pModule->GetFilePath (), Token->m_Pos.m_Line, Token->m_Pos.m_Col);
				return false;
			}
		}
	}

	m_pModule->m_NamespaceMgr.SetGlobalNamespace (); // just in case

	return true;
}

bool
CFunctionMgr::CompileFunctions ()
{
	bool Result;

	CSetCurrentThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		// set namespace

		CNamespace* pNamespace = pFunction->GetNamespace ();
		m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);

		// resolve orphans

		EFunction FunctionKind = pFunction->GetFunctionKind ();
		if (FunctionKind == EFunction_Undefined)
		{
			Result = ResolveOrphanFunction (pFunction);
			if (!Result)
				return false;

			FunctionKind = pFunction->GetFunctionKind ();
		}

		// prologue

		CValue ThisValue;
		Result = Prologue (pFunction, pFunction->GetBody ()->GetHead ()->m_Pos, &ThisValue);
		if (!Result)
			return false;

		// parse body

		CParser Parser;
		Parser.m_Stage = CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;
		Parser.m_ThisValue = ThisValue;
		Parser.Create (ESymbol_compound_stmt, true); 
			
		rtl::CBoxIteratorT <CToken> Token = pFunction->GetBody ()->GetHead ();
		for (; Token; Token++)
		{
			Result = Parser.ParseToken (&*Token);
			if (!Result)
			{
				err::PushSrcPosError (m_pModule->GetFilePath (), Token->m_Pos.m_Line, Token->m_Pos.m_Col);
				return false;
			}
		}

		pFunction->m_Ast = Parser.GetAst ();

		// epilogue

		Result = Epilogue (pFunction->GetBody ()->GetTail ()->m_Pos);
		if (!Result)
			return false;
	}

	m_pModule->m_NamespaceMgr.SetGlobalNamespace (); // just in case

	return true;
}

bool
CFunctionMgr::Prologue (
	CFunction* pFunction,
	const CToken::CPos& Pos,
	CValue* pThisValue
	)
{
	bool Result;

	m_pCurrentFunction = pFunction;

	// get this-value for methods

	CClassType* pThisType = NULL;
	CClassType* pOriginType = NULL;

	EFunction FunctionKind = pFunction->GetFunctionKind ();
	switch (FunctionKind)
	{
	case EFunction_Global:
		break;

	case EFunction_Method:
		pThisType = pFunction->GetClassType ();
		pOriginType = pFunction->GetVTableClassType ();
		if (!pOriginType)
			pOriginType = pThisType;
		break;

	case EFunction_PreConstructor:
	case EFunction_Constructor:
	case EFunction_Destructor:
		pThisType = pFunction->GetClassType ();
		pOriginType = pThisType;
		break;

	default:
		ASSERT (false);
	}

	// create entry block and scope

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos);
	pFunction->m_pScope = pScope;

	// store arguments

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

	if (pThisType)
	{
		llvm::Value* pLlvmArg = LlvmArg;
		CValue ArgValue (pLlvmArg, pOriginType);			
		Result = CreateThisValue (ArgValue, pThisType, pThisValue);
		if (!Result)
			return false;

		LlvmArg++;
	}

	rtl::CIteratorT <CFunctionFormalArg> Arg = pFunction->GetFirstArg ();
	for (; Arg; Arg++, LlvmArg++)
	{
		CFunctionFormalArg* pArg = *Arg;
		llvm::Value* pLlvmArg = LlvmArg;

		CVariable* pArgVariable = m_pModule->m_VariableMgr.CreateVariable (
			pArg->GetName (), 
			pArg->GetType (), 
			true
			);

		CValue ArgValue (pLlvmArg, pArg->GetType ());

		m_pModule->m_LlvmBuilder.CreateStore (ArgValue, pArgVariable);
		pScope->AddItem (pArgVariable);
	}

	// store scope level

	pFunction->m_pScopeLevelVariable = m_pModule->m_VariableMgr.CreateVariable (
		_T("ScopeLevel"), 
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), 
		true
		);

	CValue ScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateLoad (m_pModule->m_VariableMgr.GetScopeLevelVariable (), NULL, &ScopeLevelValue);
	m_pModule->m_LlvmBuilder.CreateStore (ScopeLevelValue, pFunction->m_pScopeLevelVariable);

	return true;
}

bool
CFunctionMgr::Epilogue (const CToken::CPos& Pos)
{
	ASSERT (m_pCurrentFunction);
	CFunction* pFunction = m_pCurrentFunction;

	// ensure return

	CType* pReturnType = pFunction->GetType ()->GetReturnType ();

	if (pReturnType->GetTypeKind () != EType_Void)
	{
		EHasReturn HasReturn = pFunction->HasReturn ();

		switch (HasReturn)
		{
		case EHasReturn_None:
			err::SetFormatStringError (
				_T("function '%s' must return a '%s' value"),
				pFunction->m_Tag,
				pReturnType->GetTypeString ()
				);
			return false;

		case EHasReturn_Some:
			err::SetFormatStringError (
				_T("not all control paths in function '%s' return a value"),
				pFunction->m_Tag
				);
			return false;
		}

		if (!m_pModule->m_ControlFlowMgr.GetCurrentBlock ()->IsTerminal ())
			m_pModule->m_ControlFlowMgr.Return (pReturnType->GetZeroValue ());
	}
	else if (!m_pModule->m_ControlFlowMgr.GetCurrentBlock ()->IsTerminal ())
	{
		m_pModule->m_ControlFlowMgr.Return ();
	}

	m_pModule->m_NamespaceMgr.CloseScope (Pos);

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

	m_pCurrentFunction = NULL;
	return true;
}

bool
CFunctionMgr::JitFunctions (llvm::ExecutionEngine* pExecutionEngine)
{
/*	err::SetFormatStringError (_T("cancel LLVM jitting for now"));
	return false;
	*/
	CSetCurrentThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;

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
	case EStdFunc_OnRuntimeError:
		pFunction = CreateOnRuntimeError ();
		break;

	case EStdFunc_CheckNullPtr:
		pFunction = CreateCheckNullPtr ();
		break;

	case EStdFunc_CheckScopeLevel:
		pFunction = CreateCheckScopeLevel ();
		break;

	case EStdFunc_CheckSafePtrRange:
		pFunction = CreateCheckSafePtrRange ();
		break;

	case EStdFunc_CheckInterfaceScopeLevel:
		pFunction = CreateCheckInterfaceScopeLevel ();
		break;

	case EStdFunc_DynamicCastInterface:
		pFunction = CreateDynamicCastInterface ();
		break;

	case EStdFunc_EventOperator:
		pFunction = CreateEventOperator ();
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


// void
// jnc.OnRuntimeError (
//		int Error,
//		int8* pCodeAddr,
//		int8* pDataAddr
//		);

CFunction*
CFunctionMgr::CreateOnRuntimeError ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (_T("jnc.OnRuntimeError"), pType);
}

// void 
// jnc.CheckNullPtr (
//		int8* p,
//		int Error
//		);

CFunction*
CFunctionMgr::CreateCheckNullPtr ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (_T("jnc.CheckNullPtr"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [0]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_success"));
	
	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	RuntimeError (ArgValue2, ArgValue1);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}


// void 
// jnc.CheckSafePtrScopeLevel (
//		size_t SrcScopeLevel
//		size_t DstScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCheckScopeLevel ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (_T("jnc.CheckScopeLevel"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_success"));
	
	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateGt_u (ArgValue1, ArgValue2, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();
	RuntimeError (ERuntimeError_ScopeMismatch, NullValue);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.Return ();

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
	CFunction* pFunction = CreateInternalFunction (_T("jnc.CheckSafePtrRange"), pType);

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
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pCmp2Block, pCmp2Block);

	CValue RegionBeginValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue3, 0, NULL, &RegionBeginValue);
	m_pModule->m_LlvmBuilder.CreateLt_u (ArgValue1, RegionBeginValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pCmp3Block, pCmp3Block);	

	CValue PtrEndValue;
	CValue RegionEndValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue3, 1, NULL, &RegionEndValue);
	m_pModule->m_LlvmBuilder.CreateGep (ArgValue1, ArgValue2, NULL ,&PtrEndValue);
	m_pModule->m_LlvmBuilder.CreateGt_u (PtrEndValue, RegionEndValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	RuntimeError (ArgValue4, ArgValue1);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void 
// jnc.CheckInterfaceScopeLevel (
//		jnc.iface* p,
//		size_t DstScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCheckInterfaceScopeLevel ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (_T("jnc.CheckInterfaceScopeLevel"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);

	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_nonull"));
	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_success"));

	CValue CmpValue;

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr)->GetZeroValue ();

	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pSuccessBlock, pNoNullBlock, pNoNullBlock);

	CValue ObjPtrValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 1, NULL, &ObjPtrValue); // TObject** ppObject
	m_pModule->m_LlvmBuilder.CreateLoad (ObjPtrValue, NULL, &ObjPtrValue);  // TObject* pObject
	
	CValue SrcScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, NULL, &SrcScopeLevelValue);     // size_t* pScopeLevel
	m_pModule->m_LlvmBuilder.CreateLoad (SrcScopeLevelValue, NULL, &SrcScopeLevelValue); // size_t ScopeLevel

	m_pModule->m_LlvmBuilder.CreateGt_u (SrcScopeLevelValue, ArgValue2, &CmpValue); // SrcScopeLevel > DstScopeLevel
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();
	RuntimeError (ERuntimeError_ScopeMismatch, NullValue);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// jnc.iface*
// jnc.DynamicCastInterface (
//		jnc.iface* p,
//		int8* pType
//		);

CFunction*
CFunctionMgr::CreateDynamicCastInterface ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (_T("jnc.DynamicCastInterface"), pType);
}

// int8*
// jnc.EventOperator (
//		jnc.event* pEvent,
//		void* pfn,
//		int CallConv,
//		jnc.iface* pIface,
//		int OpKind
//		);

CFunction*
CFunctionMgr::CreateEventOperator ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleEvent)->GetPointerType (EType_Pointer_u),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
		m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (_T("jnc.EventOperator"), pType);
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
	return CreateInternalFunction (_T("jnc.HeapAllocate"), pType);
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
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT)
	};

	rtl::CString Tag;
	Tag = pClassType->GetQualifiedName ();
	Tag.Append (_T(".$init"));

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (Tag, pType);

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

	// store CClassType*

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (TypeValue, PtrValue);

	// store ScopeLevel

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 1, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue2, PtrValue);

	InitializeInterface (pClassType, ObjectPtrValue, IfacePtrValue, VTablePtrValue);

	CFunction* pPreConstructor = pClassType->GetPreConstructor ();
	if (pPreConstructor)
		m_pModule->m_LlvmBuilder.CreateCall (pPreConstructor, pPreConstructor->GetType (), IfacePtrValue, NULL);

	m_pModule->m_ControlFlowMgr.Return ();

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

		m_pModule->m_LlvmBuilder.CreateGep2 (
			IfacePtrValue, 
			BaseType->GetFieldBaseType ()->GetLlvmIndex (), 
			NULL, 
			&BaseClassPtrValue
			);

		m_pModule->m_LlvmBuilder.CreateGep2 (
			VTablePtrValue, 
			BaseType->GetVTableIndex (), 
			NULL, 
			&BaseClassVTablePtrValue
			);

		m_pModule->m_LlvmBuilder.CreateBitCast (
			BaseClassVTablePtrValue, 
			pBaseClassType->GetVTableStructType ()->GetPointerType (EType_Pointer_u),
			&BaseClassVTablePtrValue
			);

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

	// adjust the pointer: argument is the pointer to the interface which DECLARES the method
	// what we want as 'this' value is the pointer to the class which IMPLEMENTS the method

	CClassBaseTypeCoord Coord;
	intptr_t Offset = 0;

	if (pResultType != pSrcType)
	{
		bool Result = pResultType->FindBaseType (pSrcType, &Coord);
		ASSERT (Result);

		Offset = -(intptr_t) Coord.m_FieldCoord.m_Offset;
	}

	if (!Offset)
	{
		m_pModule->m_LlvmBuilder.CreateBitCast (ArgValue, pResultType, pResultValue);
		return true;
	}

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (ArgValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);
	m_pModule->m_LlvmBuilder.CreateGep (PtrValue, Offset, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pResultType, pResultValue);
	return true;
}

bool
CFunctionMgr::RuntimeError (
	const CValue& ErrorValue,
	const CValue& DataAddrValue
	)
{
	CFunction* pOnRuntimeError = GetStdFunction (EStdFunc_OnRuntimeError);

	// TODO: calc real code address

	CValue CodeAddrValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	m_pModule->m_LlvmBuilder.CreateCall3 (
		pOnRuntimeError, 
		pOnRuntimeError->GetType (),
		ErrorValue, 
		CodeAddrValue,
		DataAddrValue,
		NULL
		);

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

