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

#define _AXL_IO_WIN_USBPCAP_H

#include "axl_io_win_UsbPcapPch.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

class UsbPcap {
protected:
	enum {
		IoctlOutpurBufferSize = 1024, // as suggested by USBPcapCMD
	};

protected:
	axl::io::win::File m_device;

public:
	bool
	isOpen() {
		return m_device.isOpen();
	}

	bool
	open(
		const sl::String_w& name,
		uint_t flags = 0
	);

	void
	close() {
		m_device.close();
	}

	sl::String_w
	getHubSymlink();

	bool
	getHubSymlink(sl::String_w* symlink);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
sl::String_w
UsbPcap::getHubSymlink() {
	sl::String_w string;
	getHubSymlink(&string);
	return string;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
