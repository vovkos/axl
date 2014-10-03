// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_PSX_COND_H

#include "axl_mt_psx_Mutex.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

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

//.............................................................................

class CondMutexPair
{
protected:
	Cond m_cond;
	Mutex m_mutex;

public:
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
		lock ();
		return m_cond.wait (m_mutex);
	}

	bool
	wait (uint_t timeout)
	{
		lock ();
		return m_cond.wait (m_mutex, timeout);
	}
};

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
