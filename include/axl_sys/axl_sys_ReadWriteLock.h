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

#pragma once

#define _AXL_SYS_READWRITELOCK_H

#include "axl_sys_Atomic.h"
#include "axl_sys_Event.h"
#include "axl_io_Mapping.h"

#ifdef _DEBUG
#	define _AXL_SYS_READWRITELOCK_DEBUG_THREADS       0 // on/off
#	define _AXL_SYS_READWRITELOCK_DEBUG_THREAD_COUNT  4 // adjust if not enough
#endif

namespace axl {
namespace sys {

//..............................................................................

class ReadWriteLock {
protected:
	// locking data may be shared between processes

	enum {
		Signature = 'klwr',
	};

	struct Data {
		uint32_t m_signature;
		int32_t m_lock;
		uint32_t m_activeReadCount;
		uint32_t m_queuedReadCount;
		uint32_t m_activeWriteCount; // one or zero
		uint32_t m_queuedWriteCount;
#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
		uint64_t m_threadIdTable[_AXL_SYS_READWRITELOCK_DEBUG_THREAD_COUNT];
#endif
	};

protected:
	volatile Data* m_data;
	io::Mapping m_mapping;
	NameableNotificationEvent m_readEvent;
	NameableEvent m_writeEvent;

public:
	ReadWriteLock() {
		m_data = NULL;
	}

	~ReadWriteLock() {
		close();
	}

	void
	close();

	bool
	create();

	bool
	create(
		const sl::StringRef& mappingName,
		const sl::StringRef& readEventName,
		const sl::StringRef& writeEventName
	);

	bool
	open(
		const sl::StringRef& mappingName,
		const sl::StringRef& readEventName,
		const sl::StringRef& writeEventName
	);

	void
	readLock();

	void
	readUnlock();

	void
	writeLock();

	void
	writeUnlock();

	void
	upgradeReadLockToWriteLock();

	void
	downgradeWriteLockToReadLock();

protected:
#if (_AXL_SYS_READWRITELOCK_DEBUG_THREADS)
	void
	onAddReadThread();

	void
	onRemoveReadThread();

	void
	onAddWriteThread();

	void
	onRemoveWriteThread();
#endif
};

//..............................................................................

} // namespace sys
} // namespace axl
