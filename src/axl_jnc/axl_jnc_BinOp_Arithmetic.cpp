#include "pch.h"
#include "axl_jnc_BinOp_Arithmetic.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................
	
bool
PointerIncrementOperator (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	bool Result;

	CValue SizeValue;
	CValue IncrementValue;
	CValue PtrValue;

	CDataPtrType* pType = (CDataPtrType*) OpValue1.GetType ();
	ASSERT (pType->GetTypeKind () == EType_DataPtr);

	if (pType->GetPtrTypeKind () == EDataPtrType_Unsafe)
	{
		pModule->m_LlvmBuilder.CreateGep (OpValue1, OpValue2, pType, pResultValue);
		return true;
	}

	SizeValue.SetConstSizeT (pType->GetTargetType ()->GetSize ());

	Result = pModule->m_OperatorMgr.BinaryOperator (EBinOp_Mul, OpValue2, SizeValue, &IncrementValue);
	if (!Result)
		return false;

	pModule->m_LlvmBuilder.CreateExtractValue (OpValue1, 0, NULL, &PtrValue);
	pModule->m_LlvmBuilder.CreateGep (PtrValue, IncrementValue, NULL, &PtrValue);
	pModule->m_LlvmBuilder.CreateInsertValue (OpValue1, PtrValue, 0, pType, pResultValue);
	return true;
}	

//.............................................................................

bool
CBinOp_Add::Operator (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	if (OpValue1.GetType ()->GetTypeKind () == EType_DataPtr && OpValue2.GetType ()->IsIntegerType ())
		return PointerIncrementOperator (m_pModule, OpValue1, OpValue2, pResultValue);
	else if (OpValue2.GetType ()->GetTypeKind () == EType_DataPtr && OpValue1.GetType ()->IsIntegerType ())
		return PointerIncrementOperator (m_pModule, OpValue2, OpValue1, pResultValue);
	else
		return CBinOpT_Arithmetic <CBinOp_Add>::Operator (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Add::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return m_pModule->m_LlvmBuilder.CreateAdd_i (OpValue1, OpValue2, pResultType, pResultValue);
}
	
llvm::Value*
CBinOp_Add::LlvmOpFp (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return m_pModule->m_LlvmBuilder.CreateAdd_f (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

bool
CBinOp_Sub::Operator (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	if (OpValue1.GetType ()->GetTypeKind () == EType_DataPtr && OpValue2.GetType ()->IsIntegerType ())
		return PointerIncrementOperator (m_pModule, OpValue1, OpValue2, pResultValue);
	else if (OpValue1.GetType ()->GetTypeKind () == EType_DataPtr && OpValue2.GetType ()->GetTypeKind () == EType_DataPtr)
		return PointerDifferenceOperator (OpValue1, OpValue2, pResultValue);
	else
		return CBinOpT_Arithmetic <CBinOp_Sub>::Operator (OpValue1, OpValue2, pResultValue);

}

llvm::Value*
CBinOp_Sub::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return m_pModule->m_LlvmBuilder.CreateSub_i (OpValue1, OpValue2, pResultType, pResultValue);
}

	
llvm::Value*
CBinOp_Sub::LlvmOpFp (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return m_pModule->m_LlvmBuilder.CreateSub_f (OpValue1, OpValue2, pResultType, pResultValue);
}

bool
CBinOp_Sub::PointerDifferenceOperator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	ASSERT (RawOpValue1.GetType ()->GetTypeKind () == EType_DataPtr);

	CType* pBaseType = ((CDataPtrType*) RawOpValue1.GetType ())->GetTargetType ();
	CDataPtrType* pPointerType = pBaseType->GetDataPtrType (EDataPtrType_Unsafe);

	CValue OpValue1;
	CValue OpValue2;

	bool Result = 
		m_pModule->m_OperatorMgr.CastOperator (RawOpValue1, pPointerType, &OpValue1) &&
		m_pModule->m_OperatorMgr.CastOperator (RawOpValue2, pPointerType, &OpValue2);

	if (!Result)
		return false;

	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);

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
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return m_pModule->m_LlvmBuilder.CreateMul_i (OpValue1, OpValue2, pResultType, pResultValue);
}
	
llvm::Value*
CBinOp_Mul::LlvmOpFp (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return m_pModule->m_LlvmBuilder.CreateMul_f (OpValue1, OpValue2, pResultType, pResultValue);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

llvm::Value*
CBinOp_Div::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ?
		m_pModule->m_LlvmBuilder.CreateDiv_u (OpValue1, OpValue2, pResultType, pResultValue) :
		m_pModule->m_LlvmBuilder.CreateDiv_i (OpValue1, OpValue2, pResultType, pResultValue);
}

	
llvm::Value*
CBinOp_Div::LlvmOpFp (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return m_pModule->m_LlvmBuilder.CreateDiv_f (OpValue1, OpValue2, pResultType, pResultValue);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

llvm::Value*
CBinOp_Mod::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ?
		m_pModule->m_LlvmBuilder.CreateMod_u (OpValue1, OpValue2, pResultType, pResultValue) :
		m_pModule->m_LlvmBuilder.CreateMod_i (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Shl::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return m_pModule->m_LlvmBuilder.CreateShl (OpValue1, OpValue2, pResultType, pResultValue);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

llvm::Value*
CBinOp_Shr::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return m_pModule->m_LlvmBuilder.CreateShr (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_BwAnd::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return m_pModule->m_LlvmBuilder.CreateAnd (OpValue1, OpValue2, pResultType, pResultValue);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

llvm::Value*
CBinOp_BwOr::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return m_pModule->m_LlvmBuilder.CreateOr (OpValue1, OpValue2, pResultType, pResultValue);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

llvm::Value*
CBinOp_BwXor::LlvmOpInt (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CType* pResultType,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return m_pModule->m_LlvmBuilder.CreateXor (OpValue1, OpValue2, pResultType, pResultValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
