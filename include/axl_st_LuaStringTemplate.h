// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ST_LUASTRINGTEMPLATE_H

#include "axl_st_StringTemplate.h"

namespace axl {
namespace st {

//.............................................................................

class LuaStringTemplate: public StringTemplate <LuaStringTemplate>
{
	friend class StringTemplate <LuaStringTemplate>;

public:
	lua::LuaState m_luaState;
	
public:
	void
	close ()
	{
		m_luaState.close ();
		StringTemplate <LuaStringTemplate>::clear ();
	}

	bool
	create ();

protected:
	bool 
	runScript (
		const sl::StringRef& fileName,
		const sl::StringRef& source
		);

	void
	createPassthroughCall (
		sl::String* script,
		size_t offset,
		size_t length	
		)
	{
		script->appendFormat ("passthrough (%d, %d);", offset, length);
	}

	void
	createEmitCall (
		sl::String* script,
		const sl::StringRef& value
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

} // namespace st
} // namespace axl

