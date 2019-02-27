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

#define _AXL_SYS_WIN_HANDLE_H

#include "axl_sl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class CloseHandle
{
public:
	void
	operator () (HANDLE h)
	{
		::CloseHandle(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle<HANDLE, CloseHandle> Handle;

//..............................................................................

class CloseRegKey
{
public:
	void
	operator () (HKEY h)
	{
		::RegCloseKey(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle<HKEY, CloseRegKey, sl::MinusOne<HKEY> > RegKeyHandle;

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
