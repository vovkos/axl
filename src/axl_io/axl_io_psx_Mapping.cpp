#include "pch.h"
#include "axl_io_psx_Mapping.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

void*
Mapping::map (
	void* addrHint,
	size_t size,
	int protection,
	uint_t flags,
	int fd,
	size_t offset
	)
{
	close ();

	m_p = ::mmap (addrHint, size, protection, flags, fd, offset);
	if (m_p == (void*) -1)
	{
		err::setLastSystemError ();
		return NULL;
	}

	m_size = size;
	return m_p;
}

void
Mapping::unmap (size_t size)
{
	if (!isOpen ())
		return;

	::munmap (m_p, size);
	m_p = NULL;
	m_size = 0;
}

//..............................................................................

bool
SharedMemory::open (
	const sl::StringRef& name,
	uint_t flags,
	mode_t mode
	)
{
	close ();

	m_h = ::shm_open (name.sz (), flags, mode);
	return err::complete (m_h != -1);
}

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
