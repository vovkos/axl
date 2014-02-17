#include "pch.h"
#include "axl_g_win_Handle.h"

namespace axl {
namespace g {
namespace win {

//.............................................................................

bool
CFileHandle::CompleteAsyncRequest (
	bool_t Result,
	OVERLAPPED* pOverlapped
	)
{
	if (!Result)
	{
		dword_t Error = ::GetLastError ();
		if (!pOverlapped || Error != ERROR_IO_PENDING)
		{
			err::SetError (Error);
			return false;
		}
	}

	return true;
}

size_t
CFileHandle::GetOverlappedResult (OVERLAPPED* pOverlapped)
{
	dword_t ActualSize;
	bool_t Result = ::GetOverlappedResult (m_h, pOverlapped, &ActualSize, true);
	if (!Result)
	{
		DWORD Error = ::GetLastError ();
		if (Error == ERROR_HANDLE_EOF)
			return 0;

		err::SetError (Error);
		return -1;
	}

	return ActualSize;
}

//.............................................................................

} // namespace win {
} // namespace g
} // namespace axl
