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
#include "axl_io_win_UsbMonitor.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

bool
UsbMonitor::open(
	const sl::String_w& name,
	Mode mode
) {
	static sl::StringRef_w symlinkPrefix = L"\\\\.\\";

	return m_device.create(
		name.isPrefix(symlinkPrefix) ? name : symlinkPrefix + name,
		mode == Mode_Enumerate ? 0 : GENERIC_READ | GENERIC_WRITE,
		mode == Mode_Enumerate ? FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE : 0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED
	);
}

bool
UsbMonitor::getHubSymlink(sl::String_w* string) {
	wchar_t* p = string->createBuffer(IoctlOutputBufferSize / sizeof(wchar_t));

	dword_t actualSize;
	bool result = m_device.ioctl(
		IOCTL_USBPCAP_GET_HUB_SYMLINK,
		NULL,
		0,
		p,
		IoctlOutputBufferSize,
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

bool
UsbMonitor::setSnapshotLength(size_t size) {
	USBPCAP_IOCTL_SIZE params;
	params.size = (uint32_t)size;

	dword_t actualSize;
	return m_device.ioctl(
		IOCTL_USBPCAP_SET_SNAPLEN_SIZE,
		&params,
		sizeof(params),
		NULL,
		0,
		(dword_t*)&actualSize
	);
}

bool
UsbMonitor::setKernelBufferSize(size_t size) {
	USBPCAP_IOCTL_SIZE params;
	params.size = (uint32_t)size;

	dword_t actualSize;
	return m_device.ioctl(
		IOCTL_USBPCAP_SETUP_BUFFER,
		&params,
		sizeof(params),
		NULL,
		0,
		(dword_t*)&actualSize
	);
}

bool
UsbMonitor::setFilter(uint_t deviceAddress) {
	deviceAddress &= 0x7f; // 1 .. 127

	USBPCAP_ADDRESS_FILTER filter = { 0 };
	filter.addresses[deviceAddress / 32] = 1 << (deviceAddress % 32);

	dword_t actualSize;
	return m_device.ioctl(
		IOCTL_USBPCAP_START_FILTERING,
		&filter,
		sizeof(filter),
		NULL,
		0,
		(dword_t*)&actualSize
	);
}

bool
UsbMonitor::setFilter(
	const uint_t* deviceAddressTable,
	size_t deviceCount
) {
	USBPCAP_ADDRESS_FILTER filter = { 0 };

	for (size_t i = 0; i < deviceCount; i++) {
		uint_t deviceAddress = deviceAddressTable[i];
		deviceAddress &= 0x7f; // 1 .. 127
		filter.addresses[deviceAddress / 32] = 1 << (deviceAddress % 32);
	}

	dword_t actualSize;
	return m_device.ioctl(
		IOCTL_USBPCAP_START_FILTERING,
		&filter,
		sizeof(filter),
		NULL,
		0,
		(dword_t*)&actualSize
	);
}

bool
UsbMonitor::clearFilter() {
	dword_t actualSize;
	return m_device.ioctl(
		IOCTL_USBPCAP_STOP_FILTERING,
		NULL,
		0,
		NULL,
		0,
		(dword_t*)&actualSize
	);
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
