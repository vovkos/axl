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

class CQtCaret: public QTimer
{
protected:
	CWidget* m_widget;
	bool m_isVisible;
	TRect m_rect;

public:
	CQtCaret ()
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
		CWidget* widget,
		const TRect& rect
		);

	virtual
	void
	hide ();

protected:
	virtual void timerEvent (QTimerEvent* e);
};

//.............................................................................

class CQtEngine: public CEngine
{
protected:
	ref::CPtrT <CFont> m_stdFontArray [EStdFont__Count];
	ref::CPtrT <CCursor> m_stdCursorArray [EStdCursor__Count];
	CQtCaret m_qtCaret;

	rtl::CStringHashTableMapT <uintptr_t> m_clipboardFormatNameMap;
	rtl::CArrayT <rtl::CString> m_clipboardFormatNameTable;
	QMimeData* m_qtClipboardMimeData;

public:
	CQtEngine ()
	{
		m_engineKind = EEngine_Qt;
		m_qtClipboardMimeData = NULL;
	}

	~CQtEngine ()
	{
		if (m_qtClipboardMimeData)
			delete m_qtClipboardMimeData;
	}

	// font

	static
	CQtEngine*
	getSingleton ()
	{
		return rtl::getSingleton <CQtEngine> ();
	}

	virtual
	ref::CPtrT <CFont>
	createFont (
		const char* faceName,
		size_t pointSize = 0,
		uint_t flags = 0
		)
	{
		return createFont (createQtFont (faceName, pointSize, flags));
	}

	ref::CPtrT <CFont>
	createFont (const QFont& qtFont);

	// cursors

	ref::CPtrT <CCursor>
	createCursor (const QCursor& qtCursor);

	// image

	virtual
	ref::CPtrT <CImage>
	createImage ();

	virtual
	ref::CPtrT <CImage>
	createImage (
		int width,
		int height,
		EPixelFormat pixelFormat,
		const void* data,
		bool isScreenCompatible = true
		);

	virtual
	ref::CPtrT <CCanvas>
	createOffscreenCanvas (
		int width,
		int height
		);

	// clipboard

	virtual
	uintptr_t 
	registerClipboardFormat (const rtl::CString& formatName);

	virtual
	bool
	readClipboard (rtl::CString* string);

	virtual
	bool
	readClipboard (
		uintptr_t format,
		rtl::CArrayT <char>* data
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
		CWidget* widget,
		const TRect& rect
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
	CFont*
	getFontMod (
		CFont* baseFont,
		uint_t flags
		);

	virtual
	ref::CPtrT <CFont>
	createStdFont (EStdFont fontKind);

	virtual
	ref::CPtrT <CCursor>
	createStdCursor (EStdCursor cursorKind);
};

//.............................................................................

} // namespace gui
} // namespace axl
