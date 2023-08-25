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

#include "axl_io_HidPch.h"
#include "axl_enc_Leb128.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_DeviceInfo.h"
#	include "axl_io_win_File.h"

#	include <cfgmgr32.h>
#	include <winioctl.h>
#	include <usbioctl.h>
#	include <usb.h>
#elif (_AXL_OS_LINUX)
#	include "axl_sys_lnx_UdevHwdb.h"
#elif (_AXL_OS_DARWIN)
#	include "axl_iok_RegistryEntry.h"
#	include <IOKit/usb/IOUSBLib.h>
#endif
