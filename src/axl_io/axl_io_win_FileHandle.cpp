//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_io_win_FileHandle.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

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

bool
FileHandle::getOverlappedResult (
	OVERLAPPED* overlapped,
	dword_t* actualSize
	) const
{
	bool_t result = ::GetOverlappedResult (m_h, overlapped, actualSize, true);
	if (!result)
	{
		DWORD error = ::GetLastError ();
		if (error != ERROR_HANDLE_EOF)
		{
			err::setError (error);
			return false;
		}

		*actualSize = 0; // EOF is not an error
	}

	return true;
}

size_t
FileHandle::getOverlappedResult (OVERLAPPED* overlapped) const
{
	dword_t actualSize;
	bool result = getOverlappedResult (overlapped, &actualSize);
	return result ? (size_t) actualSize : -1;
}

//..............................................................................

} // namespace win {
} // namespace io
} // namespace axl
