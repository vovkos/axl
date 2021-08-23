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
	MutexAttr()
	{
		int result = ::pthread_mutexattr_init(&m_attr);
		ASSERT(result == 0);
	}

	MutexAttr(int type)
	{
		int result = ::pthread_mutexattr_init(&m_attr);
		ASSERT(result == 0);
		setType(type);
	}

	~MutexAttr()
	{
		int result = ::pthread_mutexattr_destroy(&m_attr);
		ASSERT(result == 0);
	}

	operator const pthread_mutexattr_t* () const
	{
		return &m_attr;
	}

	operator pthread_mutexattr_t* ()
	{
		return &m_attr;
	}

#if (!_AXL_OS_DARWIN) // PTHREAD_PROCESS_SHARED on Darwin is... weird, better not use it
	bool
	getProcessShared(int* value) const;

	bool
	setProcessShared(int value);
#endif

	bool
	getType(int* value) const;

	bool
	setType(int value);
};

//..............................................................................

class Mutex
{
protected:
	pthread_mutex_t m_mutex;

public:
	Mutex(const pthread_mutexattr_t* attr = NULL)
	{
		int result = ::pthread_mutex_init(&m_mutex, attr);
		ASSERT(result == 0);
	}

	Mutex(int type)
	{
		MutexAttr attr(type);
		int result = ::pthread_mutex_init(&m_mutex, attr);
		ASSERT(result == 0);
	}

	~Mutex()
	{
		int result = ::pthread_mutex_destroy(&m_mutex);
		ASSERT(result == 0);
	}

	operator pthread_mutex_t* ()
	{
		return &m_mutex;
	}

	bool
	tryLock();

	void
	lock()
	{
		int result = ::pthread_mutex_lock(&m_mutex);
		ASSERT(result == 0);
	}

#if (!_AXL_OS_DARWIN)
	bool
	lock(uint_t timeout);
#endif

	void
	unlock()
	{
		::pthread_mutex_unlock(&m_mutex);
	}
};

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
