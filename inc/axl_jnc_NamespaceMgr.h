// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

//.............................................................................

class CNamespaceMgr
{
protected:
	friend class CModule;
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

	void
	SetCurrentNamespace (CNamespace* pNamespace)
	{
		m_pCurrentNamespace = pNamespace ? pNamespace : &m_GlobalNamespace;
	}

	void
	SetGlobalNamespace ()
	{
		m_pCurrentNamespace = &m_GlobalNamespace;
	}

	rtl::CString
	CreateQualifiedName (const tchar_t* pName)
	{
		return m_pCurrentNamespace->CreateQualifiedName (pName);
	}

	CGlobalNamespace*
	CreateNamespace (const rtl::CString& Name);

	CGlobalNamespace*
	OpenNamespace (
		const CToken::CPos& Pos,
		const rtl::CString& Name
		);

	CGlobalNamespace*
	OpenNamespace (
		const CToken::CPos& Pos,
		const CQualifiedName& Name
		);

	void
	OpenNamespace (CNamespace* pNamespace);

	void
	CloseNamespace (size_t Count = 1);

	CScope*
	OpenScope (const CToken::CPos& Pos);

	void
	CloseScope (const CToken::CPos& Pos);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
