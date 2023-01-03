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

#define _AXL_IO_SERIALPORTENUMERATOR_H

namespace axl {
namespace io {

class SerialPortEnumerator;

//..............................................................................

enum SerialPortDescFlag {
	SerialPortDescFlag_Description  = 0x01,
	SerialPortDescFlag_Manufacturer = 0x02,
	SerialPortDescFlag_HardwareIds  = 0x04,
	SerialPortDescFlag_Driver       = 0x08,
	SerialPortDescFlag_Location     = 0x10,

	SerialPortDescFlag_All =
		SerialPortDescFlag_Description |
		SerialPortDescFlag_Manufacturer |
		SerialPortDescFlag_HardwareIds |
		SerialPortDescFlag_Driver |
		SerialPortDescFlag_Location,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct SerialPortDesc: sl::ListLink {
	sl::String m_deviceName; // always queried; the rest depends on the mask
	sl::String m_description;
	sl::String m_manufacturer;
	sl::String m_hardwareIds;
	sl::String m_driver;
	sl::String m_location;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
enumerateSerialPorts(
	sl::List<SerialPortDesc>* portList,
	uint_t flags = 0
);

//..............................................................................

} // namespace io
} // namespace axl
