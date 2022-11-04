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
#include "axl_io_UsbMonitor.h"
#include "axl_io_File.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getUsbMonTransferTypeString(UsbMonTransferType transferType) {
	static const char* stringTable[] = {
		"Isochronous", // 0
		"Interrupt",   // 1
		"Control",     // 2
		"Bulk",        // 3
	};

	return (size_t)transferType < countof(stringTable) ?
		stringTable[transferType] :
		"Unknown";
}

//..............................................................................

bool
UsbMonitor::open(
	const sl::String& captureDeviceName,
	uint_t flags
) {
	int openFlags = (flags & io::FileFlag_Asynchronous) ?
		O_RDWR | O_NONBLOCK :
		O_RDWR;

	return m_device.open(captureDeviceName, openFlags);
}

size_t
UsbMonitor::read(
	void* p,
	size_t size
) {
	for (;;) {
		size_t result = m_device.read(m_readBuffer, m_readBuffer.getCount());
		if (result == -1)
			return -1;

		if (result < sizeof(lnx::usbmon::mon_bin_hdr)) {
			err::setFormatStringError("unexpected usbmon read size: %d", result);
			return -1;
		}

		const lnx::usbmon::mon_bin_hdr* hdr = (lnx::usbmon::mon_bin_hdr*) m_readBuffer.cp();
		if (hdr->)
	}
}

//..............................................................................

} // namespace io
} // namespace axl
