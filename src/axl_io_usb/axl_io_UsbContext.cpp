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

#include "pch.h"
#include "axl_io_UsbContext.h"

namespace axl {
namespace io {

//..............................................................................

bool
UsbContext::create() {
	close();

	int result = libusb_init(&m_h);
	return result == 0 ? true : err::fail(UsbError((int)result));
}

bool
UsbContext::createDefault() {
	close();

	int result = libusb_init(NULL);
	if (result != 0)
		return err::fail(UsbError((int)result));

	m_h = NULL;
	return true;
}

bool
UsbContext::handleEvents(uint_t timeout) {
	if (timeout == -1)
		return handleEvents();

	timeval tv = { 0 };
	tv.tv_usec = timeout * 1000;

	int result = libusb_handle_events_timeout(m_h, &tv);
	return result == 0 ? true : err::fail(UsbError((int)result));
}

//..............................................................................

} // namespace io
} // namespace axl
