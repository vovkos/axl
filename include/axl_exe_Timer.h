// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_TIMER_H

#include "axl_exe_WorkerThread.h"
#include "axl_win_Timer.h"

namespace axl {
namespace mt {

//.............................................................................

class Timer
{
public:
	enum TimerKind
	{
		TimerKind_ManualReset = 1,
		TimerKind_Periodic    = 2
	};

protected:
	win::Timer m_timer;
	ref::Ptr <exe::IFunction> m_onTimer;
	ref::Ptr <exe::Workerhread> m_workerThread;

public:
	~Timer ()
	{
		cancel ();
	}

	bool 
	setTimer (	
		ulong_t timeout,
		exe::IFunction* onTimer,
		int flags
		);

	void
	cancel ();

protected:
	bool 
	setTimer_wt (	
		ulong_t timeout,
		exe::IFunction* onTimer,
		int flags
		);

	void
	cancel_wt ();

	static
	VOID 
	CALLBACK
	timerProc (
		LPVOID context,
		DWORD timerLo,
		DWORD timerHi
		);
};

//.............................................................................

inline
ulong_t
getCurrentTime ()
{
	return win::Timer::getTickCount ();
}

//.............................................................................

} // namespace mt
} // namespace axl
