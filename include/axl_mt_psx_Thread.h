// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_PSX_THREAD_H

#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

class CThreadAttr
{
protected:
	pthread_attr_t m_attr;
	
public:
	CThreadAttr ()
	{
		pthread_attr_init (&m_attr);
	}

	~CThreadAttr ()
	{
		pthread_attr_destroy (&m_attr);
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
		pthread_attr_getaffinity_np (&m_attr, sizeof (cpu_set_t), &cpuSet);
		return cpuSet;
	}
	
	bool
	setAffinity (cpu_set_t cpuSet)
	{
		int result = pthread_attr_setaffinity_np (&m_attr, sizeof (cpu_set_t), &cpuSet);
		return result == 0 ? true : err::fail (result);		
	}
	
	size_t 
	getStackSize () const
	{
		size_t size = -1;
		pthread_attr_getstacksize (&m_attr, &size);
		return size;
	}

	bool
	setStackSize (size_t size)
	{
		int result = pthread_attr_setstacksize (&m_attr, size);
		return result == 0 ? true : err::fail (result);		
	}
};

//.............................................................................

typedef void* (FThreadProc) (void* context);

//.............................................................................

class CThread
{
protected:
	pthread_t m_threadId;
	bool m_isOpen;
	
public:
	CThread ()
	{
		m_isOpen = false;
	}
	
	~CThread ()
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
		FThreadProc pfThreadProc,
		void* context = NULL
		);

	bool 
	create (
		FThreadProc pfThreadProc,
		void* context = NULL
		)
	{
		return create (NULL, pfThreadProc, context);
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
