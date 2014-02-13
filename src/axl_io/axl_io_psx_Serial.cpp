#include "pch.h"
#include "axl_io_psx_Serial.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

bool
CSerial::Open (const char* pName)
{
	Close ();

	// on POSIX opening normally (in blocking mode) means waiting for DCD -- wtf?

	m_h = open (pName, O_RDWR | O_NONBLOCK | O_NOCTTY, 0);
	if (m_h == -1)
		return err::FailWithLastSystemError ();

	// restore blocking mode

	return SetBlockingMode (true);
}

bool
CSerial::SetBlockingMode (bool IsBlocking)
{
	int Result = ::fcntl (m_h, F_GETFL, 0);
	if (Result == -1)
		return err::FailWithLastSystemError ();

	int Flags = Result;
	if (IsBlocking)
		Flags &= ~O_NONBLOCK;
	else
		Flags |= O_NONBLOCK;

	Result = ::fcntl (m_h, F_SETFL, Flags);
	return err::Complete (Result != -1);
}

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

size_t
CSerial::Read (
	void* p,
	size_t Size
	) const
{
	size_t ActualSize = read (m_h, p, Size);
	if (ActualSize == -1)
		err::SetLastSystemError ();

	return ActualSize;
}

size_t
CSerial::Write (
	const void* p,
	size_t Size
	)
{
	size_t ActualSize = write (m_h, p, Size);
	if (ActualSize == -1)
		err::SetLastSystemError ();

	return ActualSize;
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

