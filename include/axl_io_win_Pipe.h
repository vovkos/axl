// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_PIPE_H

#include "axl_g_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................
	
class CPipe: public g::win::CFileHandle
{
public:
	bool 
	Create (
		const wchar_t* pName,
		uint_t OpenMode,
		uint_t PipeMode,
		dword_t MaxInstanceCount,
		dword_t TxBufferSize,
		dword_t RxBufferSize,
		uint_t Timeout,
		SECURITY_ATTRIBUTES* pSecAttr
		);

	bool 
	Open (
		const wchar_t* pName,
		uint_t Access = GENERIC_READ | GENERIC_WRITE,
		uint_t Flags = FILE_FLAG_OVERLAPPED
		);

	bool 
	Connect (OVERLAPPED* pOverlapped = NULL)
	{
		bool_t Result = ::ConnectNamedPipe (m_h, pOverlapped);
		return err::Complete (Result);
	}

	bool 
	Read (
		void* p, 
		dword_t Size, 
		dword_t* pActualSize,
		OVERLAPPED* pOverlapped = NULL
		)
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
		)
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
} // namespace io
} // namespace axl
