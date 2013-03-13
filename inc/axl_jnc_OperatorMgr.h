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
#include "axl_jnc_CastOp_Struct.h"
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
	friend class CParser;
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
	CCast_ClassPtr m_Cast_ClassPtr;
	CCast_FunctionPtr m_Cast_FunctionPtr;
	CCast_PropertyPtr m_Cast_PropertyPtr;

	// tables

	IUnaryOperator* m_UnaryOperatorTable [EUnOp__Count];
	IBinaryOperator* m_BinaryOperatorTable [EBinOp__Count];		
	ICastOperator* m_CastOperatorTable [EType__Count];
	ICastOperator* m_StdCastOperatorTable [EStdCast__Count];
	
	rtl::CStringHashTableMapT <EMulticastMethod> m_MulticastMethodMap;

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

	bool
	PrepareDataPtr (
		const CValue& Value,
		ERuntimeError Error,
		CValue* pResultValue
		);

	bool
	PrepareDataPtr (
		CValue* pValue,
		ERuntimeError Error
		)
	{
		return PrepareDataPtr (*pValue, Error, pValue);
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
		return CastOperator (EStorage_Undefined, OpValue, pType, pResultValue);
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
		return CastOperator (EStorage_Undefined, *pValue, pType, pValue);
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
		return CastOperator (EStorage_Undefined, OpValue, TypeKind, pResultValue);
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
		return CastOperator (EStorage_Undefined, *pValue, TypeKind, pValue);
	}

	// new & delete operators

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

	// member operators

	CType*
	GetMemberOperatorResultType (
		const CValue& OpValue,
		const tchar_t* pName
		);

	bool
	GetMemberOperatorResultType (
		const CValue& OpValue,
		const tchar_t* pName,
		CValue* pResultValue
		);

	bool
	GetMemberOperatorResultType (
		CValue* pValue,
		const tchar_t* pName
		)
	{
		return GetMemberOperatorResultType (*pValue, pName, pValue);
	}

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

	// call operators

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

	// base type constructor calls

	bool
	CallBaseTypeConstructor (
		CType* pType,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	PostBaseTypeConstructorList ();

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

	// property accessors

	CType*
	GetFunctionType (
		const CValue& OpValue,
		CFunctionType* pFunctionType
		);

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

	// augmented property fields

	CType*
	GetAuPropertyFieldMemberType (
		const CValue& OpValue,
		EAuPropertyField Field
		);

	bool
	GetAuPropertyFieldMemberType (
		const CValue& OpValue,
		EAuPropertyField Field,
		CValue* pResultValue
		);

	bool
	GetAuPropertyFieldMemberType (
		CValue* pValue,	
		EAuPropertyField Field
		)
	{
		return GetAuPropertyFieldMemberType (*pValue, Field, pValue);
	}

	bool
	GetAuPropertyFieldMember (
		const CValue& OpValue,
		EAuPropertyField Field,
		CValue* pResultValue
		);

	bool
	GetAuPropertyFieldMember (
		CValue* pValue,	
		EAuPropertyField Field
		)
	{
		return GetAuPropertyFieldMember (*pValue, Field, pValue);
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
		const CValue& OpValue1,
		const CValue& OpValue2
		);

	// misc

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
	ProcessDestructList (rtl::CBoxListT <CValue>* pList);
	
	bool
	CreateClosureObject (
		EStorage StorageKind,
		const CValue& OpValue, // function or property ptr
		rtl::CArrayT <size_t>* pClosureMap,
		CValue* pResultValue
		);

	bool
	GetAutoEvData (
		CAutoEv* pAutoEv,
		CValue* pResultValue
		);

	bool
	GetAutoEvBindSiteArray (
		CAutoEv* pAutoEv,
		CValue* pResultValue
		);

protected:

	// checks

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

	CType*
	GetFieldMemberType (
		const CValue& OpValue,
		CStructField* pMember
		);

	bool
	GetFieldMember (
		CStructField* pMember,
		CBaseTypeCoord* pCoord,
		CValue* pResultValue
		);

	CType*
	GetStructMemberType (
		const CValue& OpValue,
		CStructType* pStructType,
		const tchar_t* pName
		);

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

	CType*
	GetUnionMemberType (
		const CValue& OpValue,
		CUnionType* pUnionType,
		const tchar_t* pName
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

	CType*
	GetMulticastMemberType (
		const CValue& OpValue,
		CMulticastType* pMulticastType,
		const tchar_t* pName
		);

	bool
	GetMulticastMember (
		const CValue& OpValue,
		CMulticastType* pMulticastType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	CType*
	GetAutoEvMemberType (
		const CValue& OpValue,
		CAutoEvType* pAutoEvType,
		const tchar_t* pName
		);

	bool
	GetAutoEvMember (
		const CValue& OpValue,
		CAutoEvType* pAutoEvType,
		const tchar_t* pName,
		CValue* pResultValue
		);

	CType*
	GetClassMemberType (
		const CValue& OpValue,
		CClassType* pClassType,
		const tchar_t* pName
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
	InitializeObject (
		const CValue& ObjPtrValue,
		CClassType* pClassType,
		rtl::CBoxListT <CValue>* pArgList,
		CValue* pResultValue
		);

	bool
	DeleteDataPtr (const CValue& OpValue);

	bool
	DeleteClassPtr (const CValue& OpValue);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

