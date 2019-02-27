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

#define _AXL_SYS_PSX_THREAD_H

#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

class ThreadAttr
{
protected:
	pthread_attr_t m_attr;

public:
	ThreadAttr()
	{
		::pthread_attr_init(&m_attr);
	}

	~ThreadAttr()
	{
		::pthread_attr_destroy(&m_attr);
	}

	operator const pthread_attr_t* () const
	{
		return &m_attr;
	}

	operator pthread_attr_t* ()
	{
		return &m_attr;
	}

#if (!_AXL_OS_DARWIN)
	cpu_set_t
	getAffinity() const
	{
		cpu_set_t cpuSet = { 0 };
		::pthread_attr_getaffinity_np(&m_attr, sizeof(cpu_set_t), &cpuSet);
		return cpuSet;
	}

	bool
	setAffinity(cpu_set_t cpuSet)
	{
		int result = ::pthread_attr_setaffinity_np(&m_attr, sizeof(cpu_set_t), &cpuSet);
		return result == 0 ? true : err::fail(result);
	}
#endif

	size_t
	getStackSize() const
	{
		size_t size = -1;
		::pthread_attr_getstacksize(&m_attr, &size);
		return size;
	}

	bool
	setStackSize(size_t size)
	{
		int result = ::pthread_attr_setstacksize(&m_attr, size);
		return result == 0 ? true : err::fail(result);
	}
};

//..............................................................................

typedef
void*
ThreadFunc(void* context);

//..............................................................................

class Thread
{
protected:
	pthread_t m_threadId;
	bool m_isOpen;

public:
	Thread()
	{
		m_isOpen = false;
	}

	~Thread()
	{
		detach();
	}

	operator pthread_t()
	{
		return m_threadId;
	}

	bool
	isOpen()
	{
		return m_isOpen;
	}

	bool
	create(
		const pthread_attr_t* attr,
		ThreadFunc* threadFunc,
		void* context = NULL
		);

	bool
	create(
		ThreadFunc* threadFunc,
		void* context = NULL
		)
	{
		return create(NULL, threadFunc, context);
	}

	bool
	cancel()
	{
		int result = ::pthread_cancel(m_threadId);
		return result != 0 ? true : err::fail(result);
	}

	bool
	join(void** retVal = NULL);

#if (!_AXL_OS_DARWIN)
	bool
	tryJoin(void** retVal = NULL);

	bool
	join(
		uint_t timeout,
		void** retVal = NULL
		);
#endif

	bool
	detach();
};

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
