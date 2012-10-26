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

	memset (m_BasicMoveOperatorTable, 0, sizeof (m_BasicMoveOperatorTable));
	m_MoveOperatorMap.Clear ();
	m_MoveOperatorList.Clear ();
	m_SuperMoveList.Clear ();
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

	AddMoveOperator (EType_Int8, EType_Int8, &m_Move_cpy);
	AddMoveOperator (EType_Int8, EType_Int8_u, &m_Move_cpy);
	AddMoveOperator (EType_Int8_u, EType_Int8, &m_Move_cpy);
	AddMoveOperator (EType_Int8_u, EType_Int8_u, &m_Move_cpy);
	AddMoveOperator (EType_Int16, EType_Int16, &m_Move_cpy);
	AddMoveOperator (EType_Int16, EType_Int16_u, &m_Move_cpy);
	AddMoveOperator (EType_Int16_u, EType_Int16, &m_Move_cpy);
	AddMoveOperator (EType_Int16_u, EType_Int16_u, &m_Move_cpy);
	AddMoveOperator (EType_Int32, EType_Int32, &m_Move_cpy);
	AddMoveOperator (EType_Int32, EType_Int32_u, &m_Move_cpy);
	AddMoveOperator (EType_Int32_u, EType_Int32, &m_Move_cpy);
	AddMoveOperator (EType_Int32_u, EType_Int32_u, &m_Move_cpy);
	AddMoveOperator (EType_Int64, EType_Int64, &m_Move_cpy);
	AddMoveOperator (EType_Int64, EType_Int64_u, &m_Move_cpy);
	AddMoveOperator (EType_Int64_u, EType_Int64, &m_Move_cpy);
	AddMoveOperator (EType_Int64_u, EType_Int64_u, &m_Move_cpy);

	AddMoveOperator (EType_Int16_be, EType_Int16_be, &m_Move_cpy);
	AddMoveOperator (EType_Int16_be, EType_Int16_beu, &m_Move_cpy);
	AddMoveOperator (EType_Int16_beu, EType_Int16_be, &m_Move_cpy);
	AddMoveOperator (EType_Int16_beu, EType_Int16_beu, &m_Move_cpy);
	AddMoveOperator (EType_Int32_be, EType_Int32_be, &m_Move_cpy);
	AddMoveOperator (EType_Int32_be, EType_Int32_beu, &m_Move_cpy);
	AddMoveOperator (EType_Int32_beu, EType_Int32_be, &m_Move_cpy);
	AddMoveOperator (EType_Int32_beu, EType_Int32_beu, &m_Move_cpy);
	AddMoveOperator (EType_Int64_be, EType_Int64_be, &m_Move_cpy);
	AddMoveOperator (EType_Int64_be, EType_Int64_beu, &m_Move_cpy);
	AddMoveOperator (EType_Int64_beu, EType_Int64_be, &m_Move_cpy);
	AddMoveOperator (EType_Int64_beu, EType_Int64_beu, &m_Move_cpy);

	// endianness swaps

	AddMoveOperator (EType_Int16, EType_Int16_be, &m_Move_i16_swp);
	AddMoveOperator (EType_Int16, EType_Int16_beu, &m_Move_i16_swp);
	AddMoveOperator (EType_Int16_u, EType_Int16_be, &m_Move_i16_swp);
	AddMoveOperator (EType_Int16_u, EType_Int16_beu, &m_Move_i16_swp);
	AddMoveOperator (EType_Int16_be, EType_Int16, &m_Move_i16_swp);
	AddMoveOperator (EType_Int16_be, EType_Int16_u, &m_Move_i16_swp);
	AddMoveOperator (EType_Int16_beu, EType_Int16, &m_Move_i16_swp);
	AddMoveOperator (EType_Int16_beu, EType_Int16_u, &m_Move_i16_swp);

	AddMoveOperator (EType_Int32, EType_Int32_be, &m_Move_i32_swp);
	AddMoveOperator (EType_Int32, EType_Int32_beu, &m_Move_i32_swp);
	AddMoveOperator (EType_Int32_u, EType_Int32_be, &m_Move_i32_swp);
	AddMoveOperator (EType_Int32_u, EType_Int32_beu, &m_Move_i32_swp);
	AddMoveOperator (EType_Int32_be, EType_Int32, &m_Move_i32_swp);
	AddMoveOperator (EType_Int32_be, EType_Int32_u, &m_Move_i32_swp);
	AddMoveOperator (EType_Int32_beu, EType_Int32, &m_Move_i32_swp);
	AddMoveOperator (EType_Int32_beu, EType_Int32_u, &m_Move_i32_swp);

	AddMoveOperator (EType_Int64, EType_Int64_be, &m_Move_i64_swp);
	AddMoveOperator (EType_Int64, EType_Int64_beu, &m_Move_i64_swp);
	AddMoveOperator (EType_Int64_u, EType_Int64_be, &m_Move_i64_swp);
	AddMoveOperator (EType_Int64_u, EType_Int64_beu, &m_Move_i64_swp);
	AddMoveOperator (EType_Int64_be, EType_Int64, &m_Move_i64_swp);
	AddMoveOperator (EType_Int64_be, EType_Int64_u, &m_Move_i64_swp);
	AddMoveOperator (EType_Int64_beu, EType_Int64, &m_Move_i64_swp);
	AddMoveOperator (EType_Int64_beu, EType_Int64_u, &m_Move_i64_swp);

	// integer truncations

	AddMoveOperator (EType_Int16, EType_Int8, &m_Move_int_trunc);
	AddMoveOperator (EType_Int16, EType_Int8_u, &m_Move_int_trunc);
	AddMoveOperator (EType_Int32, EType_Int8, &m_Move_int_trunc);
	AddMoveOperator (EType_Int32, EType_Int8_u, &m_Move_int_trunc);
	AddMoveOperator (EType_Int32, EType_Int16, &m_Move_int_trunc);
	AddMoveOperator (EType_Int32, EType_Int16_u, &m_Move_int_trunc);
	AddMoveOperator (EType_Int64, EType_Int8, &m_Move_int_trunc);
	AddMoveOperator (EType_Int64, EType_Int8_u, &m_Move_int_trunc);
	AddMoveOperator (EType_Int64, EType_Int16, &m_Move_int_trunc);
	AddMoveOperator (EType_Int64, EType_Int16_u, &m_Move_int_trunc);
	AddMoveOperator (EType_Int64, EType_Int32, &m_Move_int_trunc);
	AddMoveOperator (EType_Int64, EType_Int32_u, &m_Move_int_trunc);

	// integer extensions

	AddMoveOperator (EType_Int8, EType_Int16, &m_Move_int_ext);
	AddMoveOperator (EType_Int8, EType_Int32, &m_Move_int_ext);
	AddMoveOperator (EType_Int8, EType_Int64, &m_Move_int_ext);
	AddMoveOperator (EType_Int8_u, EType_Int16, &m_Move_int_ext_u);
	AddMoveOperator (EType_Int8_u, EType_Int32, &m_Move_int_ext_u);
	AddMoveOperator (EType_Int8_u, EType_Int64, &m_Move_int_ext_u);
	AddMoveOperator (EType_Int16, EType_Int32, &m_Move_int_ext);
	AddMoveOperator (EType_Int16, EType_Int64, &m_Move_int_ext);
	AddMoveOperator (EType_Int16_u, EType_Int32, &m_Move_int_ext_u);
	AddMoveOperator (EType_Int16_u, EType_Int64, &m_Move_int_ext_u);
	AddMoveOperator (EType_Int32, EType_Int64, &m_Move_int_ext);
	AddMoveOperator (EType_Int32_u, EType_Int64, &m_Move_int_ext_u);

	// floating extensions & truncations

	AddMoveOperator (EType_Float, EType_Float, &m_Move_cpy);
	AddMoveOperator (EType_Double, EType_Double, &m_Move_cpy);
	AddMoveOperator (EType_Float, EType_Double, &m_Move_f32_f64);
	AddMoveOperator (EType_Double, EType_Float, &m_Move_f64_f32);

	// integer to floating point

	AddMoveOperator (EType_Int32, EType_Float, &m_Move_i32_f32);
	AddMoveOperator (EType_Int32_u, EType_Float, &m_Move_i32u_f32);
	AddMoveOperator (EType_Int32, EType_Double, &m_Move_i32_f64);
	AddMoveOperator (EType_Int32_u, EType_Double, &m_Move_i32u_f64);
	AddMoveOperator (EType_Int64, EType_Float, &m_Move_i64_f32);
	AddMoveOperator (EType_Int64_u, EType_Float, &m_Move_i64u_f32);
	AddMoveOperator (EType_Int64, EType_Double, &m_Move_i64_f64);
	AddMoveOperator (EType_Int64_u, EType_Double, &m_Move_i64u_f64);

	// floating point to integer

	AddMoveOperator (EType_Float, EType_Int32, &m_Move_f32_i32);
	AddMoveOperator (EType_Double, EType_Int32, &m_Move_f64_i32);
	AddMoveOperator (EType_Float, EType_Int64, &m_Move_f32_i64);
	AddMoveOperator (EType_Double, EType_Int64, &m_Move_f64_i64);

	// build super moves for basic types (floyd-warshall)
	
	for (size_t k = 0; k < EType__BasicTypeCount; k++)
	for (size_t i = 0; i < EType__BasicTypeCount; i++)
	for (size_t j = 0; j < EType__BasicTypeCount; j++)
	{
		CMoveOperator* pOperatorIK = m_BasicMoveOperatorTable [i] [k];
		CMoveOperator* pOperatorKJ = m_BasicMoveOperatorTable [k] [j];
		if (!pOperatorIK || !pOperatorKJ)
			continue;

		CMoveOperator* pOperatorIJ = m_BasicMoveOperatorTable [i] [j];
		if (pOperatorIJ && pOperatorIJ->m_Price < pOperatorIK->m_Price + pOperatorKJ->m_Price)
			continue;

		CSuperMove* pSuperMove = AXL_MEM_NEW (CSuperMove);
		pSuperMove->m_pFirst = pOperatorIK;
		pSuperMove->m_pSecond = pOperatorKJ;
		m_SuperMoveList.InsertTail (pSuperMove);

		pOperatorIJ = AddMoveOperator ((EType) i, (EType) j, pSuperMove);
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

CMoveOperator*
COperatorMgr::FindMoveOperator (
	CType* pSrcType,
	CType* pDstType
	)
{
	rtl::CStringA Signature = pSrcType->GetSignature () + pDstType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, CMoveOperator*> It = m_MoveOperatorMap.Find (Signature);
	if (It)
		return It->m_Value;

	return NULL;
}

CMoveOperator*
COperatorMgr::AddMoveOperator (
	CType* pSrcType,
	CType* pDstType,
	IMoveOperatorLo* pOperatorLo
	)
{
	rtl::CStringA Signature = pSrcType->GetSignature () + pDstType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, CMoveOperator*> It = m_MoveOperatorMap.Goto (Signature);
	if (It->m_Value)
	{
		CMoveOperator* pOperator = It->m_Value;
		
		ASSERT (pOperator->m_Signature == Signature);
		pOperator->m_pOperatorLo = pOperatorLo;
		return pOperator;
	}

	CMoveOperator* pOperator = AXL_MEM_NEW (CMoveOperator);
	pOperator->m_Signature = Signature;
	pOperator->m_pSrcType = pSrcType;
	pOperator->m_pDstType = pDstType;
	pOperator->m_pOperatorLo = pOperatorLo;
	m_MoveOperatorList.InsertTail (pOperator);
	It->m_Value = pOperator;

	EType DstTypeKind = pDstType->GetTypeKind ();
	EType SrcTypeKind = pSrcType->GetTypeKind ();
	if (DstTypeKind < EType__BasicTypeCount && SrcTypeKind < EType__BasicTypeCount)
		m_BasicMoveOperatorTable [SrcTypeKind] [DstTypeKind] = pOperator;

	return pOperator;
}

CMoveOperator*
COperatorMgr::AddMoveOperator (
	EType SrcTypeKind,
	EType DstTypeKind,
	IMoveOperatorLo* pOperatorLo
	)
{
	CType* pSrcType = m_pModule->m_TypeMgr.GetBasicType (SrcTypeKind);
	CType* pDstType = m_pModule->m_TypeMgr.GetBasicType (DstTypeKind);
	return AddMoveOperator (pSrcType, pDstType, pOperatorLo);
}

bool
COperatorMgr::MoveOperator (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	// TODO: take care of properties

	// TODO: take care of references

	// TODO: take care of const modifiers

	CType* pSrcType = SrcValue.GetType ();
	CType* pDstType = DstValue.GetType ();

	CMoveOperator* pOperator = FindMoveOperator (pSrcType, pDstType);	
	if (!pOperator)
	{
		err::SetFormatStringError (
			_T("cannot convert from '%s' to '%s'"),
			pSrcType->GetTypeString (),
			pDstType->GetTypeString ()
			);
		return false;
	}
	
	return pOperator->Move (SrcValue, DstValue);
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
	EValue OpValueKind = OpValue.GetValueKind ();
	
	if (OpValueKind == EValue_Const)
	{
		return 
			pResultValue->CreateConst (pType) &&
			MoveOperator (OpValue, *pResultValue);
	}
	else
	{
		CVariable* pTemp = m_pModule->m_VariableMgr.CreateTempVariable (pType);
		pResultValue->SetVariable (pTemp);
		return MoveOperator (OpValue, *pResultValue);
	}
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
