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

#define _AXL_IO_USBHUBDB_H

#include "axl_io_UsbPch.h"
#include "axl_sl_HashTable.h"

namespace axl {
namespace sys {
namespace win {

class DeviceInfo;

} // namespace win
} // namespace io

namespace io {
namespace win {

//..............................................................................

class UsbHub:
	public io::win::File {
protected:
	sl::String_w m_path;

public:
	const sl::String_w&
	getPath() const {
		return m_path;
	}

	bool
	open(const sl::StringRef_w& path);

	bool
	getConfigurationDescriptor(
		sl::Array<char>* descriptor,
		uint_t port
	) const;

	bool
	getStringDescriptor(
		sl::String_w* string,
		uint_t port,
		uchar_t descriptorId,
		ushort_t languageId
	) const;

protected:
	size_t
	getDescriptorImpl(
		void* p, // at least sizeof(USB_DESCRIPTOR_REQUEST) + length
		uint_t port,
		ushort_t value,
		ushort_t index,
		ushort_t length
	) const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
UsbHub::open(const sl::StringRef_w& path) {
	bool result = create(
		path,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING
	);

	if (!result)
		return false;

	m_path = path;
	return true;
}

//..............................................................................

class UsbHubDb {
protected:
	sl::SimpleHashTable<dword_t, UsbHub> m_hubMap; // DEVINST -> HUB

public:
	bool
	isEmpty() const {
		return m_hubMap.isEmpty();
	}

	void
	clear() {
		m_hubMap.clear();
	}

	UsbHub*
	getUsbHub(sys::win::DeviceInfo* deviceInfo);
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
