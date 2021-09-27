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

#define _AXL_GUI_GDICURSOR_H

#include "axl_gui_GdiPch.h"
#include "axl_gui_Cursor.h"

namespace axl {
namespace gui {

//..............................................................................

class GdiCursor:
	public Cursor,
	public sl::Handle<HCURSOR> {
	friend class GdiEngine;

public:
	GdiCursor();
};

//..............................................................................

} // namespace gui
} // namespace axl
