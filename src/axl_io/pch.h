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

#include "axl_io_Pch.h"
#include "axl_g_Module.h"
#include "axl_err_Error.h"
#include "axl_sys_Time.h"

#if (_AXL_OS_WIN)
#	include <iphlpapi.h>
#	include <winioctl.h>
#	include <shlobj.h>
#elif (_AXL_OS_POSIX)
#	include <ifaddrs.h>
#	include <net/if.h>
#	include <netdb.h>
#	if (_AXL_OS_LINUX)
#		include <linux/if_packet.h>
#		if (_AXL_SYS_LNX_LIBUDEV)
#			include <libudev.h>
#			include <linux/serial.h>
#		endif
#	elif (_AXL_OS_DARWIN)
#		include <libproc.h>
#		include <mach-o/dyld.h>
#		include <net/if_dl.h>
#		include "axl_iok_Pch.h"
#	endif
#endif
