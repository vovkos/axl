// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnOp.h"
#include "axl_jnc_UnOp_Arithmetic.h"
#include "axl_jnc_UnOp_Ptr.h"
#include "axl_jnc_BinOp.h"
#include "axl_jnc_BinOp_Arithmetic.h"
#include "axl_jnc_BinOp_Assign.h"
#include "axl_jnc_BinOp_Cmp.h"
#include "axl_jnc_CastOp.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_EventType.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

class COperatorMgr
{
protected:
	friend class CModule;
	friend class CFunctionMgr;
	friend class CCast_fn;

	CModule* m_pModule;

	// unary operators

	CUnOp_Plus m_UnOp_Plus;
	CUnOp_Minus m_UnOp_Minus;
	CUnOp_BwNot m_UnOp_BwNot;
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
	CBinOp_BwAnd m_BinOp_BwAnd;
	CBinOp_BwOr m_BinOp_BwOr;
	CBinOp_BwXor m_BinOp_BwXor;

	// comparison operators

	CBinOp_Eq m_BinOp_Eq;
	CBinOp_Ne m_BinOp_Ne;
	CBinOp_Lt m_BinOp_Lt;
	CBinOp_Le m_BinOp_Le;
	CBinOp_Gt m_BinOp_Gt;
	CBinOp_Ge m_BinOp_Ge;

	// other binary operators

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
	CCast_fn m_Cast_fn;
	CCast_prop m_Cast_prop;
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

	bool
	CalcScopeLevelValue (
		CScope* pScope,
		CValue* pScopeLevelValue
		);

	CValue 
	CalcScopeLevelValue (CScope* pScope)
	{
		CValue ScopeLevelValue;
		CalcScopeLevelValue (pScope, &ScopeLevelValue);
		return ScopeLevelValue;
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
	
	bool
	PrepareArgumentReturnValue (CValue* pValue);

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
		)
	{
		return UnaryOperator (OpKind, *pValue, pValue);
	}

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
		)
	{
		return BinaryOperator (OpKind, *pValue, OpValue2, pValue);
	}

	// move & cast operators

	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		);

	ECast
	GetCastKind (
		const CValue& SrcValue,
		CType* pDstType
		);

	bool
	CheckCastKind (
		const CValue& SrcValue,
		CType* pDstType
		);

	bool
	CheckCastKind (
		CType* pSrcType,
		CType* pDstType
		);

	ECast
	GetArgCastKind (
		CFunctionType* pFunctionType,
		const rtl::CArrayT <CType*>& ArgTypeArray
		);

	ECast
	GetArgCastKind (
		CFunctionType* pFunctionType,
		const rtl::CBoxListT <CValue>* pArgList
		);

	ECast
	GetFunctionCastKind (
		CFunctionType* pSrcType,
		CFunctionType* pDstType
		);

	bool
	CastOperator (
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);

	bool
	CastOperator (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		)
	{
		return CastOperator (EAlloc_Undefined, OpValue, pType, pResultValue);
	}

	bool
	CastOperator (
		EAlloc AllocKind,
		CValue* pValue,
		CType* pType
		)
	{
		return CastOperator (AllocKind, *pValue, pType, pValue);
	}

	bool
	CastOperator (
		CValue* pValue,
		CType* pType
		)
	{
		return CastOperator (EAlloc_Undefined, *pValue, pType, pValue);
	}

	bool
	CastOperator (
		EAlloc AllocKind,
		const CValue& OpValue,
		EType TypeKind,
		CValue* pResultValue
		);

	bool
	CastOperator (
		const CValue& OpValue,
		EType TypeKind,
		CValue* pResultValue
		)
	{
		return CastOperator (EAlloc_Undefined, OpValue, TypeKind, pResultValue);
	}

	bool
	CastOperator (
		EAlloc AllocKind,
		CValue* pValue,
		EType TypeKind
		)
	{
		return CastOperator (AllocKind, *pValue, TypeKind, pValue);
	}

	bool
	CastOperator (
		CValue* pValue,
		EType TypeKind
		)
	{
		return CastOperator (EAlloc_Undefined, *pValue, TypeKind, pValue);
	}

	bool
	MoveOperator (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	bool
	BinOpMoveOperator (
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
	NewOperator (
		EAlloc AllocKind,
		CType* pType,
		CValue* pResultValue
		)
	{
		return AllocKind == EAlloc_Stack ? 
			StackNewOperator (pType, pResultValue) :
			HeapNewOperator (pType, pResultValue);
	}

	bool
	GetFieldMember (
		const CValue& ThisValue,
		CStructMember* pMember,
		CValue* pResultValue
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
		)
	{
		return MemberOperator (*pValue, pName, pValue);
	}

	bool
	WeakMemberOperator (
		const CValue& OpValue,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	WeakMemberOperator (
		CValue* pValue,
		const tchar_t* pName
		)
	{
		return WeakMemberOperator (*pValue, pName, pValue);
	}

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
		)
	{
		return CallOperator (*pValue, pArgList, pValue);
	}

	bool
	ClosureOperator (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	ClosureOperator (
		CValue* pValue,
		rtl::CBoxListT <CValue>* pArgList
		)
	{
		return ClosureOperator (*pValue,  pArgList, pValue);
	}

	bool
	GetPropertyGetter (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyGetter (CValue* pValue)
	{
		return GetPropertyGetter (*pValue, pValue);
	}

	bool
	GetPropertySetter (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertySetter (CValue* pValue)
	{
		return GetPropertySetter (*pValue, pValue);
	}

	bool
	GetPropertyPropValue (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyPropValue (CValue* pValue)
	{
		return GetPropertyPropValue (*pValue, pValue);
	}

	bool
	GetPropertyOnChangeEvent (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyOnChangeEvent (CValue* pValue)
	{
		return GetPropertyOnChangeEvent (*pValue, pValue);
	}


	bool
	EventOperator (
		const CValue& Event,
		const CValue& Handler,
		EEventOp OpKind
		);

	// load & store operators

	bool
	LoadReferenceOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	LoadReferenceOperator (CValue* pValue)
	{
		return LoadReferenceOperator (*pValue, pValue);
	}

	bool
	StoreReferenceOperator (
		const CValue& SrcValue,
		const CValue& DstValue,
		bool KeepProperty
		);

	bool
	GetPropertyOperator (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	SetPropertyOperator (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	bool
	GetVirtualMethodMember (
		CFunction* pFunction,
		CClosure* pClosure,
		CValue* pResultValue
		);

	bool
	GetVirtualPropertyMember (
		CProperty* pProperty,
		CClosure* pClosure,
		CValue* pResultValue
		);

	bool
	GetClassFieldMemberValue (
		const CValue& ObjValue,
		CStructMember* pMember,
		CValue* pValue
		);

	bool
	SetClassFieldMemberValue (
		const CValue& ObjValue,
		CStructMember* pMember,
		const CValue& Value
		);

	// funcion pointer to unsafe pfn + closure

	bool
	NormalizeFunctionPointer (
		const CValue& Value,
		CValue* pResultValue
		);

	bool
	NormalizeFunctionPointer (CValue* pValue)
	{
		return NormalizeFunctionPointer (*pValue, pValue);
	}

	bool
	ProcessDestructList (rtl::CBoxListT <CValue>* pList);
	
protected:
	enum EPrepareReferenceFlag
	{
		EPrepareReferenceFlag_Load         = 1,
		EPrepareReferenceFlag_Store        = 2,
		EPrepareReferenceFlag_NoRangeCheck = 4
	};

	bool
	PrepareReference (
		const CValue& Value,
		int Flags,
		CValue* pPtrValue
		);

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
	VTableIndexOperatorImpl ( 
		const CValue& ValuePtr,
		size_t Index,
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
		CStructMember* pMember,
		CClassBaseTypeCoord* pCoord,
		CValue* pResultValue
		);

	bool
	ClassMethodMemberOperator (
		const CValue& OpValue,
		CFunction* pFunction,
		CValue* pResultValue
		);

	bool
	ClassPropertyMemberOperator (
		const CValue& OpValue,
		CPropertyType* pPropertyType,
		CValue* pResultValue
		);

	bool
	GetClassVTable (
		const CValue& OpValue,
		CClassType* pClassType,
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

	bool
	CallImpl (
		const CValue& PfnValue,
		CFunctionType* pFunctionType,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	CallEvent (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	CallFunctionPtr (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	CastArgList (
		CFunctionType* pFunctionType,
		rtl::CBoxListT <CValue>* pArgList,
		rtl::CArrayT <CValue>* pArgArray
		);

	CType*
	GetVarArgType (
		CType* pType,
		bool IsUnsafeVarArg
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

