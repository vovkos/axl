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

#include "axl_sl_Handle.h"
#include "axl_sl_Array.h"
#include "axl_err_ErrorMgr.h"

#if (_WIN32)
#	define _WINSOCKAPI_ // prevent winsock inclusion
#	pragma warning(disable: 4200) // warning C4200: nonstandard extension used : zero-sized array in struct/union
#endif

#include <openssl/err.h>
#include <openssl/ssl.h>

#if (_AXL_OS_WIN)
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif
