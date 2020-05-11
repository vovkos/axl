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
#	include <psapi.h>
#	include <dbghelp.h>
#elif (_AXL_OS_LINUX)
#	include <link.h>
#elif (_AXL_OS_DARWIN)
#   include <mach-o/dyld.h>
#endif