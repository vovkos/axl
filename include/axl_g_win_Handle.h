// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_WIN_HANDLE_H

#include "axl_rtl_Handle.h"
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

typedef rtl::Handle <HANDLE, CloseHandle> Handle;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class FileHandle: public rtl::Handle <HANDLE, CloseHandle, rtl::MinusOne <HANDLE> >
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

typedef rtl::Handle <HKEY, CloseRegKey, rtl::MinusOne <HKEY> > RegKeyHandle;

//.............................................................................

} // namespace win
} // namespace g
} // namespace axl
