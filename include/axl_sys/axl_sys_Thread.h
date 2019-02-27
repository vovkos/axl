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

#define _AXL_SYS_THREAD_H

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Thread.h"
#elif (_AXL_OS_POSIX)
#	include "axl_sys_psx_Thread.h"
#include "axl_sys_Event.h"
#endif

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

template <typename T>
class ThreadImpl
{
public:
	win::Thread m_thread;

public:
	~ThreadImpl()
	{
		waitAndClose();
	}

	bool
	isOpen()
	{
		return m_thread.isOpen();
	}

	uint64_t
	getThreadId()
	{
		return m_thread.getThreadId();
	}

	bool
	start()
	{
		return m_thread.create(NULL, 0, threadFuncImpl, (T*)this, 0);
	}

	bool
	wait(uint_t timeout = -1)
	{
		return !m_thread.isOpen() || m_thread.wait(timeout) == win::WaitResult_Object0;
	}

	void
	waitAndClose(uint_t timeout = -1)
	{
		bool result = wait(timeout);
		if (!result)
		{
			ASSERT(false); // terminating thread
			m_thread.terminate(err::SystemErrorCode_IoTimeout);
		}

		m_thread.close();
	}

	bool
	terminate()
	{
		return m_thread.terminate(err::SystemErrorCode_IoTimeout);
	}

protected:
	static
	dword_t
	WINAPI
	threadFuncImpl(void* context)
	{
		((T*)context)->threadFunc();
		return 0;
	}
};

//..............................................................................

inline
uint64_t
getCurrentThreadId()
{
	return ::GetCurrentThreadId();
}

//..............................................................................

#elif (_AXL_OS_POSIX)

class ThreadImplRoot
{
public:
	psx::Thread m_thread;

public:
	bool
	isOpen()
	{
		return m_thread.isOpen();
	}

	uint64_t
	getThreadId()
	{
		return (uint64_t)(pthread_t)m_thread;
	}

	bool
	terminate()
	{
		return m_thread.cancel();
	}
};

//..............................................................................

#if (_AXL_OS_DARWIN)

template <typename T>
class ThreadImpl: public ThreadImplRoot
{
public:
	NotificationEvent m_threadCompletedEvent;

public:
	~ThreadImpl()
	{
		waitAndClose();
	}

	bool
	start()
	{
		ASSERT(!m_thread.isOpen());

		m_threadCompletedEvent.reset();
		return m_thread.create(threadFuncImpl, (T*)this);
	}

	bool
	wait(uint_t timeout = -1)
	{
		return !m_thread.isOpen() || m_threadCompletedEvent.wait(timeout);
	}

	void
	waitAndClose(uint_t timeout = -1)
	{
		if (!m_thread.isOpen())
			return;

		bool result = m_threadCompletedEvent.wait(timeout);
		if (result)
		{
			m_thread.join();
		}
		else
		{
			ASSERT(false); // cancelling thread
			m_thread.cancel();
		}

		m_thread.detach();
	}

protected:
	static
	void*
	threadFuncImpl(void* context)
	{
		T* self = (T*)context;
		self->threadFunc();
		self->m_threadCompletedEvent.signal();
		return NULL;
	}
};

#else

//..............................................................................

template <typename T>
class ThreadImpl: public ThreadImplRoot
{
public:
	~ThreadImpl()
	{
		waitAndClose();
	}

	bool
	start()
	{
		ASSERT(!m_thread.isOpen());
		return m_thread.create(threadFunc, (T*)this);
	}

	bool
	wait(uint_t timeout = -1)
	{
		return !m_thread.isOpen() || m_thread.join(timeout);
	}

	void
	waitAndClose(uint_t timeout = -1)
	{
		bool result = wait(timeout);
		if (!result)
		{
			ASSERT(false);
			m_thread.cancel();
		}

		m_thread.detach();
	}

protected:
	static
	void*
	threadFunc(void* context)
	{
		((T*)context)->threadFunc();
		return NULL;
	}
};

//..............................................................................

#endif

inline
uint64_t
getCurrentThreadId()
{
	return (uint64_t) ::pthread_self();
}

#endif

//..............................................................................

} // namespace sys
} // namespace axl
