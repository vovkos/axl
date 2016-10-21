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

#include "pch.h"
#include "axl_lua_LuaState.h"

namespace axl {
namespace lua {

//..............................................................................

bool
LuaState::create ()
{
	close ();

	m_h = luaL_newstate ();
	if (!m_h)
	{
		err::setError (err::SystemErrorCode_InsufficientResources);
		return false;
	}

	return true;
}

bool
LuaState::complete (int result)
{
	ASSERT (isOpen ());

	if (result == LUA_OK)
		return true;

	err::setError (popString ());
	return false;
}

bool
LuaState::load (
	const sl::StringRef& name,
	const sl::StringRef& source
	)
{
	ASSERT (isOpen ());

	int result = luaL_loadbuffer (m_h, source.cp (), source.getLength (), name.sz ());
	return complete (result);
}

#ifdef _AXL_DEBUG
void
LuaState::trace ()
{
	int top = getTop ();

	for (int i = 1; i <= top; i++)
	{
		int type = getType (i);
		const void* p = lua_topointer (m_h, i);

		switch (type)
		{
		case LUA_TSTRING:
			TRACE ("%08x %s\n", p, getString (i).sz ());
			break;

		case LUA_TBOOLEAN:
			TRACE ("%08x %s\n", p, getBoolean (i) ? "true" : "false");
			break;

		case LUA_TNUMBER:
			TRACE ("%08x %g\n", p, getNumber (i));
			break;

		default:
			TRACE ("%08x %s\n", p, getTypeName (type).sz ());
			break;

		}
	}
}
#endif

void
LuaState::error (const sl::StringRef& string)
{
	where ();
	pushString (string);
	concatenate ();
	error ();
}

void
LuaState::formatError_va (
	const char* format,
	axl_va_list va
	)
{
	where ();
	pushFormatString_va (format, va);
	concatenate ();
	error ();
}

void
LuaState::pushString (const sl::StringRef& string)
{
	ASSERT (isOpen ());
	lua_pushlstring (m_h, string.cp (), string.getLength ());
}

//..............................................................................

} // namespace lua
} // namespace axl
