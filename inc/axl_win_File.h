// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_FILE_H

#include "axl_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................

class CFile: public CFileHandle
{
public:
	bool
	Create (
		const tchar_t* pFileName, 
		ulong_t AccessMode,
		ulong_t ShareMode,
		SECURITY_ATTRIBUTES* pSecAttr,
		ulong_t CreationDisposition,
		ulong_t FlagsAttributes
		)
	{
		Close ();

		m_h = ::CreateFile (
			pFileName, 
			AccessMode,
			ShareMode,
			pSecAttr,
			CreationDisposition,
			FlagsAttributes, 
			NULL
			);

		return err::Complete (m_h != INVALID_HANDLE_VALUE);
	}

	uint64_t 
	GetSize ();

	bool 
	SetSize (uint64_t Size);

	uint64_t 
	GetPosition ();

	bool 
	SetPosition (uint64_t Offset);

	bool 
	Read (
		void* p, 
		ulong_t Size, 
		ulong_t* pActualSize,
		OVERLAPPED* pOverlapped = NULL
		)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::ReadFile (m_h, p, Size, pActualSize, pOverlapped);
		return err::Complete (Result);
	}

	bool 
	Write (
		const void* p, 
		ulong_t Size,
		ulong_t* pActualSize,
		OVERLAPPED* pOverlapped = NULL
		)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::WriteFile (m_h, p, (ulong_t) Size, pActualSize, pOverlapped);
		return err::Complete (Result);
	}

	bool 
	ReadEx (
		void* p, 
		ulong_t Size, 
		OVERLAPPED* pOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
		)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::ReadFileEx (m_h, p, Size, pOverlapped, pfnOnComplete);
		return err::Complete (Result);
	}

	bool 
	WriteEx (
		const void* p, 
		ulong_t Size,
		OVERLAPPED* pOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfnOnComplete
		)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::WriteFileEx (m_h, p, (ulong_t) Size, pOverlapped, pfnOnComplete);
		return err::Complete (Result);
	}

	static 
	bool 
	DoesFileExist (const tchar_t* pFileName)
	{ 
		return ::GetFileAttributes (pFileName) != INVALID_FILE_ATTRIBUTES;
	}
};

//.............................................................................

} // namespace win
} // namespace axl
