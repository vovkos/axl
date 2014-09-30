// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_EVENT_H

#include "axl_rtl_Func.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_mt_win_Event.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_mt_psx_Mutex.h"
#	include "axl_mt_psx_Sem.h"
#	include "axl_mt_psx_Cond.h"
#	include "axl_io_psx_Mapping.h"
#endif

namespace axl {
namespace mt {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

class CEventRoot
{
public:
	win::CEvent m_event;

protected:
	CEventRoot () // protected construction
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
		return m_event.wait (timeout) == win::EWaitResult_Object0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CEvent: public CEventRoot
{
public:
	CEvent ()
	{
		m_event.create (NULL, false, false, NULL);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CNotificationEvent: public CEventRoot
{
public:
	CNotificationEvent ()
	{
		m_event.create (NULL, true, false, NULL);
	}
};

//.............................................................................

#elif (_AXL_ENV == AXL_ENV_POSIX)

class CEvent
{
protected:
	psx::CMutex m_mutex;
	psx::CSem m_sem;

public:
	bool
	signal ();

	bool
	reset ();

	bool
	wait (uint_t timeout = -1)
	{
		return m_sem.wait (timeout);
	}
};

class CNotificationEvent
{
protected:
	psx::CMutex m_mutex;
	psx::CCond m_cond;
	bool m_state;

public:
	CNotificationEvent ()
	{
		m_state = false;
	}

	bool
	signal ();

	bool
	reset ();

	bool
	wait (uint_t timeout = -1);
};

#endif

//.............................................................................

} // namespace mt
} // namespace axl
