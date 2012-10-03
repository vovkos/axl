#include "stdafx.h"
#include "axl_exe_WorkerThreadPool.h"
#include "axl_ref_Factory.h"
#include "axl_g_Module.h"

namespace axl {
namespace exe {

//.............................................................................
	
CWorkerThreadPool::CThreadPin::CThreadPin ()
{
	m_pThreadPool = NULL;
	m_pThreadEntry = NULL;
	m_ReserveEventCount = 0;
}

CWorkerThreadPool::CThreadPin::~CThreadPin ()
{
	m_pThreadPool->m_Lock.Lock ();

	ASSERT (m_pThreadEntry->m_ReserveEventCount >= m_ReserveEventCount);
	m_pThreadEntry->m_ReserveEventCount -= m_ReserveEventCount;
	m_pThreadEntry->m_PinCount--;

	if (!m_pThreadEntry->m_PinCount)
		m_pThreadEntry->m_Thread.Stop (false);

	m_pThreadPool->m_Lock.Unlock ();
}

//.............................................................................

bool 
CWorkerThreadPool::Start (size_t ThreadCount)
{
	ASSERT (m_ThreadList.IsEmpty ());
	return Grow (ThreadCount);
}

bool 
CWorkerThreadPool::Grow (size_t ThreadCount)
{
	if (ThreadCount == 0)
		ThreadCount = g::GetModule ()->GetSystemInfo ()->m_ProcessorCount;

	rtl::CStdListT <TThreadEntry> ThreadList;

	for (size_t i = 0; i < ThreadCount; i++)
	{
		TThreadEntry* pThreadEntry = AXL_MEM_NEW (TThreadEntry);
		pThreadEntry->m_ReserveEventCount = 0;
		pThreadEntry->m_PinCount = 1;
		ThreadList.InsertTail (pThreadEntry);
		
		bool Result = pThreadEntry->m_Thread.Start (); 
		if (!Result)
			return false;
	}

	m_Lock.Lock ();
	m_ThreadList.InsertListTail (&ThreadList);
	m_Lock.Unlock ();
	return true;
}

void 
CWorkerThreadPool::Stop (ulong_t Timeout)
{
	m_Lock.Lock ();

	rtl::CStdListT <TThreadEntry> ThreadList;
	ThreadList.TakeOver (&m_ThreadList);

	rtl::CIteratorT <TThreadEntry> It = ThreadList.GetHead ();
	for (; It; It++)
	{
		It->m_PinCount--;
		if (!It->m_PinCount)
			It->m_Thread.Stop (false);
	}

	m_Lock.Unlock ();

	// wait

	while (!ThreadList.IsEmpty ())
	{
		TThreadEntry* pThreadEntry = ThreadList.RemoveHead ();
		pThreadEntry->m_Thread.WaitAndClose (Timeout);
		AXL_MEM_DELETE (pThreadEntry);
	}
}

ref::CPtrT <CWorkerThread> 
CWorkerThreadPool::GetThread (size_t ReserveEventCount)
{
	ASSERT (ReserveEventCount <= MAXIMUM_WAIT_OBJECTS);

	rtl::CIteratorT <TThreadEntry> It;

	m_Lock.Lock ();

	for (size_t i = 0;; i++) 
	{
		It = m_ThreadList.GetHead ();
		for (; It; It++)
		{
			if (It->m_ReserveEventCount + ReserveEventCount <= MAXIMUM_WAIT_OBJECTS)
				break;
		}

		if (It)
			break;

		if (i >= 2) // try 3 times, if fail each time probably wrong use
		{
			m_Lock.Unlock ();
			return NULL;
		}

		m_Lock.Unlock ();

		bool Result = Grow ();
		if (!Result)
			return NULL;

		m_Lock.Lock ();
	}

	TThreadEntry* pThreadEntry = *It;
	pThreadEntry->m_ReserveEventCount += ReserveEventCount;
	pThreadEntry->m_PinCount++;

	if (It != m_ThreadList.GetTail ()) // ensure rotation: try to balance workload
	{
		m_ThreadList.Remove (It);
		m_ThreadList.InsertTail (pThreadEntry);
	}

	m_Lock.Unlock ();

	ref::CPtrT <CThreadPin> ThreadPin = AXL_REF_NEW (CThreadPin);
	ThreadPin->m_pThreadPool = this;
	ThreadPin->m_pThreadEntry = pThreadEntry;
	ThreadPin->m_ReserveEventCount = ReserveEventCount;

	return ref::CPtrT <CWorkerThread> (
		&pThreadEntry->m_Thread, 
		ThreadPin.GetRefCount ()
		);
}

//.............................................................................

} // namespace exe
} // namespace axl

