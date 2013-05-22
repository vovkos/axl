#include "pch.h"
#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

bool
CFile::Open (
	const char* pFileName, 
	uint_t OpenFlags,
	mode_t Mode
	)
{
	Close ();

	m_h = open (pFileName, OpenFlags, Mode);
	return err::Complete (m_h != -1);
}

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

size_t
CFile::Read (
	void* p, 
	ulong_t Size
	) const
{
	size_t ActualSize = read (m_h, p, Size);
	if (ActualSize == -1)
		err::SetLastSystemError ();

	return ActualSize;
}

size_t 
CFile::Write (
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

