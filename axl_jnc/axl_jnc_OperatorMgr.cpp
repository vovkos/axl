#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

COperatorMgr::COperatorMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
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
	m_SuperCastList.Clear ();
}

void
COperatorMgr::AddStdOperators ()
{
	AddStdCastOperators ();
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
COperatorMgr::AddStdCastOperators ()
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

	AddCastOperator (EType_Int16, EType_Int16_be, &m_Cast_int_swp);
	AddCastOperator (EType_Int16, EType_Int16_beu, &m_Cast_int_swp);
	AddCastOperator (EType_Int16_u, EType_Int16_be, &m_Cast_int_swp);
	AddCastOperator (EType_Int16_u, EType_Int16_beu, &m_Cast_int_swp);
	AddCastOperator (EType_Int16_be, EType_Int16, &m_Cast_int_swp);
	AddCastOperator (EType_Int16_be, EType_Int16_u, &m_Cast_int_swp);
	AddCastOperator (EType_Int16_beu, EType_Int16, &m_Cast_int_swp);
	AddCastOperator (EType_Int16_beu, EType_Int16_u, &m_Cast_int_swp);

	AddCastOperator (EType_Int32, EType_Int32_be, &m_Cast_int_swp);
	AddCastOperator (EType_Int32, EType_Int32_beu, &m_Cast_int_swp);
	AddCastOperator (EType_Int32_u, EType_Int32_be, &m_Cast_int_swp);
	AddCastOperator (EType_Int32_u, EType_Int32_beu, &m_Cast_int_swp);
	AddCastOperator (EType_Int32_be, EType_Int32, &m_Cast_int_swp);
	AddCastOperator (EType_Int32_be, EType_Int32_u, &m_Cast_int_swp);
	AddCastOperator (EType_Int32_beu, EType_Int32, &m_Cast_int_swp);
	AddCastOperator (EType_Int32_beu, EType_Int32_u, &m_Cast_int_swp);

	AddCastOperator (EType_Int64, EType_Int64_be, &m_Cast_int_swp);
	AddCastOperator (EType_Int64, EType_Int64_beu, &m_Cast_int_swp);
	AddCastOperator (EType_Int64_u, EType_Int64_be, &m_Cast_int_swp);
	AddCastOperator (EType_Int64_u, EType_Int64_beu, &m_Cast_int_swp);
	AddCastOperator (EType_Int64_be, EType_Int64, &m_Cast_int_swp);
	AddCastOperator (EType_Int64_be, EType_Int64_u, &m_Cast_int_swp);
	AddCastOperator (EType_Int64_beu, EType_Int64, &m_Cast_int_swp);
	AddCastOperator (EType_Int64_beu, EType_Int64_u, &m_Cast_int_swp);

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
		ICastOperator* pOperatorIK = m_BasicCastOperatorTable [i] [k];
		ICastOperator* pOperatorKJ = m_BasicCastOperatorTable [k] [j];
		if (!pOperatorIK || !pOperatorKJ)
			continue;

		size_t SuperPrice = pOperatorIK->GetPrice () + pOperatorKJ->GetPrice ();

		ICastOperator* pOperatorIJ = m_BasicCastOperatorTable [i] [j];
		if (pOperatorIJ && pOperatorIJ->GetPrice () <= SuperPrice)
			continue;

		CSuperCast* pSuperCast = AXL_MEM_NEW (CSuperCast);
		pSuperCast->m_pIntermediateType = m_pModule->m_TypeMgr.GetBasicType ((EType) k);
		pSuperCast->m_pFirst = pOperatorIK;
		pSuperCast->m_pSecond = pOperatorKJ;
		pSuperCast->m_Price = SuperPrice;
		m_SuperCastList.InsertTail (pSuperCast);

		pOperatorIJ = AddCastOperator ((EType) i, (EType) j, pSuperCast);
	}
}

IUnaryOperator*
COperatorMgr::GetUnaryOperator (
	EUnOp OpKind,
	CType* pOpType,
	TUnaryOperatorTypeInfo* pTypeInfo
	)
{
	ASSERT (OpKind > 0 && OpKind < EUnOp__Count);
	IUnaryOperator* pOperator = m_UnaryOperatorTable [OpKind].GetOperator (pOpType, pTypeInfo);
	if (pOperator)
		return pOperator;

	switch (OpKind)
	{
	case EUnOp_Addr:
		pOperator = &m_UnOp_addr;
		break;

	case EUnOp_Indir:
		pOperator = &m_UnOp_indir;
		break;
	}

	if (!pOperator)
		return NULL;

	bool Result = pOperator->GetTypeInfo (pOpType, pTypeInfo);
	return Result ? pOperator : NULL;
}


IUnaryOperator*
COperatorMgr::AddUnaryOperator (
	EUnOp OpKind,
	EType OpTypeKind,
	IUnaryOperator* pOperator
	)
{
	CType* pOpType = m_pModule->m_TypeMgr.GetBasicType (OpTypeKind);
	return AddUnaryOperator (OpKind, pOpType, pOperator);
}

bool
COperatorMgr::UnaryOperator (
	EUnOp OpKind,
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pOpType = OpValue.GetType ();

	TUnaryOperatorTypeInfo TypeInfo;
	IUnaryOperator* pOperator = GetUnaryOperator (OpKind, pOpType, &TypeInfo);	
	if (!pOperator)
	{
		err::SetFormatStringError (
			_T("unary '%s' cannot be applied to '%s'"),
			GetUnOpString (OpKind),
			pOpType->GetTypeString ()
			);
		return false;
	}

	CValue CastOpValue;
	bool Result = CastOperator (OpValue, TypeInfo.m_pOpType, &CastOpValue);
	if (!Result)
		return false;
	
	EValue OpValueKind = CastOpValue.GetValueKind ();
	return OpValueKind == EValue_Const ?
		pOperator->ConstOperator (CastOpValue, pResultValue) :
		pOperator->LlvmOperator (CastOpValue, pResultValue);
}

bool
COperatorMgr::UnaryOperator (
	EUnOp OpKind,
	CValue* pValue
	)
{
	CValue ResultValue;

	bool Result = UnaryOperator (OpKind, *pValue, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

IBinaryOperator*
COperatorMgr::AddBinaryOperator (
	EBinOp OpKind,
	EType OpTypeKind1,
	EType OpTypeKind2,
	IBinaryOperator* pOperator
	)
{
	CType* pOpType1 = m_pModule->m_TypeMgr.GetBasicType (OpTypeKind1);
	CType* pOpType2 = m_pModule->m_TypeMgr.GetBasicType (OpTypeKind2);
	return AddBinaryOperator (OpKind, pOpType1, pOpType2, pOperator);
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

	TBinaryOperatorTypeInfo TypeInfo;
	IBinaryOperator* pOperator = GetBinaryOperator (OpKind, pOpType1, pOpType2, &TypeInfo);	
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
	
	CValue CastOpValue1;
	CValue CastOpValue2;

	bool Result = 
		CastOperator (OpValue1, TypeInfo.m_pOpType1, &CastOpValue1) &&
		CastOperator (OpValue2, TypeInfo.m_pOpType2, &CastOpValue2);

	if (!Result)
		return false;
	
	EValue OpValueKind1 = CastOpValue1.GetValueKind ();
	EValue OpValueKind2 = CastOpValue2.GetValueKind ();
	
	return OpValueKind1 == EValue_Const && OpValueKind2 == EValue_Const ?
		pOperator->ConstOperator (OpValue1, OpValue2, pResultValue) :
		pOperator->LlvmOperator (OpValue1, OpValue2, pResultValue);
}

bool
COperatorMgr::BinaryOperator (
	EBinOp OpKind,
	CValue* pValue,
	const CValue& OpValue2
	)
{
	CValue ResultValue;

	bool Result = BinaryOperator (OpKind, *pValue, OpValue2, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

ICastOperator*
COperatorMgr::FindCastOperator (
	CType* pSrcType,
	CType* pDstType
	)
{
	rtl::CStringA Signature = pSrcType->GetSignature () + pDstType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, ICastOperator*> It = m_CastOperatorMap.Find (Signature);
	if (It)
		return It->m_Value;

	// TODO: do proper special cases of casting

	EType SrcTypeKind = pSrcType->GetTypeKind ();
	EType DstTypeKind = pDstType->GetTypeKind ();
	
	if (SrcTypeKind == EType_Array && DstTypeKind == EType_Pointer_c)
		return &m_Cast_arr_ptr_c;

	return NULL;
}

ICastOperator*
COperatorMgr::AddCastOperator (
	CType* pSrcType,
	CType* pDstType,
	ICastOperator* pOperator
	)
{
	rtl::CStringA Signature = pSrcType->GetSignature () + pDstType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, ICastOperator*> It = m_CastOperatorMap.Goto (Signature);

	ICastOperator* pPrevOperator = It->m_Value;
	if (!pPrevOperator)
		m_CastSignatureCache.InsertTail (Signature);

	It->m_Value = pOperator;
	return pPrevOperator;
}

ICastOperator*
COperatorMgr::AddCastOperator (
	EType SrcTypeKind,
	EType DstTypeKind,
	ICastOperator* pOperator
	)
{
	CType* pSrcType = m_pModule->m_TypeMgr.GetBasicType (SrcTypeKind);
	CType* pDstType = m_pModule->m_TypeMgr.GetBasicType (DstTypeKind);
	return AddCastOperator (pSrcType, pDstType, pOperator);
}

ECast
COperatorMgr::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	ICastOperator* pOperator = FindCastOperator (pSrcType, pDstType);
	return pOperator ? pOperator->GetCastKind (pSrcType, pDstType) : ECast_None;
}

bool
COperatorMgr::CastOperator (
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	if (pType->Cmp (OpValue.GetType ()) == 0)
	{
		*pResultValue = OpValue;
		return true;
	}

	ICastOperator* pOperator = FindCastOperator (OpValue.GetType (), pType);	
	if (!pOperator)
	{
		err::SetFormatStringError (
			_T("cannot convert from '%s' to '%s'"),
			OpValue.GetType ()->GetTypeString (),
			pType->GetTypeString ()
			);
		return false;
	}
	
	EValue OpValueKind = OpValue.GetValueKind ();
	if (OpValueKind == EValue_Const)
	{
		return 
			pResultValue->CreateConst (pType) &&
			pOperator->ConstCast (OpValue, *pResultValue);
	}

	ASSERT (
		OpValueKind == EValue_Const || 
		OpValueKind == EValue_Variable || 
		OpValueKind == EValue_LlvmRegister);

	return pOperator->LlvmCast (OpValue, pType, pResultValue);
}

bool
COperatorMgr::CastOperator (
	CValue* pValue,
	CType* pType
	)
{
	CValue ResultValue;

	bool Result = CastOperator (*pValue, pType, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
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
	const CValue& OpValue,
	const tchar_t* pName,
	CValue* pResultValue
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
	CValue ResultValue;

	bool Result = MemberOperator (*pValue, pName, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

bool
COperatorMgr::CallOperator (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	bool Result;

	EValue ValueKind = OpValue.GetValueKind ();
	if (ValueKind != EValue_GlobalFunction)
	{
		err::SetFormatStringError (_T("cannot call %s"), OpValue.GetValueKindString ());
		return false;
	}

	CGlobalFunction* pGlobalFunction = OpValue.GetGlobalFunction ();

	// TODO: find overload based on arg list

	CFunction* pFunction = pGlobalFunction->GetFunction ();
	CFunctionType* pFunctionType = pFunction->GetType ();
	llvm::Function* pLlvmFunction = pFunction->GetLlvmFunction ();
	
	size_t FormalArgCount = pFunctionType->GetArgCount ();
	size_t ActualArgCount = pArgList->GetCount ();

	bool IsVarArg = pFunctionType->IsVarArg ();
	if (IsVarArg && ActualArgCount < FormalArgCount ||
		!IsVarArg && ActualArgCount != FormalArgCount)
	{
		err::SetFormatStringError (_T("function '%s' takes %d arguments; %d passed"), pGlobalFunction->GetName (), FormalArgCount, ActualArgCount);
		return false;
	}

	char LlvmArgBuffer [256];
	rtl::CArrayT <llvm::Value*> LlvmArgArray (ref::EBuf_Stack, LlvmArgBuffer, sizeof (LlvmArgBuffer));
	
	rtl::CBoxIteratorT <CValue> Arg = pArgList->GetHead ();
	for (size_t i = 0; i < FormalArgCount; Arg++, i++)
	{
		CType* pFormalArgType = pFunctionType->GetArgType (i);
		CValue ArgCast;

		Result = CastOperator (*Arg, pFormalArgType, &ArgCast);
		if (!Result)
			return false;

		llvm::Value* pLlvmArg = LoadValue (ArgCast);
		LlvmArgArray.Append (pLlvmArg);
	}

	// vararg

	for (; Arg; Arg++)
	{
		llvm::Value* pLlvmArg = LoadValue (*Arg);
		LlvmArgArray.Append (pLlvmArg);
	}

	llvm::Instruction* pLlvmCall = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateCall (
		pLlvmFunction,
		llvm::ArrayRef <llvm::Value*> (LlvmArgArray, LlvmArgArray.GetCount ())
		);
		
	CType* pReturnType = pFunction->GetType ()->GetReturnType ();
	pResultValue->SetLlvmRegister (pLlvmCall, pReturnType);
	return true;
}

bool
COperatorMgr::CallOperator (
	CValue* pValue,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	CValue ResultValue;

	bool Result = CallOperator (*pValue, pArgList, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
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

llvm::Value*
COperatorMgr::LoadValue (const CValue& Value)
{
	EValue ValueKind = Value.GetValueKind ();
	ASSERT (ValueKind == EValue_Const || ValueKind == EValue_Variable || ValueKind == EValue_LlvmRegister);

	llvm::Value* pLlvmValue = Value.GetLlvmValue ();
	if (ValueKind == EValue_Variable)
		pLlvmValue = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLoad (pLlvmValue);

	return pLlvmValue;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
