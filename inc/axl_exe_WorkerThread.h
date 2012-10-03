// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYSX_WORKERTHREAD_H

#include "axl_exe_Scheduler.h"
#include "axl_exe_InvokeList.h"
#include "axl_rtl_Array.h"
#include "axl_mt_Lock.h"
#include "axl_mt_Event.h"
#include "axl_mt_Thread.h"

namespace axl {
namespace exe {

//.............................................................................

class CWorkerThread: 
	public mt::CThreadImplT <CWorkerThread>,
	public exe::IScheduler
{ 
public:
	AXL_OBJ_SIMPLE_CLASS (CWorkerThread, exe::IScheduler)

protected:
	struct TUserEvent: rtl::TListLink
	{
		mt::CEvent* m_pEvent;
		ref::CPtrT <exe::IFunction> m_OnEvent;
	};

protected:
	mt::CLock m_Lock;
	mt::CEvent m_Event; 
	exe::CInvokeList m_InvokeList;
	bool m_TerminateFlag;

	// these are touched only within the worker thread

	rtl::CStdListT <TUserEvent> m_UserEventList;
	rtl::CArrayT <mt::CEvent*> m_WaitArray;
	rtl::CArrayT <exe::IFunction*> m_FunctionArray;

public:
	CWorkerThread ();
	
	~CWorkerThread ()
	{
		Stop ();
	}

	bool 
	Start ();

	void
	Stop (
		bool DoWaitAndClose = true,
		ulong_t Timeout = -1
		);

	handle_t
	AddEvent (
		mt::CEvent* pEvent, 
		exe::IFunction* pOnEvent
		);

	void 
	RemoveEvent (handle_t hEvent);

	// IScheduler

	virtual 
	EScheduleResult
	ScheduleV (
		exe::IFunction* pFunction, 
		va_list va
		);

	// CThreadImpl

	ulong_t 
	ThreadProc ();

protected:
	TUserEvent*
	AXL_CDECL
	AddEvent_wt (
		mt::CEvent* pEvent,
		exe::IFunction* pOnEvent
		);

	void
	AXL_CDECL
	RemoveEvent_wt (TUserEvent* pUserEvent);

	bool
	Process_wt ();

	bool 
	CWorkerThread::CanInvokeNow ();

	void
	SignalStop ();
};

//.............................................................................

ref::CPtrT <CWorkerThread>
GetWorkerThread (size_t ReserveEventCount = 0);

ref::CPtrT <CWorkerThread>
GetWorkerThread (
	mt::CEvent* pEvent,
	exe::IFunction* pOnEvent,
	handle_t* phEvent
	);

//.............................................................................


} // namespace exe
} // namespace axl
