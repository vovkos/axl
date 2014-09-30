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
	win::CSemaphore m_semaphore;

public:
	CSemaphore (size_t initialCount = 0)
	{
		create (initialCount);
	}

	bool 
	isOpen ()
	{
		return m_semaphore.isOpen ();
	}

	void 
	close ()
	{
		m_semaphore.close ();
	}

	bool 
	create (size_t initialCount = 0)
	{
		return m_semaphore.create (NULL, (uint_t) initialCount, MAXLONG, NULL);
	}

	bool
	signal (size_t count = 1)
	{
		return m_semaphore.signal ((uint_t) count);
	}

	bool
	wait (uint_t timeout = -1)
	{
		return m_semaphore.wait (timeout) == win::EWaitResult_Object0;
	}
};

//.............................................................................

} // namespace mt
} // namespace axl
