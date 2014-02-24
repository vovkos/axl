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

class CCond
{
protected:	
	pthread_cond_t m_Cond;
	
public:
	CCond ()
	{		
		pthread_cond_init (&m_Cond, NULL);
	}

	~CCond ()
	{
		pthread_cond_destroy (&m_Cond);
	}

	operator pthread_cond_t* ()
	{
		return &m_Cond;
	}	
	
	bool 
	Signal ()
	{
		int Result = pthread_cond_signal (&m_Cond);
		return Result == 0 ? true : err::Fail (Result);
	}

	bool 
	Broadcast ()
	{
		int Result = pthread_cond_broadcast (&m_Cond);
		return Result == 0 ? true : err::Fail (Result);
	}	

	bool 
	Wait (pthread_mutex_t* pMutex)
	{
		int Result = pthread_cond_wait (&m_Cond, pMutex);
		return Result == 0 ? true : err::Fail (Result);	
	}
	
	bool 
	Wait (
		pthread_mutex_t* pMutex,
		uint_t Timeout
		);
};

//.............................................................................

class CCondMutexPair
{
protected:
	CCond m_Cond;
	CMutex m_Mutex;
	
public:
	void
	Lock ()
	{
		m_Mutex.Lock ();
	}

	void
	Unlock ()
	{
		m_Mutex.Unlock ();
	}

	bool 
	Signal ()
	{
		return m_Cond.Signal ();
	}

	bool 
	Broadcast ()
	{
		return m_Cond.Broadcast ();
	}

	bool 
	Wait ()
	{
		Lock ();
		return m_Cond.Wait (m_Mutex);
	}	
	
	bool 
	Wait (uint_t Timeout)
	{
		Lock ();
		return m_Cond.Wait (m_Mutex, Timeout);
	}	
};

//.............................................................................

} // namespace psx
} // namespace mt 
} // namespace axl
