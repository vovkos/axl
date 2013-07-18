#include "pch.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Parser.llk.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::Allocate (
	EStorage StorageKind,
	CType* pType,
	const CValue& SizeValue,
	const char* pTag,
	CValue* pResultValue
	)
{
	llvm::GlobalVariable* pLlvmGlobalVariable;
	CFunction* pAlloc;

	CType* pPtrType = pType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe);

	bool IsNonConstSizeArray = SizeValue.GetValueKind () != EValue_Const;
	if (!IsNonConstSizeArray)
	{
		size_t Size = SizeValue.GetSizeT ();
		if (Size != pType->GetSize ())
		{
			ASSERT (Size % pType->GetSize () == 0);
			size_t ElementCount = Size / pType->GetSize ();
			pType = m_pModule->m_TypeMgr.GetArrayType (pType, ElementCount);
		}
	}
	
	CValue PtrValue;
	switch (StorageKind)
	{
	case EStorage_Static:
		if (IsNonConstSizeArray)
		{
			err::SetFormatStringError ("cannot create non-const-sized arrays with 'static new'");
			return false;
		}

		pLlvmGlobalVariable = m_pModule->m_VariableMgr.CreateLlvmGlobalVariable (pType, pTag);
		PtrValue.SetLlvmValue (pLlvmGlobalVariable, pPtrType);
		break;

	case EStorage_Stack:
		if (IsNonConstSizeArray)
		{
			err::SetFormatStringError ("cannot create non-const-sized arrays with 'stack new'");
			return false;
		}

		if (!m_pModule->m_NamespaceMgr.GetCurrentScope ())
		{
			err::SetFormatStringError ("'stack new' operator can only be called at local scope");
			return false;
		}

		if (m_pModule->m_ControlFlowMgr.GetFlags () & EControlFlowFlag_HasJump)
		{
			err::SetFormatStringError ("'stack new' cannot be used in branched expression");
			return false;
		}

		m_pModule->m_LlvmBuilder.CreateAlloca (pType, pTag, pPtrType, &PtrValue);

		if (pType->GetFlags () & ETypeFlag_GcRoot)
			MarkGcRoot (PtrValue, pType);

		break;

	case EStorage_Heap:
		pAlloc = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_HeapAlloc);
		m_pModule->m_LlvmBuilder.CreateCall (pAlloc, pAlloc->GetType (), SizeValue, &PtrValue);
		MarkGcRoot (PtrValue, pType);
		break;


	case EStorage_UHeap:
		pAlloc = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_UHeapAlloc);
		m_pModule->m_LlvmBuilder.CreateCall (pAlloc, pAlloc->GetType (), SizeValue, &PtrValue);
		break;

	default:
		err::SetFormatStringError ("invalid storage specifier '%s' in 'new' operator", GetStorageKindString (StorageKind));
		return false;
	}

	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPtrType, pResultValue);
	return true;
}

bool
COperatorMgr::Prime (
	EStorage StorageKind,
	const CValue& PtrValue,
	CType* pType,
	const CValue& ArraySizeValue,
	CValue* pResultValue
	)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	CValue ScopeLevelValue;

	if (StorageKind == EStorage_Stack)
	{
		ASSERT (pScope);
		ScopeLevelValue = CalcScopeLevelValue (pScope);
	}
	else
		ScopeLevelValue.SetConstSizeT (0);

	if (pType->GetTypeKind () != EType_Class)
	{
		m_pModule->m_LlvmBuilder.CreateStore (pType->GetZeroValue (), PtrValue);

		pResultValue->SetThinDataPtr (
			PtrValue.GetLlvmValue (),
			pType->GetDataPtrType (EDataPtrType_Thin),
			ScopeLevelValue,
			PtrValue,
			pType->GetSize ()
			);

		return true;
	}

	CClassType* pClassType = (CClassType*) pType;
	
	uint_t Flags = EObjectFlag_Alive;
	switch (StorageKind)
	{
	case EStorage_Static:
		Flags |= EObjectFlag_Static;
		break;

	case EStorage_Stack:
		Flags |= EObjectFlag_Stack;
		break;

	case EStorage_UHeap:
		Flags |= EObjectFlag_UHeap;
		break;
	}

	if (pClassType->GetFlags () & EClassTypeFlag_Abstract)
	{
		err::SetFormatStringError ("cannot instantiate abstract '%s'", pClassType->GetTypeString ().cc ()); 
		return false;
	}

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "prime object");

	CFunction* pPrimer = pClassType->GetPrimer ();

	m_pModule->m_LlvmBuilder.CreateCall3 (
		pPrimer,
		pPrimer->GetType (),
		PtrValue,
		ScopeLevelValue,
		CValue (Flags, EType_Int_p),
		NULL
		);

	if (StorageKind == EStorage_Heap)
	{
		CFunction* pAddObject = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_GcAddObject);

		CValue ObjectPtrValue;
		m_pModule->m_LlvmBuilder.CreateBitCast (
			PtrValue,
			m_pModule->GetSimpleType (EStdType_ObjectHdrPtr),
			&ObjectPtrValue
			);

		m_pModule->m_LlvmBuilder.CreateCall (
			pAddObject,
			pAddObject->GetType (),
			ObjectPtrValue,
			NULL
			);
	}

	m_pModule->m_LlvmBuilder.CreateGep2 (PtrValue, 1, pClassType->GetClassPtrType (), pResultValue);

	return true;
}

bool
COperatorMgr::Construct (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	CType* pType = RawOpValue.GetType ();
	EType PtrTypeKind = pType->GetTypeKind ();

	switch (PtrTypeKind)
	{
	case EType_DataRef:
	case EType_DataPtr:
		pType = ((CDataPtrType*) pType)->GetTargetType ();
		break;

	case EType_ClassPtr:
	case EType_ClassRef:
		pType = ((CClassPtrType*) pType)->GetTargetType ();
		break;

	default:
		err::SetFormatStringError ("'%s' is not a pointer or reference", pType->GetTypeString ().cc ());
		return false;
	}

	EType TypeKind = pType->GetTypeKind ();

	CFunction* pConstructor = NULL;
	
	switch (TypeKind)
	{
	case EType_Struct:
	case EType_Union:
		pConstructor = ((CDerivableType*) pType)->GetConstructor ();
		break;

	case EType_Class:
		pConstructor = ((CClassType*) pType)->GetConstructor ();
		break;
	}

	if (!pConstructor)
	{
		if (pArgList && !pArgList->IsEmpty ())
		{
			err::SetFormatStringError ("'%s' has no constructor", pType->GetTypeString ().cc ());
			return false;
		}

		return true;
	}

	rtl::CBoxListT <CValue> ArgList;
	if (!pArgList)
		pArgList = &ArgList;

	CValue OpValue = RawOpValue;
	if (PtrTypeKind == EType_DataRef || PtrTypeKind == EType_ClassRef)
	{
		bool Result = UnaryOperator (EUnOp_Addr, &OpValue);
		if (!Result)
			return false;
	}
	
	pArgList->InsertHead (OpValue);

	return CallOperator (pConstructor, pArgList);
}

bool
COperatorMgr::ParseInitializer (
	const CValue& RawValue,
	const rtl::CConstBoxListT <CToken>& ConstructorTokenList,
	const rtl::CConstBoxListT <CToken>& InitializerTokenList
	)
{
	bool Result;

	CValue Value = RawValue;

	if (RawValue.GetType ()->GetTypeKind () == EType_DataRef)
		Value.OverrideType (((CDataPtrType*) RawValue.GetType ())->GetUnConstPtrType ());
	else if (RawValue.GetType ()->GetTypeKind () == EType_ClassRef)
		Value.OverrideType (((CClassPtrType*) RawValue.GetType ())->GetUnConstPtrType ());

	rtl::CBoxListT <CValue> ArgList;
	if (!ConstructorTokenList.IsEmpty ())
	{
		CParser Parser;
		Parser.m_pModule = m_pModule;
		Parser.m_Stage = CParser::EStage_Pass2;

		m_pModule->m_ControlFlowMgr.ResetJumpFlag ();

		Result = Parser.ParseTokenList (ESymbol_expression_or_empty_list_save_list, ConstructorTokenList);
		if (!Result)
			return false;

		ArgList.TakeOver (&Parser.m_ExpressionValueList);
	}

	Result = Construct (Value, &ArgList);
	if (!Result)
		return false;

	if (!InitializerTokenList.IsEmpty ())
	{
		CParser Parser;
		Parser.m_pModule = m_pModule;
		Parser.m_Stage = CParser::EStage_Pass2;

		m_pModule->m_ControlFlowMgr.ResetJumpFlag ();

		if (InitializerTokenList.GetHead ()->m_Token == '{')
		{
			Parser.m_CurlyInitializerTargetValue = Value;
			Result = Parser.ParseTokenList (ESymbol_curly_initializer, InitializerTokenList);
			if (!Result)
				return false;
		}
		else
		{
			Result = 
				Parser.ParseTokenList (ESymbol_expression_save_value, InitializerTokenList) &&
				m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, Value, Parser.m_ExpressionValue);

			if (!Result)
				return false;
		}
	}

	return true;
}

bool
COperatorMgr::ParseExpression (
	const rtl::CConstBoxListT <CToken>& ExpressionTokenList,
	CValue* pResultValue
	)
{
	CParser Parser;
	Parser.m_pModule = m_pModule;
	Parser.m_Stage = CParser::EStage_Pass2;

	m_pModule->m_ControlFlowMgr.ResetJumpFlag ();

	bool Result = Parser.ParseTokenList (ESymbol_expression_save_value, ExpressionTokenList);
	if (!Result)
		return false;

	*pResultValue = Parser.m_ExpressionValue;
	return true;
}

bool
COperatorMgr::ParseConstExpression (
	const rtl::CConstBoxListT <CToken>& ExpressionTokenList,
	CValue* pResultValue
	)
{
	CParser Parser;
	Parser.m_pModule = m_pModule;
	Parser.m_Stage = CParser::EStage_Pass2;
	Parser.m_Flags |= CParser::EFlag_ConstExpression;

	m_pModule->m_ControlFlowMgr.ResetJumpFlag ();

	bool Result = Parser.ParseTokenList (ESymbol_expression_save_value, ExpressionTokenList);
	if (!Result)
		return false;

	ASSERT (Parser.m_ExpressionValue.GetValueKind () == EValue_Const);
	*pResultValue = Parser.m_ExpressionValue;
	return true;
}

bool
COperatorMgr::ParseConstIntegerExpression (
	const rtl::CConstBoxListT <CToken>& ExpressionTokenList,
	intptr_t* pInteger
	)
{
	CValue Value;
	bool Result = ParseConstExpression (ExpressionTokenList, &Value);
	if (!Result)
		return false;

	if (!(Value.GetType ()->GetTypeKindFlags () & ETypeKindFlag_Integer))
	{
		err::SetFormatStringError ("expression is not integer constant");
		return false;
	}
	
	*pInteger = 0;
	memcpy (pInteger, Value.GetConstData (), Value.GetType ()->GetSize ());
	return true;
}

bool
COperatorMgr::EvaluateAlias (
	const rtl::CConstBoxListT <CToken> TokenList,
	CValue* pResultValue
	)
{
	CParser Parser;
	Parser.m_pModule = m_pModule;
	Parser.m_Stage = CParser::EStage_Pass2;

	bool Result = Parser.ParseTokenList (ESymbol_expression_save_value, TokenList);
	if (!Result)
		return false;

	*pResultValue = Parser.m_ExpressionValue;
	return true;
}

bool
COperatorMgr::NewOperator (
	EStorage StorageKind,
	CType* pType,
	const CValue& ElementCountValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	bool Result;

	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	ASSERT (pScope);

	CValue SizeValue (pType->GetSize (), GetSimpleType (m_pModule, EType_SizeT));
	if (ElementCountValue)
	{
		Result = 
			BinaryOperator (EBinOp_Mul, &SizeValue, ElementCountValue) &&
			CastOperator (&SizeValue, EType_SizeT);
		
		if (!Result)
			return false;
	}

	CValue PtrValue;
	Result = Allocate (StorageKind, pType, SizeValue, "new", &PtrValue);
	if (!Result)
		return false;

	if (StorageKind != EStorage_Static)
		return 
			Prime (StorageKind, PtrValue, pType, SizeValue, pResultValue) && 
			Construct (*pResultValue, pArgList);

	TOnceStmt Stmt;
	m_pModule->m_ControlFlowMgr.OnceStmt_Create (&Stmt);

	CToken::CPos Pos;
	Result = m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&Stmt, Pos);
	if (!Result)
		return false;

	Result = 
		Prime (StorageKind, PtrValue, pType, SizeValue, pResultValue) && 
		Construct (*pResultValue, pArgList);

	if (!Result)
		return false;

	m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&Stmt, Pos);

	return true;
}

bool
COperatorMgr::DeleteOperator (const CValue& RawOpValue)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	CFunction* pFree;

	CValue PtrValue;
	EType TypeKind = OpValue.GetType ()->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_DataPtr:
		Result = PrepareDataPtr (&OpValue);
		if (!Result)
			return false;

		pFree = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_UHeapFree);	
		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);
		break;

	case EType_ClassPtr:
		{
		CClassType* pClassType = ((CClassPtrType*) OpValue.GetType ())->GetTargetType ();
		CFunction* pDestructor = pClassType->GetDestructor ();
		if (pDestructor)
		{
			Result = CallOperator (pDestructor, OpValue);
			if (!Result)
				return false;			
		}
		else
		{
			CheckClassPtrNull (OpValue);
		}

		pFree = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_UHeapFreeClassPtr);	
		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &PtrValue);
		}
		break;

	default:
		err::SetFormatStringError ("cannot delete '%s'", OpValue.GetType ()->GetTypeString ().cc ());
		return false;
	}
			
	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (pFree, pFree->GetType (), PtrValue, &ReturnValue);
	return true;
}

void
COperatorMgr::ProcessDestructArray (
	CVariable* const* ppDestructArray,
	size_t Count
	)
{
	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "process destruct list");

	for (intptr_t i = Count - 1; i >= 0; i--)
	{
		CVariable* pVariable = ppDestructArray [i];
		ASSERT (pVariable->GetType ()->GetTypeKind () == EType_Class);

		CClassType* pType = (CClassType*) pVariable->GetType ();
		CFunction* pDestructor = pType->GetDestructor ();
		ASSERT (pDestructor);

		m_pModule->m_LlvmBuilder.CreateCall (pDestructor, pDestructor->GetType (), pVariable, NULL);		
	}
}

void
COperatorMgr::ProcessLazyStaticDestructList (const rtl::CConstListT <TLazyStaticDestruct>& List)
{
	rtl::CIteratorT <TLazyStaticDestruct> Destruct = List.GetHead ();
	for (; Destruct; Destruct++)
	{
		TLazyStaticDestruct* pDestruct = *Destruct;

		CBasicBlock* pDestructBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("destruct_block");
		CBasicBlock* pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("follow_block");

		CValue CmpValue;

		m_pModule->m_OperatorMgr.BinaryOperator (
			EBinOp_Ne, 
			pDestruct->m_pFlagVariable, 
			pDestruct->m_pFlagVariable->GetType ()->GetZeroValue (),
			&CmpValue
			);

		m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pDestructBlock, pFollowBlock);

		CValue ArgValue;
		size_t ArgCount = 0;

		if (pDestruct->m_pVariable)
		{
			ArgValue.SetVariable (pDestruct->m_pVariable);
			ArgCount = 1;
		}

		m_pModule->m_LlvmBuilder.CreateCall (
			pDestruct->m_pDestructor, 
			pDestruct->m_pDestructor->GetType (), 
			&ArgValue, ArgCount,
			NULL
			);

		m_pModule->m_ControlFlowMgr.Follow (pFollowBlock);
	}
}

void
COperatorMgr::NullifyGcRootList (const rtl::CConstBoxListT <CValue>& List)
{
	if (List.IsEmpty ())
		return;

	CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "nullify gcroot list");

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	rtl::CBoxIteratorT <CValue> It = List.GetTail ();
	for (; It; It--)
	{
		CValue Value = *It;
		ASSERT (Value.GetType ()->GetTypeKind () == EType_DataPtr);
		
		m_pModule->m_LlvmBuilder.CreateStore (NullValue, Value);
	}
}

void
COperatorMgr::MarkGcRoot (
	const CValue& PtrValue,
	CType* pType
	)
{
	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->GetEntryBlock ());

	CType* pBytePtrType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
	
	CValue GcRootValue;
	m_pModule->m_LlvmBuilder.CreateAlloca (
		pBytePtrType, 
		"gc_root", 
		pBytePtrType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe), 
		&GcRootValue
		);

	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	ASSERT (pScope);

	pScope->AddToGcRootList (GcRootValue);

	CFunction* pMarkGcRoot = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_MarkGcRoot);
	ASSERT (pMarkGcRoot);
	
	CValue ArgValueArray [2];
	ArgValueArray [0] = GcRootValue;
	ArgValueArray [1].CreateConst (&pType, m_pModule->GetSimpleType (EStdType_BytePtr));
		
	CValue ResultValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pMarkGcRoot, 
		pMarkGcRoot->GetType (),
		ArgValueArray, 2,
		&ResultValue
		);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pBlock);
	
	CValue BytePtrValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pBytePtrType, &BytePtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (BytePtrValue, GcRootValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
