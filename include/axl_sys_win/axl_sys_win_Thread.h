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

#define _AXL_SYS_WIN_THREAD_H

#include "axl_sys_win_WaitableHandle.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class Thread: public WaitableHandle
{
protected:
	uint_t m_threadId;

public:
	Thread()
	{
		m_threadId = -1;
	}

	uint_t
	getThreadId()
	{
		return m_threadId;
	}

	void
	close()
	{
		WaitableHandle::close();
		m_threadId = -1;
	}

	bool
	create(
		SECURITY_ATTRIBUTES* secAttr,
		size_t stackSize,
		PTHREAD_START_ROUTINE threadFunc,
		void* context,
		uint_t flags
		);

	bool
	setPriority(int priority)
	{
		bool_t result = ::SetThreadPriority(m_h, priority);
		return err::complete(result);
	}

	bool
	terminate(dword_t exitCode)
	{
		bool_t result = ::TerminateThread(m_h, exitCode);
		return err::complete(result);
	}

	bool
	getExitCode(dword_t* exitCode)
	{
		bool_t result = ::GetExitCodeThread(m_h, exitCode);
		return err::complete(result);
	}
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
