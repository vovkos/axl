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

#pragma once

//..............................................................................

// axl

#include "axl_sl_String.h"
#include "axl_sl_AutoPtrArray.h"
#include "axl_sl_CmdLineParser.h"
#include "axl_sl_BoyerMooreFind.h"
#include "axl_sl_HashTable.h"

#include "axl_io_Socket.h"
#include "axl_io_Serial.h"
#include "axl_io_File.h"
#include "axl_io_MappedFile.h"
#include "axl_io_FilePathUtils.h"
#include "axl_io_NetworkAdapter.h"
#include "axl_io_SharedMemoryTransport.h"
#include "axl_io_FileEnumerator.h"

#include "axl_sys_SjljTry.h"
#include "axl_sys_DynamicLib.h"
#include "axl_sys_Time.h"
#include "axl_sys_Event.h"
#include "axl_sys_Process.h"
#include "axl_sys_ReadWriteLock.h"
#include "axl_sys_Thread.h"

#include "axl_fsm_Regex.h"
#include "axl_fsm_StdRegexNameMgr.h"
#include "axl_zip_ZipReader.h"
#include "axl_err_Errno.h"

#include "axl_enc_EscapeEncoding.h"
#include "axl_enc_Base32Encoding.h"
#include "axl_enc_HexEncoding.h"

#ifdef _AXL_XML
#	include "axl_xml_ExpatParser.h"
#endif

#ifdef _AXL_IO_USB
#	include "axl_io_UsbContext.h"
#	include "axl_io_UsbEventThread.h"
#	include "axl_io_UsbDevice.h"
#	include "axl_io_UsbTransfer.h"
#endif

#ifdef _AXL_IO_PCAP
#	include "axl_io_PcapPch.h"
#	include "axl_io_Pcap.h"
#endif

#ifdef _AXL_CRY
#	include "axl_cry_BigNum.h"
#	include "axl_cry_Bio.h"
#	include "axl_cry_Asn1.h"
#	include "axl_cry_X509.h"
#	include "axl_cry_Pkcs7.h"
#	include "axl_cry_CryptoError.h"
#	include "axl_cry_AppStoreReceipt.h"
#endif

#ifdef _AXL_IO_SSL
#	include "axl_io_SslCtx.h"
#	include "axl_io_Ssl.h"
#endif

#ifdef _AXL_SPY
#	include "axl_spy_ImportEnumerator.h"
#	include "axl_spy_ImportWriteProtection.h"
#	include "axl_spy_ModuleEnumerator.h"
#	include "axl_spy_Hook.h"
#endif

#ifdef _AXL_INI
#	include "axl_ini_Parser.h"
#endif

#ifdef _AXL_SYS_CA
#	include "axl_sys_CodeAuthenticator.h"
#endif

#if (_AXL_OS_WIN)
#	include "axl_sys_win_VirtualMemory.h"
#	include "axl_sys_win_Process.h"
#	include "axl_sys_win_NtDll.h"
#	include "axl_sys_win_NtStatus.h"
#	include "axl_sys_win_Authenticode.h"
#elif (_AXL_OS_POSIX)
#	include "axl_io_psx_Mapping.h"
#	include "axl_io_psx_Pty.h"
#	include "axl_sys_psx_Sem.h"
#	if (_AXL_OS_DARWIN)
#		include "axl_sys_drw_Semaphore.h"
#		include "axl_cf_Url.h"
#	endif
#endif

using namespace axl;

//..............................................................................

#if (_AXL_OS_WIN)
#	include <ws2tcpip.h>
#	include <iphlpapi.h>
#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "iphlpapi.lib")
#	pragma comment(lib, "wintrust.lib")
#	pragma comment(lib, "crypt32.lib")
#elif (_AXL_OS_POSIX)
#	include <netdb.h>
#	include <signal.h>
#	include <sys/ucontext.h>
#	include <netinet/tcp.h>
#endif

#ifdef _AXL_IO_USB
#	if (_AXL_OS_WIN)
#		define _WINSOCKAPI_ // prevent winsock inclusion
#		pragma warning(disable: 4200) // warning C4200: nonstandard extension used : zero-sized array in struct/union
#	endif
extern "C" {
#	include <libusb-1.0/libusb.h>
} // extern "C" {
#endif

//..............................................................................

#include <string> // for spy testing
