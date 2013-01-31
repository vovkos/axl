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
	// jnc.CheckInterfaceScopeLevel (
	//		object p,
	//		size_t DstScopeLevel
	//		);

	EStdFunc_CheckInterfaceScopeLevel,

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
	class CThunk: public rtl::TListLink
	{
	public:
		rtl::CString m_Signature;
		CFunction* m_pTargetFunction;
		CFunctionType* m_pTargetFunctionType;
		CFunctionPtrType* m_pFunctionPtrType;
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
	rtl::CStringHashTableMapAT <CThunk*> m_ThunkMap;
	rtl::CStdListT <CThunk> m_ThunkList;

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
	GetThunkFunction (
		CFunctionType* pTargetFunctionType,
		CFunction* pTargetFunction, // could be NULL
		CClassType* pClosureType,   // could be NULL
		const rtl::CArrayT <size_t>& ClosureMap,
		CFunctionPtrType* pFunctionPtrType
		);

	CFunction*
	GetThunkFunction (
		CFunctionType* pTargetFunctionType,
		CFunction* pTargetFunction, // could be NULL
		CFunctionPtrType* pFunctionPtrType
		)
	{
		return GetThunkFunction (
			pTargetFunctionType, 
			pTargetFunction, 
			NULL, 
			rtl::CArrayT <size_t> (), 
			pFunctionPtrType
			);
	}

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
	CreateCheckInterfaceScopeLevel ();

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

