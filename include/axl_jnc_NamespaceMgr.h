// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Namespace.h"
#include "axl_jnc_Scope.h"
#include "axl_jnc_Orphan.h"

namespace axl {
namespace jnc {

class CModule;
class CClassType;

//.............................................................................

class CScopeLevelObjHdrStack
{
	friend class CNamespaceMgr;

protected:
	CModule* m_pModule;

	rtl::CBoxListT <CValue> m_ValueList;
	rtl::CArrayT <CValue*> m_ValueArray;

public:
	CScopeLevelObjHdrStack ()
	{
		m_pModule = NULL;
	}

	void
	Clear ()
	{
		m_ValueList.Clear ();
		m_ValueArray.Clear ();
	}

	void
	TakeOver (CScopeLevelObjHdrStack* pSrcStack);

	CValue
	GetScopeLevelObjHdr (size_t ScopeLevel);
};

//.............................................................................

class CNamespaceMgr
{
	friend class CModule;
	friend class CParser;
	friend class CFunctionMgr;

protected:
	struct TNamespaceStackEntry
	{
		CNamespace* m_pNamespace;
		EAccess m_AccessKind;
	};

protected:
	CModule* m_pModule;

	CGlobalNamespace m_GlobalNamespace;
	rtl::CStdListT <CGlobalNamespace> m_NamespaceList;
	rtl::CStdListT <CScope> m_ScopeList;
	rtl::CStdListT <COrphan> m_OrphanList;

	rtl::CArrayT <TNamespaceStackEntry> m_NamespaceStack;

	CNamespace* m_pCurrentNamespace;
	CScope* m_pCurrentScope;
	EAccess m_CurrentAccessKind;

	intptr_t m_SourcePosLockCount;

	CValue m_StaticObjectValue;
	CScopeLevelObjHdrStack m_ScopeLevelObjHdrStack;

public:
	CNamespaceMgr ();
	
	~CNamespaceMgr ()
	{
		Clear ();
	}

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	bool
	AddStdItems ();

	COrphan*
	CreateOrphan (
		EOrphan OrphanKind,
		CFunctionType* pFunctionType
		);

	bool
	ResolveOrphans ();

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

	EAccess
	GetCurrentAccessKind ()
	{
		return m_CurrentAccessKind;
	}

	CValue
	GetCurrentScopeObjHdr ()
	{
		return GetScopeLevelObjHdr (m_pCurrentScope);
	}

	CValue
	GetScopeLevelObjHdr (CScope* pScope)
	{
		return pScope ? m_ScopeLevelObjHdrStack.GetScopeLevelObjHdr (pScope->GetLevel ()) : GetStaticObjHdr ();
	}

	CValue
	GetScopeLevelObjHdr (size_t ScopeLevel)
	{
		return ScopeLevel ? m_ScopeLevelObjHdrStack.GetScopeLevelObjHdr (ScopeLevel) : GetStaticObjHdr ();
	}
	
	CValue
	GetStaticObjHdr ();

	void
	OpenNamespace (CNamespace* pNamespace);

	void
	CloseNamespace ();

	CScope*
	OpenInternalScope ();

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
