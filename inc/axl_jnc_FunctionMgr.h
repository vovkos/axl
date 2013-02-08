// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"
#include "axl_jnc_PropertyTemplate.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EStdFunc
{
	// void
	// jnc.OnRuntimeError (
	//		int Error,
	//		int8* pCodeAddr,
	//		int8* pDataAddr
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
	//		jnc.sptrv Validator,
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
	// jnc.DynamicCastInterface (
	//		object p,
	//		int8* pType
	//		);

	EStdFunc_DynamicCastInterface,

	// void
	// jnc.EventOperator (
	//		jnc.event* pEvent,
	//		void* pfn,
	//		object pClosure,
	//		int OpKind
	//		);

	EStdFunc_EventOperator,

	// void
	// jnc.FireSimpleEvent (jnc.event* pEvent);

	EStdFunc_FireSimpleEvent,

	// int8*
	// jnc.HeapAllocate (int8* pType);

	EStdFunc_HeapAllocate,

	EStdFunc__Count
};

//.............................................................................

class CFunctionMgr
{
protected:
	friend class CModule;
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

	class CThunk: public rtl::TListLink
	{
	public:
		EThunk m_ThunkKind;
		rtl::CStringA m_Signature;
		CFunction* m_pTargetFunction; 
		CFunctionType* m_pTargetFunctionType;
		CClassType* m_pClosureType;
		rtl::CArrayT <size_t> m_ClosureMap;

		CFunction* m_pThunkFunction;

		CThunk ();
	};

protected:
	CModule* m_pModule;

	rtl::CStdListT <CFunction> m_FunctionList;
	rtl::CStdListT <CProperty> m_PropertyList;
	rtl::CStdListT <CPropertyTemplate> m_PropertyTemplateList;
	rtl::CArrayT <CFunction*> m_OrphanFunctionArray;
	rtl::CArrayT <CClassType*> m_GlobalAutoEvTypeArray;
	rtl::CArrayT <CProperty*> m_AutoPropertyArray;

	rtl::CStdListT <CThunk> m_ThunkList;
	rtl::CStringHashTableMapAT <CFunction*> m_ThunkFunctionMap;
	rtl::CStringHashTableMapAT <CProperty*> m_ThunkPropertyMap;

	CFunction* m_pCurrentFunction;
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

	CProperty*
	CreateProperty (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CPropertyTemplate*
	CreatePropertyTemplate ();

	bool
	ResolveOrphanFunctions ();

	bool
	CompileFunctions ();

	bool
	Prologue (
		CFunction* pFunction,
		const CToken::CPos& Pos,
		CValue* pThisValue
		);

	bool
	Epilogue (const CToken::CPos& Pos);

	bool
	JitFunctions (llvm::ExecutionEngine* pExecutionEngine);

	CFunction*
	GetStdFunction (EStdFunc Func);

	CFunction*
	CreateClassInitializer (CClassType* pType);

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

protected:
	bool
	CompileDirectThunk (CThunk* pThunk);

	bool
	CompileClosureThunk (CThunk* pThunk);

	bool
	CompileAutoPropertyAccessors (CProperty* pProperty);

	// LLVM code support functions

	CFunction*
	CreateInternalFunction (
		const tchar_t* pTag,
		CFunctionType* pType 
		);

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
	CreateDynamicCastInterface ();

	CFunction*
	CreateEventOperator ();

	CFunction*
	CreateFireSimpleEvent ();

	CFunction*
	CreateHeapAllocate ();

	bool
	InitializeInterface (
		CClassType* pType,
		const CValue& ObjectPtrValue,
		const CValue& IfacePtrValue,
		const CValue& VTablePtrValue
		);

	bool
	CreateThisValue (
		const CValue& ArgValue,
		CClassType* pResultType,
		CValue* pResultValue
		);

	bool
	RuntimeError (
		const CValue& ErrorValue,
		const CValue& DataAddrValue
		);

	bool
	RuntimeError (
		ERuntimeError Error,
		const CValue& DataAddrValue
		)
	{
		return RuntimeError (CValue (Error, EType_Int), DataAddrValue);
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

