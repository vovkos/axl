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

#define _AXL_SYS_EVENT_H

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Event.h"
#	include "axl_sl_Operator.h"
#elif (_AXL_OS_POSIX)
#	include "axl_sys_psx_Cond.h"
#	include "axl_io_psx_Mapping.h"
#	include "axl_sys_Semaphore.h"
#endif

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

template <typename IsNotificationEvent>
class EventBase {
public:
	win::Event m_event;

public:
	EventBase() {
		m_event.create(NULL, IsNotificationEvent()(), false, NULL);
	}

	bool
	signal() {
		return m_event.signal();
	}

	bool
	reset() {
		return m_event.reset();
	}

	bool
	wait(uint_t timeout = -1) {
		return m_event.wait(timeout) == win::WaitResult_Object0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef EventBase<sl::False<bool> > Event;
typedef EventBase<sl::True<bool> > NotificationEvent;

//..............................................................................

#elif (_AXL_OS_POSIX)

template <typename IsNotificationEvent>
class CondMutexEventBase {
protected:
	psx::CondMutexPair m_condMutexPair;
	volatile bool m_state;

public:
	CondMutexEventBase() {
		 m_state = false;
	}

	CondMutexEventBase(
		const pthread_condattr_t* condAttr,
		const pthread_mutexattr_t* mutexAttr
	):
		m_condMutexPair(condAttr, mutexAttr) {
		m_state = false;
	}

	bool
	signal() {
		m_condMutexPair.lock();
		if (m_state) {
			m_condMutexPair.unlock();
			return true;
		}

		m_state = true;

		bool result = IsNotificationEvent()() ?
			m_condMutexPair.broadcast() :
			m_condMutexPair.signal();

		m_condMutexPair.unlock();
		return result;
	}

	bool
	reset() {
		m_condMutexPair.lock();
		m_state = false;
		m_condMutexPair.unlock();
		return true;
	}

	bool
	wait(uint_t timeout = -1) {
		m_condMutexPair.lock();

		if (m_state) {
			if (!IsNotificationEvent()())
				m_state = false;

			m_condMutexPair.unlock();
			return true;
		}

		bool result = m_condMutexPair.wait(timeout);
		if (!result) {
			m_condMutexPair.unlock();
			return false;
		}

		if (!IsNotificationEvent()())
			m_state = false;

		m_condMutexPair.unlock();
		return true;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CondMutexEventBase<sl::False<bool> > Event;
typedef CondMutexEventBase<sl::True<bool> > NotificationEvent;

#endif

//..............................................................................

// there is no simple mapping between win32/linux/osx IPC primitives to
// allow natural implementaiton of a universal nameable event -- which can be:

// 1.a) auto-reset
// 1.b) manual-reset
// 2.a) anonymous
// 2.b) named

// on windows, everything maps directly to win::Event
// on linux, we can use a condvar/mutex/flags in a shared memory block
// on darwin, support for PTHREAD_PROCESS_SHARED is questionable, and we need
//    a hand-made implementation via a named semaphore and a shared memory block

#if (_AXL_OS_WIN)

template <typename IsNotificationEvent>
class NameableEventBase {
protected:
	sys::win::Event m_event;

public:
	void
	close() {
		m_event.close();
	}

	bool
	create() {
		return m_event.create(NULL, IsNotificationEvent()(), false);
	}

	bool
	create(const sl::StringRef& name) {
		return m_event.create(NULL, IsNotificationEvent()(), false, name.s2());
	}

	bool
	open(const sl::StringRef& name) {
		return m_event.open(EVENT_ALL_ACCESS, false, name.s2());
	}

	bool
	wait(uint_t timeout = -1) {
		return m_event.wait(timeout) == sys::win::WaitResult_Object0;
	}

	bool
	signal() {
		return m_event.signal();
	}

	bool
	reset() {
		return m_event.reset();
	}
};

//..............................................................................

#elif (_AXL_OS_POSIX)
#	if (_AXL_OS_DARWIN)

template <typename IsNotificationEvent>
class NamedSemEventBase {
protected:
	struct Data {
		int32_t m_lock;
		uint32_t m_waitCount;

		union {
			bool m_state;
			uint64_t _m_align;
		};
	};

protected:
	sl::String m_name;
	psx::NamedSem m_sem;
	io::psx::Mapping m_mapping;
	volatile Data* m_data;

public:
	NamedSemEventBase() {
		 m_data = NULL;
	}

	void
	close() {
		if (!m_data)
			return;

		m_sem.close();
		m_mapping.close();
		m_data = NULL;

		if (!m_name.isEmpty()) {
			psx::NamedSem::unlink(m_name);
			io::psx::SharedMemory::unlink(m_name);
			m_name.clear();
		}
	}

	bool
	create(const sl::StringRef& name) {
		close();

		io::psx::SharedMemory sharedMemory;
		bool result = sharedMemory.open(name, O_RDWR | O_CREAT | O_EXCL);
		if (!result)
			return false;

		result =
			sharedMemory.setSize(sizeof(Data)) &&
			(m_data = (Data*)m_mapping.map(
				NULL,
				sizeof(Data),
				PROT_READ | PROT_WRITE,
				MAP_SHARED,
				sharedMemory
			));

		if (!result) {
			sharedMemory.unlink(name);
			return false;
		}

		result = m_sem.open(name, O_CREAT | O_EXCL);
		if (!result) {
			sharedMemory.unlink(name);
			m_mapping.close();
			m_data = NULL;
			return false;
		}

		m_name = name;
		return true;
	}

	bool
	open(const sl::StringRef& name) {
		close();

		io::psx::SharedMemory sharedMemory;
		bool result = sharedMemory.open(name, O_RDWR);
		if (!result)
			return false;

		m_data = (Data*)m_mapping.map(
			NULL,
			sizeof(Data),
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			sharedMemory
		);

		if (!m_data)
			return false;

		result = m_sem.open(name, 0);
		if (!result) {
			m_mapping.close();
			m_data = NULL;
			return false;
		}

		return true;
	}

	bool
	signal() {
		ASSERT(m_data);

		sys::atomicLock(&m_data->m_lock);
		if (m_data->m_state) {
			sys::atomicUnlock(&m_data->m_lock);
			return true;
		}

		m_data->m_state = true;

		bool result = true;
		if (IsNotificationEvent()())
			for (uint32_t i = 0; i < m_data->m_waitCount; i++)
				result = m_sem.signal() && result;
		else if (m_data->m_waitCount)
			result = m_sem.signal();

		sys::atomicUnlock(&m_data->m_lock);
		return result;
	}

	bool
	reset() {
		ASSERT(m_data);

		sys::atomicLock(&m_data->m_lock);
		m_data->m_state = false;
		sys::atomicUnlock(&m_data->m_lock);
		return true;
	}

	bool
	wait() {
		sys::atomicLock(&m_data->m_lock);
		if (m_data->m_state) {
			if (!IsNotificationEvent()())
				m_data->m_state = false;

			sys::atomicUnlock(&m_data->m_lock);
			return true;
		}

		m_data->m_waitCount++;

		for (;;) {
			sys::atomicUnlock(&m_data->m_lock);

			bool result = m_sem.wait();

			sys::atomicLock(&m_data->m_lock);
			if (!result || m_data->m_state)
				break;
		}

		bool result = m_data->m_state;
		if (result && !IsNotificationEvent()())
			m_data->m_state = false;

		m_data->m_waitCount--;
		sys::atomicUnlock(&m_data->m_lock);
		return result;
	}
};

// cannot be destroyed while someone is waiting on it!

template <typename IsNotificationEvent>
class NameableEventBase {
protected:
	enum EventKind {
		EventKind_None = 0,
		EventKind_Unnamed,
		EventKind_Named,
	};

	typedef CondMutexEventBase<IsNotificationEvent> UnnamedEventImpl;
	typedef NamedSemEventBase<IsNotificationEvent> NamedEventImpl;

protected:
	union {
		char m_eventBuffer[AXL_MAX(sizeof(UnnamedEventImpl), sizeof(NamedEventImpl))]; // avoid heap allocations
		uint64_t _m_align;
	};

	EventKind m_eventKind;

public:
	NameableEventBase() {
		m_eventKind = EventKind_None;
		create();
	}

	~NameableEventBase() {
		close();
	}

	void
	close() {
		if (!m_eventKind)
			return;

		m_eventKind == EventKind_Named ?
			getNamedEvent()->~NamedEventImpl() :
			getUnnamedEvent()->~UnnamedEventImpl();

		m_eventKind = EventKind_None;
	}

	bool
	create() {
		close();

		new(m_eventBuffer) UnnamedEventImpl;
		m_eventKind = EventKind_Unnamed;
		return true;
	}

	bool
	create(const sl::StringRef& name) {
		close();

		new(m_eventBuffer) NamedEventImpl;
		m_eventKind = EventKind_Named;

		bool result = getNamedEvent()->create(name);
		if (!result) {
			close();
			return false;
		}

		return true;
	}

	bool
	open(const sl::StringRef& name) {
		close();

		new(m_eventBuffer) NamedEventImpl;
		m_eventKind = EventKind_Named;

		bool result = getNamedEvent()->open(name);
		if (!result) {
			close();
			return false;
		}

		return true;

	}

	bool
	wait() {
		ASSERT(m_eventKind);
		return m_eventKind == EventKind_Named ?
			getNamedEvent()->wait() :
			getUnnamedEvent()->wait();
	}

	bool
	signal() {
		ASSERT(m_eventKind);
		return m_eventKind == EventKind_Named ?
			getNamedEvent()->signal() :
			getUnnamedEvent()->signal();
	}

	bool
	reset() {
		ASSERT(m_eventKind);
		return m_eventKind == EventKind_Named ?
			getNamedEvent()->reset() :
			getUnnamedEvent()->reset();
	}

protected:
	UnnamedEventImpl*
	getUnnamedEvent() {
		ASSERT(m_eventKind == EventKind_Unnamed);
		return (UnnamedEventImpl*)m_eventBuffer;
	}

	NamedEventImpl*
	getNamedEvent() {
		ASSERT(m_eventKind == EventKind_Named);
		return (NamedEventImpl*)m_eventBuffer;
	}

};

#	else // _AXL_OS_DARWIN

// cannot be destroyed while someone is waiting on it!

template <typename IsNotificationEvent>
class NameableEventBase {
protected:
	typedef CondMutexEventBase<IsNotificationEvent> EventImpl;

protected:
	EventImpl* m_event;
	io::psx::Mapping m_mapping;
	sl::String m_name;

	union {
		char m_unnamedEventBuffer[sizeof(EventImpl)]; // avoid heap allocations
		uint64_t _m_align;
	};

public:
	NameableEventBase() {
		m_event = NULL;
		create();
	}

	~NameableEventBase() {
		close();
	}

	void
	close() {
		if (!m_event)
			return;

		if (m_event == (EventImpl*)m_unnamedEventBuffer) {
			m_event->~EventImpl();
		} else if (!m_name.isEmpty()) {
			io::psx::SharedMemory::unlink(m_name);
			m_event->~EventImpl();
		}

		m_mapping.close();
		m_name.clear();
		m_event = NULL;
	}

	bool
	create() {
		close();

		m_event = (EventImpl*)m_unnamedEventBuffer;
		new(m_event) EventImpl;
		return true;
	}

	bool
	create(const sl::StringRef& name) {
		close();

		psx::CondAttr condAttr;
		psx::MutexAttr mutexAttr;

		bool result =
			condAttr.setProcessShared(PTHREAD_PROCESS_SHARED) &&
			mutexAttr.setProcessShared(PTHREAD_PROCESS_SHARED);

		if (!result)
			return false;

		io::psx::SharedMemory sharedMemory;
		result = sharedMemory.open(name, O_RDWR | O_CREAT | O_EXCL);
		if (!result)
			return false;

		result =
			sharedMemory.setSize(sizeof(EventImpl)) &&
			(m_event = (EventImpl*)m_mapping.map(
				NULL,
				sizeof(EventImpl),
				PROT_READ | PROT_WRITE,
				MAP_SHARED,
				sharedMemory
			));

		if (!result) {
			sharedMemory.unlink(name);
			return false;
		}

		new (m_event) EventImpl(condAttr, mutexAttr);
		m_name = name;
		return true;
	}

	bool
	open(const sl::StringRef& name) {
		close();

		io::psx::SharedMemory sharedMemory;

		bool result = sharedMemory.open(name, O_RDWR); // no O_CREAT
		if (!result)
			return false;

		m_event = (EventImpl*)m_mapping.map(
			NULL,
			sizeof(EventImpl),
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			sharedMemory
		);

		return m_event != NULL;
	}

	bool
	wait(uint_t timeout = -1) {
		ASSERT(m_event);
		return m_event->wait(timeout);
	}

	bool
	signal() {
		ASSERT(m_event);
		return m_event->signal();
	}

	bool
	reset() {
		ASSERT(m_event);
		return m_event->reset();
	}
};

#	endif // _AXL_OS_DARWIN
#endif    // _AXL_OS_POSIX

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef NameableEventBase<sl::False<bool> > NameableEvent;
typedef NameableEventBase<sl::True<bool> > NameableNotificationEvent;

//..............................................................................

} // namespace sys
} // namespace axl
