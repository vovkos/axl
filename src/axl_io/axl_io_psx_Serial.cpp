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
#include "axl_io_psx_Serial.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

uint_t
Serial::getStatusLines() const
{
	int lines = 0;
	int result = ::ioctl(m_h, TIOCMGET, &lines);
	if (result == -1)
	{
		err::setLastSystemError();
		return -1;
	}

	return lines;
}

bool
Serial::setDtr(bool isSet)
{
	int lines = TIOCM_DTR;
	int result = ::ioctl(m_h, isSet ? TIOCMBIS : TIOCMBIC, &lines);
	return err::complete(result != -1);
}

bool
Serial::setRts(bool isSet)
{
	int lines = TIOCM_RTS;
	int result = ::ioctl(m_h, isSet ? TIOCMBIS : TIOCMBIC, &lines);
	return err::complete(result != -1);
}

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
