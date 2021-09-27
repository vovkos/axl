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
#include "axl_io_UsbTransfer.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getUsbTransferStatusString(libusb_transfer_status status) {
	const char* stringTable[] = {
		"LIBUSB_TRANSFER_COMPLETED",
		"LIBUSB_TRANSFER_ERROR",
		"LIBUSB_TRANSFER_TIMED_OUT",
		"LIBUSB_TRANSFER_CANCELLED",
		"LIBUSB_TRANSFER_STALL",
		"LIBUSB_TRANSFER_NO_DEVICE",
		"LIBUSB_TRANSFER_OVERFLOW",
	};

	return (size_t)status < countof(stringTable) ?
		stringTable[(size_t)status] :
		"libusb-undefined-status";
}

//..............................................................................

bool
UsbTransfer::create(size_t isochronousPacketCount) {
	close();

	m_h = libusb_alloc_transfer(isochronousPacketCount);
	if (!m_h) {
		err::setError(err::SystemErrorCode_InsufficientResources);
		return false;
	}

	return true;
}

bool
UsbTransfer::submit() {
	ASSERT(m_h);

	int result = libusb_submit_transfer(m_h);
	return result == 0 ? true : err::fail(UsbError(result));
}

bool
UsbTransfer::cancel() {
	ASSERT(m_h);

	int result = libusb_cancel_transfer(m_h);
	return result == 0 ? true : err::fail(UsbError(result));
}

//..............................................................................

} // namespace io
} // namespace axl
