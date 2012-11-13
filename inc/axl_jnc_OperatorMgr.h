// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnaryOperator.h"
#include "axl_jnc_BinaryOperator.h"
#include "axl_jnc_CastOperator.h"

namespace axl {
namespace jnc {

class CArrayType;
class CStructType;
class CClassType;

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

	CUnOp_addr m_UnOp_addr;
	CUnOp_indir m_UnOp_indir;
	
	CUnOpT_inc <EUnOp_PreInc> m_UnOp_preinc;
	CUnOpT_inc <EUnOp_PreInc> m_UnOp_predec;
	CUnOpT_inc <EUnOp_PostInc> m_UnOp_postinc;
	CUnOpT_inc <EUnOp_PostInc> m_UnOp_postdec;

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
	CBinOpT_i32 <CBinOp_Div_u> m_BinOp_Div_i32u;
	CBinOpT_i64 <CBinOp_Div_u> m_BinOp_Div_i64u;

	CBinOpT_i32 <CBinOp_Mod> m_BinOp_Mod_i32;
	CBinOpT_i64 <CBinOp_Mod> m_BinOp_Mod_i64;
	CBinOpT_i32 <CBinOp_Mod_u> m_BinOp_Mod_i32u;
	CBinOpT_i64 <CBinOp_Mod_u> m_BinOp_Mod_i64u;

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

	// relational operators

	CBinOpT_cmp_i32 <CBinOp_Eq> m_BinOp_Eq_i32;
	CBinOpT_cmp_i64 <CBinOp_Eq> m_BinOp_Eq_i64;
	CBinOpT_cmp_f32 <CBinOp_Eq> m_BinOp_Eq_f32;
	CBinOpT_cmp_f64 <CBinOp_Eq> m_BinOp_Eq_f64;

	CBinOpT_cmp_i32 <CBinOp_Ne> m_BinOp_Ne_i32;
	CBinOpT_cmp_i64 <CBinOp_Ne> m_BinOp_Ne_i64;
	CBinOpT_cmp_f32 <CBinOp_Ne> m_BinOp_Ne_f32;
	CBinOpT_cmp_f64 <CBinOp_Ne> m_BinOp_Ne_f64;

	CBinOpT_cmp_i32 <CBinOp_Lt> m_BinOp_Lt_i32;
	CBinOpT_cmp_i64 <CBinOp_Lt> m_BinOp_Lt_i64;
	CBinOpT_cmp_f32 <CBinOp_Lt> m_BinOp_Lt_f32;
	CBinOpT_cmp_f64 <CBinOp_Lt> m_BinOp_Lt_f64;
	CBinOpT_cmp_i32 <CBinOp_Lt_u> m_BinOp_Lt_i32u;
	CBinOpT_cmp_i64 <CBinOp_Lt_u> m_BinOp_Lt_i64u;

	CBinOpT_cmp_i32 <CBinOp_Le> m_BinOp_Le_i32;
	CBinOpT_cmp_i64 <CBinOp_Le> m_BinOp_Le_i64;
	CBinOpT_cmp_f32 <CBinOp_Le> m_BinOp_Le_f32;
	CBinOpT_cmp_f64 <CBinOp_Le> m_BinOp_Le_f64;
	CBinOpT_cmp_i32 <CBinOp_Le_u> m_BinOp_Le_i32u;
	CBinOpT_cmp_i64 <CBinOp_Le_u> m_BinOp_Le_i64u;

	CBinOpT_cmp_i32 <CBinOp_Gt> m_BinOp_Gt_i32;
	CBinOpT_cmp_i64 <CBinOp_Gt> m_BinOp_Gt_i64;
	CBinOpT_cmp_f32 <CBinOp_Gt> m_BinOp_Gt_f32;
	CBinOpT_cmp_f64 <CBinOp_Gt> m_BinOp_Gt_f64;
	CBinOpT_cmp_i32 <CBinOp_Gt_u> m_BinOp_Gt_i32u;
	CBinOpT_cmp_i64 <CBinOp_Gt_u> m_BinOp_Gt_i64u;

	CBinOpT_cmp_i32 <CBinOp_Ge> m_BinOp_Ge_i32;
	CBinOpT_cmp_i64 <CBinOp_Ge> m_BinOp_Ge_i64;
	CBinOpT_cmp_f32 <CBinOp_Ge> m_BinOp_Ge_f32;
	CBinOpT_cmp_f64 <CBinOp_Ge> m_BinOp_Ge_f64;
	CBinOpT_cmp_i32 <CBinOp_Ge_u> m_BinOp_Ge_i32u;
	CBinOpT_cmp_i64 <CBinOp_Ge_u> m_BinOp_Ge_i64u;

	// cast operators

	CCast_cpy m_Cast_cpy;

	CCast_int_trunc m_Cast_int_trunc;
	CCast_int_ext m_Cast_int_ext;
	CCast_int_ext_u m_Cast_int_ext_u;
	CCast_int_swp m_Cast_int_swp;

	CCast_f32_f64 m_Cast_f32_f64;
	CCast_f64_f32 m_Cast_f64_f32;

	CCast_num_bool m_Cast_num_bool;
	CCast_bool_int m_Cast_bool_int;

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

	// tables

	CUnaryOperatorOverload m_UnaryOperatorTable [EUnOp__Count];
	CBinaryOperatorOverload m_BinaryOperatorTable [EBinOp__Count];		
	ICastOperator* m_BasicCastOperatorTable [EType__BasicTypeCount] [EType__BasicTypeCount];

	rtl::CStringHashTableMapT <ICastOperator*> m_CastOperatorMap;
	rtl::CBoxListT <rtl::CString> m_CastSignatureCache;
	rtl::CStdListT <CSuperCast> m_SuperCastList;
	
public:
	COperatorMgr ();

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

	IUnaryOperator*
	AddUnaryOperator (
		EUnOp OpKind,
		CType* pOpType,
		IUnaryOperator* pOperator
		)
	{
		ASSERT (OpKind > 0 && OpKind < EUnOp__Count);
		return m_UnaryOperatorTable [OpKind].AddOperator (pOpType, pOperator);
	}

	IUnaryOperator*
	AddUnaryOperator (
		EUnOp OpKind,
		EType OpTypeKind,
		IUnaryOperator* pOperator
		);

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

	IBinaryOperator*
	AddBinaryOperator (
		EBinOp OpKind,
		CType* pOpType1,
		CType* pOpType2,
		IBinaryOperator* pOperator
		)
	{
		ASSERT (OpKind > 0 && OpKind < EBinOp__Count);
		return m_BinaryOperatorTable [OpKind].AddOperator (pOpType1, pOpType2, pOperator);
	}

	IBinaryOperator*
	AddBinaryOperator (
		EBinOp OpKind,
		EType OpTypeKind1,
		EType OpTypeKind2,
		IBinaryOperator* pOperator
		);

	IBinaryOperator*
	AddBinaryOperator (
		EBinOp OpKind,
		CType* pType,
		IBinaryOperator* pOperator
		)
	{
		return AddBinaryOperator (OpKind, pType, pType, pOperator);
	}

	IBinaryOperator*
	AddBinaryOperator (
		EBinOp OpKind,
		EType TypeKind,
		IBinaryOperator* pOperator
		)
	{
		return AddBinaryOperator (OpKind, TypeKind, TypeKind, pOperator);
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

	// move & cast operators

	ICastOperator*
	AddCastOperator (
		CType* pSrcType,
		CType* pDstType,
		ICastOperator* pOperator
		);

	ICastOperator*
	AddCastOperator (
		EType SrcTypeKind,
		EType DstTypeKind,
		ICastOperator* pOperator
		);

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

protected:
	void
	AddStdUnaryOperators ();

	void
	AddStdBinaryOperators ();

	void
	AddStdCastOperators ();

	IUnaryOperator*
	GetUnaryOperator (
		EUnOp OpKind,
		CType* pOpType,
		TUnaryOperatorTypeInfo* pTypeInfo
		);

	IBinaryOperator*
	GetBinaryOperator (
		EBinOp OpKind,
		CType* pOpType1,
		CType* pOpType2,
		TBinaryOperatorTypeInfo* pTypeInfo
		)
	{
		ASSERT (OpKind > 0 && OpKind < EBinOp__Count);
		return m_BinaryOperatorTable [OpKind].GetOperator (pOpType1, pOpType2, pTypeInfo);
	}

	ICastOperator*
	GetCastOperator (
		CType* pSrcType,
		CType* pDstType
		);

	// prepare is: unqualify, get property, peel double references

	CType* 
	PrepareOperandType (CType* pOpType);

	bool 
	PrepareOperand (
		const CValue& OpValue,
		CValue* pOpValue
		);

	bool 
	PrepareOperand (CValue* pOpValue);

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

	// special cast operators

	bool
	CastPointerOperator (
		const CValue& OpValue,
		CPointerType* pSrcType,
		CPointerType* pDstType,
		CValue* pResultValue
		);

	bool
	CastArrayReferenceToPointerOperator (
		const CValue& OpValue,
		CArrayType* pArrayType,
		CPointerType* pPointerType,
		CValue* pResultValue
		);

	bool
	CastArrayToPointerOperator (
		const CValue& OpValue,
		CArrayType* pArrayType,
		CPointerType* pPointerType,
		CValue* pResultValue
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
	ClassMemberOperator (
		const CValue& OpValue,
		CClassType* pClassType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	// llvm helpers

	llvm::Value*
	CreateLlvmGep (
		llvm::Value* pLlvmPtr,
		llvm::Value* pLlvmIndex0,
		llvm::Value* pLlvmIndex1
		);

	llvm::Value*
	CreateLlvmGep (
		llvm::Value* pLlvmPtr,
		intptr_t Index0,
		intptr_t Index1
		);

	llvm::Value*
	CreateLlvmSafePointer (
		llvm::Value* pLlvmPtr,
		llvm::Value* pLlvmParentPtr,
		CType* pParentType
		);

	llvm::Value*
	CreateLlvmDynamicPointer (
		llvm::Value* pLlvmPtr,
		CType* pType
		);

	llvm::Value*
	ModifyLlvmSafePointer (
		llvm::Value* pLlvmSafePtr,
		llvm::Value* pLlvmPtr
		);

	void
	CheckLlvmSafePointer (
		llvm::Value* pLlvmSafePtr,
		size_t Size,
		int Access
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {

