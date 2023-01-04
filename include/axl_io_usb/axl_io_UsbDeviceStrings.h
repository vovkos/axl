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

#define _AXL_IO_USBDEVICESTRINGS_H

#include "axl_io_UsbPch.h"

namespace axl {

#if (_AXL_OS_WIN)

namespace sys {
namespace win {

class DeviceInfo;

} // namespace win
} // namespace sys

namespace io {
namespace win {

class UsbHubDb;

} // namespace win
} // namespace io

#elif (_AXL_OS_LINUX)

namespace sys {
namespace lnx {

class UdevHwdb;

} // namespace lnx
} // namespace sys

#elif (_AXL_OS_DARWIN)

namespace cf {

class MutableDictionary;

} // namespace cf

#endif

namespace io {

class UsbDevice;

//..............................................................................

const char*
getUsbClassString(uint_t usbClass);

const char*
getUsbSpeedString(libusb_speed speed);

//..............................................................................

enum UsbDeviceStringId {
	UsbDeviceStringId_Description            = 0x01,
	UsbDeviceStringId_Manufacturer           = 0x02,
	UsbDeviceStringId_Driver                 = 0x04,
	UsbDeviceStringId_ManufacturerDescriptor = 0x08,
	UsbDeviceStringId_ProductDescriptor      = 0x10,
	UsbDeviceStringId_SerialNumberDescriptor = 0x20,

	UsbDeviceStringId_Database =
		UsbDeviceStringId_Description |
		UsbDeviceStringId_Manufacturer |
		UsbDeviceStringId_Driver,

	UsbDeviceStringId_Descriptors =
		UsbDeviceStringId_ManufacturerDescriptor |
		UsbDeviceStringId_ProductDescriptor |
		UsbDeviceStringId_SerialNumberDescriptor,

	UsbDeviceStringId_All =
		UsbDeviceStringId_Database |
		UsbDeviceStringId_Descriptors,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UsbDeviceStrings {
	sl::String m_description;  // from the system DB (fallback to descriptor)
	sl::String m_manufacturer; // from the system DB (fallback to descriptor)
	sl::String m_driver;
	sl::String m_manufacturerDescriptor;
	sl::String m_productDescriptor;
	sl::String m_serialNumberDescriptor;

#if (_AXL_OS_WIN)
	bool
	queryStrings(
		win::UsbHubDb* hubDb,
		sys::win::DeviceInfo* deviceInfo,
		uint_t port,
		uint_t manufacturerDescriptorId,
		uint_t productDescriptorId,
		uint_t serialNumberDescriptorId,
		sl::String_w* string,
		uint_t mask // UsbDeviceStringId-s
	);
#elif (_AXL_OS_LINUX)
	bool
	queryStrings(
		const sys::lnx::UdevHwdb& hwdb,
		io::UsbDevice* device,
		const libusb_device_descriptor& deviceDescriptor,
		sl::String* sysFsPrefix,
		sl::String* string,
		sl::String_utf16* string_utf16,
		uint_t mask // UsbDeviceStringId-s
	);
#elif (_AXL_OS_DARWIN)
	bool
	queryStrings(
		const cf::MutableDictionary& usbDeviceDict,
		cf::MutableDictionary* propertyMatchDict,
		io::UsbDevice* device,
		const libusb_device_descriptor& deviceDescriptor,
		sl::String* string,
		sl::String_utf16* string_utf16,
		uint_t mask // UsbDeviceStringId-s
	);
#endif
};

//..............................................................................

} // namespace io
} // namespace axl
