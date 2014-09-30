#include "pch.h"
#include "axl_mt_win_WaitableHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

WaitResultKind 
WaitableHandle::completeWait (dword_t result)
{
	switch (result)
	{
	case WAIT_FAILED:
		return err::failWithLastSystemError (WaitResultKind_Fail);

	case WAIT_TIMEOUT:
		return WaitResultKind_Timeout;

	case WAIT_IO_COMPLETION:
		return WaitResultKind_IoCompletion;

	default:
		if (result >= WAIT_OBJECT_0 &&
			result <= WAIT_OBJECT_0 + MAXIMUM_WAIT_OBJECTS
			)
		{
			return (WaitResultKind) (WaitResultKind_Object0 + result - WAIT_OBJECT_0);
		}

		err::setError (err::StatusKind_InvalidParameter);
		return WaitResultKind_Fail;
	}
}

//.............................................................................

} // namespace win
} // namespace mt
} // namespace axl
