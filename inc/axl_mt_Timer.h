// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_TIMER_H

#include "axl_exe_WorkerThread.h"
#include "axl_win_Timer.h"

namespace axl {
namespace mt {

//.............................................................................

class CTimer
{
public:
	enum ETimer
	{
		ETimer_ManualReset = 1,
		ETimer_Periodic    = 2
	};

protected:
	win::CTimer m_Timer;
	ref::CPtrT <exe::IFunction> m_OnTimer;
	ref::CPtrT <exe::CWorkerThread> m_WorkerThread;

public:
	~CTimer ()
	{
		Cancel ();
	}

	bool 
	SetTimer (	
		ulong_t Timeout,
		exe::IFunction* pOnTimer,
		int Flags
		);

	void
	Cancel ();

protected:
	bool 
	SetTimer_wt (	
		ulong_t Timeout,
		exe::IFunction* pOnTimer,
		int Flags
		);

	void
	Cancel_wt ();

	static
	VOID 
	CALLBACK
	TimerProc (
		LPVOID pContext,
		DWORD TimerLo,
		DWORD TimerHi
		);
};

//.............................................................................

inline
ulong_t
GetCurrentTime ()
{
	return win::CTimer::GetTickCount ();
}

//.............................................................................

} // namespace mt
} // namespace axl
