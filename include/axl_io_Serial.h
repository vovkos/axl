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

enum ESerialBaud
{
	ESerialBaud_110    = 110,
	ESerialBaud_300    = 300,
	ESerialBaud_600    = 600,
	ESerialBaud_1200   = 1200,
	ESerialBaud_2400   = 2400,
	ESerialBaud_4800   = 4800,
	ESerialBaud_9600   = 9600,
	ESerialBaud_14400  = 14400,
	ESerialBaud_19200  = 19200,
	ESerialBaud_38400  = 38400,
	ESerialBaud_56000  = 56000,
	ESerialBaud_57600  = 57600,
	ESerialBaud_115200 = 115200,
	ESerialBaud_128000 = 128000,
	ESerialBaud_256000 = 256000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESerialDataBits
{
	ESerialDataBits_7 = 7,
	ESerialDataBits_8 = 8,
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

enum ESerialFlowControl
{
	ESerialFlowControl_None = 0,
	ESerialFlowControl_RtsCts,
	ESerialFlowControl_XonXoff,
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
	ESerialSetting_BaudRate        = 0x01,
	ESerialSetting_DataBits        = 0x02,
	ESerialSetting_StopBits        = 0x04,
	ESerialSetting_Parity          = 0x08,
	ESerialSetting_FlowControl     = 0x10,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSerialSettings
{
	uint_t m_BaudRate;
	uint_t m_DataBits;
	ESerialStopBits m_StopBits;
	ESerialParity m_Parity;
	ESerialFlowControl m_FlowControl;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CSerial
{
public:
#if (_AXL_ENV == AXL_ENV_WIN)
	win::CSerial m_Serial;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	psx::CSerial m_Serial;
#endif

public:
	bool
	IsOpen ()
	{
		return m_Serial.IsOpen ();
	}

	bool
	Open (const char* pName)
	{
#if (_AXL_ENV == AXL_ENV_WIN)
		return m_Serial.Open (pName, FILE_FLAG_OVERLAPPED);
#else
		return m_Serial.Open (pName);
#endif
	}

	void
	Close ()
	{
		return m_Serial.Close ();
	}

	bool
	SetSettings (
		const TSerialSettings* pSettings,
		uint_t Mask = -1
		);

	bool
	GetSettings (TSerialSettings* pSettings);

	bool
	SetDtr (bool IsSet)
	{
		return m_Serial.SetDtr (IsSet);
	}

	bool
	SetRts (bool IsSet)
	{
		return m_Serial.SetRts (IsSet);
	}

	uint_t
	GetStatusLines ();

#if (_AXL_ENV == AXL_ENV_WIN)
	size_t
	Read (
		void* p,
		size_t Size
		);

	size_t
	Write (
		const void* p,
		size_t Size
		);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	size_t
	Read (
		void* p,
		size_t Size
		)
	{
		return m_Serial.Read (p, Size);
	}

	size_t
	Write (
		const void* p,
		size_t Size
		)
	{
		return m_Serial.Write (p, Size);
	}
#endif

};

//.............................................................................

} // namespace io
} // namespace axl

