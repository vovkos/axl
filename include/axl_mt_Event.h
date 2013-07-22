// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
#endif

namespace axl {
namespace mt {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

class CEvent
{
public:
	win::CEvent m_Event;	

public:
	CEvent ()
	{
		m_Event.Create (NULL, false, false, NULL);
	}

	bool
	Signal ()
	{ 
		return m_Event.Signal ();
	}

	bool
	Wait (uint_t Timeout = -1)
	{
		return m_Event.Wait (Timeout) == win::EWaitResult_Object0;
	}
};

//.............................................................................

class CNotificationEvent
{
public:
	win::CEvent m_Event;	

public:
	CNotificationEvent ()
	{
		m_Event.Create (NULL, true, false, NULL);
	}

	bool
	Signal ()
	{ 
		return m_Event.Signal ();
	}

	bool
	Reset ()
	{ 
		return m_Event.Reset ();
	}

	bool
	Wait (uint_t Timeout = -1)
	{
		return m_Event.Wait (Timeout) == win::EWaitResult_Object0;
	}
};

//.............................................................................

#elif (_AXL_ENV == AXL_ENV_POSIX)

class CEvent
{
protected:
	psx::CMutex m_Mutex;
	psx::CSem m_Sem;

public:
	bool
	Signal ();

	bool
	Wait (uint_t Timeout = -1)
	{
		return m_Sem.Wait (Timeout);
	}
};

class CNotificationEvent
{
protected:
	psx::CMutex m_Mutex;
	psx::CCond m_Cond;
	bool m_State;

public:
	CNotificationEvent ()
	{
		m_State = false;
	}	
	
	bool
	Signal ();
	
	bool
	Reset ();

	bool
	Wait (uint_t Timeout = -1);
};

#endif

//.............................................................................

} // namespace mt 
} // namespace axl
