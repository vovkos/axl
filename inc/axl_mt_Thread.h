// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_THREAD_H

#include "axl_win_Thread.h"

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

	uintptr_t 
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
	Terminate (ulong_t ExitCode)
	{
		ASSERT (IsOpen ());
		return m_Thread.Terminate (ExitCode);
	}

	void 
	WaitAndClose (ulong_t Timeout = -1)
	{
		if (!m_Thread.IsOpen ())
			return;

		win::CWaitableHandle::EWaitResult Result = m_Thread.Wait (Timeout);
		if (Result != win::CWaitableHandle::EWaitResult_Object0)
		{
			ASSERT (false); // terminating thread
			Terminate (err::EStatus_IoTimeout);
		}

		m_Thread.Close ();
	}	


protected:
	static 
	DWORD
	WINAPI
	ThreadProc (LPVOID pContext)
	{
		return ((T*) pContext)->ThreadProc ();
	}
};

//.............................................................................

inline
uintptr_t 
GetCurrentThreadId ()
{
	return win::CThread::GetCurrentThreadId ();
}

//.............................................................................

} // namespace mt
} // namespace axl

