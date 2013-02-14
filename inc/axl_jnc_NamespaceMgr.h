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
	CNamespace* m_pCurrentNamespace;
	CScope* m_pCurrentScope;
	rtl::CArrayT <CNamespace*> m_NamespaceStack;

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
		)
	{
		return OpenNamespaceImpl (Pos, Name, true);
	}

	CGlobalNamespace*
	OpenNamespace (
		const CToken::CPos& Pos,
		const CQualifiedName& Name
		);

	void
	OpenNamespace (CNamespace* pNamespace);

	void
	CloseNamespace ();

	CScope*
	OpenScope (
		const CToken::CPos& Pos,
		CBasicBlock* pBreakBlock = NULL,
		CBasicBlock* pContinueBlock = NULL
		);

	void
	CloseScope (const CToken::CPos& Pos);

	CScope*
	FindBreakScope (size_t Level);

	CScope*
	FindContinueScope (size_t Level);

protected:
	CGlobalNamespace*
	OpenNamespaceImpl (
		const CToken::CPos& Pos,
		const rtl::CString& Name,
		bool AddToStack
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
