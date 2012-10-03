// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_SEMAPHORE_H

#include "axl_win_Semaphore.h"

namespace axl {
namespace mt {

//.............................................................................

class CSemaphore
{
public:
	win::CSemaphore m_Semaphore;

public:
	CSemaphore (ulong_t InitialCount = 0)
	{
		Create (InitialCount);
	}

	operator HANDLE ()
	{ 
		return m_Semaphore;
	}

	bool 
	IsOpen ()
	{
		return m_Semaphore.IsOpen ();
	}

	void 
	Close ()
	{
		m_Semaphore.Close ();
	}

	bool 
	Create (ulong_t InitialCount = 0)
	{
		return m_Semaphore.Create (NULL, InitialCount, MAXLONG, NULL);
	}

	bool
	Signal (ulong_t Count = 1)
	{
		return m_Semaphore.Signal (Count);
	}

	bool
	Wait (ulong_t Timeout = -1)
	{
		return m_Semaphore.Wait (Timeout) == win::CWaitableHandle::EWaitResult_Object0;
	}
};

//.............................................................................

} // namespace mt
} // namespace axl
