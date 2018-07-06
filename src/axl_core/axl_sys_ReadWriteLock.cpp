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
		AXL_MEM_FREE (m_data);

	m_data = NULL;

	m_mapping.close ();
	m_file.close ();
	m_readEvent.close ();
	m_writeEvent.close ();
}

bool
ReadWriteLock::create ()
{
	close ();

	m_data = AXL_MEM_NEW (Data);
	m_readEvent.create ();
	m_writeEvent.create ();
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
		m_mapping.open (&m_file, 0, sizeof (Data)) &&
		m_readEvent.create (readEventName) &&
		m_writeEvent.create (writeEventName);

	if (!result)
		return false;

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
		m_mapping.open (&m_file, 0, sizeof (Data)) &&
		m_readEvent.open (readEventName) &&
		m_writeEvent.open (writeEventName);

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

	m_readEvent.reset ();
	m_data->m_queuedReadCount++;
	sys::atomicUnlock (&m_data->m_lock);

	result = m_readEvent.wait (timeout);

	sys::atomicLock (&m_data->m_lock);
	m_data->m_queuedReadCount--;

	if (result)
		m_data->m_activeReadCount++;

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
			m_writeEvent.signal ();
		else if (m_data->m_queuedReadCount)
			m_readEvent.signal ();
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

	m_writeEvent.reset ();
	m_data->m_writeCount++;
	sys::atomicUnlock (&m_data->m_lock);

	result = m_writeEvent.wait (timeout);

	sys::atomicLock (&m_data->m_lock);

	if (!result)
	{
		m_data->m_writeCount--;
		if (!m_data->m_writeCount && m_data->m_queuedReadCount)
			m_readEvent.signal ();
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
		m_readEvent.signal ();
	else if (m_data->m_writeCount)
		m_writeEvent.signal ();

	sys::atomicUnlock (&m_data->m_lock);
}

//..............................................................................

} // namespace sys
} // namespace axl
