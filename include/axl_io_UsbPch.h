// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_sl_Handle.h"
#include "axl_sl_Array.h"
#include "axl_err_ErrorMgr.h"

#if (_WIN32)
#	define _WINSOCKAPI_ // prevent winsock inclusion
#	pragma warning (disable: 4200) // warning C4200: nonstandard extension used : zero-sized array in struct/union
#endif

extern "C" {
#	include <libusb-1.0/libusb.h>
} // extern "C" {
