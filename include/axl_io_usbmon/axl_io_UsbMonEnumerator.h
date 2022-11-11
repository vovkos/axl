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

#define _AXL_IO_USBMONNUMERATOR_H

#include "axl_sl_String.h"
#include "axl_sl_List.h"

namespace axl {
namespace io {

//..............................................................................

enum UsbMonDeviceDescMask {
	UsbMonDeviceDescMask_Hubs                   = 0x0001,
	UsbMonDeviceDescMask_Description            = 0x0002,
	UsbMonDeviceDescMask_Manufacturer           = 0x0004,
	UsbMonDeviceDescMask_Driver                 = 0x0008,
	UsbMonDeviceDescMask_ManufacturerDescriptor = 0x0020,
	UsbMonDeviceDescMask_ProductDescriptor      = 0x0040,
	UsbMonDeviceDescMask_SerialNumberDescriptor = 0x0080,
	UsbMonDeviceDescMask_Descriptors            = 0x00e0,
	UsbMonDeviceDescMask_All                    = 0x00ff,
	UsbMonDeviceDescMask_Default                =
		UsbMonDeviceDescMask_Description |
		UsbMonDeviceDescMask_Manufacturer,
};

enum UsbMonDeviceDescFlag {
	UsbMonDeviceDescFlag_Hub              = 0x01,
	UsbMonDeviceDescFlag_DeviceDescriptor = 0x02,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum UsbMonDeviceSpeed {
	UsbMonDeviceSpeed_Unknown = 0,
	UsbMonDeviceSpeed_Low,
	UsbMonDeviceSpeed_Full,
	UsbMonDeviceSpeed_High,
	UsbMonDeviceSpeed_Super,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getUsbMonSpeedString(UsbMonDeviceSpeed speed);

//..............................................................................

struct UsbMonDeviceDesc: sl::ListLink {
	sl::String m_captureDeviceName;
	sl::String m_description;  // from the system DB (fallback to descriptor)
	sl::String m_manufacturer; // from the system DB (fallback to descriptor)
	sl::String m_driver;
	sl::String m_manufacturerDescriptor;
	sl::String m_productDescriptor;
	sl::String m_serialNumberDescriptor;

	UsbMonDeviceSpeed m_speed;

	uint_t m_vendorId;
	uint_t m_productId;
	uint_t m_address;
	uint_t m_class;
	uint_t m_subClass;
	uint_t m_manufacturerDescriptorId;
	uint_t m_productDescriptorId;
	uint_t m_serialNumberDescriptorId;
	uint_t m_flags;

	UsbMonDeviceDesc() {
		memset(this, 0, sizeof(UsbMonDeviceDesc));
	}
};

//..............................................................................

size_t
enumerateUsbMonDevices(
	sl::List<UsbMonDeviceDesc>* deviceList,
	uint_t mask = UsbMonDeviceDescMask_Default
);

//..............................................................................

} // namespace io
} // namespace axl
