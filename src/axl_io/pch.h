#pragma once

#include "axl_g_Pch.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include <iphlpapi.h>
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include <ifaddrs.h>
#	include <net/if.h>
#endif

#ifdef _AXL_IO_PCAP
#	include <pcap/pcap.h>
#endif

#ifdef _AXL_IO_USB
#	pragma warning (disable: 4200)
#	include <libusb.h>
#endif