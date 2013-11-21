// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	friend class CModule;
	friend class CParser;
	friend class CFunctionMgr;

protected:
	CModule* m_pModule;

	CGlobalNamespace m_GlobalNamespace;
	rtl::CStdListT <CGlobalNamespace> m_NamespaceList;
	rtl::CStdListT <CScope> m_ScopeList;

	rtl::CArrayT <CNamespace*> m_NamespaceStack;
	CNamespace* m_pCurrentNamespace;
	CScope* m_pCurrentScope;

	intptr_t m_SourcePosLockCount;

public:
	CNamespaceMgr ();

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	bool
	AddStdItems ();

	void
	LockSourcePos ()
	{
		m_SourcePosLockCount++;
	}

	void
	UnlockSourcePos ()
	{
		m_SourcePosLockCount--;
	}

	void
	SetSourcePos (const CToken::CPos& Pos);

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

	CScope*
	OpenScope (const CToken::CPos& Pos);

	void
	CloseScope ();

	EAccess
	GetAccessKind (CNamespace* pNamespace);

	rtl::CString
	CreateQualifiedName (const char* pName)
	{
		return m_pCurrentNamespace->CreateQualifiedName (pName);
	}

	CGlobalNamespace*
	CreateGlobalNamespace (
		const rtl::CString& Name,
		CNamespace* pParentNamespace = NULL
		);

	CScope*
	FindBreakScope (size_t Level);

	CScope*
	FindContinueScope (size_t Level);

	CScope*
	FindCatchScope ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
