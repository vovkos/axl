// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_PSX_MUTEX_H

#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

class CMutexAttr
{
protected:
	pthread_mutexattr_t m_Attr;
	
public:
	CMutexAttr ()
	{
		pthread_mutexattr_init (&m_Attr);
	}

	~CMutexAttr ()
	{
		pthread_mutexattr_destroy (&m_Attr);
	}
	
	operator const pthread_mutexattr_t* () const
	{
		return &m_Attr;
	}

	operator pthread_mutexattr_t* ()
	{
		return &m_Attr;
	}
	
	int
	GetType () const
	{
		int Type = PTHREAD_MUTEX_NORMAL;
		pthread_mutexattr_gettype (&m_Attr, &Type);
		return Type;
	}
	
	bool
	SetType (int Type)
	{
		int Result = pthread_mutexattr_settype (&m_Attr, Type);
		return Result == 0 ? true : err::Fail (Result);		
	}

	int
	GetRobust () const
	{
		int Robust = PTHREAD_MUTEX_STALLED;
		pthread_mutexattr_getrobust (&m_Attr, &Robust);
		return Robust;
	}
	
	bool
	SetRobust (int Robust)
	{
		int Result = pthread_mutexattr_setrobust (&m_Attr, Robust);
		return Result == 0 ? true : err::Fail (Result);		
	}
};

//.............................................................................

class CMutex
{
protected:
	pthread_mutex_t m_Mutex;
	
public:
	CMutex (pthread_mutexattr_t* pAttr = NULL)
	{		
		pthread_mutex_init (&m_Mutex, pAttr);
	}

	~CMutex ()
	{
		pthread_mutex_destroy (&m_Mutex);
	}

	operator pthread_mutex_t* ()
	{
		return &m_Mutex;
	}	
	
	void 
	Lock ()
	{
		pthread_mutex_lock (&m_Mutex);
	}

	bool
	Lock (uint_t Timeout);
	
	void 
	Unlock ()
	{
		pthread_mutex_unlock (&m_Mutex);
	}
};

//.............................................................................

} // namespace psx
} // namespace mt 
} // namespace axl
