// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Lexer.h"

namespace axl {
namespace jnc {

class CModule;
class CAttributeBlock;

//.............................................................................

enum EModuleItem
{
	EModuleItem_Undefined = 0,
	EModuleItem_Namespace,	
	EModuleItem_Scope,	
	EModuleItem_Alias,
	EModuleItem_Type,
	EModuleItem_Const,
	EModuleItem_Variable,
	EModuleItem_Function,
	EModuleItem_Property,
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
	EStorage_Static,
	EStorage_Local,
	EStorage_Member,
	EStorage_Mutable,
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

	EStorage m_StorageKind;
	EAccess m_AccessKind;
	CToken::CPos m_Pos;
	CAttributeBlock* m_pAttributeBlock;

public:
	CModuleItemDecl ()
	{
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
		m_pAttributeBlock = NULL;
	}

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

	CAttributeBlock* 
	GetAttributeBlock ()
	{
		return m_pAttributeBlock;
	}
};

//.............................................................................

class CModuleItemName
{
protected:
	friend class CNamespace;

	rtl::CString m_Name;
	rtl::CString m_QualifiedName;

public:
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
	CModuleItem ()
	{
		m_pModule = NULL;
		m_ItemKind = EModuleItem_Undefined;
		m_pItemDecl = NULL;
	}

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

class CDeclModuleItem:
	public CModuleItem,
	public CModuleItemDecl
{
public:
	CDeclModuleItem ()
	{
		m_pItemDecl = this;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CNamedModuleItem:
	public CDeclModuleItem,
	public CModuleItemName
{
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
