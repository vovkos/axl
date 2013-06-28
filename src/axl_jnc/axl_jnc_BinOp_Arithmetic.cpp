#include "pch.h"
#include "axl_jnc_BinOp_Arithmetic.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................
	
bool
DataPtrIncrementOperator (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	ASSERT (OpValue1.GetType ()->GetTypeKind () == EType_DataPtr);
	
	CDataPtrType* pOpType = (CDataPtrType*) OpValue1.GetType ();
	CDataPtrType* pResultType = pOpType->GetUnCheckedPtrType ();
	
	if (pOpType->GetPtrTypeKind () == EDataPtrType_Thin)
	{
		pModule->m_LlvmBuilder.CreateGep (OpValue1, OpValue2, pResultType, pResultValue);
		
		if (!(pOpType->GetFlags () & EPtrTypeFlag_Unsafe))
			pResultValue->SetThinDataPtr (pResultValue->GetLlvmValue (), pResultType, OpValue1.GetThinDataPtrValidator ());
	}
	else
	{
		CValue PtrValue;
		pModule->m_LlvmBuilder.CreateExtractValue (OpValue1, 0, NULL, &PtrValue);
		pModule->m_LlvmBuilder.CreateGep (PtrValue, OpValue2, NULL, &PtrValue);
		pModule->m_LlvmBuilder.CreateInsertValue (OpValue1, PtrValue, 0, pResultType, pResultValue);
	}
	
	return true;
}	

bool
DataPtrDifferenceOperator (
	CModule* pModule,
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	ASSERT (RawOpValue1.GetType ()->GetTypeKind () == EType_DataPtr);
	ASSERT (RawOpValue2.GetType ()->GetTypeKind () == EType_DataPtr);
	
	CDataPtrType* pOpType = (CDataPtrType*) RawOpValue1.GetType ();
	CDataPtrType* pBytePtrType = (CDataPtrType*) pModule->GetSimpleType (EStdType_BytePtr);

	CValue OpValue1;
	CValue OpValue2;

	bool Result = 
		pModule->m_OperatorMgr.CastOperator (RawOpValue1, pBytePtrType, &OpValue1) &&
		pModule->m_OperatorMgr.CastOperator (RawOpValue2, pBytePtrType, &OpValue2);

	if (!Result)
		return false;

	CType* pType = pModule->GetSimpleType (EType_Int_p);

	CValue CastValue1;
	CValue CastValue2;
	CValue DiffValue;
	CValue SizeValue;

	SizeValue.SetConstSizeT (pOpType->GetTargetType ()->GetSize ());

	pModule->m_LlvmBuilder.CreatePtrToInt (OpValue1, pType, &CastValue1);
	pModule->m_LlvmBuilder.CreatePtrToInt (OpValue2, pType, &CastValue2);
	pModule->m_LlvmBuilder.CreateSub_i (OpValue1, OpValue2, pType, &DiffValue);
	pModule->m_LlvmBuilder.CreateDiv_i (DiffValue, SizeValue, pType, pResultValue);
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
	if (OpValue1.GetType ()->GetTypeKind () == EType_DataPtr && 
		(OpValue2.GetType ()->GetTypeKindFlags () & ETypeKindFlag_Integer))
		return DataPtrIncrementOperator (m_pModule, OpValue1, OpValue2, pResultValue);
	else if (
		OpValue2.GetType ()->GetTypeKind () == EType_DataPtr && 
		(OpValue1.GetType ()->GetTypeKindFlags () & ETypeKindFlag_Integer))
		return DataPtrIncrementOperator (m_pModule, OpValue2, OpValue1, pResultValue);
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
	if (OpValue1.GetType ()->GetTypeKind () == EType_DataPtr && 
		(OpValue2.GetType ()->GetTypeKindFlags () & ETypeKindFlag_Integer))
	{
		CValue NegOpValue2;
		return 
			m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, OpValue2, &NegOpValue2) &&
			DataPtrIncrementOperator (m_pModule, OpValue1, NegOpValue2, pResultValue);
	}

	if (OpValue1.GetType ()->GetTypeKind () == EType_DataPtr && OpValue2.GetType ()->GetTypeKind () == EType_DataPtr)
		return DataPtrDifferenceOperator (m_pModule, OpValue1, OpValue2, pResultValue);
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
