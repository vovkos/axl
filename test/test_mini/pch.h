#pragma once

//.............................................................................

#include "axl_g_Pch.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include <ws2tcpip.h>
#	include <iphlpapi.h>
#	pragma comment (lib, "ws2_32.lib")
#	pragma comment (lib, "iphlpapi.lib")
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include <netdb.h>
#	include <signal.h>
#	include <ucontext.h>
#endif

#ifdef _AXL_IO_USB
#	if (_AXL_ENV == AXL_ENV_WIN)
#		define _WINSOCKAPI_ // prevent winsock inclusion
#		pragma warning (disable: 4200) // warning C4200: nonstandard extension used : zero-sized array in struct/union
#	endif
extern "C" {
#	include <libusb.h>
} // extern "C" {
#endif

// axl

#include "axl_sl_String.h"
#include "axl_err_Error.h"
#include "axl_fsm_RegExp.h"
#include "axl_fsm_StdRegExpNameMgr.h"
#include "axl_sl_AutoPtrArray.h"
#include "axl_mt_LongJmpTry.h"
#include "axl_sys_DynamicLibrary.h"
#include "axl_io_Socket.h"
#include "axl_io_File.h"
#include "axl_io_NetworkAdapter.h"
#include "axl_io_UsbDevice.h"
#include "axl_g_Time.h"
#include "axl_mt_Event.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_mem_win_VirtualMemory.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_io_psx_Mapping.h"
#	include "axl_mt_psx_Sem.h"
#endif

using namespace axl;

//.............................................................................
