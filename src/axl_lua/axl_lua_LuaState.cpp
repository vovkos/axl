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

//..............................................................................

#if (LUA_VERSION_NUM < 502)

void
lua_len_impl(
	lua_State* state,
	int index
) {
	int type = lua_type(state, index);
	switch (type) {
	case LUA_TSTRING:
		lua_pushinteger(state, lua_objlen(state, index));
		break;

	case LUA_TTABLE:
		if (!luaL_callmeta(state, index, "__len"))
			lua_pushinteger(state, lua_objlen(state, index));
		break;

	case LUA_TUSERDATA:
		if (luaL_callmeta(state, index, "__len"))
			break;

		// fallthrough

	default:
		luaL_error(state, "invalid lua_len on %s", lua_typename(state, type));
	}
}

#endif

namespace axl {
namespace lua {

//..............................................................................

bool
LuaState::create() {
	close();

	m_h = luaL_newstate();
	if (!m_h) {
		err::setError(err::SystemErrorCode_InsufficientResources);
		return false;
	}

	return true;
}

bool
LuaState::complete(int result) {
	ASSERT(isOpen());

	if (result == LUA_OK)
		return true;

	err::setError(popString());
	return false;
}

bool
LuaState::load(
	const sl::StringRef& name,
	const sl::StringRef& source
) {
	ASSERT(isOpen());

	int result = luaL_loadbuffer(m_h, source.cp(), source.getLength(), name.sz());
	return complete(result);
}

bool
LuaState::loadFile(const sl::StringRef& fileName) {
	ASSERT(isOpen());

	int result = luaL_loadfile(m_h, fileName.cp());
	return complete(result);
}

bool
LuaState::doFile(const sl::StringRef& fileName) {
	ASSERT(isOpen());

	int result = luaL_dofile(m_h, fileName.cp());
	return complete(result);
}

#ifdef _AXL_DEBUG
void
LuaState::trace() {
	int top = getTop();

	for (int i = 1; i <= top; i++) {
		int type = getType(i);
		const void* p = lua_topointer(m_h, i);

		switch (type) {
		case LUA_TSTRING:
			TRACE("%08x %s\n", p, getString(i).sz());
			break;

		case LUA_TBOOLEAN:
			TRACE("%08x %s\n", p, getBoolean(i) ? "true" : "false");
			break;

		case LUA_TNUMBER:
			TRACE("%08x %g\n", p, getNumber(i));
			break;

		default:
			TRACE("%08x %s\n", p, getTypeName(type).sz());
			break;

		}
	}
}
#endif

void
LuaState::prepareErrorString(const sl::StringRef& string) {
	ASSERT(isOpen());

	int result = lua_checkstack(m_h, 2);
	ASSERT(result);

	where();
	pushString(string);
	concatenate();
}

bool
LuaState::tryCheckStack(int extraSlotCount) {
	ASSERT(isOpen());

	int result = lua_checkstack(m_h, extraSlotCount);
	if (!result) {
		err::setError(err::SystemErrorCode_InsufficientResources);
		return false;
	}

	return true;
}

void
LuaState::checkStack(int extraSlotCount) {
	bool result = tryCheckStack(extraSlotCount);
	if (!result) {
		prepareLastErrorString();
		error();
	}
}

bool
LuaState::tryCreateTable(
	size_t elementCount,
	size_t memberCount,
	size_t extraStackSlotCount
) {
	bool result = tryCheckStack(extraStackSlotCount);
	if (!result)
		return false;

	lua_createtable(m_h, elementCount, memberCount);
	return true;
}

void
LuaState::createTable(
	size_t elementCount,
	size_t memberCount,
	size_t extraStackSlotCount
) {
	bool result = tryCreateTable(elementCount, memberCount, extraStackSlotCount);
	if (!result) {
		prepareLastErrorString();
		error();
	}
}

int
LuaState::getGlobalArrayLen(const sl::StringRef& name) {
	getGlobal(name);
	size_t len = getRawLen();
	pop();
	return len;
}

void
LuaState::setGlobalArrayElement(
	const sl::StringRef& name,
	int index
) {
	getGlobal(name);
	swap();
	setArrayElement(index);
	pop();
}

void
LuaState::setGlobalMember(
	const sl::StringRef& name,
	const sl::StringRef& member
) {
	getGlobal(name);
	swap();
	setMember(member);
	pop();
}



//..............................................................................

} // namespace lua
} // namespace axl
