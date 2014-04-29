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

class CCloseHandle
{
public:
	void
	operator () (HANDLE h)
	{
		::CloseHandle (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef rtl::CHandleT <HANDLE, CCloseHandle> CHandle;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CFileHandle: public rtl::CHandleT <HANDLE, CCloseHandle, rtl::CMinusOneT <HANDLE> >
{
public:
	static
	bool
	CompleteAsyncRequest (
		bool_t Result,
		OVERLAPPED* pOverlapped
		);

	size_t
	GetOverlappedResult (OVERLAPPED* pOverlapped);
};

//.............................................................................

class CCloseRegKey
{
public:
	void
	operator () (HKEY h)
	{
		::RegCloseKey (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef rtl::CHandleT <HKEY, CCloseRegKey, rtl::CMinusOneT <HKEY> > CRegKeyHandle;

//.............................................................................

} // namespace win
} // namespace g
} // namespace axl
