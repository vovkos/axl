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
#include "axl_sys_ReadWriteLock.h"

namespace axl {
namespace sys {

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
			((ReadWriteLock::NamedSemEvent*) this)->m_sem.tryWait () :
			((ReadWriteLock::NamedSemEvent*) this)->m_sem.wait () :
#else
		((ReadWriteLock::NamedSemEvent*) this)->m_sem.wait (timeout) :
#endif
		((ReadWriteLock::UnnamedEvent*) this)->m_event.wait (timeout);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
ReadWriteLock::NamedSemEvent::create (const sl::StringRef& name)
{
	return true;
}

bool
ReadWriteLock::NamedSemEvent::open (const sl::StringRef& name)
{
	return true;
}

//..............................................................................

#endif

ReadWriteLock::ReadWriteLock ()
{
	m_data = NULL;

#if (_AXL_OS_POSIX)
	m_readEvent = NULL;
	m_writeEvent = NULL;
#endif
}

void
ReadWriteLock::close ()
{
	if (m_data && m_data != m_mapping.p ())
		AXL_MEM_FREE (m_data);

	m_data = NULL;

	m_mapping.close ();
	m_file.close ();

#if (_AXL_OS_WIN)
	m_readEvent.close ();
	m_writeEvent.close ();
#elif (_AXL_OS_POSIX)
	if (m_readEvent)
	{
		AXL_MEM_DELETE (m_readEvent);
		m_readEvent = NULL;
	}

	if (m_writeEvent)
	{
		AXL_MEM_DELETE (m_writeEvent);
		m_writeEvent = NULL;
	}
#endif
}

bool
ReadWriteLock::create ()
{
	close ();

	m_data = AXL_MEM_NEW (Data);

#if (_AXL_OS_WIN)
	m_readEvent.create (NULL, true, false);
	m_writeEvent.create (NULL, false, false);
#elif (_AXL_OS_POSIX)
	m_readEvent = AXL_MEM_NEW_ARGS (UnnamedEvent, (true));
	m_writeEvent = AXL_MEM_NEW_ARGS (UnnamedEvent, (false));
#endif

	return true;
}

bool
ReadWriteLock::create (
	const sl::StringRef& fileName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName
	)
{
	close ();

	bool result =
		m_file.open (fileName, io::FileFlag_ShareWrite | io::FileFlag_DeleteOnClose) &&
		m_mapping.open (&m_file, 0, sizeof (Data));

	if (!result)
		return false;

#if (_AXL_OS_WIN)
	result =
		m_readEvent.create (NULL, true, false, sl::String_w (readEventName)) &&
		m_writeEvent.create (NULL, false, false, sl::String_w (writeEventName));

	if (!result)
		return false;
#elif (_AXL_OS_POSIX)
	NamedSemEvent* readEvent = AXL_MEM_NEW (NamedSemEvent);
	NamedSemEvent* writeEvent = AXL_MEM_NEW (NamedSemEvent);

	result =
		readEvent->create (readEventName) &&
		writeEvent->create (writeEventName);

	if (!result)
		return false;

	m_readEvent = readEvent;
	m_writeEvent = writeEvent;
#endif

	m_data = (Data*) m_mapping.p ();
	memset (m_data, 0, sizeof (Data));
	return true;
}

bool
ReadWriteLock::open (
	const sl::StringRef& fileName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName
	)
{
	close ();

	bool result =
		m_file.open (fileName, io::FileFlag_ShareWrite | io::FileFlag_OpenExisting) &&
		m_mapping.open (&m_file, 0, sizeof (Data));

	if (!result)
		return false;

#if (_AXL_OS_WIN)
	result =
		m_readEvent.open (EVENT_ALL_ACCESS, false, sl::String_w (readEventName)) &&
		m_writeEvent.open (EVENT_ALL_ACCESS, false, sl::String_w (writeEventName));
#elif (_AXL_OS_POSIX)
	NamedSemEvent* readEvent = AXL_MEM_NEW (NamedSemEvent);
	NamedSemEvent* writeEvent = AXL_MEM_NEW (NamedSemEvent);

	result =
		readEvent->open (readEventName) &&
		writeEvent->open (writeEventName);

	if (!result)
		return false;

	m_readEvent = readEvent;
	m_writeEvent = writeEvent;
#endif

	if (!result)
		return false;

	m_data = (Data*) m_mapping.p ();
	return true;
}

bool
ReadWriteLock::readLock (uint_t timeout)
{
	bool result;

	sys::atomicLock (&m_data->m_lock);
	if (!m_data->m_writeCount)
	{
		m_data->m_activeReadCount++;
		sys::atomicUnlock (&m_data->m_lock);
		return true;
	}

	m_readEvent->reset ();
	m_data->m_queuedReadCount++;
	sys::atomicUnlock (&m_data->m_lock);

	result = m_readEvent->wait (timeout);

	sys::atomicLock (&m_data->m_lock);

	result ?
		m_data->m_activeReadCount++ :
		m_data->m_queuedReadCount--;

	sys::atomicUnlock (&m_data->m_lock);
	return result;
}

void
ReadWriteLock::readUnlock ()
{
	sys::atomicLock (&m_data->m_lock);
	ASSERT (m_data->m_activeReadCount);
	m_data->m_activeReadCount--;

	if (!m_data->m_activeReadCount)
	{
		if (m_data->m_writeCount) // check writers first, so we alternate
			m_writeEvent->signal ();
		else if (m_data->m_queuedReadCount)
			m_readEvent->signal ();
	}

	sys::atomicUnlock (&m_data->m_lock);
}

bool
ReadWriteLock::writeLock (uint_t timeout)
{
	bool result;

	sys::atomicLock (&m_data->m_lock);
	if (!m_data->m_activeReadCount && !m_data->m_writeCount)
	{
		m_data->m_writeCount = 1;
		sys::atomicUnlock (&m_data->m_lock);
		return true;
	}

	m_writeEvent->reset ();
	m_data->m_writeCount++;
	sys::atomicUnlock (&m_data->m_lock);

	result = m_writeEvent->wait (timeout);

	sys::atomicLock (&m_data->m_lock);

	if (!result)
	{
		m_data->m_writeCount--;
		if (!m_data->m_writeCount && m_data->m_queuedReadCount)
			m_readEvent->signal ();
	}

	sys::atomicUnlock (&m_data->m_lock);
	return result;
}

void
ReadWriteLock::writeUnlock ()
{
	sys::atomicLock (&m_data->m_lock);
	ASSERT (!m_data->m_activeReadCount);
	m_data->m_writeCount--;

	if (m_data->m_queuedReadCount) // check readers first, so we alternate
		m_readEvent->signal ();
	else if (m_data->m_writeCount)
		m_writeEvent->signal ();

	sys::atomicUnlock (&m_data->m_lock);
}

//..............................................................................

} // namespace sys
} // namespace axl
