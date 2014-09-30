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

class Widget;

//.............................................................................

enum EngineKind
{
	EngineKind_Undefined = 0,
	EngineKind_Gdi,
	EngineKind_Qt,
	EngineKind_Gtk,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getEngineKindString (EngineKind engineKind);

//.............................................................................

class Engine
{
protected:
	struct SharedOffscreenCanvas
	{
		Size m_size;
		ref::Ptr <Canvas> m_canvas;
	};

protected:
	EngineKind m_engineKind;
	ref::Ptr <Font> m_stdFontArray [StdFontKind__Count];
	ref::Ptr <Cursor> m_stdCursorArray [StdCursorKind__Count];
	SharedOffscreenCanvas m_sharedOffscreenCanvasArray [FormFactorKind__Count];

public:
	Engine ()
	{
		m_engineKind = EngineKind_Undefined;
	}

	EngineKind
	getEngineKind ()
	{
		return m_engineKind;
	}

	// fonts

	Font*
	getStdFont (StdFontKind fontKind);

	virtual
	ref::Ptr <Font>
	createFont (
		const char* faceName,
		size_t pointSize,
		uint_t flags = 0
		) = 0;

	ref::Ptr <Font>
	createFont (const FontDesc& fontDesc)
	{
		return createFont (
			fontDesc.m_faceName,
			fontDesc.m_pointSize,
			fontDesc.m_flags
			);
	}

	// cursors

	Cursor*
	getStdCursor (StdCursorKind cursorKind);

	// images

	virtual
	ref::Ptr <Image>
	createImage () = 0;

	virtual
	ref::Ptr <Image>
	createImage (
		int width,
		int height,
		PixelFormatKind pixelFormat,
		const void* data,
		bool isScreenCompatible = true
		) = 0;

	ref::Ptr <Image>
	createImage (
		const ImageDesc& imageDesc,
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
	ref::Ptr <Canvas>
	createOffscreenCanvas (
		int width,
		int height
		) = 0;

	ref::Ptr <Canvas>
	createOffscreenCanvas (const Size& size)
	{
		return createOffscreenCanvas (
			size.m_width,
			size.m_height
			);
	}

	Canvas*
	getSharedOffscreenCanvas (
		int width,
		int height
		);

	Canvas*
	getSharedOffscreenCanvas (const Size& size)
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
	registerClipboardFormat (const rtl::String& formatName) = 0;

	virtual
	bool
	readClipboard (rtl::String* string) = 0;

	virtual
	bool
	readClipboard (
		uintptr_t format,
		rtl::Array <char>* data
		) = 0;

	rtl::String
	readClipboard ()
	{
		rtl::String string;
		readClipboard (&string);
		return string;
	}

	rtl::Array <char>
	readClipboard (uintptr_t format)
	{
		rtl::Array <char> data;
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
	writeClipboard (const rtl::String& string)
	{
		return writeClipboard (string, string.getLength ());
	}

	bool
	writeClipboard (
		uintptr_t format,
		const rtl::Array <char>& data
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
		Widget* widget,
		const Rect& rect
		) = 0;

	virtual
	void
	hideCaret () = 0;

protected:
	friend class Font;

	virtual
	Font*
	getFontMod (
		Font* baseFont,
		uint_t flags
		) = 0;

	virtual
	ref::Ptr <Font>
	createStdFont (StdFontKind fontKind) = 0;

	virtual
	ref::Ptr <Cursor>
	createStdCursor (StdCursorKind cursorKind) = 0;
};

//.............................................................................

} // namespace gui
} // namespace axl

