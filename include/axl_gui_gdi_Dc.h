// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDI_DC_H

#include "axl_gui_Canvas.h"
#include "axl_gui_gdi_Bitmap.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {
namespace gdi {

//.............................................................................

class CScreenDc
{
protected:
	HDC m_hdc;

public:
	CScreenDc ()
	{
		m_hdc = ::GetDC (NULL);
	};

	~CScreenDc ()
	{
		::ReleaseDC (NULL, m_hdc);
	};

	operator HDC ()
	{
		return m_hdc;
	}
};

//.............................................................................

class CDc: 
	public ICanvas,
	public rtl::CHandleT <HDC>
{
	friend class CEngine;
	friend struct IGdiWidget;

protected:
	enum EDestruct
	{
		EDestruct_None = 0,
		EDestruct_DeleteDc,
		EDestruct_ReleaseDc
	};

protected:	
	EDestruct m_DestructKind;

	HWND m_hWnd;
	HDC m_hCompatibleDc;
	HBITMAP m_hBitmap;
	HBITMAP m_hPrevBitmap;
	HFONT m_hPrevFont;

public:
	CDc ();

	~CDc ()
	{
		Release ();
	}

	void
	Attach (
		HDC hdc,
		HWND hWnd,
		EDestruct DestructKind
		);

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
		IImage* pImage,
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
		IImageList* pImageList,
		size_t Index
		);

	virtual
	bool
	CopyRect (
		ICanvas* pSrcCanvas,
		int xDst,
		int yDst,
		int xSrc,
		int ySrc,
		int Width,
		int Height
		);

protected:
	void
	Release ();
};

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl
