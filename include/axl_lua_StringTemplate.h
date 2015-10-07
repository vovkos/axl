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

class StringTemplate
{
protected:
	struct EmitContext
	{
		StringTemplate* m_self;
		sl::String* m_output;
		const char* m_source;
	};

protected:
	lex::LineCol m_lineCol;

public:
	LuaState m_luaState;
	
public:
	void
	reset ();

	bool
	process (
		sl::String* output,
		const sl::String& filePath,
		const char* source,
		size_t length = -1
		);

protected:
	bool
	extractLuaSource (
		sl::String* luaSource,
		const sl::String& filePath,
		const char* source,
		size_t length = -1
		);

	void
	countLineCol (
		const char* p,
		size_t length = -1
		);

	static
	int 
	getLine_lua (lua_State* h);

	static
	int 
	getCol_lua (lua_State* h);

	static
	int 
	emit_lua (lua_State* h);

	static
	int 
	passthrough_lua (lua_State* h);
};

//.............................................................................

} // namespace lua
} // namespace axl

