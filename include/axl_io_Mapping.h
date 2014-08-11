// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_MAPPING_H

#include "axl_io_File.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_io_win_Mapping.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_io_psx_Mapping.h"
#	include "axl_rtl_String.h"
#endif

namespace axl {
namespace io {

//.............................................................................

class CMapping
{
protected:
	void* m_p;
	size_t m_Size;

#if (_AXL_ENV == AXL_ENV_WIN)
	win::CMapping m_Mapping;
	win::CMappedView m_View;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	psx::CMapping m_Mapping;
	psx::CSharedMemory m_SharedMemory;
	rtl::CString m_SharedMemoryName;
#endif

public:
	CMapping ()
	{
		m_p = NULL;
		m_Size = 0;
	}

	operator void* ()
	{
		return m_p;
	}

	bool
	IsOpen ()
	{
		return m_p != NULL;
	}

	void*
	p ()
	{
		return m_p;
	}

	size_t
	GetSize ()
	{
		return m_Size;
	}

	void*
	Open (
		CFile* pFile,
		uint64_t Offset,
		size_t Size,
		uint_t Flags = 0 // EFileFlag
		);

	void*
	Open (
		CFile* pFile,
		uint_t Flags = 0 // EFileFlag
		)
	{
		return Open (pFile, 0, -1, Flags);
	}

	void*
	Open (
		const char* pName,
		size_t Size,
		uint_t Flags = 0 // EFileFlag
		);

	void
	Close ();
};

//.............................................................................

} // namespace io
} // namespace axl
