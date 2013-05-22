// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_WIN_HANDLE_H

#include "axl_rtl_Handle.h"

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
typedef rtl::CHandleT <HANDLE, CCloseHandle, rtl::CMinusOneT <HANDLE> > CFileHandle;

//.............................................................................

} // namespace win
} // namespace g
} // namespace axl
