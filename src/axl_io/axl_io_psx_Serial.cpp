#include "pch.h"
#include "axl_io_psx_Serial.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

uint_t
CSerial::getStatusLines () const
{
	int lines = 0;
	int result = ioctl (m_h, TIOCMGET, &lines);
	if (result == -1)
	{
		err::setLastSystemError ();
		return -1;
	}

	return lines;
}

bool
CSerial::setDtr (bool isSet)
{
	int lines = TIOCM_DTR;
	int result = ioctl (m_h, isSet ? TIOCMBIS : TIOCMBIC, &lines);
	return err::complete (result != -1);
}

bool
CSerial::setRts (bool isSet)
{
	int lines = TIOCM_RTS;
	int result = ioctl (m_h, isSet ? TIOCMBIS : TIOCMBIC, &lines);
	return err::complete (result != -1);
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

