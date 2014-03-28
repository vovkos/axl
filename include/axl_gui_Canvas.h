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
	CFont* m_pFont;
	TColorAttr m_ColorAttr;

public:
	CFont* m_pBaseFont;
	TTextAttr m_BaseTextAttr;
	TTextAttr m_DefTextAttr;
	TPalette m_Palette;

public:
	CCanvas ()
	{
		m_pFont = NULL;
		m_pBaseFont = NULL;
		m_BaseTextAttr.Setup (EStdPalColor_WidgetText, EStdPalColor_WidgetBack, 0);
	}

	// rect drawing

	virtual
	bool
	DrawRect (
		int Left,
		int Top,
		int Right,
		int Bottom,
		uint_t Color
		) = 0;

	bool
	DrawRect (
		int Left,
		int Top,
		int Right,
		int Bottom
		)
	{
		return DrawRect (Left, Top, Right, Bottom, m_DefTextAttr.m_BackColor);
	}

	bool
	DrawRect (
		const TRect& Rect,
		uint_t Color
		)
	{
		return DrawRect (
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom,
			Color
			);
	}

	bool
	DrawRect (const TRect& Rect)
	{
		return DrawRect (
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom,
			m_DefTextAttr.m_BackColor
			);
	}

	// default to utf8 

	bool
	DrawText (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		uint_t TextColor,
		uint_t BackColor,
		uint_t FontFlags,
		const char* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf8 (
			x,
			y,
			Left,
			Top,
			Right,
			Bottom,
			TextColor,
			BackColor,
			FontFlags,
			pText,
			Length
			);
	}

	bool
	DrawText (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		const char* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf8 (x, y, Left, Top, Right, Bottom, pText, Length);
	}

	bool
	DrawText (
		const TPoint& Point,
		const TRect& Rect,
		const TTextAttr& TextAttr,
		const char* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf8 (Point, Rect, TextAttr, pText, Length);
	}

	bool
	DrawText (
		const TPoint& Point,
		const TRect& Rect,
		const char* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf8 (Point, Rect, pText, Length);
	}

	// utf8 text drawing

	virtual
	bool
	DrawText_utf8 (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		uint_t TextColor,
		uint_t BackColor,
		uint_t FontFlags,
		const utf8_t* pText,
		size_t Length = -1
		) = 0;

	bool
	DrawText_utf8 (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		const utf8_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf8 (
			x,
			y,
			Left,
			Top,
			Right,
			Bottom,
			m_DefTextAttr.m_ForeColor,
			m_DefTextAttr.m_BackColor,
			m_DefTextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	bool
	DrawText_utf8 (
		const TPoint& Point,
		const TRect& Rect,
		const TTextAttr& TextAttr,
		const utf8_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf8 (
			Point.m_x,
			Point.m_y,
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom,
			TextAttr.m_ForeColor,
			TextAttr.m_BackColor,
			TextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	bool
	DrawText_utf8 (
		const TPoint& Point,
		const TRect& Rect,
		const utf8_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf8 (
			Point.m_x,
			Point.m_y,
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom,
			m_DefTextAttr.m_ForeColor,
			m_DefTextAttr.m_BackColor,
			m_DefTextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	// utf16 text drawing

	virtual
	bool
	DrawText_utf16 (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		uint_t TextColor,
		uint_t BackColor,
		uint_t FontFlags,
		const utf16_t* pText,
		size_t Length = -1
		) = 0;

	bool
	DrawText_utf16 (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		const utf16_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf16 (
			x,
			y,
			Left,
			Top,
			Right,
			Bottom,
			m_DefTextAttr.m_ForeColor,
			m_DefTextAttr.m_BackColor,
			m_DefTextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	bool
	DrawText_utf16 (
		const TPoint& Point,
		const TRect& Rect,
		const TTextAttr& TextAttr,
		const utf16_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf16 (
			Point.m_x,
			Point.m_y,
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom,
			TextAttr.m_ForeColor,
			TextAttr.m_BackColor,
			TextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	bool
	DrawText_utf16 (
		const TPoint& Point,
		const TRect& Rect,
		const utf16_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf16 (
			Point.m_x,
			Point.m_y,
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom,
			m_DefTextAttr.m_ForeColor,
			m_DefTextAttr.m_BackColor,
			m_DefTextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	// utf32 text drawing

	virtual
	bool
	DrawText_utf32 (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		uint_t TextColor,
		uint_t BackColor,
		uint_t FontFlags,
		const utf32_t* pText,
		size_t Length = -1
		) = 0;

	bool
	DrawText_utf32 (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		const utf32_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf32 (
			x,
			y,
			Left,
			Top,
			Right,
			Bottom,
			m_DefTextAttr.m_ForeColor,
			m_DefTextAttr.m_BackColor,
			m_DefTextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	bool
	DrawText_utf32 (
		const TPoint& Point,
		const TRect& Rect,
		const TTextAttr& TextAttr,
		const utf32_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf32 (
			Point.m_x,
			Point.m_y,
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom,
			TextAttr.m_ForeColor,
			TextAttr.m_BackColor,
			TextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	bool
	DrawText_utf32 (
		const TPoint& Point,
		const TRect& Rect,
		const utf32_t* pText,
		size_t Length = -1
		)
	{
		return DrawText_utf32 (
			Point.m_x,
			Point.m_y,
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom,
			m_DefTextAttr.m_ForeColor,
			m_DefTextAttr.m_BackColor,
			m_DefTextAttr.m_FontFlags,
			pText,
			Length
			);
	}

	// image drawing

	virtual
	bool
	DrawImage (
		int x,
		int y,
		CImage* pImage,
		int Left,
		int Top,
		int Right,
		int Bottom
		) = 0;

	bool
	DrawImage (
		const TPoint& Point,
		CImage* pImage,
		const TRect& Rect = TRect ()
		)
	{
		return DrawImage (
			Point.m_x,
			Point.m_y,
			pImage,
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom
			);
	}

	// bitblt

	virtual
	bool
	CopyRect (
		CCanvas* pSrcCanvas,
		int xDst,
		int yDst,
		int xSrc,
		int ySrc,
		int Width,
		int Height
		) = 0;

	bool
	CopyRect (
		CCanvas* pSrcCanvas,
		const TPoint& DstPoint,
		const TPoint& SrcPoint,
		const TSize& Size
		)
	{
		return CopyRect (
			pSrcCanvas,
			DstPoint.m_x,
			DstPoint.m_y,
			SrcPoint.m_x,
			SrcPoint.m_y,
			Size.m_Width,
			Size.m_Height
			);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl
