// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnaryOperator.h"
#include "axl_jnc_BinaryOperator.h"
#include "axl_jnc_MoveOperator.h"

namespace axl {
namespace jnc {

//.............................................................................

class COperatorMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	// unary operators

	CUnOpT_i32 <CUnOp_Minus> m_UnOp_Minus_i32;
	CUnOpT_i64 <CUnOp_Minus> m_UnOp_Minus_i64;
	CUnOpT_f32 <CUnOp_Minus> m_UnOp_Minus_f32;
	CUnOpT_f64 <CUnOp_Minus> m_UnOp_Minus_f64;

	CUnOpT_i32 <CUnOp_BitwiseNot> m_UnOp_BitwiseNot_i32;
	CUnOpT_i64 <CUnOp_BitwiseNot> m_UnOp_BitwiseNot_i64;

	// binary operators

	CBinOpT_i32 <CBinOp_Add> m_BinOp_Add_i32;
	CBinOpT_i64 <CBinOp_Add> m_BinOp_Add_i64;
	CBinOpT_f32 <CBinOp_Add> m_BinOp_Add_f32;
	CBinOpT_f64 <CBinOp_Add> m_BinOp_Add_f64;

	CBinOpT_i32 <CBinOp_Sub> m_BinOp_Sub_i32;
	CBinOpT_i64 <CBinOp_Sub> m_BinOp_Sub_i64;
	CBinOpT_f32 <CBinOp_Sub> m_BinOp_Sub_f32;
	CBinOpT_f64 <CBinOp_Sub> m_BinOp_Sub_f64;

	CBinOpT_i32 <CBinOp_Mul> m_BinOp_Mul_i32;
	CBinOpT_i64 <CBinOp_Mul> m_BinOp_Mul_i64;
	CBinOpT_f32 <CBinOp_Mul> m_BinOp_Mul_f32;
	CBinOpT_f64 <CBinOp_Mul> m_BinOp_Mul_f64;

	CBinOpT_i32 <CBinOp_Div> m_BinOp_Div_i32;
	CBinOpT_i64 <CBinOp_Div> m_BinOp_Div_i64;
	CBinOpT_f32 <CBinOp_Div> m_BinOp_Div_f32;
	CBinOpT_f64 <CBinOp_Div> m_BinOp_Div_f64;
	CBinOpT_i32 <CBinOp_DivU> m_BinOp_Div_i32u;
	CBinOpT_i64 <CBinOp_DivU> m_BinOp_Div_i64u;

	CBinOpT_i32 <CBinOp_Mod> m_BinOp_Mod_i32;
	CBinOpT_i64 <CBinOp_Mod> m_BinOp_Mod_i64;
	CBinOpT_i32 <CBinOp_ModU> m_BinOp_Mod_i32u;
	CBinOpT_i64 <CBinOp_ModU> m_BinOp_Mod_i64u;

	CBinOpT_i32 <CBinOp_Shl> m_BinOp_Shl_i32;
	CBinOpT_i64 <CBinOp_Shl> m_BinOp_Shl_i64;
	CBinOpT_i32 <CBinOp_Shr> m_BinOp_Shr_i32;
	CBinOpT_i64 <CBinOp_Shr> m_BinOp_Shr_i64;

	CBinOpT_i32 <CBinOp_BitwiseAnd> m_BinOp_BitwiseAnd_i32;
	CBinOpT_i64 <CBinOp_BitwiseAnd> m_BinOp_BitwiseAnd_i64;
	CBinOpT_i32 <CBinOp_BitwiseOr> m_BinOp_BitwiseOr_i32;
	CBinOpT_i64 <CBinOp_BitwiseOr> m_BinOp_BitwiseOr_i64;
	CBinOpT_i32 <CBinOp_BitwiseXor> m_BinOp_BitwiseXor_i32;
	CBinOpT_i64 <CBinOp_BitwiseXor> m_BinOp_BitwiseXor_i64;

	//CRelOp_Eq m_RelOp_Eq;
	//CRelOp_Ne m_RelOp_Ne;
	//CRelOp_Lt m_RelOp_Lt;
	//CRelOp_Le m_RelOp_Le;
	//CRelOp_Gt m_RelOp_Gt;
	//CRelOp_Ge m_RelOp_Ge;

	// move operators

	CMove_8 m_Move_8;
	CMove_16 m_Move_16;
	CMove_32 m_Move_32;
	CMove_64 m_Move_64;	
	CMove_i16_swp m_Move_i16_swp;
	CMove_i32_swp m_Move_i32_swp;
	CMove_i64_swp m_Move_i64_swp;
	CMove_i8_16 m_Move_i8_16;
	CMove_i8u_16 m_Move_i8u_16;
	CMove_i8_32 m_Move_i8_32;
	CMove_i8u_32 m_Move_i8u_32;
	CMove_i8_64 m_Move_i8_64;
	CMove_i8u_64 m_Move_i8u_64;
	CMove_i16_32 m_Move_i16_32;
	CMove_i16u_32 m_Move_i16u_32;
	CMove_i16_64 m_Move_i16_64;
	CMove_i16u_64 m_Move_i16u_64;
	CMove_i32_64 m_Move_i32_64;
	CMove_i32u_64 m_Move_i32u_64;
	CMove_i32_f32 m_Move_i32_f32;
	CMove_i32u_f32 m_Move_i32u_f32;
	CMove_i32_f64 m_Move_i32_f64;
	CMove_i32u_f64 m_Move_i32u_f64;
	CMove_i64_f32 m_Move_i64_f32;
	CMove_i64u_f32 m_Move_i64u_f32;
	CMove_i64_f64 m_Move_i64_f64;
	CMove_i64u_f64 m_Move_i64u_f64;
	CMove_f32_i32 m_Move_f32_i32;
	CMove_f32_i64 m_Move_f32_i64;
	CMove_f64_i32 m_Move_f64_i32;
	CMove_f64_i64 m_Move_f64_i64;

	// tables

	CUnaryOperatorOverload m_UnaryOperatorTable [EUnOp__Count];
	CBinaryOperatorOverload m_BinaryOperatorTable [EBinOp__Count];	
	CMoveOperator* m_BasicMoveOperatorTable [EType__BasicTypeCount] [EType__BasicTypeCount];
	rtl::CStringHashTableMapT <CMoveOperator*> m_MoveOperatorMap;
	rtl::CStdListT <CMoveOperator> m_MoveOperatorList;
	rtl::CStdListT <CSuperMove> m_SuperMoveList;
	
public:
	COperatorMgr (CModule* pModule);

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	void
	AddStdOperators ();

	// unary operators

	CUnaryOperator*
	FindUnaryOperator (
		EUnOp OpKind,
		CType* pOpType
		)
	{
		ASSERT (OpKind > 0 && OpKind < EUnOp__Count);
		return m_UnaryOperatorTable [OpKind].FindOperator (pOpType);
	}

	CUnaryOperator*
	AddUnaryOperator (
		EUnOp OpKind,
		CType* pReturnType,
		CType* pOpType,
		IUnaryOperatorLo* pOperatorLo
		)
	{
		ASSERT (OpKind > 0 && OpKind < EUnOp__Count);
		return m_UnaryOperatorTable [OpKind].AddOperator (pReturnType, pOpType, pOperatorLo);
	}

	CUnaryOperator*
	AddUnaryOperator (
		EUnOp OpKind,
		EType ReturnTypeKind,
		EType OpTypeKind,
		IUnaryOperatorLo* pOperatorLo
		);

	CUnaryOperator*
	AddUnaryOperator (
		EUnOp OpKind,
		CType* pType,
		IUnaryOperatorLo* pOperatorLo
		)
	{
		return AddUnaryOperator (OpKind, pType, pType, pOperatorLo);
	}

	CUnaryOperator*
	AddUnaryOperator (
		EUnOp OpKind,
		EType TypeKind,
		IUnaryOperatorLo* pOperatorLo
		)
	{
		return AddUnaryOperator (OpKind, TypeKind, TypeKind, pOperatorLo);
	}

	bool
	UnaryOperator (
		EUnOp OpKind,
		const CValue& OpValue,
		CValue* pResultValue
		);
	
	bool
	UnaryOperator (
		EUnOp OpKind,
		CValue* pValue
		);

	// binary operators

	CBinaryOperator*
	FindBinaryOperator (
		EBinOp OpKind,
		CType* pOpType1,
		CType* pOpType2
		)
	{
		ASSERT (OpKind > 0 && OpKind < EBinOp__Count);
		return m_BinaryOperatorTable [OpKind].FindOperator (pOpType1, pOpType2);
	}

	CBinaryOperator*
	AddBinaryOperator (
		EBinOp OpKind,
		CType* pReturnType,
		CType* pOpType1,
		CType* pOpType2,
		IBinaryOperatorLo* pOperatorLo
		)
	{
		ASSERT (OpKind > 0 && OpKind < EBinOp__Count);
		return m_BinaryOperatorTable [OpKind].AddOperator (pReturnType, pOpType1, pOpType2, pOperatorLo);
	}

	CBinaryOperator*
	AddBinaryOperator (
		EBinOp OpKind,
		EType ReturnTypeKind,
		EType OpTypeKind1,
		EType OpTypeKind2,
		IBinaryOperatorLo* pOperatorLo
		);

	CBinaryOperator*
	AddBinaryOperator (
		EBinOp OpKind,
		CType* pType,
		IBinaryOperatorLo* pOperatorLo
		)
	{
		return AddBinaryOperator (OpKind, pType, pType, pType, pOperatorLo);
	}

	CBinaryOperator*
	AddBinaryOperator (
		EBinOp OpKind,
		EType TypeKind,
		IBinaryOperatorLo* pOperatorLo
		)
	{
		return AddBinaryOperator (OpKind, TypeKind, TypeKind, TypeKind, pOperatorLo);
	}

	bool
	BinaryOperator (
		EBinOp OpKind,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);

	bool
	BinaryOperator (
		EBinOp OpKind,
		CValue* pValue,
		const CValue& OpValue2
		);

	// move operators

	ECanMove
	CanMove (
		CType* pSrcType,
		CType* pDstType
		)
	{
		CMoveOperator* pMoveOperator = FindMoveOperator (pSrcType, pDstType);
		return pMoveOperator ? pMoveOperator->GetCanMove () : ECanMove_None;
	}

	CMoveOperator*
	FindMoveOperator (
		CType* pSrcType,
		CType* pDstType
		);

	CMoveOperator*
	AddMoveOperator (
		CType* pSrcType,
		CType* pDstType,
		IMoveOperatorLo* pOperatorLo
		);

	CMoveOperator*
	AddMoveOperator (
		EType SrcTypeKind,
		EType DstTypeKind,
		IMoveOperatorLo* pOperatorLo
		);

	bool
	MoveOperator (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	bool
	MoveOperator (
		const CValue& SrcValue,
		const CValue& DstValue,
		EBinOp Op
		);

	// cast operators

	bool
	CastOperator (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);

	bool
	CastOperator (
		CValue* pValue,
		CType* pType
		);

	// misc operators

	bool
	ConditionalOperator (
		CValue* pOpValue,
		const CValue& TrueValue,
		const CValue& FalseValue
		);

	bool
	MemberOperator (
		const CValue& OpValue,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	MemberOperator (
		CValue* pValue,
		const tchar_t* pName
		);

	bool
	CallOperator (
		CValue* pValue,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	PostfixIncOperator (CValue* pValue);

	bool
	PostfixDecOperator (CValue* pValue);

protected:
	void
	AddStdUnaryOperators ();

	void
	AddStdBinaryOperators ();

	void
	AddStdMoveOperators ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {

