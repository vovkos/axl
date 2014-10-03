#include "pch.h"
#include "axl_mt_psx_Thread.h"
#include "axl_g_Time.h"

#include "axl_mt_Thread.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool
Thread::create (
	const pthread_attr_t* attr,
	FThreadProc pfThreadProc,
	void* context
	)
{
	detach ();

	int result = ::pthread_create (&m_threadId, attr, pfThreadProc, context);
	if (result != 0)
		return err::fail (result);

	m_isOpen = true;
	return true;
}

bool
Thread::join (
	uint_t timeout,
	void** retVal
	)
{
	if (!m_isOpen)
		return true;

	int result;

	switch (timeout)
	{
	case 0:
		result = ::pthread_tryjoin_np (m_threadId, retVal);
		break;

	case -1:
		result = ::pthread_join (m_threadId, retVal);
		break;

	default:
		timespec timespec = { 0 };
		g::getAbsTimespecFromTimeout (timeout, &timespec);
		result = ::pthread_timedjoin_np (m_threadId, retVal, &timespec);
	}

	if (result != 0)
		return err::fail (result);

	m_isOpen = false;
	return true;
}

bool
Thread::detach ()
{
	if (!m_isOpen)
		return true;

	int result = ::pthread_detach (m_threadId);
	if (result != 0)
		return err::fail (result);

	m_isOpen = false;
	return true;
}

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
