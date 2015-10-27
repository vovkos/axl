#include "pch.h"
#include "axl_io_win_File.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool
File::create (
	const wchar_t* fileName,
	uint_t accessMode,
	uint_t shareMode,
	SECURITY_ATTRIBUTES* secAttr,
	uint_t creationDisposition,
	uint_t flagsAttributes
	)
{
	close ();

	m_h = ::CreateFileW (
		fileName,
		accessMode,
		shareMode,
		secAttr,
		creationDisposition,
		flagsAttributes,
		NULL
		);

	return err::complete (m_h != INVALID_HANDLE_VALUE);
}

uint64_t
File::getSize () const
{
	ULARGE_INTEGER size;

	size.LowPart = ::GetFileSize (m_h, &size.HighPart);
	if (size.LowPart == INVALID_FILE_SIZE)
		return err::failWithLastSystemError (-1);

	return size.QuadPart;
}

bool
File::setSize (uint64_t size)
{
	uint64_t position = getPosition ();

	bool_t result = setPosition (size);
	if (!result)
		return false;

	result = ::SetEndOfFile (m_h);
	if (!result)
		return err::failWithLastSystemError ();

	setPosition (position);
	return true;
}

uint64_t
File::getPosition () const
{
	LARGE_INTEGER offset = { 0 };

	offset.LowPart = ::SetFilePointer (m_h, 0, &offset.HighPart, FILE_CURRENT);
	if (offset.LowPart == INVALID_FILE_SIZE)
		return err::failWithLastSystemError (-1);

	return offset.QuadPart;
}

bool
File::setPosition (uint64_t _Offset) const
{
	LARGE_INTEGER offset;
	offset.QuadPart = _Offset;

	dword_t result = ::SetFilePointer (m_h, offset.LowPart, &offset.HighPart, FILE_BEGIN);
	return err::complete (result != INVALID_SET_FILE_POINTER);
}

size_t
File::read (
	void* p,
	size_t size
	) const
{
	dword_t actualSize;
	bool_t result = read (p, (dword_t) size, &actualSize, NULL);
	return result ? (size_t) actualSize : -1;
}

size_t
File::write (
	const void* p,
	size_t size
	)
{
	dword_t actualSize;
	bool_t result = write (p, (dword_t) size, &actualSize, NULL);
	return result ? (size_t) actualSize : -1;
}

bool
File::overlappedRead (
	void* p,
	dword_t size,
	dword_t* actualSize
	) const
{
	StdOverlapped overlapped;
	bool result = read (p, size, actualSize, &overlapped);
	return result ? getOverlappedResult (&overlapped, actualSize) : false;
}

bool
File::overlappedWrite (
	const void* p,
	dword_t size,
	dword_t* actualSize
	)
{
	StdOverlapped overlapped;
	bool result = write (p, size, actualSize, &overlapped);
	return result ? getOverlappedResult (&overlapped, actualSize) : false;
}

bool
File::overlappedIoctl (
	dword_t code,
	const void* inData,
	dword_t inDataSize,
	void* outData,
	dword_t outDataSize,
	dword_t* actualSize
	)
{
	StdOverlapped overlapped;
	bool result = ioctl (code, inData, inDataSize, outData, outDataSize, actualSize, &overlapped);
	return result ? getOverlappedResult (&overlapped, actualSize) : false;
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl

