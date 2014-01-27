#include "pch.h"
#include "axl_io_psx_Mapping.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

void*
CMapping::Map (
	void* pAddrHint,
	size_t Size,
	int Protection,
	uint_t Flags,
	int fd,
	size_t Offset
	)
{
	Close ();

	m_p = mmap (pAddrHint, Size, Protection, Flags, fd, Offset);
	if (m_p == (void*) -1)
	{
		err::SetLastSystemError ();
		return NULL;
	}

	m_Size = Size;
	return m_p;
}

void
CMapping::Unmap (size_t Size)
{
	if (!IsOpen ())
		return;

	munmap (m_p, Size);
	m_p = NULL;
	m_Size = 0;
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
