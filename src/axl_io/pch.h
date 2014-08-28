#pragma once

#include "axl_g_Pch.h"

#ifdef _AXL_IO_PCAP
#	include <pcap/pcap.h>
#endif

#ifdef _AXL_IO_USB
#	pragma warning (disable: 4200)
#	include <libusb.h>
#endif