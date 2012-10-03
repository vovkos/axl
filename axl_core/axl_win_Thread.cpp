#include "stdafx.h"
#include "axl_win_Thread.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................

bool 
CThread::Create (
	SECURITY_ATTRIBUTES* pSecAttr,
	ulong_t StackSize,
	LPTHREAD_START_ROUTINE pfnThreadProc,
	void* pContext,
	ulong_t Flags
	)
{
	Close ();

	ulong_t ThreadId;

	m_h = ::CreateThread (pSecAttr, StackSize, pfnThreadProc, pContext, Flags, &ThreadId);
	return err::Complete (m_h != NULL);
}

bool
CThread::Terminate (ulong_t ExitCode)
{
	ASSERT (IsOpen ());

	bool_t Result = ::TerminateThread (m_h, -1);
	return err::Complete (Result);
}

ulong_t
CThread::GetExitCode ()
{
	ASSERT (IsOpen ());
	
	ulong_t Code;
	
	bool_t Result = ::GetExitCodeThread (m_h, &Code);
	if (!Result)
	{
		err::SetLastSystemError ();
		return -1;
	}

	return Code;
}

bool
CThread::SetPriority (int Priority)
{
	ASSERT (IsOpen ());

	bool_t Result = ::SetThreadPriority (m_h, Priority);
	return err::Complete (Result);
}

//.............................................................................

} // namespace win
} // namespace axl
