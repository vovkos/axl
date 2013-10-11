// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_ENGINE_H

#include "axl_ref_Ptr.h"
#include "axl_gui_Font.h"
#include "axl_gui_Cursor.h"
#include "axl_gui_Image.h"
#include "axl_gui_ImageList.h"
#include "axl_gui_Canvas.h"
#include "axl_rtl_String.h"

namespace axl {
namespace gui {

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

struct IEngine
{
protected:
	struct TSharedOffscreenCanvas
	{
		TSize m_Size;
		ref::CPtrT <ICanvas> m_Canvas;
	};

protected:
	EEngine m_EngineKind;
	TSharedOffscreenCanvas m_SharedOffscreenCanvasArray [EFormFactor__Count];

public:
	IEngine ()
	{
		m_EngineKind = EEngine_Undefined;
	}

	EEngine 
	GetEngineKind ()
	{
		return m_EngineKind;
	}

	// fonts

	virtual
	IFont*
	GetDefaultFont () = 0;

	virtual
	IFont*
	GetDefaultMonospaceFont () = 0;

	virtual
	ref::CPtrT <IFont>
	CreateFont (
		const char* pFaceName,
		size_t PointSize,
		uint_t Flags = 0
		) = 0;

	ref::CPtrT <IFont>
	CreateFont (const TFontDesc& FontDesc)
	{
		return CreateFont (
			FontDesc.m_FaceName,
			FontDesc.m_PointSize,
			FontDesc.m_Flags
			);
	}

	// images

	virtual
	ICursor*
	GetStdCursor (EStdCursor CursorKind) = 0;

	virtual
	ref::CPtrT <IImage>
	CreateImage (
		int Width,
		int Height,
		EPixelFormat PixelFormat,
		const void* pData,
		bool IsScreenCompatible = true
		) = 0;

	ref::CPtrT <IImage>
	CreateImage (
		const TImageDesc& ImageDesc,
		bool IsScreenCompatible = true
		)
	{
		return CreateImage (
			ImageDesc.m_Size.m_Width,
			ImageDesc.m_Size.m_Height,
			ImageDesc.m_PixelFormat,
			ImageDesc.m_pData
			);
	}

	virtual
	ref::CPtrT <IImageList>
	CreateImageList (
		int Width,
		int Height
		) = 0;

	ref::CPtrT <IImageList>
	CreateImageList (const TSize& Size)
	{
		return CreateImageList (Size.m_Width, Size.m_Height);
	}

	virtual
	ref::CPtrT <IImageList>
	CreateImageList (
		IImage* pStipImage,
		int Width = 0 // if 0 then take pStripImage->GetSize ().m_Height
		) = 0;

	// offscreen canvas

	virtual
	ref::CPtrT <ICanvas>
	CreateOffscreenCanvas (
		int Width,
		int Height
		) = 0;

	ref::CPtrT <ICanvas>
	CreateOffscreenCanvas (const TSize& Size)
	{
		return CreateOffscreenCanvas (
			Size.m_Width,
			Size.m_Height
			);
	}

	ICanvas*
	GetSharedOffscreenCanvas (
		int Width,
		int Height
		);

	ICanvas*
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
	bool
	ReadClipboard (rtl::CString* pString) = 0;

	virtual
	rtl::CString
	ReadClipboard ()
	{
		rtl::CString String;
		ReadClipboard (&String);
		return String;
	}

	virtual
	bool
	WriteClipboard (
		const char* pString,
		size_t Length = -1
		) = 0;

	virtual
	bool
	WriteClipboard (const rtl::CString& String)
	{
		return WriteClipboard (String, String.GetLength ());
	}

protected:
	friend struct IFont;

	virtual
	IFont*
	GetFontMod (
		IFont* pBaseFont,
		uint_t Flags
		) = 0;
};

//.............................................................................

} // namespace gui
} // namespace axl

