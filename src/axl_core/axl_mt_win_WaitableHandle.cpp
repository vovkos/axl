#include "pch.h"
#include "axl_mt_win_WaitableHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

EWaitResult 
CWaitableHandle::completeWait (dword_t result)
{
	switch (result)
	{
	case WAIT_FAILED:
		return err::failWithLastSystemError (EWaitResult_Fail);

	case WAIT_TIMEOUT:
		return EWaitResult_Timeout;

	case WAIT_IO_COMPLETION:
		return EWaitResult_IoCompletion;

	default:
		if (result >= WAIT_OBJECT_0 &&
			result <= WAIT_OBJECT_0 + MAXIMUM_WAIT_OBJECTS
			)
		{
			return (EWaitResult) (EWaitResult_Object0 + result - WAIT_OBJECT_0);
		}

		err::setError (err::EStatus_InvalidParameter);
		return EWaitResult_Fail;
	}
}

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
