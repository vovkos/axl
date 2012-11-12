// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"

namespace axl {
namespace jnc {

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

	CFunction* m_pCreateSafePtr;
	CFunction* m_pCheckSafePtr;
	CFunction* m_pOnInvalidSafePtr;
	CFunction* m_pLoadDynamicPtr;
	CFunction* m_pStoreDynamicPtr;

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

	// LLVM code support functions

	// jnc.ptr3 
	// jnc.CreateSafePtr (
	//		int8* p1,
	//		int8* pRegionBegin,
	//		int8* pRegionEnd
	//		);

	CFunction*
	GetCreateSafePtr();

	// void 
	// jnc.CheckSafePtr (
	//		jnc.ptr3 p,
	//		size_t Size
	//		);

	CFunction*
	GetCheckSafePtr ();

	// void
	// jnc.OnInvalidSafePtr (int8* unsafe pSrc);

	CFunction*
	GetOnInvalidSafePtr ();

	// jnc.ptr3 
	// jnc.LoadDynamicPtr (jnc.ptr2);

	CFunction*
	GetLoadDynamicPtr ();

	// void
	// jnc.StoreDynamicPtr (
	//		int8* unsafe pSrc
	//		int8* unsafe pSrcType
	//		jnc.ptr2 pDst,
	//		);

	CFunction*
	GetStoreDynamicPtr ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
