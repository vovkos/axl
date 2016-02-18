#include "pch.h"
#include "axl_sys_win_WaitableHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

WaitResult 
WaitableHandle::completeWait (dword_t result)
{
	switch (result)
	{
	case WAIT_FAILED:
		return err::failWithLastSystemError (WaitResult_Fail);

	case WAIT_TIMEOUT:
		return WaitResult_Timeout;

	case WAIT_IO_COMPLETION:
		return WaitResult_IoCompletion;

	default:
		if (result >= WAIT_OBJECT_0 &&
			result <= WAIT_OBJECT_0 + MAXIMUM_WAIT_OBJECTS
			)
		{
			return (WaitResult) (WaitResult_Object0 + result - WAIT_OBJECT_0);
		}

		err::setError (err::SystemErrorCode_InvalidParameter);
		return WaitResult_Fail;
	}
}

//.............................................................................

} // namespace win
} // namespace sys
} // namespace axl
