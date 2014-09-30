// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILE_H

#if (_AXL_ENV == AXL_ENV_WIN)
#include "axl_io_win_File.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#include "axl_io_psx_File.h"
#endif

namespace axl {
namespace io {

//.............................................................................

enum EFileFlag
{
	EFileFlag_ReadOnly      = 0x01,
	EFileFlag_OpenExisting  = 0x02,
	EFileFlag_Exclusive     = 0x04,
	EFileFlag_ShareWrite    = 0x08,
	EFileFlag_DeleteOnClose = 0x10,
	EFileFlag_Asynchronous  = 0x20,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CFile
{
public:
#if (_AXL_ENV == AXL_ENV_WIN)
	win::CFile m_file;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	psx::CFile m_file;
#endif

public:
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

	size_t
	writeFormat_va (
		const char* formatString,
		axl_va_list va
		);

	size_t
	writeFormat (
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return writeFormat_va (formatString, va);
	}

	bool
	flush ()
	{
		return m_file.flush ();
	}
};

//.............................................................................

#if (_AXL_ENV == AXL_ENV_POSIX)

uint_t
getPosixOpenFlags (uint_t fileFlags);

#endif

//.............................................................................

} // namespace io
} // namespace axl
