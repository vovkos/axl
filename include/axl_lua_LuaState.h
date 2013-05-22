// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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

class CLuaClose
{
public:
	void 
	operator () (lua_State* pLua)
	{
		lua_close (pLua);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CLuaState: public rtl::CHandleT <lua_State*, CLuaClose>
{
public:
	CLuaState ()
	{
		Create ();
	}

	CLuaState (lua_State* pLuaState)
	{
		Attach (pLuaState);
	}

	bool
	Create ();

	bool
	Load (
		const char* pName,
		const char* pSource,
		size_t Length = -1
		);

#ifdef _DEBUG
	void
	Trace ();
#endif

	void 
	ClearStack ()
	{
		ASSERT (IsOpen ());
		lua_settop (m_h, 0);
	}

	int 
	GetTop ()
	{
		ASSERT (IsOpen ());
		return lua_gettop (m_h);
	}

	int 
	GetType (int Index)
	{
		ASSERT (IsOpen ());
		return lua_type (m_h, Index);
	}

	const char*
	GetTypeName (int Type)
	{
		ASSERT (IsOpen ());
		return lua_typename (m_h, Type);
	}

	void 
	Insert (int Index)
	{
		ASSERT (IsOpen ());
		lua_insert (m_h, Index);
	}

	void 
	Remove (int Index)
	{
		ASSERT (IsOpen ());
		lua_remove (m_h, Index);
	}

	void 
	Swap ()
	{
		Insert (-2);
	}

	void
	PushNil ()
	{
		ASSERT (IsOpen ());
		lua_pushnil (m_h);
	}

	void
	PushInteger (lua_Integer Value)
	{
		ASSERT (IsOpen ());
		lua_pushinteger (m_h, Value);
	}

	void
	PushBoolean (bool Value)
	{
		ASSERT (IsOpen ());
		lua_pushboolean (m_h, Value);
	}

	void
	PushString (
		const char* pString,
		size_t Length = -1
		)
	{
		ASSERT (IsOpen ());

		if (!pString)
			pString = "";

		if (Length != -1)
			lua_pushlstring (m_h, pString, Length);
		else
			lua_pushstring (m_h, pString);
	}

	void
	PushClosure (
		lua_CFunction pf,
		size_t ContextArgumentCount
		)
	{
		ASSERT (IsOpen ());
		lua_pushcclosure (m_h, pf, ContextArgumentCount);
	}	

	void
	PushFunction (lua_CFunction pf)
	{
		ASSERT (IsOpen ());
		lua_pushcfunction (m_h, pf);
	}	

	void
	PushFunction (
		lua_CFunction pf,
		intptr_t Context
		)
	{
		PushInteger (Context);
		PushClosure (pf, 1);
	}

	void
	CreateTable (
		size_t ElementCount = 0,
		size_t MemberCount = 0
		)
	{
		ASSERT (IsOpen ());
		lua_createtable (m_h, ElementCount, MemberCount);
	}

	void
	GetTable (int Index = -2)
	{
		ASSERT (IsOpen ());
		lua_gettable (m_h, Index);
	}

	void
	SetTable (int Index = -3)
	{
		ASSERT (IsOpen ());
		lua_settable (m_h, Index);
	}

	void
	GetMember (const char* pName)
	{
		PushString (pName);
		GetTable ();
	}

	void
	SetMember (const char* pName)
	{
		PushString (pName);
		Swap ();
		SetTable ();
	}

	void
	GetArrayElement (int Index)
	{
		PushInteger (Index);
		GetTable ();
	}

	void
	SetArrayElement (int Index)
	{
		PushInteger (Index);
		Swap ();
		SetTable ();
	}

	void
	SetMemberInteger (
		const char* pName,
		lua_Integer Value
		)
	{
		PushString (pName);
		PushInteger (Value);
		SetTable ();
	}

	void
	SetMemberBoolean (
		const char* pName,
		bool Value
		)
	{
		PushString (pName);
		PushBoolean (Value);
		SetTable ();
	}

	void
	SetMemberString (
		const char* pName,
		const char* pString,
		size_t Length = -1
		)
	{
		PushString (pName);
		PushString (pString, Length);
		SetTable ();
	}

	void
	SetArrayElementInteger (
		int Index,
		lua_Integer Value
		)
	{
		PushInteger (Index);
		PushInteger (Value);
		SetTable ();
	}

	void
	SetArrayElementString (
		int Index,
		const char* pString,
		size_t Length = -1
		)
	{
		PushInteger (Index);
		PushString (pString, Length);
		SetTable ();
	}

	void
	Pop (size_t Count = 1)
	{
		ASSERT (IsOpen ());
		lua_pop (m_h, Count);
	}

	lua_Integer
	GetInteger (int Index = -1)
	{
		ASSERT (IsOpen ());
		return lua_tointeger (m_h, Index);
	}

	lua_Number
	GetNumber (int Index = -1)
	{
		ASSERT (IsOpen ());
		return lua_tonumber (m_h, Index);
	}

	bool
	GetBoolean (int Index = -1)
	{
		ASSERT (IsOpen ());
		return lua_toboolean (m_h, Index) != 0;
	}

	const char*
	GetString (int Index = -1)
	{
		ASSERT (IsOpen ());
		return lua_tostring (m_h, Index);
	}

	lua_Integer
	PopInteger ()
	{
		lua_Integer Value = GetInteger (-1);
		Pop ();
		return Value;
	}

	bool
	PopBoolean ()
	{
		bool Value = GetBoolean (-1);
		Pop ();
		return Value;
	}

	rtl::CString
	PopString ()
	{
		rtl::CString String = GetString (-1);
		Pop ();
		return String;
	}

	void
	GetGlobal (const char* pName)
	{
		ASSERT (IsOpen ());
		lua_getglobal (m_h, pName);
	}

	void
	SetGlobal (const char* pName)
	{
		ASSERT (IsOpen ());
		lua_setglobal (m_h, pName);
	}

	void
	GetGlobalArrayElement (
		const char* pName,
		int Index
		)
	{
		GetGlobal (pName);
		GetArrayElement (Index);
		Remove (-2);
	}

	void
	GetGlobalMember (
		const char* pName,
		const char* pMember
		)
	{
		GetGlobal (pName);
		GetMember (pMember);
		Remove (-2);
	}


	void
	SetGlobalInteger (
		const char* pName,
		lua_Integer Value
		)
	{
		PushInteger (Value);
		SetGlobal (pName);
	}

	void
	SetGlobalBoolean (
		const char* pName,
		bool Value
		)
	{
		PushBoolean (Value);
		SetGlobal (pName);
	}

	void
	SetGlobalString (
		const char* pName,
		const char* pString,
		size_t Length = -1
		)
	{
		PushString (pString, Length);
		SetGlobal (pName);
	}

	void
	RegisterFunction (
		const char* pName,
		lua_CFunction pf
		)
	{
		PushFunction (pf);
		SetGlobal (pName);
	}

	void
	RegisterFunction (
		const char* pName,
		lua_CFunction pf,
		intptr_t Context
		)
	{
		PushFunction (pf, Context);
		SetGlobal (pName);
	}

	intptr_t
	GetContext ()
	{
		int Index = GetUpValueIndex ();
		return GetInteger (Index);
	}

	int
	GetUpValueIndex (int Index = 1)
	{
		ASSERT (IsOpen ());
		return lua_upvalueindex (Index);
	}

	void
	Call (
		size_t ArgumentCount,
		size_t ResultCount = LUA_MULTRET
		)
	{
		ASSERT (IsOpen ());
		lua_call (m_h, ArgumentCount, ResultCount);
	}

	bool
	PCall (
		size_t ArgumentCount,
		size_t ResultCount = LUA_MULTRET
		)
	{
		ASSERT (IsOpen ());
		return Complete (lua_pcall (m_h, ArgumentCount, ResultCount, 0));
	}

protected:
	bool
	Complete (int Result);
};

//.............................................................................

} // namespace lua {
} // namespace axl {
