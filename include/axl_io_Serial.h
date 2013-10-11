// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SERIAL_H

#include "axl_io_Transport.h"
#include "axl_exe_WorkerThread.h"

#include "axl_io_win_Serial.h"

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

enum ESerialFlow
{
	ESerialFlow_None = 0,
	ESerialFlow_RtsCts,
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

enum ESerialData
{
	ESerialData_7 = 7,
	ESerialData_8 = 8,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESerialStop
{
	ESerialStop_1  = 0,
	ESerialStop_15 = 1,
	ESerialStop_2  = 2,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESerialLines
{
	ESerialLines_Dtr     = 0x01,
	ESerialLines_Rts     = 0x02,
	ESerialLines_Dsr     = 0x04,
	ESerialLines_Cts     = 0x08,
	ESerialLines_Dcd     = 0x10,
	ESerialLines_Ring    = 0x20,

	ESerialLines_Control = 0x03,
	ESerialLines_Status  = 0x3c,
	ESerialLines_All     = 0xff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ESerialSettings
{
	ESerialSettings_BaudRate        = 0x01,
	ESerialSettings_FlowControl     = 0x02,
	ESerialSettings_Parity          = 0x04,
	ESerialSettings_DataBits        = 0x08,
	ESerialSettings_StopBits        = 0x10,
	ESerialSettings_IntervalTimeout = 0x20,
	ESerialSettings_All             = 0xff,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSerialSettings
{
	ESerialBaud m_BaudRate;
	ESerialFlow m_FlowControl;
	ESerialParity m_Parity;
	ESerialData m_DataBits;
	ESerialStop m_StopBits;
	uint_t m_IntervalTimeout;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CSerial
{
protected:
	g::win::CFileHandle m_Serial;
	ref::CPtrT <exe::CWorkerThread> m_WorkerThread;
	exe::CFunction m_OnEvent;

public:
	~CSerial ()
	{
		Close ();
	}

	bool 
	Open (const char* pName);

	void 
	Close ();

	bool 
	SetSettings (
		const TSerialSettings* pSettings,
		int Mask = 0
		);

	bool 
	GetSettings (TSerialSettings* pSettings);

	bool
	SetControlLines (
		int Lines,
		int Mask
		);

	int
	GetControlLines ();

	int
	GetStatusLines ();

	bool
	SetEventHandler (
		int Mask,
		const exe::CFunction& OnEvent
		);
};

//.............................................................................

} // namespace io
} // namespace axl

