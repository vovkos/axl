#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

COperatorMgr::COperatorMgr (CModule* pModule)
{
	m_pModule = pModule;
}

void
COperatorMgr::Clear ()
{
	for (size_t i = 0; i < EUnOp__Count; i++)
		m_UnaryOperatorTable [i].Clear ();

	for (size_t i = 0; i < EBinOp__Count; i++)
		m_BinaryOperatorTable [i].Clear ();

	memset (m_BasicCastOperatorTable, 0, sizeof (m_BasicCastOperatorTable));
	m_CastOperatorMap.Clear ();
	m_CastOperatorList.Clear ();
	m_SuperCastList.Clear ();
}

void
COperatorMgr::AddStdOperators ()
{
	AddStdMoveOperators ();
	AddStdUnaryOperators ();
	AddStdBinaryOperators ();
}

void
COperatorMgr::AddStdUnaryOperators ()
{
	AddUnaryOperator (EUnOp_Minus, EType_Int32, &m_UnOp_Minus_i32);
	AddUnaryOperator (EUnOp_Minus, EType_Int64, &m_UnOp_Minus_i64);
	AddUnaryOperator (EUnOp_Minus, EType_Float, &m_UnOp_Minus_f32);
	AddUnaryOperator (EUnOp_Minus, EType_Double, &m_UnOp_Minus_f64);

	AddUnaryOperator (EUnOp_BitwiseNot, EType_Int32, &m_UnOp_BitwiseNot_i32);
	AddUnaryOperator (EUnOp_BitwiseNot, EType_Int64, &m_UnOp_BitwiseNot_i64);
}

void
COperatorMgr::AddStdBinaryOperators ()
{
	AddBinaryOperator (EBinOp_Add, EType_Int32, &m_BinOp_Add_i32);
	AddBinaryOperator (EBinOp_Add, EType_Int64, &m_BinOp_Add_i64);
	AddBinaryOperator (EBinOp_Add, EType_Float, &m_BinOp_Add_f32);
	AddBinaryOperator (EBinOp_Add, EType_Double, &m_BinOp_Add_f64);

	AddBinaryOperator (EBinOp_Sub, EType_Int32, &m_BinOp_Sub_i32);
	AddBinaryOperator (EBinOp_Sub, EType_Int64, &m_BinOp_Sub_i64);
	AddBinaryOperator (EBinOp_Sub, EType_Float, &m_BinOp_Sub_f32);
	AddBinaryOperator (EBinOp_Sub, EType_Double, &m_BinOp_Sub_f64);

	AddBinaryOperator (EBinOp_Mul, EType_Int32, &m_BinOp_Mul_i32);
	AddBinaryOperator (EBinOp_Mul, EType_Int64, &m_BinOp_Mul_i64);
	AddBinaryOperator (EBinOp_Mul, EType_Float, &m_BinOp_Mul_f32);
	AddBinaryOperator (EBinOp_Mul, EType_Double, &m_BinOp_Mul_f64);

	AddBinaryOperator (EBinOp_Div, EType_Int32, &m_BinOp_Div_i32);
	AddBinaryOperator (EBinOp_Div, EType_Int64, &m_BinOp_Div_i64);
	AddBinaryOperator (EBinOp_Div, EType_Float, &m_BinOp_Div_f32);
	AddBinaryOperator (EBinOp_Div, EType_Double, &m_BinOp_Div_f64);
	AddBinaryOperator (EBinOp_Div, EType_Int32_u, &m_BinOp_Div_i32u);
	AddBinaryOperator (EBinOp_Div, EType_Int64_u, &m_BinOp_Div_i64u);

	AddBinaryOperator (EBinOp_Mod, EType_Int32, &m_BinOp_Mod_i32);
	AddBinaryOperator (EBinOp_Mod, EType_Int64, &m_BinOp_Mod_i64);
	AddBinaryOperator (EBinOp_Mod, EType_Int32_u, &m_BinOp_Mod_i32u);
	AddBinaryOperator (EBinOp_Mod, EType_Int64_u, &m_BinOp_Mod_i64u);

	AddBinaryOperator (EBinOp_Shl, EType_Int32, &m_BinOp_Shl_i32);
	AddBinaryOperator (EBinOp_Shl, EType_Int64, &m_BinOp_Shl_i64);

	AddBinaryOperator (EBinOp_Shr, EType_Int32, &m_BinOp_Shr_i32);
	AddBinaryOperator (EBinOp_Shr, EType_Int64, &m_BinOp_Shr_i64);

	AddBinaryOperator (EBinOp_BitwiseAnd, EType_Int32, &m_BinOp_BitwiseAnd_i32);
	AddBinaryOperator (EBinOp_BitwiseAnd, EType_Int64, &m_BinOp_BitwiseAnd_i64);

	AddBinaryOperator (EBinOp_BitwiseOr, EType_Int32, &m_BinOp_BitwiseOr_i32);
	AddBinaryOperator (EBinOp_BitwiseOr, EType_Int64, &m_BinOp_BitwiseOr_i64);

	AddBinaryOperator (EBinOp_BitwiseXor, EType_Int32, &m_BinOp_BitwiseXor_i32);
	AddBinaryOperator (EBinOp_BitwiseXor, EType_Int64, &m_BinOp_BitwiseXor_i64);
}

void
COperatorMgr::AddStdMoveOperators ()
{
	// integer copies

	AddCastOperator (EType_Int8, EType_Int8, &m_Cast_cpy);
	AddCastOperator (EType_Int8, EType_Int8_u, &m_Cast_cpy);
	AddCastOperator (EType_Int8_u, EType_Int8, &m_Cast_cpy);
	AddCastOperator (EType_Int8_u, EType_Int8_u, &m_Cast_cpy);
	AddCastOperator (EType_Int16, EType_Int16, &m_Cast_cpy);
	AddCastOperator (EType_Int16, EType_Int16_u, &m_Cast_cpy);
	AddCastOperator (EType_Int16_u, EType_Int16, &m_Cast_cpy);
	AddCastOperator (EType_Int16_u, EType_Int16_u, &m_Cast_cpy);
	AddCastOperator (EType_Int32, EType_Int32, &m_Cast_cpy);
	AddCastOperator (EType_Int32, EType_Int32_u, &m_Cast_cpy);
	AddCastOperator (EType_Int32_u, EType_Int32, &m_Cast_cpy);
	AddCastOperator (EType_Int32_u, EType_Int32_u, &m_Cast_cpy);
	AddCastOperator (EType_Int64, EType_Int64, &m_Cast_cpy);
	AddCastOperator (EType_Int64, EType_Int64_u, &m_Cast_cpy);
	AddCastOperator (EType_Int64_u, EType_Int64, &m_Cast_cpy);
	AddCastOperator (EType_Int64_u, EType_Int64_u, &m_Cast_cpy);

	AddCastOperator (EType_Int16_be, EType_Int16_be, &m_Cast_cpy);
	AddCastOperator (EType_Int16_be, EType_Int16_beu, &m_Cast_cpy);
	AddCastOperator (EType_Int16_beu, EType_Int16_be, &m_Cast_cpy);
	AddCastOperator (EType_Int16_beu, EType_Int16_beu, &m_Cast_cpy);
	AddCastOperator (EType_Int32_be, EType_Int32_be, &m_Cast_cpy);
	AddCastOperator (EType_Int32_be, EType_Int32_beu, &m_Cast_cpy);
	AddCastOperator (EType_Int32_beu, EType_Int32_be, &m_Cast_cpy);
	AddCastOperator (EType_Int32_beu, EType_Int32_beu, &m_Cast_cpy);
	AddCastOperator (EType_Int64_be, EType_Int64_be, &m_Cast_cpy);
	AddCastOperator (EType_Int64_be, EType_Int64_beu, &m_Cast_cpy);
	AddCastOperator (EType_Int64_beu, EType_Int64_be, &m_Cast_cpy);
	AddCastOperator (EType_Int64_beu, EType_Int64_beu, &m_Cast_cpy);

	// endianness swaps

	AddCastOperator (EType_Int16, EType_Int16_be, &m_Cast_i16_swp);
	AddCastOperator (EType_Int16, EType_Int16_beu, &m_Cast_i16_swp);
	AddCastOperator (EType_Int16_u, EType_Int16_be, &m_Cast_i16_swp);
	AddCastOperator (EType_Int16_u, EType_Int16_beu, &m_Cast_i16_swp);
	AddCastOperator (EType_Int16_be, EType_Int16, &m_Cast_i16_swp);
	AddCastOperator (EType_Int16_be, EType_Int16_u, &m_Cast_i16_swp);
	AddCastOperator (EType_Int16_beu, EType_Int16, &m_Cast_i16_swp);
	AddCastOperator (EType_Int16_beu, EType_Int16_u, &m_Cast_i16_swp);

	AddCastOperator (EType_Int32, EType_Int32_be, &m_Cast_i32_swp);
	AddCastOperator (EType_Int32, EType_Int32_beu, &m_Cast_i32_swp);
	AddCastOperator (EType_Int32_u, EType_Int32_be, &m_Cast_i32_swp);
	AddCastOperator (EType_Int32_u, EType_Int32_beu, &m_Cast_i32_swp);
	AddCastOperator (EType_Int32_be, EType_Int32, &m_Cast_i32_swp);
	AddCastOperator (EType_Int32_be, EType_Int32_u, &m_Cast_i32_swp);
	AddCastOperator (EType_Int32_beu, EType_Int32, &m_Cast_i32_swp);
	AddCastOperator (EType_Int32_beu, EType_Int32_u, &m_Cast_i32_swp);

	AddCastOperator (EType_Int64, EType_Int64_be, &m_Cast_i64_swp);
	AddCastOperator (EType_Int64, EType_Int64_beu, &m_Cast_i64_swp);
	AddCastOperator (EType_Int64_u, EType_Int64_be, &m_Cast_i64_swp);
	AddCastOperator (EType_Int64_u, EType_Int64_beu, &m_Cast_i64_swp);
	AddCastOperator (EType_Int64_be, EType_Int64, &m_Cast_i64_swp);
	AddCastOperator (EType_Int64_be, EType_Int64_u, &m_Cast_i64_swp);
	AddCastOperator (EType_Int64_beu, EType_Int64, &m_Cast_i64_swp);
	AddCastOperator (EType_Int64_beu, EType_Int64_u, &m_Cast_i64_swp);

	// integer truncations

	AddCastOperator (EType_Int16, EType_Int8, &m_Cast_int_trunc);
	AddCastOperator (EType_Int16, EType_Int8_u, &m_Cast_int_trunc);
	AddCastOperator (EType_Int32, EType_Int8, &m_Cast_int_trunc);
	AddCastOperator (EType_Int32, EType_Int8_u, &m_Cast_int_trunc);
	AddCastOperator (EType_Int32, EType_Int16, &m_Cast_int_trunc);
	AddCastOperator (EType_Int32, EType_Int16_u, &m_Cast_int_trunc);
	AddCastOperator (EType_Int64, EType_Int8, &m_Cast_int_trunc);
	AddCastOperator (EType_Int64, EType_Int8_u, &m_Cast_int_trunc);
	AddCastOperator (EType_Int64, EType_Int16, &m_Cast_int_trunc);
	AddCastOperator (EType_Int64, EType_Int16_u, &m_Cast_int_trunc);
	AddCastOperator (EType_Int64, EType_Int32, &m_Cast_int_trunc);
	AddCastOperator (EType_Int64, EType_Int32_u, &m_Cast_int_trunc);

	// integer extensions

	AddCastOperator (EType_Int8, EType_Int16, &m_Cast_int_ext);
	AddCastOperator (EType_Int8, EType_Int32, &m_Cast_int_ext);
	AddCastOperator (EType_Int8, EType_Int64, &m_Cast_int_ext);
	AddCastOperator (EType_Int8_u, EType_Int16, &m_Cast_int_ext_u);
	AddCastOperator (EType_Int8_u, EType_Int32, &m_Cast_int_ext_u);
	AddCastOperator (EType_Int8_u, EType_Int64, &m_Cast_int_ext_u);
	AddCastOperator (EType_Int16, EType_Int32, &m_Cast_int_ext);
	AddCastOperator (EType_Int16, EType_Int64, &m_Cast_int_ext);
	AddCastOperator (EType_Int16_u, EType_Int32, &m_Cast_int_ext_u);
	AddCastOperator (EType_Int16_u, EType_Int64, &m_Cast_int_ext_u);
	AddCastOperator (EType_Int32, EType_Int64, &m_Cast_int_ext);
	AddCastOperator (EType_Int32_u, EType_Int64, &m_Cast_int_ext_u);

	// floating extensions & truncations

	AddCastOperator (EType_Float, EType_Float, &m_Cast_cpy);
	AddCastOperator (EType_Double, EType_Double, &m_Cast_cpy);
	AddCastOperator (EType_Float, EType_Double, &m_Cast_f32_f64);
	AddCastOperator (EType_Double, EType_Float, &m_Cast_f64_f32);

	// integer to floating point

	AddCastOperator (EType_Int32, EType_Float, &m_Cast_i32_f32);
	AddCastOperator (EType_Int32_u, EType_Float, &m_Cast_i32u_f32);
	AddCastOperator (EType_Int32, EType_Double, &m_Cast_i32_f64);
	AddCastOperator (EType_Int32_u, EType_Double, &m_Cast_i32u_f64);
	AddCastOperator (EType_Int64, EType_Float, &m_Cast_i64_f32);
	AddCastOperator (EType_Int64_u, EType_Float, &m_Cast_i64u_f32);
	AddCastOperator (EType_Int64, EType_Double, &m_Cast_i64_f64);
	AddCastOperator (EType_Int64_u, EType_Double, &m_Cast_i64u_f64);

	// floating point to integer

	AddCastOperator (EType_Float, EType_Int32, &m_Cast_f32_i32);
	AddCastOperator (EType_Double, EType_Int32, &m_Cast_f64_i32);
	AddCastOperator (EType_Float, EType_Int64, &m_Cast_f32_i64);
	AddCastOperator (EType_Double, EType_Int64, &m_Cast_f64_i64);

	// build super moves for basic types (floyd-warshall)
	
	for (size_t k = 0; k < EType__BasicTypeCount; k++)
	for (size_t i = 0; i < EType__BasicTypeCount; i++)
	for (size_t j = 0; j < EType__BasicTypeCount; j++)
	{
		CCastOperator* pOperatorIK = m_BasicCastOperatorTable [i] [k];
		CCastOperator* pOperatorKJ = m_BasicCastOperatorTable [k] [j];
		if (!pOperatorIK || !pOperatorKJ)
			continue;

		CCastOperator* pOperatorIJ = m_BasicCastOperatorTable [i] [j];
		if (pOperatorIJ && pOperatorIJ->m_Price <= pOperatorIK->m_Price + pOperatorKJ->m_Price)
			continue;

		CSuperCast* pSuperCast = AXL_MEM_NEW (CSuperCast);
		pSuperCast->m_pIntermediateType = m_pModule->m_TypeMgr.GetBasicType ((EType) k);
		pSuperCast->m_pFirst = pOperatorIK->m_pOperatorLo;
		pSuperCast->m_pSecond = pOperatorKJ->m_pOperatorLo;
		m_SuperCastList.InsertTail (pSuperCast);

		pOperatorIJ = AddCastOperator ((EType) i, (EType) j, pSuperCast);
		pOperatorIJ->m_Price = pOperatorIK->m_Price + pOperatorKJ->m_Price;
	}
}

CUnaryOperator*
COperatorMgr::AddUnaryOperator (
	EUnOp OpKind,
	EType ReturnTypeKind,
	EType OpTypeKind,
	IUnaryOperatorLo* pOperatorLo
	)
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetBasicType (ReturnTypeKind);
	CType* pOpType = m_pModule->m_TypeMgr.GetBasicType (OpTypeKind);
	return AddUnaryOperator (OpKind, pReturnType, pOpType, pOperatorLo);
}

bool
COperatorMgr::UnaryOperator (
	EUnOp OpKind,
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pOpType = OpValue.GetType ();

	CUnaryOperator* pOperator = FindUnaryOperator (OpKind, pOpType);	
	if (!pOperator)
	{
		err::SetFormatStringError (
			_T("unary '%s' cannot be applied to '%s'"),
			GetUnOpString (OpKind),
			pOpType->GetTypeString ()
			);
		return false;
	}
	
	return pOperator->Operator (OpValue, pResultValue);
}

bool
COperatorMgr::UnaryOperator (
	EUnOp OpKind,
	CValue* pValue
	)
{
	bool Result;

	CValue ResultValue;

	Result = UnaryOperator (OpKind, *pValue, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

CBinaryOperator*
COperatorMgr::AddBinaryOperator (
	EBinOp OpKind,
	EType ReturnTypeKind,
	EType OpTypeKind1,
	EType OpTypeKind2,
	IBinaryOperatorLo* pOperatorLo
	)
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetBasicType (ReturnTypeKind);
	CType* pOpType1 = m_pModule->m_TypeMgr.GetBasicType (OpTypeKind1);
	CType* pOpType2 = m_pModule->m_TypeMgr.GetBasicType (OpTypeKind2);
	return AddBinaryOperator (OpKind, pReturnType, pOpType1, pOpType2, pOperatorLo);
}

bool
COperatorMgr::BinaryOperator (
	EBinOp OpKind,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	CType* pOpType1 = OpValue1.GetType ();
	CType* pOpType2 = OpValue2.GetType ();

	CBinaryOperator* pOperator = FindBinaryOperator (OpKind, pOpType1, pOpType2);	
	if (!pOperator)
	{
		err::SetFormatStringError (
			_T("binary '%s' cannot be applied to '%s' and '%s'"),
			GetBinOpString (OpKind),
			pOpType1->GetTypeString (),
			pOpType2->GetTypeString ()
			);
		return false;
	}
	
	return pOperator->Operator (OpValue1, OpValue2, pResultValue);
}

bool
COperatorMgr::BinaryOperator (
	EBinOp OpKind,
	CValue* pValue,
	const CValue& OpValue2
	)
{
	bool Result;

	CValue ResultValue;

	Result = BinaryOperator (OpKind, *pValue, OpValue2, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

CCastOperator*
COperatorMgr::FindCastOperator (
	CType* pSrcType,
	CType* pDstType
	)
{
	rtl::CStringA Signature = pSrcType->GetSignature () + pDstType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, CCastOperator*> It = m_CastOperatorMap.Find (Signature);
	if (It)
		return It->m_Value;

	return NULL;
}

CCastOperator*
COperatorMgr::AddCastOperator (
	CType* pSrcType,
	CType* pDstType,
	ICastOperatorLo* pOperatorLo
	)
{
	rtl::CStringA Signature = pSrcType->GetSignature () + pDstType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, CCastOperator*> It = m_CastOperatorMap.Goto (Signature);
	if (It->m_Value)
	{
		CCastOperator* pOperator = It->m_Value;
		
		ASSERT (pOperator->m_Signature == Signature);
		pOperator->m_pOperatorLo = pOperatorLo;
		return pOperator;
	}

	CCastOperator* pOperator = AXL_MEM_NEW (CCastOperator);
	pOperator->m_Signature = Signature;
	pOperator->m_pSrcType = pSrcType;
	pOperator->m_pDstType = pDstType;
	pOperator->m_pOperatorLo = pOperatorLo;
	m_CastOperatorList.InsertTail (pOperator);
	It->m_Value = pOperator;

	EType DstTypeKind = pDstType->GetTypeKind ();
	EType SrcTypeKind = pSrcType->GetTypeKind ();
	if (DstTypeKind < EType__BasicTypeCount && SrcTypeKind < EType__BasicTypeCount)
		m_BasicCastOperatorTable [SrcTypeKind] [DstTypeKind] = pOperator;

	return pOperator;
}

CCastOperator*
COperatorMgr::AddCastOperator (
	EType SrcTypeKind,
	EType DstTypeKind,
	ICastOperatorLo* pOperatorLo
	)
{
	CType* pSrcType = m_pModule->m_TypeMgr.GetBasicType (SrcTypeKind);
	CType* pDstType = m_pModule->m_TypeMgr.GetBasicType (DstTypeKind);
	return AddCastOperator (pSrcType, pDstType, pOperatorLo);
}

llvm::Value*
COperatorMgr::LoadValue (const CValue& Value)
{
	EValue ValueKind = Value.GetValueKind ();
	ASSERT (ValueKind == EValue_Variable || ValueKind == EValue_LlvmRegister);

	llvm::Value* pLlvmValue = Value.GetLlvmValue ();
	if (ValueKind == EValue_Variable)
		pLlvmValue = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLoad (pLlvmValue);

	return pLlvmValue;
}

bool
COperatorMgr::MoveOperator (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	EValue SrcValueKind = SrcValue.GetValueKind ();
	EValue DstValueKind = DstValue.GetValueKind ();

	CType* pSrcType = SrcValue.GetType ();
	CType* pDstType = DstValue.GetType ();

	// TODO: take care of properties

	// TODO: take care of references

	// TODO: take care of const modifiers

	llvm::Value* pLlvmSrcValue = NULL;
	llvm::Value* pLlvmDstValue = DstValue.GetLlvmValue ();

	if (pSrcType->Cmp (pDstType) == 0)
	{
		pLlvmSrcValue = LoadValue (SrcValue);
	}
	else
	{
		CValue CastValue;
		bool Result = CastOperator (SrcValue, DstValue.GetType (), &CastValue);
		if (!Result)
			return false;

		pLlvmSrcValue = CastValue.GetLlvmValue ();
	}

	m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateStore (pLlvmSrcValue, pLlvmDstValue);
	return true;
}

bool
COperatorMgr::MoveOperator (
	const CValue& SrcValue,
	const CValue& DstValue,
	EBinOp OpKind
	)
{
	if (OpKind == EBinOp_None)
		return MoveOperator (SrcValue, DstValue);

	CValue RValue;
	return 
		BinaryOperator (OpKind, DstValue, SrcValue, &RValue) &&
		MoveOperator (RValue, DstValue);
}

bool
COperatorMgr::CastOperator (
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	CCastOperator* pOperator = FindCastOperator (OpValue.GetType (), pType);	
	if (!pOperator)
	{
		err::SetFormatStringError (
			_T("cannot convert from '%s' to '%s'"),
			OpValue.GetType (),
			pType->GetTypeString ()
			);
		return false;
	}
	
	return pOperator->Cast (OpValue, pType, pResultValue);
}

bool
COperatorMgr::CastOperator (
	CValue* pValue,
	CType* pType
	)
{
	bool Result;

	CValue ResultValue;

	Result = CastOperator (*pValue, pType, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

bool
COperatorMgr::ConditionalOperator (
	CValue* pOpValue,
	const CValue& TrueValue,
	const CValue& FalseValue
	)
{
	return true;
}

bool
COperatorMgr::MemberOperator (
	CValue* pValue,
	const tchar_t* pName
	)
{
	return true;
}

bool
COperatorMgr::CallOperator (
	CValue* pValue,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	return true;
}

bool
COperatorMgr::PostfixIncOperator (CValue* pValue)
{
	return true;
}

bool
COperatorMgr::PostfixDecOperator (CValue* pValue)
{
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
