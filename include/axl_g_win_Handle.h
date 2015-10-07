// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_WIN_HANDLE_H

#include "axl_sl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace g {
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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class FileHandle: public sl::Handle <HANDLE, CloseHandle, sl::MinusOne <HANDLE> >
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

} // namespace win
} // namespace g
} // namespace axl
