// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_EVENT_H

#include "axl_win_Event.h"

namespace axl {
namespace mt {

//.............................................................................

class CEvent
{
public:
	enum EKind
	{
		EKind_Synchronization = 0,
		EKind_Notification    = 1,
	};

public:
	win::CEvent m_Event;

public:
	CEvent (EKind Kind = EKind_Synchronization)
	{
		Create (Kind);
	}

	operator HANDLE ()
	{ 
		return m_Event;
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
	Create (EKind Kind = EKind_Synchronization)
	{
		return m_Event.Create (NULL, Kind == EKind_Notification, false, NULL);
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
	Wait (ulong_t Timeout = -1)
	{
		return m_Event.Wait (Timeout) == win::CWaitableHandle::EWaitResult_Object0;
	}
};

//.............................................................................

} // namespace mt 
} // namespace axl
