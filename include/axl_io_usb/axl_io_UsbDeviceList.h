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

#include "axl_io_UsbPch.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//..............................................................................

class FreeUsbDeviceList {
public:
	void
	operator () (libusb_device** h) {
		libusb_free_device_list(h, true);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbDeviceList: public sl::Handle<libusb_device**, FreeUsbDeviceList> {
public:
	size_t
	enumerateDevices(libusb_context* context = NULL);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
UsbDeviceList::enumerateDevices(libusb_context* context) {
	close();

	ssize_t result = libusb_get_device_list(context, &m_h);
	return result >= 0 ? result : err::fail<size_t>(-1, UsbError((int)result));
}

//..............................................................................

} // namespace io
} // namespace axl
