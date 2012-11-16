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

	memset (m_UnaryOperatorTable, 0, sizeof (m_UnaryOperatorTable));
	memset (m_BinaryOperatorTable, 0, sizeof (m_BinaryOperatorTable));
	memset (m_CastOperatorTable, 0, sizeof (m_CastOperatorTable));

	// unary operators

	m_UnaryOperatorTable [EUnOp_Minus]      = &m_UnOp_Minus;
	m_UnaryOperatorTable [EUnOp_BitwiseNot] = &m_UnOp_BitwiseNot;
	m_UnaryOperatorTable [EUnOp_Addr]       = &m_UnOp_Addr;
	m_UnaryOperatorTable [EUnOp_Indir]      = &m_UnOp_Indir;
	m_UnaryOperatorTable [EUnOp_LogicalNot] = NULL; // &m_UnOp_LogicalNot;
	m_UnaryOperatorTable [EUnOp_PreInc]     = &m_UnOp_PreInc;
	m_UnaryOperatorTable [EUnOp_PreDec]     = &m_UnOp_PreDec;
	m_UnaryOperatorTable [EUnOp_PostInc]    = &m_UnOp_PostInc;
	m_UnaryOperatorTable [EUnOp_PostDec]    = &m_UnOp_PostDec;
	m_UnaryOperatorTable [EUnOp_Ptr]        = NULL; // &m_UnOp_Ptr;

	// binary operators

	m_BinaryOperatorTable [EBinOp_Add]        = &m_BinOp_Add;
	m_BinaryOperatorTable [EBinOp_Sub]        = &m_BinOp_Sub;
	m_BinaryOperatorTable [EBinOp_Mul]        = &m_BinOp_Mul;
	m_BinaryOperatorTable [EBinOp_Div]        = &m_BinOp_Div;
	m_BinaryOperatorTable [EBinOp_Mod]        = &m_BinOp_Mod;
	m_BinaryOperatorTable [EBinOp_Shl]        = &m_BinOp_Shl;
	m_BinaryOperatorTable [EBinOp_Shr]        = &m_BinOp_Shr;
	m_BinaryOperatorTable [EBinOp_BitwiseAnd] = &m_BinOp_BitwiseAnd;
	m_BinaryOperatorTable [EBinOp_BitwiseOr]  = &m_BinOp_BitwiseOr;
	m_BinaryOperatorTable [EBinOp_BitwiseXor] = &m_BinOp_BitwiseXor;
	m_BinaryOperatorTable [EBinOp_Eq]         = &m_BinOp_Eq;
	m_BinaryOperatorTable [EBinOp_Ne]         = &m_BinOp_Ne;
	m_BinaryOperatorTable [EBinOp_Lt]         = &m_BinOp_Lt;
	m_BinaryOperatorTable [EBinOp_Le]         = &m_BinOp_Le;
	m_BinaryOperatorTable [EBinOp_Gt]         = &m_BinOp_Gt;
	m_BinaryOperatorTable [EBinOp_Ge]         = &m_BinOp_Ge;
	m_BinaryOperatorTable [EBinOp_LogicalAnd] = NULL; // &m_BinOp_LogicalAnd;
	m_BinaryOperatorTable [EBinOp_LogicalOr]  = NULL; // &m_BinOp_LogicalOr;
	m_BinaryOperatorTable [EBinOp_Idx]        = NULL; // &m_BinOp_Idx;

	// integer copies

	m_CastOperatorTable [EType_Int8] [EType_Int8]           = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int8] [EType_Int8_u]         = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int8_u] [EType_Int8]         = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int8_u] [EType_Int8_u]       = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int16] [EType_Int16]         = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int16] [EType_Int16_u]       = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int16_u] [EType_Int16]       = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int16_u] [EType_Int16_u]     = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int32] [EType_Int32]         = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int32] [EType_Int32_u]       = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int32_u] [EType_Int32]       = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int32_u] [EType_Int32_u]     = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int64] [EType_Int64]         = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int64] [EType_Int64_u]       = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int64_u] [EType_Int64]       = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int64_u] [EType_Int64_u]     = &m_Cast_cpy;

	m_CastOperatorTable [EType_Int16_be] [EType_Int16_be]   = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int16_be] [EType_Int16_beu]  = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int16_beu] [EType_Int16_be]  = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int16_beu] [EType_Int16_beu] = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int32_be] [EType_Int32_be]   = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int32_be] [EType_Int32_beu]  = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int32_beu] [EType_Int32_be]  = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int32_beu] [EType_Int32_beu] = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int64_be] [EType_Int64_be]   = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int64_be] [EType_Int64_beu]  = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int64_beu] [EType_Int64_be]  = &m_Cast_cpy;
	m_CastOperatorTable [EType_Int64_beu] [EType_Int64_beu] = &m_Cast_cpy;

	// endianness swaps

	m_CastOperatorTable [EType_Int16] [EType_Int16_be]    = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int16] [EType_Int16_beu]   = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int16_u] [EType_Int16_be]  = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int16_u] [EType_Int16_beu] = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int16_be] [EType_Int16]    = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int16_be] [EType_Int16_u]  = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int16_beu] [EType_Int16]   = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int16_beu] [EType_Int16_u] = &m_Cast_int_swp;

	m_CastOperatorTable [EType_Int32] [EType_Int32_be]    = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int32] [EType_Int32_beu]   = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int32_u] [EType_Int32_be]  = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int32_u] [EType_Int32_beu] = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int32_be] [EType_Int32]    = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int32_be] [EType_Int32_u]  = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int32_beu] [EType_Int32]   = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int32_beu] [EType_Int32_u] = &m_Cast_int_swp;

	m_CastOperatorTable [EType_Int64] [EType_Int64_be]    = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int64] [EType_Int64_beu]   = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int64_u] [EType_Int64_be]  = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int64_u] [EType_Int64_beu] = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int64_be] [EType_Int64]    = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int64_be] [EType_Int64_u]  = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int64_beu] [EType_Int64]   = &m_Cast_int_swp;
	m_CastOperatorTable [EType_Int64_beu] [EType_Int64_u] = &m_Cast_int_swp;

	// integer truncations

	m_CastOperatorTable [EType_Int16] [EType_Int8]    = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int16] [EType_Int8_u]  = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int32] [EType_Int8]    = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int32] [EType_Int8_u]  = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int32] [EType_Int16]   = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int32] [EType_Int16_u] = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int64] [EType_Int8]    = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int64] [EType_Int8_u]  = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int64] [EType_Int16]   = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int64] [EType_Int16_u] = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int64] [EType_Int32]   = &m_Cast_int_trunc;
	m_CastOperatorTable [EType_Int64] [EType_Int32_u] = &m_Cast_int_trunc;

	// integer extensions

	m_CastOperatorTable [EType_Int8] [EType_Int16]    = &m_Cast_int_ext;
	m_CastOperatorTable [EType_Int8] [EType_Int32]    = &m_Cast_int_ext;
	m_CastOperatorTable [EType_Int8] [EType_Int64]    = &m_Cast_int_ext;
	m_CastOperatorTable [EType_Int8_u] [EType_Int16]  = &m_Cast_int_ext_u;
	m_CastOperatorTable [EType_Int8_u] [EType_Int32]  = &m_Cast_int_ext_u;
	m_CastOperatorTable [EType_Int8_u] [EType_Int64]  = &m_Cast_int_ext_u;
	m_CastOperatorTable [EType_Int16] [EType_Int32]   = &m_Cast_int_ext;
	m_CastOperatorTable [EType_Int16] [EType_Int64]   = &m_Cast_int_ext;
	m_CastOperatorTable [EType_Int16_u] [EType_Int32] = &m_Cast_int_ext_u;
	m_CastOperatorTable [EType_Int16_u] [EType_Int64] = &m_Cast_int_ext_u;
	m_CastOperatorTable [EType_Int32] [EType_Int64]   = &m_Cast_int_ext;
	m_CastOperatorTable [EType_Int32_u] [EType_Int64] = &m_Cast_int_ext_u;

	// floating extensions & truncations

	m_CastOperatorTable [EType_Float] [EType_Float]   = &m_Cast_cpy;
	m_CastOperatorTable [EType_Double] [EType_Double] = &m_Cast_cpy;
	m_CastOperatorTable [EType_Float] [EType_Double]  = &m_Cast_f32_f64;
	m_CastOperatorTable [EType_Double] [EType_Float]  = &m_Cast_f64_f32;

	// integer to floating point

	m_CastOperatorTable [EType_Int32] [EType_Float]    = &m_Cast_i32_f32;
	m_CastOperatorTable [EType_Int32_u] [EType_Float]  = &m_Cast_i32u_f32;
	m_CastOperatorTable [EType_Int32] [EType_Double]   = &m_Cast_i32_f64;
	m_CastOperatorTable [EType_Int32_u] [EType_Double] = &m_Cast_i32u_f64;
	m_CastOperatorTable [EType_Int64] [EType_Float]    = &m_Cast_i64_f32;
	m_CastOperatorTable [EType_Int64_u] [EType_Float]  = &m_Cast_i64u_f32;
	m_CastOperatorTable [EType_Int64] [EType_Double]   = &m_Cast_i64_f64;
	m_CastOperatorTable [EType_Int64_u] [EType_Double] = &m_Cast_i64u_f64;

	// floating point to integer

	m_CastOperatorTable [EType_Float] [EType_Int32]  = &m_Cast_f32_i32;
	m_CastOperatorTable [EType_Double] [EType_Int32] = &m_Cast_f64_i32;
	m_CastOperatorTable [EType_Float] [EType_Int64]  = &m_Cast_f32_i64;
	m_CastOperatorTable [EType_Double] [EType_Int64] = &m_Cast_f64_i64;

	// build super moves for basic types (floyd-warshall)

	for (size_t k = 0; k < EType__BasicTypeCount; k++)
	for (size_t i = 0; i < EType__BasicTypeCount; i++)
	for (size_t j = 0; j < EType__BasicTypeCount; j++)
	{
		ICastOperator* pOperatorIK = m_CastOperatorTable [i] [k];
		ICastOperator* pOperatorKJ = m_CastOperatorTable [k] [j];
		if (!pOperatorIK || !pOperatorKJ)
			continue;

		size_t SuperPrice = pOperatorIK->GetPrice () + pOperatorKJ->GetPrice ();

		ICastOperator* pOperatorIJ = m_CastOperatorTable [i] [j];
		if (pOperatorIJ && pOperatorIJ->GetPrice () <= SuperPrice)
			continue;

		CSuperCast* pSuperCast = AXL_MEM_NEW (CSuperCast);
		pSuperCast->m_pIntermediateType = m_pModule->m_TypeMgr.GetBasicType ((EType) k);
		pSuperCast->m_pFirst = pOperatorIK;
		pSuperCast->m_pSecond = pOperatorKJ;
		pSuperCast->m_Price = SuperPrice;
		m_SuperCastList.InsertTail (pSuperCast);

		m_CastOperatorTable [i] [j] = pSuperCast;
	}

	// integer to bool (only after building super moves!)

	for (size_t i = EType_Int8; i < EType_Int64_u; i++)
	{
		m_CastOperatorTable [i] [EType_Bool] = &m_Cast_num_bool;
		m_CastOperatorTable [EType_Bool] [i] = &m_Cast_int_ext_u;
	}

	for (size_t i = EType_Int16_be; i < EType_Int64_beu; i++)
		m_CastOperatorTable [i] [EType_Bool] = &m_Cast_num_bool;	

	m_CastOperatorTable [EType_Float] [EType_Bool]  = &m_Cast_num_bool;	
	m_CastOperatorTable [EType_Double] [EType_Bool] = &m_Cast_num_bool;	
}

bool
COperatorMgr::UnaryOperator (
	EUnOp OpKind,
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	ASSERT (OpKind >= 0 && OpKind < EUnOp__Count);
	IUnaryOperator* pOperator = m_UnaryOperatorTable [OpKind];
	
	if (!pOperator)
	{
		err::SetFormatStringError (_T("unary '%s' is not supported"), GetUnOpString (OpKind));
		return false;
	}

	CValue OpValue;
	return 
		PrepareOperand (RawOpValue, &OpValue, pOperator->GetFlags ()) &&
		pOperator->Operator (OpValue, pResultValue);
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

bool
COperatorMgr::BinaryOperator (
	EBinOp OpKind,
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	ASSERT (OpKind >= 0 && OpKind < EBinOp__Count);
	IBinaryOperator* pOperator = m_BinaryOperatorTable [OpKind];
	
	if (!pOperator)
	{
		err::SetFormatStringError (_T("binary '%s' is not supported"), GetBinOpString (OpKind));
		return false;
	}

	CValue OpValue1;
	CValue OpValue2;

	return
		PrepareOperand (RawOpValue1, &OpValue1, pOperator->GetFlags ()) &&
		PrepareOperand (RawOpValue2, &OpValue2, pOperator->GetFlags ()) &&
		pOperator->Operator (OpValue1, OpValue2, pResultValue);
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
	CType* pOpType,
	CType* pType
	)
{
	if (pOpType->Cmp (pType) == 0)
		return &m_Cast_cpy;

	EType OpTypeKind = pOpType->GetTypeKind ();
	EType TypeKind = pType->GetTypeKind ();

	CType* pBaseType;

	switch (OpTypeKind)
	{
	case EType_Property:
		pBaseType = ((CPropertyType*) pOpType)->GetGetterType ()->GetReturnType ();
		return GetCastOperator (pBaseType, pType) ? &m_Cast_getp : NULL;

	case EType_Reference:
	case EType_Reference_u:
		pBaseType = ((CPointerType*) pOpType)->GetBaseType ();
		return 
			pBaseType->GetTypeKind () == EType_Array ? GetCastOperator (pBaseType, pType) : 
			GetCastOperator (pBaseType, pType) ? &m_Cast_load : NULL;

	case EType_Pointer:
		return pType->IsPointerType () ? &m_Cast_ptr : NULL;

	case EType_Pointer_u:
		return TypeKind == EType_Pointer_u ? &m_Cast_ptr : NULL;

	case EType_Array:
		return 
			TypeKind == EType_Array ? (ICastOperator*) &m_Cast_arr :
			pType->IsPointerType () ? (ICastOperator*) &m_Cast_arr_ptr : NULL;

	default:
		return OpTypeKind < EType__BasicTypeCount && TypeKind < EType__BasicTypeCount ?
			m_CastOperatorTable [OpTypeKind] [TypeKind] : NULL;
	}
}

ECast
COperatorMgr::GetCastKind (
	CType* pOpType,
	CType* pType
	)
{
	pOpType = PrepareOperandType (pOpType);

	ICastOperator* pOperator = GetCastOperator (pOpType, pType);
	return pOperator ? pOperator->GetCastKind (pOpType, pType) : ECast_None;
}

bool
COperatorMgr::CastOperator (
	const CValue& RawOpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	CType* pOpType = OpValue.GetType ();

	ICastOperator* pOperator = GetCastOperator (pOpType, pType);	
	if (!pOperator)
	{
		SetCastError (pOpType, pType);
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
	const CValue& RawOpValue,
	const CValue& DstValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	CType* pDstType = DstValue.GetType ();
	if (pDstType->GetTypeKind () == EType_Property)
		return SetPropertyOperator (OpValue, DstValue.GetProperty ());

	if (!pDstType->IsReferenceType ())
	{
		err::SetFormatStringError (_T("left operand must be l-value"));
		return false;
	}

	return StoreReferenceOperator (OpValue, DstValue);
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
	const CValue& RawOpValue,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	CType* pType = OpValue.GetType ();

	if (pType->IsReferenceType ())
	{
		pType = ((CPointerType*) pType)->GetBaseType ();

		if (pType->IsPointerType ())
		{
			pType = ((CPointerType*) pType)->GetBaseType ();

			Result = UnaryOperator (EUnOp_Indir, &OpValue);
			if (!Result)
				return false;
		}
	}

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
COperatorMgr::PointerToMemberOperator (
	const CValue& RawOpValue,
	const tchar_t* pName,
	CValue* pResultValue
	)
{	
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;
/*
	TUnaryOperatorTypeInfo TypeInfo;
	IUnaryOperator* pOperator = GetUnaryOperator (EUnOp_Ptr, OpValue.GetType (), &TypeInfo);
	if (pOperator)
	{
		CValue CastOpValue;
		Result = 
			CastOperator (OpValue, TypeInfo.m_pOpType, &CastOpValue) &&
			pOperator->Operator (CastOpValue, &OpValue) && 
			PrepareOperand (&OpValue);

		if (!Result)
			return false;
	}
*/

	return MemberOperator (OpValue, pName, pResultValue);
}

bool
COperatorMgr::PointerToMemberOperator (
	CValue* pValue,
	const tchar_t* pName
	)
{
	CValue ResultValue;

	bool Result = PointerToMemberOperator (*pValue, pName, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

bool
COperatorMgr::StructMemberOperator (
	const CValue& OpValue,
	CStructType* pStructType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CStructMember* pMember = pStructType->FindMember (pName);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pStructType->GetTypeString ());
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

	CType* pOpType = OpValue.GetType ();

	if (!pOpType->IsReferenceType ())
	{
		llvm::Value* pLlvmMember = m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue.GetLlvmValue (), pMember->GetLlvmIndex (), "member");
		pResultValue->SetLlvmRegister (pLlvmMember, pMember->GetType ());
		return true;
	}

	if (pOpType->GetTypeKind () == EType_Reference_u)
	{
		llvm::Value* pLlvmGep = CreateLlvmGep (OpValue.GetLlvmValue (), 0, pMember->GetLlvmIndex ());
		CType* pResultType = pMember->GetType ()->GetPointerType (EType_Reference_u);
		pResultValue->SetLlvmRegister (pLlvmGep, pResultType);
		return true;
	}

	if (OpValue.GetValueKind () == EValue_Variable)
	{
		ASSERT (pOpType->GetTypeKind () == EType_Reference);
		llvm::Value* pLlvmGep = CreateLlvmGep (OpValue.GetLlvmValue (), 0, pMember->GetLlvmIndex ());
		pResultValue->SetVariable (OpValue.GetVariable (), pLlvmGep, pMember->GetType ());
		return true;
	}
	else
	{
		llvm::Value* pLlvmFatPtr = OpValue.GetLlvmValue ();		
		llvm::Value* pLlvmPtr = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmFatPtr, 0, "sptr_ptr");
		pLlvmPtr = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtr, pStructType->GetPointerType (EType_Pointer_u)->GetLlvmType (), "sptr_ptr_cast");

		llvm::Value* pLlvmGep = CreateLlvmGep (pLlvmPtr, 0, pMember->GetLlvmIndex ());
		pLlvmFatPtr = ModifyLlvmSafePtr (pLlvmFatPtr, pLlvmGep);
		
		CType* pResultType = pMember->GetType ()->GetPointerType (EType_Reference);
		pResultValue->SetLlvmRegister (pLlvmFatPtr, pResultType);
		return true;
	}
}

bool
COperatorMgr::ClassMemberOperator (
	const CValue& OpValue,
	CClassType* pClassType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CModuleItem* pClassMember = pClassType->FindMember (pName);
	if (!pClassMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pClassType->GetTypeString ());
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
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

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

	CType* pReturnType = pFunction->GetType ()->GetReturnType ();
	if (pReturnType->GetTypeKind () != EType_Void)
	{
		llvm::Instruction* pLlvmCall = m_pModule->m_LlvmBuilder.CreateCall (
			pLlvmFunction,
			llvm::ArrayRef <llvm::Value*> (LlvmArgArray, LlvmArgArray.GetCount ()),
			"call"
			);
		
		pResultValue->SetLlvmRegister (pLlvmCall, pReturnType);
	}
	else
	{
		m_pModule->m_LlvmBuilder.CreateCall (
			pLlvmFunction,
			llvm::ArrayRef <llvm::Value*> (LlvmArgArray, LlvmArgArray.GetCount ())
			);
		
		pResultValue->SetVoid ();
	}

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

CType* 
COperatorMgr::PrepareOperandType (
	CType* pOpType,
	int Flags
	) 
{
	CType* pType = pOpType;
	
	for (;;)
	{
		CType* pPrevType = pType;

		EType TypeKind = pType->GetTypeKind ();
		switch (TypeKind)
		{			
		case EType_Qualifier:
			pType = ((CQualifierType*) pType)->GetBaseType ();
			break;

		case EType_Reference:
		case EType_Reference_u:
			CType* pBaseType;

			pBaseType = ((CPointerType*) pType)->GetBaseType ();
			if (pBaseType->IsReferenceType () || (Flags & EOpFlag_LoadReference)) // double reference
				pType = pBaseType;

			break;

		case EType_Property:
			pType = ((CPropertyType*) pType)->GetGetterType ()->GetReturnType ();
			break;

		case EType_Enum:
			if (Flags & EOpFlag_EnumToInt)
				pType = m_pModule->m_TypeMgr.GetBasicType (EType_Int);

			break;
		}

		if (pType == pPrevType)
			return pType;
	}
}

bool 
COperatorMgr::PrepareOperand (
	const CValue& OpValue,
	CValue* pOpValue,
	int Flags
	) 
{
	bool Result;

	CValue Value = OpValue;

	for (;;)
	{
		CType* pType = Value.GetType ();

		EType TypeKind = pType->GetTypeKind ();
		switch (TypeKind)
		{			
		case EType_Qualifier:
			Value.OverrideType (((CQualifierType*) pType)->GetBaseType ());
			break;

		case EType_Reference:
		case EType_Reference_u:
			CType* pBaseType;

			pBaseType = ((CPointerType*) pType)->GetBaseType ();
			if (pBaseType->IsReferenceType () || (Flags & EOpFlag_LoadReference)) // double reference
			{
				Result = LoadReferenceOperator (&Value);
				if (!Result)
					return false;
			}

			break;

		case EType_Property:
			Result = GetPropertyOperator (Value.GetProperty (), &Value);
			if (!Result)
				return false;

			break;

		case EType_Enum:
			if (Flags & EOpFlag_EnumToInt)
				Value.OverrideType (EType_Int);

			break;
		}

		if (Value.GetType () == pType)
		{
			*pOpValue = Value;
			return true;
		}
	}
}

bool 
COperatorMgr::PrepareOperand (
	CValue* pOpValue,
	int Flags
	)
{
	CValue OpValue;

	bool Result = PrepareOperand (*pOpValue, &OpValue, Flags);
	if (!Result)
		return false;

	*pOpValue = OpValue;
	return true;
}

bool
COperatorMgr::LoadReferenceOperator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CPointerType* pType = (CPointerType*) OpValue.GetType ();
	ASSERT (pType->IsReferenceType ());

	EType TypeKind = pType->GetTypeKind ();
	CType* pTargetType = pType->GetBaseType ();

	bool IsVolatile = false;
	if (pTargetType->GetTypeKind () == EType_Qualifier)
	{
		IsVolatile = (pTargetType->GetFlags () & ETypeQualifier_Volatile) != 0;
		pTargetType = ((CDerivedType*) pTargetType)->GetBaseType ();
	}

	llvm::Value* pLlvmValue = OpValue.GetLlvmValue ();

	if (TypeKind == EType_Reference_u || OpValue.GetValueKind () == EValue_Variable)
	{
		llvm::Value* pLlvmLoad = m_pModule->m_LlvmBuilder.CreateLoad (pLlvmValue, IsVolatile, "loa");
		pResultValue->SetLlvmRegister (pLlvmLoad, pTargetType);
		return true;
	}

	CheckLlvmSafePtrRange (pLlvmValue, pTargetType->GetSize (), ESafePtrError_Load);

	llvm::Value* pLlvmPtr = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmValue, 0, "sptr_ptr");

	llvm::Type* pLlvmPtrType = pTargetType->GetPointerType (EType_Pointer_u)->GetLlvmType ();
	pLlvmPtr = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtr, pLlvmPtrType, "sptr_ptr_cast");
	llvm::Value* pLlvmLoad = m_pModule->m_LlvmBuilder.CreateLoad (pLlvmPtr, IsVolatile, "loa");
	
	pResultValue->SetLlvmRegister (pLlvmLoad, pTargetType);
	return true;
}

bool
COperatorMgr::LoadReferenceOperator (CValue* pValue)
{
	CValue ResultValue;

	bool Result = LoadReferenceOperator (*pValue, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

bool
COperatorMgr::StoreReferenceOperator (
	const CValue& SrcValue,
	const CValue& RawDstValue
	)
{
	bool Result;

	CValue DstValue = RawDstValue;

	CPointerType* pDstType = (CPointerType*) DstValue.GetType ();
	ASSERT (pDstType->IsReferenceType ());

	CType* pTargetType = pDstType->GetBaseType ();
	while (pTargetType->IsReferenceType ()) // peel double references
	{
		Result = LoadReferenceOperator (&DstValue);
		if (!Result)
			return false;

		pDstType = (CPointerType*) pTargetType;
		pTargetType = pDstType->GetBaseType ();
	}

	bool IsVolatile = false;
	if (pTargetType->GetTypeKind () == EType_Qualifier)
	{
		IsVolatile = (pTargetType->GetFlags () & ETypeQualifier_Volatile) != 0;
		pTargetType = ((CDerivedType*) pTargetType)->GetBaseType ();
	}
	
	CValue CastValue;
	Result = CastOperator (SrcValue, pTargetType, &CastValue);
	if (!Result)
		return false;

	llvm::Value* pLlvmSrcValue = CastValue.GetLlvmValue ();
	llvm::Value* pLlvmDstValue = DstValue.GetLlvmValue ();

	if (pTargetType->GetTypeKind () == EType_Pointer)
	{
		if (CastValue.GetValueKind () == EValue_Variable)
		{
			CVariable* pVariable = CastValue.GetVariable ();
			CScope* pScope = pVariable->GetScope ();

			pLlvmSrcValue = CreateLlvmSafePtr (
				pLlvmSrcValue, 
				pVariable->GetLlvmValue (), 
				pVariable->GetType (),
				pScope ? pScope->GetLevel () : 0
				);
		}

		Result = CheckLlvmSafePtrScope (pLlvmSrcValue, CastValue, DstValue);
		if (!Result)
			return false;
	}

	if (pDstType->GetTypeKind () == EType_Reference_u || DstValue.GetValueKind () == EValue_Variable) // no need to do a range check
	{
		m_pModule->m_LlvmBuilder.CreateStore (pLlvmSrcValue, pLlvmDstValue, IsVolatile);
		return true;
	}

	CheckLlvmSafePtrRange (pLlvmDstValue, pTargetType->GetSize (), ESafePtrError_Store); 
	
	llvm::Value* pLlvmPtr = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmDstValue, 0, "sptr_ptr");
	llvm::Type* pLlvmPtrType = pTargetType->GetPointerType (EType_Pointer_u)->GetLlvmType ();

	pLlvmPtr = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtr, pLlvmPtrType, "sptr_ptr_cast");
	m_pModule->m_LlvmBuilder.CreateStore (pLlvmSrcValue, pLlvmPtr, IsVolatile);
	return true;
}

bool
COperatorMgr::GetPropertyOperator (
	CProperty* pProperty,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("getting property is not implemented yet"));
	return false;
}

bool
COperatorMgr::SetPropertyOperator (
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

llvm::Value*
COperatorMgr::CreateLlvmGep (
	llvm::Value* pLlvmPtr,
	llvm::Value* pLlvmIndex0,
	llvm::Value* pLlvmIndex1
	)
{
	if (!pLlvmIndex0)
	{
		CValue Index0;
		Index0.SetConstInt32 (0, EType_Int32);
		pLlvmIndex0 = Index0.GetLlvmValue ();
	}
		
	if (!pLlvmIndex1)
		return m_pModule->m_LlvmBuilder.CreateGEP (pLlvmPtr, pLlvmIndex0, "gep");

	llvm::Value* LlvmIndexArray [] =
	{
		pLlvmIndex0,
		pLlvmIndex1,
	};

	return m_pModule->m_LlvmBuilder.CreateGEP (
		pLlvmPtr, 
		llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, 2), 
		"gep"
		);
}

llvm::Value*
COperatorMgr::CreateLlvmGep (
	llvm::Value* pLlvmPtr,
	intptr_t Index0,
	intptr_t Index1
	)
{
	CValue IndexValue0;
	IndexValue0.SetConstInt32 (Index0, EType_Int32);

	CValue IndexValue1;
	IndexValue1.SetConstInt32 (Index1, EType_Int32);

	return CreateLlvmGep (pLlvmPtr, IndexValue0.GetLlvmValue (), IndexValue1.GetLlvmValue ());
}

llvm::Value*
COperatorMgr::CreateLlvmSafePtr (
	llvm::Value* pLlvmPtr,
	llvm::Value* pLlvmParentPtr,
	CType* pParentType,
	size_t ScopeLevel
	)
{
	// specialize CallOperator () for effiency

	CType* pBytePtrType = m_pModule->m_TypeMgr.GetBytePtrType ();

	CValue ParentSizeValue;
	ParentSizeValue.SetConstSizeT (pParentType->GetSize ());

	CValue ScopeLevelValue;
	ScopeLevelValue.SetConstSizeT (ScopeLevel);

	llvm::Value* LlvmArgArray [4];
	LlvmArgArray [0] = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtr, pBytePtrType->GetLlvmType (), "sptr_p_cast");
	LlvmArgArray [1] = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmParentPtr, pBytePtrType->GetLlvmType (), "sptr_beg_cast");
	LlvmArgArray [2] = ParentSizeValue.GetLlvmValue ();
	LlvmArgArray [3] = ScopeLevelValue.GetLlvmValue ();

	CFunction* pCreateSafePtr = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CreateSafePtr);

	return m_pModule->m_LlvmBuilder.CreateCall (
		pCreateSafePtr->GetLlvmFunction (),
		llvm::ArrayRef <llvm::Value*> (LlvmArgArray, countof (LlvmArgArray)), 
		"sptr"
		);
}

llvm::Value*
COperatorMgr::ModifyLlvmSafePtr (
	llvm::Value* pLlvmSafePtr,
	llvm::Value* pLlvmPtr
	)
{
	CType* pBytePtrType = m_pModule->m_TypeMgr.GetBytePtrType ();
	
	pLlvmPtr = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtr, pBytePtrType->GetLlvmType (), "sptr_p_cast");
	pLlvmSafePtr = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmSafePtr, pLlvmPtr, 0, "sptr");

	return pLlvmSafePtr;
}

void
COperatorMgr::CheckLlvmSafePtrRange (
	llvm::Value* pLlvmSafePtr,
	size_t Size,
	ESafePtrError Error
	)
{
	CValue SizeValue;
	SizeValue.SetConstSizeT (Size);

	CValue ErrorValue;
	ErrorValue.SetConstInt32 (Error, EType_Int);

	CFunction* pCheckSafePtrRange = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckSafePtrRange);

	m_pModule->m_LlvmBuilder.CreateCall3 (
		pCheckSafePtrRange->GetLlvmFunction (),
		pLlvmSafePtr,
		SizeValue.GetLlvmValue (),
		ErrorValue.GetLlvmValue ()
		);
}

void
COperatorMgr::CheckLlvmSafePtrScope (
	llvm::Value* pLlvmSafePtr,
	size_t ScopeLevel
	)
{
	CValue ScopeLevelValue;
	ScopeLevelValue.SetConstSizeT (ScopeLevel);

	CFunction* pCheckSafePtrScope = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CheckSafePtrScope);

	m_pModule->m_LlvmBuilder.CreateCall2 (
		pCheckSafePtrScope->GetLlvmFunction (),
		pLlvmSafePtr,
		ScopeLevelValue.GetLlvmValue ()
		);
}

bool
COperatorMgr::CheckLlvmSafePtrScope (
	llvm::Value* pLlvmSafePtr,
	const CValue& SrcValue,
	size_t DstScopeLevel
	)
{
	if (SrcValue.GetValueKind () == EValue_Variable)
	{
		CScope* pSrcScope = SrcValue.GetVariable ()->GetScope ();
		size_t SrcScopeLevel = pSrcScope ? pSrcScope->GetLevel () : 0;

		if (SrcScopeLevel <= DstScopeLevel)
			return true; // OK!

		err::SetFormatStringError (_T("safe pointer/reference scope level mismatch"));
		return false;
	}

	CheckLlvmSafePtrScope (pLlvmSafePtr, DstScopeLevel);
	return true;
}

bool
COperatorMgr::CheckLlvmSafePtrScope (
	llvm::Value* pLlvmSafePtr,
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	if (DstValue.GetValueKind () == EValue_Variable)
	{
		CScope* pDstScope = DstValue.GetVariable ()->GetScope ();
		size_t DstScopeLevel = pDstScope ? pDstScope->GetLevel () : 0;

		return CheckLlvmSafePtrScope (pLlvmSafePtr, SrcValue, DstScopeLevel);
	}

	CheckLlvmSafePtrScope (pLlvmSafePtr, 0);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
