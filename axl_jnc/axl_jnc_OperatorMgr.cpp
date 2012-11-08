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

	AddBinaryOperator (EBinOp_Eq, EType_Int32, &m_BinOp_Eq_i32);
	AddBinaryOperator (EBinOp_Eq, EType_Int64, &m_BinOp_Eq_i64);
	AddBinaryOperator (EBinOp_Eq, EType_Float, &m_BinOp_Eq_f32);
	AddBinaryOperator (EBinOp_Eq, EType_Double, &m_BinOp_Eq_f64);

	AddBinaryOperator (EBinOp_Ne, EType_Int32, &m_BinOp_Ne_i32);
	AddBinaryOperator (EBinOp_Ne, EType_Int64, &m_BinOp_Ne_i64);
	AddBinaryOperator (EBinOp_Ne, EType_Float, &m_BinOp_Ne_f32);
	AddBinaryOperator (EBinOp_Ne, EType_Double, &m_BinOp_Ne_f64);

	AddBinaryOperator (EBinOp_Lt, EType_Int32, &m_BinOp_Lt_i32);
	AddBinaryOperator (EBinOp_Lt, EType_Int64, &m_BinOp_Lt_i64);
	AddBinaryOperator (EBinOp_Lt, EType_Float, &m_BinOp_Lt_f32);
	AddBinaryOperator (EBinOp_Lt, EType_Double, &m_BinOp_Lt_f64);
	AddBinaryOperator (EBinOp_Lt, EType_Int32_u, &m_BinOp_Lt_i32u);
	AddBinaryOperator (EBinOp_Lt, EType_Int64_u, &m_BinOp_Lt_i64u);

	AddBinaryOperator (EBinOp_Le, EType_Int32, &m_BinOp_Le_i32);
	AddBinaryOperator (EBinOp_Le, EType_Int64, &m_BinOp_Le_i64);
	AddBinaryOperator (EBinOp_Le, EType_Float, &m_BinOp_Le_f32);
	AddBinaryOperator (EBinOp_Le, EType_Double, &m_BinOp_Le_f64);
	AddBinaryOperator (EBinOp_Le, EType_Int32_u, &m_BinOp_Le_i32u);
	AddBinaryOperator (EBinOp_Le, EType_Int64_u, &m_BinOp_Le_i64u);

	AddBinaryOperator (EBinOp_Gt, EType_Int32, &m_BinOp_Gt_i32);
	AddBinaryOperator (EBinOp_Gt, EType_Int64, &m_BinOp_Gt_i64);
	AddBinaryOperator (EBinOp_Gt, EType_Float, &m_BinOp_Gt_f32);
	AddBinaryOperator (EBinOp_Gt, EType_Double, &m_BinOp_Gt_f64);
	AddBinaryOperator (EBinOp_Gt, EType_Int32_u, &m_BinOp_Gt_i32u);
	AddBinaryOperator (EBinOp_Gt, EType_Int64_u, &m_BinOp_Gt_i64u);

	AddBinaryOperator (EBinOp_Ge, EType_Int32, &m_BinOp_Ge_i32);
	AddBinaryOperator (EBinOp_Ge, EType_Int64, &m_BinOp_Ge_i64);
	AddBinaryOperator (EBinOp_Ge, EType_Float, &m_BinOp_Ge_f32);
	AddBinaryOperator (EBinOp_Ge, EType_Double, &m_BinOp_Ge_f64);
	AddBinaryOperator (EBinOp_Ge, EType_Int32_u, &m_BinOp_Ge_i32u);
	AddBinaryOperator (EBinOp_Ge, EType_Int64_u, &m_BinOp_Ge_i64u);
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

	// integer to bool point (after building super moves!)

	for (size_t i = EType_Int8; i < EType_Int64_u; i++)
	{
		AddCastOperator ((EType) i, EType_Bool, &m_Cast_int_bool);	
		AddCastOperator (EType_Bool, (EType) i, &m_Cast_bool_int);	
	}

	for (size_t i = EType_Int16_be; i < EType_Int64_beu; i++)
	{
		AddCastOperator ((EType) i, EType_Bool, &m_Cast_int_bool);	
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

	case EUnOp_PreInc:
		pOperator = &m_UnOp_preinc;
		break;

	case EUnOp_PreDec:
		pOperator = &m_UnOp_predec;
		break;

	case EUnOp_PostInc:
		pOperator = &m_UnOp_postinc;
		break;

	case EUnOp_PostDec:
		pOperator = &m_UnOp_postdec;
		break;
	}

	if (!pOperator)
		return NULL;

	bool Result = pOperator->GetTypeInfo (pOpType, pTypeInfo);
	return Result ? pOperator : NULL;
}

bool // unqualify, get property, peel double reference
COperatorMgr::PrepareOperand (
	const CValue& OpValue,
	CValue* pOpValue
	)
{
	CValue Value = OpValue;

	for (;;)
	{
		CType* pType = Value.GetType ();

		EType TypeKind = Value.GetType ()->GetTypeKind ();
		switch (TypeKind)
		{			
		case EType_Qualifier:
			pType = ((CQualifierType*) pType)->GetBaseType ();
			Value.OverrideType (pType);
			break;

		case EType_Reference:
		case EType_Reference_u:
		case EType_Reference_d:
			pType = ((CPointerType*) pType)->GetBaseType ();
			break;

		case EType_Property:
			break;
		}

		break;
	}

	*pOpValue = Value;
	return true;
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
		pOperator->ConstOperator (CastOpValue1, CastOpValue2, pResultValue) :
		pOperator->LlvmOperator (CastOpValue1, CastOpValue2, pResultValue);
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
COperatorMgr::GetCastOperator (
	CType* pSrcType,
	CType* pDstType
	)
{
	if (pSrcType->Cmp (pDstType) == 0)
		return &m_Cast_cpy;

	EType SrcTypeKind = pSrcType->GetTypeKind ();
	EType DstTypeKind = pDstType->GetTypeKind ();

	switch (SrcTypeKind)
	{
	case EType_Reference:
	case EType_Reference_u:
		return GetReferenceCastOperator ((CPointerType*) pSrcType, pDstType);

	case EType_Pointer:
		return GetPointerCastOperator ((CPointerType*) pSrcType, pDstType);

	case EType_Property:
		return GetPropertyCastOperator ((CPropertyType*) pSrcType, pDstType);
	}

	rtl::CStringA Signature = pSrcType->GetSignature () + pDstType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, ICastOperator*> It = m_CastOperatorMap.Find (Signature);
	if (It)
		return It->m_Value;

	return NULL;
}

ICastOperator*
COperatorMgr::GetReferenceCastOperator (
	CPointerType* pSrcType,
	CType* pDstType
	)
{
	CType* pSrcBaseType = pSrcType->GetBaseType ();

	if (pSrcBaseType->GetTypeKind () == EType_Array && pDstType->GetTypeKind () == EType_Pointer_u)
	{
		CArrayType* pArrayType = (CArrayType*) pSrcBaseType;
		CPointerType* pPointerType = (CPointerType*) pDstType;

		ECast CastKind = GetCastKind (pArrayType->GetBaseType (), pPointerType->GetBaseType ());
		return CastKind ? &m_Cast_arr_ptr_c : NULL;
	}

	ECast CastKind = GetCastKind (pSrcBaseType, pDstType);
	return CastKind ? &m_Cast_load : NULL;
}

ICastOperator*
COperatorMgr::GetPointerCastOperator (
	CPointerType* pSrcType,
	CType* pDstType
	)
{
	if (!pDstType->IsPointerType ())
		return NULL;

	if (pSrcType->GetTypeKind () == EType_Pointer_u && pDstType->GetTypeKind () == EType_Pointer)
		return NULL;

	#pragma AXL_TODO ("also check that we do not cast non-POD structs")
	
	return &m_Cast_ptr;
}

ICastOperator*
COperatorMgr::GetPropertyCastOperator (
	CPropertyType* pSrcType,
	CType* pDstType
	)
{
	CType* pReturnType = pSrcType->GetGetterType ()->GetReturnType ();
	ECast CastKind = GetCastKind (pReturnType, pDstType);
	return CastKind ? &m_Cast_getp : NULL;
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

	EType SrcTypeKind = pSrcType->GetTypeKind ();
	EType DstTypeKind = pDstType->GetTypeKind ();

	if (SrcTypeKind < EType__BasicTypeCount && DstTypeKind < EType__BasicTypeCount)
		m_BasicCastOperatorTable [SrcTypeKind] [DstTypeKind] = pOperator;

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
	ICastOperator* pOperator = GetCastOperator (pSrcType, pDstType);
	return pOperator ? pOperator->GetCastKind (pSrcType, pDstType) : ECast_None;
}

bool
COperatorMgr::CastOperator (
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ICastOperator* pOperator = GetCastOperator (OpValue.GetType (), pType);	
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
COperatorMgr::CastOperator (
	const CValue& OpValue,
	EType TypeKind,
	CValue* pResultValue
	)
{
	CType* pType = m_pModule->m_TypeMgr.GetBasicType (TypeKind);
	return CastOperator (OpValue, pType, pResultValue);
}

bool
COperatorMgr::CastOperator (
	CValue* pValue,
	EType TypeKind
	)
{
	CType* pType = m_pModule->m_TypeMgr.GetBasicType (TypeKind);
	return CastOperator (pValue, pType);
}

bool
COperatorMgr::MoveOperator (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CType* pDstType = DstValue.GetType ();
	EType DstTypeKind = pDstType->GetTypeKind ();

	switch (DstTypeKind)
	{
	case EType_Reference:
	case EType_Reference_u:
		return ReferenceMoveOperator (SrcValue, DstValue, (CPointerType*) pDstType);

	case EType_Property:
		return PropertyMoveOperator (SrcValue, DstValue.GetProperty ());

	default:
		err::SetFormatStringError (_T("left operand must be l-value"));
		return false;
	}
}

bool
COperatorMgr::ReferenceMoveOperator (
	const CValue& SrcValue,
	const CValue& DstValue,
	CPointerType* pDstType
	)
{
	EType DstTypeKind = pDstType->GetTypeKind ();
	ASSERT (DstTypeKind == EType_Reference || DstTypeKind == EType_Reference_u);

	CType* pTargetType = pDstType->GetBaseType ();
	EType TargetTypeKind = pTargetType->GetTypeKind ();

	CValue CastValue;

	switch (TargetTypeKind)
	{
	case EType_Reference:
	case EType_Reference_u:
	case EType_Reference_d:
		return 
			CastOperator (DstValue, pTargetType, &CastValue) &&
			MoveOperator (SrcValue, CastValue);
	}
	
	bool Result = CastOperator (SrcValue, pTargetType, &CastValue);
	if (!Result)
		return false;

	if ((TargetTypeKind == EType_Pointer || TargetTypeKind == EType_Pointer_d) && 
		CastValue.GetValueKind () == EValue_Variable)
		return VariableToFatPointerMoveOperator (CastValue.GetVariable (), DstValue);

	m_pModule->m_LlvmBuilder.CreateStore (
		CastValue.GetLlvmValue (), 
		DstValue.GetLlvmValue ()
		);

	return true;
}

bool
COperatorMgr::VariableToFatPointerMoveOperator (
	CVariable* pVariable,
	const CValue& DstValue
	)
{
	CType* pCharPointerType = m_pModule->m_TypeMgr.GetPointerType (EType_Int8, EType_Pointer_u);
	
	llvm::Value* pLlvmVariable = m_pModule->m_LlvmBuilder.CreateBitCast (
		pVariable->GetLlvmValue (),
		pCharPointerType->GetLlvmType ()
		);
	
	CType* pType = pVariable->GetType ();
	llvm::Value* pLlvmTypePtr = CValue (pCharPointerType, &pType).GetLlvmValue ();

	llvm::Value* pLlvmFatPointer = llvm::UndefValue::get (m_pModule->m_TypeMgr.GetLlvmFatPointerType ());

	pLlvmFatPointer = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmFatPointer, pLlvmVariable, 0);
	pLlvmFatPointer = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmFatPointer, pLlvmTypePtr, 1);
	pLlvmFatPointer = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmFatPointer, pLlvmVariable, 2);
	pLlvmFatPointer = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmFatPointer, pLlvmTypePtr, 3);

	m_pModule->m_LlvmBuilder.CreateStore (
		pLlvmFatPointer, 
		DstValue.GetLlvmValue ()
		);

	return true;
}

bool
COperatorMgr::PropertyMoveOperator (
	const CValue& SrcValue,
	CProperty* pProperty
	)
{
	CFunctionOverload* pSetter = pProperty->GetSetter ();
	if (!pSetter)
	{
		err::SetFormatStringError (_T("cannot move to a read-only property"));
		return false;
	}

	err::SetFormatStringError (_T("setting properies not implemented yet"));
	return false;
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
	CType* pType = OpValue.GetType ();

	if (pType->IsReferenceType ())
		pType = ((CPointerType*) pType)->GetBaseType ();

	if (pType->IsPointerType () || pType->IsReferenceType ())
		pType = ((CPointerType*) pType)->GetBaseType ();

	EType TypeKind = pType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_Struct:
	case EType_Union:
		return StructMemberOperator (OpValue, (CStructType*) pType, pName, pResultValue);

	case EType_Interface:
	case EType_Class:
		return ClassMemberOperator (OpValue, (CClassType*) pType, pName, pResultValue);

	default:
		err::SetFormatStringError (_T("member operator cannot be applied to '%s'"), pType->GetTypeString ());
		return false;
	}
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
COperatorMgr::StructMemberOperator (
	const CValue& OpValue,
	CStructType* pType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CStructMember* pMember = ((CStructType*) pType)->FindMember (pName);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pType->GetTypeString ());
		return false;
	}

	EValue OpValueKind = OpValue.GetValueKind ();
	if (OpValueKind == EValue_Const)
	{
		pResultValue->CreateConst (
			pMember->GetType (), 
			(char*) OpValue.GetConstData () + pMember->GetOffset ()
			);

		return true;
	}

	if (!OpValue.GetType ()->IsReferenceType ())
	{
		llvm::Value* pLlvmMember = m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue.GetLlvmValue (), pMember->GetLlvmIndex ());
		pResultValue->SetLlvmRegister (pLlvmMember, pMember->GetType ());
		return true;
	}
	else
	{
		CValue Zero;
		Zero.SetConstInt32 (0, EType_Int32);

		CValue Index;
		Index.SetConstInt32 (pMember->GetLlvmIndex (), EType_Int32);

		llvm::Value* pLlvmValue = OpValue.GetLlvmValue ();
		llvm::Value* pLlvmZero = Zero.GetLlvmValue ();
		llvm::Value* pLlvmIndex = Index.GetLlvmValue ();

		llvm::Value* LlvmIndexArray [] =
		{
			pLlvmZero,
			pLlvmIndex,
		};

		llvm::Value* pLlvmGep = m_pModule->m_LlvmBuilder.CreateGEP (
			pLlvmValue, 
			llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, 2)
			);

		pResultValue->SetLlvmRegister (pLlvmGep, pMember->GetType ()->GetPointerType (EType_Reference_u));
		return true;
	}
}

bool
COperatorMgr::ClassMemberOperator (
	const CValue& OpValue,
	CClassType* pType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CModuleItem* pClassMember = pType->FindMember (pName);
	if (!pClassMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pType->GetTypeString ());
		return false;
	}

	err::SetFormatStringError (_T("class member operator is not implemented yet"));
	return false;
}

CType*
GetVarArgType (
	CType* pType,
	bool IsUnsafeVarArg
	)
{
	EType TypeKind = pType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_Reference:
	case EType_Reference_u:
		return ((CPointerType*) pType)->GetBaseType ();

	default:
		return pType;
	}
}

bool
COperatorMgr::CallOperator (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	bool Result;

	CFunction* pFunction;

	EValue ValueKind = OpValue.GetValueKind ();
	switch (ValueKind)
	{
	case EValue_Function:
		pFunction = OpValue.GetFunction ();
		break;

	case EValue_FunctionOverload:
		// TODO: find overload based on arg list
		pFunction = OpValue.GetFunctionOverload ()->GetFunction ();
		break;

	default:
		err::SetFormatStringError (_T("cannot call %s"), OpValue.GetValueKindString ());
		return false;
	}

	CFunctionType* pFunctionType = pFunction->GetType ();
	llvm::Function* pLlvmFunction = pFunction->GetLlvmFunction ();
	
	size_t FormalArgCount = pFunctionType->GetArgCount ();
	size_t ActualArgCount = pArgList->GetCount ();

	bool IsVarArg = (pFunctionType->GetFlags () & EFunctionTypeFlag_IsVarArg) != 0;
	bool IsUnsafeVarArg = (pFunctionType->GetFlags () & EFunctionTypeFlag_IsUnsafeVarArg) != 0;

	if (IsVarArg && ActualArgCount < FormalArgCount ||
		!IsVarArg && ActualArgCount != FormalArgCount)
	{
		err::SetFormatStringError (_T("function '%s' takes %d arguments; %d passed"), pFunction->GetTag (), FormalArgCount, ActualArgCount);
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

		LlvmArgArray.Append (ArgCast.GetLlvmValue ());
	}

	// vararg

	for (; Arg; Arg++)
	{
		CType* pFormalArgType = GetVarArgType (Arg->GetType (), IsUnsafeVarArg);

		CValue ArgCast;
		Result = CastOperator (*Arg, pFormalArgType, &ArgCast);
		if (!Result)
			return false;

		LlvmArgArray.Append (ArgCast.GetLlvmValue ());
	}

	llvm::Instruction* pLlvmCall = m_pModule->m_LlvmBuilder.CreateCall (
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

//.............................................................................

} // namespace axl {
} // namespace jnc {
