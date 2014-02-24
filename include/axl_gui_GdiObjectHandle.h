// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIOBJECTHANDLE_H

#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {

//.............................................................................

class CDeleteGdiObject
{
public:
	void
	operator () (HGDIOBJ h)
	{
		::DeleteObject (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CGdiObjectHandleT: public rtl::CHandleT <T, CDeleteGdiObject>
{
public:
	CGdiObjectHandleT ()
	{
	}

	CGdiObjectHandleT (T h):
		rtl::CHandleT <T, CDeleteGdiObject> (h)
	{
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
