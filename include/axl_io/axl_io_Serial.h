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

#define _AXL_IO_SERIAL_H

#include "axl_io_File.h"
#include "axl_io_SerialSettings.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_Serial.h"
#elif (_AXL_OS_POSIX)
#	include "axl_io_psx_Serial.h"
#endif

namespace axl {
namespace io {

//..............................................................................

class Serial
{
public:
#if (_AXL_OS_WIN)
	win::Serial m_serial;
#elif (_AXL_OS_POSIX)
	psx::Serial m_serial;
#endif

public:
	bool
	isOpen()
	{
		return m_serial.isOpen();
	}

	bool
	open(
		const sl::StringRef& name,
		uint_t flags = 0
		);

	void
	close()
	{
		return m_serial.close();
	}

	bool
	setSettings(
		const SerialSettings* settings,
		uint_t mask = -1
		);

	bool
	getSettings(SerialSettings* settings);

	bool
	setDtr(bool isSet)
	{
		return m_serial.setDtr(isSet);
	}

	bool
	setRts(bool isSet)
	{
		return m_serial.setRts(isSet);
	}

	bool
	setBreakCondition(bool isSet)
	{
		return m_serial.setBreakCondition(isSet);
	}

	uint_t
	getStatusLines();

#if (_AXL_OS_WIN)
	size_t
	read(
		void* p,
		size_t size
		) const
	{
		return m_serial.overlappedRead(p, size);
	}

	size_t
	write(
		const void* p,
		size_t size
		)
	{
		return m_serial.overlappedWrite(p, size);
	}
#elif (_AXL_OS_POSIX)
	size_t
	read(
		void* p,
		size_t size
		) const
	{
		return m_serial.read(p, size);
	}

	size_t
	write(
		const void* p,
		size_t size
		)
	{
		return m_serial.write(p, size);
	}
#endif

};

//..............................................................................

class SerialPortDesc: public sl::ListLink
{
	friend class SerialPortEnumerator;

protected:
	sl::String m_deviceName;
	sl::String m_description;

public:
	const sl::String&
	getDeviceName()
	{
		return m_deviceName;
	}

	const sl::String&
	getDescription()
	{
		return m_description;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
createSerialPortDescList(sl::List<SerialPortDesc>* portList);

//..............................................................................

} // namespace io
} // namespace axl
