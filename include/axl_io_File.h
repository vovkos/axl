// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILE_H

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_io_win_File.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_io_psx_File.h"
#	include "axl_sl_String.h"
#endif

namespace axl {
namespace io {

//.............................................................................

enum FileFlag
{
	FileFlag_ReadOnly      = 0x01,
	FileFlag_OpenExisting  = 0x02,
	FileFlag_Exclusive     = 0x04,
	FileFlag_ShareWrite    = 0x08,
	FileFlag_DeleteOnClose = 0x10,
	FileFlag_Asynchronous  = 0x20,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class File
{
public:
#if (_AXL_ENV == AXL_ENV_WIN)
	typedef win::File FileImpl;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	typedef psx::File FileImpl;
#endif
	
	typedef FileImpl::T Handle;
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
		const char* fileName,
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

//.............................................................................

class TemporaryFile: public File
{
#if (_AXL_ENV == AXL_ENV_POSIX)
protected:
	sl::String m_fileName;

public:
	~TemporaryFile ()
	{
		close ();
	}
#endif

public:
#if (_AXL_ENV == AXL_ENV_POSIX)
	void 
	close ();

	bool
	open (
		const char* fileName,
		uint_t flags = 0
		);
#else
	bool
	open (
		const char* fileName,
		uint_t flags = 0
		)
	{
		return io::File::open (fileName, flags | FileFlag_DeleteOnClose);
	}
#endif
};

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

inline
bool
renameFile (
	const char* srcFileName,
	const char* dstFileName
	)
{
	bool_t result = ::MoveFileA (srcFileName, dstFileName);
	return err::complete (result);
}

inline
bool
deleteFile (const char* fileName)
{
	bool_t result = ::DeleteFileA (fileName);
	return err::complete (result);
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

inline
bool
renameFile (
	const char* srcFileName,
	const char* dstFileName
	)
{
	int result = rename (srcFileName, dstFileName);
	return err::complete (result == 0);
}

inline
bool
deleteFile (const char* fileName)
{
	int result = unlink (fileName);
	return err::complete (result == 0);
}

#endif

uint64_t
copyFile (
	const char* srcFileName,
	const char* dstFileName,
	uint64_t size = -1
	);

//.............................................................................

} // namespace io
} // namespace axl
