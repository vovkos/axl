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

#pragma once

#define _AXL_IO_FILE_H

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_File.h"
#elif (_AXL_OS_POSIX)
#	include "axl_io_psx_File.h"
#endif

namespace axl {
namespace io {

//..............................................................................

enum FileFlag
{
	FileFlag_ReadOnly      = 0x01,
	FileFlag_WriteOnly     = 0x02,
	FileFlag_OpenExisting  = 0x04,
	FileFlag_Exclusive     = 0x08,
	FileFlag_ShareWrite    = 0x10,
	FileFlag_DeleteOnClose = 0x20,
	FileFlag_Asynchronous  = 0x40,
	FileFlag_Clear         = 0x80,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class File
{
public:
#if (_AXL_OS_WIN)
	typedef win::File FileImpl;
#elif (_AXL_OS_POSIX)
	typedef psx::File FileImpl;
#endif

	typedef FileImpl::H Handle;

public:
	FileImpl m_file;

public:
	static
	Handle
	getInvalidHandle ()
	{
		return FileImpl::getInvalidHandle ();
	}

	Handle
	getHandle () const
	{
		return m_file;
	}

	bool
	isOpen () const
	{
		return m_file.isOpen ();
	}

	void
	close ()
	{
		m_file.close ();
	}

	bool
	open (
		const sl::StringRef& fileName,
		uint_t flags = 0
		);

	uint64_t
	getSize () const
	{
		return m_file.getSize ();
	}

	bool
	setSize (uint64_t size)
	{
		return m_file.setSize (size);
	}

	uint64_t
	getPosition () const
	{
		return m_file.getPosition ();
	}

	bool
	setPosition (uint64_t offset) const
	{
		return m_file.setPosition (offset);
	}

	size_t
	read (
		void* p,
		size_t size
		) const
	{
		return m_file.read (p, size);
	}

	size_t
	write (
		const void* p,
		size_t size
		)
	{
		return m_file.write (p, size);
	}

	size_t
	readAt (
		uint64_t offset,
		void* p,
		size_t size
		) const
	{
		return setPosition (offset) ? read (p, size) : -1;
	}

	size_t
	writeAt	(
		uint64_t offset,
		const void* p,
		size_t size
		)
	{
		return setPosition (offset) ? write (p, size) : -1;
	}

	bool
	flush ()
	{
		return m_file.flush ();
	}
};

//..............................................................................

class TemporaryFile: public File
{
#if (_AXL_OS_POSIX)
protected:
	sl::String m_fileName;

public:
	~TemporaryFile ()
	{
		close ();
	}
#endif

public:
#if (_AXL_OS_POSIX)
	void
	close ();

	bool
	open (
		const sl::StringRef& fileName,
		uint_t flags = 0
		);
#else
	bool
	open (
		const sl::StringRef& fileName,
		uint_t flags = 0
		)
	{
		return io::File::open (fileName, flags | FileFlag_DeleteOnClose);
	}
#endif
};

//..............................................................................

#if (_AXL_OS_WIN)

inline
bool
renameFile (
	const sl::StringRef& srcFileName,
	const sl::StringRef& dstFileName
	)
{
	bool_t result = ::MoveFileA (srcFileName.sz (), dstFileName.sz ());
	return err::complete (result);
}

inline
bool
deleteFile (const sl::StringRef& fileName)
{
	bool_t result = ::DeleteFileA (fileName.sz ());
	return err::complete (result);
}

#elif (_AXL_OS_POSIX)

inline
bool
renameFile (
	const sl::StringRef& srcFileName,
	const sl::StringRef& dstFileName
	)
{
	int result = ::rename (srcFileName.sz (), dstFileName.sz ());
	return err::complete (result == 0);
}

inline
bool
deleteFile (const sl::StringRef& fileName)
{
	int result = ::unlink (fileName.sz ());
	return err::complete (result == 0);
}

#endif

uint64_t
copyFile (
	const sl::StringRef& srcFileName,
	const sl::StringRef& dstFileName,
	uint64_t size = -1
	);

uint64_t
copyFile (
	const io::File* srcFile,
	const sl::StringRef& dstFileName,
	uint64_t size = -1
	);

uint64_t
copyFile (
	const io::File* srcFile,
	io::File* dstFile,
	uint64_t size = -1
	);

//..............................................................................

} // namespace io
} // namespace axl
