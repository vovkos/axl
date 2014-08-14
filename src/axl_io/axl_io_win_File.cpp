#include "pch.h"
#include "axl_io_win_File.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool
CFile::Create (
	const wchar_t* pFileName,
	uint_t AccessMode,
	uint_t ShareMode,
	SECURITY_ATTRIBUTES* pSecAttr,
	uint_t CreationDisposition,
	uint_t FlagsAttributes
	)
{
	Close ();

	m_h = ::CreateFileW (
		pFileName,
		AccessMode,
		ShareMode,
		pSecAttr,
		CreationDisposition,
		FlagsAttributes,
		NULL
		);

	return err::Complete (m_h != INVALID_HANDLE_VALUE);
}

uint64_t
CFile::GetSize () const
{
	ULARGE_INTEGER Size;

	Size.LowPart = ::GetFileSize (m_h, &Size.HighPart);
	if (Size.LowPart == INVALID_FILE_SIZE)
		return err::FailWithLastSystemError (-1);

	return Size.QuadPart;
}

bool
CFile::SetSize (uint64_t Size)
{
	uint64_t Position = GetPosition ();

	bool_t Result = SetPosition (Size);
	if (!Result)
		return false;

	Result = ::SetEndOfFile (m_h);
	if (!Result)
		return err::FailWithLastSystemError ();

	SetPosition (Position);
	return true;
}

uint64_t
CFile::GetPosition () const
{
	LARGE_INTEGER Offset = { 0 };

	Offset.LowPart = ::SetFilePointer (m_h, 0, &Offset.HighPart, FILE_CURRENT);
	if (Offset.LowPart == INVALID_FILE_SIZE)
		return err::FailWithLastSystemError (-1);

	return Offset.QuadPart;
}

bool
CFile::SetPosition (uint64_t _Offset) const
{
	LARGE_INTEGER Offset;
	Offset.QuadPart = _Offset;

	dword_t Result = ::SetFilePointer (m_h, Offset.LowPart, &Offset.HighPart, FILE_BEGIN);
	return err::Complete (Result != INVALID_SET_FILE_POINTER);
}

size_t
CFile::Read (
	void* p,
	size_t Size
	) const
{
	dword_t ActualSize;
	bool_t Result = Read (p, (dword_t) Size, &ActualSize, NULL);
	return Result ? ActualSize : -1;
}

size_t
CFile::Write (
	const void* p,
	size_t Size
	)
{
	dword_t ActualSize;
	bool_t Result = Write (p, (dword_t) Size, &ActualSize, NULL);
	return Result ? ActualSize : -1;
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl

