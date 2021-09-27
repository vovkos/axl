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

#define _AXL_IO_USBTRANSFER_H

#include "axl_io_UsbPch.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getUsbTransferStatusString(libusb_transfer_status status);

//..............................................................................

class FreeUsbTransfer {
public:
	void
	operator () (libusb_transfer* h) {
		libusb_free_transfer(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbTransfer: public sl::Handle<libusb_transfer*, FreeUsbTransfer> {
public:
	bool
	create(size_t isochronousPacketCount = 0);

	bool
	submit();

	bool
	cancel();

	static
	void
	fillControlSetup(
		libusb_control_setup* setup,
		uint_t requestType,
		uint_t requestCode,
		uint_t value,
		uint_t index,
		size_t size
	) {
		libusb_fill_control_setup((uchar_t*)setup, requestType, requestCode, value, index, size);
	}

	void
	fillControlTransfer(
		libusb_device_handle* deviceOpenHandle,
		libusb_control_setup* setup,
		libusb_transfer_cb_fn callback,
		void* context,
		uint_t timeout = -1
	) {
		ASSERT(m_h);

		libusb_fill_control_transfer(
			m_h,
			deviceOpenHandle,
			(uchar_t*)setup,
			callback,
			context,
			timeout != -1 ? timeout : 0
		);
	}

	void
	fillBulkTransfer(
		libusb_device_handle* deviceOpenHandle,
		uint_t endpointId,
		void* buffer,
		size_t size,
		libusb_transfer_cb_fn callback,
		void* context,
		uint_t timeout = -1
	) {
		ASSERT(m_h);

		libusb_fill_bulk_transfer(
			m_h,
			deviceOpenHandle,
			endpointId,
			(uchar_t*)buffer,
			size,
			callback,
			context,
			timeout != -1 ? timeout : 0
		);
	}

	void
	fillInterruptTransfer(
		libusb_device_handle* deviceOpenHandle,
		uint_t endpointId,
		void* buffer,
		size_t size,
		libusb_transfer_cb_fn callback,
		void* context,
		uint_t timeout = -1
	) {
		ASSERT(m_h);

		libusb_fill_interrupt_transfer(
			m_h,
			deviceOpenHandle,
			endpointId,
			(uchar_t*)buffer,
			size,
			callback,
			context,
			timeout != -1 ? timeout : 0
		);
	}
};

//..............................................................................

} // namespace io
} // namespace axl
