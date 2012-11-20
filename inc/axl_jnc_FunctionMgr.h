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
	// jnc.sptr 
	// jnc.CreateSafePtr (
	//		int8* p,
	//		int8* pRegionBegin,
	//		size_t Size,
	//		size_t ScopeLevel
	//		);

	EStdFunc_CreateSafePtr,

	// void 
	// jnc.CheckSafePtrRange (
	//		jnc.sptr p,
	//		size_t Size,
	//		int Error
	//		);

	EStdFunc_CheckSafePtrRange,

	// void 
	// jnc.CheckSafePtrScope (
	//		jnc.sptr p,
	//		size_t ScopeLevel
	//		);

	EStdFunc_CheckSafePtrScope,

	// void
	// jnc.OnInvalidSafePtr (
	//		jnc.sptr pSrc,
	//		int Error
	//		);

	EStdFunc_OnInvalidSafePtr,

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
	GetCreateSafePtr ();

	CFunction*
	GetCheckSafePtrRange ();

	CFunction*
	GetCheckSafePtrScope ();

	CFunction*
	GetOnInvalidSafePtr ();

	CFunction*
	GetLoadDynamicPtr ();

	CFunction*
	GetStoreDynamicPtr ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
