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

enum SerialPortDescMask {
	SerialPortDescMask_Description  = 0x01,
	SerialPortDescMask_Manufacturer = 0x02,
	SerialPortDescMask_HardwareIds  = 0x04,
	SerialPortDescMask_Driver       = 0x08,
	SerialPortDescMask_Location     = 0x10,
	SerialPortDescMask__All         = 0x1f,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class SerialPortDesc: public sl::ListLink {
	friend class SerialPortEnumerator;

protected:
	sl::String m_deviceName; // always queried; the rest depends on the mask
	sl::String m_description;
	sl::String m_manufacturer;
	sl::String m_hardwareIds;
	sl::String m_driver;
	sl::String m_location;

public:
	const sl::String&
	getDeviceName() const {
		return m_deviceName;
	}

	const sl::String&
	getDescription() const {
		return m_description;
	}

	const sl::String&
	getManufacturer() const {
		return m_manufacturer;
	}

	const sl::String&
	getHardwareIds() const {
		return m_hardwareIds;
	}

	const sl::String&
	getDriver() const {
		return m_driver;
	}

	const sl::String&
	getLocation() const {
		return m_location;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
enumerateSerialPorts(
	sl::List<SerialPortDesc>* portList,
	uint_t mask = 0
);

//..............................................................................

} // namespace io
} // namespace axl
