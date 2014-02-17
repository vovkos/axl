#include "pch.h"
#include "axl_io_psx_Serial.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

uint_t
CSerial::GetStatusLines () const
{
	int Lines = 0;
	int Result = ioctl (m_h, TIOCMGET, &Lines);
	if (Result == -1)
	{
		err::SetLastSystemError ();
		return -1;
	}

	return Lines;
}

bool
CSerial::SetDtr (bool IsSet)
{
	int Lines = TIOCM_DTR;
	int Result = ioctl (m_h, IsSet ? TIOCMBIS : TIOCMBIC, &Lines);
	return err::Complete (Result != -1);
}

bool
CSerial::SetRts (bool IsSet)
{
	int Lines = TIOCM_RTS;
	int Result = ioctl (m_h, IsSet ? TIOCMBIS : TIOCMBIC, &Lines);
	return err::Complete (Result != -1);
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

