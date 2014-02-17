// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_PSX_SERIAL_H

#include "axl_io_psx_Fd.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class CSerial: public CFd
{
public:
	bool
	Open (const char* pName)
	{
		return
			CFd::Open (pName, O_RDWR | O_NONBLOCK | O_NOCTTY, 0) &&
			SetBlockingMode (true);
	}

	bool
	GetAttr (termios* pAttr) const
	{
		int Result = tcgetattr (m_h, pAttr);
		return err::Complete (Result != -1);
	}

	bool
	SetAttr (
		const termios* pAttr,
		int Action = TCSANOW
		)
	{
		int Result = tcsetattr (m_h, Action, pAttr);
		return err::Complete (Result != -1);
	}

	bool
	Drain ()
	{
		int Result = tcdrain (m_h);
		return err::Complete (Result != -1);
	}

	bool
	Flush (int QueueSelector = TCIOFLUSH)
	{
		int Result = tcflush (m_h, QueueSelector);
		return err::Complete (Result != -1);
	}

	bool
	Flow (int Action)
	{
		int Result = tcflow (m_h, Action);
		return err::Complete (Result != -1);
	}

	uint_t
	GetStatusLines () const;

	bool
	SetDtr (bool IsSet);

	bool
	SetRts (bool IsSet);

	bool
	Wait (uint_t Mask)
	{
		int Result = ioctl (m_h, TIOCMIWAIT, Mask);
		return err::Complete (Result != -1);
	}
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
