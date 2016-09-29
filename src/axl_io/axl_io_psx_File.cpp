#include "pch.h"
#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

uint64_t
File::getSize () const
{
#if (_AXL_OS_DARWIN)
	struct stat stat;
	int result = ::fstat (m_h, &stat);
#else
	struct stat64 stat;
	int result = ::fstat64 (m_h, &stat);
#endif

	if (result == -1)
	{
		err::setLastSystemError ();
		return -1;
	}

	return stat.st_size;
}

uint64_t
File::getPosition () const
{
#if (_AXL_OS_DARWIN)
	uint64_t offset = ::lseek (m_h, 0, SEEK_CUR);
#else
	uint64_t offset = ::lseek64 (m_h, 0, SEEK_CUR);
#endif

	if (offset == -1)
		err::setLastSystemError ();

	return offset;
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

