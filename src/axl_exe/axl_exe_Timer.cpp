#include "pch.h"
#include "axl_mt_Timer.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace mt {

//.............................................................................

bool
Timer::setTimer (	
	ulong_t timeout,
	exe::IFunction* onTimer,
	int flags
	)
{
	if (!m_workerThread)
	{
		m_workerThread = exe::getWorkerThread ();
		if (!m_workerThread)
			return false;
	}

	return m_workerThread->syncSchedule <exe::ArgSeq_4 <
		Timer*, 
		ulong_t, 
		exe::IFunction*, 
		int > > (

		pvoid_cast (&Timer::setTimer_wt),
		this, 
		timeout, 
		onTimer,
		flags
		) != 0;
}

void
Timer::cancel ()
{
	if (m_workerThread)
		m_workerThread->syncSchedule <exe::Arg <Timer*> > (
		pvoid_cast (&Timer::cancel_wt),
		this
		);
}

bool 
Timer::setTimer_wt (	
	ulong_t timeout,
	exe::IFunction* onTimer,
	int flags
	)
{
	bool result;

	if (!m_timer.isOpen ())
	{
		result = m_timer.create (NULL, (flags & TimerKind_ManualReset) != 0, NULL);
		if (!result)
			return false;
	}

	uint64_t dueTime = int32x32To64(timeout, -10000);
	ulong_t period = (flags & TimerKind_Periodic) ? timeout : 0;

	result = m_timer.setTimer (dueTime, period, timerProc, this);
	if (!result)
		return false;

	m_onTimer = ref::getPtrOrClone (onTimer);
	return true;
}

void
Timer::cancel_wt ()
{
	if (m_timer.isOpen ())
		m_timer.cancel ();

	m_onTimer = ref::PtrKind_Null;
}

VOID 
CALLBACK
Timer::timerProc (
	LPVOID context,
	DWORD timerLo,
	DWORD timerHi
	)
{
	Timer* this = (Timer*) context;
	
	if (this->m_onTimer)
		this->m_onTimer->invoke (0);
}

//.............................................................................

} // namespace mt
} // namespace axl
