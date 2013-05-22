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
		axl_va_list va
		);

	// CWindowImpl

	AXL_G_WINDOW_MSG_MAP_BEGIN ()
		AXL_G_WINDOW_MSG_HANDLER (WM_PROCESS_INVOKE_LIST, OnProcessInvokeList)
	AXL_G_WINDOW_MSG_MAP_END ()

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

