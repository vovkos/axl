#include "pch.h"
#include "axl_mt_psx_Sem.h"
#include "axl_g_Time.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool
CSem::Init (
	bool IsShared,
	uint_t Value
	)
{
	Close ();

	int Result = sem_init (&m_UnnamedSem, IsShared, Value);
	if (Result != 0)
		return err::FailWithLastSystemError ();

	m_pSem = &m_UnnamedSem;
	return true;
}

bool
CSem::Open (
	const char* pName,
	int Flags,
	mode_t Mode,
	uint_t Value
	)
{
	Close ();

	m_pSem = sem_open (pName, Flags, Mode, Value);
	return err::Complete (m_pSem != SEM_FAILED);
}

void
CSem::Close ()
{
	if (!m_pSem)
		return;

	if (m_pSem == &m_UnnamedSem)
		sem_destroy (&m_UnnamedSem);
	else
		sem_close (m_pSem);

	m_pSem = NULL;
}

bool
CSem::Wait (uint_t Timeout)
{
	int Result;

	switch (Timeout)
	{
	case 0:
		Result = sem_trywait (m_pSem);
		break;

	case -1:
		Result = sem_wait (m_pSem);
		break;

	default:
		timespec Timespec = { 0 };
		g::GetAbsTimespecFromTimeout (Timeout, &Timespec);
		Result = sem_timedwait (m_pSem, &Timespec);
	}

	return err::Complete (Result == 0);
}

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
