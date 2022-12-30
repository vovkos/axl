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

#define _AXL_IO_USBDEVICEDESC_H

#include "axl_sl_String.h"

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

namespace io {

class UsbDevice;

} // namespace io

namespace sys {
namespace lnx {

class UdevHwdb;

} // namespace lnx
} // namespace sys

#endif

namespace io {

//..............................................................................

const char*
getUsbClassString(uint_t usbClass);

const char*
getUsbSpeedString(libusb_speed speed);

//..............................................................................

enum UsbDeviceDescFlag {
	UsbDeviceDescFlag_Description            = 0x01,
	UsbDeviceDescFlag_Manufacturer           = 0x02,
	UsbDeviceDescFlag_Driver                 = 0x04,
	UsbDeviceDescFlag_ManufacturerDescriptor = 0x10,
	UsbDeviceDescFlag_ProductDescriptor      = 0x20,
	UsbDeviceDescFlag_SerialNumberDescriptor = 0x40,

	UsbDeviceDescFlag_Descriptors =
		UsbDeviceDescFlag_ManufacturerDescriptor |
		UsbDeviceDescFlag_ProductDescriptor |
		UsbDeviceDescFlag_SerialNumberDescriptor,

	UsbDeviceDescFlag_All =
		UsbDeviceDescFlag_Description |
		UsbDeviceDescFlag_Manufacturer |
		UsbDeviceDescFlag_Driver |
		UsbDeviceDescFlag_Descriptors,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UsbDeviceDesc {
	sl::String m_description;  // from the system DB (fallback to descriptor)
	sl::String m_manufacturer; // from the system DB (fallback to descriptor)
	sl::String m_driver;
	sl::String m_manufacturerDescriptor;
	sl::String m_productDescriptor;
	sl::String m_serialNumberDescriptor;

	uint16_t m_vendorId;
	uint16_t m_productId;

	uint8_t m_address;
	uint8_t m_port;
	uint8_t m_class;
	uint8_t m_subClass;
	uint8_t m_speed; // libusb_speed
	uint8_t m_manufacturerDescriptorId;
	uint8_t m_productDescriptorId;
	uint8_t m_serialNumberDescriptorId;

	UsbDeviceDesc() {
		memset(&m_vendorId, 0, sizeof(UsbDeviceDesc) - offsetof(UsbDeviceDesc, m_vendorId));
	}

#if (_AXL_OS_WIN)
	bool
	queryStrings(
		win::UsbHubDb* hubDb,
		sys::win::DeviceInfo* deviceInfo,
		uint_t port,
		sl::String_w* string,
		uint_t flags
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
		uint_t flags
	);
#endif
};

//..............................................................................

} // namespace io
} // namespace axl
