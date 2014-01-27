// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILE_H

#include "axl_g_Def.h"

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

//.............................................................................

} // namespace io
} // namespace axl

#if (_AXL_ENV == AXL_ENV_WIN)

// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILE_H

#include "axl_io_win_File.h"

namespace axl {
namespace io {

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CFile
{
public:
	win::CFile m_File;

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
		dword_t ActualSize;
		bool Result = m_File.Read (p, (dword_t) Size, &ActualSize);
		return Result ? ActualSize : -1;
	}

	size_t
	Write (
		const void* p,
		size_t Size
		)
	{
		dword_t ActualSize;
		bool Result = m_File.Write (p, (dword_t) Size, &ActualSize);
		return Result ? ActualSize : -1;
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

} // namespace io
} // namespace axl

#elif (_AXL_ENV == AXL_ENV_POSIX)

// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILE_PSX_H

#include "axl_io_psx_File.h"

namespace axl {
namespace io {

//.............................................................................

class CFile
{
public:
	psx::CFile m_File;

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

} // namespace io
} // namespace axl


#endif
