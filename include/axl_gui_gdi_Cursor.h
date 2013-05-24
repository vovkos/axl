// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDI_CURSOR_H

#include "axl_gui_Cursor.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {
namespace gdi {

//.............................................................................

class CCursor: 
	public ICursor,
	public rtl::CHandleT <HCURSOR>
{
	friend class CEngine;

public:
	AXL_OBJ_CLASS_0 (CCursor, ICursor);

	CCursor ();
};

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl

