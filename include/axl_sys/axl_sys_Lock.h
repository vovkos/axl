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

#define _AXL_SYS_LOCK_H

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_CriticalSection.h"
#elif (_AXL_OS_POSIX)
#	include "axl_sys_psx_Mutex.h"
#endif

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

class Lock
{
public:
	win::CriticalSection m_criticalSection;

public:
	void
	lock()
	{
		m_criticalSection.enter();
	}

	void
	unlock()
	{
		m_criticalSection.leave();
	}
};

#elif (_AXL_OS_POSIX)

class Lock
{
public:
	psx::Mutex m_mutex;

public:
	void
	lock()
	{
		m_mutex.lock();
	}

	void
	unlock()
	{
		m_mutex.unlock();
	}
};

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ScopeLock
{
protected:
	Lock* m_lock;

public:
	ScopeLock(Lock* lock)
	{
		m_lock = lock;
		lock->lock();
	}

	~ScopeLock()
	{
		m_lock->unlock();
	}
};

//..............................................................................

} // namespace sys
} // namespace axl
