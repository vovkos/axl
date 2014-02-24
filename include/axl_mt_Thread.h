// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_THREAD_H

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_mt_win_Thread.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_mt_psx_Thread.h"
#endif

namespace axl {
namespace mt {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

template <typename T>
class CThreadImplT
{
public:
	win::CThread m_Thread;

public:
	~CThreadImplT ()
	{
		WaitAndClose ();
	}

	bool
	IsOpen ()
	{
		return m_Thread.IsOpen ();
	}

	uint64_t
	GetThreadId ()
	{
		return m_Thread.GetThreadId ();
	}

	bool
	Start ()
	{
		return m_Thread.Create (NULL, 0, ThreadProc, (T*) this, 0);
	}

	void
	WaitAndClose (uint_t Timeout = -1)
	{
		if (!m_Thread.IsOpen ())
			return;

		win::EWaitResult Result = m_Thread.Wait (Timeout);
		if (Result != win::EWaitResult_Object0)
		{
			ASSERT (false); // terminating thread
			m_Thread.Terminate (err::EStatus_IoTimeout);
		}

		m_Thread.Close ();
	}


protected:
	static
	dword_t
	WINAPI
	ThreadProc (void* pContext)
	{
		((T*) pContext)->ThreadProc ();
		return 0;
	}
};

//.............................................................................

inline
uint64_t
GetCurrentThreadId ()
{
	return ::GetCurrentThreadId ();
}

//.............................................................................

#elif (_AXL_ENV == AXL_ENV_POSIX)

template <typename T>
class CThreadImplT
{
public:
	psx::CThread m_Thread;

public:
	~CThreadImplT ()
	{
		WaitAndClose ();
	}

	bool
	IsOpen ()
	{
		return m_Thread.IsOpen ();
	}

	uint64_t
	GetThreadId ()
	{
		return m_Thread;
	}

	bool
	Start ()
	{
		return m_Thread.Create (ThreadProc, (T*) this);
	}

	void
	WaitAndClose (uint_t Timeout = -1)
	{
		if (!m_Thread.IsOpen ())
			return;

		if (!m_Thread.Join (Timeout))
			m_Thread.Detach ();
	}


protected:
	static
	void*
	ThreadProc (void* pContext)
	{
		((T*) pContext)->ThreadProc ();
		return NULL;
	}
};

inline
uint64_t
GetCurrentThreadId ()
{
	return pthread_self ();
}

#endif

//.............................................................................

} // namespace mt
} // namespace axl

