#include "pch.h"
#include "axl_io_psx_Fd.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

bool
Fd::open (
	const sl::StringRef& fileName,
	uint_t openFlags,
	mode_t mode
	)
{
	close ();

	m_h = ::open (fileName, openFlags, mode);
	return err::complete (m_h != -1);
}

bool
Fd::setBlockingMode (bool isBlocking)
{
	int result = ::fcntl (m_h, F_GETFL, 0);
	if (result == -1)
		return err::failWithLastSystemError ();

	int flags = result;
	if (isBlocking)
		flags &= ~O_NONBLOCK;
	else
		flags |= O_NONBLOCK;

	result = ::fcntl (m_h, F_SETFL, flags);
	return err::complete (result != -1);
}

size_t
Fd::getIncomingDataSize ()
{
	int value;
	int result = ::ioctl (m_h, FIONREAD, &value);
	if (result == -1)
	{
		err::setLastSystemError ();
		return -1;
	}

	return value;
}

size_t
Fd::read (
	void* p,
	size_t size
	) const
{
	size_t actualSize = ::read (m_h, p, size);
	if (actualSize == -1)
		err::setLastSystemError ();

	return actualSize;
}

size_t
Fd::write (
	const void* p,
	size_t size
	)
{
	size_t actualSize = ::write (m_h, p, size);
	if (actualSize == -1)
		err::setLastSystemError ();

	return actualSize;
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

