#include "stdafx.h"
#include "axl_win_WaitableHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................

CWaitableHandle::EWaitResult 
CWaitableHandle::CompleteWait (ulong_t Result)
{
	switch (Result)
	{
	case WAIT_FAILED:
		err::SetLastSystemError ();
		return CWaitableHandle::EWaitResult_Fail;

	case WAIT_TIMEOUT:
		return CWaitableHandle::EWaitResult_Timeout;

	case WAIT_IO_COMPLETION:
		return CWaitableHandle::EWaitResult_IoCompletion;

	default:
		if (Result >= WAIT_OBJECT_0 &&
			Result <= WAIT_OBJECT_0 + MAXIMUM_WAIT_OBJECTS
			)
		{
			return (EWaitResult) (EWaitResult_Object0 + Result - WAIT_OBJECT_0);
		}

		err::SetError (err::EStatus_InvalidParameter);
		return CWaitableHandle::EWaitResult_Fail;
	}
}

//.............................................................................

} // namespace win
} // namespace axl
