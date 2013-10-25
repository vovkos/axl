#include "pch.h"
#include "axl_mt_psx_Thread.h"
#include "axl_g_Time.h"

#include "axl_mt_Thread.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool
CThread::Create (
	const pthread_attr_t* pAttr,
	FThreadProc pfThreadProc,
	void* pContext
	)
{
	Detach ();

	int Result = pthread_create (&m_ThreadId, pAttr, pfThreadProc, pContext);
	if (Result != 0)
		return err::Fail (Result);

	m_IsOpen = true;
	return true;
}

bool
CThread::Join (
	uint_t Timeout,
	void** ppRetVal
	)
{
	if (!m_IsOpen)
		return true;

	int Result;

	switch (Timeout)
	{
	case 0:
		Result = pthread_tryjoin_np (m_ThreadId, ppRetVal);
		break;

	case -1:
		Result = pthread_join (m_ThreadId, ppRetVal);
		break;

	default:
		timespec Timespec = { 0 };
		g::GetAbsTimespecFromTimeout (Timeout, &Timespec);
		Result = pthread_timedjoin_np (m_ThreadId, ppRetVal, &Timespec);
	}

	if (Result != 0)
		return err::Fail (Result);

	m_IsOpen = false;
	return true;
}

bool
CThread::Detach ()
{
	if (!m_IsOpen)
		return true;

	int Result = pthread_detach (m_ThreadId);
	if (Result != 0)
		return err::Fail (Result);

	m_IsOpen = false;
	return true;
}

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
