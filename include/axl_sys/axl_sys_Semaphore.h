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

#define _AXL_SYS_SEMAPHORE_H

#include "axl_win_Semaphore.h"

namespace axl {
namespace sys {

//..............................................................................

class Semaphore
{
public:
	win::Semaphore m_semaphore;

public:
	Semaphore (size_t initialCount = 0)
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
		return m_semaphore.wait (timeout) == win::WaitResult_Object0;
	}
};

//..............................................................................

} // namespace sys
} // namespace axl
