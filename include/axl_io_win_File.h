// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_FILE_H

#include "axl_g_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class CFile: public g::win::CFileHandle
{
public:
	bool
	Create (
		const wchar_t* pFileName, 
		uint_t AccessMode,
		uint_t ShareMode,
		SECURITY_ATTRIBUTES* pSecAttr,
		uint_t CreationDisposition,
		uint_t FlagsAttributes = 0
		)
	{
		Close ();

		m_h = ::CreateFileW (
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
	GetSize () const;

	bool 
	SetSize (uint64_t Size);

	uint64_t 
	GetPosition () const;

	bool 
	SetPosition (uint64_t Offset) const;

	bool 
	Read (
		void* p, 
		dword_t Size, 
		dword_t* pActualSize,
		OVERLAPPED* pOverlapped = NULL
		) const
	{
		bool_t Result = ::ReadFile (m_h, p, Size, pActualSize, pOverlapped);
		return err::Complete (Result);
	}

	bool 
	Write (
		const void* p, 
		dword_t Size,
		dword_t* pActualSize,
		OVERLAPPED* pOverlapped = NULL
		)
	{
		bool_t Result = ::WriteFile (m_h, p, Size, pActualSize, pOverlapped);
		return err::Complete (Result);
	}

	bool 
	ReadEx (
		void* p, 
		dword_t Size, 
		OVERLAPPED* pOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		) const
	{
		bool_t Result = ::ReadFileEx (m_h, p, Size, pOverlapped, pfOnComplete);
		return err::Complete (Result);
	}

	bool 
	WriteEx (
		const void* p, 
		dword_t Size,
		OVERLAPPED* pOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		)
	{
		bool_t Result = ::WriteFileEx (m_h, p, Size, pOverlapped, pfOnComplete);
		return err::Complete (Result);
	}
};

//.............................................................................
	
} // namespace win
} // namespace io {
} // namespace axl
