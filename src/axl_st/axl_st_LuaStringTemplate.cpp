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
#include "axl_st_LuaStringTemplate.h"
#include "axl_st_StringTemplateLexer.h"

namespace axl {
namespace st {

//..............................................................................

void
LuaStringTemplate::close ()
{
	m_luaState.close ();
	StringTemplate <LuaStringTemplate>::clear ();
	m_argCount = 0;
}

bool
LuaStringTemplate::create ()
{
	close ();

	bool result = m_luaState.create ();
	if (!result)
		return false;

	m_luaState.openLibs ();

	m_luaState.registerFunction ("getLine", getLine_lua, this);
	m_luaState.registerFunction ("getCol", getCol_lua, this);
	m_luaState.registerFunction ("emit", emit_lua, this);
	m_luaState.registerFunction ("passthrough", passthrough_lua, this);

	return true;
}

bool
LuaStringTemplate::setArgCount (size_t count)
{
	int top = m_luaState.getTop ();
	if (count > (size_t) top)
	{
		err::setError (err::SystemErrorCode_InvalidParameter);
		return false;
	}

	m_argCount = count;
	return true;
}

bool
LuaStringTemplate::runScript (
	const sl::StringRef& fileName,
	const sl::StringRef& source
	)
{
	int top = m_luaState.getTop ();

	bool result = m_luaState.load (fileName, source);
	if (!result)
		return false;

	if (m_argCount)
	{
		ASSERT (m_argCount <= (size_t) top);

		for (int i = top - m_argCount + 1; i <= top; i++)
			m_luaState.pushValue (i);
	}

	result = m_luaState.PCall (m_argCount, 0);

	m_argCount = 0;

	// clean up even on failure

	m_luaState.setTop (top);

	return result;
}

void
LuaStringTemplate::createEmitCall (
	sl::String* script,
	const sl::StringRef& value
	)
{
	script->append ("emit (");
	script->append (value);
	script->append (");");
}

int
LuaStringTemplate::getLine_lua (lua_State* h)
{
	lua::LuaNonOwnerState luaState (h);
	LuaStringTemplate* self = (LuaStringTemplate*) luaState.getContext ();
	ASSERT (self->m_luaState == h && !self->m_emitContextStack.isEmpty ());

	EmitContext* context = *self->m_emitContextStack.getTail ();

	luaState.pushInteger (context->m_lineCol.m_line);

	luaState.detach ();
	return 1;
}

int
LuaStringTemplate::getCol_lua (lua_State* h)
{
	lua::LuaNonOwnerState luaState (h);
	LuaStringTemplate* self = (LuaStringTemplate*) luaState.getContext ();
	ASSERT (self->m_luaState == h && !self->m_emitContextStack.isEmpty ());

	EmitContext* context = *self->m_emitContextStack.getTail ();
	ASSERT (self->m_luaState == h);

	luaState.pushInteger (context->m_lineCol.m_col);
	return 1;
}

int
LuaStringTemplate::emit_lua (lua_State* h)
{
	lua::LuaNonOwnerState luaState (h);
	LuaStringTemplate* self = (LuaStringTemplate*) luaState.getContext ();
	ASSERT (self->m_luaState == h && !self->m_emitContextStack.isEmpty ());

	EmitContext* context = *self->m_emitContextStack.getTail ();

	size_t count = luaState.getTop ();
	for (size_t i = 1; i <= count; i++)
	{
		sl::StringRef string = luaState.getString (i);
		context->m_output->append (string);
		context->m_lineCol.incrementalCount (string);
	}

	return 0;
}

int
LuaStringTemplate::passthrough_lua (lua_State* h)
{
	lua::LuaNonOwnerState luaState (h);
	LuaStringTemplate* self = (LuaStringTemplate*) luaState.getContext ();
	ASSERT (self->m_luaState == h && !self->m_emitContextStack.isEmpty ());

	EmitContext* context = *self->m_emitContextStack.getTail ();

	size_t offset = (size_t) luaState.getInteger (1);
	size_t length = (size_t) luaState.getInteger (2);

	sl::StringRef string = context->m_frame.getSubString (offset, length);
	context->m_output->append (string);
	context->m_lineCol.incrementalCount (string);

	return 0;
}

//..............................................................................

} // namespace st
} // namespace axl
