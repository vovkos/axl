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

#define _AXL_GUI_GDIOBJECTHANDLE_H

#include "axl_gui_GdiPch.h"

namespace axl {
namespace gui {

//..............................................................................

class DeleteGdiObject
{
public:
	void
	operator () (HGDIOBJ h)
	{
		::DeleteObject(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class GdiObjectHandle: public sl::Handle<T, DeleteGdiObject>
{
public:
	GdiObjectHandle()
	{
	}

	GdiObjectHandle(T h):
		sl::Handle<T, DeleteGdiObject> (h)
	{
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
