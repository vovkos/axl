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

#define _AXL_SYS_INVERTEDSEMAPHORE_H

#include "axl_sys_Lock.h"
#include "axl_sys_Event.h"

namespace axl {
namespace sys {

//..............................................................................

class InvertedSemaphore: public NotificationEvent
{
protected:
	Lock m_lock;
	intptr_t m_count;

public:
	InvertedSemaphore(intptr_t count = 0)
	{
		setImpl(count);
	}

	void
	set(intptr_t count)
	{
		m_lock.lock();
		setImpl(count);
		m_lock.unlock();
	}

	void
	add(intptr_t delta)
	{
		m_lock.lock();
		setImpl(m_count + delta);
		m_lock.unlock();
	}

protected:
	void
	setImpl(intptr_t count) // called under lock
	{
		ASSERT(count >= 0);
		m_count = count;

		if (m_count)
			reset();
		else
			signal();
	}

};

//..............................................................................

} // namespace sys
} // namespace axl
