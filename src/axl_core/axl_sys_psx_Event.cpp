#include "pch.h"
#include "axl_sys_Event.h"

namespace axl {
namespace sys {

//.............................................................................

void
EventRoot::reset ()
{
    m_condMutexPair.lock ();
	m_state = false;
	m_condMutexPair.unlock ();
}

bool
EventRoot::wait (uint_t timeout)
{
	m_condMutexPair.lock ();
	
	if (m_state)
	{
		if (!m_isNotificationEvent)
			m_state = false;
		
		m_condMutexPair.unlock ();
		return true;
	}

    bool result = m_condMutexPair.wait (timeout);
    if (!result)
    {
        m_condMutexPair.unlock ();
        return false;
    }

	if (!m_isNotificationEvent)
		m_state = false;

	m_condMutexPair.unlock ();
	
    return true;
}

bool
EventRoot::signal ()
{
	m_condMutexPair.lock ();
	if (m_state)
	{
		m_condMutexPair.unlock ();
		return true;
	}

	bool result = m_isNotificationEvent ?
		m_condMutexPair.broadcast () :
		m_condMutexPair.signal ();

	if (!result)
	{
		m_condMutexPair.unlock ();
		return false;
	}

	m_state = true;
	m_condMutexPair.unlock ();
    
	return true;
}

//.............................................................................

} // namespace sys
} // namespace axl
