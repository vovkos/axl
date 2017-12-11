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
#include "axl_sl_List.h"
#include "axl_sl_BoxList.h"
#include "axl_sl_Array.h"
#include "axl_sl_RbTree.h"

#include "axl_sys_Lock.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Event.h"
#	include <ws2ipdef.h>
#	include <ws2tcpip.h>
#elif (_AXL_OS_POSIX)
#	include "axl_sys_psx_Sem.h"
#endif
