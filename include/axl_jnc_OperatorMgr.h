// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
#include "axl_jnc_CastOp_Struct.h"
#include "axl_jnc_CastOp_DataPtr.h"
#include "axl_jnc_CastOp_ClassPtr.h"
#include "axl_jnc_CastOp_FunctionPtr.h"
#include "axl_jnc_CastOp_PropertyPtr.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_VariableMgr.h"

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
	friend class CModule;
	friend class CVariableMgr;
	friend class CFunctionMgr;
	friend class CParser;
	friend class CCast_FunctionPtr;

protected:
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
	CBinOp_RefAssign m_BinOp_RefAssign;
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

	CCast_Default m_Cast_Default;
	CCast_Copy m_Cast_Copy;
	CCast_SwapByteOrder m_Cast_SwapByteOrder;
	CCast_PtrFromInt m_Cast_PtrFromInt;
	CCast_Bool m_Cast_Bool;
	CCast_Int m_Cast_Int;
	CCast_BeInt m_Cast_BeInt;
	CCast_Fp m_Cast_Fp;
	CCast_Array m_Cast_Array;
	CCast_Enum m_Cast_Enum;
	CCast_Struct m_Cast_Struct;
	CCast_DataPtr m_Cast_DataPtr;
	CCast_DataRef m_Cast_DataRef;
	CCast_ClassPtr m_Cast_ClassPtr;
	CCast_FunctionPtr m_Cast_FunctionPtr;
	CCast_FunctionRef m_Cast_FunctionRef;
	CCast_PropertyPtr m_Cast_PropertyPtr;
	CCast_PropertyRef m_Cast_PropertyRef;

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

	void
	MarkGcRoot (
		const CValue& PtrValue,
		CType* pType
		);

	// load reference, get property, enum->int, bool->int, array->ptr -- unless specified otherwise with Flags

	void 
	PrepareOperandType (
		const CValue& OpValue,
		CValue* pOpValue,
		uint_t OpFlags = 0
		);

	void 
	PrepareOperandType (
		CValue* pOpValue,
		uint_t OpFlags = 0
		)
	{
		PrepareOperandType (*pOpValue, pOpValue, OpFlags);
	}

	CType*
	PrepareOperandType (
		const CValue& OpValue,
		uint_t OpFlags = 0
		);

	bool 
	PrepareOperand (
		const CValue& OpValue,
		CValue* pOpValue,
		uint_t OpFlags = 0
		);

	bool 
	PrepareOperand (
		CValue* pOpValue,
		uint_t OpFlags = 0
		)
	{
		return PrepareOperand (*pOpValue, pOpValue, OpFlags);
	}
	
	bool
	PrepareArgumentReturnValue (CValue* pValue);

	bool
	PrepareDataPtr (
		const CValue& Value,
		CValue* pResultValue
		);

	bool
	PrepareDataPtr (CValue* pValue)
	{
		return PrepareDataPtr (*pValue, pValue);
	}

	// unary operators

	CType*
	GetUnaryOperatorResultType (
		EUnOp OpKind,
		const CValue& OpValue
		);

	bool
	GetUnaryOperatorResultType (
		EUnOp OpKind,
		const CValue& OpValue,
		CValue* pResultValue
		);
	
	bool
	GetUnaryOperatorResultType (
		EUnOp OpKind,
		CValue* pValue
		)
	{
		return GetUnaryOperatorResultType (OpKind, *pValue, pValue);
	}

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

	CType*
	GetBinaryOperatorResultType (
		EBinOp OpKind,
		const CValue& OpValue1,
		const CValue& OpValue2
		);

	bool
	GetBinaryOperatorResultType (
		EBinOp OpKind,
		const CValue& OpValue1,
		const CValue& OpValue2,
		CValue* pResultValue
		);

	bool
	GetBinaryOperatorResultType (
		EBinOp OpKind,
		CValue* pValue,
		const CValue& OpValue2
		)
	{
		return GetBinaryOperatorResultType (OpKind, *pValue, OpValue2, pValue);
	}

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

	// conditional operator

	CType*
	GetConditionalOperatorResultType (
		const CValue& TrueValue,
		const CValue& FalseValue
		);

	bool 
	GetConditionalOperatorResultType (
		const CValue& TrueValue,
		const CValue& FalseValue,
		CValue* pResultValue
		);

	bool 
	ConditionalOperator (
		const CValue& TrueValue,
		const CValue& FalseValue,
		CBasicBlock* pThenBlock,
		CBasicBlock* pPhiBlock,
		CValue* pResultValue = NULL
		);

	// cast operators

	ICastOperator*
	GetStdCastOperator (EStdCast CastKind)
	{
		ASSERT ((size_t) CastKind < EStdCast__Count);
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
		CFunctionArg* const* pArgArray,
		size_t ArgCount
		);

	ECast
	GetArgCastKind (
		CFunctionType* pFunctionType,
		const rtl::CArrayT <CFunctionArg*>& ArgArray
		)
	{
		return GetArgCastKind (pFunctionType, ArgArray, ArgArray.GetCount ());
	}

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
		EStorage StorageKind,
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
		return CastOperator (EStorage_Heap, OpValue, pType, pResultValue);
	}
	
	bool
	CastOperator (
		EStorage StorageKind,
		CValue* pValue,
		CType* pType
		)
	{
		return CastOperator (StorageKind, *pValue, pType, pValue);
	}

	bool
	CastOperator (
		CValue* pValue,
		CType* pType
		)
	{
		return CastOperator (EStorage_Heap, *pValue, pType, pValue);
	}

	bool
	CastOperator (
		EStorage StorageKind,
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
		return CastOperator (EStorage_Heap, OpValue, TypeKind, pResultValue);
	}
		
	bool
	CastOperator (
		EStorage StorageKind,
		CValue* pValue,
		EType TypeKind
		)
	{
		return CastOperator (StorageKind, *pValue, TypeKind, pValue);
	}

	bool
	CastOperator (
		CValue* pValue,
		EType TypeKind
		)
	{
		return CastOperator (EStorage_Heap, *pValue, TypeKind, pValue);
	}

	// new & delete operators

	bool
	Allocate (
		EStorage StorageKind,
		CType* pType,
		const char* pTag,
		CValue* pResultValue
		);

	bool
	Prime (
		EStorage StorageKind,
		const CValue& PtrValue,
		CType* pType,
		CValue* pResultValue
		);

	bool
	Construct (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList = NULL
		);

	bool
	ParseInitializer (
		const CValue& Value,
		const rtl::CConstBoxListT <CToken>& ConstructorTokenList,
		const rtl::CConstBoxListT <CToken>& InitializerTokenList
		);

	bool
	ParseConstExpression (
		const rtl::CConstBoxListT <CToken>& ExpressionTokenList,
		CValue* pResultValue
		);

	bool
	ParseConstIntegerExpression (
		const rtl::CConstBoxListT <CToken>& ExpressionTokenList,
		intptr_t* pInteger
		);

	CType* 
	GetNewOperatorResultType (CType* pType)
	{
		return pType->GetTypeKind () == EType_Class ? 
			(CType*) ((CClassType*) pType)->GetClassPtrType () : 
			pType->GetDataPtrType ();
	}

	bool
	NewOperator (
		EStorage StorageKind,
		CType* pType,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	DeleteOperator (const CValue& OpValue);

	bool
	EvaluateAlias (
		const rtl::CConstBoxListT <CToken> TokenList,
		CValue* pResultValue
		);

	// member operators

	CType*
	GetMemberOperatorResultType (
		const CValue& OpValue,
		size_t Index
		);

	bool
	GetMemberOperatorResultType (
		const CValue& OpValue,
		size_t Index,
		CValue* pResultValue
		);

	bool
	GetMemberOperatorResultType (
		CValue* pValue,
		size_t Index
		)
	{
		return GetMemberOperatorResultType (*pValue, Index, pValue);
	}

	bool
	MemberOperator (
		const CValue& OpValue,
		size_t Index,
		CValue* pResultValue
		);

	bool
	MemberOperator (
		CValue* pValue,
		size_t Index
		)
	{
		return MemberOperator (*pValue, Index, pValue);
	}

	CType*
	GetMemberOperatorResultType (
		const CValue& OpValue,
		const char* pName
		);

	bool
	GetMemberOperatorResultType (
		const CValue& OpValue,
		const char* pName,
		CValue* pResultValue
		);

	bool
	GetMemberOperatorResultType (
		CValue* pValue,
		const char* pName
		)
	{
		return GetMemberOperatorResultType (*pValue, pName, pValue);
	}

	bool
	MemberOperator (
		const CValue& OpValue,
		const char* pName,
		CValue* pResultValue
		);

	bool
	MemberOperator (
		CValue* pValue,
		const char* pName
		)
	{
		return MemberOperator (*pValue, pName, pValue);
	}

	// call operators

	void
	CallTraceFunction (
		const char* pFunctionName,
		const char* pString
		);

	CType*
	GetCallOperatorResultType (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	GetCallOperatorResultType (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	GetCallOperatorResultType (
		CValue* pValue,
		rtl::CBoxListT <CValue>* pArgList
		)
	{
		return GetCallOperatorResultType (*pValue, pArgList, pValue);
	}

	bool
	CallOperator (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue = NULL
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
		CValue* pResultValue = NULL
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		return CallOperator (OpValue, &ArgList, pResultValue);
	}

	bool
	CallOperator (
		const CValue& OpValue,
		const CValue& ArgValue,
		CValue* pResultValue = NULL
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
		CValue* pResultValue = NULL
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (ArgValue1);
		ArgList.InsertTail (ArgValue2);
		return CallOperator (OpValue, &ArgList, pResultValue);
	}

	void
	GcSafePoint ();

	// closure operators

	CType*
	GetClosureOperatorResultType (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	GetClosureOperatorResultType (
		const CValue& OpValue,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	GetClosureOperatorResultType (
		CValue* pValue,
		rtl::CBoxListT <CValue>* pArgList
		)
	{
		return GetClosureOperatorResultType (*pValue,  pArgList, pValue);
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
	ClosureOperator (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		return ClosureOperator (OpValue, &ArgList, pResultValue);
	}

	bool
	ClosureOperator (
		const CValue& OpValue,
		const CValue& ArgValue,
		CValue* pResultValue
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (ArgValue);
		return ClosureOperator (OpValue, &ArgList, pResultValue);
	}

	bool
	ClosureOperator2 (
		const CValue& OpValue,
		const CValue& ArgValue1,
		const CValue& ArgValue2,
		CValue* pResultValue
		)
	{
		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (ArgValue1);
		ArgList.InsertTail (ArgValue2);
		return ClosureOperator (OpValue, &ArgList, pResultValue);
	}

	CType*
	GetFunctionType (
		const CValue& OpValue,
		CFunctionType* pFunctionType
		);

	// property getter

	CType*
	GetPropertyGetterType (const CValue& OpValue);

	bool
	GetPropertyGetterType (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyGetterType (CValue* pValue)
	{
		return GetPropertyGetterType (*pValue, pValue);
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

	// property setter

	CType*
	GetPropertySetterType (
		const CValue& OpValue,
		const CValue& ArgValue
		);

	bool
	GetPropertySetterType (
		const CValue& OpValue,
		const CValue& ArgValue,
		CValue* pResultValue
		);

	bool
	GetPropertySetterType (
		CValue* pValue,
		const CValue& ArgValue
		)
	{
		return GetPropertySetterType (*pValue, ArgValue, pValue);
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

	CType*
	GetPropertySetterType (const CValue& OpValue)
	{
		return GetPropertySetterType (OpValue, CValue ());
	}

	bool
	GetPropertySetterType (
		const CValue& OpValue,
		CValue* pResultValue
		)
	{
		return GetPropertySetterType (OpValue, CValue (), pResultValue);
	}

	bool
	GetPropertySetterType (CValue* pValue)
	{
		return GetPropertySetterType (*pValue, CValue (), pValue);
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

	// property binder

	CType*
	GetPropertyBinderType (const CValue& OpValue);

	bool
	GetPropertyBinderType (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyBinderType (CValue* pValue)
	{
		return GetPropertyBinderType (*pValue, pValue);
	}

	bool
	GetPropertyBinder (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyBinder (CValue* pValue)
	{
		return GetPropertyBinder (*pValue, pValue);
	}

	// propvalue & onchange

	CType*
	GetPropertyPropValueType (const CValue& OpValue);

	bool
	GetPropertyPropValueType (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyPropValueType (CValue* pValue)
	{
		return GetPropertyPropValueType (*pValue, pValue);
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

	CType*
	GetPropertyOnChangeType (const CValue& OpValue);

	bool
	GetPropertyOnChangeType (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyOnChangeType (CValue* pValue)
	{
		return GetPropertyOnChangeType (*pValue, pValue);
	}

	bool
	GetPropertyOnChange (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetPropertyOnChange (CValue* pValue)
	{
		return GetPropertyOnChange (*pValue, pValue);
	}

	// misc property functions

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
	GetPropertyVTable (
		const CValue& OpValue,
		CValue* pResultValue
		);

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
		const CValue& DstValue,
		const CValue& SrcValue
		);

	// weakening

	CType*
	GetWeakenedType (const CValue& OpValue);

	bool
	GetWeakenedType (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	GetWeakenedType (CValue* pValue)
	{
		return GetWeakenedType (*pValue, pValue);
	}

	bool
	Weaken (
		const CValue& OpValue,
		CValue* pResultValue
		);

	bool
	Weaken (CValue* pValue)
	{
		return Weaken (*pValue, pValue);
	}

	// fields

	bool
	GetField (
		const CValue& OpValue,
		CStructField* pMember,
		CBaseTypeCoord* pCoord,
		CValue* pResultValue
		);

	bool
	GetField (
		const CValue& OpValue,
		CStructField* pMember,
		CValue* pResultValue
		)
	{
		return GetField (OpValue, pMember, NULL, pResultValue);
	}

	bool
	GetField (
		CValue* pValue,
		CStructField* pMember
		)
	{
		return GetField (*pValue, pMember, NULL, pValue);
	}

	// impl

	bool
	GetStructField  (
		const CValue& OpValue,
		CStructField* pMember,
		CBaseTypeCoord* pCoord,
		CValue* pResultValue
		);

	bool
	GetUnionField (
		const CValue& OpValue,
		CStructField* pMember,
		CValue* pResultValue
		);

	bool
	GetClassField (
		const CValue& OpValue,
		CStructField* pMember,
		CBaseTypeCoord* pCoord,
		CValue* pResultValue
		);

	bool
	GetPropertyField (
		const CValue& OpValue,
		CModuleItem* pMember,
		CValue* pResultValue
		);

	// misc

	bool
	GetVirtualMethod (
		CFunction* pFunction,
		CClosure* pClosure,
		CValue* pResultValue
		);

	bool
	GetVirtualProperty (
		CProperty* pProperty,
		CClosure* pClosure,
		CValue* pResultValue
		);

	void
	GetThinDataPtrRange (
		const CValue& Value,
		CValue* pRangeBeginValue,
		CValue* pRangeEndValue
		);

	void
	GetThinDataPtrScopeLevel (
		const CValue& Value,
		CValue* pResultValue
		);

	void
	GetClassPtrScopeLevel (
		const CValue& Value,
		CValue* pResultValue
		);

	void
	ProcessDestructArray (
		CVariable* const* ppDestructArray,
		size_t Count
		);

	void
	ProcessDestructArray (const rtl::CArrayT <CVariable*> DestructArray)
	{
		ProcessDestructArray (DestructArray, DestructArray.GetCount ());
	}

	void
	ProcessLazyStaticDestructList (const rtl::CConstListT <TLazyStaticDestruct>& List);

	void
	NullifyGcRootList (const rtl::CConstBoxListT <CValue>& List);
	
	// closures

	bool
	CreateClosureObject (
		EStorage StorageKind,
		const CValue& OpValue, // thin function or property ptr with closure
		CType* pThunkType, // function or property type
		CValue* pResultValue
		);

	bool
	CreateDataClosureObject (
		EStorage StorageKind,
		const CValue& OpValue, // data ptr
		CPropertyType* pThunkType, // function or property type
		CValue* pResultValue
		);

	// checks

	void
	GetDataRefScopeLevel (
		const CValue& Value,
		CValue* pScopeLevelValue
		);

	void
	CheckDataPtrRange (const CValue& Value);

	void
	CheckDataPtrRange (
		const CValue& PtrValue,
		size_t Size,
		const CValue& RangeBeginValue,
		const CValue& RangeEndValue
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

protected:
	// bit fields

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
	GetNamespaceMemberType (
		CNamespace* pNamespace,
		const char* pName,
		CValue* pResultValue
		);

	bool
	GetNamespaceMember (
		CNamespace* pNamespace,
		const char* pName,
		CValue* pResultValue
		);

	CType*
	GetNamedTypeMemberType (
		const CValue& OpValue,
		CNamedType* pNamedType,
		const char* pName
		);

	bool
	GetNamedTypeMember (
		const CValue& OpValue,
		CNamedType* pNamedType,
		const char* pName,
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

	bool
	DeleteDataPtr (const CValue& OpValue);

	bool
	DeleteClassPtr (const CValue& OpValue);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

