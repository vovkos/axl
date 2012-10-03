#include "stdafx.h"
#include "axl_mt_Timer.h"
#include "axl_ref_Clone.h"

namespace axl {
namespace mt {

//.............................................................................

bool
CTimer::SetTimer (	
	ulong_t Timeout,
	exe::IFunction* pOnTimer,
	int Flags
	)
{
	if (!m_WorkerThread)
	{
		m_WorkerThread = exe::GetWorkerThread ();
		if (!m_WorkerThread)
			return false;
	}

	return m_WorkerThread->SyncSchedule <exe::CArgSeqT_4 <
		CTimer*, 
		ulong_t, 
		exe::IFunction*, 
		int > > (

		pvoid_cast (&CTimer::SetTimer_wt),
		this, 
		Timeout, 
		pOnTimer,
		Flags
		) != 0;
}

void
CTimer::Cancel ()
{
	if (m_WorkerThread)
		m_WorkerThread->SyncSchedule <exe::CArgT <CTimer*> > (
		pvoid_cast (&CTimer::Cancel_wt),
		this
		);
}

bool 
CTimer::SetTimer_wt (	
	ulong_t Timeout,
	exe::IFunction* pOnTimer,
	int Flags
	)
{
	bool Result;

	if (!m_Timer.IsOpen ())
	{
		Result = m_Timer.Create (NULL, (Flags & ETimer_ManualReset) != 0, NULL);
		if (!Result)
			return false;
	}

	uint64_t DueTime = Int32x32To64(Timeout, -10000);
	ulong_t Period = (Flags & ETimer_Periodic) ? Timeout : 0;

	Result = m_Timer.SetTimer (DueTime, Period, TimerProc, this);
	if (!Result)
		return false;

	m_OnTimer = ref::GetPtrOrClone (pOnTimer);
	return true;
}

void
CTimer::Cancel_wt ()
{
	if (m_Timer.IsOpen ())
		m_Timer.Cancel ();

	m_OnTimer = NULL;
}

VOID 
CALLBACK
CTimer::TimerProc (
	LPVOID pContext,
	DWORD TimerLo,
	DWORD TimerHi
	)
{
	CTimer* pThis = (CTimer*) pContext;
	
	if (pThis->m_OnTimer)
		pThis->m_OnTimer->Invoke (0);
}

//.............................................................................

} // namespace mt
} // namespace axl
