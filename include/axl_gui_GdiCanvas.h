// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDICANVAS_H

#include "axl_gui_Canvas.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {

//.............................................................................

class ScreenDc
{
protected:
	HDC m_hdc;

public:
	ScreenDc ()
	{
		m_hdc = ::GetDC (NULL);
	};

	~ScreenDc ()
	{
		::ReleaseDC (NULL, m_hdc);
	};

	operator HDC ()
	{
		return m_hdc;
	}
};

//.............................................................................

class GdiCanvas:
	public Canvas,
	public rtl::Handle <HDC>
{
	friend class GdiEngine;
	friend class GdiWidgetImpl;

protected:
	enum DestructKind
	{
		DestructKind_None = 0,
		DestructKind_DeleteDc,
		DestructKind_ReleaseDc
	};

protected:
	DestructKind m_destructKind;

	HWND m_hWnd;
	HDC m_hCompatibleDc;
	HBITMAP m_hBitmap;
	HBITMAP m_hPrevBitmap;
	HFONT m_hPrevFont;

public:
	GdiCanvas ();

	~GdiCanvas ()
	{
		release ();
	}

	void
	attach (
		HDC hdc,
		HWND hWnd,
		DestructKind destructKind
		);

	virtual
	bool
	drawRect (
		int left,
		int top,
		int right,
		int bottom,
		uint_t color
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
		);

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
		Image* image,
		int left,
		int top,
		int right,
		int bottom
		);

	virtual
	bool
	copyRect (
		Canvas* srcCanvas,
		int xDst,
		int yDst,
		int xSrc,
		int ySrc,
		int width,
		int height
		);

protected:
	void
	release ();
};

//.............................................................................

} // namespace gui
} // namespace axl
