// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QT_ENGINE_H

#include "axl_gui_Engine.h"
#include "axl_rtl_Singleton.h"

#include "axl_gui_qt_Cursor.h"
#include "axl_gui_qt_Font.h"
// #include "axl_gui_qt_Image.h"
// #include "axl_gui_qt_ImageList.h"
#include "axl_gui_qt_Painter.h"
#include "axl_gui_qt_Pixmap.h"
#include "axl_gui_qt_Widget.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

class CEngine: public IEngine
{
protected:
	ref::CPtrT <IFont> m_DefaultGuiFont;
	ref::CPtrT <IFont> m_DefaultMonospaceFont;
	ref::CPtrT <ICursor> m_StdCursorArray [EStdCursor__Count];	

public:
	CEngine ()
	{
		m_EngineKind = EEngine_Qt;
	}

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
		)
	{
		return CreateFont (CreateQtFont (pFaceName, PointSize, Flags));
	}

	ref::CPtrT <IFont>
	CreateFont (const QFont& QtFont);
	
	virtual
	ICursor*
	GetStdCursor (EStdCursor CursorKind);

	ref::CPtrT <ICursor>
	CreateCursor (const QCursor& QtCursor);
	
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
	QFont
	CreateQtFont (
		const char* pFaceName,
		size_t PointSize,
		uint_t Flags
		);
};

//.............................................................................

} // namespace qt
} // namespace gui
} // namespace axl
