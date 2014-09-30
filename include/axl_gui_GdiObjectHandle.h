// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIOBJECTHANDLE_H

#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {

//.............................................................................

class DeleteGdiObject
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
class GdiObjectHandle: public rtl::Handle <T, DeleteGdiObject>
{
public:
	GdiObjectHandle ()
	{
	}

	GdiObjectHandle (T h):
		rtl::Handle <T, DeleteGdiObject> (h)
	{
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
