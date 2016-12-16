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

size_t
FileHandle::read (
	void* p,
	size_t size
	) const
{
	dword_t actualSize;
	bool result = read (p, (dword_t) size, &actualSize);
	return result ? (size_t) actualSize : -1;
}

size_t
FileHandle::write (
	const void* p,
	size_t size
	)
{
	dword_t actualSize;
	bool result = write (p, (dword_t) size, &actualSize);
	return result ? (size_t) actualSize : -1;
}

size_t
FileHandle::overlappedRead (
	void* p,
	size_t size
	) const
{
	dword_t actualSize;
	bool result = overlappedRead (p, (dword_t) size, &actualSize);
	return result ? (size_t) actualSize : -1;
}

size_t
FileHandle::overlappedWrite (
	const void* p,
	size_t size
	)
{
	dword_t actualSize;
	bool result = overlappedWrite (p, (dword_t) size, &actualSize);
	return result ? (size_t) actualSize : -1;
}

bool
FileHandle::overlappedRead (
	void* p,
	dword_t size,
	dword_t* actualSize
	) const
{
	StdOverlapped overlapped;
	bool result = overlappedRead (p, size, &overlapped);
	return result ? getOverlappedResult (&overlapped, actualSize) : false;
}

bool
FileHandle::overlappedWrite (
	const void* p,
	dword_t size,
	dword_t* actualSize
	)
{
	StdOverlapped overlapped;
	bool result = overlappedWrite (p, size, &overlapped);
	return result ? getOverlappedResult (&overlapped, actualSize) : false;
}

bool
FileHandle::overlappedIoctl (
	dword_t code,
	const void* inData,
	dword_t inDataSize,
	void* outData,
	dword_t outDataSize,
	dword_t* actualSize
	)
{
	StdOverlapped overlapped;
	bool result = overlappedIoctl (code, inData, inDataSize, outData, outDataSize, &overlapped);
	return result ? getOverlappedResult (&overlapped, actualSize) : false;
}

bool
FileHandle::completeOverlappedRequest (bool_t result)
{
	if (result)
		return true;

	dword_t error = ::GetLastError ();
	if (error != ERROR_IO_PENDING)
	{
		err::setError (error);
		return false;
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
