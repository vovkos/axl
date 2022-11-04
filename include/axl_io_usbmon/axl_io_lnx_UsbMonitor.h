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

#include "axl_sl_String.h"
#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace lnx {

//..............................................................................

class UsbMonitor
{
public:
	enum
	{
		// as suggested by USBPcapCMD

		IoctlOutputBufferSize = 1024,             // 1K
		DefaultKernelBufferSize = 1 * 1024 * 1024,  // 1M
		DefaultSnapshotLength = 65535,            // 64K - 1
	};

	enum Mode
	{
		Mode_Capture,
		Mode_Enumerate,
	};

protected:
	axl::io::psx::File m_device;

public:
	bool
	isOpen()
	{
		return m_device.isOpen();
	}

	bool
	open(
		const sl::String_w& name,
		Mode mode = Mode_Capture
	);

	void
	close()
	{
		m_device.close();
	}

	sl::String_w
	getHubSymlink();

	bool
	getHubSymlink(sl::String_w* symlink);

	bool
	setSnapshotLength(size_t size);

	bool
	setKernelBufferSize(size_t size);

	bool
	setFilter(uint_t deviceAddress);

	bool
	setFilter(
		const uint_t* deviceAddressTable,
		size_t deviceCount
	);

	bool
	clearFilter();

/*	size_t
	read(
		void* p,
		size_t size
	) {
		return m_device.overlappedRead(p, size);
	}

	bool
	overlappedRead(
		void* p,
		dword_t size,
		OVERLAPPED* overlapped
	) {
		return m_device.overlappedRead(p, size, overlapped);
	}

	bool
	getOverlappedResult(
		OVERLAPPED* overlapped,
		dword_t* actualSize
	) {
		return m_device.getOverlappedResult(overlapped, actualSize);
	}

	size_t
	getOverlappedResult(OVERLAPPED* overlapped) {
		return m_device.getOverlappedResult(overlapped);
	} */
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
sl::String_w
UsbMonitor::getHubSymlink()
{
	sl::String_w string;
	getHubSymlink(&string);
	return string;
}

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
