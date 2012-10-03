// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_HANDLE_H

#include "axl_rtl_Handle.h"

namespace axl {
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

typedef rtl::CHandleT <HANDLE, NULL, CCloseHandle> CHandle;

typedef rtl::CHandleT <HANDLE, INVALID_HANDLE_VALUE, CCloseHandle> CFileHandle;

//.............................................................................

} // namespace win
} // namespace axl
