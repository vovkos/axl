#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Closure.h"
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
	m_BinaryOperatorTable [EBinOp_Idx]        = &m_BinOp_Idx;

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

	for (size_t k = 0; k < EType__PrimitiveTypeCount; k++)
	for (size_t i = 0; i < EType__PrimitiveTypeCount; i++)
	for (size_t j = 0; j < EType__PrimitiveTypeCount; j++)
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
		pSuperCast->m_pIntermediateType = m_pModule->m_TypeMgr.GetPrimitiveType ((EType) k);
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
		if (TypeKind == EType_PropertyPointer)
			return &m_Cast_prop;

		pBaseType = ((CPropertyType*) pOpType)->GetGetter ()->GetType ()->GetReturnType ();
		return GetCastOperator (pBaseType, pType) ? &m_Cast_getp : NULL;

	case EType_PropertyPointer:
		if (TypeKind == EType_PropertyPointer)
			return &m_Cast_prop;

		pBaseType = ((CPropertyPointerType*) pOpType)->GetPropertyType ()->GetGetter ()->GetType ()->GetReturnType ();
		return GetCastOperator (pBaseType, pType) ? &m_Cast_getp : NULL;

	case EType_Reference:
	case EType_Reference_u:
		if (TypeKind == EType_PropertyPointer)
			return &m_Cast_prop;

		pBaseType = ((CPointerType*) pOpType)->GetBaseType ();
		return 
			pBaseType->GetTypeKind () == EType_Array ? GetCastOperator (pBaseType, pType) : 
			GetCastOperator (pBaseType, pType) ? &m_Cast_load : NULL;

	case EType_Pointer:
		return pType->IsPointerType () ? &m_Cast_ptr : NULL;

	case EType_Pointer_u:
		return 
			pOpType->IsUnsafeFunctionPointerType () ? 
			TypeKind == EType_FunctionPointer ? (ICastOperator*) &m_Cast_fn : NULL :
			TypeKind == EType_Pointer_u ? (ICastOperator*) &m_Cast_ptr : NULL;

	case EType_FunctionPointer:
		return TypeKind == EType_FunctionPointer ? &m_Cast_fn :  NULL;

	case EType_Array:
		return 
			TypeKind == EType_Array ? (ICastOperator*) &m_Cast_arr :
			pType->IsPointerType () ? (ICastOperator*) &m_Cast_arr_ptr : NULL;

	case EType_BitField:
		pBaseType = ((CBitFieldType*) pOpType)->GetBaseType ();
		return GetCastOperator (pBaseType, pType) ? &m_Cast_getbf : NULL;

	case EType_Interface:
	case EType_Class:
		return pType->IsClassType () ? &m_Cast_class : NULL;

	default:
		if (TypeKind == EType_BitField)
		{
			pBaseType = ((CBitFieldType*) pType)->GetBaseType ();
			return GetCastOperator (pOpType, pBaseType) ? &m_Cast_setbf : NULL;
		}

		return OpTypeKind < EType__PrimitiveTypeCount && TypeKind < EType__PrimitiveTypeCount ?
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

ECast
COperatorMgr::GetCallCastKind (
	CFunctionType* pFunctionType,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	rtl::CArrayT <CType*> ArgTypeArray = pFunctionType->GetArgTypeArray ();
	size_t FormalArgCount = ArgTypeArray.GetCount ();
	size_t ActualArgCount = pArgList->GetCount ();

	if (FormalArgCount > ActualArgCount || 
		ActualArgCount > FormalArgCount && !(pFunctionType->GetFlags () & EFunctionTypeFlag_IsVarArg))
	{
		return ECast_None;
	}

	ECast WorstCastKind = ECast_Identitiy;

	rtl::CBoxIteratorT <CValue> ActualArg = pArgList->GetHead ();
	for (size_t i = 0; i < FormalArgCount; i++, ActualArg++)
	{
		CType* pFormalArgType = ArgTypeArray [i];
		CType* pActualArgType = ActualArg->GetType ();

		ECast CastKind = GetCastKind (pActualArgType, pFormalArgType);
		if (!CastKind)
			return ECast_None;

		if (CastKind < WorstCastKind)
			WorstCastKind = CastKind;
	}

	return WorstCastKind;
}

bool
COperatorMgr::CastOperator (
	const CValue& RawOpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	int OpFlags = pType->IsPropertyType () ? EOpFlag_KeepProperty : 0;

	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue, OpFlags);
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
	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
	return CastOperator (OpValue, pType, pResultValue);
}

bool
COperatorMgr::CastOperator (
	CValue* pValue,
	EType TypeKind
	)
{
	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
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

	EType DstTypeKind = DstValue.GetType ()->GetTypeKind ();
	switch (DstTypeKind)
	{
	case EType_Property:
	case EType_PropertyPointer:
		return SetPropertyOperator (OpValue, DstValue);

	case EType_Reference:
	case EType_Reference_u:
		return StoreReferenceOperator (OpValue, DstValue, false);

	default:
		err::SetFormatStringError (_T("left operand must be l-value"));
		return false;
	}
}

bool
COperatorMgr::BinOpMoveOperator (
	const CValue& SrcValue,
	const CValue& DstValue,
	EBinOp OpKind
	)
{
	CType* pTargetType = PrepareOperandType (DstValue.GetType (), EOpFlag_LoadReference);
	EType TargetTypeKind = pTargetType->GetTypeKind ();

	switch (TargetTypeKind)
	{
	case EType_Event:
		switch (OpKind)
		{
		case EBinOp_Add:
			return EventOperator (DstValue, SrcValue, EEventOp_AddHandler);

		case EBinOp_Sub:
			return EventOperator (DstValue, SrcValue, EEventOp_RemoveHandler);

		default:
			err::SetFormatStringError (_T("invalid operator for events"), GetBinOpString (OpKind));
			return false;
		}

		break;

	case EType_Interface:
	case EType_Class:
		#pragma AXL_TODO ("overloaded operators for interfaces & classes")
		break;
	}

	CValue RValue;
	return 
		BinaryOperator (OpKind, DstValue, SrcValue, &RValue) &&
		MoveOperator (RValue, DstValue);
}

bool
COperatorMgr::RefMoveOperator (
	const CValue& OpValue,
	const CValue& DstValue
	)
{
	CType* pDstType = DstValue.GetType ();
	if (!pDstType->IsReferenceType ())
	{
		err::SetFormatStringError (_T("left operand must be a reference to l-value"));
		return false;
	}

	CType* pBaseType = ((CPointerType*) pDstType)->GetBaseType ();
	EType BaseTypeKind = pBaseType->GetTypeKind ();
	switch (BaseTypeKind)
	{
	case EType_Reference:
	case EType_Reference_u:
		return RefMoveReferenceOperator (OpValue, DstValue, (CPointerType*) pBaseType);

	case EType_PropertyPointer:
		return RefMovePropertyPointerOperator (OpValue, DstValue, (CPropertyPointerType*) pBaseType);

	default:
		err::SetFormatStringError (_T("left operand must be a reference to l-value"));
		return false;
	}
}

bool
COperatorMgr::RefMoveReferenceOperator (
	const CValue& RawOpValue,
	const CValue& RawDstValue,
	CPointerType* pReferenceType
	)
{
	CValue OpValue = RawOpValue;
	CValue DstValue = RawDstValue;

	EType OuterReferenceTypeKind = DstValue.GetType ()->GetTypeKind ();
	EType InnerReferenceTypeKind = pReferenceType->GetTypeKind ();
	EType PointerTypeKind = InnerReferenceTypeKind == EType_Reference ? EType_Pointer : EType_Pointer_u;
	CType* pBaseType = pReferenceType->GetBaseType ();
	CType* pPointerType = pBaseType->GetPointerType (PointerTypeKind);
	CType* pOverrideType = pPointerType->GetPointerType (OuterReferenceTypeKind);

	DstValue.OverrideType (pOverrideType);

	return 
		UnaryOperator (EUnOp_Addr, &OpValue) &&
		MoveOperator (OpValue, DstValue);
}

bool
COperatorMgr::RefMovePropertyPointerOperator (
	const CValue& RawOpValue,
	const CValue& DstValue,
	CPropertyPointerType* pPropertyPointerType
	)
{
	CValue OpValue;
	return 
		CastOperator (RawOpValue, pPropertyPointerType, &OpValue) &&
		StoreReferenceOperator (OpValue, DstValue, true);
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
		pType = ((CPointerType*) pType)->GetBaseType ();

	if (pType->IsPointerType ())
	{
		pType = ((CPointerType*) pType)->GetBaseType ();

		Result = UnaryOperator (EUnOp_Indir, &OpValue);
		if (!Result)
			return false;
	}

	EType TypeKind = pType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_Struct:
		return StructMemberOperator (OpValue, (CStructType*) pType, pName, pResultValue);

	case EType_Union:
		return UnionMemberOperator (OpValue, (CUnionType*) pType, pName, pResultValue);

	case EType_Interface:
	case EType_Class:
		return 
			PrepareOperand (&OpValue, EOpFlag_LoadReference) &&
			ClassMemberOperator (OpValue, (CClassType*) pType, pName, pResultValue);

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
	CStructBaseTypeCoord Coord;
	CStructMember* pMember = pStructType->FindMember (pName, &Coord);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pStructType->GetTypeString ());
		return false;
	}

	Coord.m_LlvmIndexArray.Append (pMember->GetLlvmIndex ());
	Coord.m_Offset += pMember->GetOffset ();
	
	EValue OpValueKind = OpValue.GetValueKind ();
	if (OpValueKind == EValue_Const)
	{
		pResultValue->CreateConst (
			(char*) OpValue.GetConstData () + Coord.m_Offset,
			pMember->GetType () 
			);

		return true;
	}

	CType* pOpType = OpValue.GetType ();

	if (!pOpType->IsReferenceType ())
	{
		m_pModule->m_LlvmBuilder.CreateExtractValue (
			OpValue, 
			Coord.m_LlvmIndexArray, 
			Coord.m_LlvmIndexArray.GetCount (),
			pMember->GetType (),
			pResultValue
			);

		return true;
	}

	Coord.m_LlvmIndexArray.Insert (0, 0);

	if (pOpType->GetTypeKind () == EType_Reference_u)
	{
		CType* pResultType = pMember->GetType ()->GetPointerType (EType_Reference_u);
		m_pModule->m_LlvmBuilder.CreateGep (
			OpValue, 
			Coord.m_LlvmIndexArray, 
			Coord.m_LlvmIndexArray.GetCount (), 
			pResultType, 
			pResultValue
			);

		return true;
	}
	
	ASSERT (pOpType->GetTypeKind () == EType_Reference);

	if (OpValue.GetValueKind () == EValue_Variable)
	{
		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateGep (
			OpValue, 
			Coord.m_LlvmIndexArray, 
			Coord.m_LlvmIndexArray.GetCount (), 
			NULL, 
			&PtrValue
			);
		
		pResultValue->SetVariable (
			OpValue.GetVariable (), 
			PtrValue.GetLlvmValue (), 
			pMember->GetType (),
			true,
			(OpValue.GetFlags () & EValueFlag_IsVariableOffset) != 0
			);
	}
	else
	{
		CValue PtrValue;
		CValue ValidatorValue;

		CType* pResultType = pMember->GetType ()->GetPointerType (EType_Reference);

		m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 1, NULL, &ValidatorValue);
		m_pModule->m_LlvmBuilder.CreateGep (
			PtrValue, 
			Coord.m_LlvmIndexArray, 
			Coord.m_LlvmIndexArray.GetCount (), 
			NULL, 
			&PtrValue
			);

		CValue SafePtrValue = pResultType->GetUndefValue ();
		m_pModule->m_LlvmBuilder.CreateInsertValue (SafePtrValue, PtrValue, 0, NULL, &SafePtrValue);
		m_pModule->m_LlvmBuilder.CreateInsertValue (SafePtrValue, ValidatorValue, 1, pResultType, pResultValue);
	}

	return true;
}

bool
COperatorMgr::UnionMemberOperator (
	const CValue& OpValue,
	CUnionType* pUnionType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CUnionMember* pMember = pUnionType->FindMember (pName);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pUnionType->GetTypeString ());
		return false;
	}
	
	EValue OpValueKind = OpValue.GetValueKind ();
	if (OpValueKind == EValue_Const)
	{
		pResultValue->CreateConst (OpValue.GetConstData (), pMember->GetType ());
		return true;
	}

	CType* pOpType = OpValue.GetType ();

	if (!pOpType->IsReferenceType ())
	{
		err::SetFormatStringError (_T("union member operator on registers is not implemented yet"));
		return false;
	}

	if (pOpType->GetTypeKind () == EType_Reference_u)
	{
		CType* pResultType = pMember->GetType ()->GetPointerType (EType_Reference_u);
		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pResultType, pResultValue);
		return true;
	}
	
	ASSERT (pOpType->GetTypeKind () == EType_Reference);

	if (OpValue.GetValueKind () == EValue_Variable)
	{
		CType* pCastType = pMember->GetType ()->GetPointerType (EType_Reference_u);
		CValue CastValue;
		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pCastType, &CastValue);
		pResultValue->SetVariable (
			OpValue.GetVariable (), 
			CastValue.GetLlvmValue (), 
			pMember->GetType (),
			true,
			(OpValue.GetFlags () & EValueFlag_IsVariableOffset) != 0
			);
	}
	else
	{
		CType* pResultType = pMember->GetType ()->GetPointerType (EType_Reference);
		pResultValue->OverrideType (OpValue, pResultType);
	}

	return true;
}

bool
COperatorMgr::ClassMemberOperator (
	const CValue& OpValue,
	CClassType* pClassType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CClassBaseTypeCoord Coord;
	CClassMember* pMember = pClassType->FindMember (pName, &Coord);	
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pClassType->GetTypeString ());
		return false;
	}

	if (pMember->GetAccess () != EAccess_Public)
	{
		err::SetFormatStringError (_T("'%s' is not accessible"), pMember->GetQualifiedName ());
		return false;
	}

	bool Result = m_pModule->m_LlvmBuilder.CheckNullPtr (OpValue);
	if (!Result)
		return false;

	EClassMember MemberKind = pMember->GetMemberKind ();
	switch (MemberKind)
	{
	case EClassMember_Field:
		return ClassFieldMemberOperator (
			OpValue, 
			pClassType, 
			(CClassFieldMember*) pMember, 
			&Coord,
			pResultValue
			);
		
	case EClassMember_Method:
		pResultValue->SetFunctionOverload ((CClassMethodMember*) pMember);
		break;

	case EClassMember_Property:
		pResultValue->SetProperty (((CClassPropertyMember*) pMember)->GetType ());
		break;

	default:
		err::SetFormatStringError (_T("invalid interface member kind"));
		return false;
	}

	CClosure* pClosure = pResultValue->CreateClosure ();
	pClosure->CreateArg (0, OpValue);
	return true;
}

bool
COperatorMgr::ClassFieldMemberOperator (
	const CValue& OpValue,
	CClassType* pClassType,
	CClassFieldMember* pMember,
	CClassBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	CValue ObjPtrValue;
	
	size_t ObjPtrIndexArray [] = 
	{
		0, // iface* 
		0, // iface.hdr*
		1, // TObjectHdr**
	};

	m_pModule->m_LlvmBuilder.CreateGep (OpValue, ObjPtrIndexArray, countof (ObjPtrIndexArray), NULL, &ObjPtrValue);  // TObjectHdr**
	m_pModule->m_LlvmBuilder.CreateLoad (ObjPtrValue, NULL, &ObjPtrValue); // TObjectHdr* 

	CValue ScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, NULL, &ScopeLevelValue);  // size_t* m_pScopeLevel
	m_pModule->m_LlvmBuilder.CreateLoad (ScopeLevelValue, NULL, &ScopeLevelValue); // size_t m_ScopeLevel
	
	pCoord->m_FieldCoord.m_LlvmIndexArray.Insert (0, 0);
	pCoord->m_FieldCoord.m_LlvmIndexArray.Append (pMember->GetLlvmIndex ());

	size_t* p = pCoord->m_FieldCoord.m_LlvmIndexArray;
	size_t Count = pCoord->m_FieldCoord.m_LlvmIndexArray.GetCount ();

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (
		OpValue, 
		pCoord->m_FieldCoord.m_LlvmIndexArray, 
		pCoord->m_FieldCoord.m_LlvmIndexArray.GetCount (), 
		NULL,
		&PtrValue
		);

	CPointerType* pResultType = pMember->GetType ()->GetPointerType (EType_Reference);

	m_pModule->m_LlvmBuilder.CreateSafePtr (
		PtrValue, 
		PtrValue, 
		pMember->GetType ()->GetSize (),
		ScopeLevelValue,
		pResultType, 
		pResultValue
		);

	return true;
}

bool
COperatorMgr::GetClassVTable (
	const CValue& OpValue,
	CClassType* pClassType,
	CValue* pResultValue
	)
{
	size_t IndexArray [] = 
	{
		0, // class.iface*
		0, // class.iface.hdr*
		0, // class.vtbl**
	};

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (
		OpValue, 
		IndexArray, 
		countof (IndexArray),
		NULL, 
		&PtrValue
		);

	// class.vtbl*

	CPointerType* pResultType = pClassType->GetVTableStructType ()->GetPointerType (EType_Pointer_u);
	m_pModule->m_LlvmBuilder.CreateLoad (PtrValue, pResultType, pResultValue);
	return true;
}

bool
COperatorMgr::GetMethodFunction (
	CFunction* pFunction,
	CClosure* pClosure,
	CValue* pResultValue
	)
{
	ASSERT (pFunction->GetFunctionKind () == EFunction_Method && pClosure);
	
	rtl::CIteratorT <CClosureArg> ClosureArg = pClosure->GetFirstArg ();
	CValue Value = ClosureArg->GetValue ();
	if (ClosureArg->GetArgIdx () != 0 || !Value.GetType ()->IsClassType ())
	{
		err::SetFormatStringError (_T("non-static method call requires an object pointer"));
		return false;
	}

	size_t VTableIndex = pFunction->GetVTableIndex ();

	CClassType* pClassType = (CClassType*) Value.GetType ();
	CVTableType* pVTableType = pFunction->GetVTableType ();
	
	if (pVTableType->GetTypeKind () == EType_Property)
		VTableIndex += ((CPropertyType*) pVTableType)->GetParentVTableIndex ();

	CClassBaseTypeCoord Coord;
	pClassType->FindBaseType (pFunction->GetVTableClassType (), &Coord);
	VTableIndex += Coord.m_VTableIndex;
	
	// class.vtbl*

	CValue PtrValue;
	GetClassVTable (Value, pClassType, &PtrValue);

	// pfn*

	m_pModule->m_LlvmBuilder.CreateGep2 (
		PtrValue, 
		VTableIndex,
		NULL, 
		&PtrValue
		);

	// pfn

	m_pModule->m_LlvmBuilder.CreateLoad (
		PtrValue, 
		NULL,
		&PtrValue
		);

	pResultValue->SetLlvmValue (PtrValue.GetLlvmValue (), pFunction->GetType ()->GetPointerType (EType_Pointer_u));
	return true;
}

bool
COperatorMgr::GetMemberProperty (
	CPropertyType* pPropertyType,
	CClosure* pClosure,
	CValue* pResultValue
	)
{
	ASSERT (pPropertyType->GetGetter ()->GetFunctionKind () == EFunction_Method && pClosure);
	
	rtl::CIteratorT <CClosureArg> ClosureArg = pClosure->GetFirstArg ();
	CValue Value = ClosureArg->GetValue ();
	if (ClosureArg->GetArgIdx () != 0 || !Value.GetType ()->IsClassType ())
	{
		err::SetFormatStringError (_T("non-static property requires an object pointer"));
		return false;
	}

	size_t VTableIndex = pPropertyType->GetParentVTableIndex ();

	CClassType* pClassType = (CClassType*) Value.GetType ();

	CClassBaseTypeCoord Coord;
	pClassType->FindBaseType (pPropertyType->GetParentClassType (), &Coord);
	VTableIndex += Coord.m_VTableIndex;

	// class.vtbl*

	CValue PtrValue;
	GetClassVTable (Value, pClassType, &PtrValue);

	// property.vtbl*

	m_pModule->m_LlvmBuilder.CreateGep2 (
		PtrValue, 
		VTableIndex,
		NULL,
		&PtrValue
		);

	m_pModule->m_LlvmBuilder.CreateBitCast (
		PtrValue, 
		pPropertyType->GetVTableStructType ()->GetPointerType (EType_Pointer_u),
		&PtrValue
		);

	pResultValue->OverrideType (PtrValue, pPropertyType);
	return true;
}

bool
COperatorMgr::StackNewOperator (
	CType* pType,
	CValue* pResultValue
	)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	if (!pScope)
	{
		err::SetFormatStringError (_T("'stack new' operator could not be called at global scope"));
		return false;
	}

	if (pType->GetTypeKind () == EType_Class)
	{
		CClassType* pClassType = (CClassType*) pType;

		CValue ObjPtrValue;
		m_pModule->m_LlvmBuilder.CreateAlloca (pClassType->GetClassStructType (), _T("new"), NULL, &ObjPtrValue);
		
		bool Result = m_pModule->m_LlvmBuilder.InitializeObject (ObjPtrValue, pClassType, pScope);
		if (!Result)
			return false;

		m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, pClassType, pResultValue);

		if (pClassType->GetDestructor ())
			pScope->AddToDestructList (*pResultValue);
	}
	else
	{
		CPointerType* pResultType = m_pModule->m_TypeMgr.GetPointerType (EType_Pointer, pType);

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateAlloca (pType, _T("new"), NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateStore (pType->GetZeroValue (), PtrValue);

		m_pModule->m_LlvmBuilder.CreateSafePtr (
			PtrValue, 
			PtrValue, 
			pType->GetSize (),
			pScope,
			pResultType,
			pResultValue
			);			
	}

	return true;
}

bool
COperatorMgr::HeapNewOperator (
	CType* pType,
	CValue* pResultValue
	)
{
	CValue TypeValue (&pType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));

	CFunction* pHeapAllocate = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_HeapAllocate);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pHeapAllocate,
		pHeapAllocate->GetType (),
		TypeValue,
		&PtrValue
		);

	if (pType->GetTypeKind () == EType_Class)
	{
		CClassType* pClassType = (CClassType*) pType;

		CPointerType* pPointerType = pClassType->GetClassStructType ()->GetPointerType (EType_Pointer_u);
		
		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPointerType, &PtrValue);

		bool Result = m_pModule->m_LlvmBuilder.InitializeObject (PtrValue, pClassType, NULL);
		if (!Result)
			return false;

		m_pModule->m_LlvmBuilder.CreateGep2 (PtrValue, 1, pClassType, pResultValue);
	}
	else
	{
		CPointerType* pPointerType = m_pModule->m_TypeMgr.GetPointerType (EType_Pointer_u, pType);
		CPointerType* pResultType = m_pModule->m_TypeMgr.GetPointerType (EType_Pointer, pType);

		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPointerType, &PtrValue);

		m_pModule->m_LlvmBuilder.CreateSafePtr (
			PtrValue, 
			PtrValue, 
			pType->GetSize (),
			NULL,
			pResultType,
			pResultValue
			);			
	}

	return true;
}

bool
COperatorMgr::ProcessDestructList (rtl::CBoxListT <CValue>* pList)
{
	rtl::CBoxIteratorT <CValue> It = pList->GetHead ();
	for (; It; It++)
	{
		CValue Value = *It;
		CClassType* pType = (CClassType*) Value.GetType ();
		ASSERT (pType->GetTypeKind () == EType_Class);

		CFunction* pDestructor = pType->GetDestructor ();
		ASSERT (pDestructor);

		m_pModule->m_LlvmBuilder.CreateCall (pDestructor, pDestructor->GetType (), Value, NULL);		
	}

	return true;
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
		return GetVarArgType (((CPointerType*) pType)->GetBaseType (), IsUnsafeVarArg);

	case EType_BitField:
		return ((CBitFieldType*) pType)->GetBaseType ();

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
	bool Result;

	rtl::CBoxListT <CValue> EmptyArgList;
	if (!pArgList)
		pArgList = &EmptyArgList;

	CValue OpValue;

	if (RawOpValue.GetValueKind () == EValue_FunctionOverload)
	{
		CFunctionOverload* pFunctionOverload = RawOpValue.GetFunctionOverload ();
		CFunction* pFunction = pFunctionOverload->ChooseOverload (pArgList);
		if (!pFunction)
			return false;

		OpValue.SetFunction (pFunction);
	}
	else
	{
		Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_LoadReference);
		if (!Result)
			return false;
	}

	CType* pOpType = OpValue.GetType ();

	CClosure* pClosure = OpValue.GetClosure ();
	if (pClosure)
	{
		Result = pClosure->Apply (pArgList);
		if (!Result)
			return false;
	}

	if (OpValue.GetValueKind () == EValue_Function)
	{
		CFunction* pFunction = OpValue.GetFunction ();

		if (pFunction->GetFunctionKind () == EFunction_Method)
		{
			if (!pClosure)
			{
				err::SetFormatStringError (_T("non-static method call requires an object pointer"));
				return false;
			}

			Result = GetMethodFunction (pFunction, pClosure, &OpValue);
			if (!Result)
				return false;
		}

		return CallImpl (OpValue, pFunction->GetType (), pArgList, pResultValue);
	}
	
	if (pOpType->GetTypeKind () == EType_Event)
		return CallEvent (OpValue, pArgList);
	else if (pOpType->GetTypeKind () == EType_FunctionPointer)
		return CallFunctionPtr (OpValue, pArgList, pResultValue);
	else if (pOpType->IsUnsafeFunctionPointerType ())
		return CallImpl (OpValue, (CFunctionType*) ((CPointerType*) pOpType)->GetBaseType (), pArgList, pResultValue);
	else 
	{
		err::SetFormatStringError (_T("cannot call '%s'"), pOpType->GetTypeString ());
		return false;
	}
}

bool
COperatorMgr::CalcScopeLevelValue (
	CScope* pScope,
	CValue* pScopeLevelValue
	)
{
	if (!pScope)
	{
		pScopeLevelValue->SetConstSizeT (0);
		return true;
	}

	m_pModule->m_LlvmBuilder.CreateComment ("calc scope level value");

	CFunction* pCurrentFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pCurrentFunction && pCurrentFunction->GetScopeLevelVariable ());

	CValue ScopeBaseLevelValue;
	m_pModule->m_LlvmBuilder.CreateLoad (pCurrentFunction->GetScopeLevelVariable (), NULL, &ScopeBaseLevelValue);

	CValue ScopeIncValue (pScope->GetLevel (), EType_SizeT);

	m_pModule->m_LlvmBuilder.CreateAdd_i (
		ScopeBaseLevelValue, 
		ScopeIncValue, 
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), 
		pScopeLevelValue
		);

	return true;
}

bool
COperatorMgr::CastArgList (
	CFunctionType* pFunctionType,
	rtl::CBoxListT <CValue>* pArgList,
	rtl::CArrayT <CValue>* pArgArray
	)
{
	bool Result;

	rtl::CArrayT <CType*> ArgTypeArray = pFunctionType->GetArgTypeArray ();

	size_t FormalArgCount = ArgTypeArray.GetCount ();
	size_t ActualArgCount = pArgList->GetCount ();

	bool IsVarArg = (pFunctionType->GetFlags () & EFunctionTypeFlag_IsVarArg) != 0;
	bool IsUnsafeVarArg = (pFunctionType->GetFlags () & EFunctionTypeFlag_IsUnsafeVarArg) != 0;

	if (IsVarArg && ActualArgCount < FormalArgCount ||
		!IsVarArg && ActualArgCount != FormalArgCount)
	{
		err::SetFormatStringError (
			_T("'%s' takes %d arguments; %d passed"), 
			pFunctionType->GetTypeString (), 
			FormalArgCount, 
			ActualArgCount
			);
		return false;
	}

	pArgArray->Clear ();
	pArgArray->Reserve (ActualArgCount);
	
	rtl::CBoxIteratorT <CValue> Arg = pArgList->GetHead ();
	for (size_t i = 0; i < FormalArgCount; Arg++, i++)
	{
		CType* pFormalArgType = ArgTypeArray [i];
		
		CValue ArgCast;
		Result = 
			CheckCastKind (Arg->GetType (), pFormalArgType) &&
			CastOperator (*Arg, pFormalArgType, &ArgCast) &&
			PrepareOperand (&ArgCast, EOpFlag_VariableToSafePtr);

		if (!Result)
			return false;

		pArgArray->Append (ArgCast);
	}

	// vararg

	for (; Arg; Arg++)
	{
		CType* pFormalArgType = GetVarArgType (Arg->GetType (), IsUnsafeVarArg);

		CValue ArgCast;
		Result = 
			CastOperator (*Arg, pFormalArgType, &ArgCast) &&
			PrepareOperand (&ArgCast, EOpFlag_VariableToSafePtr);

		if (!Result)
			return false;

		pArgArray->Append (ArgCast);
	}

	return true;
}

bool
COperatorMgr::CallEvent (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	CEventType* pEventType = (CEventType*) OpValue.GetType ();
	ASSERT (pEventType->GetTypeKind () == EType_Event);
	
	CFunctionPointerType* pFunctionPointerType = pEventType->GetFunctionPointerType ();
	CFunctionType* pFunctionType = pFunctionPointerType->GetFunctionType ();

	CValue HandlerValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &HandlerValue);

	CType* pHandlerPtrType = pEventType->GetHandlerStructType ()->GetPointerType (EType_Pointer_u);

	CValue HandlerVariable;
	m_pModule->m_LlvmBuilder.CreateAlloca (pHandlerPtrType, "event_handler", NULL, &HandlerVariable);
	m_pModule->m_LlvmBuilder.CreateStore (HandlerValue, HandlerVariable);

	CBasicBlock* pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("event_cond"));
	CBasicBlock* pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("event_loop"));
	CBasicBlock* pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("event_follow"));

	m_pModule->m_ControlFlowMgr.Follow (pConditionBlock);

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateLoad (HandlerVariable, NULL, &HandlerValue);
	m_pModule->m_LlvmBuilder.CreateEq_i (HandlerValue, pHandlerPtrType->GetZeroValue (), &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFollowBlock, pBodyBlock, pBodyBlock);

	CValue FunctionPtrValue;
	m_pModule->m_LlvmBuilder.CreateLoad (HandlerValue, NULL, &HandlerValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (HandlerValue, 0, pEventType->GetFunctionPointerType (), &FunctionPtrValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (HandlerValue, 1, NULL, &HandlerValue);
	m_pModule->m_LlvmBuilder.CreateStore (HandlerValue, HandlerVariable);

	CValue ResultValue;
	CallFunctionPtr (FunctionPtrValue, pArgList, &ResultValue);

	m_pModule->m_ControlFlowMgr.Jump (pConditionBlock, pFollowBlock);

	return true;
}

bool
COperatorMgr::CallFunctionPtr (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	CFunctionPointerType* pFunctionPointerType = (CFunctionPointerType*) OpValue.GetType ();
	ASSERT (pFunctionPointerType->GetTypeKind () == EType_FunctionPointer);

	CFunctionType* pFunctionType = pFunctionPointerType->GetFunctionType ();

	bool Result = m_pModule->m_LlvmBuilder.CheckNullPtr (OpValue);
	if (!Result)
		return false;
	
	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr)->GetZeroValue ();
	CValue InterfaceValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 2, m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr), &InterfaceValue);

	CBasicBlock* pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("pfn_phi"));
	CBasicBlock* pGlobalBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("pfn_global"));
	CBasicBlock* pMethodBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("pfn_method"));
	
	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (InterfaceValue, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pGlobalBlock, pMethodBlock);

	CValue GlobalReturnValue;
	Result = CallImpl (OpValue, pFunctionPointerType->GetFunctionType (), pArgList, &GlobalReturnValue);
	if (!Result)
		return false;

	pGlobalBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	m_pModule->m_ControlFlowMgr.Jump (pPhiBlock, pMethodBlock);

	pArgList->InsertHead (InterfaceValue);

	CValue MethodReturnValue;
	Result = CallImpl (OpValue, pFunctionPointerType->GetMemberFunctionType (), pArgList, &MethodReturnValue);
	if (!Result)
		return false;

	pMethodBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	m_pModule->m_ControlFlowMgr.Follow (pPhiBlock);

	m_pModule->m_LlvmBuilder.CreatePhi (
		GlobalReturnValue, 
		pGlobalBlock, 
		MethodReturnValue, 
		pMethodBlock, 
		pResultValue
		);

	return true;
}

bool
COperatorMgr::CallImpl (
	const CValue& OpValue,
	CFunctionType* pFunctionType,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	char Buffer [256];
	rtl::CArrayT <CValue> ArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	bool Result = CastArgList (pFunctionType, pArgList, &ArgArray);
	if (!Result)
		return false;

	CScope* pCurrentScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	CValue ScopeLevelValue = CalcScopeLevelValue (pCurrentScope);

	m_pModule->m_LlvmBuilder.CreateComment ("update scope level before call");
	m_pModule->m_LlvmBuilder.CreateStore (ScopeLevelValue, m_pModule->m_VariableMgr.GetScopeLevelVariable ());

	// simple call

	if (OpValue.GetType ()->IsUnsafeFunctionPointerType ())
	{
		m_pModule->m_LlvmBuilder.CreateCall (
			OpValue,
			pFunctionType,
			ArgArray,
			ArgArray.GetCount (),
			pResultValue
			);

		return true;
	}

	// switch between calling conventions
	
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_FunctionPointer);
	CFunctionPointerType* pFunctionPointerType = (CFunctionPointerType*) OpValue.GetType ();

	CValue FunctionPtrValue;
	CValue CallConvValue;
	CValue StdcallValue (ECallConv_Stdcall, EType_Int_p);

	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &FunctionPtrValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 1, NULL, &CallConvValue);

	if (pFunctionType != pFunctionPointerType->GetFunctionType ())
		m_pModule->m_LlvmBuilder.CreateBitCast (FunctionPtrValue, pFunctionType->GetPointerType (EType_Pointer_u), &FunctionPtrValue);

	CBasicBlock* pCmpBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	CBasicBlock* pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("callconv_phi"));
	CBasicBlock* pCdeclBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("callconv_cdecl"));
	CBasicBlock* pStdcallBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("callconv_stdcall"));

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (CallConvValue, StdcallValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pStdcallBlock, pCdeclBlock);
	
	CValue StdcallReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		FunctionPtrValue,
		ECallConv_Stdcall,
		ArgArray,
		ArgArray.GetCount (),
		pFunctionType->GetReturnType (),
		&StdcallReturnValue
		);

	m_pModule->m_ControlFlowMgr.Jump (pPhiBlock, pCdeclBlock);

	CValue CdeclReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		FunctionPtrValue,
		ECallConv_Cdecl,
		ArgArray,
		ArgArray.GetCount (),
		pFunctionType->GetReturnType (),
		&CdeclReturnValue
		);

	m_pModule->m_ControlFlowMgr.Follow (pPhiBlock);

	m_pModule->m_LlvmBuilder.CreatePhi (
		CdeclReturnValue, 
		pCdeclBlock, 
		StdcallReturnValue, 
		pStdcallBlock, 
		pResultValue
		);

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
			if (pBaseType->IsReferenceType () || 
				pBaseType->IsClassType () ||
				(Flags & EOpFlag_LoadReference)) 
				pType = pBaseType;

			break;

		case EType_Property:
			if (!(Flags & EOpFlag_KeepProperty))
				pType = ((CPropertyType*) pType)->GetGetter ()->GetType ()->GetReturnType ();
			break;

		case EType_PropertyPointer:
			if (!(Flags & EOpFlag_KeepProperty))
				pType = ((CPropertyPointerType*) pType)->GetPropertyType ()->GetGetter ()->GetType ()->GetReturnType ();
			break;

		case EType_Enum:
			if (Flags & EOpFlag_EnumToInt)
				pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int);

			break;

		case EType_BitField:
			if (Flags & EOpFlag_BitFieldToInt)
				pType = ((CBitFieldType*) pType)->GetBaseType ();

			break;

		case EType_Bool:
			if (Flags & EOpFlag_BoolToInt)
				pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int);

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
			if (pBaseType->IsReferenceType () || 
				pBaseType->IsClassType () || 
				(Flags & EOpFlag_LoadReference))
			{
				Result = LoadReferenceOperator (&Value);
				if (!Result)
					return false;
			}

			break;

		case EType_Pointer:
			if (Value.GetValueKind () == EValue_Variable && (Flags & EOpFlag_VariableToSafePtr))
				m_pModule->m_LlvmBuilder.CreateSafePtr (
					Value, 
					Value.GetVariable (),
					(CPointerType*) pType,
					&Value
					);
			break;

		case EType_Property:
		case EType_PropertyPointer:
			if (!(Flags & EOpFlag_KeepProperty))
			{
				Result = GetPropertyOperator (Value, &Value);
				if (!Result)
					return false;
			}

			break;

		case EType_Enum:
			if (Flags & EOpFlag_EnumToInt)
				Value.OverrideType (EType_Int);

			break;

		case EType_BitField:
			if (Flags & EOpFlag_BitFieldToInt)
			{
				Result = CastOperator (&Value, ((CBitFieldType*) pType)->GetBaseType ());
				if (!Result)
					return false;
			}

			break;

		case EType_Bool:
			if (Flags & EOpFlag_BoolToInt)
			{
				Result = CastOperator (&Value, EType_Int);
				if (!Result)
					return false;
			}

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

	CValue PtrValue;
	bool Result = PrepareReference (OpValue, EPrepareReferenceFlag_Load, &PtrValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateLoad (PtrValue, pTargetType, pResultValue, IsVolatile);
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
COperatorMgr::CheckCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	ECast CastKind = GetCastKind (pSrcType, pDstType);
	switch (CastKind)
	{
	case ECast_Lossy:
		err::SetFormatStringError (
			_T("conversion from '%s' to '%s' requires explicit cast"),
			pSrcType->GetTypeString (),
			pDstType->GetTypeString ()
			);
		return false;

	case ECast_None:
		SetCastError (pSrcType, pDstType);
		return false;
	}

	return true;
}

bool
COperatorMgr::PrepareReference (
	const CValue& Value,
	int Flags,
	CValue* pPtrValue
	)
{
	CType* pType = Value.GetType ();
	if (pType->GetTypeKind () == EType_Reference_u)
	{
		// no need to do a range check
		*pPtrValue = Value;
		return true;
	}

	ASSERT (pType->GetTypeKind () == EType_Reference);

	CValue ValidatorValue;
	if (Value.GetValueKind () == EValue_Variable)
	{
		*pPtrValue = Value;

		if (!(Value.GetFlags () & EValueFlag_IsVariableOffset) || (Flags & EPrepareReferenceFlag_NoRangeCheck))
			return true;

		m_pModule->m_LlvmBuilder.CreateSafePtrValidator (Value.GetVariable (), &ValidatorValue);
	}
	else
	{
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, pPtrValue);

		if (Flags & EPrepareReferenceFlag_NoRangeCheck)
			return true;

		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 1, NULL, &ValidatorValue);
	}

	ERuntimeError Error = (Flags & EPrepareReferenceFlag_Store) ? ERuntimeError_StoreOutOfRange : ERuntimeError_LoadOutOfRange;

	CType* pTargetType = ((CPointerType*) pType)->GetBaseType ();
	return m_pModule->m_LlvmBuilder.CheckSafePtrRange (*pPtrValue, pTargetType->GetSize (), ValidatorValue, Error);
}

bool
COperatorMgr::StoreReferenceOperator (
	const CValue& RawSrcValue,
	const CValue& RawDstValue,
	bool KeepProperty
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

	if (!KeepProperty && pTargetType->GetTypeKind () == EType_PropertyPointer)
	{
		return 
			LoadReferenceOperator (&DstValue) &&
			SetPropertyOperator (RawSrcValue, DstValue);
	} 

	bool IsVolatile = false;
	if (pTargetType->GetTypeKind () == EType_Qualifier)
	{
		IsVolatile = (pTargetType->GetFlags () & ETypeQualifier_Volatile) != 0;
		pTargetType = ((CDerivedType*) pTargetType)->GetBaseType ();
	}

	if (pTargetType->GetTypeKind () == EType_Event)
		return EventOperator (DstValue, RawSrcValue, EEventOp_SetHandler);

	Result = CheckCastKind (RawSrcValue.GetType (), pTargetType);
	if (!Result)
		return false;

	CValue SrcValue;
	Result = CastOperator (RawSrcValue, pTargetType, &SrcValue);
	if (!Result)
		return false;

	int PrepareReferenceFlags = EPrepareReferenceFlag_Store;

	EType TargetTypeKind = pTargetType->GetTypeKind ();
	switch (TargetTypeKind)
	{
	case EType_Pointer:
		Result = m_pModule->m_LlvmBuilder.CheckSafePtrScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		if (SrcValue.GetValueKind () == EValue_Variable)
			m_pModule->m_LlvmBuilder.CreateSafePtr (
				SrcValue, 
				SrcValue.GetVariable (),
				(CPointerType*) pTargetType,
				&SrcValue
				);

		break;

	case EType_Class:
	case EType_Interface:
		Result = m_pModule->m_LlvmBuilder.CheckInterfaceScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		break;

	case EType_FunctionPointer:
		Result = m_pModule->m_LlvmBuilder.CheckFunctionPointerScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		break;

	case EType_PropertyPointer:
		Result = m_pModule->m_LlvmBuilder.CheckPropertyPointerScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		break;

	case EType_BitField:
		Result = MergeBitField (&SrcValue, DstValue);
		if (!Result)
			return false;

		PrepareReferenceFlags |= EPrepareReferenceFlag_NoRangeCheck; // mergebitfield checks ptr range
		break;
	}
	
	CValue PtrValue;
	Result = PrepareReference (DstValue, PrepareReferenceFlags, &PtrValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateStore (SrcValue, PtrValue, IsVolatile);
	return true;
}

bool
COperatorMgr::EventOperator (
	const CValue& Event,
	const CValue& RawHandler,
	EEventOp OpKind
	)
{
	CPointerType* pReferenceType = (CPointerType*) Event.GetType ();
	ASSERT (pReferenceType->IsReferenceType ());

	CType* pTargetType = pReferenceType->GetBaseType ();
	
	bool IsVolatile = false;
	if (pTargetType->GetTypeKind () == EType_Qualifier)
	{
		IsVolatile = (pTargetType->GetFlags () & ETypeQualifier_Volatile) != 0;
		pTargetType = ((CDerivedType*) pTargetType)->GetBaseType ();
	}

	CEventType* pEventType = (CEventType*) pTargetType;
	ASSERT (pEventType->GetTypeKind () == EType_Event);

	CValue PtrValue;
	bool Result = PrepareReference (Event, EPrepareReferenceFlag_Store, &PtrValue);
	if (!Result)
		return false;

	if (RawHandler.GetValueKind () == EValue_Null)
	{
		if (OpKind == EEventOp_SetHandler)
			m_pModule->m_LlvmBuilder.CreateStore (pEventType->GetZeroValue (), PtrValue, IsVolatile);

		// else ignore += null or -= null;
		return true;
	}

	CValue Handler;
	Result = CastOperator (RawHandler, pEventType->GetFunctionPointerType (), &Handler);
	if (!Result)
		return false;

	CValue FunctionPtr;
	CValue CallConv;
	CValue InterfacePtr;
	m_pModule->m_LlvmBuilder.CreateExtractValue (Handler, 0, NULL, &FunctionPtr);
	m_pModule->m_LlvmBuilder.CreateExtractValue (Handler, 1, NULL, &CallConv);
	m_pModule->m_LlvmBuilder.CreateExtractValue (Handler, 2, NULL, &InterfacePtr);
	m_pModule->m_LlvmBuilder.CreateBitCast (FunctionPtr, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &FunctionPtr);
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractEvent)->GetPointerType (EType_Pointer_u), &PtrValue);

	CFunction* pFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_EventOperator);

	CValue ArgArray [] = 
	{
		PtrValue,
		FunctionPtr,
		CallConv,
		InterfacePtr,
		CValue (OpKind, EType_Int)
	};

	m_pModule->m_LlvmBuilder.CreateCall (
		pFunction,
		pFunction->GetType (),
		ArgArray, 
		countof (ArgArray),
		NULL
		);
	
	return true;
}

bool
COperatorMgr::MergeBitField (
	const CValue& RawSrcValue,
	const CValue& RawDstValue,
	CValue* pResultValue
	)
{
	ASSERT (RawSrcValue.GetType ()->GetTypeKind () == EType_BitField);
	ASSERT (RawDstValue.GetType ()->IsReferenceType ());

	CBitFieldType* pSrcType = (CBitFieldType*) RawSrcValue.GetType ();
	CType* pBaseType = pSrcType->GetBaseType ();
	size_t BitOffset = pSrcType->GetBitOffset ();
	size_t BitCount = pSrcType->GetBitCount ();

	CValue MaskValue;

	if (pSrcType->GetSize () <= 4)
	{
		uint32_t Mask = ~((((uint32_t) 1 << BitCount) - 1) << BitOffset);	
		MaskValue.SetConstInt32 (Mask, EType_Int32_u);
	}
	else
	{
		uint64_t Mask = ~((((uint64_t) 1 << BitCount) - 1) << BitOffset);	
		MaskValue.SetConstInt64 (Mask, EType_Int64_u);
	}

	CValue DstValue;
	bool Result = LoadReferenceOperator (RawDstValue, &DstValue);
	if (!Result)
		return false;

	DstValue.OverrideType (pBaseType);

	CValue SrcValue (RawSrcValue, pBaseType);
	return 
		BinaryOperator (EBinOp_BitwiseAnd, &DstValue, MaskValue) &&
		BinaryOperator (EBinOp_BitwiseOr, SrcValue, DstValue, pResultValue);
}

bool
COperatorMgr::MergeBitField (
	CValue* pValue,
	const CValue& DstValue
	)
{
	CValue ResultValue;

	bool Result = MergeBitField (*pValue, DstValue, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

bool
COperatorMgr::GetPropertyOperator (
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	CPropertyType* pPropertyType;
	CValue OpValue;
	CValue InterfaceValue;

	if (RawOpValue.GetType ()->GetTypeKind () == EType_PropertyPointer)
	{
		m_pModule->m_LlvmBuilder.CheckNullPtr (RawOpValue);

		CPropertyPointerType* pPropertyPointerType = (CPropertyPointerType*) RawOpValue.GetType ();
		pPropertyType = pPropertyPointerType->GetPropertyType ();
		m_pModule->m_LlvmBuilder.CreateExtractValue (RawOpValue, 0, pPropertyType, &OpValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (RawOpValue, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr), &InterfaceValue);
	}
	else
	{
		ASSERT (RawOpValue.GetType ()->GetTypeKind () == EType_Property);
		pPropertyType = (CPropertyType*) RawOpValue.GetType ();
		OpValue = RawOpValue;
	}
	
	CFunction* pFunction = pPropertyType->GetGetter ();
	ASSERT (pFunction->GetVTableType () == pPropertyType);

	CValue FunctionValue;

	if (OpValue.GetValueKind () == EValue_Property)
	{
		FunctionValue = pFunction;
	}
	else
	{
		// pfn*

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (OpValue, pFunction->GetVTableIndex (), NULL, &PtrValue);

		// pfn

		m_pModule->m_LlvmBuilder.CreateLoad (PtrValue, pFunction->GetType ()->GetPointerType (EType_Pointer_u), &FunctionValue);
	}

	if (RawOpValue.GetType ()->GetTypeKind () == EType_PropertyPointer)
	{
		m_pModule->m_LlvmBuilder.CreateFunctionPointer (
			FunctionValue, 
			pFunction->GetType ()->GetCallingConvention (),
			InterfaceValue,
			pFunction->GetType ()->GetFunctionPointerType (),
			&FunctionValue
			);
	}

	FunctionValue.SetClosure (OpValue.GetClosure ());
	return CallOperator (FunctionValue, NULL, pResultValue);
}

bool
COperatorMgr::SetPropertyOperator (
	const CValue& SrcValue,
	const CValue& RawDstValue
	)
{
	CPropertyType* pPropertyType;
	CValue DstValue;
	CValue InterfaceValue;

	if (RawDstValue.GetType ()->GetTypeKind () == EType_PropertyPointer)
	{
		CPropertyPointerType* pPropertyPointerType = (CPropertyPointerType*) RawDstValue.GetType ();
		pPropertyType = pPropertyPointerType->GetPropertyType ();
		m_pModule->m_LlvmBuilder.CreateExtractValue (RawDstValue, 0, pPropertyType, &DstValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (RawDstValue, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr), &InterfaceValue);
	}
	else
	{
		ASSERT (RawDstValue.GetType ()->GetTypeKind () == EType_Property);
		pPropertyType = (CPropertyType*) RawDstValue.GetType ();
		DstValue = RawDstValue;
	}

	if (pPropertyType->IsReadOnly ())
	{
		err::SetFormatStringError (_T("cannot assign to a read-only property"));
		return false;
	}

	rtl::CBoxListT <CValue> ArgList;
	ArgList.InsertTail (SrcValue);

	CFunctionOverload* pSetter = pPropertyType->GetSetter ();
	CFunction* pFunction = pSetter->ChooseOverload (&ArgList);
	if (!pFunction)
		return false;

	ASSERT (pFunction->GetVTableType () == pPropertyType);

	CValue FunctionValue;
	if (DstValue.GetValueKind () == EValue_Property)
	{
		FunctionValue = pFunction;
	}
	else
	{
		// pfn*

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (DstValue, pFunction->GetVTableIndex (), NULL, &PtrValue);

		// pfn

		m_pModule->m_LlvmBuilder.CreateLoad (PtrValue, pFunction->GetType ()->GetPointerType (EType_Pointer_u), &FunctionValue);
	}

	if (RawDstValue.GetType ()->GetTypeKind () == EType_PropertyPointer)
	{
		m_pModule->m_LlvmBuilder.CreateFunctionPointer (
			FunctionValue, 
			pFunction->GetType ()->GetCallingConvention (),
			InterfaceValue,
			pFunction->GetType ()->GetFunctionPointerType (),
			&FunctionValue
			);
	}

	FunctionValue.SetClosure (DstValue.GetClosure ());

	CValue ReturnValue;
	return CallOperator (FunctionValue, &ArgList, &ReturnValue);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
