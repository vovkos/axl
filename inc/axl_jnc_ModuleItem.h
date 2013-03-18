// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Lexer.h"

namespace axl {
namespace jnc {

class CModule;
class CNamespace;
class CAttributeBlock;

//.............................................................................

enum EModuleItem
{
	EModuleItem_Undefined = 0,
	EModuleItem_Namespace,	
	EModuleItem_Scope,	
	EModuleItem_Type,
	EModuleItem_Typedef,
	EModuleItem_Alias,
	EModuleItem_Const,
	EModuleItem_Variable,
	EModuleItem_Function,
	EModuleItem_Property,
	EModuleItem_PropertyTemplate,
	EModuleItem_AutoEv,
	EModuleItem_EnumConst,
	EModuleItem_StructField,
	EModuleItem__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetModuleItemKindString (EModuleItem ItemKind);

//.............................................................................

enum EStorage
{
	EStorage_Undefined = 0,
	EStorage_Typedef,
	EStorage_Alias,
	EStorage_Static,
	EStorage_Tls,
	EStorage_Stack,
	EStorage_Heap,
	EStorage_UHeap,
	EStorage_Member,
	EStorage_Abstract,
	EStorage_Virtual,
	EStorage_Override,
	EStorage__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetStorageKindString (EStorage StorageKind);

//.............................................................................

enum EAccess
{
	EAccess_Undefined = 0,
	EAccess_Public,
	EAccess_Protected,
	EAccess__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetAccessKindString (EAccess AccessKind);

//.............................................................................

class CModuleItemDecl
{
protected:
	friend class CParser;
	friend class CNamespace;

	EStorage m_StorageKind;
	EAccess m_AccessKind;
	CToken::CPos m_Pos;
	rtl::CString m_Name;
	rtl::CString m_QualifiedName;
	CNamespace* m_pParentNamespace;
	CAttributeBlock* m_pAttributeBlock;

public:
	CModuleItemDecl ();

	EStorage
	GetStorageKind ()
	{
		return m_StorageKind;
	}

	EAccess
	GetAccessKind ()
	{
		return m_AccessKind;
	}

	const CToken::CPos&
	GetPos ()
	{	
		return m_Pos;
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
	GetQualifiedName ()
	{
		return m_QualifiedName;
	}

	CNamespace* 
	GetParentNamespace ()
	{
		return m_pParentNamespace;
	}

	CAttributeBlock* 
	GetAttributeBlock ()
	{
		return m_pAttributeBlock;
	}
};

//.............................................................................

class CModuleItem: public rtl::TListLink
{
protected:
	CModule* m_pModule;
	EModuleItem m_ItemKind;
	CModuleItemDecl* m_pItemDecl;

public:
	rtl::CString m_Tag;

public:
	CModuleItem ();

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	EModuleItem 
	GetItemKind ()
	{	
		return m_ItemKind;
	}

	CModuleItemDecl*
	GetItemDecl ()
	{
		return m_pItemDecl;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUserModuleItem:
	public CModuleItem,
	public CModuleItemDecl
{
public:
	CUserModuleItem ()
	{
		m_pItemDecl = this;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
