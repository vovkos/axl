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
	public sl::ListLink,
	public io::win::File {
public:
	bool
	getConfigurationDescriptor(
		sl::Array<char>* descriptor,
		uint_t port
	);

	bool
	getStringDescriptor(
		sl::String_w* string,
		uint_t port,
		uchar_t descriptorId,
		ushort_t languageId
	);

protected:
	size_t
	getDescriptorImpl(
		void* p, // at least sizeof(USB_DESCRIPTOR_REQUEST) + length
		uint_t port,
		ushort_t value,
		ushort_t index,
		ushort_t length
	);
};

//..............................................................................

class UsbHubDb {
protected:
	sl::List<UsbHub> m_hubList;
	sl::SimpleHashTable<dword_t, UsbHub*> m_hubMap; // DEVINST -> HUB

public:
	bool
	isEmpty() const {
		return m_hubList.isEmpty();
	}

	void
	clear() {
		m_hubList.clear();
		m_hubMap.clear();
	}

	UsbHub*
	getUsbHub(sys::win::DeviceInfo* deviceInfo);
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
