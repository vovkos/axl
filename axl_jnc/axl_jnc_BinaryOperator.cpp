#include "stdafx.h"
#include "axl_jnc_BinaryOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetBinOpString (EBinOp OpKind)
{
	switch (OpKind)
	{
	case EBinOp_Add:
		return _T("+");

	case EBinOp_Sub:
		return _T("-");

	case EBinOp_Mul:
		return _T("*");

	case EBinOp_Div:
		return _T("/");

	case EBinOp_Mod:
		return _T("%");

	case EBinOp_Shl:
		return _T("<<");

	case EBinOp_Shr:
		return _T(">>");

	case EBinOp_BitwiseAnd:
		return _T("&");

	case EBinOp_BitwiseOr:
		return _T("|");

	case EBinOp_BitwiseXor:
		return _T("^");

	case EBinOp_Eq:
		return _T("==");

	case EBinOp_Ne:
		return _T("!=");

	case EBinOp_Lt:
		return _T("<");

	case EBinOp_Le:
		return _T("<=");

	case EBinOp_Gt:
		return _T(">");

	case EBinOp_Ge:
		return _T(">=");

	case EBinOp_LogicalAnd:
		return _T("&&");

	case EBinOp_LogicalOr:
		return _T("||");

	case EBinOp_Idx:
		return _T("index-operator");

	default:
		return _T("undefined-binary-operator");
	};
}

//.............................................................................

IBinaryOperator::IBinaryOperator()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_OpKind = EBinOp_None;
	m_Flags = 0;
}

//.............................................................................

bool
CBinOp_Add::Operator (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	if (OpValue1.GetType ()->IsPointerType () && OpValue2.GetType ()->IsIntegerType ())
		return PointerIncrementOperator (OpValue1, OpValue2, pResultValue);
	else if (OpValue2.GetType ()->IsPointerType () && OpValue1.GetType ()->IsIntegerType ())
		return PointerIncrementOperator (OpValue2, OpValue1, pResultValue);
	else
		return CBinaryArithmeticOperatorT <CBinOp_Add>::Operator (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Add::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateAdd_i (OpValue1, OpValue2, pResultType, pResultValue);
}
	
llvm::Value*
CBinOp_Add::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateAdd_f (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

bool
CBinOp_Sub::Operator (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	if (OpValue1.GetType ()->IsPointerType () && OpValue2.GetType ()->IsIntegerType ())
		return PointerIncrementOperator (OpValue1, OpValue2, pResultValue);
	else if (OpValue1.GetType ()->IsPointerType () && OpValue2.GetType ()->IsPointerType ())
		return PointerDifferenceOperator (OpValue1, OpValue2, pResultValue);
	else
		return CBinaryArithmeticOperatorT <CBinOp_Sub>::Operator (OpValue1, OpValue2, pResultValue);

}

llvm::Value*
CBinOp_Sub::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateSub_i (OpValue1, OpValue2, pResultType, pResultValue);
}

	
llvm::Value*
CBinOp_Sub::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateSub_f (OpValue1, OpValue2, pResultType, pResultValue);
}

bool
CBinOp_Sub::PointerDifferenceOperator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	ASSERT (RawOpValue1.GetType ()->IsPointerType ());

	CType* pBaseType = ((CPointerType*) RawOpValue1.GetType ())->GetBaseType ()->GetUnqualifiedType ();
	CPointerType* pPointerType = pBaseType->GetPointerType (EType_Pointer_u);

	CValue OpValue1;
	CValue OpValue2;

	bool Result = 
		m_pModule->m_OperatorMgr.CastOperator (RawOpValue1, pPointerType, &OpValue1) &&
		m_pModule->m_OperatorMgr.CastOperator (RawOpValue2, pPointerType, &OpValue2);

	if (!Result)
		return false;

	CType* pType = m_pModule->m_TypeMgr.GetBasicType (EType_Int_p);

	CValue CastValue1;
	CValue CastValue2;
	CValue DiffValue;
	CValue SizeValue;

	SizeValue.SetConstSizeT (pBaseType->GetSize ());

	m_pModule->m_LlvmBuilder.CreatePtrToInt (OpValue1, pType, &CastValue1);
	m_pModule->m_LlvmBuilder.CreatePtrToInt (OpValue2, pType, &CastValue2);
	m_pModule->m_LlvmBuilder.CreateSub_i (OpValue1, OpValue2, pType, &DiffValue);
	m_pModule->m_LlvmBuilder.CreateDiv_i (DiffValue, SizeValue, pType, pResultValue);
	return true;
}

//.............................................................................

llvm::Value*
CBinOp_Mul::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateMul_i (OpValue1, OpValue2, pResultType, pResultValue);
}
	
llvm::Value*
CBinOp_Mul::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateMul_f (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Div::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ?
		pModule->m_LlvmBuilder.CreateDiv_u (OpValue1, OpValue2, pResultType, pResultValue) :
		pModule->m_LlvmBuilder.CreateDiv_i (OpValue1, OpValue2, pResultType, pResultValue);
}

	
llvm::Value*
CBinOp_Div::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateDiv_f (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Mod::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ?
		pModule->m_LlvmBuilder.CreateMod_u (OpValue1, OpValue2, pResultType, pResultValue) :
		pModule->m_LlvmBuilder.CreateMod_i (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Shl::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateShl (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Shr::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateShr (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseAnd::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateAnd (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseOr::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateOr (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseXor::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateXor (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Eq::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateEq_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Eq::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateEq_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Ne::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateNe_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Ne::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateNe_f (OpValue1, OpValue2, pResultValue);
}
	
//.............................................................................

llvm::Value*
CBinOp_Lt::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateLt_u (OpValue1, OpValue2, pResultValue) :
		pModule->m_LlvmBuilder.CreateLt_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Lt::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateLt_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Le::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateLe_u (OpValue1, OpValue2, pResultValue) :
		pModule->m_LlvmBuilder.CreateLe_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Le::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateLe_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Gt::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateGt_u (OpValue1, OpValue2, pResultValue) :
		pModule->m_LlvmBuilder.CreateGt_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Gt::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateGt_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Ge::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateGe_u (OpValue1, OpValue2, pResultValue) :
		pModule->m_LlvmBuilder.CreateGe_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Ge::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateGe_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

bool
CBinOp_Idx::Operator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	bool Result;

	CValue OpValue1;
	CValue OpValue2;

	Result = m_pModule->m_OperatorMgr.CastOperator (RawOpValue2, EType_Int_p, &OpValue2);
	if (!Result)
		return false;

	CType* pOpType1 = RawOpValue1.GetType ();
	if (pOpType1->IsReferenceType ())
	{
		CType* pBaseType = ((CPointerType*) pOpType1)->GetBaseType ();

		if (pBaseType->GetTypeKind () == EType_Array)
			return ArrayIndexOperator (RawOpValue1, (CArrayType*) pBaseType, OpValue2, pResultValue);

		Result = m_pModule->m_OperatorMgr.LoadReferenceOperator (RawOpValue1, &OpValue1);
		if (!Result)
			return false;

		pOpType1 = OpValue1.GetType ();
	}

	EType TypeKind = pOpType1->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Pointer:
	case EType_Pointer_u:
		return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Add, OpValue1, OpValue2, pResultValue);

	case EType_Array:
		return ArrayIndexOperator (OpValue1, (CArrayType*) pOpType1, OpValue2, pResultValue);

	case EType_Property:
		return PropertyIndexOperator (OpValue1, OpValue2, pResultValue);

	default:
		err::SetFormatStringError (_T("cannot index '%s'"), pOpType1->GetTypeString ());
		return false;
	}
}

bool
CBinOp_Idx::ArrayIndexOperator (
	const CValue& OpValue1,
	CArrayType* pArrayType,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	CType* pElementType = pArrayType->GetBaseType ();

	if (OpValue1.GetValueKind () == EValue_Const && OpValue2.GetValueKind ())
	{
		pResultValue->CreateConst (pElementType, (char*) OpValue1.GetConstData () + OpValue2.GetSizeT () * pElementType->GetSize ());
		return true;
	}

	llvm::Value* pLlvmValue = OpValue1.GetLlvmValue ();

	EType OpTypeKind1 = OpValue1.GetType ()->GetTypeKind ();
	if (OpTypeKind1 == EType_Reference)
	{
		if (OpValue1.GetValueKind () == EValue_Variable)
		{
			CValue GepValue;
			m_pModule->m_LlvmBuilder.CreateGep2 (OpValue1, OpValue2, NULL, &GepValue);
			pResultValue->SetVariable (OpValue1.GetVariable (), GepValue.GetLlvmValue (), pElementType);
		}
		else
		{
			CValue SizeValue;
			SizeValue.SetConstSizeT (pElementType->GetSize ());
		
			CValue IncrementValue;
			bool Result = m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Mul, OpValue2, SizeValue, &IncrementValue);
			if (!Result)
				return false;

			CType* pResultType = pElementType->GetPointerType (EType_Reference);

			CValue PtrValue;
			m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue1, 0, NULL, &PtrValue);
			m_pModule->m_LlvmBuilder.CreateGep (PtrValue, IncrementValue, NULL, &PtrValue);
			m_pModule->m_LlvmBuilder.CreateInsertValue (OpValue1, PtrValue, 0, pResultType, pResultValue);
		}
	}
	else if (OpTypeKind1 == EType_Reference_u)
	{
		CType* pResultType = pElementType->GetPointerType (EType_Reference_u);
		m_pModule->m_LlvmBuilder.CreateGep2 (OpValue1, OpValue2, pResultType, pResultValue);
	}
	else 
	{
		ASSERT (OpTypeKind1 == EType_Array);

		err::SetFormatStringError (_T("indexing register-based arrays is not supported yet"));
		return false;
	}

	return true;
}

bool
CBinOp_Idx::PropertyIndexOperator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("indexing of properties is not implemented yet"));
	return false;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
