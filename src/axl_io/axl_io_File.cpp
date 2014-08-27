#include "pch.h"
#include "axl_io_File.h"
#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

bool
CFile::Open (
	const char* pFileName,
	uint_t Flags
	)
{
	uint_t AccessMode = (Flags & EFileFlag_ReadOnly) ?
		GENERIC_READ :
		GENERIC_READ | GENERIC_WRITE;

	uint_t ShareMode = (Flags & EFileFlag_Exclusive) ?
		0 :
		(Flags & EFileFlag_ReadOnly) || (Flags & EFileFlag_ShareWrite) ?
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE :
			FILE_SHARE_READ;

	uint_t CreationDisposition = (Flags & (EFileFlag_ReadOnly | EFileFlag_OpenExisting)) ?
		OPEN_EXISTING :
		OPEN_ALWAYS;

	uint_t FlagsAttributes = (Flags & EFileFlag_DeleteOnClose) ?
		FILE_FLAG_DELETE_ON_CLOSE :
		0;

	if (Flags & EFileFlag_Asynchronous)
		FlagsAttributes |= FILE_FLAG_OVERLAPPED;

	char Buffer [256];
	rtl::CString_w FileName (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	FileName = pFileName;

	return m_File.Create (
		FileName,
		AccessMode, 
		ShareMode, 
		NULL, 
		CreationDisposition, 
		FlagsAttributes
		);
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

uint_t
GetPosixOpenFlags (uint_t FileFlags)
{
	uint_t PosixFlags = (FileFlags & EFileFlag_ReadOnly) ? O_RDONLY : O_RDWR;

	if (!(FileFlags & EFileFlag_OpenExisting))
		PosixFlags |= O_CREAT;

	if (FileFlags & EFileFlag_Asynchronous)
		PosixFlags |= O_NONBLOCK;

	return PosixFlags;
}

bool
CFile::Open (
	const char* pFileName,
	uint_t Flags
	)
{
	uint_t PosixFlags = GetPosixOpenFlags (Flags);

	// TODO: handle exclusive and share write flags with fcntl locks

	bool Result = m_File.Open (pFileName, PosixFlags);
	if (!Result)
		return false;

	if (Flags & EFileFlag_DeleteOnClose)
		unlink (pFileName);

	return true;
}

#endif

size_t
CFile::WriteFormat_va (
	const char* pFormat,
	axl_va_list va
	)
{
	char Buffer [256];
	rtl::CString String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Format_va (pFormat, va);

	return Write (String, String.GetLength ());
}

//.............................................................................

} // namespace io
} // namespace axl
