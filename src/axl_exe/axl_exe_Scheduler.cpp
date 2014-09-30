#include "pch.h"
#include "axl_exe_Scheduler.h"
#include "axl_exe_ScheduledFunction.h"
#include "axl_mt_Event.h"

namespace axl {
namespace exe {

//.............................................................................

static
void
invokeAndSignalV (
	IFunction* function,
	intptr_t* invokeResult,
	mt::Event* event,
	axl_va_list va
	)
{
	*invokeResult = function->invokeV (va);
	event->signal ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

intptr_t
IScheduler::syncScheduleV (
	IFunction* function, 
	axl_va_list va
	)
{
	intptr_t invokeResult = -1;
	mt::Event event;
	
	int result = schedule <exe::ArgSeq_4 <
		IFunction*,
		intptr_t*,
		mt::Event*,
		axl_va_list
		> > (invokeAndSignalV, function, &invokeResult, &event, va); 

	if (result == ScheduleResultKind_Pending)
		event.wait ();

	return invokeResult;
}

//.............................................................................

} // namespace exe
} // namespace axl
