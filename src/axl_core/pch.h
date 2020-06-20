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

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include <sddl.h>
#	include <wintrust.h>
#	include <wincrypt.h>
#	include <softpub.h>
#elif (_AXL_OS_LINUX && defined _AXL_SYS_LNX_LIBUDEV)
#	include <libudev.h>
#elif (_AXL_OS_DARWIN)
#	include <mach/mach.h>
#	include <mach/mach_time.h>
#	include <CoreFoundation/CoreFoundation.h>
#	include <IOKit/IOBSD.h>
#	include <IOKit/IOKitLib.h>
#	include <IOKit/serial/IOSerialKeys.h>
#	include <IOKit/serial/ioss.h>
#endif
