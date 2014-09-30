#include "pch.h"
#include "axl_win_Timer.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................

bool
CTimer::create (
	SECURITY_ATTRIBUTES* secAttr,
	bool isManualReset,
	const tchar_t* name
	)
{
	close ();

	m_h = ::CreateWaitableTimer (secAttr, isManualReset, name);
	return err::complete (m_h != NULL);
}

bool
CTimer::open (
	ulong_t access,
	bool doInheritHandle,
	const tchar_t* name
	)
{
	close ();

	m_h = ::OpenWaitableTimer (access, doInheritHandle, name);
	return err::complete (m_h != NULL);
}

bool 
CTimer::setTimer (	
	uint64_t _DueTime,
	ulong_t period, 
	PTIMERAPCROUTINE pfThreadProc,
	void* context
	)
{
	ASSERT (isOpen ());

	LARGE_INTEGER dueTime;
	dueTime.QuadPart = _DueTime;

	bool_t result = ::SetWaitableTimer (m_h, &dueTime, period, pfThreadProc, context, false);
	return err::complete (result);
}

//.............................................................................

} // namespace win
} // namespace axl
