// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_UnOp.h"
#include "axl_jnc_UnOp_Arithmetic.h"
#include "axl_jnc_UnOp_LogNot.h"
#include "axl_jnc_UnOp_Ptr.h"
#include "axl_jnc_UnOp_Inc.h"
#include "axl_jnc_BinOp.h"
#include "axl_jnc_BinOp_Arithmetic.h"
#include "axl_jnc_BinOp_Logic.h"
#include "axl_jnc_BinOp_Cmp.h"
#include "axl_jnc_BinOp_At.h"
#include "axl_jnc_BinOp_Idx.h"
#include "axl_jnc_BinOp_Assign.h"
#include "axl_jnc_CastOp.h"
#include "axl_jnc_CastOp_Bool.h"
#include "axl_jnc_CastOp_Int.h"
#include "axl_jnc_CastOp_Fp.h"
#include "axl_jnc_CastOp_Array.h"
#include "axl_jnc_CastOp_DataPtr.h"
#include "axl_jnc_CastOp_ClassPtr.h"
#include "axl_jnc_CastOp_FunctionPtr.h"
#include "axl_jnc_CastOp_PropertyPtr.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_MulticastType.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

enum EStdCast
{
	EStdCast_Copy,
	EStdCast_SwapByteOrder,
	EStdCast_PtrFromInt,
	EStdCast_Int,
	EStdCast_Fp,
	EStdCast__Count
};

//.............................................................................

class COperatorMgr
{
protected:
	friend class CModule;
	friend class CFunctionMgr;
	friend class CCast_FunctionPtr;

	CModule* m_pModule;

	// unary arithmetics

	CUnOp_Plus m_UnOp_Plus;
	CUnOp_Minus m_UnOp_Minus;
	CUnOp_BwNot m_UnOp_BwNot;
	CUnOp_LogNot m_UnOp_LogNot;	
	
	// pointer operators

	CUnOp_Addr m_UnOp_Addr;
	CUnOp_Indir m_UnOp_Indir;	

	// increment operators

	CUnOp_PreInc m_UnOp_PreInc;
	CUnOp_PreInc m_UnOp_PreDec;
	CUnOp_PostInc m_UnOp_PostInc;
	CUnOp_PostInc m_UnOp_PostDec;

	// binary arithmetics

	CBinOp_Add m_BinOp_Add;
	CBinOp_Sub m_BinOp_Sub;
	CBinOp_Mul m_BinOp_Mul;
	CBinOp_Div m_BinOp_Div;
	CBinOp_Mod m_BinOp_Mod;
	CBinOp_Shl m_BinOp_Shl;
	CBinOp_Shr m_BinOp_Shr;
	CBinOp_BwAnd m_BinOp_BwAnd;
	CBinOp_BwXor m_BinOp_BwXor;
	CBinOp_BwOr m_BinOp_BwOr;

	// special operators

	CBinOp_At m_BinOp_At;
	CBinOp_Idx m_BinOp_Idx;

	// binary logic operators

	CBinOp_LogAnd m_BinOp_LogAnd;
	CBinOp_LogOr m_BinOp_LogOr;

	// comparison operators

	CBinOp_Eq m_BinOp_Eq;
	CBinOp_Ne m_BinOp_Ne;
	CBinOp_Lt m_BinOp_Lt;
	CBinOp_Le m_BinOp_Le;
	CBinOp_Gt m_BinOp_Gt;
	CBinOp_Ge m_BinOp_Ge;

	// assignment operators

	CBinOp_Assign m_BinOp_Assign;
	CBinOp_OpAssign m_BinOp_AddAssign;
	CBinOp_OpAssign m_BinOp_SubAssign;
	CBinOp_OpAssign m_BinOp_MulAssign;
	CBinOp_OpAssign m_BinOp_DivAssign;
	CBinOp_OpAssign m_BinOp_ModAssign;
	CBinOp_OpAssign m_BinOp_ShlAssign;
	CBinOp_OpAssign m_BinOp_ShrAssign;
	CBinOp_OpAssign m_BinOp_AndAssign;
	CBinOp_OpAssign m_BinOp_XorAssign;
	CBinOp_OpAssign m_BinOp_OrAssign;
	CBinOp_OpAssign m_BinOp_AtAssign;

	// cast operators

	CCast_Copy m_Cast_Copy;
	CCast_SwapByteOrder m_Cast_SwapByteOrder;
	CCast_PtrFromInt m_Cast_PtrFromInt;
	CCast_Bool m_Cast_Bool;
	CCast_Int m_Cast_Int;
	CCast_BeInt m_Cast_BeInt;
	CCast_Fp m_Cast_Fp;
	CCast_Array m_Cast_Array;
	CCast_Enum m_Cast_Enum;
	CCast_DataPtr m_Cast_DataPtr;
	CCast_ClassPtr m_Cast_ClassPtr;
	CCast_FunctionPtr m_Cast_FunctionPtr;
	CCast_PropertyPtr m_Cast_PropertyPtr;

	// tables

	IUnaryOperator* m_UnaryOperatorTable [EUnOp__Count];
	IBinaryOperator* m_BinaryOperatorTable [EBinOp__Count];		
	ICastOperator* m_CastOperatorTable [EType__Count];
	ICastOperator* m_StdCastOperatorTable [EStdCast__Count];
	
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

	// load reference, get property, enum->int, bool->int, array->ptr -- unless specified otherwise with Flags

	void 
	PrepareOperandType (
		const CValue& OpValue,
		CValue* pOpValue,
		int OpFlags = 0
		);

	void 
	PrepareOperandType (
		CValue* pOpValue,
		int OpFlags = 0
		)
	{
		PrepareOperandType (*pOpValue, pOpValue, OpFlags);
	}

	bool 
	PrepareOperand (
		const CValue& OpValue,
		CValue* pOpValue,
		int OpFlags = 0
		);

	bool 
	PrepareOperand (
		CValue* pOpValue,
		int OpFlags = 0
		)
	{
		return PrepareOperand (*pOpValue, pOpValue, OpFlags);
	}
	
	bool
	PrepareArgumentReturnValue (CValue* pValue);

	// unary operators

	bool
	UnaryOperator (
		EUnOp OpKind,
		const CValue& OpValue,
		CValue* pResultValue = NULL
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
		CValue* pResultValue = NULL
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

	// cast operators

	ICastOperator*
	GetStdCastOperator (EStdCast CastKind)
	{
		ASSERT (CastKind >= 0 && CastKind < EStdCast__Count);
		return m_StdCastOperatorTable [CastKind];
	}

	ECast
	GetCastKind (
		const CValue& OpValue,
		CType* pType
		);

	bool
	CheckCastKind (
		const CValue& OpValue,
		CType* pType
		);

	ECast
	GetArgCastKind (
		CFunctionType* pFunctionType,
		const rtl::CArrayT <CType*>& ArgTypeArray
		)
	{
		return GetArgCastKind (pFunctionType, ArgTypeArray, ArgTypeArray.GetCount ());
	}

	ECast
	GetArgCastKind (
		CFunctionType* pFunctionType,
		CType* const* ppArgTypeArray,
		size_t Count
		);

	ECast
	GetArgCastKind (
		CFunctionType* pFunctionType,
		const rtl::CConstBoxListT <CValue>& ArgList
		);

	ECast
	GetFunctionCastKind (
		CFunctionType* pSrcType,
		CFunctionType* pDstType
		);

	ECast
	GetPropertyCastKind (
		CPropertyType* pSrcType,
		CPropertyType* pDstType
		);

	bool
	CastOperator (
		EAlloc AllocKind,
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue = NULL
		);

	bool
	CastOperator (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue = NULL
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
		CValue* pResultValue = NULL
		);

	bool
	CastOperator (
		const CValue& OpValue,
		EType TypeKind,
		CValue* pResultValue = NULL
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
		CStructField* pMember,
		CBaseTypeCoord* pCoord,
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
	CallOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		return CallOperator (OpValue, &ArgList, pResultValue);
	}

	bool
	CallOperator (
		const CValue& OpValue,
		const CValue& ArgValue,
		CValue* pResultValue
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (ArgValue);
		return CallOperator (OpValue, &ArgList, pResultValue);
	}

	bool
	CallOperator2 (
		const CValue& OpValue,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		CValue* pResultValue
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (ArgValue1);
		ArgList.InsertTail (ArgValue2);
		return CallOperator (OpValue, &ArgList, pResultValue);
	}

	bool
	CallOperator3 (
		const CValue& OpValue,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		const CValue& ArgValue3,
		CValue* pResultValue
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (ArgValue1);
		ArgList.InsertTail (ArgValue2);
		ArgList.InsertTail (ArgValue3);
		return CallOperator (OpValue, &ArgList, pResultValue);
	}

	bool
	CallOperator4 (
		const CValue& OpValue,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		const CValue& ArgValue3,
		const CValue& ArgValue4,
		CValue* pResultValue
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (ArgValue1);
		ArgList.InsertTail (ArgValue2);
		ArgList.InsertTail (ArgValue3);
		ArgList.InsertTail (ArgValue4);
		return CallOperator (OpValue, &ArgList, pResultValue);
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

	void
	GetThinDataPtrScopeLevel (
		const CValue& Value,
		CValue* pResultValue
		);

	void
	GetThinDataPtrValidator (
		const CValue& Value,
		CValue* pResultValue
		);

	bool
	GetPropertyThinPtr (
		CProperty* pProperty,
		CClosure* pClosure,
		CPropertyPtrType* pPtrType,
		CValue* pResultValue
		);

	bool
	GetPropertyThinPtr (
		CProperty* pProperty,
		CClosure* pClosure,
		CValue* pResultValue
		)
	{
		return GetPropertyThinPtr (
			pProperty, 
			pClosure, 
			pProperty->GetType ()->GetPropertyPtrType (EPropertyPtrType_Thin), 
			pResultValue
			);
	}
	
	bool
	GetPropertyVTable (
		CProperty* pProperty,
		CClosure* pClosure,
		CValue* pResultValue
		);

	bool
	GetProperty (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	SetProperty (
		const CValue& OpValue,
		const CValue& SrcValue
		);

	bool
	GetPropertyVTable (
		const CValue& OpValue,
		CValue* pResultValue
		);

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
		const CValue& ArgValue,
		CValue* pResultValue
		);

	bool
	GetPropertySetter (
		CValue* pValue,
		const CValue& ArgValue
		)
	{
		return GetPropertySetter (*pValue, ArgValue, pValue);
	}

	bool
	GetPropertySetter (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		return GetPropertySetter (OpValue, CValue (), pResultValue);
	}

	bool
	GetPropertySetter (CValue* pValue)
	{
		return GetPropertySetter (*pValue, CValue (), pValue);
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

	// load & store operators

	bool
	LoadDataRef (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	LoadDataRef (CValue* pValue)
	{
		return LoadDataRef (*pValue, pValue);
	}

	bool
	StoreDataRef (
		const CValue& OpValue1,
		const CValue& OpValue2
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
		CStructField* pMember,
		CValue* pValue
		);

	bool
	SetClassFieldMemberValue (
		const CValue& ObjValue,
		CStructField* pMember,
		const CValue& Value
		);

	bool
	ProcessDestructList (rtl::CBoxListT <CValue>* pList);
	
	bool
	CreateClosureObject (
		EAlloc AllocKind,
		const CValue& OpValue, // function or property ptr
		rtl::CArrayT <size_t>* pClosureMap,
		CValue* pResultValue
		);

protected:
	// checks

	bool
	PrepareDataRef (
		const CValue& Value,
		ERuntimeError Error,
		CValue* pPtrValue
		);

	void
	GetDataRefScopeLevel (
		const CValue& Value,
		CValue* pScopeLevelValue
		);

	void
	CheckDataPtrRange (
		const CValue& PtrValue,
		size_t Size,
		const CValue& ValidatorValue,
		ERuntimeError Error
		);

	bool 
	CheckDataPtrScopeLevel (
		const CValue& SrcValue,
		const CValue& DstValue
		); // can sometimes detect invalid assigns at compile time

	void
	CheckClassPtrNull (const CValue& Value);

	void
	CheckClassPtrScopeLevel (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	void
	CheckFunctionPtrNull (const CValue& Value);

	void
	CheckFunctionPtrScopeLevel (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	void
	CheckPropertyPtrNull (const CValue& Value);

	void
	CheckPropertyPtrScopeLevel (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	bool
	ExtractBitField (
		const CValue& Value,
		CBitFieldType* pBitFieldType,
		CValue* pResultValue
		);
	
	bool
	MergeBitField (
		const CValue& Value,
		const CValue& ShadowValue,
		CBitFieldType* pBitFieldType,
		CValue* pResultValue
		);

	// member operators

	bool
	GetStructMember (
		const CValue& OpValue,
		CStructType* pStructType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	GetStructFieldMember  (
		const CValue& OpValue,
		CStructField* pMember,
		CBaseTypeCoord* pCoord,
		CValue* pResultValue
		);

	bool
	GetUnionMember (
		const CValue& OpValue,
		CUnionType* pUnionType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	GetUnionFieldMember (
		const CValue& OpValue,
		CStructField* pMember,
		CValue* pResultValue
		);

	bool
	GetClassMember (
		const CValue& OpValue,
		CClassType* pClassType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	GetClassFieldMember (
		const CValue& OpValue,
		CStructField* pMember,
		CBaseTypeCoord* pCoord,
		CValue* pResultValue
		);

	bool
	GetClassMethodMember (
		const CValue& OpValue,
		CFunction* pFunction,
		CValue* pResultValue
		);

	bool
	GetClassPropertyMember (
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
	CallImpl (
		const CValue& PfnValue,
		CFunctionType* pFunctionType,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	CallMulticast (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	CallClosureFunctionPtr (
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

