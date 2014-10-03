// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTENGINE_H

#include "axl_gui_Engine.h"
#include "axl_rtl_Singleton.h"
#include "axl_rtl_StringHashTable.h"
#include "axl_gui_QtCanvas.h"
#include "axl_gui_QtCursor.h"
#include "axl_gui_QtFont.h"
#include "axl_gui_QtImage.h"
#include "axl_gui_QtWidget.h"

namespace axl {
namespace gui {

//.............................................................................

class QtCaret: public QTimer
{
protected:
	Widget* m_widget;
	bool m_isVisible;
	Rect m_rect;

public:
	QtCaret ()
	{
		m_isVisible = false;
		m_widget = NULL;
	}

	bool
	isVisible ()
	{
		return m_isVisible;
	}

	bool
	show (
		Widget* widget,
		const Rect& rect
		);

	virtual
	void
	hide ();

protected:
	virtual void timerEvent (QTimerEvent* e);
};

//.............................................................................

class QtEngine: public Engine
{
protected:
	ref::Ptr <Font> m_stdFontArray [StdFontKind__Count];
	ref::Ptr <Cursor> m_stdCursorArray [StdCursorKind__Count];
	QtCaret m_qtCaret;

	rtl::StringHashTableMap <uintptr_t> m_clipboardFormatNameMap;
	rtl::Array <rtl::String> m_clipboardFormatNameTable;
	QMimeData* m_qtClipboardMimeData;

public:
	QtEngine ()
	{
		m_engineKind = EngineKind_Qt;
		m_qtClipboardMimeData = NULL;
	}

	~QtEngine ()
	{
		if (m_qtClipboardMimeData)
			delete m_qtClipboardMimeData;
	}

	// font

	static
	QtEngine*
	getSingleton ()
	{
		return rtl::getSingleton <QtEngine> ();
	}

	virtual
	ref::Ptr <Font>
	createFont (
		const char* faceName,
		size_t pointSize = 0,
		uint_t flags = 0
		)
	{
		return createFont (createQtFont (faceName, pointSize, flags));
	}

	ref::Ptr <Font>
	createFont (const QFont& qtFont);

	// cursors

	ref::Ptr <Cursor>
	createCursor (const QCursor& qtCursor);

	// image

	virtual
	ref::Ptr <Image>
	createImage ();

	virtual
	ref::Ptr <Image>
	createImage (
		int width,
		int height,
		PixelFormat pixelFormat,
		const void* data,
		bool isScreenCompatible = true
		);

	virtual
	ref::Ptr <Canvas>
	createOffscreenCanvas (
		int width,
		int height
		);

	// clipboard

	virtual
	uintptr_t 
	registerClipboardFormat (const rtl::String& formatName);

	virtual
	bool
	readClipboard (rtl::String* string);

	virtual
	bool
	readClipboard (
		uintptr_t format,
		rtl::Array <char>* data
		);

	virtual
	bool
	writeClipboard (
		const char* string,
		size_t length = -1
		);

	virtual
	bool
	writeClipboard (
		uintptr_t format,
		const void* data,
		size_t size
		);

	virtual
	bool
	commitClipboard ();

	// caret

	virtual
	bool
	showCaret (
		Widget* widget,
		const Rect& rect
		)
	{
		return m_qtCaret.show (widget, rect);
	}

	virtual
	void
	hideCaret ()
	{
		m_qtCaret.hide ();
	}

	bool
	isCaretVisible ()
	{
		return m_qtCaret.isVisible ();
	}

private slots:
	void
	caretTimer_Timeout ();

protected:
	QFont
	createQtFont (
		const char* faceName,
		size_t pointSize,
		uint_t flags
		);

	virtual
	Font*
	getFontMod (
		Font* baseFont,
		uint_t flags
		);

	virtual
	ref::Ptr <Font>
	createStdFont (StdFontKind fontKind);

	virtual
	ref::Ptr <Cursor>
	createStdCursor (StdCursorKind cursorKind);
};

//.............................................................................

} // namespace gui
} // namespace axl
