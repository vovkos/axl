#include "pch.h"
#include "axl_win_Timer.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................

bool
CTimer::Create (
	SECURITY_ATTRIBUTES* pSecAttr,
	bool IsManualReset,
	const tchar_t* pName
	)
{
	Close ();

	m_h = ::CreateWaitableTimer (pSecAttr, IsManualReset, pName);
	return err::Complete (m_h != NULL);
}

bool
CTimer::Open (
	ulong_t Access,
	bool DoInheritHandle,
	const tchar_t* pName
	)
{
	Close ();

	m_h = ::OpenWaitableTimer (Access, DoInheritHandle, pName);
	return err::Complete (m_h != NULL);
}

bool 
CTimer::SetTimer (	
	uint64_t _DueTime,
	ulong_t Period, 
	PTIMERAPCROUTINE pfThreadProc,
	void* pContext
	)
{
	ASSERT (IsOpen ());

	LARGE_INTEGER DueTime;
	DueTime.QuadPart = _DueTime;

	bool_t Result = ::SetWaitableTimer (m_h, &DueTime, Period, pfThreadProc, pContext, false);
	return err::Complete (Result);
}

//.............................................................................

} // namespace win
} // namespace axl
