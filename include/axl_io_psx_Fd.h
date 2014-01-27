// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PSX_FD_H

#include "axl_rtl_Handle.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class CCloseFd
{
public:
	void
	operator () (int h)
	{
		::close (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef rtl::CHandleT <int, CCloseFd, rtl::CMinusOneT <int> > CFd;

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
