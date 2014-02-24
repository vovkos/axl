// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_THREAD_H

#include "axl_mt_win_WaitableHandle.h"
#include "axl_err_Error.h"

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
	GetThreadId ()
	{
		uint_t Id = ::GetThreadId (m_h);
		return err::Complete (Id, (uint_t) -1);
	}

	bool 
	SetPriority (int Priority)
	{
		bool_t Result = ::SetThreadPriority (m_h, Priority);
		return err::Complete (Result);
	}

	bool 
	Terminate (dword_t ExitCode)
	{
		bool_t Result = ::TerminateThread (m_h, ExitCode);
		return err::Complete (Result);
	}

	bool
	GetExitCode (dword_t* pExitCode)
	{
		bool_t Result = ::GetExitCodeThread (m_h, pExitCode);
		return err::Complete (Result);
	}
};

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
