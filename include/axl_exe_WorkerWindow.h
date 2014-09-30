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

class CWorkerWindow: 
	public g::win::CWindowImplT <CWorkerWindow>,
	public exe::IScheduler
{
public:
	AXL_OBJ_CLASS_0 (CWorkerWindow, exe::IScheduler)

protected:
	enum
	{
		WM_PROCESS_INVOKE_LIST = WM_USER + 1,
	};

	mt::CLock m_lock;
	exe::CInvokeList m_invokeList;
	ulong_t m_threadId;

public:
	CWorkerWindow ()
	{
		m_threadId = 0;
	}

	bool 
	create ()
	{
		m_threadId = ::GetCurrentThreadId ();
		return CWindowImplT <CWorkerWindow>::create (NULL);
	}

	// IScheduler

	virtual 
	EScheduleResult
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

