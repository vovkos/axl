#include "stdafx.h"
#include "axl_lua_LuaState.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace lua {

//.............................................................................

bool
CLuaState::Create ()
{
	Close ();

	m_h = luaL_newstate ();
	if (!m_h)
	{
		err::SetError (err::EStatus_InsufficientResources);
		return false;
	}

	return true;
}

bool
CLuaState::Complete (int Result)
{
	ASSERT (IsOpen ());

	if (Result == LUA_OK)
		return true;

	rtl::CString Message = PopString ();
	err::SetStringError (Message, Message.GetLength ());
	return false;
}

bool
CLuaState::Load (
	const char* pName,
	const char* pSource,
	size_t Length
	)
{
	ASSERT (IsOpen ());

	if (Length == -1)
		Length = strlen (pSource);

	int Result = luaL_loadbuffer (m_h, pSource, Length, pName);
	return Complete (Result);
}

#ifdef _DEBUG
void
CLuaState::Trace ()
{
	int Top = GetTop ();

	for (int i = 1; i <= Top; i++) 
	{
		int Type = GetType (i);
		const void* p = lua_topointer (m_h, i);
		
		switch (Type) 
		{    
		case LUA_TSTRING:
			TRACE ("%08x %s\n", p, GetString(i));
			break;
    
		case LUA_TBOOLEAN:
			TRACE ("%08x %s\n", p, GetBoolean (i) ? "true" : "false");
			break;
    
		case LUA_TNUMBER:
			TRACE ("%08x %g\n", p, GetNumber (i));
			break;

		default:
			TRACE ("%08x %s\n", p, GetTypeName (Type));
			break;
    
		}
	}
}
#endif
//.............................................................................

} // namespace lua
} // namespace axl

