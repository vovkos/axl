#pragma once

//.............................................................................

// axl

#include "axl_sl_String.h"
#include "axl_err_Error.h"
#include "axl_fsm_RegExp.h"
#include "axl_fsm_StdRegExpNameMgr.h"
#include "axl_sl_AutoPtrArray.h"
#include "axl_sl_CmdLineParser.h"
#include "axl_sys_SjljTry.h"
#include "axl_sys_DynamicLibrary.h"
#include "axl_io_Socket.h"
#include "axl_io_Serial.h"
#include "axl_io_File.h"
#include "axl_io_NetworkAdapter.h"
#include "axl_io_SharedMemoryTransport.h"
#include "axl_sys_Time.h"
#include "axl_sys_Event.h"
#include "axl_io_FilePathUtils.h"
#include "axl_zip_ZipReader.h"
#include "axl_enc_EscapeEncoding.h"

#ifdef _AXL_XML
#	include "axl_xml_ExpatParser.h"
#endif

#ifdef _AXL_IO_USB
#	include "axl_io_UsbDevice.h"
#endif

#if (_AXL_OS_WIN)
#	include "axl_sys_win_VirtualMemory.h"
#	include "axl_sys_win_Process.h"
#elif (_AXL_OS_POSIX)
#	include "axl_io_psx_Mapping.h"
#	include "axl_sys_psx_Sem.h"
#	if (_AXL_OS_DARWIN)
#		include "axl_sys_drw_Semaphore.h"
#	endif
#endif

#include "axl_io_FileEnumerator.h"

using namespace axl;

//.............................................................................

#if (_AXL_OS_WIN)
#	include <ws2tcpip.h>
#	include <iphlpapi.h>
#	pragma comment (lib, "ws2_32.lib")
#	pragma comment (lib, "iphlpapi.lib")
#elif (_AXL_OS_POSIX)
#	include <netdb.h>
#	include <signal.h>
#	include <sys/ucontext.h>
#endif

#ifdef _AXL_IO_USB
#	if (_AXL_OS_WIN)
#		define _WINSOCKAPI_ // prevent winsock inclusion
#		pragma warning (disable: 4200) // warning C4200: nonstandard extension used : zero-sized array in struct/union
#	endif
extern "C" {
#	include <libusb-1.0/libusb.h>
} // extern "C" {
#endif

#ifdef _AXL_XML
#	include "axl_xml_Pch.h"
#endif

//.............................................................................
