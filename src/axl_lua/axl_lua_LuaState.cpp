#include "pch.h"
#include "axl_lua_LuaState.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace lua {

//.............................................................................

bool
LuaState::create ()
{
	close ();

	m_h = luaL_newstate ();
	if (!m_h)
	{
		err::setError (err::StatusKind_InsufficientResources);
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

	rtl::String message = popString ();
	err::setStringError (message, message.getLength ());
	return false;
}

bool
LuaState::load (
	const char* name,
	const char* source,
	size_t length
	)
{
	ASSERT (isOpen ());

	if (length == -1)
		length = strlen (source);

	int result = luaL_loadbuffer (m_h, source, length, name);
	return complete (result);
}

#ifdef _DEBUG
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
			TRACE ("%08x %s\n", p, getString(i));
			break;
	
		case LUA_TBOOLEAN:
			TRACE ("%08x %s\n", p, getBoolean (i) ? "true" : "false");
			break;
	
		case LUA_TNUMBER:
			TRACE ("%08x %g\n", p, getNumber (i));
			break;

		default:
			TRACE ("%08x %s\n", p, getTypeName (type));
			break;
	
		}
	}
}
#endif
//.............................................................................

} // namespace lua
} // namespace axl
