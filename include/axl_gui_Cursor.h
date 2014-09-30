// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_CURSOR_H

#include "axl_gui_Def.h"

namespace axl {
namespace gui {

//.............................................................................

enum StdCursorKind
{
	StdCursorKind_Arrow = 0,
	StdCursorKind_Wait,
	StdCursorKind_IBeam,
	StdCursorKind_Hyperlink,
	StdCursorKind_SizeNS,
	StdCursorKind_SizeWE,
	StdCursorKind_SizeNWSE,
	StdCursorKind_SizeNESW,
	StdCursorKind_SizeAll,
	
	StdCursorKind__Count
};

//.............................................................................

class Cursor: public GuiItem
{
};

//.............................................................................

} // namespace gui
} // namespace axl

