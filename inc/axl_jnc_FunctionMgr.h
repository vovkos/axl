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
	// jnc.sptrv
	// jnc.CreateSafePtrValidator (
	//		int8* pRegionBegin,
	//		size_t Size,
	//		size_t ScopeLevel
	//		);

	EStdFunc_CreateSafePtrValidator,

	// void 
	// jnc.CheckSafePtrRange (
	//		int8* p,
	//		size_t Size,
	//		jnc.sptrv Validator,
	//		int Error
	//		);

	EStdFunc_CheckSafePtrRange,

	// void 
	// jnc.CheckSafePtrScope (
	//		int8* p,
	//		jnc.sptrv Validator,
	//		size_t DstScopeLevel
	//		);

	EStdFunc_CheckSafePtrScope,

	// void
	// jnc.OnInvalidSafePtr (
	//		int8* p,
	//		jnc.sptrv Validator,
	//		int Error
	//		);

	EStdFunc_OnInvalidSafePtr,

	// void 
	// jnc.CheckInterfaceNull (
	//		int8* p,
	//		size_t ScopeLevel
	//		);

	EStdFunc_CheckInterfaceNull,

	// void 
	// jnc.CheckInterfaceScope (
	//		int8* p,
	//		size_t ScopeLevel,
	//		size_t DstScopeLevel
	//		);

	EStdFunc_CheckInterfaceScope,

	// void
	// jnc.OnInvalidInterface (
	//		int8* p,
	//		int Error
	//		);

	EStdFunc_OnInvalidInterface,

	// int8*
	// jnc.DynamicCastInterface (
	//		int8* p,
	//		int8* pType
	//		);

	EStdFunc_DynamicCastInterface,

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
	CModule* m_pModule;

	rtl::CStdListT <CFunction> m_FunctionList;
	rtl::CStdListT <CGlobalFunction> m_GlobalFunctionList;
	
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
		CNamespace* pNamespace,
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
	CompileFunctions ();

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
	CreateCreateSafePtrValidator ();

	CFunction*
	CreateCheckSafePtrRange ();

	CFunction*
	CreateCheckSafePtrScope ();

	CFunction*
	CreateOnInvalidSafePtr ();

	CFunction*
	CreateCheckInterfaceNull ();

	CFunction*
	CreateCheckInterfaceScope ();

	CFunction*
	CreateOnInvalidInterface ();

	CFunction*
	CreateDynamicCastInterface ();

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
};

//.............................................................................

} // namespace axl {
} // namespace jnc {

