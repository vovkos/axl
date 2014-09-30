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
getEngineKindString (EEngine engineKind);

//.............................................................................

class CEngine
{
protected:
	struct TSharedOffscreenCanvas
	{
		TSize m_size;
		ref::CPtrT <CCanvas> m_canvas;
	};

protected:
	EEngine m_engineKind;
	ref::CPtrT <CFont> m_stdFontArray [EStdFont__Count];
	ref::CPtrT <CCursor> m_stdCursorArray [EStdCursor__Count];
	TSharedOffscreenCanvas m_sharedOffscreenCanvasArray [EFormFactor__Count];

public:
	CEngine ()
	{
		m_engineKind = EEngine_Undefined;
	}

	EEngine
	getEngineKind ()
	{
		return m_engineKind;
	}

	// fonts

	CFont*
	getStdFont (EStdFont fontKind);

	virtual
	ref::CPtrT <CFont>
	createFont (
		const char* faceName,
		size_t pointSize,
		uint_t flags = 0
		) = 0;

	ref::CPtrT <CFont>
	createFont (const TFontDesc& fontDesc)
	{
		return createFont (
			fontDesc.m_faceName,
			fontDesc.m_pointSize,
			fontDesc.m_flags
			);
	}

	// cursors

	CCursor*
	getStdCursor (EStdCursor cursorKind);

	// images

	virtual
	ref::CPtrT <CImage>
	createImage () = 0;

	virtual
	ref::CPtrT <CImage>
	createImage (
		int width,
		int height,
		EPixelFormat pixelFormat,
		const void* data,
		bool isScreenCompatible = true
		) = 0;

	ref::CPtrT <CImage>
	createImage (
		const TImageDesc& imageDesc,
		bool isScreenCompatible = true
		)
	{
		return createImage (
			imageDesc.m_size.m_width,
			imageDesc.m_size.m_height,
			imageDesc.m_pixelFormat,
			imageDesc.m_data,
			isScreenCompatible
			);
	}

	// offscreen canvas

	virtual
	ref::CPtrT <CCanvas>
	createOffscreenCanvas (
		int width,
		int height
		) = 0;

	ref::CPtrT <CCanvas>
	createOffscreenCanvas (const TSize& size)
	{
		return createOffscreenCanvas (
			size.m_width,
			size.m_height
			);
	}

	CCanvas*
	getSharedOffscreenCanvas (
		int width,
		int height
		);

	CCanvas*
	getSharedOffscreenCanvas (const TSize& size)
	{
		return getSharedOffscreenCanvas (
			size.m_width,
			size.m_height
			);
	}

	void
	deleteAllSharedOffscreenCanvases ();

	// clipboard

	virtual
	uintptr_t 
	registerClipboardFormat (const rtl::CString& formatName) = 0;

	virtual
	bool
	readClipboard (rtl::CString* string) = 0;

	virtual
	bool
	readClipboard (
		uintptr_t format,
		rtl::CArrayT <char>* data
		) = 0;

	rtl::CString
	readClipboard ()
	{
		rtl::CString string;
		readClipboard (&string);
		return string;
	}

	rtl::CArrayT <char>
	readClipboard (uintptr_t format)
	{
		rtl::CArrayT <char> data;
		readClipboard (format, &data);
		return data;
	}

	virtual
	bool
	writeClipboard (
		const char* string,
		size_t length = -1
		) = 0;

	virtual
	bool
	writeClipboard (
		uintptr_t format,
		const void* data,
		size_t size
		) = 0;

	bool
	writeClipboard (const rtl::CString& string)
	{
		return writeClipboard (string, string.getLength ());
	}

	bool
	writeClipboard (
		uintptr_t format,
		const rtl::CArrayT <char>& data
		)
	{
		return writeClipboard (format, data, data.getCount ());
	}

	virtual
	bool
	commitClipboard () = 0;

	// caret

	virtual
	bool
	showCaret (
		CWidget* widget,
		const TRect& rect
		) = 0;

	virtual
	void
	hideCaret () = 0;

protected:
	friend class CFont;

	virtual
	CFont*
	getFontMod (
		CFont* baseFont,
		uint_t flags
		) = 0;

	virtual
	ref::CPtrT <CFont>
	createStdFont (EStdFont fontKind) = 0;

	virtual
	ref::CPtrT <CCursor>
	createStdCursor (EStdCursor cursorKind) = 0;
};

//.............................................................................

} // namespace gui
} // namespace axl

