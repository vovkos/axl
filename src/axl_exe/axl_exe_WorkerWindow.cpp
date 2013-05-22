#include "pch.h"
#include "axl_exe_WorkerWindow.h"
#include "axl_exe_Function.h"

namespace axl {
namespace exe {

//.............................................................................

CWorkerWindow::EScheduleResult
CWorkerWindow::ScheduleV (
	exe::IFunction* pFunction, 
	axl_va_list va
	)
{
	// check if we are already in worker thread

	if (m_ThreadId == ::GetCurrentThreadId ())
	{
		pFunction->InvokeV (va);
		return EScheduleResult_Invoke;
	}

	m_Lock.Lock ();

	m_InvokeList.AddV (pFunction, va);

	if (m_InvokeList.GetCount () == 1) // post msg only once
		PostMessage (WM_PROCESS_INVOKE_LIST);

	m_Lock.Unlock ();

	return EScheduleResult_Pending;
}

LRESULT
CWorkerWindow::OnProcessInvokeList (
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam,
	bool* pIsHandled
	)
{
	exe::CInvokeList InvokeList;

	m_Lock.Lock ();
	InvokeList.TakeOver (&m_InvokeList);
	m_Lock.Unlock ();

	InvokeList.Process ();

	return 0;
}

//.............................................................................

} // namespace exe
} // namespace axl

