#pragma once

#include "axl_g_Pch.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	define getsockerror WSAGetLastError
#	define socklen_t    int

#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <netinet/ip.h>
#	include <arpa/inet.h>
#	define SOCKET         int
#	define INVALID_SOCKET (-1)
#	define closesocket    close

inline
int
getsockerror ()
{
	return errno;
}

#endif

//.............................................................................

// AXL

#include "axl_jnc_Pch.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Disassembler.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"
#include "axl_io_FilePathUtils.h"
#include "axl_io_File.h"
#include "axl_io_MappedFile.h"
#include "axl_err_Errno.h"
#include "axl_mt_Thread.h"
#include "axl_rtl_String.h"
#include "axl_rtl_CmdLineParser.h"
#include "axl_g_Time.h"

using namespace axl;

//.............................................................................
