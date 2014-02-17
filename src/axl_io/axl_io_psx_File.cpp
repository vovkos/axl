#include "pch.h"
#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

uint64_t
CFile::GetSize () const
{
	struct stat64 Stat;
	int Result = fstat64 (m_h, &Stat);
	if (Result == -1)
	{
		err::SetLastSystemError ();
		return -1;
	}

	return Stat.st_size;
}

uint64_t
CFile::GetPosition () const
{
	uint64_t Offset = lseek64 (m_h, 0, SEEK_CUR);
	if (Offset == -1)
		err::SetLastSystemError ();

	return Offset;
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

