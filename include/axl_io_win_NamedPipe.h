// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_NAMEDPIPE_H

#include "axl_g_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................
	
class NamedPipe: public g::win::FileHandle
{
public:
	bool 
	create (
		const wchar_t* name,
		uint_t openMode,
		uint_t pipeMode,
		dword_t maxInstanceCount,
		dword_t txBufferSize,
		dword_t rxBufferSize,
		uint_t timeout,
		SECURITY_ATTRIBUTES* secAttr
		);

	bool 
	open (
		const wchar_t* name,
		uint_t access = GENERIC_READ | GENERIC_WRITE,
		uint_t flags = FILE_FLAG_OVERLAPPED
		);

	bool 
	connect (OVERLAPPED* overlapped = NULL)
	{
		bool_t result = ::ConnectNamedPipe (m_h, overlapped);
		return completeAsyncRequest (result, overlapped);
	}

	bool 
	read (
		void* p, 
		dword_t size, 
		dword_t* actualSize,
		OVERLAPPED* overlapped = NULL
		) const
	{
		bool_t result = ::ReadFile (m_h, p, size, actualSize, overlapped);
		return completeAsyncRequest (result, overlapped);
	}

	bool 
	write (
		const void* p, 
		dword_t size,
		dword_t* actualSize,
		OVERLAPPED* overlapped = NULL
		)
	{
		bool_t result = ::WriteFile (m_h, p, size, actualSize, overlapped);
		return completeAsyncRequest (result, overlapped);
	}

	bool 
	readEx (
		void* p, 
		dword_t size, 
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		) const
	{
		bool_t result = ::ReadFileEx (m_h, p, size, overlapped, pfOnComplete);
		return completeAsyncRequest (result, overlapped);
	}

	bool 
	writeEx (
		const void* p, 
		dword_t size,
		OVERLAPPED* overlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE pfOnComplete
		)
	{
		bool_t result = ::WriteFileEx (m_h, p, size, overlapped, pfOnComplete);
		return completeAsyncRequest (result, overlapped);
	}
};

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
