// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_THREAD_H

#include "axl_mt_win_Thread.h"

namespace axl {
namespace mt {

//.............................................................................

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

	operator HANDLE ()
	{ 
		return m_Thread;
	}

	bool
	IsOpen ()
	{
		return m_Thread.IsOpen ();
	}

	uint_t 
	GetThreadId ()
	{
		return m_Thread.GetThreadId ();
	}

	bool 
	Start ()
	{
		return m_Thread.Create (NULL, 0, ThreadProc, (T*) this, 0);
	}

	bool
	Terminate (int ExitCode)
	{
		return m_Thread.Terminate (ExitCode);
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
			Terminate (err::EStatus_IoTimeout);
		}

		m_Thread.Close ();
	}	


protected:
	static 
	dword_t
	WINAPI
	ThreadProc (void* pContext)
	{
		return ((T*) pContext)->ThreadProc ();
	}
};

//.............................................................................

inline
uint_t 
GetCurrentThreadId ()
{
	return win::CThread::GetCurrentThreadId ();
}

//.............................................................................

} // namespace mt
} // namespace axl

