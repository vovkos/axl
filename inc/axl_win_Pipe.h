// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_PIPE_H

#include "axl_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................
	
class CPipe: public CFileHandle
{
public:
	bool 
	Create (
		const tchar_t* pName,
		ulong_t OpenMode,
		ulong_t PipeMode,
		ulong_t MaxInstanceCount,
		ulong_t TxBufferSize,
		ulong_t RxBufferSize,
		ulong_t Timeout,
		SECURITY_ATTRIBUTES* pSecAttr
		)
	{
		Close ();

		m_h = ::CreateNamedPipe(
			pName, 
			OpenMode, 
			PipeMode, 
			MaxInstanceCount, 
			TxBufferSize, 
			RxBufferSize, 
			Timeout, 
			pSecAttr
			);

		return err::Complete (m_h != INVALID_HANDLE_VALUE);
	}

	bool 
	Open (
		const tchar_t* pName,
		ulong_t Access = GENERIC_READ | GENERIC_WRITE,
		ulong_t Flags = FILE_FLAG_OVERLAPPED
		)
	{
		Close ();

		m_h = ::CreateFile (pName, Access, 0, NULL, OPEN_EXISTING, Flags, NULL);
		return err::Complete (m_h != INVALID_HANDLE_VALUE);
	}

	bool 
	Connect (OVERLAPPED* pOverlapped = NULL)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::ConnectNamedPipe (m_h, pOverlapped);
		return err::Complete (Result);
	}

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
};

//.............................................................................

} // namespace win
} // namespace axl
