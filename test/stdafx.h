// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <new>
#include <typeinfo>

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

#include <tchar.h>

#include <winsock2.h>
#include <ws2ipdef.h>
#pragma comment (lib, "ws2_32.lib")

#include <string>
#include <vector>

//.............................................................................

// atl

#include <atlbase.h>
#include <atlwin.h>

//.............................................................................

// LUA

extern "C" {

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

} // extern "C" {

//.............................................................................

// axl

#define _AXL_DISABLE_PREMATURE_WINDOW_DESTRUCTION_CHECK

#include <axl_rtl_Array.h>
#include <axl_rtl_HashTable.h>
#include <axl_rtl_BoxList.h>
#include <axl_rtl_Packer.h>
#include <axl_rtl_Unpacker.h>

#include <axl_exe_WorkerThread.h>
#include <axl_exe_WorkerWindow.h>
#include <axl_exe_ScheduledFunction.h>

#include <axl_win_Window.h>
#include <axl_mt_Timer.h>
#include <axl_io_File.h>
#include <axl_io_MappedFile.h>

#include <axl_ref_Buf.h>
#include <axl_ref_Clone.h>
#include <axl_ref_WeakPtr.h>
#include <axl_ref_Child.h>

#include <axl_io_ListenerSock.h>
#include <axl_io_DgramSock.h>

#include <axl_lex_RagelLexer.h>

#include <axl_st_StringTemplate.h>

//.............................................................................
