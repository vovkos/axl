// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_pg_DefineMgr.h"

namespace axl {
namespace pg {

//.............................................................................

enum EClassFlag
{
	EClassFlag_IsDefault   = 0x01,
	EClassFlag_IsNamed     = 0x02,
	EClassFlag_IsDefined   = 0x04,
	EClassFlag_IsUsed      = 0x08,
	EClassFlag_IsReachable = 0x10,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClass: public rtl::TListLink
{
public:
	int m_Flags;

	lex::CSrcPos m_SrcPos;

	CClass* m_pBaseClass;

	rtl::CString m_Name;
	rtl::CString m_Members;

	CClass ()
	{
		m_Flags = 0;
		m_pBaseClass = NULL;
	}

	void Export (lua::CLuaState* pLuaState);
};

//.............................................................................

class CClassMgr
{
protected:
	rtl::CStdListT <CClass> m_ClassList;
	rtl::CStringHashTableMapT <CClass*> m_ClassMap; 

public:
	bool
	IsEmpty ()
	{
		return m_ClassList.IsEmpty ();
	}
	
	size_t
	GetCount ()
	{
		return m_ClassList.GetCount ();
	}

	rtl::CIteratorT <CClass>
	GetHead ()
	{
		return m_ClassList.GetHead ();
	}

	void
	Clear ()
	{
		m_ClassList.Clear ();
		m_ClassMap.Clear ();
	}

	CClass*
	GetClass (const rtl::CString& Name);

	CClass*
	CreateUnnamedClass ();
	
	CClass*
	FindClass (const tchar_t* pName)
	{
		rtl::CStringHashTableMapIteratorT <CClass*> It = m_ClassMap.Find (pName);
		return It ? It->m_Value : NULL;
	}

	void
	DeleteClass (CClass* pClass);

	bool
	Verify ();

	void
	DeleteUnusedClasses ();

	void
	DeleteUnreachableClasses ();
};

//.............................................................................

} // namespace pg {
} // namespace axl {
