#include "pch.h"
#include "axl_st_LuaStringTemplate.h"
#include "axl_st_StringTemplateLexer.h"
#include "axl_io_MappedFile.h"

namespace axl {
namespace st {

//.............................................................................

bool
LuaStringTemplate::create ()
{
	close ();

	bool result = m_luaState.create ();
	if (!result)
		return false;

	m_luaState.openLibs ();

	m_luaState.registerFunction ("getLine", getLine_lua, (intptr_t) this);
	m_luaState.registerFunction ("getCol", getCol_lua, (intptr_t) this);
	m_luaState.registerFunction ("emit", emit_lua, (intptr_t) this);
	m_luaState.registerFunction ("passthrough", passthrough_lua, (intptr_t) this);

	return true;
}

bool 
LuaStringTemplate::runScript (
	const sl::StringRef& fileName,
	const sl::StringRef& source
	)
{
	bool result = m_luaState.load (fileName, source, source.getLength ());
	if (!result)
		return false;

	int top = m_luaState.getTop ();
	
	result = m_luaState.PCall (0, 0);

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
		const char* p = luaState.getString (i);
		context->m_output->append (p);
		context->m_lineCol.incrementalCount (p);
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

	size_t offset = luaState.getInteger (1);
	size_t length = luaState.getInteger (2);

	const char* p = context->m_frame + offset;

	context->m_output->append (p, length);
	context->m_lineCol.incrementalCount (p, length);

	return 0;
}

//.............................................................................

} // namespace st
} // namespace axl

