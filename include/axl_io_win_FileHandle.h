// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_WIN_HANDLE_H

#include "axl_sys_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class FileHandle: public sl::Handle <HANDLE, sys::win::CloseHandle, sl::MinusOne <HANDLE> >
{
public:
	static
	bool
	completeAsyncRequest (
		bool_t result,
		OVERLAPPED* overlapped
		);

	size_t
	getOverlappedResult (OVERLAPPED* overlapped);
};

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
