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

#include "axl_io_UsbMonitorBase.h"
#include "axl_sl_String.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_UsbPcap.h"
#elif (_AXL_OS_LINUX)
#	include "axl_io_lnx_UsbMon.h"
#endif

namespace axl {
namespace io {
namespace lnx {

//..............................................................................

class UsbMonitor: public UsbMonitorBase {
public:
	axl::io::lnx::UsbMon m_device;  // for asynchronous access
	uint_t m_filterAddress;         // freely adjustable

public:
	UsbMonitor() {
		m_filterAddress = 0;
	}

	bool
	isOpen() const {
		return m_device.isOpen();
	}

	bool
	open(
		const sl::String& captureDeviceName,
		uint_t flags = 0
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
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
UsbMonitor::open(
	const sl::String& captureDeviceName,
	uint_t flags
) {
	int openFlags = (flags & io::FileFlag_Asynchronous) ? O_RDWR | O_NONBLOCK : O_RDWR;
	return m_device.open(captureDeviceName, openFlags);
}

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
