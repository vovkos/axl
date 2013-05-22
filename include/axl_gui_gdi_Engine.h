// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDI_ENGINE_H

#include "axl_gui_Engine.h"
#include "axl_gui_gdi_Dc.h"
#include "axl_gui_gdi_Font.h"
#include "axl_gui_gdi_Cursor.h"
#include "axl_gui_gdi_Bitmap.h"
#include "axl_gui_gdi_ImageList.h"
#include "axl_gui_gdi_Widget.h"
#include "axl_rtl_Singleton.h"

namespace axl {
namespace gui {
namespace gdi {

//.............................................................................

class CEngine: public IEngine
{
public:
	AXL_OBJ_CLASS_0 (CEngine, IEngine);

protected:
	ref::CPtrT <IFont> m_DefaultGuiFont;
	ref::CPtrT <IFont> m_DefaultMonospaceFont;
	ref::CPtrT <ICursor> m_StdCursorArray [EStdCursor__Count];	
	HWND m_hWndClipboardOwner;

public:
	CEngine ()
	{
		m_EngineKind = EEngine_Gdi;
		m_hWndClipboardOwner = NULL;
	}

	~CEngine ();

	static
	CEngine*
	GetSingleton ()
	{
		return rtl::GetSingleton <CEngine> ();
	}

	IFont* 
	GetDefaultGuiFont ();

	virtual
	IFont*
	GetDefaultFont ()
	{
		return GetDefaultGuiFont ();
	}

	virtual
	IFont*
	GetDefaultMonospaceFont ();

	virtual
	ref::CPtrT <IFont>
	CreateFont (
		const char* pFaceName,
		size_t PointSize = 0,
		uint_t Flags = 0
		);

	ref::CPtrT <IFont>
	CreateStockFont (int StockFontKind);

	ref::CPtrT <IFont>
	CreateFont (HFONT hFont);

	ref::CPtrT <ICursor>
	CreateStockCursor (LPCTSTR pStockCursorRes);

	virtual
	ICursor*
	GetStdCursor (EStdCursor CursorKind);

	virtual
	IFont*
	GetFontMod (
		IFont* pBaseFont,
		uint_t Flags
		);

	virtual
	ref::CPtrT <IImage>
	CreateImage (
		int Width,
		int Height,
		EPixelFormat PixelFormat,
		const void* pData,
		bool IsScreenCompatible = true
		);

	virtual
	ref::CPtrT <IImageList>
	CreateImageList (
		int Width,
		int Height
		);

	virtual
	ref::CPtrT <IImageList>
	CreateImageList (
		IImage* pStipImage,
		int Width = 0
		);

	virtual
	ref::CPtrT <ICanvas>
	CreateOffscreenCanvas (
		int Width,
		int Height
		);

	virtual
	bool
	ReadClipboard (rtl::CString* pString);

	virtual
	bool
	WriteClipboard (
		const char* pString,
		size_t Length = -1
		);

protected:
	bool
	OpenClipboard ();
};

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl
