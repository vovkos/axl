#include "pch.h"
#include "axl_exe_Synchronizer.h"
#include "axl_exe_Function.h"
#include "axl_mt_Thread.h"

namespace axl {
namespace exe {

//.............................................................................

Synchronizer::Synchronizer (int flags)
{
	m_invokeThreadId = -1;
	m_flags = flags;
}

Synchronizer::ScheduleResultKind
Synchronizer::scheduleV (
	IFunction* function, 
	axl_va_list va
	)
{
	ulong_t currentThreadId = mt::getCurrentThreadId ();

	m_lock.lock ();

	if (m_invokeThreadId != -1 &&
		((m_flags & FlagsKind_AlwaysEnqueue) ||  m_invokeThreadId != currentThreadId))
	{
		m_invokeList.addV (function, va);
		m_lock.unlock ();
		
		return ScheduleResultKind_Pending;
	}

	m_invokeThreadId = currentThreadId;
	m_lock.unlock ();

	function->invokeV (va);

	m_lock.lock ();
	while (!m_invokeList.isEmpty ())
	{
		InvokeList invokeList;
		invokeList.takeOver (&m_invokeList);
		m_lock.unlock ();
		
		invokeList.process ();
		ASSERT (invokeList.isEmpty ());
		
		m_lock.lock ();
	}

	m_lock.unlock ();

	return ScheduleResultKind_Invoke;
}

//.............................................................................

} // namespace exe
} // namespace axl
