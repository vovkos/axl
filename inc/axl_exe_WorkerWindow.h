// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYSX_WORKERWINDOW_H

#include "axl_exe_Scheduler.h"
#include "axl_exe_InvokeList.h"
#include "axl_win_Window.h"

namespace axl {
namespace exe {

//.............................................................................

class CWorkerWindow: 
	public win::CWindowImplT <CWorkerWindow>,
	public exe::IScheduler
{
public:
	AXL_OBJ_SIMPLE_CLASS (CWorkerWindow, exe::IScheduler)

protected:
	enum
	{
		WM_PROCESS_INVOKE_LIST = WM_USER + 1,
	};

	mt::CLock m_Lock;
	exe::CInvokeList m_InvokeList;
	ulong_t m_ThreadId;

public:
	CWorkerWindow ()
	{
		m_ThreadId = 0;
	}

	bool 
	Create ()
	{
		m_ThreadId = ::GetCurrentThreadId ();
		return CWindowImplT <CWorkerWindow>::Create (NULL);
	}

	// IScheduler

	virtual 
	EScheduleResult
	ScheduleV (
		exe::IFunction* pFunction, 
		va_list va
		);

	// CWindowImpl

	AXL_WINDOW_MESSAGE_MAP_BEGIN ()
		AXL_WINDOW_MESSAGE_HANDLER (WM_PROCESS_INVOKE_LIST, OnProcessInvokeList)
	AXL_WINDOW_MESSAGE_MAP_END ()

	LRESULT 
	OnProcessInvokeList (
		UINT Msg, 
		WPARAM wParam, 
		LPARAM lParam, 
		bool* pIsHandled
		);
};

//.............................................................................

} // namespace exe
} // namespace axl

