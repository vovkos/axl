// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#define _CRT_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" string functions

// CRT

#include <new>
#include <typeinfo>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

// SDK

#include <tchar.h>
#include <guiddef.h>
#include <cguid.h>
#include <windows.h>

// LUA

extern "C" {

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

} // extern "C" {

#ifdef _DEBUG
#pragma comment (lib, "lua_d.lib")
#else
#pragma comment (lib, "lua.lib")
#endif