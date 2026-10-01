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

#define _AXL_IO_USBHOTPLUGCALLBACK_H

#include "axl_io_UsbPch.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//..............................................................................

struct UsbHotplugCallbackHandle {
	libusb_context* m_context;
	libusb_hotplug_callback_handle m_h;

	UsbHotplugCallbackHandle(
		libusb_context* context,
		libusb_hotplug_callback_handle h
	) {
		m_context = context;
		m_h = h;
	}

	explicit
	UsbHotplugCallbackHandle(libusb_hotplug_callback_handle h):
		UsbHotplugCallbackHandle(NULL, h) {}

	UsbHotplugCallbackHandle():
		UsbHotplugCallbackHandle(NULL, NULL) {}

	operator libusb_hotplug_callback_handle() const {
		return m_h;
	}

	bool
	operator == (const UsbHotplugCallbackHandle& src) const {
		return m_h == src.m_h;
	}

	bool
	operator != (const UsbHotplugCallbackHandle& src) const {
		return m_h != src.m_h;
	}

	void
	deregister() {
		ASSERT(m_h);
		libusb_hotplug_deregister_callback(m_context, m_h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DeregisterUsbHotplugCallback {
public:
	void
	operator () (UsbHotplugCallbackHandle h) {
		h.deregister();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbHotplugCallback: public sl::Handle<
	UsbHotplugCallbackHandle,
	DeregisterUsbHotplugCallback
> {
public:
	bool
	create(
		libusb_context* context,
		int events, // libusb_hotplug_event
		int flags,  // libusb_hotplug_flag
		int vendorId,
		int productId,
		int deviceClass,
		libusb_hotplug_callback_fn callback,
		void* callbackContext
	);

	bool
	create(
		int events, // libusb_hotplug_event
		int flags,  // libusb_hotplug_flag
		int vendorId,
		int productId,
		int deviceClass,
		libusb_hotplug_callback_fn callback,
		void* callbackContext
	) {
		return create(NULL, events, flags, vendorId, productId, deviceClass, callback, callbackContext);
	}

	bool
	create(
		libusb_context* context,
		int events,
		libusb_hotplug_callback_fn callback,
		void* callbackContext
	) {
		return create(
			context,
			events,
			0,
			LIBUSB_HOTPLUG_MATCH_ANY,
			LIBUSB_HOTPLUG_MATCH_ANY,
			LIBUSB_HOTPLUG_MATCH_ANY,
			callback,
			callbackContext
		);
	}

	bool
	create(
		int events,
		libusb_hotplug_callback_fn callback,
		void* callbackContext
	) {
		return create(NULL, events, callback, callbackContext);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
UsbHotplugCallback::create(
	libusb_context* context,
	int events,
	int flags,
	int vendorId,
	int productId,
	int deviceClass,
	libusb_hotplug_callback_fn callback,
	void* callbackContext
) {
	close();

	int result = libusb_hotplug_register_callback(
		context,
		(libusb_hotplug_event)events,
		(libusb_hotplug_flag)flags,
		vendorId,
		productId,
		deviceClass,
		callback,
		callbackContext,
		&m_h.m_h
	);

	if (result != LIBUSB_SUCCESS)
		return err::fail(UsbError(result));

	m_h.m_context = context;
	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
