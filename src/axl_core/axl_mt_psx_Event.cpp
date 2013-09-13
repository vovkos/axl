#include "pch.h"
#include "axl_mt_Event.h"

namespace axl {
namespace mt {

//.............................................................................

bool
CEvent::Signal ()
{
	m_Mutex.Lock ();

	int Value = 0;
	bool Result = m_Sem.GetValue (&Value);
	if (Result && Value <= 0)
		Result = m_Sem.Post ();

	m_Mutex.Unlock ();

	return Result;
}

bool
CEvent::Reset ()
{
	m_Mutex.Lock ();

	int Value = 0;
	bool Result = m_Sem.GetValue (&Value);
	if (Result && Value > 0)
		Result = m_Sem.Wait ();

	m_Mutex.Unlock ();

	return Result;
}

//.............................................................................

bool
CNotificationEvent::Signal ()
{
	m_Mutex.Lock ();

	bool Result = m_Cond.Broadcast ();			
	if (Result)
		m_State = true;

	m_Mutex.Unlock ();
	return Result;
}

bool
CNotificationEvent::Reset ()
{
	m_Mutex.Lock ();
	m_State = false;
	m_Mutex.Unlock ();
	return true;		
}

bool
CNotificationEvent::Wait (uint_t Timeout)
{
	m_Mutex.Lock ();
	if (m_State)
	{
		m_Mutex.Unlock ();
		return true;
	}

	bool Result = m_Cond.Wait (m_Mutex, Timeout);
	m_Mutex.Unlock ();
	return Result;		
}

//.............................................................................

} // namespace mt
} // namespace axl
