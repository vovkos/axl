#include "pch.h"
#include "axl_io_File.h"
#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

bool
File::open (
	const char* fileName,
	uint_t flags
	)
{
	uint_t accessMode = (flags & FileFlag_ReadOnly) ?
		GENERIC_READ :
		GENERIC_READ | GENERIC_WRITE;

	uint_t shareMode = (flags & FileFlag_Exclusive) ?
		0 :
		(flags & FileFlag_ReadOnly) || (flags & FileFlag_ShareWrite) ?
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE :
			FILE_SHARE_READ;

	uint_t creationDisposition = (flags & (FileFlag_ReadOnly | FileFlag_OpenExisting)) ?
		OPEN_EXISTING :
		OPEN_ALWAYS;

	uint_t flagsAttributes = (flags & FileFlag_DeleteOnClose) ?
		FILE_FLAG_DELETE_ON_CLOSE :
		0;

	if (flags & FileFlag_Asynchronous)
		flagsAttributes |= FILE_FLAG_OVERLAPPED;

	char buffer [256];
	rtl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
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

bool
File::open (
	const char* fileName,
	uint_t flags
	)
{
	uint_t posixFlags = (flags & FileFlag_ReadOnly) ? O_RDONLY : O_RDWR;

	if (!(flags & (FileFlag_ReadOnly | FileFlag_OpenExisting)))
		posixFlags |= O_CREAT;

	if (flags & FileFlag_Asynchronous)
		posixFlags |= O_NONBLOCK;

	// TODO: handle exclusive and share write flags with fcntl locks

	bool result = m_file.open (fileName, posixFlags);
	if (!result)
		return false;

	if (flags & FileFlag_DeleteOnClose)
		unlink (fileName);

	return true;
}

#endif

size_t
File::writeFormat_va (
	const char* formatString,
	axl_va_list va
	)
{
	char buffer [256];
	rtl::String string (ref::BufKind_Stack, buffer, sizeof (buffer));
	string.format_va (formatString, va);

	return write (string, string.getLength ());
}

//.............................................................................

#if (_AXL_ENV == AXL_ENV_POSIX)

void 
TemporaryFile::close ()
{
	if (!isOpen ())
		return;

	File::close ();
	deleteFile (m_fileName);
	m_fileName.clear ();
}

bool
TemporaryFile::open (
	const char* fileName,
	uint_t flags = 0
	)
{
	close ();

	bool result = io::File::open (fileName, flags & ~FileFlag_DeleteOnClose);
	if (!result)
		return false;

	m_fileName = fileName;
	return true;
}

#endif

//.............................................................................

} // namespace io
} // namespace axl
