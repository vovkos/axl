// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_MAPPING_H

#include "axl_io_File.h"
#include "axl_win_Mapping.h"

namespace axl {
namespace io {

//.............................................................................

class CMapping
{
public:
	enum EMapping
	{
		EMapping_ReadOnly = 1
	};

protected:
	win::CMapping m_Mapping;

public:
	operator HANDLE ()
	{
		return m_Mapping;
	}

	bool
	IsOpen ()
	{
		return m_Mapping.IsOpen ();
	}

	void
	Close ()
	{
		m_Mapping.Close ();			 
	}

	bool 
	CreateFileMapping (
		io::CFile* pFile, 
		uint64_t MaxSize = 0, 
		int Flags = 0
		)
	{
		ulong_t PageProtection = (Flags & EMapping_ReadOnly) ? PAGE_READONLY : PAGE_READWRITE;
		return m_Mapping.Create (*pFile, NULL, PageProtection, MaxSize, NULL);
	}

	bool 
	CreateSharedMemory (
		const tchar_t* pName, 
		size_t Size
		)
	{
		return m_Mapping.Create (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, Size, pName);
	}

	bool 
	OpenSharedMemory (
		const tchar_t* pName, 
		int Flags = 0
		)
	{
		ulong_t Access = (Flags & EMapping_ReadOnly) ? FILE_MAP_READ : FILE_MAP_WRITE;
		return m_Mapping.Open (Access, false, pName);
	}
};

//.............................................................................

class CMappedView
{
public:
	enum EView
	{
		EView_ReadOnly = 1
	};

protected:
	win::CMappedView m_View;

public:
	operator void* ()
	{
		return m_View;
	}

	bool
	IsOpen ()
	{
		return m_View.IsOpen ();
	}

	void
	Close ()
	{
		m_View.Close ();			 
	}

	void* 
	View (
		CMapping* pMapping, 
		uint64_t Offset = 0, 
		size_t Size = 0, 
		int Flags = 0
		)
	{
		ulong_t Access = (Flags & EView_ReadOnly) ? FILE_MAP_READ : FILE_MAP_WRITE;
		return m_View.View (*pMapping, Access, Offset, (ulong_t) Size); 
	}
};

//.............................................................................

} // namespace io
} // namespace axl

