// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_FILE_H

#include "axl_win_File.h"

namespace axl {
namespace io {

//.............................................................................

class CFile
{
public:
	enum EFile
	{
		EFile_ReadOnly      = 0x01,
		EFile_OpenExisting  = 0x02,
		EFile_Exclusive     = 0x04,
		EFile_ShareWrite    = 0x08,
		EFile_DeleteOnClose = 0x10,
	};

public:
	win::CFile m_File;

public:
	operator HANDLE ()
	{ 
		return m_File;
	}

	bool
	IsOpen ()
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
		const tchar_t* pFileName, 
		int Flags = 0
		)
	{
		ulong_t AccessMode = (Flags & EFile_ReadOnly) ? 
			GENERIC_READ : 
			GENERIC_READ | GENERIC_WRITE;

		ulong_t ShareMode = (Flags & EFile_Exclusive) ? 
			0 : 
			(Flags & EFile_ShareWrite) ? 
				FILE_SHARE_READ | FILE_SHARE_WRITE : 
				FILE_SHARE_READ;

		ulong_t CreationDisposition = (Flags & (EFile_ReadOnly | EFile_OpenExisting)) ? 
			OPEN_EXISTING : 
			OPEN_ALWAYS;

		ulong_t FlagsAttributes = (Flags & EFile_DeleteOnClose) ? 
			FILE_FLAG_DELETE_ON_CLOSE :
			0;

		return m_File.Create (pFileName, AccessMode, ShareMode, NULL, CreationDisposition, FlagsAttributes);
	}

	uint64_t 
	GetSize ()
	{
		return m_File.GetSize ();
	}

	bool 
	SetSize (uint64_t Size)
	{
		return m_File.SetSize (Size);
	}

	uint64_t 
	GetPosition ()
	{
		return m_File.GetPosition ();
	}

	bool 
	SetPosition (uint64_t Offset)
	{
		return m_File.SetPosition (Offset);
	}

	size_t
	Read (
		void* p, 
		size_t Size
		)
	{
		ulong_t ActualSize;
		bool Result = m_File.Read (p, (ulong_t) Size, &ActualSize);
		return Result ? ActualSize : -1;
	}

	size_t
	Write (
		const void* p, 
		size_t Size
		)
	{
		ulong_t ActualSize;
		bool Result = m_File.Write (p, (ulong_t) Size, &ActualSize);
		return Result ? ActualSize : -1;
	}

	size_t
	ReadAt (
		uint64_t Offset, 
		void* p, 
		size_t Size
		)
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

