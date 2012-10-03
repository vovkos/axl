// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_THREAD_H

#include "axl_win_WaitableHandle.h"

namespace axl {
namespace win {

//.............................................................................

class CThread: public CWaitableHandle
{
public:
	bool 
	Create (
		SECURITY_ATTRIBUTES* pSecAttr,
		ulong_t StackSize,
		LPTHREAD_START_ROUTINE pfnThreadProc,
		void* pContext,
		ulong_t Flags
		);

	ulong_t
	GetThreadId ()
	{
		ASSERT (IsOpen ());
		return ::GetThreadId (m_h);
	}

	bool 
	SetPriority (int Priority);

	bool 
	Terminate (ulong_t ExitCode);

	ulong_t
	GetExitCode ();

	static
	handle_t
	GetCurrentThread ()
	{
		return ::GetCurrentThread ();
	}

	static
	ulong_t 
	GetCurrentThreadId ()
	{
		return ::GetCurrentThreadId ();
	}
};

//.............................................................................

} // namespace win
} // namespace axl
