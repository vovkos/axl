// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_CURSOR_H

#include "axl_gui_Def.h"

namespace axl {
namespace gui {

//.............................................................................

enum EStdCursor
{
	EStdCursor_Arrow = 0,
	EStdCursor_Wait,
	EStdCursor_IBeam,
	EStdCursor_Hyperlink,
	EStdCursor_SizeNS,
	EStdCursor_SizeWE,
	EStdCursor_SizeNWSE,
	EStdCursor_SizeNESW,
	EStdCursor_SizeAll,
	
	EStdCursor__Count
};

//.............................................................................

struct ICursor: IGuiItem
{
};

//.............................................................................

} // namespace gui
} // namespace axl

