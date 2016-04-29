#pragma once

#include "axl_g_Pch.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include <sddl.h>
#elif (_AXL_POSIX == AXL_POSIX_LINUX && defined _AXL_SYS_LNX_LIBUDEV)
#	include <libudev.h>
#elif (_AXL_POSIX == AXL_POSIX_DARWIN)
#	include <CoreFoundation/CoreFoundation.h>
#	include <IOKit/IOBSD.h>
#	include <IOKit/IOKitLib.h>
#	include <IOKit/serial/IOSerialKeys.h>
#	include <IOKit/serial/ioss.h>
#endif