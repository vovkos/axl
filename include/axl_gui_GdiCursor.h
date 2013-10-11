// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDICURSOR_H

#include "axl_gui_Cursor.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {

//.............................................................................

class CGdiCursor: 
	public CCursor,
	public rtl::CHandleT <HCURSOR>
{
	friend class CGdiEngine;

public:
	CGdiCursor ();
};

//.............................................................................

} // namespace gui
} // namespace axl

