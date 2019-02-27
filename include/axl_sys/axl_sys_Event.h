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
#endif

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

template <typename IsNotificationEvent>
class EventBase
{
public:
	win::Event m_event;

public:
	EventBase()
	{
		m_event.create(NULL, IsNotificationEvent() (), false, NULL);
	}

	bool
	signal()
	{
		return m_event.signal();
	}

	bool
	reset()
	{
		return m_event.reset();
	}

	bool
	wait(uint_t timeout = -1)
	{
		return m_event.wait(timeout) == win::WaitResult_Object0;
	}
};

//..............................................................................

#elif (_AXL_OS_POSIX)

template <typename IsNotificationEvent>
class EventBase
{
protected:
	psx::CondMutexPair m_condMutexPair;
	volatile bool m_state;

public:
	EventBase()
	{
		 m_state = false;
	}

	EventBase(
		const pthread_condattr_t* condAttr,
		const pthread_mutexattr_t* mutexAttr
		):
		m_condMutexPair(condAttr, mutexAttr)
	{
		m_state = false;
	}

	void
	close();

	bool
	reset()
	{
		m_condMutexPair.lock();
		m_state = false;
		m_condMutexPair.unlock();
		return true;
	}

	bool
	wait(uint_t timeout = -1)
	{
		m_condMutexPair.lock();

		if (m_state)
		{
			if (!IsNotificationEvent() ())
				m_state = false;

			m_condMutexPair.unlock();
			return true;
		}

		bool result = m_condMutexPair.wait(timeout);
		if (!result)
		{
			m_condMutexPair.unlock();
			return false;
		}

		if (!IsNotificationEvent() ())
			m_state = false;

		m_condMutexPair.unlock();

		return true;
	}

	bool
	signal()
	{
		m_condMutexPair.lock();
		if (m_state)
		{
			m_condMutexPair.unlock();
			return true;
		}

		bool result = IsNotificationEvent() () ?
			m_condMutexPair.broadcast() :
			m_condMutexPair.signal();

		if (!result)
		{
			m_condMutexPair.unlock();
			return false;
		}

		m_state = true;
		m_condMutexPair.unlock();

		return true;
	}
};

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef EventBase<sl::False<bool> > Event;
typedef EventBase<sl::True<bool> > NotificationEvent;

//..............................................................................

// there is no simple mapping between win32/linux/osx IPC primitives to
// allow natural implementaiton of a universal nameable event -- which can be:

// 1.a) auto-reset
// 1.b) manual-reset
// 2.a) anonymous
// 2.b) named

// on windows, everything maps directly to win::Event
// on posix, we need to use a condvar/mutex/flags in a shared memory block

#if (_AXL_OS_WIN)

template <typename IsNotificationEvent>
class NameableEventBase
{
protected:
	sys::win::Event m_event;

public:
	void
	close()
	{
		m_event.close();
	}

	bool
	create()
	{
		return m_event.create(NULL, IsNotificationEvent() (), false);
	}

	bool
	create(const sl::StringRef& name)
	{
		return m_event.create(NULL, IsNotificationEvent() (), false, name.s2());
	}

	bool
	open(const sl::StringRef& name)
	{
		return m_event.open(EVENT_ALL_ACCESS, false, name.s2());
	}

	bool
	wait(uint_t timeout = -1)
	{
		return m_event.wait(timeout) == sys::win::WaitResult_Object0;
	}

	bool
	signal()
	{
		return m_event.signal();
	}

	bool
	reset()
	{
		return m_event.reset();
	}
};

//..............................................................................

#elif (_AXL_OS_POSIX)

// cannot be destroyed while someone is waiting on it!

template <typename IsNotificationEvent>
class NameableEventBase
{
public:
	typedef EventBase<IsNotificationEvent> EventImpl;

protected:
	EventImpl* m_event;
	io::psx::Mapping m_mapping;
	sl::String m_name;

	union
	{
		char m_unnamedEventBuffer[sizeof(EventImpl)]; // avoid heap allocations
		uint64_t _m_align;
	};

public:
	NameableEventBase()
	{
		m_event = NULL;
		create();
	}

	~NameableEventBase()
	{
		close();
	}

	void
	close()
	{
		if (!m_event)
			return;

		if (m_event == (EventImpl*)m_unnamedEventBuffer)
		{
			m_event->~EventImpl();
		}
		else if (!m_name.isEmpty())
		{
			io::psx::SharedMemory::unlink(m_name);
			m_event->~EventImpl();
		}

		m_mapping.close();
		m_name.clear();
		m_event = NULL;
	}

	bool
	create()
	{
		close();

		m_event = (EventImpl*)m_unnamedEventBuffer;
		new(m_event)EventImpl;
		return true;
	}

	bool
	create(const sl::StringRef& name)
	{
		close();

		io::psx::SharedMemory sharedMemory;

		bool result =
			sharedMemory.open(name) &&
			sharedMemory.setSize(sizeof(EventImpl));

		if (!result)
			return false;

		m_event = (EventImpl*)m_mapping.map(
			NULL,
			sizeof(EventImpl),
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			sharedMemory
			);

		if (!m_event)
		{
			sharedMemory.unlink(name);
			return false;
		}

		psx::CondAttr condAttr;
		psx::MutexAttr mutexAttr;

		condAttr.setProcessShared(PTHREAD_PROCESS_SHARED);
		mutexAttr.setProcessShared(PTHREAD_PROCESS_SHARED);

		new(m_event)EventImpl(condAttr, mutexAttr);
		m_name = name;
		return true;
	}

	bool
	open(const sl::StringRef& name)
	{
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
	wait(uint_t timeout = -1)
	{
		ASSERT(m_event);
		return m_event->wait(timeout);
	}

	bool
	signal()
	{
		ASSERT(m_event);
		return m_event->signal();
	}

	bool
	reset()
	{
		ASSERT(m_event);
		m_event->reset(); // returns void on POSIX
		return true;
	}
};

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef NameableEventBase<sl::False<bool> > NameableEvent;
typedef NameableEventBase<sl::True<bool> > NameableNotificationEvent;

//..............................................................................

} // namespace sys
} // namespace axl
