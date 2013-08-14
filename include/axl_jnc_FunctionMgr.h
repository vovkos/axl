// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"
#include "axl_jnc_ScheduleLauncherFunction.h"
#include "axl_jnc_ThunkFunction.h"
#include "axl_jnc_ThunkProperty.h"
#include "axl_jnc_PropertyTemplate.h"
#include "axl_jnc_ClassType.h"

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
	//		int8* pRangeEnd
	//		);

	EStdFunc_CheckDataPtrRange,

	// void 
	// jnc.CheckClassPtrScopeLevel (
	//		object* p,
	//		size_t DstScopeLevel
	//		);

	EStdFunc_CheckClassPtrScopeLevel,

	// object*
	// jnc.DynamicCastClassPtr (
	//		object* p,
	//		int8* pType
	//		);

	EStdFunc_DynamicCastClassPtr,

	// object*
	// jnc.StrengthenClassPtr (weak object* p);

	EStdFunc_StrengthenClassPtr,

	// int8*
	// jnc.HeapAlloc (size_t Size);

	EStdFunc_HeapAlloc,

	// int8*
	// jnc.UHeapAlloc (size_t Size);

	EStdFunc_UHeapAlloc,

	// void
	// jnc.UHeapFree (int8* p);

	EStdFunc_UHeapFree,

	// void
	// jnc.UHeapFreeClassPtr (object* p);

	EStdFunc_UHeapFreeClassPtr,

	// void
	// jnc.GcAddObject (object.hdr* p);

	EStdFunc_GcAddObject,

	// void
	// jnc.GetSafePoint ();

	EStdFunc_GcSafePoint,

	// void
	// jnc.MarkGcRoot (
	//		int8* p,
	//		int8* pType
	//		);

	EStdFunc_MarkGcRoot,

	// void
	// jnc.RunGc ();

	EStdFunc_RunGc,

	// void
	// jnc.RunGcWaitForDestructors ();

	EStdFunc_RunGcWaitForDestructors,

	// i64
	// jnc.GetCurrentThreadId ();

	EStdFunc_GetCurrentThreadId,

	// i64
	// jnc.CreateThread (function* pf ());

	EStdFunc_CreateThread,

	// void
	// jnc.Sleep (uint_t MsCount);

	EStdFunc_Sleep,

	// jnc.TTlsStruct*
	// jnc.GetTls ();

	EStdFunc_GetTls,

	// size_t
	// jnc.AppendFmtLiteral_a (
	//		jnc.TFmtLiteral* pLiteral,
	//		char* p,
	//		size_t Length
	//		);

	EStdFunc_AppendFmtLiteral_a,

	// size_t
	// jnc.AppendFmtLiteral_p (
	//		jnc.TFmtLiteral* pFmtLiteral,
	//		char* pFmtSpecifier,
	//		jnc.TPtr Ptr
	//		);

	EStdFunc_AppendFmtLiteral_p,

	// size_t
	// jnc.AppendFmtLiteral_i32 (
	//		jnc.TFmtLiteral* pFmtLiteral,
	//		char* pFmtSpecifier,
	//		i32 i
	//		);

	EStdFunc_AppendFmtLiteral_i32,

	// size_t
	// jnc.AppendFmtLiteral_ui32 (
	//		jnc.TFmtLiteral* pFmtLiteral,
	//		char* pFmtSpecifier,
	//		i32 i
	//		);

	EStdFunc_AppendFmtLiteral_ui32,

	// size_t
	// jnc.AppendFmtLiteral_i64 (
	//		jnc.TFmtLiteral* pFmtLiteral,
	//		char* pFmtSpecifier,
	//		i64 i
	//		);

	EStdFunc_AppendFmtLiteral_i64,

	// size_t
	// jnc.AppendFmtLiteral_ui64 (
	//		jnc.TFmtLiteral* pFmtLiteral,
	//		char* pFmtSpecifier,
	//		i64 i
	//		);

	EStdFunc_AppendFmtLiteral_ui64,

	// size_t
	// jnc.AppendFmtLiteral_f (
	//		jnc.TFmtLiteral* pFmtLiteral,
	//		char* pFmtSpecifier,
	//		double f
	//		);

	EStdFunc_AppendFmtLiteral_f,

	EStdFunc__Count
};

//.............................................................................

class CFunctionMgr
{
	friend class CModule;
	friend class CDerivableType;
	friend class CClassType;
	friend class CFunction;
	friend class CParser;

protected:
	struct TEmissionContext: rtl::TListLink
	{
		CFunction* m_pCurrentFunction;

		CNamespace* m_pCurrentNamespace;
		CScope* m_pCurrentScope;

		CBasicBlock* m_pCurrentBlock;
		CBasicBlock* m_pReturnBlock;
		CBasicBlock* m_pSilentReturnBlock;
		CBasicBlock* m_pUnreachableBlock;
		uint_t m_ControlFlowMgrFlags;

		CValue m_ThisValue;
		CValue m_ScopeLevelValue;
		CValue m_VTablePtrPtrValue; 
		CValue m_VTablePtrValue;
	};

protected:
	CModule* m_pModule;

	// unfortunately LLVM does not provide a slot for back-pointer from llvm::Function, hence the map

	rtl::CHashTableMapT <llvm::Function*, CFunction*, rtl::CHashIdT <llvm::Function*> > m_LlvmFunctionMap; 

	rtl::CStdListT <CFunction> m_FunctionList;
	rtl::CStdListT <CProperty> m_PropertyList;
	rtl::CStdListT <CPropertyTemplate> m_PropertyTemplateList;
	rtl::CStdListT <CScheduleLauncherFunction> m_ScheduleLauncherFunctionList;
	rtl::CStdListT <CThunkFunction> m_ThunkFunctionList;
	rtl::CStdListT <CThunkProperty> m_ThunkPropertyList;
	rtl::CStdListT <CDataThunkProperty> m_DataThunkPropertyList;
	rtl::CStringHashTableMapT <CFunction*> m_ThunkFunctionMap;
	rtl::CStringHashTableMapT <CProperty*> m_ThunkPropertyMap;
	rtl::CStringHashTableMapT <CFunction*> m_ScheduleLauncherFunctionMap;

	CFunction* m_pCurrentFunction;
	
	CValue m_ThisValue;
	CValue m_ScopeLevelValue;
	CValue m_VTablePtrPtrValue; 
	CValue m_VTablePtrValue;

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

	CFunction*
	FindFunctionByLlvmFunction (llvm::Function* pLlvmFunction)
	{
		rtl::CHashTableMapIteratorT <llvm::Function*, CFunction*> It = m_LlvmFunctionMap.Find (pLlvmFunction);
		return It ? It->m_Value : NULL;
	}

	CProperty*
	GetCurrentProperty ()
	{
		return m_pCurrentFunction ? m_pCurrentFunction->GetProperty () : NULL;
	}

	CValue 
	GetThisValue ()
	{
		return m_ThisValue;
	}

	CValue 
	GetScopeLevelValue ()
	{
		return m_ScopeLevelValue;
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

	rtl::CConstListT <CThunkFunction>
	GetThunkFunctionList ()
	{
		return m_ThunkFunctionList;
	}

	rtl::CConstListT <CThunkProperty>
	GetThunkPropertyList ()
	{
		return m_ThunkPropertyList;
	}

	rtl::CConstListT <CDataThunkProperty>
	GetDataThunkPropertyList ()
	{
		return m_DataThunkPropertyList;
	}

	CFunction*
	CreateFunction (
		EFunction FunctionKind,
		CFunctionType* pType
		);

	CFunction*
	CreateInternalFunction (
		const char* pTag,
		CFunctionType* pType 
		);

	CFunction*
	CreateFunction (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		CFunctionType* pType
		);

	CProperty*
	CreateProperty (
		EProperty PropertyKind,
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		uint_t Flags = 0
		);

	CProperty*
	CreateProperty (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		uint_t Flags = 0
		)
	{
		return CreateProperty (EProperty_Normal, Name, QualifiedName, Flags);
	}

	CProperty*
	CreateInternalProperty (
		const char* pTag,
		CPropertyType* pType
		);

	CPropertyTemplate*
	CreatePropertyTemplate ();

	bool
	Prologue (
		CFunction* pFunction,
		const CToken::CPos& Pos
		);

	bool
	Epilogue (const CToken::CPos& Pos);

	bool
	FireOnChangeEvent ();

	void
	InternalPrologue (
		CFunction* pFunction,
		CValue* pArgValueArray = NULL,
		size_t ArgCount = 0
		);

	void
	InternalEpilogue ();

	bool
	InjectTlsPrologues ();

	bool
	JitFunctions (llvm::ExecutionEngine* pExecutionEngine);

	// std functions

	CFunction*
	GetStdFunction (EStdFunc Func);

	CFunction*
	GetDirectThunkFunction (
		CFunction* pTargetFunction,
		CFunctionType* pThunkFunctionType,
		bool HasUnusedClosure = false
		);

	CProperty*
	GetDirectThunkProperty (
		CProperty* pTargetProperty,
		CPropertyType* pThunkPropertyType,
		bool HasUnusedClosure = false
		);

	CProperty*
	GetDirectDataThunkProperty (
		CVariable* pTargetVariable,
		CPropertyType* pThunkPropertyType,
		bool HasUnusedClosure = false
		);

	CFunction*
	GetScheduleLauncherFunction (
		CFunctionPtrType* pTargetFunctionPtrType,
		EClassPtrType SchedulerPtrTypeKind = EClassPtrType_Normal
		);

protected:
	bool
	CreateShadowArgVariables ();

	void
	CreateThisValue (const CValue& ThisArgValue);

	void
	PushEmissionContext ();

	void
	PopEmissionContext ();

	void
	CutVTable (const CValue& ThisArgValue);

	void
	RestoreVTable ();

	void
	InjectTlsPrologue (CFunction* pFunction);

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
	CreateStrengthenClassPtr ();

	CFunction*
	CreateHeapAlloc ();

	CFunction*
	CreateUHeapAlloc ();

	CFunction*
	CreateUHeapFree ();

	CFunction*
	CreateUHeapFreeClassPtr ();

	CFunction*
	CreateGcAddObject ();

	CFunction*
	CreateMarkGcRoot ();

	CFunction*
	CreateRunGc ();

	CFunction*
	CreateRunGcWaitForDestructors ();

	CFunction*
	CreateGetCurrentThreadId ();

	CFunction*
	CreateCreateThread ();

	CFunction*
	CreateSleep ();

	CFunction*
	CreateGetTls ();

	CFunction*
	CreateGcSafePoint ();

	CFunction*
	CreateAppendFmtLiteral_a ();

	CFunction*
	CreateAppendFmtLiteral_p ();

	CFunction*
	CreateAppendFmtLiteral_i32 ();

	CFunction*
	CreateAppendFmtLiteral_ui32 ();

	CFunction*
	CreateAppendFmtLiteral_i64 ();

	CFunction*
	CreateAppendFmtLiteral_ui64 ();

	CFunction*
	CreateAppendFmtLiteral_f ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
