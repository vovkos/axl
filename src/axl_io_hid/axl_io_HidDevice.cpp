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
#include "axl_io_HidDevice.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getHidBusTypeString(hid_bus_type busType) {
	static const char* stringTable[] = {
		"unknown",    // HID_API_BUS_UNKNOWN = 0x00,
		"USB",        // HID_API_BUS_USB = 0x01,
		"Bluetooth",  // HID_API_BUS_BLUETOOTH = 0x02,
		"I2C",        // HID_API_BUS_I2C = 0x03,
		"SPI",        // HID_API_BUS_SPI = 0x04,
	};

	return (size_t)busType < countof(stringTable) ?
		stringTable[busType] :
		stringTable[0];
}

//..............................................................................

} // namespace io
} // namespace axl
