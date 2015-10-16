// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_WIN_HANDLE_H

#include "axl_sl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

class CloseHandle
{
public:
	void
	operator () (HANDLE h)
	{
		::CloseHandle (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle <HANDLE, CloseHandle> Handle;

//.............................................................................

class CloseRegKey
{
public:
	void
	operator () (HKEY h)
	{
		::RegCloseKey (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle <HKEY, CloseRegKey, sl::MinusOne <HKEY> > RegKeyHandle;

//.............................................................................

class CloseServiceHandle
{
public:
	void
	operator () (SC_HANDLE h)
	{
		::CloseServiceHandle (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle <SC_HANDLE, CloseServiceHandle> ServiceHandle;

//.............................................................................

} // namespace win
} // namespace sys
} // namespace axl
