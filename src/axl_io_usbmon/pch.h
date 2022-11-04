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

#if (_AXL_OS_WIN)
#	include <winioctl.h>
#	include <setupapi.h>
#	include <cfgmgr32.h>
#	include <usbioctl.h>
#elif (_AXL_OS_LINUX)
#	include <libudev.h>
#endif
