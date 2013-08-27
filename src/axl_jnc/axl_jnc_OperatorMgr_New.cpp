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
	CType* pPtrType = pType->GetDataPtrType_c ();

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
	else if (StorageKind != EStorage_Heap && StorageKind != EStorage_UHeap)
	{
		err::SetFormatStringError ("cannot create non-const-sized arrays with '%s new'", GetStorageKindString (StorageKind));
		return false;
	}

	CVariable* pVariable;
	CFunction* pFunction;
	CBasicBlock* pPrevBlock;

	CValue PtrValue;
	switch (StorageKind)
	{
	case EStorage_Static:
		pVariable = m_pModule->m_VariableMgr.CreateVariable (StorageKind, pTag, pTag, pType);
		
		pFunction = m_pModule->GetConstructor ();
		pPrevBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->GetEntryBlock ());
		m_pModule->m_VariableMgr.AllocatePrimeStaticVariable (pVariable);
		m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevBlock);

		PtrValue.SetLlvmValue (pVariable->GetLlvmValue (), pPtrType);
		break;

	case EStorage_Thread:
		pVariable = m_pModule->m_VariableMgr.CreateVariable (StorageKind, pTag, pTag, pType);
		PtrValue.SetLlvmValue (pVariable->GetLlvmValue (), pPtrType);
		break;

	case EStorage_Stack:
		pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
		pPrevBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->GetEntryBlock ());
		m_pModule->m_LlvmIrBuilder.CreateAlloca (pType, pTag, pPtrType, &PtrValue);
		m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevBlock);

		if (pType->GetFlags () & ETypeFlag_GcRoot)
			MarkGcRoot (PtrValue, pType);
		break;

	case EStorage_Heap:
		pFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_HeapAlloc);
		m_pModule->m_LlvmIrBuilder.CreateCall (pFunction, pFunction->GetType (), SizeValue, &PtrValue);
		MarkGcRoot (PtrValue, pType);
		break;

	case EStorage_UHeap:
		pFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_UHeapAlloc);
		m_pModule->m_LlvmIrBuilder.CreateCall (pFunction, pFunction->GetType (), SizeValue, &PtrValue);
		break;

	default:
		err::SetFormatStringError ("invalid storage specifier '%s' in 'new' operator", GetStorageKindString (StorageKind));
		return false;
	}

	m_pModule->m_LlvmIrBuilder.CreateBitCast (PtrValue, pPtrType, pResultValue);
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
		m_pModule->m_LlvmIrBuilder.CreateStore (pType->GetZeroValue (), PtrValue);

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

	if (!pClassType->IsCreatable ())
	{
		err::SetFormatStringError ("cannot instantiate abstract '%s'", pClassType->GetTypeString ().cc ()); 
		return false;
	}

	CLlvmScopeComment Comment (&m_pModule->m_LlvmIrBuilder, "prime object");

	CFunction* pPrimer = pClassType->GetPrimer ();

	m_pModule->m_LlvmIrBuilder.CreateCall3 (
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
		m_pModule->m_LlvmIrBuilder.CreateBitCast (
			PtrValue,
			m_pModule->GetSimpleType (EStdType_ObjectHdrPtr),
			&ObjectPtrValue
			);

		m_pModule->m_LlvmIrBuilder.CreateCall (
			pAddObject,
			pAddObject->GetType (),
			ObjectPtrValue,
			NULL
			);
	}

	CFunction* pDestructor = pClassType->GetDestructor ();
	if (StorageKind != EStorage_Stack || !pDestructor)
	{
		m_pModule->m_LlvmIrBuilder.CreateGep2 (PtrValue, 1, pClassType->GetClassPtrType (), pResultValue);
		return true;
	}

	CVariable* pFlagVariable = NULL; 
	if (m_pModule->m_ControlFlowMgr.GetFlags () & EControlFlowFlag_HasJump)
	{
		CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
		CBasicBlock* pPrevBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->GetEntryBlock ());
		m_pModule->m_LlvmIrBuilder.CreateGep2 (PtrValue, 1, pClassType->GetClassPtrType (), pResultValue);

		pFlagVariable = m_pModule->m_VariableMgr.CreateOnceFlagVariable (EStorage_Stack);
		m_pModule->m_VariableMgr.AllocatePrimeInitializeVariable (pFlagVariable);
		m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevBlock);

		m_pModule->m_OperatorMgr.BinaryOperator (
			EBinOp_Assign, 
			pFlagVariable,
			CValue ((int64_t) 1, pFlagVariable->GetType ())
			);
	}
	else
	{
		m_pModule->m_LlvmIrBuilder.CreateGep2 (PtrValue, 1, pClassType->GetClassPtrType (), pResultValue);
	}

	m_pModule->m_NamespaceMgr.GetCurrentScope ()->m_DestructList.AddDestructor (
		pDestructor, 
		*pResultValue,
		pFlagVariable
		);

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
COperatorMgr::ParseAutoSizeArrayInitializer (
	const rtl::CConstBoxListT <CToken>& InitializerTokenList,
	size_t* pElementCount
	)
{
	int FirstToken = InitializerTokenList.GetHead ()->m_Token;
	switch (FirstToken)
	{
	case EToken_Literal:
	case EToken_HexLiteral:
		*pElementCount = ParseAutoSizeArrayLiteralInitializer (InitializerTokenList);
		break;

	case '{':
		*pElementCount = ParseAutoSizeArrayCurlyInitializer (InitializerTokenList);
		break;

	default:
		err::SetFormatStringError ("invalid initializer for auto-size-array");
		return false;
	}

	return true;
}

// it's both more efficient AND easier to parse these by hand

size_t
COperatorMgr::ParseAutoSizeArrayLiteralInitializer (const rtl::CConstBoxListT <CToken>& InitializerTokenList)
{
	size_t ElementCount = 0;

	rtl::CBoxIteratorT <CToken> Token = InitializerTokenList.GetHead ();
	for (; Token; Token++)
	{
		switch (Token->m_Token)
		{
		case EToken_Literal:
			ElementCount += Token->m_Data.m_String.GetLength ();
			break;

		case EToken_HexLiteral:
			ElementCount += Token->m_Data.m_BinData.GetCount ();
			break;
		}
	}

	if (InitializerTokenList.GetTail ()->m_Token == EToken_Literal)
		ElementCount++;

	return ElementCount;
}

size_t 
COperatorMgr::ParseAutoSizeArrayCurlyInitializer (const rtl::CConstBoxListT <CToken>& InitializerTokenList)
{
	intptr_t Level = 0;
	size_t ElementCount = 0;

	bool IsElement = false;

	rtl::CBoxIteratorT <CToken> Token = InitializerTokenList.GetHead ();
	for (; Token; Token++)
	{
		switch (Token->m_Token)
		{
		case '{':
			if (Level == 1)
				IsElement = true;

			Level++;
			break;

		case '}':
			if (Level == 1 && IsElement)
			{
				ElementCount++;
				IsElement = false;
			}

			Level--;
			break;

		case ',':
			if (Level == 1)
			{
				ElementCount++;
				IsElement = false;
			}
			
			break;

		default:
			if (Level == 1)
				IsElement = true;		
		}
	}

	return ElementCount;
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

	if (StorageKind != EStorage_Static && StorageKind != EStorage_Thread)
		return 
			Prime (StorageKind, PtrValue, pType, SizeValue, pResultValue) && 
			Construct (*pResultValue, pArgList);

	TOnceStmt Stmt;
	CToken::CPos Pos;

	Result = 
		m_pModule->m_ControlFlowMgr.OnceStmt_Create (&Stmt, StorageKind) &&
		m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&Stmt, Pos);

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
		m_pModule->m_LlvmIrBuilder.CreateBitCast (OpValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);
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
		m_pModule->m_LlvmIrBuilder.CreateBitCast (OpValue, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &PtrValue);
		}
		break;

	default:
		err::SetFormatStringError ("cannot delete '%s'", OpValue.GetType ()->GetTypeString ().cc ());
		return false;
	}
			
	CValue ReturnValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (pFree, pFree->GetType (), PtrValue, &ReturnValue);
	return true;
}

void
COperatorMgr::NullifyGcRootList (const rtl::CConstBoxListT <CValue>& List)
{
	if (List.IsEmpty ())
		return;

	CLlvmScopeComment Comment (&m_pModule->m_LlvmIrBuilder, "nullify gcroot list");

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	rtl::CBoxIteratorT <CValue> It = List.GetTail ();
	for (; It; It--)
	{
		CValue Value = *It;
		ASSERT (Value.GetType ()->GetTypeKind () == EType_DataPtr);
		
		m_pModule->m_LlvmIrBuilder.CreateStore (NullValue, Value);
	}
}

void
COperatorMgr::MarkGcRoot (
	const CValue& PtrValue,
	CType* pType
	)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->GetEntryBlock ());

	CType* pBytePtrType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
	
	CValue GcRootValue;
	m_pModule->m_LlvmIrBuilder.CreateAlloca (
		pBytePtrType, 
		"gc_root", 
		pBytePtrType->GetDataPtrType_c (), 
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
	m_pModule->m_LlvmIrBuilder.CreateCall (
		pMarkGcRoot, 
		pMarkGcRoot->GetType (),
		ArgValueArray, 2,
		&ResultValue
		);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pBlock);
	
	CValue BytePtrValue;
	m_pModule->m_LlvmIrBuilder.CreateBitCast (PtrValue, pBytePtrType, &BytePtrValue);
	m_pModule->m_LlvmIrBuilder.CreateStore (BytePtrValue, GcRootValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
