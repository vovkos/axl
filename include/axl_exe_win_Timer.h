// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_TIMER_H

#include "axl_exe_Function.h"
#include "axl_win_WaitableHandle.h"

namespace axl {
namespace win {

//.............................................................................

class Timer: public WaitableHandle
{
public:
	bool
	create (
		SECURITY_ATTRIBUTES* secAttr,
		bool isManualReset,
		const tchar_t* name
		);

	bool
	open (
		ulong_t access,
		bool doInheritHandle,
		const tchar_t* name
		);

	bool 
	setTimer (	
		uint64_t _DueTime,
		ulong_t period, 
		PTIMERAPCROUTINE pfTimerProc,
		void* context
		);

	void
	cancel ()
	{
		ASSERT (isOpen ());
		::CancelWaitableTimer (m_h);
	}

	static
	ulong_t 
	getTickCount ()
	{
		return ::GetTickCount ();
	}
};

//.............................................................................

} // namespace win
} // namespace axl
