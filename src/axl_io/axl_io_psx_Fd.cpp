#include "pch.h"
#include "axl_io_psx_Fd.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

bool
CFd::Open (
	const char* pFileName,
	uint_t OpenFlags,
	mode_t Mode
	)
{
	Close ();

	m_h = open (pFileName, OpenFlags, Mode);
	return err::Complete (m_h != -1);
}

bool
CFd::SetBlockingMode (bool IsBlocking)
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

size_t
CFd::GetIncomingDataSize ()
{
	int Value;
	int Result = ::ioctl (m_h, FIONREAD, &Value);
	if (Result == -1)
	{
		err::SetLastSystemError ();
		return -1;
	}

	return Value;
}

size_t
CFd::Read (
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
CFd::Write (
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

