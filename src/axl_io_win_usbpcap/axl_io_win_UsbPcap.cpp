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
#include "axl_io_win_UsbPcap.h"
#include "USBPcap.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

bool
UsbPcap::open(
	const sl::String_w& name,
	uint_t flags
) {
	bool isReadOnly = (flags & io::FileFlag_ReadOnly) != 0;
	static sl::StringRef_w symlinkPrefix = L"\\\\.\\";

	return m_device.create(
		name.isPrefix(symlinkPrefix) ? name : symlinkPrefix + name,
		isReadOnly ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE,
		isReadOnly ? FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE : 0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED
	);
}

bool
UsbPcap::getHubSymlink(sl::String_w* string) {
	wchar_t* p = string->createBuffer(IoctlOutpurBufferSize / sizeof(wchar_t));

	dword_t actualSize;
	bool result = m_device.ioctl(
		IOCTL_USBPCAP_GET_HUB_SYMLINK,
		NULL,
		0,
		p,
		IoctlOutpurBufferSize,
		(dword_t*)&actualSize
	);

	if (!result)
		return false;

	if (!actualSize) {
		err::setError("missing USB hub symlink information");
		return false;
	}

	string->overrideLength(actualSize / sizeof(wchar_t) - 1);
	return true;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
