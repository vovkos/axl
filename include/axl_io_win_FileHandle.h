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

#define _AXL_IO_WIN_HANDLE_H

#include "axl_io_win_Pch.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

class FileHandle: public sl::Handle <HANDLE, sys::win::CloseHandle, sl::MinusOne <HANDLE> >
{
public:
	static
	bool
	completeAsyncRequest (
		bool_t result,
		OVERLAPPED* overlapped
		);

	bool
	getOverlappedResult (
		OVERLAPPED* overlapped,
		dword_t* actualSize
		) const;

	size_t
	getOverlappedResult (OVERLAPPED* overlapped) const;
};

//..............................................................................

class StdOverlapped: public OVERLAPPED
{
public:
	sys::NotificationEvent m_completionEvent;

public:
	StdOverlapped ()
	{
		memset (this, 0, sizeof (OVERLAPPED));
		hEvent = m_completionEvent.m_event;
	}
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
