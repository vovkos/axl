// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTENGINE_H

#include "axl_gui_Engine.h"
#include "axl_rtl_Singleton.h"

#include "axl_gui_QtCanvas.h"
#include "axl_gui_QtCursor.h"
#include "axl_gui_QtFont.h"
#include "axl_gui_QtImage.h"
// #include "axl_gui_QtImageList.h"
#include "axl_gui_QtWidget.h"

namespace axl {
namespace gui {

//.............................................................................

class CQtEngine: public CEngine
{
protected:
	ref::CPtrT <CFont> m_DefaultGuiFont;
	ref::CPtrT <CFont> m_DefaultMonospaceFont;
	ref::CPtrT <CCursor> m_StdCursorArray [EStdCursor__Count];	

public:
	CQtEngine ()
	{
		m_EngineKind = EEngine_Qt;
	}

	static
	CQtEngine*
	GetSingleton ()
	{
		return rtl::GetSingleton <CQtEngine> ();
	}

	CFont* 
	GetDefaultGuiFont ();

	virtual
	CFont*
	GetDefaultFont ()
	{
		return GetDefaultGuiFont ();
	}

	virtual
	CFont*
	GetDefaultMonospaceFont ();

	virtual
	ref::CPtrT <CFont>
	CreateFont (
		const char* pFaceName,
		size_t PointSize = 0,
		uint_t Flags = 0
		)
	{
		return CreateFont (CreateQtFont (pFaceName, PointSize, Flags));
	}

	ref::CPtrT <CFont>
	CreateFont (const QFont& QtFont);
	
	virtual
	CCursor*
	GetStdCursor (EStdCursor CursorKind);

	ref::CPtrT <CCursor>
	CreateCursor (const QCursor& QtCursor);
	
	virtual
	CFont*
	GetFontMod (
		CFont* pBaseFont,
		uint_t Flags
		);

	virtual
	ref::CPtrT <CImage>
	CreateImage (
		int Width,
		int Height,
		EPixelFormat PixelFormat,
		const void* pData,
		bool IsScreenCompatible = true
		);

	virtual
	ref::CPtrT <CImageList>
	CreateImageList (
		int Width,
		int Height
		);

	virtual
	ref::CPtrT <CImageList>
	CreateImageList (
		CImage* pStipImage,
		int Width = 0
		);

	virtual
	ref::CPtrT <ÑCanvas>
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

} // namespace gui
} // namespace axl
