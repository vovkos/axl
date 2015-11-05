// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_THREAD_H

#include "axl_mt_win_WaitableHandle.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

class Thread: public WaitableHandle
{
public:
	bool 
	create (
		SECURITY_ATTRIBUTES* secAttr,
		size_t stackSize,
		PTHREAD_START_ROUTINE threadFunc,
		void* context,
		uint_t flags
		);

	uint_t
	getThreadId ()
	{
		uint_t id = ::GetThreadId (m_h);
		return err::complete (id, (uint_t) -1);
	}

	bool 
	setPriority (int priority)
	{
		bool_t result = ::SetThreadPriority (m_h, priority);
		return err::complete (result);
	}

	bool 
	terminate (dword_t exitCode)
	{
		bool_t result = ::TerminateThread (m_h, exitCode);
		return err::complete (result);
	}

	bool
	getExitCode (dword_t* exitCode)
	{
		bool_t result = ::GetExitCodeThread (m_h, exitCode);
		return err::complete (result);
	}
};

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
