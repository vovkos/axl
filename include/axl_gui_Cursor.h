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
	// {402C6FE5-1D7A-4CC5-9D80-00CCC399383B}
	AXL_OBJ_INTERFACE (
		ICursor,
		0x402c6fe5, 0x1d7a, 0x4cc5, 0x9d, 0x80, 0x00, 0xcc, 0xc3, 0x99, 0x38, 0x3b
		)
		
};

//.............................................................................

} // namespace gui
} // namespace axl

