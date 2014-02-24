// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LUA_STRINGTEMPLATE_H

#include "axl_lua_LuaState.h"
#include "axl_lex_Token.h"

namespace axl {
namespace lua {

//.............................................................................

class CStringTemplate
{
protected:
	struct TEmitContext
	{
		CStringTemplate* m_pThis;
		rtl::CString* m_pResult;
		const char* m_pTemplate;
	};

protected:
	lex::CLineCol m_LineCol;

public:
	CLuaState m_LuaState;
	
public:
	void
	Reset ();

	bool
	Process (
		rtl::CString* pResult,
		const rtl::CString& FilePath,
		const char* pTemplate,
		size_t Length = -1
		);

protected:
	bool
	ExtractLuaSource (
		rtl::CString* pLuaSource,
		const rtl::CString& FilePath,
		const char* pTemplate,
		size_t Length = -1
		);

	void
	CountLineCol (
		const char* p,
		size_t Length = -1
		);

	static
	int 
	GetLine_lua (lua_State* pLuaState);

	static
	int 
	GetCol_lua (lua_State* pLuaState);

	static
	int 
	Emit_lua (lua_State* pLuaState);

	static
	int 
	Passthrough_lua (lua_State* pLuaState);
};

//.............................................................................

} // namespace lua
} // namespace axl

