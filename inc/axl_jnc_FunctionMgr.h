// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"

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
	// jnc.CheckSafePtrRange (
	//		int8* p,
	//		size_t Size,
	//		jnc.sptrv Validator,
	//		int Error
	//		);

	EStdFunc_CheckSafePtrRange,

	// void 
	// jnc.CheckInterfaceScopeLevel (
	//		jnc.iface* p,
	//		size_t DstScopeLevel
	//		);

	EStdFunc_CheckInterfaceScopeLevel,

	// int8*
	// jnc.DynamicCastInterface (
	//		int8* p,
	//		int8* pType
	//		);

	EStdFunc_DynamicCastInterface,

	// int8*
	// jnc.EventOperator (
	//		jnc.event* pEvent,
	//		void* pfn,
	//		int CallConv,
	//		jnc.iface* pIface,
	//		int OpKind
	//		);

	EStdFunc_EventOperator,

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

	CModule* m_pModule;

	rtl::CStdListT <CFunction> m_FunctionList;
	rtl::CStdListT <CGlobalFunction> m_GlobalFunctionList;
	
	rtl::CArrayT <CClassType*> m_GlobalAutoEvTypeArray;

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

	rtl::CIteratorT <CFunction>
	GetFirstFunction ()
	{
		return m_FunctionList.GetHead ();
	}

	rtl::CIteratorT <CGlobalFunction>
	GetFirstGlobalFunction ()
	{
		return m_GlobalFunctionList.GetHead ();
	}

	CFunction*
	CreateAnonimousFunction (CFunctionType* pType);

	CFunction*
	CreateFunction (
		const CQualifiedName& Name,
		CFunctionType* pType,
		rtl::CStdListT <CFunctionFormalArg>* pArgList = NULL
		);

	CFunction*
	CreatePropertyAccessorFunction (
		EPropertyAccessor AccessorKind,
		CFunctionType* pType,
		rtl::CStdListT <CFunctionFormalArg>* pArgList = NULL
		);

	CGlobalFunction*
	CreateGlobalFunction (
		const rtl::CString& Name,
		CFunction* pFunction
		);

	bool
	CompileGlobalAutoEv ();

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

protected:
	bool
	ResolveOrphanFunction (CFunction* pFunction);

	CFunction*
	CreateInternalFunction (
		const rtl::CString& Name,
		CFunctionType* pType
		);

	// LLVM code support functions

	CFunction*
	CreateOnRuntimeError ();

	CFunction*
	CreateCheckNullPtr ();

	CFunction*
	CreateCheckScopeLevel ();

	CFunction*
	CreateCheckSafePtrRange ();

	CFunction*
	CreateCheckInterfaceScopeLevel ();

	CFunction*
	CreateDynamicCastInterface ();

	CFunction*
	CreateEventOperator ();

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

} // namespace axl {
} // namespace jnc {

