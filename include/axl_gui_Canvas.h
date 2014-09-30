// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_CANVAS_H

#include "axl_gui_Def.h"
#include "axl_gui_Color.h"
#include "axl_gui_Font.h"
#include "axl_gui_Image.h"

namespace axl {
namespace gui {

//.............................................................................

class CCanvas: public CGuiItem
{
protected:
	CFont* m_font;
	TColorAttr m_colorAttr;

public:
	CFont* m_baseFont;
	TTextAttr m_baseTextAttr;
	TTextAttr m_defTextAttr;
	TPalette m_palette;

public:
	CCanvas ()
	{
		m_font = NULL;
		m_baseFont = NULL;
		m_baseTextAttr.setup (EStdPalColor_WidgetText, EStdPalColor_WidgetBack, 0);
	}

	// rect drawing

	virtual
	bool
	drawRect (
		int left,
		int top,
		int right,
		int bottom,
		uint_t color
		) = 0;

	bool
	drawRect (
		int left,
		int top,
		int right,
		int bottom
		)
	{
		return drawRect (left, top, right, bottom, m_defTextAttr.m_backColor);
	}

	bool
	drawRect (
		const TRect& rect,
		uint_t color
		)
	{
		return drawRect (
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			color
			);
	}

	bool
	drawRect (const TRect& rect)
	{
		return drawRect (
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_defTextAttr.m_backColor
			);
	}

	// default to utf8 

	bool
	drawText (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const char* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (
			x,
			y,
			left,
			top,
			right,
			bottom,
			textColor,
			backColor,
			fontFlags,
			text,
			length
			);
	}

	bool
	drawText (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		const char* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (x, y, left, top, right, bottom, text, length);
	}

	bool
	drawText (
		const TPoint& point,
		const TRect& rect,
		const TTextAttr& textAttr,
		const char* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (point, rect, textAttr, text, length);
	}

	bool
	drawText (
		const TPoint& point,
		const TRect& rect,
		const char* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (point, rect, text, length);
	}

	// utf8 text drawing

	virtual
	bool
	drawText_utf8 (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const utf8_t* text,
		size_t length = -1
		) = 0;

	bool
	drawText_utf8 (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		const utf8_t* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (
			x,
			y,
			left,
			top,
			right,
			bottom,
			m_defTextAttr.m_foreColor,
			m_defTextAttr.m_backColor,
			m_defTextAttr.m_fontFlags,
			text,
			length
			);
	}

	bool
	drawText_utf8 (
		const TPoint& point,
		const TRect& rect,
		const TTextAttr& textAttr,
		const utf8_t* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			textAttr.m_foreColor,
			textAttr.m_backColor,
			textAttr.m_fontFlags,
			text,
			length
			);
	}

	bool
	drawText_utf8 (
		const TPoint& point,
		const TRect& rect,
		const utf8_t* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_defTextAttr.m_foreColor,
			m_defTextAttr.m_backColor,
			m_defTextAttr.m_fontFlags,
			text,
			length
			);
	}

	// utf16 text drawing

	virtual
	bool
	drawText_utf16 (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const utf16_t* text,
		size_t length = -1
		) = 0;

	bool
	drawText_utf16 (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		const utf16_t* text,
		size_t length = -1
		)
	{
		return drawText_utf16 (
			x,
			y,
			left,
			top,
			right,
			bottom,
			m_defTextAttr.m_foreColor,
			m_defTextAttr.m_backColor,
			m_defTextAttr.m_fontFlags,
			text,
			length
			);
	}

	bool
	drawText_utf16 (
		const TPoint& point,
		const TRect& rect,
		const TTextAttr& textAttr,
		const utf16_t* text,
		size_t length = -1
		)
	{
		return drawText_utf16 (
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			textAttr.m_foreColor,
			textAttr.m_backColor,
			textAttr.m_fontFlags,
			text,
			length
			);
	}

	bool
	drawText_utf16 (
		const TPoint& point,
		const TRect& rect,
		const utf16_t* text,
		size_t length = -1
		)
	{
		return drawText_utf16 (
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_defTextAttr.m_foreColor,
			m_defTextAttr.m_backColor,
			m_defTextAttr.m_fontFlags,
			text,
			length
			);
	}

	// utf32 text drawing

	virtual
	bool
	drawText_utf32 (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const utf32_t* text,
		size_t length = -1
		) = 0;

	bool
	drawText_utf32 (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		const utf32_t* text,
		size_t length = -1
		)
	{
		return drawText_utf32 (
			x,
			y,
			left,
			top,
			right,
			bottom,
			m_defTextAttr.m_foreColor,
			m_defTextAttr.m_backColor,
			m_defTextAttr.m_fontFlags,
			text,
			length
			);
	}

	bool
	drawText_utf32 (
		const TPoint& point,
		const TRect& rect,
		const TTextAttr& textAttr,
		const utf32_t* text,
		size_t length = -1
		)
	{
		return drawText_utf32 (
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			textAttr.m_foreColor,
			textAttr.m_backColor,
			textAttr.m_fontFlags,
			text,
			length
			);
	}

	bool
	drawText_utf32 (
		const TPoint& point,
		const TRect& rect,
		const utf32_t* text,
		size_t length = -1
		)
	{
		return drawText_utf32 (
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_defTextAttr.m_foreColor,
			m_defTextAttr.m_backColor,
			m_defTextAttr.m_fontFlags,
			text,
			length
			);
	}

	// image drawing

	virtual
	bool
	drawImage (
		int x,
		int y,
		CImage* image,
		int left,
		int top,
		int right,
		int bottom
		) = 0;

	bool
	drawImage (
		const TPoint& point,
		CImage* image,
		const TRect& rect = TRect ()
		)
	{
		return drawImage (
			point.m_x,
			point.m_y,
			image,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom
			);
	}

	// bitblt

	virtual
	bool
	copyRect (
		CCanvas* srcCanvas,
		int xDst,
		int yDst,
		int xSrc,
		int ySrc,
		int width,
		int height
		) = 0;

	bool
	copyRect (
		CCanvas* srcCanvas,
		const TPoint& dstPoint,
		const TPoint& srcPoint,
		const TSize& size
		)
	{
		return copyRect (
			srcCanvas,
			dstPoint.m_x,
			dstPoint.m_y,
			srcPoint.m_x,
			srcPoint.m_y,
			size.m_width,
			size.m_height
			);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
