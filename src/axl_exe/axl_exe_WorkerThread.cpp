#include "pch.h"
#include "axl_exe_WorkerThread.h"
#include "axl_exe_WorkerThreadPool.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace exe {

//.............................................................................

CWorkerThread::CWorkerThread ()
{
	m_TerminateFlag = false;

	m_WaitArray.SetCount (1);
	m_WaitArray [0] = &m_Event;
}

bool 
CWorkerThread::Start ()
{
	ASSERT (!IsOpen ());

	m_TerminateFlag = false;
	return CThreadImplT <CWorkerThread>::Start ();
}

void 
CWorkerThread::Stop (
	bool DoWaitAndClose,
	ulong_t Timeout
	)
{
	SignalStop ();
	
	if (DoWaitAndClose)
		WaitAndClose (Timeout);
}

void 
CWorkerThread::SignalStop ()
{
	if (m_TerminateFlag)
		return;

	m_Lock.Lock ();
	m_TerminateFlag = true;
	m_Event.Signal ();
	m_Lock.Unlock ();
}

// AddEvent / RemoveEvent should only be called once the thread is running

handle_t
CWorkerThread::AddEvent (
	mt::CEvent* pEvent, 
	exe::IFunction* pOnEvent
	)
{
	ASSERT (IsOpen ());

	return (handle_t) SyncSchedule <exe::CArgSeqT_3 <
		CWorkerThread*,
		mt::CEvent*, 
		exe::IFunction*
		> > (pvoid_cast (&CWorkerThread::AddEvent_wt), this, pEvent, pOnEvent);
}

void 
CWorkerThread::RemoveEvent (handle_t hEvent)
{
	ASSERT (IsOpen ());

	SyncSchedule <exe::CArgSeqT_2 <
		CWorkerThread*,
		mt::CEvent*
		> > (pvoid_cast (&CWorkerThread::RemoveEvent_wt), this, hEvent);
}

inline
bool 
CWorkerThread::CanInvokeNow ()
{
#if (_AXL_ENV == AXL_ENV_WIN)
	return GetThreadId () == mt::GetCurrentThreadId ();
#elif (_AXL_ENV == AXL_ENV_NT)
	return GetThreadId () == mt::GetCurrentThreadId () && KeGetCurrentIrql () == PASSIVE_LEVEL;
#endif
}

CWorkerThread::EScheduleResult
CWorkerThread::ScheduleV (
	exe::IFunction* pFunction, 
	axl_va_list va
	)
{
	ASSERT (IsOpen ());

	if (CanInvokeNow ())
	{
		pFunction->InvokeV (va);
		return EScheduleResult_Invoke;
	}

	m_Lock.Lock ();
	m_InvokeList.AddV (pFunction, va);
	m_Event.Signal ();
	m_Lock.Unlock ();

	return EScheduleResult_Pending;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// everything below runs in worker thread

ulong_t 
CWorkerThread::ThreadProc ()
{
	bool TerminateFlag = false;

	HANDLE WaitArray [MAXIMUM_WAIT_OBJECTS];

	do
	{
		size_t Count = m_WaitArray.GetCount ();
		ASSERT (Count > 0 && Count <= MAXIMUM_WAIT_OBJECTS);

		for (size_t i = 0; i < Count; i++)
			WaitArray [i] = *m_WaitArray [i];

		ulong_t Result = win::CWaitableHandle::MultiWait (WaitArray, Count, false, -1, true);
		switch (Result)
		{
		case 0: 
			TerminateFlag = Process_wt ();
			break;

		default:
			if (Result >= 1 && (size_t) Result < Count)
			{
				size_t Index = Result - 1;
				ASSERT (Index < m_FunctionArray.GetCount ());
				m_FunctionArray [Index]->Invoke (0);
			}
		}
	} while (!TerminateFlag);

	return 0;
}

CWorkerThread::TUserEvent*
CWorkerThread::AddEvent_wt (
	mt::CEvent* pEvent,
	exe::IFunction* pOnEvent
	)
{
	size_t Count = m_WaitArray.GetCount ();
	if (Count >= MAXIMUM_WAIT_OBJECTS)
		return NULL;

	TUserEvent* pUserEvent = AXL_MEM_NEW (TUserEvent);
	pUserEvent->m_pEvent = pEvent;
	pUserEvent->m_OnEvent = ref::Clone (pOnEvent);
	m_UserEventList.InsertTail (pUserEvent);

	m_WaitArray.Append (pEvent);
	m_FunctionArray.Append (pUserEvent->m_OnEvent);

	return pUserEvent;
}

void
CWorkerThread::RemoveEvent_wt (TUserEvent* pUserEvent)
{
	m_UserEventList.Delete (pUserEvent);

	size_t Count = m_UserEventList.GetCount ();

	m_WaitArray.SetCount (Count + 1);
	m_FunctionArray.SetCount (Count);

	rtl::CIteratorT <TUserEvent> It = m_UserEventList.GetHead ();
	mt::CEvent** ppEvent = m_WaitArray.GetBuffer () + 1;
	exe::IFunction** ppOnEvent = m_FunctionArray.GetBuffer ();

	for (; It; It++, ppEvent++, ppOnEvent++)
	{
		*ppEvent = It->m_pEvent;
		*ppOnEvent = It->m_OnEvent;
	}
}

bool
CWorkerThread::Process_wt ()
{
	exe::CInvokeList InvokeList;

	m_Lock.Lock ();
	InvokeList.TakeOver (&m_InvokeList);
	bool TerminateFlag = m_TerminateFlag;
	m_Lock.Unlock ();

	InvokeList.Process ();
	return TerminateFlag;
}

//.............................................................................

ref::CPtrT <CWorkerThread>
GetWorkerThread (size_t ReserveEventCount)
{
	exe::CWorkerThreadPool* pPool = rtl::GetSingleton <exe::CWorkerThreadPool> ();
	return pPool->GetThread (ReserveEventCount);
}

ref::CPtrT <CWorkerThread>
GetWorkerThread (
	mt::CEvent* pEvent,
	exe::IFunction* pOnEvent,
	handle_t* phEvent
	)
{
	exe::CWorkerThreadPool* pPool = rtl::GetSingleton <exe::CWorkerThreadPool> ();
	ref::CPtrT <CWorkerThread> Thread = pPool->GetThread (1);
	if (!Thread)
		return ref::EPtr_Null;

	handle_t hEvent = Thread->AddEvent (pEvent, pOnEvent);
	if (!hEvent)
		return ref::EPtr_Null;

	*phEvent = hEvent;
	return Thread;
}

//.............................................................................

} // namespace exe
} // namespace axl

