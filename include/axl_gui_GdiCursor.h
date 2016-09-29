// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDICURSOR_H

#include "axl_gui_GdiPch.h"
#include "axl_gui_Cursor.h"

namespace axl {
namespace gui {

//.............................................................................

class GdiCursor: 
	public Cursor,
	public sl::Handle <HCURSOR>
{
	friend class GdiEngine;

public:
	GdiCursor ();
};

//.............................................................................

} // namespace gui
} // namespace axl

