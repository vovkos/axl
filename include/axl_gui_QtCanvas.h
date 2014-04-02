// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTCANVAS_H

#include "axl_gui_Canvas.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {

//.............................................................................

class CQtCanvas:
	public CCanvas,
	public rtl::CHandleT <QPainter*, rtl::CCppDeleteT <QPainter> >
{
	friend class CQtEngine;

public:
	CQtCanvas ();

	virtual
	bool
	DrawRect (
		int Left,
		int Top,
		int Right,
		int Bottom,
		uint_t Color
		);

	bool
	DrawText_qt (
		int x,
		int y,
		int Left,
		int Top,
		int Right,
		int Bottom,
		uint_t TextColor,
		uint_t BackColor,
		uint_t FontFlags,
		const QString& String
		);

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
		);

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
		)
	{
		return DrawText_qt (
			x,
			y,
			Left,
			Top,
			Right,
			Bottom,
			TextColor,
			BackColor,
			FontFlags,
			QString ((const QChar*) pText, Length)
			);
	}

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
		);

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
		);

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
		);
};

//.............................................................................

} // namespace gui
} // namespace axl
