#include "pch.h"
#include "axl_sys_psx_Sem.h"
#include "axl_sys_Time.h"

namespace axl {
namespace sys {
namespace psx {

//.............................................................................

#if (!_AXL_OS_DARWIN)

bool
Sem::wait (uint_t timeout)
{
	int result;

	switch (timeout)
	{
	case 0:
		result = sem_trywait (&m_sem);
		break;

	case -1:
		result = sem_wait (&m_sem);
		break;

	default:
		timespec timespec = { 0 };
		sys::getAbsTimespecFromTimeout (timeout, &timespec);
		result = sem_timedwait (&m_sem, &timespec);
	}

	return err::complete (result == 0);
}

#endif

//.............................................................................

bool
NamedSem::open (
	const sl::StringRef& name,
	int flags,
	mode_t mode,
	uint_t value
	)
{
	close ();

	m_sem = sem_open (name.sz (), flags, mode, value);
	return err::complete (m_sem != SEM_FAILED);
}

void
NamedSem::close ()
{
	if (!m_sem)
		return;

	sem_close (m_sem);
	m_sem = NULL;
}

#if (!_AXL_OS_DARWIN)
bool
NamedSem::wait (uint_t timeout)
{
	int result;

	switch (timeout)
	{
	case 0:
		result = sem_trywait (m_sem);
		break;

	case -1:
		result = sem_wait (m_sem);
		break;

	default:
		timespec timespec = { 0 };
		sys::getAbsTimespecFromTimeout (timeout, &timespec);
		result = sem_timedwait (m_sem, &timespec);
	}

	return err::complete (result == 0);
}
#endif

//.............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
