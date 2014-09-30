// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_EXE_WORKERTHREAD_H

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
	AXL_OBJ_CLASS_0 (CWorkerThread, exe::IScheduler)

protected:
	struct TUserEvent: rtl::TListLink
	{
		mt::CEvent* m_event;
		ref::CPtrT <exe::IFunction> m_onEvent;
	};

protected:
	mt::CLock m_lock;
	mt::CEvent m_event; 
	exe::CInvokeList m_invokeList;
	bool m_terminateFlag;

	// these are touched only within the worker thread

	rtl::CStdListT <TUserEvent> m_userEventList;
	rtl::CArrayT <mt::CEvent*> m_waitArray;
	rtl::CArrayT <exe::IFunction*> m_functionArray;

public:
	CWorkerThread ();
	
	~CWorkerThread ()
	{
		stop ();
	}

	bool 
	start ();

	void
	stop (
		bool doWaitAndClose = true,
		ulong_t timeout = -1
		);

	handle_t
	addEvent (
		mt::CEvent* event, 
		exe::IFunction* onEvent
		);

	void 
	removeEvent (handle_t hEvent);

	// IScheduler

	virtual 
	EScheduleResult
	scheduleV (
		exe::IFunction* function, 
		axl_va_list va
		);

	// CThreadImpl

	ulong_t 
	threadProc ();

protected:
	TUserEvent*
	AXL_CDECL
	addEvent_wt (
		mt::CEvent* event,
		exe::IFunction* onEvent
		);

	void
	AXL_CDECL
	removeEvent_wt (TUserEvent* userEvent);

	bool
	process_wt ();

	bool 
	CWorkerThread::canInvokeNow ();

	void
	signalStop ();
};

//.............................................................................

ref::CPtrT <CWorkerThread>
getWorkerThread (size_t reserveEventCount = 0);

ref::CPtrT <CWorkerThread>
getWorkerThread (
	mt::CEvent* event,
	exe::IFunction* onEvent,
	handle_t* phEvent
	);

//.............................................................................


} // namespace exe
} // namespace axl
