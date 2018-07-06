//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_sys_Event.h"

namespace axl {
namespace sys {

#if (_AXL_OS_POSIX)

//..............................................................................

void
EventBase::reset ()
{
	m_condMutexPair.lock ();
	m_state = false;
	m_condMutexPair.unlock ();
}

bool
EventBase::wait (uint_t timeout)
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
EventBase::signal ()
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

//..............................................................................

#endif

#if (_AXL_OS_POSIX)

//..............................................................................

bool
ReadWriteLock::EventBase::signal ()
{
	return m_eventKind == EventKind_NamedSem ?
		((NamedSemEvent*) this)->m_sem.signal () :
		((UnnamedEvent*) this)->m_event.signal ();
}

void
ReadWriteLock::EventBase::reset ()
{
	if (m_eventKind == EventKind_NamedSem)
		while (((NamedSemEvent*) this)->m_sem.tryWait ());
	else
		((UnnamedEvent*) this)->m_event.reset ();
}

bool
ReadWriteLock::EventBase::wait (uint_t timeout)
{
	return m_eventKind == EventKind_NamedSem ?
#if (_AXL_OS_DARWIN) // no sem_timedwait on darwin
		timeout == 0 ?
			((NamedSemEvent*) this)->m_sem.tryWait () :
			((NamedSemEvent*) this)->m_sem.wait () :
#else
		((NamedSemEvent*) this)->m_sem.wait (timeout) :
#endif
		((UnnamedEvent*) this)->m_event.wait (timeout);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ReadWriteLock::NamedSemEvent::~NamedSemEvent ()
{
	if (!m_name.isEmpty ())
		sys::psx::NamedSem::unlink (m_name);
}

bool
ReadWriteLock::NamedSemEvent::create (const sl::StringRef& name)
{
	ASSERT (!m_sem);

	bool result = m_sem.open (name, O_CREAT);
	if (!result)
		return false;

	m_name = name;
	return true;
}

bool
ReadWriteLock::NamedSemEvent::open (const sl::StringRef& name)
{
	ASSERT (!m_sem);
	return m_sem.open (name, 0);
}

//..............................................................................

#endif

} // namespace sys
} // namespace axl
