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

//..............................................................................

void
ReadWriteLock::close ()
{
	if (m_data && m_data != m_mapping.p ())
		AXL_MEM_FREE ((void*) m_data);

	m_data = NULL;

	m_readEvent.close ();
	m_writeEvent.close ();
	m_mapping.close ();
}

bool
ReadWriteLock::create ()
{
	close ();

	m_data = AXL_MEM_NEW (Data);
	m_readEvent.create ();
	m_writeEvent.create ();

	m_data->m_signature = Signature;

	return true;
}

bool
ReadWriteLock::create (
	const sl::StringRef& mappingName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName
	)
{
	close ();

	bool result =
		m_mapping.open (mappingName, sizeof (Data)) &&
		m_readEvent.create (readEventName) &&
		m_writeEvent.create (writeEventName);

	if (!result)
		return false;

	m_data = (Data*) m_mapping.p ();
	memset ((void*) m_data, 0, sizeof (Data));
	m_data->m_signature = Signature;
	return true;
}

bool
ReadWriteLock::open (
	const sl::StringRef& mappingName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName
	)
{
	close ();

	bool result =
		m_mapping.open (mappingName, sizeof (Data), io::FileFlag_OpenExisting) &&
		m_readEvent.open (readEventName) &&
		m_writeEvent.open (writeEventName);

	if (!result)
		return false;

	m_data = (Data*) m_mapping.p ();

	if (m_data->m_signature != Signature)
	{
		err::setError (err::SystemErrorCode_InvalidParameter);
		return false;
	}

	return true;
}

bool
ReadWriteLock::readLock (uint_t timeout)
{
	bool result;

	sys::atomicLock (&m_data->m_lock);
	if (!m_data->m_activeWriteCount && !m_data->m_queuedWriteCount)
	{
		m_data->m_activeReadCount++;
		sys::atomicUnlock (&m_data->m_lock);
		return true;
	}

	m_data->m_queuedReadCount++;

	for (;;) // loop is required (obvious)
	{
		m_readEvent.reset ();
		sys::atomicUnlock (&m_data->m_lock);

		result = m_readEvent.wait (timeout);

		// another reader might squeeze in here, finish and start a writer

		sys::atomicLock (&m_data->m_lock);

		if (!result)
		{
			if (!m_data->m_activeReadCount && m_data->m_queuedWriteCount)
				m_writeEvent.signal ();

			break;
		}

		if (!m_data->m_activeWriteCount)
		{
			m_data->m_activeReadCount++;
			break;
		}
	}

	m_data->m_queuedReadCount--;
	sys::atomicUnlock (&m_data->m_lock);

	return result;
}

void
ReadWriteLock::readUnlock ()
{
	sys::atomicLock (&m_data->m_lock);
	ASSERT (m_data->m_activeReadCount && !m_data->m_activeWriteCount);
	m_data->m_activeReadCount--;

	if (m_data->m_queuedWriteCount) // check writers first, so we alternate
	{
		if (!m_data->m_activeReadCount)
			m_writeEvent.signal ();
	}
	else
	{
		if (m_data->m_queuedReadCount)
			m_readEvent.signal ();
	}

	sys::atomicUnlock (&m_data->m_lock);
}

bool
ReadWriteLock::writeLock (uint_t timeout)
{
	bool result;

	sys::atomicLock (&m_data->m_lock);
	if (!m_data->m_activeReadCount && !m_data->m_activeWriteCount && !m_data->m_queuedWriteCount)
	{
		m_data->m_activeWriteCount = 1;
		sys::atomicUnlock (&m_data->m_lock);
		return true;
	}

	m_data->m_queuedWriteCount++;

	for (;;) // loop is STILL required (non-obvious)
	{
		m_writeEvent.reset ();
		sys::atomicUnlock (&m_data->m_lock);

		// another writer might squeeze in here, finish and wake up readers
		// one reader might start, finish and wake up this writer

		result = m_writeEvent.wait (timeout);

		// another reader woken up by the first writer might have read-locked

		sys::atomicLock (&m_data->m_lock);

		if (!result)
		{
			if (!m_data->m_activeWriteCount && m_data->m_queuedReadCount)
				m_readEvent.signal ();

			break;
		}

		if (!m_data->m_activeReadCount && !m_data->m_activeWriteCount)
		{
			m_data->m_activeWriteCount++;
			break;
		}
	}

	m_data->m_queuedWriteCount--;
	sys::atomicUnlock (&m_data->m_lock);

	return result;
}

void
ReadWriteLock::writeUnlock ()
{
	sys::atomicLock (&m_data->m_lock);
	ASSERT (!m_data->m_activeReadCount && m_data->m_activeWriteCount == 1);
	m_data->m_activeWriteCount = 0;

	if (m_data->m_queuedReadCount) // check readers first, so we alternate
		m_readEvent.signal ();
	else if (m_data->m_queuedWriteCount)
		m_writeEvent.signal ();

	sys::atomicUnlock (&m_data->m_lock);
}

//..............................................................................

} // namespace sys
} // namespace axl
