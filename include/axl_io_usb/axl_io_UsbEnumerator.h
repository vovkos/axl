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

#define _AXL_IO_USBENUMERATOR_H

#include "axl_io_UsbDeviceDesc.h"
#include "axl_io_UsbDeviceList.h"

namespace axl {
namespace io {

//..............................................................................

enum UsbEnumFlag {
	UsbEnumFlag_Hubs = 0x0100,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UsbDeviceEntry:
	sl::ListLink,
	UsbDeviceDesc {
	libusb_device* m_device;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
enumerateUsbDevices(
	libusb_context* context,
	UsbDeviceList* loDeviceList,
	sl::List<UsbDeviceEntry>* deviceList,
	uint_t flags = 0
);

inline
size_t
enumerateUsbDevices(
	UsbDeviceList* loDeviceList,
	sl::List<UsbDeviceEntry>* deviceList,
	uint_t flags = 0
) {
	return enumerateUsbDevices(NULL, loDeviceList, deviceList, flags);
}

inline
size_t
enumerateUsbDevices(
	sl::List<UsbDeviceEntry>* deviceList,
	uint_t flags = 0
) {
	return enumerateUsbDevices(NULL, deviceList, flags);
}

//..............................................................................

} // namespace io
} // namespace axl
