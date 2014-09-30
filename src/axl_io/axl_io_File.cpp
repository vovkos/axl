#include "pch.h"
#include "axl_io_File.h"
#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

bool
CFile::open (
	const char* fileName,
	uint_t flags
	)
{
	uint_t accessMode = (flags & EFileFlag_ReadOnly) ?
		GENERIC_READ :
		GENERIC_READ | GENERIC_WRITE;

	uint_t shareMode = (flags & EFileFlag_Exclusive) ?
		0 :
		(flags & EFileFlag_ReadOnly) || (flags & EFileFlag_ShareWrite) ?
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE :
			FILE_SHARE_READ;

	uint_t creationDisposition = (flags & (EFileFlag_ReadOnly | EFileFlag_OpenExisting)) ?
		OPEN_EXISTING :
		OPEN_ALWAYS;

	uint_t flagsAttributes = (flags & EFileFlag_DeleteOnClose) ?
		FILE_FLAG_DELETE_ON_CLOSE :
		0;

	if (flags & EFileFlag_Asynchronous)
		flagsAttributes |= FILE_FLAG_OVERLAPPED;

	char buffer [256];
	rtl::CString_w fileName_w (ref::EBuf_Stack, buffer, sizeof (buffer));
	fileName_w = fileName;

	return m_file.create (
		fileName_w,
		accessMode,
		shareMode,
		NULL,
		creationDisposition,
		flagsAttributes
		);
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

uint_t
getPosixOpenFlags (uint_t fileFlags)
{
	uint_t posixFlags = (fileFlags & EFileFlag_ReadOnly) ? O_RDONLY : O_RDWR;

	if (!(fileFlags & (EFileFlag_ReadOnly | EFileFlag_OpenExisting)))
		posixFlags |= O_CREAT;

	if (fileFlags & EFileFlag_Asynchronous)
		posixFlags |= O_NONBLOCK;

	return posixFlags;
}

bool
CFile::open (
	const char* fileName,
	uint_t flags
	)
{
	uint_t posixFlags = getPosixOpenFlags (flags);

	// TODO: handle exclusive and share write flags with fcntl locks

	bool result = m_file.open (fileName, posixFlags);
	if (!result)
		return false;

	// if (Flags & EFileFlag_DeleteOnClose)
	//	unlink (pFileName);

	return true;
}

#endif

size_t
CFile::writeFormat_va (
	const char* formatString,
	axl_va_list va
	)
{
	char buffer [256];
	rtl::CString string (ref::EBuf_Stack, buffer, sizeof (buffer));
	string.format_va (formatString, va);

	return write (string, string.getLength ());
}

//.............................................................................

} // namespace io
} // namespace axl
