#include "pch.h"
#include "axl_mt_Timer.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace mt {

//.............................................................................

bool
CTimer::setTimer (	
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

	return m_workerThread->syncSchedule <exe::CArgSeqT_4 <
		CTimer*, 
		ulong_t, 
		exe::IFunction*, 
		int > > (

		pvoid_cast (&CTimer::setTimer_wt),
		this, 
		timeout, 
		onTimer,
		flags
		) != 0;
}

void
CTimer::cancel ()
{
	if (m_workerThread)
		m_workerThread->syncSchedule <exe::CArgT <CTimer*> > (
		pvoid_cast (&CTimer::cancel_wt),
		this
		);
}

bool 
CTimer::setTimer_wt (	
	ulong_t timeout,
	exe::IFunction* onTimer,
	int flags
	)
{
	bool result;

	if (!m_timer.isOpen ())
	{
		result = m_timer.create (NULL, (flags & ETimer_ManualReset) != 0, NULL);
		if (!result)
			return false;
	}

	uint64_t dueTime = int32x32To64(timeout, -10000);
	ulong_t period = (flags & ETimer_Periodic) ? timeout : 0;

	result = m_timer.setTimer (dueTime, period, timerProc, this);
	if (!result)
		return false;

	m_onTimer = ref::getPtrOrClone (onTimer);
	return true;
}

void
CTimer::cancel_wt ()
{
	if (m_timer.isOpen ())
		m_timer.cancel ();

	m_onTimer = ref::EPtr_Null;
}

VOID 
CALLBACK
CTimer::timerProc (
	LPVOID context,
	DWORD timerLo,
	DWORD timerHi
	)
{
	CTimer* this = (CTimer*) context;
	
	if (this->m_onTimer)
		this->m_onTimer->invoke (0);
}

//.............................................................................

} // namespace mt
} // namespace axl
