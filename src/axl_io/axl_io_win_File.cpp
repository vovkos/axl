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
File::create (
	const sl::StringRef_w& fileName,
	uint_t accessMode,
	uint_t shareMode,
	SECURITY_ATTRIBUTES* secAttr,
	uint_t creationDisposition,
	uint_t flagsAttributes
	)
{
	close ();

	m_h = ::CreateFileW (
		fileName.sz (),
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
		return err::failWithLastSystemError <uint64_t> (-1);

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

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
