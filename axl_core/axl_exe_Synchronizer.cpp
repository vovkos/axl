#include "stdafx.h"
#include "axl_exe_Synchronizer.h"
#include "axl_exe_Function.h"
#include "axl_mt_Thread.h"

namespace axl {
namespace exe {

//.............................................................................

CSynchronizer::CSynchronizer (int Flags)
{
	m_InvokeThreadId = -1;
	m_Flags = Flags;
}

CSynchronizer::EScheduleResult
CSynchronizer::ScheduleV (
	IFunction* pFunction, 
	va_list va
	)
{
	ulong_t CurrentThreadId = mt::GetCurrentThreadId ();

	m_Lock.Lock ();

	if (m_InvokeThreadId != -1 &&
		((m_Flags & EFlags_AlwaysEnqueue) ||  m_InvokeThreadId != CurrentThreadId))
	{
		m_InvokeList.AddV (pFunction, va);
		m_Lock.Unlock ();
		
		return EScheduleResult_Pending;
	}

	m_InvokeThreadId = CurrentThreadId;
	m_Lock.Unlock ();

	pFunction->InvokeV (va);

	m_Lock.Lock ();
	while (!m_InvokeList.IsEmpty ())
	{
		CInvokeList InvokeList;
		InvokeList.TakeOver (&m_InvokeList);
		m_Lock.Unlock ();
		
		InvokeList.Process ();
		ASSERT (InvokeList.IsEmpty ());
		
		m_Lock.Lock ();
	}

	m_Lock.Unlock ();

	return EScheduleResult_Invoke;
}

//.............................................................................

} // namespace exe
} // namespace axl
