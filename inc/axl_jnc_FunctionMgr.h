// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

class CFunctionMgr
{
protected:
	rtl::CStdListT <CFunction> m_FunctionList;
	rtl::CStdListT <CProperty> m_PropertyList;
	rtl::CStdListT <CGlobalFunction> m_GlobalFunctionList;
	rtl::CStdListT <CGlobalProperty> m_GlobalPropertyList;

public:
	void
	Clear ();

	CFunction*
	CreateFunction (
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

	CGlobalFunction*
	CreateGlobalFunction (
		const rtl::CString& Name,
		CFunction* pFunction
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
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
