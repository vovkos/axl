// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_THREAD_H

#include "axl_mt_win_WaitableHandle.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

class CThread: public CWaitableHandle
{
public:
	bool 
	Create (
		SECURITY_ATTRIBUTES* pSecAttr,
		size_t StackSize,
		PTHREAD_START_ROUTINE pfThreadProc,
		void* pContext,
		uint_t Flags
		);

	uint_t
	GetThreadId ();

	bool 
	SetPriority (int Priority);

	bool 
	Terminate (uint_t ExitCode);

	uint_t
	GetExitCode ();

	static
	HANDLE
	GetCurrentThread ()
	{
		return ::GetCurrentThread ();
	}

	static
	uint_t 
	GetCurrentThreadId ()
	{
		return ::GetCurrentThreadId ();
	}
};

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
