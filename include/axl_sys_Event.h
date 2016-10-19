// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_EVENT_H

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Event.h"
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

class EventRoot
{
protected:
	psx::CondMutexPair m_condMutexPair;
	bool m_isNotificationEvent;
	volatile bool m_state;

public:
	EventRoot (bool isNotificationEvent)
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

class Event: public EventRoot
{
public:
	Event ():
		EventRoot (false)
	{
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class NotificationEvent: public EventRoot
{
public:
	NotificationEvent ():
		EventRoot (true)
	{
	}
};

#endif

//..............................................................................

} // namespace sys
} // namespace axl
