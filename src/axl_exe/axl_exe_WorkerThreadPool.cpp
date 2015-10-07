#include "pch.h"
#include "axl_exe_WorkerThreadPool.h"
#include "axl_ref_Factory.h"
#include "axl_g_Module.h"

namespace axl {
namespace exe {

//.............................................................................
	
WorkerhreadPool::ThreadPin::ThreadPin ()
{
	m_threadPool = NULL;
	m_threadEntry = NULL;
	m_reserveEventCount = 0;
}

WorkerhreadPool::ThreadPin::~ThreadPin ()
{
	m_threadPool->m_lock.lock ();

	ASSERT (m_threadEntry->m_reserveEventCount >= m_reserveEventCount);
	m_threadEntry->m_reserveEventCount -= m_reserveEventCount;
	m_threadEntry->m_pinCount--;

	if (!m_threadEntry->m_pinCount)
		m_threadEntry->m_thread.stop (false);

	m_threadPool->m_lock.unlock ();
}

//.............................................................................

bool 
WorkerhreadPool::start (size_t threadCount)
{
	ASSERT (m_threadList.isEmpty ());
	return grow (threadCount);
}

bool 
WorkerhreadPool::grow (size_t threadCount)
{
	if (threadCount == 0)
		threadCount = g::getModule ()->getSystemInfo ()->m_processorCount;

	sl::StdList <ThreadEntry> threadList;

	for (size_t i = 0; i < threadCount; i++)
	{
		ThreadEntry* threadEntry = AXL_MEM_NEW (ThreadEntry);
		threadEntry->m_reserveEventCount = 0;
		threadEntry->m_pinCount = 1;
		threadList.insertTail (threadEntry);
		
		bool result = threadEntry->m_thread.start (); 
		if (!result)
			return false;
	}

	m_lock.lock ();
	m_threadList.insertListTail (&threadList);
	m_lock.unlock ();
	return true;
}

void 
WorkerhreadPool::stop (ulong_t timeout)
{
	m_lock.lock ();

	sl::StdList <ThreadEntry> threadList;
	threadList.takeOver (&m_threadList);

	sl::Iterator <ThreadEntry> it = threadList.getHead ();
	for (; it; it++)
	{
		it->m_pinCount--;
		if (!it->m_pinCount)
			it->m_thread.stop (false);
	}

	m_lock.unlock ();

	// wait

	while (!threadList.isEmpty ())
	{
		ThreadEntry* threadEntry = threadList.removeHead ();
		threadEntry->m_thread.waitAndClose (timeout);
		AXL_MEM_DELETE (threadEntry);
	}
}

ref::Ptr <Workerhread> 
WorkerhreadPool::getThread (size_t reserveEventCount)
{
	ASSERT (reserveEventCount <= MAXIMUM_WAIT_OBJECTS);

	sl::Iterator <ThreadEntry> it;

	m_lock.lock ();

	for (size_t i = 0;; i++) 
	{
		it = m_threadList.getHead ();
		for (; it; it++)
		{
			if (it->m_reserveEventCount + reserveEventCount <= MAXIMUM_WAIT_OBJECTS)
				break;
		}

		if (it)
			break;

		if (i >= 2) // try 3 times, if fail each time probably wrong use
		{
			m_lock.unlock ();
			return ref::PtrKind_Null;
		}

		m_lock.unlock ();

		bool result = grow ();
		if (!result)
			return ref::PtrKind_Null;

		m_lock.lock ();
	}

	ThreadEntry* threadEntry = *it;
	threadEntry->m_reserveEventCount += reserveEventCount;
	threadEntry->m_pinCount++;

	if (it != m_threadList.getTail ()) // ensure rotation: try to balance workload
	{
		m_threadList.remove (it);
		m_threadList.insertTail (threadEntry);
	}

	m_lock.unlock ();

	ref::Ptr <ThreadPin> threadPin = AXL_REF_NEW (ThreadPin);
	threadPin->m_threadPool = this;
	threadPin->m_threadEntry = threadEntry;
	threadPin->m_reserveEventCount = reserveEventCount;

	return ref::Ptr <Workerhread> (
		&threadEntry->m_thread, 
		threadPin.getRefCount ()
		);
}

//.............................................................................

} // namespace exe
} // namespace axl

