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
class ThreadImpl
{
public:
	win::Thread m_thread;

public:
	~ThreadImpl ()
	{
		waitAndClose ();
	}

	bool
	isOpen ()
	{
		return m_thread.isOpen ();
	}

	uint64_t
	getThreadId ()
	{
		return m_thread.getThreadId ();
	}

	bool
	start ()
	{
		return m_thread.create (NULL, 0, threadProc, (T*) this, 0);
	}

	void
	waitAndClose (uint_t timeout = -1)
	{
		if (!m_thread.isOpen ())
			return;

		win::WaitResult result = m_thread.wait (timeout);
		if (result != win::WaitResult_Object0)
		{
			ASSERT (false); // terminating thread
			m_thread.terminate (err::SystemErrorCode_IoTimeout);
		}

		m_thread.close ();
	}


protected:
	static
	dword_t
	WINAPI
	threadProc (void* context)
	{
		((T*) context)->threadProc ();
		return 0;
	}
};

//.............................................................................

inline
uint64_t
getCurrentThreadId ()
{
	return ::GetCurrentThreadId ();
}

//.............................................................................

#elif (_AXL_ENV == AXL_ENV_POSIX)

template <typename T>
class ThreadImpl
{
public:
	psx::Thread m_thread;

public:
	~ThreadImpl ()
	{
		waitAndClose ();
	}

	bool
	isOpen ()
	{
		return m_thread.isOpen ();
	}

	uint64_t
	getThreadId ()
	{
		return m_thread;
	}

	bool
	start ()
	{
		return m_thread.create (threadProc, (T*) this);
	}

	void
	waitAndClose (uint_t timeout = -1)
	{
		if (!m_thread.isOpen ())
			return;

		if (!m_thread.join (timeout))
			m_thread.detach ();
	}


protected:
	static
	void*
	threadProc (void* context)
	{
		((T*) context)->threadProc ();
		return NULL;
	}
};

inline
uint64_t
getCurrentThreadId ()
{
	return ::pthread_self ();
}

#endif

//.............................................................................

} // namespace mt
} // namespace axl

