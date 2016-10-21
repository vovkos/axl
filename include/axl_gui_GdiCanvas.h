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

#pragma once

#define _AXL_GUI_GDICANVAS_H

#include "axl_gui_GdiPch.h"
#include "axl_gui_Canvas.h"

namespace axl {
namespace gui {

//..............................................................................

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

//..............................................................................

class GdiCanvas:
	public Canvas,
	public sl::Handle <HDC>
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
		const sl::StringRef_utf8& text
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
		const sl::StringRef_utf16& text
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
		const sl::StringRef_utf32& text
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

//..............................................................................

} // namespace gui
} // namespace axl
