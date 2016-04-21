#include "pch.h"
#include "axl_io_File.h"
#include "axl_io_Mapping.h"
#include "axl_sl_String.h"

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
	uint_t accessMode = 
		(flags & FileFlag_ReadOnly) ? GENERIC_READ :
		(flags & FileFlag_WriteOnly) ? GENERIC_WRITE : GENERIC_READ | GENERIC_WRITE;

	uint_t shareMode = 
		(flags & FileFlag_Exclusive) ? 0 :
		(flags & (FileFlag_ReadOnly | FileFlag_ShareWrite)) ?
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
	sl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
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
	uint_t posixFlags = 
		(flags & FileFlag_ReadOnly) ? O_RDONLY : 
		(flags & FileFlag_WriteOnly) ? O_WRONLY : O_RDWR;

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
	uint_t flags
	)
{
	close ();

	bool result = File::open (fileName, flags & ~FileFlag_DeleteOnClose);
	if (!result)
		return false;

	m_fileName = fileName;
	return true;
}

#endif

//.............................................................................

uint64_t
copyFile (
	const char* srcFileName,
	const char* dstFileName,
	uint64_t size
	)
{
	File srcFile;
	File dstFile;

	bool result = 
		srcFile.open (srcFileName, FileFlag_ReadOnly) &&
		dstFile.open (dstFileName);

	if (!result)
		return -1;

	enum 
	{
		BlockSize = 32 * 1024, // 32 K
	};

	if (size == -1)
		size = srcFile.getSize ();

	uint64_t offset = 0;

#if (_AXL_ENV == AXL_ENV_WIN)
	win::Mapping srcMapping;
	win::Mapping dstMapping;
	win::MappedView srcView;
	win::MappedView dstView;

	result = 
		srcMapping.create (srcFile.m_file, NULL, PAGE_READONLY, size) &&
		dstMapping.create (dstFile.m_file, NULL, PAGE_READWRITE, size);

	while (size)
	{
		size_t blockSize = (size_t) AXL_MIN (BlockSize, size);

		const void* src = srcView.view (srcMapping, FILE_MAP_READ, offset, blockSize);
		void* dst = dstView.view (dstMapping, FILE_MAP_READ | FILE_MAP_WRITE, offset, blockSize);

		if (!src || !dst)
			return -1;

		memcpy (dst, src, blockSize);

		offset += blockSize;
		size -= blockSize;
	}

	srcMapping.close ();
	dstMapping.close ();
	srcView.close ();
	dstView.close ();
#else 
	psx::Mapping srcMapping;
	psx::Mapping dstMapping;

	result = dstFile.setSize (size);
	if (!result)
		return -1;

	while (size)
	{
		size_t blockSize = AXL_MIN (BlockSize, size);

		const void* src = srcMapping.map (NULL, blockSize, PROT_READ, MAP_SHARED, srcFile.m_file, offset);
		void* dst = dstMapping.map (NULL, blockSize, PROT_READ | PROT_WRITE, MAP_SHARED, dstFile.m_file, offset);

		if (!src || !dst)
			return -1;

		memcpy (dst, src, blockSize);

		offset += blockSize;
		size -= blockSize;
	}

	srcMapping.close ();
	dstMapping.close ();
#endif

	return offset;
}

//.............................................................................

} // namespace io
} // namespace axl
