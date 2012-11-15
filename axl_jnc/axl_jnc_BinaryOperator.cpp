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
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateAdd (pOpValue1, pOpValue2, "add_i");
}
	
llvm::Value*
CBinOp_Add::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFAdd (pOpValue1, pOpValue2, "add_f");
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
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateSub (pOpValue1, pOpValue2, "sub_i");
}

	
llvm::Value*
CBinOp_Sub::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFSub (pOpValue1, pOpValue2, "sub_f");
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

	CType* pType = m_pModule->m_TypeMgr.GetBasicType (EType_SizeT);

	CValue SizeValue;
	SizeValue.SetConstSizeT (pBaseType->GetSize ());

	llvm::Value* pLlvmOp1 = m_pModule->m_LlvmBuilder.CreatePtrToInt (OpValue1.GetLlvmValue (), pType->GetLlvmType (), "ptr1_i");
	llvm::Value* pLlvmOp2 = m_pModule->m_LlvmBuilder.CreatePtrToInt (OpValue2.GetLlvmValue (), pType->GetLlvmType (), "ptr2_i");
	llvm::Value* pLlvmDiff = m_pModule->m_LlvmBuilder.CreateSub (pLlvmOp1, pLlvmOp2, "ptr_diff_i");
	pLlvmDiff = m_pModule->m_LlvmBuilder.CreateUDiv (pLlvmDiff, SizeValue.GetLlvmValue (), "ptr_diff");

	pResultValue->SetLlvmRegister (pLlvmDiff, pType);
	return true;
}

//.............................................................................

llvm::Value*
CBinOp_Mul::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateMul (pOpValue1, pOpValue2, "mul_i");
}
	
llvm::Value*
CBinOp_Mul::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFMul (pOpValue1, pOpValue2, "mul_f");
}

//.............................................................................

llvm::Value*
CBinOp_Div::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateSDiv (pOpValue1, pOpValue2, "div_i");
}

	
llvm::Value*
CBinOp_Div::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFDiv (pOpValue1, pOpValue2, "div_f");
}

//.............................................................................

llvm::Value*
CBinOp_Mod::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateSRem (pOpValue1, pOpValue2, "mod");
}

//.............................................................................

llvm::Value*
CBinOp_Shl::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateShl (pOpValue1, pOpValue2, "shl");
}

//.............................................................................

llvm::Value*
CBinOp_Shr::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateLShr (pOpValue1, pOpValue2, "shr");
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseAnd::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateAnd (pOpValue1, pOpValue2, "and");
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseOr::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateOr (pOpValue1, pOpValue2, "or");
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseXor::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateXor (pOpValue1, pOpValue2, "xor");
}

//.............................................................................

llvm::Value*
CBinOp_Eq::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateICmpEQ (pOpValue1, pOpValue2, "eq_i");
}

llvm::Value*
CBinOp_Eq::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFCmpOEQ (pOpValue1, pOpValue2, "eq_f");
}

//.............................................................................

llvm::Value*
CBinOp_Ne::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateICmpNE (pOpValue1, pOpValue2, "ne_i");
}

llvm::Value*
CBinOp_Ne::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFCmpONE (pOpValue1, pOpValue2, "ne_f");
}
	
//.............................................................................

llvm::Value*
CBinOp_Lt::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateICmpULT (pOpValue1, pOpValue2, "lt_u") :
		pModule->m_LlvmBuilder.CreateICmpSLT (pOpValue1, pOpValue2, "lt_i");
}

llvm::Value*
CBinOp_Lt::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFCmpOLT (pOpValue1, pOpValue2, "lt_f");
}

//.............................................................................

llvm::Value*
CBinOp_Le::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateICmpULE (pOpValue1, pOpValue2, "le_u") :
		pModule->m_LlvmBuilder.CreateICmpSLE (pOpValue1, pOpValue2, "le_i");
}

llvm::Value*
CBinOp_Le::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFCmpOLE (pOpValue1, pOpValue2, "le_f");
}

//.............................................................................

llvm::Value*
CBinOp_Gt::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateICmpUGT (pOpValue1, pOpValue2, "gt_u") :
		pModule->m_LlvmBuilder.CreateICmpSGT (pOpValue1, pOpValue2, "gt_i");
}

llvm::Value*
CBinOp_Gt::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFCmpOGT (pOpValue1, pOpValue2, "gt_f");
}

//.............................................................................

llvm::Value*
CBinOp_Ge::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateICmpUGE (pOpValue1, pOpValue2, "ge_u") :
		pModule->m_LlvmBuilder.CreateICmpSGE (pOpValue1, pOpValue2, "ge_i");
}

llvm::Value*
CBinOp_Ge::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_LlvmBuilder.CreateFCmpOGE (pOpValue1, pOpValue2, "ge_f");
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
