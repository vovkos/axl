#include "pch.h"
#include "axl_mt_Event.h"

namespace axl {
namespace mt {

//.............................................................................

bool
Event::signal ()
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
Event::reset ()
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
NotificationEvent::signal ()
{
	m_mutex.lock ();

	bool result = m_cond.broadcast ();			
	if (result)
		m_state = true;

	m_mutex.unlock ();
	return result;
}

bool
NotificationEvent::reset ()
{
	m_mutex.lock ();
	m_state = false;
	m_mutex.unlock ();
	return true;		
}

bool
NotificationEvent::wait (uint_t timeout)
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
