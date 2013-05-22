#include "pch.h"
#include "axl_mt_win_Thread.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

bool 
CThread::Create (
	SECURITY_ATTRIBUTES* pSecAttr,
	size_t StackSize,
	PTHREAD_START_ROUTINE pfThreadProc,
	void* pContext,
	uint_t Flags
	)
{
	Close ();

	dword_t ThreadId;

	m_h = ::CreateThread (pSecAttr, StackSize, pfThreadProc, pContext, Flags, &ThreadId);
	return err::Complete (m_h != NULL);
}

uint_t
CThread::GetThreadId ()
{
	uint_t Id = ::GetThreadId (m_h);
	return err::Complete (Id, (uint_t) -1);
}

bool
CThread::Terminate (uint_t ExitCode)
{
	bool_t Result = ::TerminateThread (m_h, ExitCode);
	return err::Complete (Result);
}

uint_t
CThread::GetExitCode ()
{
	dword_t Code = 0;	
	::GetExitCodeThread (m_h, &Code);
	return Code;
}

bool
CThread::SetPriority (int Priority)
{
	bool_t Result = ::SetThreadPriority (m_h, Priority);
	return err::Complete (Result);
}

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
