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
#include "axl_sys_Thread.h"

namespace axl {
namespace sys {

//..............................................................................

void
ReadWriteLock::close() {
	if (m_data && m_data != m_mapping.p())
		delete m_data;

	m_data = NULL;

	m_readEvent.close();
	m_writeEvent.close();
	m_mapping.close();
}

bool
ReadWriteLock::create() {
	close();

	m_data = new (mem::ZeroInit) Data;
	m_readEvent.create();
	m_writeEvent.create();

	m_data->m_signature = Signature;

	return true;
}

bool
ReadWriteLock::create(
	const sl::StringRef& mappingName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName
) {
	close();

	bool result =
		m_mapping.open(mappingName, sizeof(Data)) &&
		m_readEvent.create(readEventName) &&
		m_writeEvent.create(writeEventName);

	if (!result)
		return false;

	m_data = (Data*)m_mapping.p();
	memset((void*)m_data, 0, sizeof(Data));
	m_data->m_signature = Signature;
	return true;
}

bool
ReadWriteLock::open(
	const sl::StringRef& mappingName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName
) {
	close();

	bool result =
		m_mapping.open(mappingName, sizeof(Data), io::FileFlag_OpenExisting) &&
		m_readEvent.open(readEventName) &&
		m_writeEvent.open(writeEventName);

	if (!result)
		return false;

	m_data = (Data*)m_mapping.p();

	if (m_data->m_signature != Signature) {
		err::setError(err::SystemErrorCode_InvalidParameter);
		return false;
	}

	return true;
}

void
ReadWriteLock::readLock() {
	bool result;

	sys::atomicLock(&m_data->m_lock);
	if (!m_data->m_activeWriteCount && !m_data->m_queuedWriteCount) {
		m_data->m_activeReadCount++;

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
		onAddReadThread();
#endif
		sys::atomicUnlock(&m_data->m_lock);
		return;
	}

	m_data->m_queuedReadCount++;

	do {
		sys::atomicUnlock(&m_data->m_lock);

		result = m_readEvent.wait();
		ASSERT(result);

		if (!result) // in release, fall back to spin-locking
			sys::yieldProcessor();

		// another reader might squeeze in here, finish and start a writer

		sys::atomicLock(&m_data->m_lock);
	} while (m_data->m_activeWriteCount);

	if (m_data->m_queuedWriteCount) // don't wake new readers (so that we alternate)
		m_readEvent.reset();

	m_data->m_queuedReadCount--;
	m_data->m_activeReadCount++;

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
	onAddReadThread();
#endif
	sys::atomicUnlock(&m_data->m_lock);
}

void
ReadWriteLock::readUnlock() {
	sys::atomicLock(&m_data->m_lock);
	ASSERT(m_data->m_activeReadCount && !m_data->m_activeWriteCount);
	m_data->m_activeReadCount--;

	if (m_data->m_queuedWriteCount) { // wake writers first (so that we alternate)
		if (!m_data->m_activeReadCount) {
			bool result = m_writeEvent.signal();
			ASSERT(result);
		}
	} else {
		if (m_data->m_queuedReadCount) {
			bool result = m_readEvent.signal();
			ASSERT(result);
		}
	}

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
	onRemoveReadThread();
#endif
	sys::atomicUnlock(&m_data->m_lock);
}

void
ReadWriteLock::writeLock() {
	sys::atomicLock(&m_data->m_lock);
	if (!m_data->m_activeReadCount && !m_data->m_activeWriteCount && !m_data->m_queuedReadCount) {
		m_readEvent.reset();
		m_data->m_activeWriteCount = 1;
#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
		onAddWriteThread();
#endif
		sys::atomicUnlock(&m_data->m_lock);
		return;
	}

	m_data->m_queuedWriteCount++;

	do {
		sys::atomicUnlock(&m_data->m_lock);

		// another writer might squeeze in here, finish and wake up readers
		// one reader might start, finish and wake up this writer

		bool result = m_writeEvent.wait();
		ASSERT(result);

		if (!result) // in release, fall back to spin-locking
			sys::yieldProcessor();

		// another reader woken up by the first writer might have read-locked

		sys::atomicLock(&m_data->m_lock);
	} while (m_data->m_activeReadCount || m_data->m_activeWriteCount);

	m_readEvent.reset();
	m_data->m_queuedWriteCount--;
	m_data->m_activeWriteCount = 1;

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
	onAddWriteThread();
#endif
	sys::atomicUnlock(&m_data->m_lock);
}

void
ReadWriteLock::writeUnlock() {
	sys::atomicLock(&m_data->m_lock);
	ASSERT(!m_data->m_activeReadCount && m_data->m_activeWriteCount == 1);
	m_data->m_activeWriteCount = 0;

	if (m_data->m_queuedReadCount) { // wake readers first (so that we alternate)
		bool result = m_readEvent.signal();
		ASSERT(result);
	} else if (m_data->m_queuedWriteCount) {
		bool result = m_writeEvent.signal();
		ASSERT(result);
	}

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
	onRemoveWriteThread();
#endif
	sys::atomicUnlock(&m_data->m_lock);
}

void
ReadWriteLock::upgradeReadLockToWriteLock() {
	bool result;

	sys::atomicLock(&m_data->m_lock);
	ASSERT(m_data->m_activeReadCount >= 1);

	if (m_data->m_activeReadCount == 1 && !m_data->m_activeWriteCount && !m_data->m_queuedReadCount) {
		m_readEvent.reset();
		m_data->m_activeWriteCount = 1;
		m_data->m_activeReadCount = 0;

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
		onRemoveReadThread();
		onAddWriteThread();
#endif
		sys::atomicUnlock(&m_data->m_lock);
		return;
	}

	m_data->m_queuedWriteCount++;

	do {
		sys::atomicUnlock(&m_data->m_lock);

		// another writer might squeeze in here, finish and wake up readers
		// one reader might start, finish and wake up this writer

		result = m_writeEvent.wait();
		ASSERT(result);

		if (!result) // in release, fall back to spin-locking
			sys::yieldProcessor();

		// another reader woken up by the first writer might have read-locked

		sys::atomicLock(&m_data->m_lock);
	} while (m_data->m_activeReadCount != 1 || m_data->m_activeWriteCount);

	m_readEvent.reset();
	m_data->m_queuedWriteCount--;
	m_data->m_activeWriteCount = 1;
	m_data->m_activeReadCount = 0;

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
	onRemoveReadThread();
	onAddWriteThread();
#endif
	sys::atomicUnlock(&m_data->m_lock);
}

void
ReadWriteLock::downgradeWriteLockToReadLock() {
	sys::atomicLock(&m_data->m_lock);
	ASSERT(m_data->m_activeReadCount == 0 && m_data->m_activeWriteCount == 1);
	m_data->m_activeWriteCount = 0;
	m_data->m_activeReadCount = 1;

	if (m_data->m_queuedReadCount) { // wake queued readers
		bool result = m_readEvent.signal();
		ASSERT(result);
	}

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
	onRemoveWriteThread();
	onAddReadThread();
#endif
	sys::atomicUnlock(&m_data->m_lock);
}

#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)

void
ReadWriteLock::onAddReadThread() {
	ASSERT(m_data->m_activeReadCount && m_data->m_activeWriteCount == 0);
	size_t i = m_data->m_activeReadCount - 1;
	if (i < countof(m_data->m_threadIdTable))
		m_data->m_threadIdTable[i] = sys::getCurrentThreadId();
}

void
ReadWriteLock::onRemoveReadThread() {
	if (!m_data->m_activeReadCount)
		return;

	size_t count = m_data->m_activeReadCount + 1;
	if (count > countof(m_data->m_threadIdTable))
		count = countof(m_data->m_threadIdTable);

	uint64_t threadId = sys::getCurrentThreadId();
	uint64_t* p = (uint64_t*)m_data->m_threadIdTable;
	uint64_t* end = p + count;
	for (; p < end; p++)
		if (*p == threadId) {
			memmove(p, p + 1, (end - p - 1) * sizeof(uint64_t));
			m_data->m_threadIdTable[count - 1] = 0;
			return;
		}

	ASSERT(false); // read thread not found
}

void
ReadWriteLock::onAddWriteThread() {
	ASSERT(m_data->m_activeReadCount == 0 && m_data->m_activeWriteCount == 1);
	m_data->m_threadIdTable[0] = sys::getCurrentThreadId();
}

void
ReadWriteLock::onRemoveWriteThread() {
	ASSERT(m_data->m_activeWriteCount == 0);
	ASSERT(m_data->m_threadIdTable[0] == sys::getCurrentThreadId());
	m_data->m_threadIdTable[0] = 0;
}

#endif

//..............................................................................

} // namespace sys
} // namespace axl
