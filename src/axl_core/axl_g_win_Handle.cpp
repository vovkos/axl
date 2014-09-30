#include "pch.h"
#include "axl_g_win_Handle.h"

namespace axl {
namespace g {
namespace win {

//.............................................................................

bool
CFileHandle::completeAsyncRequest (
	bool_t result,
	OVERLAPPED* overlapped
	)
{
	if (!result)
	{
		dword_t error = ::GetLastError ();
		if (!overlapped || error != ERROR_IO_PENDING)
		{
			err::setError (error);
			return false;
		}
	}

	return true;
}

size_t
CFileHandle::getOverlappedResult (OVERLAPPED* overlapped)
{
	dword_t actualSize;
	bool_t result = ::GetOverlappedResult (m_h, overlapped, &actualSize, true);
	if (!result)
	{
		DWORD error = ::GetLastError ();
		if (error == ERROR_HANDLE_EOF)
			return 0;

		err::setError (error);
		return -1;
	}

	return actualSize;
}

//.............................................................................

} // namespace win {
} // namespace g
} // namespace axl
