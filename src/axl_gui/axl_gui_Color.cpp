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

#include "pch.h"
#include "axl_gui_Color.h"
#include "axl_sl_CallOnce.h"

namespace axl {
namespace gui {

//..............................................................................

uint_t
Palette::getColorRgb(uint_t color) {
	if (!(color & ColorFlag_Index))
		return color;

	size_t i = color & ColorFlag_IndexMask;
	color = i < m_count ? m_colorArray[i] : -1;

	if (color & ColorFlag_Index) { // allow two-staged index lookups
		i = color & ColorFlag_IndexMask;
		color = i < m_count ? m_colorArray[i] : -1;
	}

	return color;
}

//..............................................................................

} // namespace gui
} // namespace axl
