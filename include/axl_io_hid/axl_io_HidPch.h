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
#include <hidapi/hidapi.h>

#if (_AXL_OS_WIN)
#	include <hidapi/hidapi_winapi.h>
#endif

#include "axl_sl_String.h"
#include "axl_sl_List.h"
#include "axl_sl_Array.h"
#include "axl_sl_HashTable.h"
#include "axl_sl_Handle.h"
#include "axl_err_Error.h"
