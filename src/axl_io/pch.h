#pragma once

#include "axl_g_Pch.h"

#if (_AXL_POSIX == AXL_POSIX_DARWIN)
#	include "axl_iok_Pch.h"
#endif

#if (_AXL_ENV == AXL_ENV_WIN)
#	include <ws2tcpip.h>
#	include <iphlpapi.h>
#	include <winioctl.h>
#	include <devguid.h>
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include <ifaddrs.h>
#	include <net/if.h>
#	include <netdb.h>
#	if (_AXL_POSIX == AXL_POSIX_LINUX)
#		include <libudev.h>
#		include <linux/serial.h>
#	elif (_AXL_POSIX == AXL_POSIX_DARWIN)
#		include <libproc.h>
#		include <mach-o/dyld.h>
#	endif
#endif

#ifdef _AXL_IO_PCAP
#	include <pcap/pcap.h>
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