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
#include "axl_io_File.h"
#include "axl_io_Mapping.h"

namespace axl {
namespace io {

//..............................................................................

#if (_AXL_OS_WIN)

bool
File::open (
	const sl::StringRef& fileName,
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

	bool result = m_file.create (
		fileName_w,
		accessMode,
		shareMode,
		NULL,
		creationDisposition,
		flagsAttributes
		);

	if (!result)
		return false;

	if (flags & FileFlag_Clear)
		m_file.setSize (0);

	return true;
}

#elif (_AXL_OS_POSIX)

bool
File::open (
	const sl::StringRef& fileName,
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

	bool result = m_file.open (fileName.sz (), posixFlags);
	if (!result)
		return false;

	if (flags & FileFlag_Unlink)
		::unlink (fileName.sz ());

	if (flags & FileFlag_Clear)
		m_file.setSize (0);

	return true;
}

#endif

//..............................................................................

uint64_t
copyFile (
	const sl::StringRef& srcFileName,
	const sl::StringRef& dstFileName,
	uint64_t size
	)
{
	File srcFile;
	bool result = srcFile.open (srcFileName, FileFlag_ReadOnly);
	if (!result)
		return -1;

	return copyFile (&srcFile, dstFileName, size);
}

uint64_t
copyFile (
	const io::File* srcFile,
	const sl::StringRef& dstFileName,
	uint64_t size
	)
{
	File dstFile;
	bool result = dstFile.open (dstFileName);
	if (!result)
		return -1;

	return copyFile (srcFile, &dstFile, size);
}

uint64_t
copyFile (
	const io::File* srcFile,
	io::File* dstFile,
	uint64_t size
	)
{
	enum
	{
		BaseBlockSize = 64 * 1024, // 64K
	};

	const g::SystemInfo* systemInfo = g::getModule ()->getSystemInfo ();
	size_t blockSize = BaseBlockSize + systemInfo->m_mappingAlignFactor - BaseBlockSize % systemInfo->m_mappingAlignFactor;

	uint64_t offset = 0;

	if (size == -1)
		size = srcFile->getSize ();

	bool result = dstFile->setSize (size);
	if (!result)
		return -1;

#if (_AXL_OS_WIN)
	win::Mapping srcMapping;
	win::Mapping dstMapping;
	win::MappedView srcView;
	win::MappedView dstView;

	bool result =
		srcMapping.create (srcFile->m_file, NULL, PAGE_READONLY, size) &&
		dstMapping.create (dstFile->m_file, NULL, PAGE_READWRITE, size);

	if (!result)
		return -1;

	while (size)
	{
		if (blockSize > size)
			blockSize = (size_t) size;

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

	while (size)
	{
		if (blockSize > size)
			blockSize = (size_t) size;

		const void* src = srcMapping.map (NULL, blockSize, PROT_READ, MAP_SHARED, srcFile->m_file, offset);
		void* dst = dstMapping.map (NULL, blockSize, PROT_READ | PROT_WRITE, MAP_SHARED, dstFile->m_file, offset);

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

//..............................................................................

} // namespace io
} // namespace axl
