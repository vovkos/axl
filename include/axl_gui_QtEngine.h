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
	CWidget* m_pWidget;
	bool m_IsVisible;
	TRect m_Rect;

public:
	CQtCaret ()
	{
		m_IsVisible = false;
		m_pWidget = NULL;
	}

	bool
	IsVisible ()
	{
		return m_IsVisible;
	}

	bool
	Show (
		CWidget* pWidget,
		const TRect& Rect
		);

	virtual
	void
	Hide ();

protected:
	virtual void timerEvent (QTimerEvent* e);
};

//.............................................................................

class CQtEngine: public CEngine
{
protected:
	ref::CPtrT <CFont> m_StdFontArray [EStdFont__Count];
	ref::CPtrT <CCursor> m_StdCursorArray [EStdCursor__Count];
	CQtCaret m_QtCaret;

	rtl::CStringHashTableMapT <uintptr_t> m_ClipboardFormatNameMap;
	rtl::CArrayT <rtl::CString> m_ClipboardFormatTable;
	QMimeData* m_pQtClipboardMimeData;

public:
	CQtEngine ()
	{
		m_EngineKind = EEngine_Qt;
		m_pQtClipboardMimeData = NULL;
	}

	~CQtEngine ()
	{
		if (m_pQtClipboardMimeData)
			delete m_pQtClipboardMimeData;
	}

	// font

	static
	CQtEngine*
	GetSingleton ()
	{
		return rtl::GetSingleton <CQtEngine> ();
	}

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

	// cursors

	ref::CPtrT <CCursor>
	CreateCursor (const QCursor& QtCursor);

	// image

	virtual
	ref::CPtrT <CImage>
	CreateImage ();

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
	ref::CPtrT <CCanvas>
	CreateOffscreenCanvas (
		int Width,
		int Height
		);

	// clipboard

	virtual
	uintptr_t 
	RegisterClipboardFormat (const rtl::CString& FormatName);

	virtual
	bool
	ReadClipboard (rtl::CString* pString);

	virtual
	bool
	ReadClipboard (
		uintptr_t Format,
		rtl::CArrayT <char>* pData
		);

	virtual
	bool
	WriteClipboard (
		const char* pString,
		size_t Length = -1
		);

	virtual
	bool
	WriteClipboard (
		uint_t Format,
		const void* pData,
		size_t Size
		);

	virtual
	bool
	CommitClipboard ();

	// caret

	virtual
	bool
	ShowCaret (
		CWidget* pWidget,
		const TRect& Rect
		)
	{
		return m_QtCaret.Show (pWidget, Rect);
	}

	virtual
	void
	HideCaret ()
	{
		m_QtCaret.Hide ();
	}

	bool
	IsCaretVisible ()
	{
		return m_QtCaret.IsVisible ();
	}

private slots:
	void
	CaretTimer_Timeout ();

protected:
	QFont
	CreateQtFont (
		const char* pFaceName,
		size_t PointSize,
		uint_t Flags
		);

	virtual
	CFont*
	GetFontMod (
		CFont* pBaseFont,
		uint_t Flags
		);

	virtual
	ref::CPtrT <CFont>
	CreateStdFont (EStdFont FontKind);

	virtual
	ref::CPtrT <CCursor>
	CreateStdCursor (EStdCursor CursorKind);
};

//.............................................................................

} // namespace gui
} // namespace axl
