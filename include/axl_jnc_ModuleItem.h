// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	EModuleItem_FunctionArg,
	EModuleItem_Function,
	EModuleItem_Property,
	EModuleItem_PropertyTemplate,
	EModuleItem_EnumConst,
	EModuleItem_StructField,
	EModuleItem_BaseTypeSlot,
	EModuleItem__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetModuleItemKindString (EModuleItem ItemKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EModuleItemFlag
{
	EModuleItemFlag_User         = 0x01,
	EModuleItemFlag_Orphan       = 0x02,
	EModuleItemFlag_NeedLayout   = 0x04,
	EModuleItemFlag_NeedCompile  = 0x08,
	EModuleItemFlag_InCalcLayout = 0x10,
	EModuleItemFlag_LayoutReady  = 0x20,
	EModuleItemFlag_Constructed  = 0x40, // fields, properties, base type slots	
};


//.............................................................................

enum EStorage
{
	EStorage_Undefined = 0,
	EStorage_Typedef,
	EStorage_Alias,
	EStorage_Static,
	EStorage_Thread,
	EStorage_Stack,
	EStorage_Heap,
	EStorage_HeapU,
	EStorage_Member,
	EStorage_Abstract,
	EStorage_Virtual,
	EStorage_Override,
	EStorage_Mutable,
	EStorage_Nullable,
	EStorage_This,
	EStorage__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
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

const char*
GetAccessKindString (EAccess AccessKind);

//.............................................................................

class CModuleItemDecl
{
	friend class CParser;
	friend class CNamespace;

protected:
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

	const CToken::CPos*
	GetPos ()
	{	
		return &m_Pos;
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
	friend class CModule;
	friend class CParser;

protected:
	CModule* m_pModule;
	EModuleItem m_ItemKind;
	uint_t m_Flags;
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

	uint_t
	GetFlags ()
	{
		return m_Flags;
	}

	CModuleItemDecl*
	GetItemDecl ()
	{
		return m_pItemDecl;
	}

	bool
	EnsureLayout ();

	virtual
	bool 
	Compile ()
	{
		ASSERT (false);
		return true;
	}

protected:
	virtual
	bool 
	CalcLayout ()
	{
		ASSERT (false);
		return true;
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
