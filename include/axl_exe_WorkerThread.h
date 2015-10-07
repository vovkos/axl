// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_EXE_WORKERTHREAD_H

#include "axl_exe_Scheduler.h"
#include "axl_exe_InvokeList.h"
#include "axl_sl_Array.h"
#include "axl_mt_Lock.h"
#include "axl_mt_Event.h"
#include "axl_mt_Thread.h"

namespace axl {
namespace exe {

//.............................................................................

class Workerhread: 
	public mt::ThreadImpl <Workerhread>,
	public exe::IScheduler
{ 
public:
	AXL_OBJ_CLASS_0 (Workerhread, exe::IScheduler)

protected:
	struct UserEvent: sl::ListLink
	{
		mt::Event* m_event;
		ref::Ptr <exe::IFunction> m_onEvent;
	};

protected:
	mt::Lock m_lock;
	mt::Event m_event; 
	exe::InvokeList m_invokeList;
	bool m_terminateFlag;

	// these are touched only within the worker thread

	sl::StdList <UserEvent> m_userEventList;
	sl::Array <mt::Event*> m_waitArray;
	sl::Array <exe::IFunction*> m_functionArray;

public:
	Workerhread ();
	
	~Workerhread ()
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
		mt::Event* event, 
		exe::IFunction* onEvent
		);

	void 
	removeEvent (handle_t hEvent);

	// IScheduler

	virtual 
	ScheduleResultKind
	scheduleV (
		exe::IFunction* function, 
		axl_va_list va
		);

	// CThreadImpl

	ulong_t 
	threadProc ();

protected:
	UserEvent*
	AXL_CDECL
	addEvent_wt (
		mt::Event* event,
		exe::IFunction* onEvent
		);

	void
	AXL_CDECL
	removeEvent_wt (UserEvent* userEvent);

	bool
	process_wt ();

	bool 
	Workerhread::canInvokeNow ();

	void
	signalStop ();
};

//.............................................................................

ref::Ptr <Workerhread>
getWorkerThread (size_t reserveEventCount = 0);

ref::Ptr <Workerhread>
getWorkerThread (
	mt::Event* event,
	exe::IFunction* onEvent,
	handle_t* phEvent
	);

//.............................................................................


} // namespace exe
} // namespace axl
