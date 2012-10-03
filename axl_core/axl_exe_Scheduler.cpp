#include "stdafx.h"
#include "axl_exe_Scheduler.h"
#include "axl_exe_ScheduledFunction.h"
#include "axl_mt_Event.h"

namespace axl {
namespace exe {

//.............................................................................

static
void
InvokeAndSignalV (
	IFunction* pFunction,
	intptr_t* pInvokeResult,
	mt::CEvent* pEvent,
	va_list va
	)
{
	*pInvokeResult = pFunction->InvokeV (va);
	pEvent->Signal ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

intptr_t
IScheduler::SyncScheduleV (
	IFunction* pFunction, 
	va_list va
	)
{
	intptr_t InvokeResult = -1;
	mt::CEvent Event;
	
	int Result = Schedule <exe::CArgSeqT_4 <
		IFunction*,
		intptr_t*,
		mt::CEvent*,
		va_list
		> > (InvokeAndSignalV, pFunction, &InvokeResult, &Event, va); 

	if (Result == EScheduleResult_Pending)
		Event.Wait ();

	return InvokeResult;
}

//.............................................................................

} // namespace exe
} // namespace axl
