// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CFile
{
public:
#if (_AXL_ENV == AXL_ENV_WIN)
	win::CFile m_File;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	psx::CFile m_File;
#endif

public:
	bool
	IsOpen () const
	{
		return m_File.IsOpen ();
	}

	void
	Close ()
	{
		m_File.Close ();
	}

	bool
	Open (
		const char* pFileName,
		uint_t Flags = 0
		);

	uint64_t
	GetSize () const
	{
		return m_File.GetSize ();
	}

	bool
	SetSize (uint64_t Size)
	{
		return m_File.SetSize (Size);
	}

	uint64_t
	GetPosition () const
	{
		return m_File.GetPosition ();
	}

	bool
	SetPosition (uint64_t Offset) const
	{
		return m_File.SetPosition (Offset);
	}

	size_t
	Read (
		void* p,
		size_t Size
		) const
	{
		return m_File.Read (p, Size);
	}

	size_t
	Write (
		const void* p,
		size_t Size
		)
	{
		return m_File.Write (p, Size);
	}

	size_t
	ReadAt (
		uint64_t Offset,
		void* p,
		size_t Size
		) const
	{
		return SetPosition (Offset) ? Read (p, Size) : -1;
	}

	size_t
	WriteAt	(
		uint64_t Offset,
		const void* p,
		size_t Size
		)
	{
		return SetPosition (Offset) ? Write (p, Size) : -1;
	}
};

//.............................................................................

#if (_AXL_ENV == AXL_ENV_POSIX)

uint_t
GetPosixOpenFlags (uint_t FileFlags);

#endif

//.............................................................................

} // namespace io
} // namespace axl
