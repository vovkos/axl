// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
		);

	uint64_t
	GetSize () const;

	bool
	SetSize (uint64_t Size);

	uint64_t
	GetPosition () const;

	bool
	SetPosition (uint64_t Offset) const;

	bool
	Flush ()
	{
		bool_t Result = ::FlushFileBuffers (m_h);
		return err::Complete (Result);
	}

	size_t
	Read (
		void* p,
		size_t Size
		) const;

	size_t
	Write (
		const void* p,
		size_t Size
		);

	bool
	Read (
		void* p,
		dword_t Size,
		dword_t* pActualSize,
		OVERLAPPED* pOverlapped = NULL
		) const
	{
		bool_t Result = ::ReadFile (m_h, p, Size, pActualSize, pOverlapped);
		return CompleteAsyncRequest (Result, pOverlapped);
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
		return CompleteAsyncRequest (Result, pOverlapped);
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
		return CompleteAsyncRequest (Result, pOverlapped);
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
		return CompleteAsyncRequest (Result, pOverlapped);
	}
};

//.............................................................................

} // namespace win
} // namespace io {
} // namespace axl
