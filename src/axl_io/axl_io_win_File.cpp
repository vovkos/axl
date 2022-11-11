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
#include "axl_io_win_File.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

bool
File::setSize(uint64_t size) {
	uint64_t position = getPosition();
	bool_t result = setPosition(size);
	if (!result)
		return false;

	result = ::SetEndOfFile(m_h);
	if (!result)
		return err::failWithLastSystemError();

	setPosition(position);
	return true;
}

bool
File::completeOverlappedIo(bool_t result) {
	if (result)
		return true;

	dword_t error = ::GetLastError();
	if (error != ERROR_IO_PENDING) {
		err::setError(error);
		return false;
	}

	return true;
}

bool
File::getOverlappedResult(
	OVERLAPPED* overlapped,
	dword_t* actualSize
) const {
	bool_t result = ::GetOverlappedResult(m_h, overlapped, actualSize, true);
	if (!result) {
		dword_t error = ::GetLastError();
		if (error != ERROR_HANDLE_EOF) {
			err::setError(error);
			return false;
		}

		*actualSize = 0; // EOF is not an error
	}

	return true;
}

//..............................................................................

} // namespace win {
} // namespace io
} // namespace axl
