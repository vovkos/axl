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

	m_UnaryOperatorTable [EUnOp_Plus]       = &m_UnOp_Plus;
	m_UnaryOperatorTable [EUnOp_Minus]      = &m_UnOp_Minus;
	m_UnaryOperatorTable [EUnOp_BwNot]      = &m_UnOp_BwNot;
	m_UnaryOperatorTable [EUnOp_Addr]       = &m_UnOp_Addr;
	m_UnaryOperatorTable [EUnOp_Indir]      = &m_UnOp_Indir;
	m_UnaryOperatorTable [EUnOp_LogNot]     = NULL; // &m_UnOp_LogicalNot;
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
	m_BinaryOperatorTable [EBinOp_BwAnd]      = &m_BinOp_BwAnd;
	m_BinaryOperatorTable [EBinOp_BwOr]       = &m_BinOp_BwOr;
	m_BinaryOperatorTable [EBinOp_BwXor]      = &m_BinOp_BwXor;
	m_BinaryOperatorTable [EBinOp_Eq]         = &m_BinOp_Eq;
	m_BinaryOperatorTable [EBinOp_Ne]         = &m_BinOp_Ne;
	m_BinaryOperatorTable [EBinOp_Lt]         = &m_BinOp_Lt;
	m_BinaryOperatorTable [EBinOp_Le]         = &m_BinOp_Le;
	m_BinaryOperatorTable [EBinOp_Gt]         = &m_BinOp_Gt;
	m_BinaryOperatorTable [EBinOp_Ge]         = &m_BinOp_Ge;
	m_BinaryOperatorTable [EBinOp_LogAnd]     = NULL; // &m_BinOp_LogicalAnd;
	m_BinaryOperatorTable [EBinOp_LogOr]      = NULL; // &m_BinOp_LogicalOr;
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
		err::SetFormatStringError (_T("unary '%s' is not supported"), GetUnOpKindString (OpKind));
		return false;
	}

	CValue OpValue;
	return 
		PrepareOperand (RawOpValue, &OpValue, pOperator->GetOpFlags ()) &&
		pOperator->Operator (OpValue, pResultValue);
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
		err::SetFormatStringError (_T("binary '%s' is not supported"), GetBinOpKindString (OpKind));
		return false;
	}

	CValue OpValue1;
	CValue OpValue2;

	return
		PrepareOperand (RawOpValue1, &OpValue1, pOperator->GetOpFlags1 ()) &&
		PrepareOperand (RawOpValue2, &OpValue2, pOperator->GetOpFlags2 ()) &&
		pOperator->Operator (OpValue1, OpValue2, pResultValue);
}

bool
COperatorMgr::CastOperator (
	EAlloc AllocKind,
	const CValue& RawOpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ICastOperator* pOperator = GetCastOperator (RawOpValue.GetType (), pType);	
	if (!pOperator)
	{
		SetCastError (RawOpValue.GetType (), pType);
		return false;
	}

	CValue OpValue;
	return 
		PrepareOperand (RawOpValue, &OpValue, pOperator->GetOpFlags ()) &&
		pOperator->Cast (AllocKind, OpValue, pType, pResultValue);
}

bool
COperatorMgr::CastOperator (
	EAlloc AllocKind,
	const CValue& OpValue,
	EType TypeKind,
	CValue* pResultValue
	)
{
	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
	return CastOperator (AllocKind, OpValue, pType, pResultValue);
}

ICastOperator*
COperatorMgr::GetCastOperator (
	CType* pOpType,
	CType* pType
	)
{
	EType TypeKind = pType->GetTypeKind ();

	int OpFlags = TypeKind == EType_PropertyPtr ? EOpFlag_KeepPropertyRef : 0;
	pOpType = PrepareOperandType (pOpType, OpFlags);
	if (pOpType->Cmp (pType) == 0)
		return &m_Cast_cpy;

	EType OpTypeKind = pOpType->GetTypeKind ();

	CType* pBaseType;

	switch (OpTypeKind)
	{
	case EType_Array:
		return 
			TypeKind == EType_Array ? (ICastOperator*) &m_Cast_arr :
			TypeKind == EType_DataPtr ? (ICastOperator*) &m_Cast_arr_ptr : NULL;

	case EType_BitField:
		pBaseType = ((CBitFieldType*) pOpType)->GetBaseType ();
		return GetCastOperator (pBaseType, pType) ? &m_Cast_getbf : NULL;

	case EType_DataRef:
		if (TypeKind == EType_PropertyPtr)
			return &m_Cast_prop;

		pBaseType = ((CDataPtrType*) pOpType)->GetDataType ();
		return 
			pBaseType->GetTypeKind () == EType_Array ? GetCastOperator (pBaseType, pType) : 
			GetCastOperator (pBaseType, pType) ? &m_Cast_load : NULL;

	case EType_DataPtr:
		return TypeKind == EType_DataPtr ? &m_Cast_ptr : NULL;

	case EType_ClassPtr:
		return TypeKind == EType_ClassPtr ? &m_Cast_class : NULL;

	case EType_FunctionRef:
	case EType_FunctionPtr:
		return &m_Cast_fn;

	case EType_PropertyRef:
		if (TypeKind == EType_PropertyPtr)
			return &m_Cast_prop;

		pBaseType = ((CPropertyPtrType*) pOpType)->GetPropertyType ()->GetReturnType ();
		return GetCastOperator (pBaseType, pType) ? &m_Cast_getp : NULL;

	case EType_PropertyPtr:
		return TypeKind == EType_PropertyPtr ? &m_Cast_prop : NULL;

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
	CType* pSrcType,
	CType* pDstType
	)
{
	ICastOperator* pOperator = GetCastOperator (pSrcType, pDstType);
	pSrcType = PrepareOperandType (pSrcType, pOperator->GetOpFlags ());
	return pOperator ? pOperator->GetCastKind (pSrcType, pDstType) : ECast_None;
}

ECast
COperatorMgr::GetCastKind (
	const CValue& SrcValue,
	CType* pDstType
	)
{
	CType* pSrcType = SrcValue.GetType ();
	CClosure* pClosure = SrcValue.GetClosure ();
	if (pClosure)
	{
		pSrcType = pClosure->GetClosureType (pSrcType);
		if (!pSrcType)
			return ECast_None;
	}

	return GetCastKind (pSrcType, pDstType);
}

ECast
COperatorMgr::GetArgCastKind (
	CFunctionType* pFunctionType,
	const rtl::CArrayT <CType*>& ActualArgTypeArray
	)
{
	rtl::CArrayT <CType*> FormalArgTypeArray = pFunctionType->GetArgTypeArray ();
	size_t FormalArgCount = FormalArgTypeArray.GetCount ();
	size_t ActualArgCount = ActualArgTypeArray.GetCount ();

	if (ActualArgCount < FormalArgCount || 
		ActualArgCount > FormalArgCount && !(pFunctionType->GetFlags () & EFunctionTypeFlag_VarArg))
	{
		return ECast_None;
	}

	ECast WorstCastKind = ECast_Identitiy;

	for (size_t i = 0; i < FormalArgCount; i++)
	{
		CType* pFormalArgType = FormalArgTypeArray [i];
		CType* pActualArgType = ActualArgTypeArray [i];;

		ECast CastKind = GetCastKind (pActualArgType, pFormalArgType);
		if (!CastKind)
			return ECast_None;

		if (CastKind < WorstCastKind)
			WorstCastKind = CastKind;
	}

	return WorstCastKind;
}

ECast
COperatorMgr::GetArgCastKind (
	CFunctionType* pFunctionType,
	const rtl::CBoxListT <CValue>* pArgList
	)
{
	size_t ArgCount = pArgList->GetCount ();

	char Buffer [256];
	rtl::CArrayT <CType*> ArgTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgTypeArray.SetCount (ArgCount);

	rtl::CBoxIteratorT <CValue> Arg = pArgList->GetHead ();
	for (size_t i = 0; Arg; Arg++, i++)
		ArgTypeArray [i] = Arg->GetType ();

	return GetArgCastKind (pFunctionType, ArgTypeArray);
}

ECast
COperatorMgr::GetFunctionCastKind (
	CFunctionType* pSrcType,
	CFunctionType* pDstType
	)
{
	ECast ArgCastKind = GetArgCastKind (pSrcType, pDstType->GetArgTypeArray ());
	if (!ArgCastKind)
		return ECast_None;

	CType* pSrcReturnType = pSrcType->GetReturnType ();
	CType* pDstReturnType = pDstType->GetReturnType ();
	
	if (pDstReturnType->GetTypeKind () == EType_Void)
		return ArgCastKind;

	ECast ReturnCastKind = GetCastKind (pSrcReturnType, pDstReturnType);
	return min (ArgCastKind, ReturnCastKind);
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
COperatorMgr::CheckCastKind (
	const CValue& SrcValue,
	CType* pDstType
	)
{
	CType* pSrcType = SrcValue.GetType ();
	CClosure* pClosure = SrcValue.GetClosure ();
	if (pClosure)
	{
		pSrcType = pClosure->GetClosureType (pSrcType);
		if (!pSrcType)
			return false;
	}

	return CheckCastKind (pSrcType, pDstType);
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
	case EType_DataRef:
		return StoreReferenceOperator (OpValue, DstValue, false);

	case EType_PropertyRef:
		return SetPropertyOperator (OpValue, DstValue);

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
	CType* pTargetType = PrepareOperandType (DstValue.GetType ());
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
			err::SetFormatStringError (_T("invalid operator for events"), GetBinOpKindString (OpKind));
			return false;
		}

		break;

	case EType_ClassPtr:
//		#pragma AXL_TODO ("overloaded operators for interfaces & classes")
		break;
	}

	CValue RValue;
	return 
		BinaryOperator (OpKind, DstValue, SrcValue, &RValue) &&
		MoveOperator (RValue, DstValue);
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
		CDataPtrType* pResultType = m_pModule->m_TypeMgr.GetDataPtrType (pType, EType_DataPtr);

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateAlloca (pType, _T("new"), NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateStore (pType->GetZeroValue (), PtrValue);

		m_pModule->m_LlvmBuilder.CreateDataPtr (
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

		CDataPtrType* pPointerType = pClassType->GetClassStructType ()->GetDataPtrType (EDataPtrType_Unsafe);
		
		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPointerType, &PtrValue);

		bool Result = m_pModule->m_LlvmBuilder.InitializeObject (PtrValue, pClassType, NULL);
		if (!Result)
			return false;

		m_pModule->m_LlvmBuilder.CreateGep2 (PtrValue, 1, pClassType, pResultValue);
	}
	else
	{
		CDataPtrType* pPointerType = pType->GetDataPtrType (EDataPtrType_Unsafe);
		CDataPtrType* pResultType = pType->GetDataPtrType ();

		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPointerType, &PtrValue);

		m_pModule->m_LlvmBuilder.CreateDataPtr (
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
		ASSERT (Value.GetType ()->GetTypeKind () == EType_ClassPtr);
		
		CClassType* pType = ((CClassPtrType*) Value.GetType ())->GetClassType ();

		CFunction* pDestructor = pType->GetDestructor ();
		ASSERT (pDestructor);

		m_pModule->m_LlvmBuilder.CreateCall (pDestructor, pDestructor->GetType (), Value, NULL);		
	}

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
		case EType_DataRef:
			if (!(Flags & EOpFlag_KeepDataRef)) // refine
				pType = ((CDataPtrType*) pType)->GetDataType ();

			break;

		case EType_PropertyRef:
			if (!(Flags & EOpFlag_KeepPropertyRef))
				pType = ((CPropertyPtrType*) pType)->GetPropertyType ()->GetReturnType ();

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
		case EType_DataRef:
			if (!(Flags & EOpFlag_KeepPropertyRef))
			{
				Result = LoadReferenceOperator (&Value);
				if (!Result)
					return false;
			}

			break;

		case EType_PropertyRef:
			if (!(Flags & EOpFlag_KeepPropertyRef))
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
COperatorMgr::EventOperator (
	const CValue& Event,
	const CValue& RawHandler,
	EEventOp OpKind
	)
{
	ASSERT (Event.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pReferenceType = (CDataPtrType*) Event.GetType ();

	CType* pTargetType = pReferenceType->GetDataType ();
	
	CEventType* pEventType = (CEventType*) pTargetType;
	ASSERT (pEventType->GetTypeKind () == EType_Event);

	CValue EventPtrValue;
	bool Result = PrepareReference (Event, EPrepareReferenceFlag_Store, &EventPtrValue);
	if (!Result)
		return false;

	if (RawHandler.GetValueKind () == EValue_Null)
	{
		if (OpKind == EEventOp_SetHandler)
			m_pModule->m_LlvmBuilder.CreateStore (
				pEventType->GetZeroValue (), 
				EventPtrValue, 
				(pReferenceType->GetFlags () & EPtrTypeFlag_Volatile) != 0
				);

		// else ignore += null or -= null;
		return true;
	}

	CValue Handler;
	Result = CastOperator (RawHandler, pEventType->GetFunctionPtrType (), &Handler);
	if (!Result)
		return false;

	CValue FunctionPtr;
	CValue InterfacePtr;
	m_pModule->m_LlvmBuilder.CreateExtractValue (Handler, 0, NULL, &FunctionPtr);
	m_pModule->m_LlvmBuilder.CreateExtractValue (Handler, 1, NULL, &InterfacePtr);
	m_pModule->m_LlvmBuilder.CreateBitCast (FunctionPtr, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &FunctionPtr);
	m_pModule->m_LlvmBuilder.CreateBitCast (EventPtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleEvent)->GetDataPtrType (EDataPtrType_Unsafe), &EventPtrValue);

	CFunction* pFunction = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_EventOperator);

	m_pModule->m_LlvmBuilder.CreateCall4 (
		pFunction,
		pFunction->GetType (),
		EventPtrValue,
		FunctionPtr,
		InterfacePtr,
		CValue (OpKind, EType_Int),
		NULL
		);
	
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
