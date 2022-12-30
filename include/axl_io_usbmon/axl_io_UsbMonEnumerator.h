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

#define _AXL_IO_USBMONNUMERATOR_H

#include "axl_io_UsbEnumerator.h"

namespace axl {
namespace io {

//..............................................................................

enum UsbMonEnumFlag {
	UsbMonEnumFlag_Hubs = UsbEnumFlag_Hubs
};

//..............................................................................

struct UsbMonDeviceDesc:
	sl::ListLink,
	UsbDeviceDesc {
	sl::String m_captureDeviceName;
	uint_t m_captureDeviceId;
};

//..............................................................................

size_t
enumerateUsbMonDevices(
	sl::List<UsbMonDeviceDesc>* deviceList,
	uint_t flags = 0 //
);

//..............................................................................

} // namespace io
} // namespace axl
