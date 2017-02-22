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

#define _AXL_SYS_PSX_MUTEX_H

#include "axl_g_Pch.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

class MutexAttr
{
protected:
	pthread_mutexattr_t m_attr;

public:
	MutexAttr ()
	{
		::pthread_mutexattr_init (&m_attr);
	}

	~MutexAttr ()
	{
		::pthread_mutexattr_destroy (&m_attr);
	}

	operator const pthread_mutexattr_t* () const
	{
		return &m_attr;
	}

	operator pthread_mutexattr_t* ()
	{
		return &m_attr;
	}

	int
	getType () const;

	bool
	setType (int type);

#if (0)
	int
	getRobust () const
	{
		int robust = PTHREAD_MUTEX_STALLED;
		::pthread_mutexattr_getrobust (&m_attr, &robust);
		return robust;
	}

	bool
	setRobust (int robust)
	{
		int result = ::pthread_mutexattr_setrobust (&m_attr, robust);
		return result == 0 ? true : err::fail (result);
	}
#endif
};

//..............................................................................

class Mutex
{
protected:
	pthread_mutex_t m_mutex;

public:
	Mutex (pthread_mutexattr_t* attr = NULL)
	{
		::pthread_mutex_init (&m_mutex, attr);
	}

	~Mutex ()
	{
		::pthread_mutex_destroy (&m_mutex);
	}

	operator pthread_mutex_t* ()
	{
		return &m_mutex;
	}

	bool
	tryLock ();

	void
	lock ()
	{
		int result = ::pthread_mutex_lock (&m_mutex);
		ASSERT (result == 0);
	}

#if (!_AXL_OS_DARWIN)
	bool
	lock (uint_t timeout);
#endif

	void
	unlock ()
	{
		::pthread_mutex_unlock (&m_mutex);
	}
};

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
