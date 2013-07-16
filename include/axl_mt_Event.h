// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_EVENT_H

#include "axl_mt_win_Event.h"

namespace axl {
namespace mt {

//.............................................................................

enum EEvent
{
	EEvent_Synchronization = 0,
	EEvent_Notification    = 1,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CEvent
{
public:
	win::CEvent m_Event;

public:
	CEvent (EEvent EventKind = EEvent_Synchronization)
	{
		Create (EventKind);
	}

	bool 
	IsOpen ()
	{
		return m_Event.IsOpen ();
	}

	void 
	Close ()
	{
		m_Event.Close ();
	}

	bool 
	Create (EEvent EventKind = EEvent_Synchronization)
	{
		return m_Event.Create (NULL, EventKind == EEvent_Notification, false, NULL);
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

} // namespace mt 
} // namespace axl
