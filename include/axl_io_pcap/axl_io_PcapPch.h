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
#include "axl_sl_String.h"
#include "axl_err_Error.h"
#include "axl_sys_Time.h"

#include <pcap/pcap.h>

#ifndef PCAP_NETMASK_UNKNOWN
#	define PCAP_NETMASK_UNKNOWN 0xffffffff
#endif
