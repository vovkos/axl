// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"

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
	//		size_t ScopeLevel,
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
	// jnc.InitializeObject (
	//		int8* p,
	//		int8* pType,
	//		int Flags
	//		);

	EStdFunc_InitializeObject,

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
	rtl::CStdListT <CProperty> m_PropertyList;
	rtl::CStdListT <CGlobalFunction> m_GlobalFunctionList;
	rtl::CStdListT <CGlobalProperty> m_GlobalPropertyList;

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

	rtl::CIteratorT <CProperty>
	GetFirstProperty ()
	{
		return m_PropertyList.GetHead ();
	}

	rtl::CIteratorT <CGlobalFunction>
	GetFirstGlobalFunction ()
	{
		return m_GlobalFunctionList.GetHead ();
	}

	rtl::CIteratorT <CGlobalProperty>
	GetFirstGlobalProperty ()
	{
		return m_GlobalPropertyList.GetHead ();
	}

	CFunction*
	CreateFunction (
		const rtl::CString& Tag,
		CFunctionType* pType,
		rtl::CStdListT <CFunctionFormalArg>* pArgList = NULL
		);

	CProperty*
	CreateProperty (
		CPropertyType* pType,
		CFunction* pGetter,
		const CFunctionOverload& Setter
		);

	CProperty*
	CreateProperty (CPropertyType* pType);

	CGlobalFunction*
	CreateGlobalFunction (
		const rtl::CString& Name,
		CFunctionType* pType,
		rtl::CStdListT <CFunctionFormalArg>* pArgList
		);

	CGlobalProperty*
	CreateGlobalProperty (
		const rtl::CString& Name,
		CPropertyType* pType,
		CFunction* pGetter,
		const CFunctionOverload& Setter
		);

	CGlobalProperty*
	CreateGlobalProperty (
		const rtl::CString& Name,
		CProperty* pProperty
		);

	bool
	CompileFunctions ();

	CFunction*
	GetStdFunction (EStdFunc Func);

protected:
	// LLVM code support functions

	CFunction*
	GetCreateSafePtrValidator ();

	CFunction*
	GetCheckSafePtrRange ();

	CFunction*
	GetCheckSafePtrScope ();

	CFunction*
	GetOnInvalidSafePtr ();

	CFunction*
	GetCheckInterfaceNull ();

	CFunction*
	GetCheckInterfaceScope ();

	CFunction*
	GetOnInvalidInterface ();

	CFunction*
	GetDynamicCastInterface ();

	CFunction*
	GetInitializeObject ();

	CFunction*
	GetHeapAllocate ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
