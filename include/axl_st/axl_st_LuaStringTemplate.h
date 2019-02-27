//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_ST_LUASTRINGTEMPLATE_H

#include "axl_st_StringTemplate.h"

namespace axl {
namespace st {

//..............................................................................

class LuaStringTemplate: public StringTemplate<LuaStringTemplate>
{
	friend class StringTemplate<LuaStringTemplate>;

protected:
	size_t m_argCount;

public:
	lua::LuaState m_luaState;

public:
	LuaStringTemplate()
	{
		m_argCount = 0;
	}

	void
	close();

	bool
	create();

	size_t
	getArgCount()
	{
		return m_argCount;
	}

	bool
	setArgCount(size_t count);

protected:
	bool
	runScript(
		const sl::StringRef& fileName,
		const sl::StringRef& source
		);

	void
	createPassthroughCall(
		sl::String* script,
		size_t offset,
		size_t length
		)
	{
		script->appendFormat("passthrough (%d, %d);", offset, length);
	}

	void
	createEmitCall(
		sl::String* script,
		const sl::StringRef& value
		);

	static
	int
	getLine_lua(lua_State* h);

	static
	int
	getCol_lua(lua_State* h);

	static
	int
	emit_lua(lua_State* h);

	static
	int
	passthrough_lua(lua_State* h);
};

//..............................................................................

} // namespace st
} // namespace axl
