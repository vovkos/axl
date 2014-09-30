#include "pch.h"
#include "axl_exe_WorkerThread.h"
#include "axl_exe_WorkerThreadPool.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace exe {

//.............................................................................

Workerhread::Workerhread ()
{
	m_terminateFlag = false;

	m_waitArray.setCount (1);
	m_waitArray [0] = &m_event;
}

bool 
Workerhread::start ()
{
	ASSERT (!isOpen ());

	m_terminateFlag = false;
	return ThreadImpl <Workerhread>::start ();
}

void 
Workerhread::stop (
	bool doWaitAndClose,
	ulong_t timeout
	)
{
	signalStop ();
	
	if (doWaitAndClose)
		waitAndClose (timeout);
}

void 
Workerhread::signalStop ()
{
	if (m_terminateFlag)
		return;

	m_lock.lock ();
	m_terminateFlag = true;
	m_event.signal ();
	m_lock.unlock ();
}

// AddEvent / RemoveEvent should only be called once the thread is running

handle_t
Workerhread::addEvent (
	mt::Event* event, 
	exe::IFunction* onEvent
	)
{
	ASSERT (isOpen ());

	return (handle_t) syncSchedule <exe::ArgSeq_3 <
		Workerhread*,
		mt::Event*, 
		exe::IFunction*
		> > (pvoid_cast (&Workerhread::addEvent_wt), this, event, onEvent);
}

void 
Workerhread::removeEvent (handle_t hEvent)
{
	ASSERT (isOpen ());

	syncSchedule <exe::ArgSeq_2 <
		Workerhread*,
		mt::Event*
		> > (pvoid_cast (&Workerhread::removeEvent_wt), this, hEvent);
}

inline
bool 
Workerhread::canInvokeNow ()
{
#if (_AXL_ENV == AXL_ENV_WIN)
	return getThreadId () == mt::getCurrentThreadId ();
#elif (_AXL_ENV == AXL_ENV_NT)
	return getThreadId () == mt::getCurrentThreadId () && keGetCurrentIrql () == PASSIVE_LEVEL;
#endif
}

Workerhread::ScheduleResultKind
Workerhread::scheduleV (
	exe::IFunction* function, 
	axl_va_list va
	)
{
	ASSERT (isOpen ());

	if (canInvokeNow ())
	{
		function->invokeV (va);
		return ScheduleResultKind_Invoke;
	}

	m_lock.lock ();
	m_invokeList.addV (function, va);
	m_event.signal ();
	m_lock.unlock ();

	return ScheduleResultKind_Pending;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// everything below runs in worker thread

ulong_t 
Workerhread::threadProc ()
{
	bool terminateFlag = false;

	HANDLE waitArray [MAXIMUM_WAIT_OBJECTS];

	do
	{
		size_t count = m_waitArray.getCount ();
		ASSERT (count > 0 && count <= MAXIMUM_WAIT_OBJECTS);

		for (size_t i = 0; i < count; i++)
			waitArray [i] = *m_waitArray [i];

		ulong_t result = win::WaitableHandle::multiWait (waitArray, count, false, -1, true);
		switch (result)
		{
		case 0: 
			terminateFlag = process_wt ();
			break;

		default:
			if (result >= 1 && (size_t) result < count)
			{
				size_t index = result - 1;
				ASSERT (index < m_functionArray.getCount ());
				m_functionArray [index]->invoke (0);
			}
		}
	} while (!terminateFlag);

	return 0;
}

Workerhread::UserEvent*
Workerhread::addEvent_wt (
	mt::Event* event,
	exe::IFunction* onEvent
	)
{
	size_t count = m_waitArray.getCount ();
	if (count >= MAXIMUM_WAIT_OBJECTS)
		return NULL;

	UserEvent* userEvent = AXL_MEM_NEW (UserEvent);
	userEvent->m_event = event;
	userEvent->m_onEvent = ref::clone (onEvent);
	m_userEventList.insertTail (userEvent);

	m_waitArray.append (event);
	m_functionArray.append (userEvent->m_onEvent);

	return userEvent;
}

void
Workerhread::removeEvent_wt (UserEvent* userEvent)
{
	m_userEventList.delete (userEvent);

	size_t count = m_userEventList.getCount ();

	m_waitArray.setCount (count + 1);
	m_functionArray.setCount (count);

	rtl::Iterator <UserEvent> it = m_userEventList.getHead ();
	mt::Event** event = m_waitArray.getBuffer () + 1;
	exe::IFunction** onEvent = m_functionArray.getBuffer ();

	for (; it; it++, event++, onEvent++)
	{
		*event = it->m_event;
		*onEvent = it->m_onEvent;
	}
}

bool
Workerhread::process_wt ()
{
	exe::InvokeList invokeList;

	m_lock.lock ();
	invokeList.takeOver (&m_invokeList);
	bool terminateFlag = m_terminateFlag;
	m_lock.unlock ();

	invokeList.process ();
	return terminateFlag;
}

//.............................................................................

ref::Ptr <Workerhread>
getWorkerThread (size_t reserveEventCount)
{
	exe::WorkerhreadPool* pool = rtl::getSingleton <exe::WorkerhreadPool> ();
	return pool->getThread (reserveEventCount);
}

ref::Ptr <Workerhread>
getWorkerThread (
	mt::Event* event,
	exe::IFunction* onEvent,
	handle_t* phEvent
	)
{
	exe::WorkerhreadPool* pool = rtl::getSingleton <exe::WorkerhreadPool> ();
	ref::Ptr <Workerhread> thread = pool->getThread (1);
	if (!thread)
		return ref::PtrKind_Null;

	handle_t hEvent = thread->addEvent (event, onEvent);
	if (!hEvent)
		return ref::PtrKind_Null;

	*phEvent = hEvent;
	return thread;
}

//.............................................................................

} // namespace exe
} // namespace axl

