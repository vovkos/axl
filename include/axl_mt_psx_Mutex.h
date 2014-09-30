// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_PSX_MUTEX_H

#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

class MutexAttr
{
protected:
	pthread_mutexattr_t m_attr;

public:
	MutexAttr ()
	{
		pthread_mutexattr_init (&m_attr);
	}

	~MutexAttr ()
	{
		pthread_mutexattr_destroy (&m_attr);
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
	getType () const
	{
		int type = PTHREAD_MUTEX_NORMAL;
		pthread_mutexattr_gettype (&m_attr, &type);
		return type;
	}

	bool
	setType (int type)
	{
		int result = pthread_mutexattr_settype (&m_attr, type);
		return result == 0 ? true : err::fail (result);
	}

#if (0)
	int
	getRobust () const
	{
		int robust = PTHREAD_MUTEX_STALLED;
		pthread_mutexattr_getrobust (&m_attr, &robust);
		return robust;
	}

	bool
	setRobust (int robust)
	{
		int result = pthread_mutexattr_setrobust (&m_attr, robust);
		return result == 0 ? true : err::fail (result);
	}
#endif
};

//.............................................................................

class Mutex
{
protected:
	pthread_mutex_t m_mutex;

public:
	Mutex (pthread_mutexattr_t* attr = NULL)
	{
		pthread_mutex_init (&m_mutex, attr);
	}

	~Mutex ()
	{
		pthread_mutex_destroy (&m_mutex);
	}

	operator pthread_mutex_t* ()
	{
		return &m_mutex;
	}

	void
	lock ()
	{
		pthread_mutex_lock (&m_mutex);
	}

	bool
	lock (uint_t timeout);

	void
	unlock ()
	{
		pthread_mutex_unlock (&m_mutex);
	}
};

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
