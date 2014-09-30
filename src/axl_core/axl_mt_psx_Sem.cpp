#include "pch.h"
#include "axl_mt_psx_Sem.h"
#include "axl_g_Time.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool
Sem::init (
	bool isShared,
	uint_t value
	)
{
	close ();

	int result = sem_init (&m_unnamedSem, isShared, value);
	if (result != 0)
		return err::failWithLastSystemError ();

	m_sem = &m_unnamedSem;
	return true;
}

bool
Sem::open (
	const char* name,
	int flags,
	mode_t mode,
	uint_t value
	)
{
	close ();

	m_sem = sem_open (name, flags, mode, value);
	return err::complete (m_sem != SEM_FAILED);
}

void
Sem::close ()
{
	if (!m_sem)
		return;

	if (m_sem == &m_unnamedSem)
		sem_destroy (&m_unnamedSem);
	else
		sem_close (m_sem);

	m_sem = NULL;
}

bool
Sem::wait (uint_t timeout)
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
		g::getAbsTimespecFromTimeout (timeout, &timespec);
		result = sem_timedwait (m_sem, &timespec);
	}

	return err::complete (result == 0);
}

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
