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

#define _AXL_SYS_PSX_COND_H

#include "axl_sys_psx_Mutex.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

class Cond
{
protected:
	pthread_cond_t m_cond;

public:
	Cond ()
	{
		::pthread_cond_init (&m_cond, NULL);
	}

	~Cond ()
	{
		::pthread_cond_destroy (&m_cond);
	}

	operator pthread_cond_t* ()
	{
		return &m_cond;
	}

	bool
	signal ()
	{
		int result = ::pthread_cond_signal (&m_cond);
		return result == 0 ? true : err::fail (result);
	}

	bool
	broadcast ()
	{
		int result = ::pthread_cond_broadcast (&m_cond);
		return result == 0 ? true : err::fail (result);
	}

	bool
	wait (pthread_mutex_t* mutex)
	{
		int result = ::pthread_cond_wait (&m_cond, mutex);
		return result == 0 ? true : err::fail (result);
	}

	bool
	wait (
		pthread_mutex_t* mutex,
		uint_t timeout
		);
};

//..............................................................................

class CondMutexPair
{
protected:
	Cond m_cond;
	Mutex m_mutex;

public:
	bool
	tryLock ()
	{
		return m_mutex.tryLock ();
	}

	void
	lock ()
	{
		m_mutex.lock ();
	}

	void
	unlock ()
	{
		m_mutex.unlock ();
	}

	bool
	signal ()
	{
		return m_cond.signal ();
	}

	bool
	broadcast ()
	{
		return m_cond.broadcast ();
	}

	bool
	wait ()
	{
		return m_cond.wait (m_mutex);
	}

	bool
	wait (uint_t timeout)
	{
		return m_cond.wait (m_mutex, timeout);
	}
};

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
