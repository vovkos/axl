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

enum ESerialFlowControl
{
	ESerialFlowControl_None = 0,
	ESerialFlowControl_RtsCts,
	ESerialFlowControl_XonXoff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESerialStopBits
{
	ESerialStopBits_1  = 0,
	ESerialStopBits_15 = 1,
	ESerialStopBits_2  = 2,
};


//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESerialParity
{
	ESerialParity_None  = 0,
	ESerialParity_Odd   = 1,
	ESerialParity_Even  = 2,
	ESerialParity_Mark  = 3,
	ESerialParity_Space = 4,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESerialStatusLine
{
	ESerialStatusLine_Cts  = 0x01,
	ESerialStatusLine_Dsr  = 0x02,
	ESerialStatusLine_Ring = 0x04,
	ESerialStatusLine_Dcd  = 0x08,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESerialSetting
{
	ESerialSetting_BaudRate     = 0x01,
	ESerialSetting_FlowControl  = 0x02,
	ESerialSetting_DataBits     = 0x04,
	ESerialSetting_StopBits     = 0x08,
	ESerialSetting_Parity       = 0x10,
	ESerialSetting_ReadInterval = 0x20,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSerialSettings
{
	uint_t m_baudRate;
	ESerialFlowControl m_flowControl;
	uint_t m_dataBits;
	ESerialStopBits m_stopBits;
	ESerialParity m_parity;
	uint_t m_readInterval; // inverse of COMMTIMEOUTS.ReadIntervalTimeout (which always makes me confused):
	                       // 0  -- return immediatly
	                       // -1 -- wait for the buffer to fill completely

	TSerialSettings ()
	{
		setup (38400);
	}

	TSerialSettings (
		uint_t baudRate,
		ESerialFlowControl flowControl = ESerialFlowControl_None,
		uint_t dataBits = 8,
		ESerialStopBits stopBits = ESerialStopBits_1,
		ESerialParity parity = ESerialParity_None,
		uint_t readInterval = 10 // 10ms can be used in general case
		)
	{
		setup (baudRate, flowControl, dataBits, stopBits, parity, readInterval);
	}

	void 
	setup (
		uint_t baudRate,
		ESerialFlowControl flowControl = ESerialFlowControl_None,
		uint_t dataBits = 8,
		ESerialStopBits stopBits = ESerialStopBits_1,
		ESerialParity parity = ESerialParity_None,
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

class CSerial
{
public:
#if (_AXL_ENV == AXL_ENV_WIN)
	win::CSerial m_serial;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	psx::CSerial m_serial;
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
		const TSerialSettings* settings,
		uint_t mask = -1
		);

	bool
	getSettings (TSerialSettings* settings);

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

