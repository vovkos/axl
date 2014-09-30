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
	drawRect (
		int left,
		int top,
		int right,
		int bottom,
		uint_t color
		);

	bool
	drawText_qt (
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const QString& string
		);

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
		);

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
		)
	{
		return drawText_qt (
			x,
			y,
			left,
			top,
			right,
			bottom,
			textColor,
			backColor,
			fontFlags,
			QString ((const QChar*) text, length)
			);
	}

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
		);

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
		);

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
		);
};

//.............................................................................

} // namespace gui
} // namespace axl
