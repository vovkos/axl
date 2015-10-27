#include "pch.h"
#include "axl_io_win_FileHandle.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool
FileHandle::completeAsyncRequest (
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

dword_t
FileHandle::getOverlappedResult (OVERLAPPED* overlapped) const
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

bool
FileHandle::getOverlappedResult (
	OVERLAPPED* overlapped,
	dword_t* resultActualSize
	) const
{
	dword_t actualSize = getOverlappedResult (overlapped);
	
	if (resultActualSize)
		*resultActualSize = actualSize;

	return actualSize != -1;
}

//.............................................................................

} // namespace win {
} // namespace io
} // namespace axl
