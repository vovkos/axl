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

	CUnOp_Minus m_UnOp_Minus;
	CUnOp_BitwiseNot m_UnOp_BitwiseNot;
	CUnOp_Addr m_UnOp_Addr;
	CUnOp_Indir m_UnOp_Indir;
	
	CUnOpT_PreInc <EUnOp_PreInc> m_UnOp_PreInc;
	CUnOpT_PreInc <EUnOp_PreDec> m_UnOp_PreDec;
	CUnOpT_PostInc <EUnOp_PostInc> m_UnOp_PostInc;
	CUnOpT_PostInc <EUnOp_PostDec> m_UnOp_PostDec;

	// binary operators

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

	IUnaryOperator* m_UnaryOperatorTable [EUnOp__Count];
	IBinaryOperator* m_BinaryOperatorTable [EBinOp__Count];		
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

	// public llvm helpers

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
	CreateLlvmSafePtr (
		llvm::Value* pLlvmPtr,
		llvm::Value* pLlvmParentPtr,
		CType* pParentType,
		size_t ScopeLevel
		);

	llvm::Value*
	CreateLlvmDynamicPtr (
		llvm::Value* pLlvmPtr,
		CType* pType,
		size_t ScopeLevel
		);

	llvm::Value*
	ModifyLlvmSafePtr (
		llvm::Value* pLlvmSafePtr,
		llvm::Value* pLlvmPtr
		);

	void
	CheckLlvmSafePtrRange (
		llvm::Value* pLlvmSafePtr,
		size_t Size,
		ESafePtrError Error
		);

	void
	CheckLlvmSafePtrScope (
		llvm::Value* pLlvmSafePtr,
		size_t ScopeLevel
		);

	bool
	CheckLlvmSafePtrScope (
		llvm::Value* pLlvmSafePtr,
		const CValue& SrcValue,
		size_t DstScopeLevel
		);

	bool
	CheckLlvmSafePtrScope (
		llvm::Value* pLlvmSafePtr,
		const CValue& SrcValue,
		const CValue& DstValue
		);

protected:
	void
	AddStdCastOperators ();

	ICastOperator*
	GetCastOperator (
		CType* pSrcType,
		CType* pDstType
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
};

//.............................................................................

} // namespace axl {
} // namespace jnc {

