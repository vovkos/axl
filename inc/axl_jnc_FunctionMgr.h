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

	CFunction* m_pCurrentFunction;

	rtl::CStdListT <CFunction> m_FunctionList;
	rtl::CStdListT <CProperty> m_PropertyList;
	rtl::CStdListT <CGlobalFunction> m_GlobalFunctionList;
	rtl::CStdListT <CGlobalProperty> m_GlobalPropertyList;

public:
	CFunctionMgr (CModule* pModule);

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
		rtl::CStdListT <CFunctionFormalArg>* pArgList
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
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
