// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SERIAL_H

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_io_win_Serial.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_io_psx_Serial.h"
#endif

namespace axl {
namespace io {

//.............................................................................

enum SerialFlowControlKind
{
	SerialFlowControlKind_None = 0,
	SerialFlowControlKind_RtsCts,
	SerialFlowControlKind_XonXoff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SerialStopBitsKind
{
	SerialStopBitsKind_1  = 0,
	SerialStopBitsKind_15 = 1,
	SerialStopBitsKind_2  = 2,
};


//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SerialParityKind
{
	SerialParityKind_None  = 0,
	SerialParityKind_Odd   = 1,
	SerialParityKind_Even  = 2,
	SerialParityKind_Mark  = 3,
	SerialParityKind_Space = 4,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SerialStatusLineKind
{
	SerialStatusLineKind_Cts  = 0x01,
	SerialStatusLineKind_Dsr  = 0x02,
	SerialStatusLineKind_Ring = 0x04,
	SerialStatusLineKind_Dcd  = 0x08,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SerialSettingKind
{
	SerialSettingKind_BaudRate     = 0x01,
	SerialSettingKind_FlowControl  = 0x02,
	SerialSettingKind_DataBits     = 0x04,
	SerialSettingKind_StopBits     = 0x08,
	SerialSettingKind_Parity       = 0x10,
	SerialSettingKind_ReadInterval = 0x20,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct SerialSettings
{
	uint_t m_baudRate;
	SerialFlowControlKind m_flowControl;
	uint_t m_dataBits;
	SerialStopBitsKind m_stopBits;
	SerialParityKind m_parity;
	uint_t m_readInterval; // inverse of COMMTIMEOUTS.ReadIntervalTimeout (which always makes me confused):
	                       // 0  -- return immediatly
	                       // -1 -- wait for the buffer to fill completely

	SerialSettings ()
	{
		setup (38400);
	}

	SerialSettings (
		uint_t baudRate,
		SerialFlowControlKind flowControl = SerialFlowControlKind_None,
		uint_t dataBits = 8,
		SerialStopBitsKind stopBits = SerialStopBitsKind_1,
		SerialParityKind parity = SerialParityKind_None,
		uint_t readInterval = 10 // 10ms can be used in general case
		)
	{
		setup (baudRate, flowControl, dataBits, stopBits, parity, readInterval);
	}

	void 
	setup (
		uint_t baudRate,
		SerialFlowControlKind flowControl = SerialFlowControlKind_None,
		uint_t dataBits = 8,
		SerialStopBitsKind stopBits = SerialStopBitsKind_1,
		SerialParityKind parity = SerialParityKind_None,
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
#if (_AXL_ENV == AXL_ENV_WIN)
	win::Serial m_serial;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	psx::Serial m_serial;
#endif

public:
	bool
	isOpen ()
	{
		return m_serial.isOpen ();
	}

	bool
	open (const char* name)
	{
#if (_AXL_ENV == AXL_ENV_WIN)
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

#if (_AXL_ENV == AXL_ENV_WIN)
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
#elif (_AXL_ENV == AXL_ENV_POSIX)
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

} // namespace io
} // namespace axl

