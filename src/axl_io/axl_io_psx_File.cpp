#include "pch.h"
#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

uint64_t
File::getSize () const
{
	struct stat64 stat;
	int result = ::fstat64 (m_h, &stat);
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
	uint64_t offset = ::lseek64 (m_h, 0, SEEK_CUR);
	if (offset == -1)
		err::setLastSystemError ();

	return offset;
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

