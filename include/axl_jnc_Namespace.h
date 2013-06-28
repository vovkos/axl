// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ModuleItem.h"
#include "axl_jnc_QualifiedName.h"
#include "axl_rtl_StringHashTable.h"

namespace axl {
namespace jnc {

class CNamespace;
class CEnumType;
class CEnumConst;
class CBaseTypeCoord;
class CFunction;

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

const char*
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

class CNamespace: public CModuleItemDecl
{
	friend class CNamespaceMgr;
	friend class CParser;

protected:
	ENamespace m_NamespaceKind;
	EAccess m_CurrentAccessKind;

	rtl::CArrayT <CModuleItem*> m_ItemArray; 
	rtl::CStringHashTableMapT <CModuleItem*> m_ItemMap; 
	rtl::CStringHashTable m_FriendSet; 

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

	rtl::CString
	CreateQualifiedName (const char* pName);

	rtl::CString
	CreateQualifiedName (const rtl::CString& Name)
	{
		return CreateQualifiedName (Name.cc ());
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
	FindItem (const char* pName);

	CModuleItem*
	FindItem (const rtl::CString& Name)
	{
		return FindItem (Name. cc ());
	}

	CModuleItem*
	FindItem (const CQualifiedName& Name);

	CModuleItem*
	FindItemTraverse (
		const rtl::CString& Name,
		CBaseTypeCoord* pCoord = NULL,
		uint_t Flags = 0
		)
	{
		return FindItemTraverseImpl (Name, pCoord, Flags);
	}

	CModuleItem*
	FindItemTraverse (
		const char* pName,
		CBaseTypeCoord* pCoord = NULL,
		uint_t Flags = 0
		)
	{
		return FindItemTraverseImpl (pName, pCoord, Flags);
	}

	CModuleItem*
	FindItemTraverse (
		const CQualifiedName& Name,
		CBaseTypeCoord* pCoord = NULL,
		uint_t Flags = 0
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

	bool
	ExposeEnumConsts (CEnumType* pMember);

protected:
	void
	Clear ();

	bool
	AddItem (
		CModuleItem* pItem,
		CModuleItemDecl* pDecl
		);

	virtual
	CModuleItem*
	FindItemTraverseImpl (
		const char* pName,
		CBaseTypeCoord* pCoord = NULL,
		uint_t Flags = 0
		);
};

//.............................................................................

class CGlobalNamespace: 
	public CModuleItem,
	public CNamespace
{
	friend class CNamespaceMgr;

public:
	CGlobalNamespace ()
	{
		m_ItemKind = EModuleItem_Namespace;
		m_NamespaceKind = ENamespace_Global;
		m_pItemDecl = this;
	}
};

//.............................................................................

inline
err::CError
SetRedefinitionError (const char* pName)
{
	return err::SetFormatStringError ("redefinition of '%s'", pName);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
