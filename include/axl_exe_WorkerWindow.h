// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_EXE_WORKERWINDOW_H

#include "axl_exe_Scheduler.h"
#include "axl_exe_InvokeList.h"
#include "axl_g_win_Window.h"

namespace axl {
namespace exe {

//.............................................................................

class WorkerWindow: 
	public g::win::WindowImpl <WorkerWindow>,
	public exe::IScheduler
{
public:
	AXL_OBJ_CLASS_0 (WorkerWindow, exe::IScheduler)

protected:
	enum
	{
		WM_PROCESS_INVOKE_LIST = WM_USER + 1,
	};

	mt::Lock m_lock;
	exe::InvokeList m_invokeList;
	ulong_t m_threadId;

public:
	WorkerWindow ()
	{
		m_threadId = 0;
	}

	bool 
	create ()
	{
		m_threadId = ::GetCurrentThreadId ();
		return WindowImpl <WorkerWindow>::create (NULL);
	}

	// IScheduler

	virtual 
	ScheduleResultKind
	scheduleV (
		exe::IFunction* function, 
		axl_va_list va
		);

	// CWindowImpl

	AXL_G_WINDOW_MSG_MAP_BEGIN ()
		AXL_G_WINDOW_MSG_HANDLER (WM_PROCESS_INVOKE_LIST, onProcessInvokeList)
	AXL_G_WINDOW_MSG_MAP_END ()

	LRESULT 
	onProcessInvokeList (
		UINT msg, 
		WPARAM wParam, 
		LPARAM lParam, 
		bool* isHandled
		);
};

//.............................................................................

} // namespace exe
} // namespace axl

