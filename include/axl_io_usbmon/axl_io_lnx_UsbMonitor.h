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

#define _AXL_IO_LNX_USBMONITOR_H

#include "axl_io_UsbMonTransfer.h"
#include "axl_io_lnx_UsbMon.h"

namespace axl {
namespace io {
namespace lnx {

//..............................................................................

class UsbMonitor {
public:
	enum {
		MinimumKernelBufferSize = 1 * 1024,
		DefaultKernelBufferSize = 1 * 1024 * 1024,
	};

protected:
	sl::Array<char> m_readBuffer;

public:
	axl::io::lnx::UsbMon m_device;  // for asynchronous access
	uint_t m_addressFilter;         // freely adjustable

public:
	UsbMonitor() {
		m_addressFilter = 0;
	}

	bool
	isOpen() const {
		return m_device.isOpen();
	}

	bool
	open(
		const sl::String& captureDeviceName,
		size_t kernelBufferSize,
		uint_t addressFilter
	);

	bool
	open(
		const sl::String& captureDeviceName,
		uint_t addressFilter
	);

	void
	close() {
		m_device.close();
	}

	size_t
	getKernelBufferSize() {
		return m_device.getKernelBufferSize();
	}

	bool
	setKernelBufferSize(size_t size) {
		return m_device.setKernelBufferSize(size);
	}

	size_t
	read(
		void* p,
		size_t size,
		uint_t timeout = -1
	);

protected:
	size_t // returns combined size of the header and data
	get(
		usbmon::mon_bin_hdr_ex* hdr,
		uint_t timeout
	);

	void
	fillUsbMonTransferHdr(
		UsbMonTransferHdr* dstHdr,
		const usbmon::mon_bin_hdr_ex* srcHdr
	);
};

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
