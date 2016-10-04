// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SERIAL_H

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_Serial.h"
#elif (_AXL_OS_POSIX)
#	include "axl_io_psx_Serial.h"
#endif

namespace axl {
namespace io {

//.............................................................................

enum SerialFlowControl
{
	SerialFlowControl_None = 0,
	SerialFlowControl_RtsCts,
	SerialFlowControl_XonXoff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SerialStopBits
{
	SerialStopBits_1  = 0,
	SerialStopBits_15 = 1,
	SerialStopBits_2  = 2,
};


//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SerialParity
{
	SerialParity_None  = 0,
	SerialParity_Odd   = 1,
	SerialParity_Even  = 2,
	SerialParity_Mark  = 3,
	SerialParity_Space = 4,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SerialStatusLine
{
	SerialStatusLine_Cts  = 0x01,
	SerialStatusLine_Dsr  = 0x02,
	SerialStatusLine_Ring = 0x04,
	SerialStatusLine_Dcd  = 0x08,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SerialSettingId
{
	SerialSettingId_BaudRate     = 0x01,
	SerialSettingId_FlowControl  = 0x02,
	SerialSettingId_DataBits     = 0x04,
	SerialSettingId_StopBits     = 0x08,
	SerialSettingId_Parity       = 0x10,
	SerialSettingId_ReadInterval = 0x20,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct SerialSettings
{
	uint_t m_baudRate;
	SerialFlowControl m_flowControl;
	uint_t m_dataBits;
	SerialStopBits m_stopBits;
	SerialParity m_parity;
	uint_t m_readInterval; // inverse of COMMTIMEOUTS.ReadIntervalTimeout (which always makes me confused):
	                       // 0  -- return immediatly
	                       // -1 -- wait for the buffer to fill completely

	SerialSettings ()
	{
		setup (38400);
	}

	SerialSettings (
		uint_t baudRate,
		SerialFlowControl flowControl = SerialFlowControl_None,
		uint_t dataBits = 8,
		SerialStopBits stopBits = SerialStopBits_1,
		SerialParity parity = SerialParity_None,
		uint_t readInterval = 10 // 10ms can be used in general case
		)
	{
		setup (baudRate, flowControl, dataBits, stopBits, parity, readInterval);
	}

	void 
	setup (
		uint_t baudRate,
		SerialFlowControl flowControl = SerialFlowControl_None,
		uint_t dataBits = 8,
		SerialStopBits stopBits = SerialStopBits_1,
		SerialParity parity = SerialParity_None,
		uint_t readInterval = 10
		)
	{
		m_baudRate = baudRate;
		m_flowControl = flowControl;
		m_dataBits = dataBits;
		m_stopBits = stopBits;
		m_parity = parity;
		m_readInterval = readInterval;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
	isOpen ()
	{
		return m_serial.isOpen ();
	}

	bool
	open (const sl::StringRef& name)
	{
#if (_AXL_OS_WIN)
		return m_serial.open (name, FILE_FLAG_OVERLAPPED);
#else
		return m_serial.open (name);
#endif
	}

	void
	close ()
	{
		return m_serial.close ();
	}

	bool
	setSettings (
		const SerialSettings* settings,
		uint_t mask = -1
		);

	bool
	getSettings (SerialSettings* settings);

	bool
	setDtr (bool isSet)
	{
		return m_serial.setDtr (isSet);
	}

	bool
	setRts (bool isSet)
	{
		return m_serial.setRts (isSet);
	}

	uint_t
	getStatusLines ();

#if (_AXL_OS_WIN)
	size_t
	read (
		void* p,
		size_t size
		);

	size_t
	write (
		const void* p,
		size_t size
		);
#elif (_AXL_OS_POSIX)
	size_t
	read (
		void* p,
		size_t size
		)
	{
		return m_serial.read (p, size);
	}

	size_t
	write (
		const void* p,
		size_t size
		)
	{
		return m_serial.write (p, size);
	}
#endif

};

//.............................................................................

class SerialPortDesc: public sl::ListLink
{
	friend class SerialPortEnumerator;

protected:
	sl::String m_deviceName;
	sl::String m_description;

public:
	sl::String 
	getDeviceName ()
	{
		return m_deviceName;
	}
		
	sl::String
	getDescription ()
	{
		return m_description;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
createSerialPortDescList (sl::StdList <SerialPortDesc>* portList);

//.............................................................................

} // namespace io
} // namespace axl
