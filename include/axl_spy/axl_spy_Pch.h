#pragma once

#include "axl_sl_String.h"
#include "axl_sl_Array.h"

#if (_AXL_OS_WIN)
#	include "axl_sl_RbTree.h"
#else
#   include "axl_sys_psx_DynamicLib.h"
#endif

#if (_AXL_OS_LINUX)
#	include <link.h>
#elif (_AXL_OS_DARWIN)
#	include <mach-o/dyld.h>
#	include <mach-o/loader.h>
#endif
