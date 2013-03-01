// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Namespace.h"
#include "axl_jnc_Scope.h"

namespace axl {
namespace jnc {

class CModule;
class CClassType;

//.............................................................................

class CNamespaceMgr
{
protected:
	friend class CModule;
	friend class CParser;

	CModule* m_pModule;

	CGlobalNamespace m_GlobalNamespace;
	rtl::CStdListT <CGlobalNamespace> m_NamespaceList;
	rtl::CStdListT <CScope> m_ScopeList;
	rtl::CArrayT <CNamespace*> m_NamespaceStack;
	CNamespace* m_pCurrentNamespace;
	CScope* m_pCurrentScope;

public:
	CNamespaceMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	CGlobalNamespace*
	GetGlobalNamespace ()
	{
		return &m_GlobalNamespace;
	}
	
	CNamespace* 
	GetCurrentNamespace ()
	{
		return m_pCurrentNamespace;
	}

	CScope* 
	GetCurrentScope ()
	{
		return m_pCurrentScope;
	}

	void
	OpenNamespace (CNamespace* pNamespace);

	void
	CloseNamespace ();

	EAccess
	GetAccessKind (CNamespace* pNamespace);

	rtl::CString
	CreateQualifiedName (const tchar_t* pName)
	{
		return m_pCurrentNamespace->CreateQualifiedName (pName);
	}

	CGlobalNamespace*
	CreateGlobalNamespace (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName
		);

	CScope*
	CreateScope ();

	CScope*
	FindBreakScope (size_t Level);

	CScope*
	FindContinueScope (size_t Level);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

