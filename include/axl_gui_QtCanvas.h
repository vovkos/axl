// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTCANVAS_H

#include "axl_gui_Canvas.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {

//.............................................................................

class CQtCanvas: 
	public ÑCanvas,
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

	virtual
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
	DrawImage (
		int x,
		int y,
		CImageList* pImageList,
		size_t Index
		);

	virtual
	bool
	CopyRect (
		ÑCanvas* pSrcCanvas,
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
