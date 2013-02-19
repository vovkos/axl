// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ModuleItem.h"
#include "axl_jnc_QualifiedName.h"

namespace axl {
namespace jnc {

class CNamespace;
class CEnumType;
class CEnumConst;
class CBaseTypeCoord;
class CFunction;

//.............................................................................

class CAlias: public CNamedModuleItem
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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CModuleItem*
UnAliasItem (CModuleItem* pItem);

//.............................................................................

enum ENamespace
{
	ENamespace_Undefined,
	ENamespace_Global,
	ENamespace_Scope,
	ENamespace_Type,
	ENamespace_TypeExtension,
	ENamespace_Property,
	ENamespace_PropertyTemplate,
	ENamespace__Count
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetNamespaceKindString (ENamespace NamespaceKind);

//.............................................................................

enum
{
	ETraverse_NoThis               = 0x01,
	ETraverse_NoExtensionNamespace = 0x02,
	ETraverse_NoBaseType           = 0x04,
	ETraverse_NoParentNamespace    = 0x08,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CNamespace: public CModuleItemName
{
protected:
	friend class CNamespaceMgr;
	friend class CParser;

	ENamespace m_NamespaceKind;
	EAccess m_CurrentAccessKind;
	CNamespace* m_pParentNamespace;	

	rtl::CArrayT <CModuleItem*> m_ItemArray; 
	rtl::CStringHashTableMapT <CModuleItem*> m_ItemMap; 
	rtl::CStringHashTable m_FriendSet; 
	rtl::CStdListT <CAlias> m_AliasList;

public:
	CNamespace ();

	ENamespace 
	GetNamespaceKind ()
	{
		return m_NamespaceKind;
	}

	EAccess 
	GetCurrentAccessKind ()
	{
		return m_CurrentAccessKind;
	}

	CNamespace* 
	GetParentNamespace ()
	{
		return m_pParentNamespace;
	}

	rtl::CString
	CreateQualifiedName (const tchar_t* pName);

	rtl::CString
	CreateQualifiedName (const rtl::CString& Name)
	{
		return CreateQualifiedName ((const tchar_t*) Name);
	}

	rtl::CString
	CreateQualifiedName (const CQualifiedName& Name)
	{
		return CreateQualifiedName (Name.GetFullName ());
	}

	bool
	IsFriend (CNamespace* pNamespace)
	{
		return m_FriendSet.Find (pNamespace->m_QualifiedName) != NULL;
	}

	CModuleItem*
	FindItem (const tchar_t* pName);

	CModuleItem*
	FindItem (const rtl::CString& Name)
	{
		return FindItem ((const tchar_t*) Name);
	}

	CModuleItem*
	FindItem (const CQualifiedName& Name);

	CModuleItem*
	FindItemTraverse (
		const rtl::CString& Name,
		CBaseTypeCoord* pCoord = NULL,
		int Flags = 0
		)
	{
		return FindItemTraverseImpl ((const tchar_t*) Name, pCoord, Flags);
	}

	CModuleItem*
	FindItemTraverse (
		const tchar_t* pName,
		CBaseTypeCoord* pCoord = NULL,
		int Flags = 0
		)
	{
		return FindItemTraverseImpl (pName, pCoord, Flags);
	}

	CModuleItem*
	FindItemTraverse (
		const CQualifiedName& Name,
		CBaseTypeCoord* pCoord = NULL,
		int Flags = 0
		);

	template <typename T>
	bool
	AddItem (T* pItem)
	{
		return AddItem (pItem, pItem);
	}

	bool
	AddFunction (CFunction* pFunction);

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
	ExposeEnumConsts (CEnumType* pMember);

protected:
	void
	Clear ();

	bool
	AddItem (
		CModuleItem* pItem,
		CModuleItemName* pName
		);

	virtual
	CModuleItem*
	FindItemTraverseImpl (
		const tchar_t* pName,
		CBaseTypeCoord* pCoord = NULL,
		int Flags = 0
		);
};

//.............................................................................

class CGlobalNamespace: 
	public CDeclModuleItem,
	public CNamespace
{
protected:
	friend class CNamespaceMgr;

public:
	CGlobalNamespace ()
	{
		m_ItemKind = EModuleItem_Namespace;
		m_NamespaceKind = ENamespace_Global;
	}
};

//.............................................................................

inline
err::CError
SetRedefinitionError (const tchar_t* pName)
{
	return err::SetFormatStringError (_T("redefinition of '%s'"), pName);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
