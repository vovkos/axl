// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnaryOperator.h"
#include "axl_jnc_BinaryOperator.h"
#include "axl_jnc_CastOperator.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

class COperatorMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	// unary operators

	CUnOp_Minus m_UnOp_Minus;
	CUnOp_BitwiseNot m_UnOp_BitwiseNot;
	CUnOp_Addr m_UnOp_Addr;
	CUnOp_Indir m_UnOp_Indir;
	
	CUnOpT_PreInc <EUnOp_PreInc> m_UnOp_PreInc;
	CUnOpT_PreInc <EUnOp_PreDec> m_UnOp_PreDec;
	CUnOpT_PostInc <EUnOp_PostInc> m_UnOp_PostInc;
	CUnOpT_PostInc <EUnOp_PostDec> m_UnOp_PostDec;

	// arithmetic binary operators

	CBinOp_Add m_BinOp_Add;
	CBinOp_Sub m_BinOp_Sub;
	CBinOp_Mul m_BinOp_Mul;
	CBinOp_Div m_BinOp_Div;
	CBinOp_Mod m_BinOp_Mod;
	CBinOp_Shl m_BinOp_Shl;
	CBinOp_Shr m_BinOp_Shr;
	CBinOp_BitwiseAnd m_BinOp_BitwiseAnd;
	CBinOp_BitwiseOr m_BinOp_BitwiseOr;
	CBinOp_BitwiseXor m_BinOp_BitwiseXor;

	// relational operators

	CBinOp_Eq m_BinOp_Eq;
	CBinOp_Ne m_BinOp_Ne;
	CBinOp_Lt m_BinOp_Lt;
	CBinOp_Le m_BinOp_Le;
	CBinOp_Gt m_BinOp_Gt;
	CBinOp_Ge m_BinOp_Ge;

	// special binary operators

	CBinOp_Idx m_BinOp_Idx;

	// cast operators

	CCast_cpy m_Cast_cpy;
	CCast_load m_Cast_load;
	CCast_getp m_Cast_getp;

	CCast_int_trunc m_Cast_int_trunc;
	CCast_int_ext m_Cast_int_ext;
	CCast_int_ext_u m_Cast_int_ext_u;
	CCast_int_swp m_Cast_int_swp;

	CCast_f32_f64 m_Cast_f32_f64;
	CCast_f64_f32 m_Cast_f64_f32;
	
	CCast_i32_f32 m_Cast_i32_f32;
	CCast_i32u_f32 m_Cast_i32u_f32;
	CCast_i32_f64 m_Cast_i32_f64;
	CCast_i32u_f64 m_Cast_i32u_f64;
	CCast_i64_f32 m_Cast_i64_f32;
	CCast_i64u_f32 m_Cast_i64u_f32;
	CCast_i64_f64 m_Cast_i64_f64;
	CCast_i64u_f64 m_Cast_i64u_f64;

	CCast_f32_i32 m_Cast_f32_i32;
	CCast_f32_i64 m_Cast_f32_i64;
	CCast_f64_i32 m_Cast_f64_i32;
	CCast_f64_i64 m_Cast_f64_i64;
	
	CCast_num_bool m_Cast_num_bool;

	CCast_getbf m_Cast_getbf;
	CCast_setbf m_Cast_setbf;

	CCast_ptr m_Cast_ptr;
	CCast_arr m_Cast_arr;
	CCast_arr_ptr m_Cast_arr_ptr;

	CCast_class m_Cast_class;

	// tables

	IUnaryOperator* m_UnaryOperatorTable [EUnOp__Count];
	IBinaryOperator* m_BinaryOperatorTable [EBinOp__Count];		
	ICastOperator* m_CastOperatorTable [EType__PrimitiveTypeCount] [EType__PrimitiveTypeCount];

	rtl::CStdListT <CSuperCast> m_SuperCastList;
	
public:
	COperatorMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	// prepare is: unqualify, get property, peel double references

	CType* 
	PrepareOperandType (
		CType* pOpType,
		int Flags = 0
		);

	bool 
	PrepareOperand (
		const CValue& OpValue,
		CValue* pOpValue,
		int Flags = 0
		);

	bool 
	PrepareOperand (
		CValue* pOpValue,
		int Flags = 0
		);

	// unary operators

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

	// move & cast operators

	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		);

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

	bool
	CastOperator (
		const CValue& OpValue,
		EType TypeKind,
		CValue* pResultValue
		);

	bool
	CastOperator (
		CValue* pValue,
		EType TypeKind
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
		EBinOp OpKind
		);

	// misc operators

	bool
	StackNewOperator (
		CType* pType,
		CValue* pResultValue
		);

	bool
	HeapNewOperator (
		CType* pType,
		CValue* pResultValue
		);

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
	PointerToMemberOperator (
		const CValue& OpValue,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	PointerToMemberOperator (
		CValue* pValue,
		const tchar_t* pName
		);

	bool
	CallOperator (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	CallOperator (
		CValue* pValue,
		rtl::CBoxListT <CValue>* pArgList
		);

	// load & store operators

	bool
	LoadReferenceOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	LoadReferenceOperator (CValue* pValue);

	bool
	StoreReferenceOperator (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	bool
	GetPropertyOperator (
		CProperty* pProperty,
		CValue* pResultValue
		);

	bool
	SetPropertyOperator (
		const CValue& OpValue,
		CProperty* pProperty
		);

protected:
	ICastOperator*
	GetCastOperator (
		CType* pSrcType,
		CType* pDstType
		);

	// member operators

	bool
	StructMemberOperator (
		const CValue& OpValue,
		CStructType* pStructType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	UnionMemberOperator (
		const CValue& OpValue,
		CUnionType* pUnionType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	ClassMemberOperator (
		const CValue& OpValue,
		CClassType* pClassType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	ClassFieldMemberOperator (
		const CValue& OpValue,
		CClassType* pClassType,
		CClassFieldMember* pMember,
		size_t BaseTypeOffset,
		rtl::CArrayT <size_t>* pLlvmBaseTypeIndexArray,
		CValue* pResultValue
		);

	bool
	ClassMethodMemberOperator (
		const CValue& OpValue,
		CClassType* pClassType,
		CClassMethodMember* pMember,
		size_t BaseTypeVTableIndex,
		CValue* pResultValue
		);

	bool
	ClassPropertyMemberOperator (
		const CValue& OpValue,
		CClassType* pClassType,
		CClassPropertyMember* pMember,
		size_t BaseTypeVTableIndex,
		CValue* pResultValue
		);

	bool
	MergeBitField (
		const CValue& SrcValue,
		const CValue& DstValue,
		CValue* pResultValue
		);

	bool
	MergeBitField (
		CValue* pValue,
		const CValue& DstValue
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {

