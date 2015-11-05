// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_PSX_THREAD_H

#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

class ThreadAttr
{
protected:
	pthread_attr_t m_attr;

public:
	ThreadAttr ()
	{
		::pthread_attr_init (&m_attr);
	}

	~ThreadAttr ()
	{
		::pthread_attr_destroy (&m_attr);
	}

	operator const pthread_attr_t* () const
	{
		return &m_attr;
	}

	operator pthread_attr_t* ()
	{
		return &m_attr;
	}

	cpu_set_t
	getAffinity () const
	{
		cpu_set_t cpuSet = { 0 };
		::pthread_attr_getaffinity_np (&m_attr, sizeof (cpu_set_t), &cpuSet);
		return cpuSet;
	}

	bool
	setAffinity (cpu_set_t cpuSet)
	{
		int result = ::pthread_attr_setaffinity_np (&m_attr, sizeof (cpu_set_t), &cpuSet);
		return result == 0 ? true : err::fail (result);
	}

	size_t
	getStackSize () const
	{
		size_t size = -1;
		::pthread_attr_getstacksize (&m_attr, &size);
		return size;
	}

	bool
	setStackSize (size_t size)
	{
		int result = ::pthread_attr_setstacksize (&m_attr, size);
		return result == 0 ? true : err::fail (result);
	}
};

//.............................................................................

typedef 
void* 
ThreadFunc (void* context);

//.............................................................................

class Thread
{
protected:
	pthread_t m_threadId;
	bool m_isOpen;

public:
	Thread ()
	{
		m_isOpen = false;
	}

	~Thread ()
	{
		detach ();
	}

	operator pthread_t ()
	{
		return m_threadId;
	}

	bool
	isOpen ()
	{
		return m_isOpen;
	}

	bool
	create (
		const pthread_attr_t* attr,
		ThreadFunc* threadFunc,
		void* context = NULL
		);

	bool
	create (
		ThreadFunc* threadFunc,
		void* context = NULL
		)
	{
		return create (NULL, threadFunc, context);
	}

	bool
	join (
		uint_t timeout,
		void** retVal = NULL
		);

	bool
	join (void** retVal = NULL)
	{
		return join (-1, retVal);
	}

	bool
	detach ();
};

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
