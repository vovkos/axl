// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

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
#elif (_AXL_OS_POSIX)
#	include "axl_sys_psx_Sem.h"
#endif
