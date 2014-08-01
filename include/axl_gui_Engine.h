// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_ENGINE_H

#include "axl_ref_Ptr.h"
#include "axl_gui_Font.h"
#include "axl_gui_Cursor.h"
#include "axl_gui_Image.h"
#include "axl_gui_Canvas.h"
#include "axl_rtl_String.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {

class CWidget;

//.............................................................................

enum EEngine
{
	EEngine_Undefined = 0,
	EEngine_Gdi,
	EEngine_Qt,
	EEngine_Gtk,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetEngineKindString (EEngine EngineKind);

//.............................................................................

class CEngine
{
protected:
	struct TSharedOffscreenCanvas
	{
		TSize m_Size;
		ref::CPtrT <CCanvas> m_Canvas;
	};

protected:
	EEngine m_EngineKind;
	ref::CPtrT <CFont> m_StdFontArray [EStdFont__Count];
	ref::CPtrT <CCursor> m_StdCursorArray [EStdCursor__Count];
	TSharedOffscreenCanvas m_SharedOffscreenCanvasArray [EFormFactor__Count];

public:
	CEngine ()
	{
		m_EngineKind = EEngine_Undefined;
	}

	EEngine
	GetEngineKind ()
	{
		return m_EngineKind;
	}

	// fonts

	CFont*
	GetStdFont (EStdFont FontKind);

	virtual
	ref::CPtrT <CFont>
	CreateFont (
		const char* pFaceName,
		size_t PointSize,
		uint_t Flags = 0
		) = 0;

	ref::CPtrT <CFont>
	CreateFont (const TFontDesc& FontDesc)
	{
		return CreateFont (
			FontDesc.m_FaceName,
			FontDesc.m_PointSize,
			FontDesc.m_Flags
			);
	}

	// cursors

	CCursor*
	GetStdCursor (EStdCursor CursorKind);

	// images

	virtual
	ref::CPtrT <CImage>
	CreateImage () = 0;

	virtual
	ref::CPtrT <CImage>
	CreateImage (
		int Width,
		int Height,
		EPixelFormat PixelFormat,
		const void* pData,
		bool IsScreenCompatible = true
		) = 0;

	ref::CPtrT <CImage>
	CreateImage (
		const TImageDesc& ImageDesc,
		bool IsScreenCompatible = true
		)
	{
		return CreateImage (
			ImageDesc.m_Size.m_Width,
			ImageDesc.m_Size.m_Height,
			ImageDesc.m_PixelFormat,
			ImageDesc.m_pData,
			IsScreenCompatible
			);
	}

	// offscreen canvas

	virtual
	ref::CPtrT <CCanvas>
	CreateOffscreenCanvas (
		int Width,
		int Height
		) = 0;

	ref::CPtrT <CCanvas>
	CreateOffscreenCanvas (const TSize& Size)
	{
		return CreateOffscreenCanvas (
			Size.m_Width,
			Size.m_Height
			);
	}

	CCanvas*
	GetSharedOffscreenCanvas (
		int Width,
		int Height
		);

	CCanvas*
	GetSharedOffscreenCanvas (const TSize& Size)
	{
		return GetSharedOffscreenCanvas (
			Size.m_Width,
			Size.m_Height
			);
	}

	void
	DeleteAllSharedOffscreenCanvases ();

	// clipboard

	virtual
	uintptr_t 
	RegisterClipboardFormat (const rtl::CString& FormatName) = 0;

	virtual
	bool
	ReadClipboard (rtl::CString* pString) = 0;

	virtual
	bool
	ReadClipboard (
		uintptr_t Format,
		rtl::CArrayT <char>* pData
		) = 0;

	rtl::CString
	ReadClipboard ()
	{
		rtl::CString String;
		ReadClipboard (&String);
		return String;
	}

	rtl::CArrayT <char>
	ReadClipboard (uintptr_t Format)
	{
		rtl::CArrayT <char> Data;
		ReadClipboard (Format, &Data);
		return Data;
	}

	virtual
	bool
	WriteClipboard (
		const char* pString,
		size_t Length = -1
		) = 0;

	virtual
	bool
	WriteClipboard (
		uintptr_t Format,
		const void* pData,
		size_t Size
		) = 0;

	bool
	WriteClipboard (const rtl::CString& String)
	{
		return WriteClipboard (String, String.GetLength ());
	}

	bool
	WriteClipboard (
		uintptr_t Format,
		const rtl::CArrayT <char>& Data
		)
	{
		return WriteClipboard (Format, Data, Data.GetCount ());
	}

	virtual
	bool
	CommitClipboard () = 0;

	// caret

	virtual
	bool
	ShowCaret (
		CWidget* pWidget,
		const TRect& Rect
		) = 0;

	virtual
	void
	HideCaret () = 0;

protected:
	friend class CFont;

	virtual
	CFont*
	GetFontMod (
		CFont* pBaseFont,
		uint_t Flags
		) = 0;

	virtual
	ref::CPtrT <CFont>
	CreateStdFont (EStdFont FontKind) = 0;

	virtual
	ref::CPtrT <CCursor>
	CreateStdCursor (EStdCursor CursorKind) = 0;
};

//.............................................................................

} // namespace gui
} // namespace axl

