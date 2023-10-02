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

#include "axl_io_UsbEnumerator.h"

namespace axl {
namespace io {

//..............................................................................

enum UsbMonEnumFlag {
	UsbMonEnumFlag_Hubs             = UsbEnumFlag_Hubs,
	// OR-ed with UsbDeviceStringId
};

//..............................................................................

struct UsbMonDeviceDesc:
	sl::ListLink,
	UsbDeviceStrings {

	sl::String m_captureDeviceName;
	uint_t m_captureDeviceId;

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

#if (_AXL_OS_WIN)
	uint_t m_devInst;
#endif
};

//..............................................................................

size_t
enumerateUsbMonDevices(
	sl::List<UsbMonDeviceDesc>* deviceList,
	uint_t flags = 0
);

//..............................................................................

} // namespace io
} // namespace axl
