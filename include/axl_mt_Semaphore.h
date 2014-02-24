// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	CSemaphore (size_t InitialCount = 0)
	{
		Create (InitialCount);
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
	Create (size_t InitialCount = 0)
	{
		return m_Semaphore.Create (NULL, (uint_t) InitialCount, MAXLONG, NULL);
	}

	bool
	Signal (size_t Count = 1)
	{
		return m_Semaphore.Signal ((uint_t) Count);
	}

	bool
	Wait (uint_t Timeout = -1)
	{
		return m_Semaphore.Wait (Timeout) == win::EWaitResult_Object0;
	}
};

//.............................................................................

} // namespace mt
} // namespace axl
