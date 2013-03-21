// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"
#include "axl_jnc_PropertyTemplate.h"
#include "axl_jnc_AutoEv.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EStdFunc
{
	// void
	// jnc.OnRuntimeError (
	//		int Error,
	//		int8* pCodeAddr
	//		);

	EStdFunc_OnRuntimeError,

	// void 
	// jnc.CheckNullPtr (
	//		int8* p,
	//		int Error
	//		);

	EStdFunc_CheckNullPtr,

	// void 
	// jnc.CheckScopeLevel (
	//		size_t SrcScopeLevel
	//		size_t DstScopeLevel
	//		);

	EStdFunc_CheckScopeLevel,

	// void 
	// jnc.CheckDataPtrRange (
	//		int8* p,
	//		size_t Size,
	//		int8* pRangeBegin,
	//		int8* pRangeEnd,
	//		int Error
	//		);

	EStdFunc_CheckDataPtrRange,

	// void 
	// jnc.CheckClassPtrScopeLevel (
	//		object p,
	//		size_t DstScopeLevel
	//		);

	EStdFunc_CheckClassPtrScopeLevel,

	// object
	// jnc.DynamicCastClassPtr (
	//		object p,
	//		int8* pType
	//		);

	EStdFunc_DynamicCastClassPtr,

	// int8*
	// jnc.HeapAlloc (int8* pType);

	EStdFunc_HeapAlloc,

	// int8*
	// jnc.UHeapAlloc (int8* pType);

	EStdFunc_UHeapAlloc,

	// void
	// jnc.UHeapFree (int8* p);

	EStdFunc_UHeapFree,

	// void
	// jnc.UHeapFreeClassPtr (object p);

	EStdFunc_UHeapFreeClassPtr,

	// intptr
	// jnc.MulticastSet (
	//		multicast* pMulticast (), 
	//		function* pfn ()
	//		);

	EStdFunc_MulticastSet,
	EStdFunc_MulticastSet_w,
	EStdFunc_MulticastSet_t,
	EStdFunc_MulticastSet_u,

	// intptr
	// jnc.MulticastAdd (
	//		multicast* pMulticast (), 
	//		function* pfn ()
	//		);

	EStdFunc_MulticastAdd,
	EStdFunc_MulticastAdd_w,
	EStdFunc_MulticastAdd_t,
	EStdFunc_MulticastAdd_u,

	// function* ()
	// jnc.MulticastRemove (
	//		multicast* pMulticast (), 
	//		intptr Handle
	//		);

	EStdFunc_MulticastRemove,
	EStdFunc_MulticastRemove_w,
	EStdFunc_MulticastRemove_t,
	EStdFunc_MulticastRemove_u,

	// mcsnapshot ()
	// jnc.MulticastSnapshot (multicast* pMulticast ());

	EStdFunc_MulticastSnapshot,
	EStdFunc_MulticastSnapshot_w,
	EStdFunc_MulticastSnapshot_t,
	EStdFunc_MulticastSnapshot_u,
 
	EStdFunc__Count
};

//.............................................................................

class CFunctionMgr
{
protected:
	friend class CModule;
	friend class CDerivableType;
	friend class CClassType;
	friend class CParser;

protected:
	enum EThunk
	{
		EThunk_Undefined = 0,
		EThunk_Direct,
		EThunk_DirectUnusedClosure,
		EThunk_Closure,
	};

	struct TThunk: rtl::TListLink
	{
		EThunk m_ThunkKind;
		rtl::CStringA m_Signature;
		CFunction* m_pTargetFunction; 
		CFunctionType* m_pTargetFunctionType;
		CClassType* m_pClosureType;
		rtl::CArrayT <size_t> m_ClosureMap;
		CFunction* m_pThunkFunction;
	};

	struct TEmissionContext: rtl::TListLink
	{
		CFunction* m_pCurrentFunction;
		CBasicBlock* m_pCurrentBlock;
		CBasicBlock* m_pReturnBlock;
		CBasicBlock* m_pSilentReturnBlock;
		int m_ControlFlowMgrFlags;
		CValue m_ThisValue;
		CValue m_ThinThisValue;
		CValue m_ScopeLevelValue;
		CValue m_VTablePtrPtrValue; 
		CValue m_VTablePtrValue;
		rtl::CIteratorT <CStructField> m_MemberNewField;
	};

protected:
	CModule* m_pModule;

	rtl::CStdListT <CFunction> m_FunctionList;
	rtl::CStdListT <CProperty> m_PropertyList;
	rtl::CStdListT <CPropertyTemplate> m_PropertyTemplateList;
	rtl::CStdListT <CAutoEv> m_AutoEvList;

	rtl::CArrayT <CFunction*> m_OrphanFunctionArray;
	rtl::CArrayT <CDerivableType*> m_DefaultPreConstructorTypeArray;

	rtl::CStdListT <TThunk> m_ThunkList;
	rtl::CStringHashTableMapAT <CFunction*> m_ThunkFunctionMap;
	rtl::CStringHashTableMapAT <CProperty*> m_ThunkPropertyMap;
	rtl::CStringHashTableMapAT <CFunction*> m_ScheduleLauncherFunctionMap;

	CFunction* m_pCurrentFunction;
	
	CValue m_ThisValue;
	CValue m_ThinThisValue;
	CValue m_ScopeLevelValue;
	CValue m_VTablePtrPtrValue; 
	CValue m_VTablePtrValue;
	rtl::CIteratorT <CStructField> m_MemberNewField;

	rtl::CStdListT <TEmissionContext> m_EmissionContextStack;

	CFunction* m_StdFunctionArray [EStdFunc__Count];

public:
	CFunctionMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	CFunction*
	GetCurrentFunction ()
	{
		return m_pCurrentFunction;
	}

	CProperty*
	GetCurrentProperty ()
	{
		return m_pCurrentFunction ? m_pCurrentFunction->GetProperty () : NULL;
	}

	CAutoEv*
	GetCurrentAutoEv ()
	{
		return m_pCurrentFunction ? m_pCurrentFunction->GetAutoEv () : NULL;
	}

	CValue 
	GetThisValue ()
	{
		return m_ThisValue;
	}

	CValue 
	GetThinThisValue ()
	{
		return m_ThinThisValue;
	}

	CValue 
	GetScopeLevelValue ()
	{
		return m_ScopeLevelValue;
	}

	CStructField*
	GetMemberNewField ()
	{
		return *m_MemberNewField;
	}

	void
	NextMemberNewField ()
	{
		ASSERT (m_MemberNewField);
		m_MemberNewField++;
	}

	void
	Clear ();

	rtl::CConstListT <CFunction>
	GetFunctionList ()
	{
		return m_FunctionList;
	}

	rtl::CConstListT <CProperty>
	GetPropertyList ()
	{
		return m_PropertyList;
	}

	CFunction*
	CreateFunction (
		EFunction FunctionKind,
		CFunctionType* pType,
		rtl::CStdListT <CFunctionFormalArg>* pArgList = NULL
		);

	CFunction*
	CreateInternalFunction (
		const tchar_t* pTag,
		CFunctionType* pType 
		);

	CProperty*
	CreateProperty (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CPropertyTemplate*
	CreatePropertyTemplate ();

	CAutoEv*
	CreateAutoEv (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CAutoEv*
	CreateUnnamedAutoEv ()
	{
		return CreateAutoEv (rtl::CString (), rtl::CString ());
	}

	bool
	CalcPropertyLayouts ();

	bool
	CalcAutoEvLayouts ();

	bool
	ResolveOrphanFunctions ();

	bool
	ScanAutoEvs ();

	bool
	CompileFunctions ();

	bool
	Prologue (
		CFunction* pFunction,
		const CToken::CPos& Pos
		);

	bool
	Epilogue (const CToken::CPos& Pos);

	void
	InternalPrologue (
		CFunction* pFunction,
		CValue* pArgValueArray = NULL,
		size_t ArgCount = 0
		);

	void
	InternalEpilogue ();

	bool
	JitFunctions (llvm::ExecutionEngine* pExecutionEngine);

	CFunction*
	GetStdFunction (EStdFunc Func);

	CFunction*
	GetDirectThunkFunction (
		CFunction* pTargetFunction,
		CFunctionType* pThunkFunctionType,
		bool HasUnusedClosure = false
		);

	CFunction*
	GetClosureThunkFunction (
		CFunctionType* pTargetFunctionType,
		CFunction* pTargetFunction, // could be NULL
		CClassType* pClosureType,
		const rtl::CArrayT <size_t>& ClosureMap,
		CFunctionType* pThunkFunctionType
		);

	CProperty*
	GetDirectThunkProperty (
		CProperty* pTargetProperty,
		CPropertyType* pThunkPropertyType,
		bool HasUnusedClosure = false
		);

	CProperty*
	GetClosureThunkProperty (
		CPropertyType* pTargetPropertyType,
		CProperty* pTargetProperty, // could be NULL
		CClassType* pClosureType,
		const rtl::CArrayT <size_t>& ClosureMap,
		CPropertyType* pThunkPropertyType
		);

	CProperty*
	GetDirectDataThunkProperty (
		CVariable* pTargetVariable,
		CPropertyType* pThunkPropertyType,
		bool HasUnusedClosure = false
		);

	CProperty*
	GetClosureDataThunkProperty (
		CDataPtrType* pTargetDataPtrType,
		CPropertyType* pThunkPropertyType
		);

	CFunction*
	GetScheduleLauncherFunction (
		CFunctionPtrType* pTargetFunctionPtrType,
		CFunction* pTargetFunction, // could be NULL
		CClassPtrType* pSchedulerType // could be weak
		);

protected:
	bool
	CompileDirectThunk (TThunk* pThunk);

	bool
	CompileClosureThunk (TThunk* pThunk);

	bool
	CompileAutoPropertyAccessors (CProperty* pProperty);

	void
	CreateThinThisValue ();

	void
	SaveEmissionContext ();

	void
	RestoreEmissionContext ();

	void
	ClearEmissionContext ();

	void
	CutVTable (const CValue& ThisArgValue);

	void
	RestoreVTable ();

	bool
	FireOnChangeEvent ();

	// LLVM code support functions

	CFunction*
	CreateOnRuntimeError ();

	CFunction*
	CreateCheckNullPtr ();

	CFunction*
	CreateCheckScopeLevel ();

	CFunction*
	CreateCheckDataPtrRange ();

	CFunction*
	CreateCheckClassPtrScopeLevel ();

	CFunction*
	CreateDynamicCastClassPtr ();

	CFunction*
	CreateHeapAlloc ();

	CFunction*
	CreateUHeapAlloc ();

	CFunction*
	CreateUHeapFree ();

	CFunction*
	CreateUHeapFreeClassPtr ();

	CFunction*
	CreateMulticastSet (
		EFunctionPtrType PtrTypeKind,
		const tchar_t* pTag
		);

	CFunction*
	CreateMulticastAdd (
		EFunctionPtrType PtrTypeKind,
		const tchar_t* pTag
		);

	CFunction*
	CreateMulticastRemove (
		EFunctionPtrType PtrTypeKind,
		const tchar_t* pTag
		);

	CFunction*
	CreateMulticastSnapshot (
		EFunctionPtrType PtrTypeKind,
		const tchar_t* pTag
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
