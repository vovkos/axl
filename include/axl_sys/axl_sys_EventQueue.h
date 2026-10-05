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

#define _AXL_SYS_EVENTQUEUE_H

#include "axl_sys_Lock.h"
#include "axl_sys_Event.h"
#include "axl_sl_List.h"
#include "axl_sl_HandleTable.h"

namespace axl {
namespace sys {

// sticky event queue -- mirrors the jancy IO wait model

class EventQueue;

//..............................................................................

typedef
void
EventQueueAsyncWaitFunc(
	uint_t triggeredEvents,
	void* context
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct EventQueueMultiWait {
	EventQueue* m_queue;
	uint_t m_triggeredEvents;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EventQueueEvent: uint_t {
	EventQueueEvent_Error  = 0x00000001,  // maps to jancy StreamEvents.IoError
	EventQueueEvent_Closed = 0x80000000,  // internal event
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class EventQueue {
protected:
	struct SyncWait: sl::ListLink {
		uint_t m_mask;
		Event* m_event;
	};

	struct AsyncWait: sl::ListLink {
		uint_t m_mask;
		EventQueueAsyncWaitFunc* m_func;
		void* m_context;
		handle_t m_handle;
	};

protected:
	mutable Lock m_lock;
	uint_t m_activeEvents;
	err::Error m_error;
	sl::AuxList<SyncWait> m_syncWaitList;
	sl::List<AsyncWait> m_asyncWaitList;
	sl::HandleTable<AsyncWait*> m_asyncWaitMap;

public:
	EventQueue() {
		m_activeEvents = 0;
	}

	~EventQueue() {
		ASSERT(m_syncWaitList.isEmpty() && m_asyncWaitList.isEmpty() && m_asyncWaitMap.isEmpty());
	}

	void
	lock() const {
		m_lock.lock();
	}

	void
	unlock() const {
		m_lock.unlock();
	}

	void
	reset();

	uint_t
	getActiveEvents() const {
		sys::ScopeLock lock(&m_lock);
		return m_activeEvents;
	}

	uint_t
	getActiveEventsNoLock() const {
		return m_activeEvents;
	}

	err::ErrorRef
	getError() const {
		sys::ScopeLock scopeLock(&m_lock);
		return m_error;
	}

	err::ErrorRef
	getErrorNoLock() const {
		return m_error;
	}

	handle_t
	asyncWait(
		uint_t eventMask,
		EventQueueAsyncWaitFunc* func,
		void* context
	);

	uint_t
	blockingWait(
		uint_t eventMask,
		uint_t timeout = -1
	);

	// returns the first entry with triggered events

	static
	EventQueueMultiWait*
	multiWait(
		uint_t eventMask,
		EventQueueMultiWait* waitArray,
		size_t count,
		uint_t timeout = -1
	);

	bool
	cancelWait(handle_t handle);

protected:
	void
	setEvents_l(uint_t events);

	void
	setEvents(uint_t events) {
		m_lock.lock();
		setEvents_l(events);
	}

	void
	setErrorEvent_l(const err::ErrorRef& error = err::getLastError()) { // unlocks
		m_error = error;
		setEvents_l(EventQueueEvent_Error);
	}

	void
	setErrorEvent(const err::ErrorRef& error = err::getLastError()) {
		m_lock.lock();
		setErrorEvent_l(error);
	}

	template <typename T>
	T
	setErrorEventAndFail_l( // unlocks
		T failResult,
		const err::ErrorRef& error = err::getLastError()
	) {
		setErrorEvent_l(error);
		return err::fail<T>(failResult, error);
	}

	template <typename T>
	T
	setErrorEventAndFail(
		T failResult,
		const err::ErrorRef& error = err::getLastError()
	) {
		m_lock.lock();
		return setErrorEventAndFail_l(failResult, error);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
EventQueue::reset() {
	ASSERT(m_syncWaitList.isEmpty() && m_asyncWaitList.isEmpty() && m_asyncWaitMap.isEmpty());
	m_activeEvents = 0;
	m_error.clear();
}

//..............................................................................

} // namespace sys
} // namespace axl
