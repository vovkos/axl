//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_WIN_IOCP_H

#include "axl_io_win_Pch.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

class Iocp: public sys::win::Handle
{
public:
	bool
	create(dword_t threadCount);

	bool
	associate(
		HANDLE hIo,
		void* context
		)
	{
		HANDLE h = ::CreateIoCompletionPort(hIo, m_h, (uintptr_t)context, 0);
		return err::complete(h != NULL);
	}

	bool
	postCompletionStatus(
		dword_t actualSize,
		void* context,
		OVERLAPPED* overlapped
		)
	{
		bool_t result = ::PostQueuedCompletionStatus(m_h, actualSize, (uintptr_t)context, overlapped);
		return err::complete(result);
	}

	bool
	getCompletionStatus(
		dword_t* actualSize,
		void** context,
		OVERLAPPED** overlapped,
		uint_t timeout
		)
	{
		bool_t result = ::GetQueuedCompletionStatus(m_h, actualSize, (ULONG_PTR*)context, overlapped, timeout);
		return err::complete(result);
	}
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
