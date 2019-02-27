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
File::create(
	const sl::StringRef_w& fileName,
	uint_t accessMode,
	uint_t shareMode,
	SECURITY_ATTRIBUTES* secAttr,
	uint_t creationDisposition,
	uint_t flagsAttributes
	)
{
	close();

	m_h = ::CreateFileW(
		fileName.sz(),
		accessMode,
		shareMode,
		secAttr,
		creationDisposition,
		flagsAttributes,
		NULL
		);

	return err::complete(m_h != INVALID_HANDLE_VALUE);
}

bool
File::duplicate(
	HANDLE sourceProcess,
	HANDLE sourceHandle,
	HANDLE targetProcess,
	dword_t accessMode,
	bool isInheritable,
	dword_t options
	)
{
	close();

	bool_t result = ::DuplicateHandle(
		sourceProcess,
		sourceHandle,
		targetProcess,
		&m_h,
		accessMode,
		isInheritable,
		options
		);

	return err::complete(result);
}

uint64_t
File::getSize() const
{
	ULARGE_INTEGER size;

	size.LowPart = ::GetFileSize(m_h, &size.HighPart);
	if (size.LowPart == INVALID_FILE_SIZE)
		return err::failWithLastSystemError(-1);

	return size.QuadPart;
}

bool
File::setSize(uint64_t size)
{
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

uint64_t
File::getPosition() const
{
	LARGE_INTEGER offset = { 0 };

	offset.LowPart = ::SetFilePointer(m_h, 0, &offset.HighPart, FILE_CURRENT);
	if (offset.LowPart == INVALID_FILE_SIZE)
		return err::failWithLastSystemError<uint64_t> (-1);

	return offset.QuadPart;
}

bool
File::setPosition(uint64_t _Offset) const
{
	LARGE_INTEGER offset;
	offset.QuadPart = _Offset;

	dword_t result = ::SetFilePointer(m_h, offset.LowPart, &offset.HighPart, FILE_BEGIN);
	return err::complete(result != INVALID_SET_FILE_POINTER);
}

size_t
File::read(
	void* p,
	size_t size
	) const
{
	dword_t actualSize;
	bool result = read(p, (dword_t)size, &actualSize);
	return result ? (size_t)actualSize : -1;
}

size_t
File::write(
	const void* p,
	size_t size
	)
{
	dword_t actualSize;
	bool result = write(p, (dword_t)size, &actualSize);
	return result ? (size_t)actualSize : -1;
}

size_t
File::overlappedRead(
	void* p,
	size_t size
	) const
{
	dword_t actualSize;
	bool result = overlappedRead(p, (dword_t)size, &actualSize);
	return result ? (size_t)actualSize : -1;
}

size_t
File::overlappedWrite(
	const void* p,
	size_t size
	)
{
	dword_t actualSize;
	bool result = overlappedWrite(p, (dword_t)size, &actualSize);
	return result ? (size_t)actualSize : -1;
}

bool
File::overlappedRead(
	void* p,
	dword_t size,
	dword_t* actualSize
	) const
{
	StdOverlapped overlapped;
	bool result = overlappedRead(p, size, &overlapped);
	return result ? getOverlappedResult(&overlapped, actualSize) : false;
}

bool
File::overlappedWrite(
	const void* p,
	dword_t size,
	dword_t* actualSize
	)
{
	StdOverlapped overlapped;
	bool result = overlappedWrite(p, size, &overlapped);
	return result ? getOverlappedResult(&overlapped, actualSize) : false;
}

bool
File::overlappedIoctl(
	dword_t code,
	const void* inData,
	dword_t inDataSize,
	void* outData,
	dword_t outDataSize,
	dword_t* actualSize
	)
{
	StdOverlapped overlapped;
	bool result = overlappedIoctl(code, inData, inDataSize, outData, outDataSize, &overlapped);
	return result ? getOverlappedResult(&overlapped, actualSize) : false;
}

bool
File::completeOverlappedRequest(bool_t result)
{
	if (result)
		return true;

	dword_t error = ::GetLastError();
	if (error != ERROR_IO_PENDING)
	{
		err::setError(error);
		return false;
	}

	return true;
}

bool
File::getOverlappedResult(
	OVERLAPPED* overlapped,
	dword_t* actualSize
	) const
{
	bool_t result = ::GetOverlappedResult(m_h, overlapped, actualSize, true);
	if (!result)
	{
		DWORD error = ::GetLastError();
		if (error != ERROR_HANDLE_EOF)
		{
			err::setError(error);
			return false;
		}

		*actualSize = 0; // EOF is not an error
	}

	return true;
}

size_t
File::getOverlappedResult(OVERLAPPED* overlapped) const
{
	dword_t actualSize;
	bool result = getOverlappedResult(overlapped, &actualSize);
	return result ? (size_t)actualSize : -1;
}

//..............................................................................

} // namespace win {
} // namespace io
} // namespace axl
