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

#define _AXL_LUA_LUASTATE_H

#include "axl_lua_Pch.h"

namespace axl {
namespace lua {

//..............................................................................

class LuaClose
{
public:
	void
	operator () (lua_State* lua)
	{
		lua_close(lua);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class LuaState: public sl::Handle<lua_State*, LuaClose>
{
public:
	LuaState()
	{
	}

	LuaState(lua_State* h)
	{
		attach(h);
	}

	bool
	create();

	bool
	load(
		const sl::StringRef& name,
		const sl::StringRef& source
		);

#ifdef _AXL_DEBUG
	void
	trace();
#endif

	void
	openLibs()
	{
		luaL_openlibs(m_h);
	}

	void
	where(int level = 1)
	{
		ASSERT(isOpen());
		luaL_where(m_h, level);
	}

	void
	prepareErrorString(const sl::StringRef& string);

	void
	prepareLastErrorString()
	{
		prepareErrorString(err::getLastErrorDescription());
	}

	void
	error()
	{
		ASSERT(isOpen());
		lua_error(m_h);
	}

	void
	clearStack()
	{
		ASSERT(isOpen());
		lua_settop(m_h, 0);
	}

	int
	getTop()
	{
		ASSERT(isOpen());
		return lua_gettop(m_h);
	}

	void
	setTop(int index)
	{
		ASSERT(isOpen());
		lua_settop(m_h, index);
	}

	bool
	tryCheckStack(int extraSlotCount);

	void
	checkStack(int extraSlotCount);

	int
	getType(int index)
	{
		ASSERT(isOpen());
		return lua_type(m_h, index);
	}

	sl::StringRef
	getTypeName(int type)
	{
		ASSERT(isOpen());
		return lua_typename(m_h, type);
	}

	void
	getLen(int index = -1)
	{
		ASSERT(isOpen());
		lua_len(m_h, index);
	}

	size_t
	getRawLen(int index = -1)
	{
		ASSERT(isOpen());
		return lua_rawlen(m_h, index);
	}

	void
	insert(int index)
	{
		ASSERT(isOpen());
		lua_insert(m_h, index);
	}

	void
	remove(int index)
	{
		ASSERT(isOpen());
		lua_remove(m_h, index);
	}

	void
	swap()
	{
		insert(-2);
	}

	void
	concatenate(int count = 2)
	{
		ASSERT(isOpen());
		lua_concat(m_h, count);
	}

	void
	pushNil()
	{
		ASSERT(isOpen());
		lua_pushnil(m_h);
	}

	void
	pushValue(int index = -1)
	{
		ASSERT(isOpen());
		lua_pushvalue(m_h, index);
	}

	void
	pushInteger(lua_Integer value)
	{
		ASSERT(isOpen());
		lua_pushinteger(m_h, value);
	}

	void
	pushBoolean(bool value)
	{
		ASSERT(isOpen());
		lua_pushboolean(m_h, value);
	}

	void
	pushString(const sl::StringRef& string)
	{
		ASSERT(isOpen());
		lua_pushlstring(m_h, string.cp(), string.getLength());
	}

	void
	pushFormatString_va(
		const char* format,
		axl_va_list va
		)
	{
		ASSERT(isOpen());
		lua_pushvfstring(m_h, format, va);
	}

	void
	pushFormatString(
		const char* format,
		...
		)
	{
		AXL_VA_DECL(va, format);
		pushFormatString_va(format, va);
	}

	void
	pushClosure(
		lua_CFunction func,
		size_t contextArgumentCount
		)
	{
		ASSERT(isOpen());
		lua_pushcclosure(m_h, func, contextArgumentCount);
	}

	void
	pushFunction(lua_CFunction func)
	{
		ASSERT(isOpen());
		lua_pushcfunction(m_h, func);
	}

	void
	pushFunction(
		lua_CFunction func,
		void* context
		)
	{
		ASSERT(isOpen());
		lua_pushlightuserdata(m_h, context);
		pushClosure(func, 1);
	}

	bool
	tryCreateTable(
		size_t elementCount = 0,
		size_t memberCount = 0,
		size_t extraStackSlotCount = LUA_MINSTACK
		);

	void
	createTable(
		size_t elementCount = 0,
		size_t memberCount = 0,
		size_t extraStackSlotCount = LUA_MINSTACK
		);

	void
	getTable(int index = -2)
	{
		ASSERT(isOpen());
		lua_gettable(m_h, index);
	}

	void
	setTable(int index = -3)
	{
		ASSERT(isOpen());
		lua_settable(m_h, index);
	}

	void
	getMember(const sl::StringRef& name)
	{
		pushString(name);
		getTable();
	}

	void
	setMember(const sl::StringRef& name)
	{
		pushString(name);
		swap();
		setTable();
	}

	void
	getArrayElement(int index)
	{
		pushInteger(index);
		getTable();
	}

	void
	setArrayElement(int index)
	{
		pushInteger(index);
		swap();
		setTable();
	}

	void
	setMemberInteger(
		const sl::StringRef& name,
		lua_Integer value
		)
	{
		pushString(name);
		pushInteger(value);
		setTable();
	}

	void
	setMemberBoolean(
		const sl::StringRef& name,
		bool value
		)
	{
		pushString(name);
		pushBoolean(value);
		setTable();
	}

	void
	setMemberString(
		const sl::StringRef& name,
		const sl::StringRef& string
		)
	{
		pushString(name);
		pushString(string);
		setTable();
	}

	void
	setArrayElementInteger(
		int index,
		lua_Integer value
		)
	{
		pushInteger(index);
		pushInteger(value);
		setTable();
	}

	void
	setArrayElementString(
		int index,
		const sl::StringRef& string
		)
	{
		pushInteger(index);
		pushString(string);
		setTable();
	}

	void
	pop(size_t count = 1)
	{
		ASSERT(isOpen());
		lua_pop(m_h, count);
	}

	lua_Integer
	getInteger(int index = -1)
	{
		ASSERT(isOpen());
		return lua_tointeger(m_h, index);
	}

	lua_Number
	getNumber(int index = -1)
	{
		ASSERT(isOpen());
		return lua_tonumber(m_h, index);
	}

	bool
	getBoolean(int index = -1)
	{
		ASSERT(isOpen());
		return lua_toboolean(m_h, index) != 0;
	}

	sl::StringRef
	getString(int index = -1)
	{
		ASSERT(isOpen());
		return lua_tostring(m_h, index);
	}

	lua_Integer
	popInteger()
	{
		lua_Integer value = getInteger(-1);
		pop();
		return value;
	}

	bool
	popBoolean()
	{
		bool value = getBoolean(-1);
		pop();
		return value;
	}

	sl::String
	popString()
	{
		sl::String string = getString(-1);
		pop();
		return string;
	}

	void
	getGlobal(const sl::StringRef& name)
	{
		ASSERT(isOpen());
		lua_getglobal(m_h, name.sz());
	}

	void
	setGlobal(const sl::StringRef& name)
	{
		ASSERT(isOpen());
		lua_setglobal(m_h, name.sz());
	}

	int
	getGlobalArrayLen(const sl::StringRef& name);

	void
	getGlobalArrayElement(
		const sl::StringRef& name,
		int index
		)
	{
		getGlobal(name);
		getArrayElement(index);
		remove(-2);
	}

	void
	setGlobalArrayElement(
		const sl::StringRef& name,
		int index
		);

	void
	getGlobalMember(
		const sl::StringRef& name,
		const sl::StringRef& member
		)
	{
		getGlobal(name);
		getMember(member);
		remove(-2);
	}

	void
	setGlobalMember(
		const sl::StringRef& name,
		const sl::StringRef& member
		);

	lua_Integer
	getGlobalInteger(const sl::StringRef& name)
	{
		getGlobal(name);
		return popInteger();
	}

	void
	setGlobalInteger(
		const sl::StringRef& name,
		lua_Integer value
		)
	{
		pushInteger(value);
		setGlobal(name);
	}

	bool
	getGlobalBoolean(const sl::StringRef& name)
	{
		getGlobal(name);
		return popBoolean();
	}

	void
	setGlobalBoolean(
		const sl::StringRef& name,
		bool value
		)
	{
		pushBoolean(value);
		setGlobal(name);
	}

	sl::String
	getGlobalString(const sl::StringRef& name)
	{
		getGlobal(name);
		return popString();
	}

	void
	setGlobalString(
		const sl::StringRef& name,
		const sl::StringRef& string
		)
	{
		pushString(string);
		setGlobal(name);
	}

	void
	registerFunction(
		const sl::StringRef& name,
		lua_CFunction func
		)
	{
		pushFunction(func);
		setGlobal(name);
	}

	void
	registerFunction(
		const sl::StringRef& name,
		lua_CFunction func,
		void* context
		)
	{
		pushFunction(func, context);
		setGlobal(name);
	}

	void*
	getContext()
	{
		int index = getUpValueIndex();
		return lua_touserdata(m_h, index);
	}

	int
	getUpValueIndex(int index = 1)
	{
		ASSERT(isOpen());
		return lua_upvalueindex(index);
	}

	void
	call(
		size_t argumentCount,
		size_t resultCount = LUA_MULTRET
		)
	{
		ASSERT(isOpen());
		lua_call(m_h, argumentCount, resultCount);
	}

	bool
	PCall(
		size_t argumentCount,
		size_t resultCount = LUA_MULTRET
		)
	{
		ASSERT(isOpen());
		return complete(lua_pcall(m_h, argumentCount, resultCount, 0));
	}

protected:
	bool
	complete(int result);
};

//..............................................................................

class LuaNonOwnerState: public LuaState
{
public:
	LuaNonOwnerState(lua_State* h)
	{
		attach(h);
	}

	~LuaNonOwnerState()
	{
		detach();
	}
};

//..............................................................................

} // namespace lua
} // namespace axl
