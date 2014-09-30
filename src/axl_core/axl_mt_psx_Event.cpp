#include "pch.h"
#include "axl_mt_Event.h"

namespace axl {
namespace mt {

//.............................................................................

bool
CEvent::signal ()
{
	m_mutex.lock ();

	int value = 0;
	bool result = m_sem.getValue (&value);
	if (result && value <= 0)
		result = m_sem.post ();

	m_mutex.unlock ();

	return result;
}

bool
CEvent::reset ()
{
	m_mutex.lock ();

	int value = 0;
	bool result = m_sem.getValue (&value);
	if (result && value > 0)
		result = m_sem.wait ();

	m_mutex.unlock ();

	return result;
}

//.............................................................................

bool
CNotificationEvent::signal ()
{
	m_mutex.lock ();

	bool result = m_cond.broadcast ();			
	if (result)
		m_state = true;

	m_mutex.unlock ();
	return result;
}

bool
CNotificationEvent::reset ()
{
	m_mutex.lock ();
	m_state = false;
	m_mutex.unlock ();
	return true;		
}

bool
CNotificationEvent::wait (uint_t timeout)
{
	m_mutex.lock ();
	if (m_state)
	{
		m_mutex.unlock ();
		return true;
	}

	bool result = m_cond.wait (m_mutex, timeout);
	m_mutex.unlock ();
	return result;		
}

//.............................................................................

} // namespace mt
} // namespace axl
