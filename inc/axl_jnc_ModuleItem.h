// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Lexer.h"

namespace axl {
namespace jnc {

//.............................................................................

class CModule;
class CAttributeSet;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EModuleItem
{
	EModuleItem_Undefined,

	EModuleItem_Namespace,	
	EModuleItem_Scope,	
	EModuleItem_Alias,
	EModuleItem_Type,
	EModuleItem_Const,
	EModuleItem_Variable,
	EModuleItem_Function,
	EModuleItem_Property,
	EModuleItem_GlobalFunction,
	EModuleItem_GlobalProperty,
	EModuleItem_EnumMember,
	EModuleItem_StructMember,
	EModuleItem_ClassMember,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CModuleItem
{
protected:
	friend class CParser;
	friend class CAttributeMgr;

	EModuleItem m_ItemKind;
	CModule* m_pModule;
	CToken::CPos m_Pos;
	CAttributeSet* m_pAttributeSet;

public:
	CModuleItem ();

	EModuleItem 
	GetItemKind ()
	{	
		return m_ItemKind;
	}

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	const CToken::CPos&
	GetPos ()
	{	
		return m_Pos;
	}

	CAttributeSet* 
	GetAttributeSet ()
	{
		return m_pAttributeSet;
	}

	static
	const tchar_t*
	GetItemKindString (EModuleItem ItemKind);

	const tchar_t*
	GetItemKindString ()
	{
		return GetItemKindString (m_ItemKind);
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
