#include "pch.h"
#include "axl_exe_WorkerThread.h"
#include "axl_exe_WorkerThreadPool.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace exe {

//.............................................................................

CWorkerThread::CWorkerThread ()
{
	m_terminateFlag = false;

	m_waitArray.setCount (1);
	m_waitArray [0] = &m_event;
}

bool 
CWorkerThread::start ()
{
	ASSERT (!isOpen ());

	m_terminateFlag = false;
	return CThreadImplT <CWorkerThread>::start ();
}

void 
CWorkerThread::stop (
	bool doWaitAndClose,
	ulong_t timeout
	)
{
	signalStop ();
	
	if (doWaitAndClose)
		waitAndClose (timeout);
}

void 
CWorkerThread::signalStop ()
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
CWorkerThread::addEvent (
	mt::CEvent* event, 
	exe::IFunction* onEvent
	)
{
	ASSERT (isOpen ());

	return (handle_t) syncSchedule <exe::CArgSeqT_3 <
		CWorkerThread*,
		mt::CEvent*, 
		exe::IFunction*
		> > (pvoid_cast (&CWorkerThread::addEvent_wt), this, event, onEvent);
}

void 
CWorkerThread::removeEvent (handle_t hEvent)
{
	ASSERT (isOpen ());

	syncSchedule <exe::CArgSeqT_2 <
		CWorkerThread*,
		mt::CEvent*
		> > (pvoid_cast (&CWorkerThread::removeEvent_wt), this, hEvent);
}

inline
bool 
CWorkerThread::canInvokeNow ()
{
#if (_AXL_ENV == AXL_ENV_WIN)
	return getThreadId () == mt::getCurrentThreadId ();
#elif (_AXL_ENV == AXL_ENV_NT)
	return getThreadId () == mt::getCurrentThreadId () && keGetCurrentIrql () == PASSIVE_LEVEL;
#endif
}

CWorkerThread::EScheduleResult
CWorkerThread::scheduleV (
	exe::IFunction* function, 
	axl_va_list va
	)
{
	ASSERT (isOpen ());

	if (canInvokeNow ())
	{
		function->invokeV (va);
		return EScheduleResult_Invoke;
	}

	m_lock.lock ();
	m_invokeList.addV (function, va);
	m_event.signal ();
	m_lock.unlock ();

	return EScheduleResult_Pending;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// everything below runs in worker thread

ulong_t 
CWorkerThread::threadProc ()
{
	bool terminateFlag = false;

	HANDLE waitArray [MAXIMUM_WAIT_OBJECTS];

	do
	{
		size_t count = m_waitArray.getCount ();
		ASSERT (count > 0 && count <= MAXIMUM_WAIT_OBJECTS);

		for (size_t i = 0; i < count; i++)
			waitArray [i] = *m_waitArray [i];

		ulong_t result = win::CWaitableHandle::multiWait (waitArray, count, false, -1, true);
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

CWorkerThread::TUserEvent*
CWorkerThread::addEvent_wt (
	mt::CEvent* event,
	exe::IFunction* onEvent
	)
{
	size_t count = m_waitArray.getCount ();
	if (count >= MAXIMUM_WAIT_OBJECTS)
		return NULL;

	TUserEvent* userEvent = AXL_MEM_NEW (TUserEvent);
	userEvent->m_event = event;
	userEvent->m_onEvent = ref::clone (onEvent);
	m_userEventList.insertTail (userEvent);

	m_waitArray.append (event);
	m_functionArray.append (userEvent->m_onEvent);

	return userEvent;
}

void
CWorkerThread::removeEvent_wt (TUserEvent* userEvent)
{
	m_userEventList.delete (userEvent);

	size_t count = m_userEventList.getCount ();

	m_waitArray.setCount (count + 1);
	m_functionArray.setCount (count);

	rtl::CIteratorT <TUserEvent> it = m_userEventList.getHead ();
	mt::CEvent** event = m_waitArray.getBuffer () + 1;
	exe::IFunction** onEvent = m_functionArray.getBuffer ();

	for (; it; it++, event++, onEvent++)
	{
		*event = it->m_event;
		*onEvent = it->m_onEvent;
	}
}

bool
CWorkerThread::process_wt ()
{
	exe::CInvokeList invokeList;

	m_lock.lock ();
	invokeList.takeOver (&m_invokeList);
	bool terminateFlag = m_terminateFlag;
	m_lock.unlock ();

	invokeList.process ();
	return terminateFlag;
}

//.............................................................................

ref::CPtrT <CWorkerThread>
getWorkerThread (size_t reserveEventCount)
{
	exe::CWorkerThreadPool* pool = rtl::getSingleton <exe::CWorkerThreadPool> ();
	return pool->getThread (reserveEventCount);
}

ref::CPtrT <CWorkerThread>
getWorkerThread (
	mt::CEvent* event,
	exe::IFunction* onEvent,
	handle_t* phEvent
	)
{
	exe::CWorkerThreadPool* pool = rtl::getSingleton <exe::CWorkerThreadPool> ();
	ref::CPtrT <CWorkerThread> thread = pool->getThread (1);
	if (!thread)
		return ref::EPtr_Null;

	handle_t hEvent = thread->addEvent (event, onEvent);
	if (!hEvent)
		return ref::EPtr_Null;

	*phEvent = hEvent;
	return thread;
}

//.............................................................................

} // namespace exe
} // namespace axl

