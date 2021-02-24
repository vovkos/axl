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

#include "axl_sl_String.h"

#if (_AXL_OS_WIN)
#	include "axl_sl_Array.h"
#	include "axl_sys_win_Authenticode.h"
#elif (_AXL_OS_LINUX)
#	include "axl_cry_Rsa.h"
#	include "axl_mem_Block.h"
#elif (_AXL_OS_DARWIN)
#	include "axl_cf_String.h"
#endif
