// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LUA_LUASTATE_H

#include "axl_rtl_Handle.h"
#include "axl_rtl_String.h"
#include "axl_err_Error.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace lua {

//.............................................................................

class LuaClose
{
public:
	void 
	operator () (lua_State* lua)
	{
		lua_close (lua);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class LuaState: public rtl::Handle <lua_State*, LuaClose>
{
public:
	LuaState ()
	{
		create ();
	}

	LuaState (lua_State* h)
	{
		attach (h);
	}

	bool
	create ();

	bool
	load (
		const char* name,
		const char* source,
		size_t length = -1
		);

#ifdef _DEBUG
	void
	trace ();
#endif

	void 
	clearStack ()
	{
		ASSERT (isOpen ());
		lua_settop (m_h, 0);
	}

	int 
	getTop ()
	{
		ASSERT (isOpen ());
		return lua_gettop (m_h);
	}

	int 
	getType (int index)
	{
		ASSERT (isOpen ());
		return lua_type (m_h, index);
	}

	const char*
	getTypeName (int type)
	{
		ASSERT (isOpen ());
		return lua_typename (m_h, type);
	}

	void 
	insert (int index)
	{
		ASSERT (isOpen ());
		lua_insert (m_h, index);
	}

	void 
	remove (int index)
	{
		ASSERT (isOpen ());
		lua_remove (m_h, index);
	}

	void 
	swap ()
	{
		insert (-2);
	}

	void
	pushNil ()
	{
		ASSERT (isOpen ());
		lua_pushnil (m_h);
	}

	void
	pushInteger (lua_Integer value)
	{
		ASSERT (isOpen ());
		lua_pushinteger (m_h, value);
	}

	void
	pushBoolean (bool value)
	{
		ASSERT (isOpen ());
		lua_pushboolean (m_h, value);
	}

	void
	pushString (
		const char* string,
		size_t length = -1
		)
	{
		ASSERT (isOpen ());

		if (!string)
			string = "";

		if (length != -1)
			lua_pushlstring (m_h, string, length);
		else
			lua_pushstring (m_h, string);
	}

	void
	pushClosure (
		lua_CFunction pf,
		size_t contextArgumentCount
		)
	{
		ASSERT (isOpen ());
		lua_pushcclosure (m_h, pf, contextArgumentCount);
	}	

	void
	pushFunction (lua_CFunction pf)
	{
		ASSERT (isOpen ());
		lua_pushcfunction (m_h, pf);
	}	

	void
	pushFunction (
		lua_CFunction pf,
		intptr_t context
		)
	{
		pushInteger (context);
		pushClosure (pf, 1);
	}

	void
	createTable (
		size_t elementCount = 0,
		size_t memberCount = 0
		)
	{
		ASSERT (isOpen ());
		lua_createtable (m_h, elementCount, memberCount);
	}

	void
	getTable (int index = -2)
	{
		ASSERT (isOpen ());
		lua_gettable (m_h, index);
	}

	void
	setTable (int index = -3)
	{
		ASSERT (isOpen ());
		lua_settable (m_h, index);
	}

	void
	getMember (const char* name)
	{
		pushString (name);
		getTable ();
	}

	void
	setMember (const char* name)
	{
		pushString (name);
		swap ();
		setTable ();
	}

	void
	getArrayElement (int index)
	{
		pushInteger (index);
		getTable ();
	}

	void
	setArrayElement (int index)
	{
		pushInteger (index);
		swap ();
		setTable ();
	}

	void
	setMemberInteger (
		const char* name,
		lua_Integer value
		)
	{
		pushString (name);
		pushInteger (value);
		setTable ();
	}

	void
	setMemberBoolean (
		const char* name,
		bool value
		)
	{
		pushString (name);
		pushBoolean (value);
		setTable ();
	}

	void
	setMemberString (
		const char* name,
		const char* string,
		size_t length = -1
		)
	{
		pushString (name);
		pushString (string, length);
		setTable ();
	}

	void
	setArrayElementInteger (
		int index,
		lua_Integer value
		)
	{
		pushInteger (index);
		pushInteger (value);
		setTable ();
	}

	void
	setArrayElementString (
		int index,
		const char* string,
		size_t length = -1
		)
	{
		pushInteger (index);
		pushString (string, length);
		setTable ();
	}

	void
	pop (size_t count = 1)
	{
		ASSERT (isOpen ());
		lua_pop (m_h, count);
	}

	lua_Integer
	getInteger (int index = -1)
	{
		ASSERT (isOpen ());
		return lua_tointeger (m_h, index);
	}

	lua_Number
	getNumber (int index = -1)
	{
		ASSERT (isOpen ());
		return lua_tonumber (m_h, index);
	}

	bool
	getBoolean (int index = -1)
	{
		ASSERT (isOpen ());
		return lua_toboolean (m_h, index) != 0;
	}

	const char*
	getString (int index = -1)
	{
		ASSERT (isOpen ());
		return lua_tostring (m_h, index);
	}

	lua_Integer
	popInteger ()
	{
		lua_Integer value = getInteger (-1);
		pop ();
		return value;
	}

	bool
	popBoolean ()
	{
		bool value = getBoolean (-1);
		pop ();
		return value;
	}

	rtl::String
	popString ()
	{
		rtl::String string = getString (-1);
		pop ();
		return string;
	}

	void
	getGlobal (const char* name)
	{
		ASSERT (isOpen ());
		lua_getglobal (m_h, name);
	}

	void
	setGlobal (const char* name)
	{
		ASSERT (isOpen ());
		lua_setglobal (m_h, name);
	}

	void
	getGlobalArrayElement (
		const char* name,
		int index
		)
	{
		getGlobal (name);
		getArrayElement (index);
		remove (-2);
	}

	void
	getGlobalMember (
		const char* name,
		const char* member
		)
	{
		getGlobal (name);
		getMember (member);
		remove (-2);
	}


	void
	setGlobalInteger (
		const char* name,
		lua_Integer value
		)
	{
		pushInteger (value);
		setGlobal (name);
	}

	void
	setGlobalBoolean (
		const char* name,
		bool value
		)
	{
		pushBoolean (value);
		setGlobal (name);
	}

	void
	setGlobalString (
		const char* name,
		const char* string,
		size_t length = -1
		)
	{
		pushString (string, length);
		setGlobal (name);
	}

	void
	registerFunction (
		const char* name,
		lua_CFunction pf
		)
	{
		pushFunction (pf);
		setGlobal (name);
	}

	void
	registerFunction (
		const char* name,
		lua_CFunction pf,
		intptr_t context
		)
	{
		pushFunction (pf, context);
		setGlobal (name);
	}

	intptr_t
	getContext ()
	{
		int index = getUpValueIndex ();
		return getInteger (index);
	}

	int
	getUpValueIndex (int index = 1)
	{
		ASSERT (isOpen ());
		return lua_upvalueindex (index);
	}

	void
	call (
		size_t argumentCount,
		size_t resultCount = LUA_MULTRET
		)
	{
		ASSERT (isOpen ());
		lua_call (m_h, argumentCount, resultCount);
	}

	bool
	PCall (
		size_t argumentCount,
		size_t resultCount = LUA_MULTRET
		)
	{
		ASSERT (isOpen ());
		return complete (lua_pcall (m_h, argumentCount, resultCount, 0));
	}

protected:
	bool
	complete (int result);
};

//.............................................................................

} // namespace lua {
} // namespace axl {
