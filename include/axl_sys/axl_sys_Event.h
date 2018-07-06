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
#endif

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

class EventRoot
{
public:
	win::Event m_event;

protected:
	EventRoot () // protected construction
	{
	}

public:
	bool
	signal ()
	{
		return m_event.signal ();
	}

	bool
	reset ()
	{
		return m_event.reset ();
	}

	bool
	wait (uint_t timeout = -1)
	{
		return m_event.wait (timeout) == win::WaitResult_Object0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Event: public EventRoot
{
public:
	Event ()
	{
		m_event.create (NULL, false, false, NULL);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class NotificationEvent: public EventRoot
{
public:
	NotificationEvent ()
	{
		m_event.create (NULL, true, false, NULL);
	}
};

//..............................................................................

#elif (_AXL_OS_POSIX)

class EventBase
{
protected:
	psx::CondMutexPair m_condMutexPair;
	bool m_isNotificationEvent;
	volatile bool m_state;

public:
	EventBase (bool isNotificationEvent)
	{
		m_isNotificationEvent = isNotificationEvent;
		m_state = false;
	}

	void
	reset ();

	bool
	signal ();

	bool
	wait (uint_t timeout = -1);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Event: public EventBase
{
public:
	Event ():
		EventBase (false)
	{
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class NotificationEvent: public EventBase
{
public:
	NotificationEvent ():
		EventBase (true)
	{
	}
};

#endif

//..............................................................................

// there is no simple mapping between win32/linux/osx IPC primitives to
// allow natural implementaiton of a universal event -- which can be:

// 1.a) auto-reset
// 1.b) manual-reset
// 2.a) anonymous
// 2.b) named

// on windows, this maps directly to win::Event

#if (_AXL_OS_WIN)

template <typename IsNotificationEvent>
class UniversalEventBase
{
protected:
	sys::win::Event m_event;

public:
	void
	close ()
	{
		m_event.close ();
	}

	bool
	create ()
	{
		return m_event.create (NULL, IsNotificationEvent () (), false);
	}

	bool
	create (const sl::StringRef& name)
	{
		return m_event.create (NULL, IsNotificationEvent () (), false, name.s2 ());
	}

	bool
	open (const sl::StringRef& name)
	{
		return m_event.open (EVENT_ALL_ACCESS, false, name.s2 ());
	}

	bool
	wait (uint_t timeout = -1)
	{
		return m_event.wait (timeout) == sys::win::WaitResult_Object0;
	}

	bool
	signal ()
	{
		return m_event.signal ();
	}

	bool
	reset ()
	{
		return m_event.reset ();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef UniversalEventBase <sl::False <bool> > UniversalEvent;
typedef UniversalEventBase <sl::True <bool> > UniversalNotificationEvent;

//..............................................................................

#elif (_AXL_OS_POSIX)
	enum EventKind
	{
		EventKind_UnnamedEvent,
		EventKind_NamedSem,
	};

	class EventBase // pseudo-virtual base
	{
	protected:
		EventKind m_eventKind;

	public:
		virtual ~EventBase ()
		{
		}

		// hot path functions are non-virtual for better performance

		bool
		signal ();

		void
		reset ();

		bool
		wait (uint_t timeout);
	};

	class UnnamedEvent: public EventBase
	{
		friend class EventBase;

	protected:
		sys::EventBase m_event;

	public:
		UnnamedEvent (bool isNotificationEvent):
			m_event (isNotificationEvent)
		{
			m_eventKind = EventKind_UnnamedEvent;
		}
	};

	class NamedSemEvent: public EventBase
	{
		friend class EventBase;

	protected:
		sys::psx::NamedSem m_sem;
		sl::String m_name;

	public:
		NamedSemEvent ()
		{
			m_eventKind = EventKind_NamedSem;
		}

		~NamedSemEvent ();

		bool
		create (const sl::StringRef& name);

		bool
		open (const sl::StringRef& name);
	};
#endif

//..............................................................................

} // namespace sys
} // namespace axl
