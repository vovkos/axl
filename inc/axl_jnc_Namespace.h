// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ModuleItem.h"

namespace axl {
namespace jnc {

class CType;
class CNamespace;
class CEnumType;
class CEnumMember;

//.............................................................................

class CQualifiedIdentifier
{
public:
	rtl::CString m_First;
	rtl::CBoxListT <rtl::CString> m_List;
};

//.............................................................................

class CName
{
protected:
	friend class CNamespace;

	rtl::CString m_Name;
	rtl::CString m_QualifiedName;

	CNamespace* m_pParentNamespace;

public:
	CName ()
	{
		m_pParentNamespace = NULL;
	}

	bool
	IsNamed ()
	{
		return !m_Name.IsEmpty ();
	}

	rtl::CString
	GetName ()
	{
		return m_Name;
	}

	rtl::CString 
	GetQualifiedName ();

	CNamespace* 
	GetParentNamespace ()
	{
		return m_pParentNamespace;
	}
};

//.............................................................................

class CAlias: 
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CNamespace;

	CModuleItem* m_pTarget;

public:
	CAlias ()
	{
		m_ItemKind = EModuleItem_Alias;
		m_pTarget = NULL;
	}

	CModuleItem*
	GetTarget ()
	{
		return m_pTarget;
	}
};

//.............................................................................

class CNamespace: public CName
{
protected:
	friend class CNamespaceMgr;

	rtl::CArrayT <CModuleItem*> m_ItemArray; 
	rtl::CHashTableMapT <const tchar_t*, CModuleItem*, rtl::CHashString, rtl::CCmpString> m_ItemMap; 
	rtl::CStdListT <CAlias> m_AliasList;

public:
	rtl::CString
	CreateQualifiedName (const tchar_t* pName);

	CModuleItem*
	FindItem (
		const tchar_t* pName,
		bool Traverse = false
		);

	template <typename T>
	bool
	AddItem (T* pItem)
	{
		return AddItem (pItem, pItem);
	}

	size_t 
	GetItemCount ()
	{
		return m_ItemArray.GetCount ();
	}

	CModuleItem*
	GetItem (size_t Index)
	{
		ASSERT (Index < m_ItemArray.GetCount ());
		return m_ItemArray [Index];
	}

	CAlias*
	CreateAlias (
		const rtl::CString& Name,
		CModuleItem* pTarget
		);

	bool
	ExposeEnumMembers (CEnumType* pMember);

protected:
	void
	Clear ();

	bool
	AddItem (
		CModuleItem* pItem,
		CName* pName
		);
};

//.............................................................................

class CScope: 
	public CModuleItem,
	public CNamespace,
	public rtl::TListLink
{
protected:
	friend class CNamespaceMgr;

	void* m_pBegin;
	void* m_pEnd;

	// local variable list for debugger

public:
	CScope ()
	{
		m_ItemKind = EModuleItem_Scope;
	}
};

//.............................................................................

class CGlobalNamespace: 
	public CModuleItem,
	public CNamespace,
	public rtl::TListLink
{
protected:
	friend class CNamespaceMgr;

public:
	CGlobalNamespace ()
	{
		m_ItemKind = EModuleItem_Namespace;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
