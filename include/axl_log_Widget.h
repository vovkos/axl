// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_WIDGET_H

#include "axl_g_Time.h"
#include "axl_log_IndexMgr.h"
#include "axl_log_CacheMgr.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_gui_Widget.h"
#include "axl_gui_TextPaint.h"
#include "axl_rtl_BmhFind.h"

namespace axl {
namespace log {

//.............................................................................

// notification messages

enum ENm
{
	ENm_First = (0U-2000U),
	ENm_CursorPosChange,
};

//.............................................................................

struct IHyperlinkHandler: public obj::IRoot
{
	// {3BB64985-06A6-4128-88D0-2BF6086D34B3}
	AXL_OBJ_INTERFACE (
		IHyperlinkHandler,
		0x3bb64985, 0x06a6, 0x4128, 0x88, 0xd0, 0x2b, 0xf6, 0x08, 0x6d, 0x34, 0xb3
		)

	virtual 
	bool 
	OnHyperlink (
		CLine* pLine, 
		const char* pHyperlink
		) = 0;
};

//.............................................................................

struct IProcessLine: public obj::IRoot
{
	// {1484DEE4-2F12-4145-99B6-33853C734B3A}
	AXL_OBJ_INTERFACE (
		IProcessLine,
		0x1484dee4, 0x2f12, 0x4145, 0x99, 0xb6, 0x33, 0x85, 0x3c, 0x73, 0x4b, 0x3a
		)

	virtual 
	void 
	Process (
		CLine* pLine,
		CLine* pPrevLine,
		size_t StartCol, // if StartCol == -1 then it is NOT the first line in range
		size_t EndCol    // if EndCol == -1 then it is NOT the last line in range
		) = 0;
};

//.............................................................................

struct IProcessBinBlock: public obj::IRoot
{
	// {E652545A-AA16-4A8E-953F-55D63A80A18F}
	AXL_OBJ_INTERFACE (
		IProcessBinBlock,
		0xe652545a, 0xaa16, 0x4a8e, 0x95, 0x3f, 0x55, 0xd6, 0x3a, 0x80, 0xa1, 0x8f
		)

	virtual 
	void 
	Process (
		const void* p,
		size_t Size
		) = 0;
};

//.............................................................................

enum EColor
{
	EColor_HiliteBack = gui::EColorFlag_Index | gui::EStdPalColor__Count,
	EColor_DelimiterLight,
	EColor_DelimiterMedium,
	EColor_DelimiterBold,
	EColor_Timestamp,
	EColor_Offset,
	
	EColor__Count = ~gui::EColorFlag_Index & (EColor_Offset + 1)
};

//.............................................................................

class CWidget: public gui::IWidget
{
	friend class CCacheMgr;
	friend class CColorizeMgr;
	
protected:
	CIndexMgr m_IndexMgr;
	CCacheMgr m_CacheMgr;
	CColorizeMgr m_ColorizeMgr;

	size_t m_MaxBinBlockBuffer;

	size_t m_IconGapSize;
	size_t m_TimestampGapSize;
	size_t m_OffsetGapSize;
	size_t m_HexGapSize;
	size_t m_CaretWidth;

	size_t m_TimestampWidth;
	size_t m_IconWidth;
	size_t m_OffsetWidth;
	size_t m_FullOffsetWidth;

	size_t m_LineCount;
	size_t m_ColCount;
	size_t m_VisibleLineCount;
	size_t m_VisibleColCount;

	size_t m_FirstVisibleLine;
	size_t m_FirstVisibleCol;
	size_t m_BaseCol;

	bool m_IsIconVisible;
	bool m_IsTimestampVisible;
	bool m_IsOffsetVisible;

	bool m_IsDragging;
	bool m_IsSelecting;

	rtl::CString m_TimestampFormat;

	gui::IImageList* m_pImageList;	
	gui::TPoint m_IconOrigin;
	gui::TSize m_CharSize;

	gui::TCursorPos m_CursorPos;
	gui::TCursorPos m_SelAnchor;
	gui::TCursorPos m_SelStart;
	gui::TCursorPos m_SelEnd;
	gui::TCursorPos m_HiliteStart;
	gui::TCursorPos m_HiliteEnd;
	
	uint_t m_ColorArray [EColor__Count];

	rtl::CArrayT <uchar_t> m_BinBlockBuffer;	
	rtl::CString m_StringBuffer;

	rtl::CBmhFind m_Find; // Boyer-Moore-Horspool find

/*
	TStockCtrlPaint m_StockCtrlPaint;
	TOffscreenBuffer m_OffscreenBuffer;
	HRGN m_hUpdateRgn;
*/

public:
	IHyperlinkHandler* m_pHyperlinkHandler;
	IVolatilePacketFilter* m_pVolatilePacketFilter;

public:
	CWidget (gui::IEngine* pEngine);

	size_t 
	GetFullOffsetWidth ()
	{
		return m_FullOffsetWidth;
	}

	size_t 
	GetHexGapSize ()
	{
		return m_HexGapSize;
	}

	// log engine

	bool
	SetPacketFile (
		CPacketFile* pPacketFile,
		IRepresentor* pRepresentor
		);

	CPacketFile* 
	GetPacketFile ()
	{
		return m_IndexMgr.GetPacketFile ();
	}

	IRepresentor* 
	GetRepresentor ()
	{
		return m_IndexMgr.GetRepresentor ();
	}

	void
	SetRepresentor (IRepresentor* pRepresentor)
	{
		m_IndexMgr.SetRepresentor (pRepresentor);
		FullUpdateLog ();
	}

	size_t 
	GetLineCount ()
	{
		return m_LineCount;
	}

	void
	UpdateLog ();

	void
	ReRepresentAll ()
	{
		m_IndexMgr.ClearIndex ();
		m_IndexMgr.UpdateIndex ();
		FullUpdateLog ();
	}

	void
	ReFilterAllVolatilePackets ()
	{
		m_IndexMgr.FilterVolatilePackets ();
		FullUpdateLog ();
	}

	// colorizer

	IColorizer* 
	GetColorizer ()
	{
		return m_ColorizeMgr.GetColorizer ();
	}

	void
	SetColorizer (IColorizer* pColorizer)
	{
		m_ColorizeMgr.SetColorizer (pColorizer);
		Redraw ();
	}

	void
	ReColorizeAll ()
	{
		m_CacheMgr.ClearCacheColorization ();
		Redraw ();
	}

	// icons

	gui::IImageList*
	GetImageList ()
	{
		return m_pImageList;
	}

	void
	SetImageList (gui::IImageList* pImageList);

	bool
	IsIconVisible ()
	{
		return m_IsIconVisible;
	}

	void 
	ShowIcon (bool IsVisible);

	// timestamp

	const rtl::CString& 
	GetTimestampFormat ()
	{
		return m_TimestampFormat;
	}

	void 
	SetTimestampFormat (const rtl::CString& Format);

	bool
	IsTimestampVisible ()
	{
		return m_IsTimestampVisible;
	}

	void 
	ShowTimestamp (bool IsVisible);

	// binary offset

	size_t 
	GetOffsetWidth ()
	{
		return m_OffsetWidth;
	}

	void 
	SetOffsetWidth (size_t Width);

	bool
	IsOffsetVisible ()
	{
		return m_IsOffsetVisible;
	}

	void 
	ShowOffset (bool IsVisible);

	// cursor & selection

	gui::TCursorPos 
	GetCursorPos ()
	{
		return m_CursorPos;
	}

	void 
	SetCursorPos (
		uint_t Line,
		uint_t Col,
		bool Select = false
		);

	void 
	SetCursorPos (
		gui::TCursorPos& Pos,
		bool Select = false
		)
	{
		SetCursorPos (Pos.m_Line, Pos.m_Col, Select);
	}

	CLine*
	GetLineAtCursor ()
	{
		return m_CacheMgr.GetLine (m_CursorPos.m_Line);
	}

	gui::TCursorPos
	GetCursorPosFromMousePos (
		int x, 
		int y,
		bool AdjustX = true
		);

	gui::TCursorPos
	GetCursorPosFromMousePos (
		const gui::TPoint& Pos,
		bool AdjustX = true
		)
	{
		return GetCursorPosFromMousePos (Pos.m_x, Pos.m_y, AdjustX);
	}

	gui::TPoint
	GetMousePosFromCursorPos (
		uint_t Line,
		uint_t Col
		)
	{
		return gui::TPoint (
			(m_BaseCol + Col - m_FirstVisibleCol) * m_CharSize.m_Width,
			(Line - m_FirstVisibleLine) * m_CharSize.m_Height
			);
	}

	gui::TPoint
	GetMousePosFromCursorPos (const gui::TCursorPos& Pos)
	{
		return GetMousePosFromCursorPos (Pos.m_Line, Pos.m_Col);
	}

	gui::TCursorPos
	ValidateCursorPos (
		uint_t Line,
		uint_t Col
		);

	gui::TCursorPos
	ValidateCursorPos (const gui::TCursorPos& Pos)
	{
		return ValidateCursorPos (Pos.m_Line, Pos.m_Col);
	}

	void 
	ValidateCursorPos (gui::TCursorPos* pPos)
	{
		*pPos = ValidateCursorPos (*pPos);
	}

	bool
	ValidateCursorPosRange (
		gui::TCursorPos* pPosStart,
		gui::TCursorPos* pPosEnd
		);

	void 
	EnsureVisible (
		uint_t Line,
		uint_t Col
		);

	void 
	EnsureVisible (const gui::TCursorPos& Pos)
	{
		EnsureVisible (Pos.m_Line, Pos.m_Col);
	}

	void 
	EnsureVisibleRange (
		const gui::TCursorPos& PosStart,
		const gui::TCursorPos& PosEnd
		);

	bool
	IsSelectionEmpty ()
	{
		return m_SelStart == m_SelEnd;
	}

	bool
	IsCursorAtLastLine ()
	{
		return m_LineCount == 0 || m_CursorPos.m_Line == m_LineCount - 1;
	}

	bool
	GetSelection (
		gui::TCursorPos* pSelStart, 
		gui::TCursorPos* pSelEnd
		);

	void
	SetSelection (
		const gui::TCursorPos& SelStart, 
		const gui::TCursorPos& SelEnd
		);

	void 
	KillSelection ();
	
	bool
	GetHilite (
		gui::TCursorPos* pHiliteStart, 
		gui::TCursorPos* pHiliteEnd
		);

	void 
	SetHilite (
		const gui::TCursorPos& PosStart,
		const gui::TCursorPos& PosEnd
		);

	void 
	KillHilite ();

	// bin line sel / hilite

	bool
	SelectBinRange (
		CBinLine* pLine,
		size_t Offset,
		size_t Length
		);

	bool
	HiliteBinRange (
		CBinLine* pLine,
		size_t Offset,
		size_t Length
		);

	// selection processing

	size_t
	ProcessRange (
		const gui::TCursorPos& PosStart,
		const gui::TCursorPos& PosEnd,
		IProcessLine* pProcess
		);

	size_t
	ProcessRangeAsBinBlock (
		const gui::TCursorPos& PosStart,
		const gui::TCursorPos& PosEnd,
		IProcessBinBlock* pProcess
		);

	size_t
	ProcessSelection (IProcessLine* pProcess)
	{
		return ProcessRange (m_SelStart, m_SelEnd, pProcess);
	}

	size_t
	ProcessSelectionAsBinBlock (IProcessBinBlock* pProcess)
	{
		return ProcessRangeAsBinBlock (m_SelStart, m_SelEnd, pProcess);
	}

	// manual scroll

	size_t
	Scroll (intptr_t Delta)
	{
		intptr_t Line = m_FirstVisibleLine + Delta;
		return Line < 0 ? ScrollToLine (0) : ScrollToLine (Line);
	}

	size_t
	ScrollToLine (size_t Line);

	size_t
	ScrollToTop ()
	{
		return ScrollToLine (0);
	}

	size_t
	ScrollToBottom ()
	{
		return ScrollToLine (-1);
	}

	// copy

	bool
	Copy ();

	// find

	bool
	SetFindPattern (
		const void* p, 
		size_t Size, 
		bool DoMatchCase
		)
	{
		m_Find.SetPattern (p, Size, DoMatchCase);
	}

	bool
	CanFindNextPrev ()
	{
		return m_Find.CanFind ();
	}

	bool
	FindNext ();

	bool
	FindPrev ();


protected:
	void 
	IncrementalUpdateLog ();

	void 
	FullUpdateLog ();

	bool
	FixupFirstVisibleLine ();

	bool
	FixupFirstVisibleCol ();

	size_t
	CalcTimestampWidth (const char* pFormat);

	void 
	RecalcBaseCol ();

	void 
	RecalcColCount ();

	void 
	RecalcHScroll ();

	void 
	RecalcVScroll ();

	void
	UpdateCaretPos ()
	{
		if (IsFocused ())
			SetCaretPos (GetMousePosFromCursorPos (m_CursorPos));
	}

	// hyperlinks

	bool
	GetHyperlinkFromMousePos (
		int x,
		int y,
		CLine** ppLine,
		rtl::CString* pHyperlink
		);

	bool
	GetHyperlinkFromMousePos (
		const gui::TPoint& Pos,
		CLine** ppLine,
		rtl::CString* pHyperlink
		)
	{
		return GetHyperlinkFromMousePos (Pos.m_x, Pos.m_y, ppLine, pHyperlink);
	}

	void
	OnHyperlink (
		CLine* pLine,
		const char* pHyperlink
		);

	// re-represent

	void
	OnReRepresentPacket (
		CCachePage* pPage,
		TReRepresent* pReRepresent
		);

	bool
	ModifyPacketVolatileFlags (
		CCachePage* pPage,
		TCacheVolatilePacket* pVolatileMsg,
		uint_t VolatileFlags
		);

	bool
	ModifyLineVolatileFlags (
		CLine* pLine,
		uint_t VolatileFlags
		);

	void
	UpdateCursorPosOnReRepresentPacket (
		size_t AnchorLine,
		size_t OldLineCount,
		size_t NewLineCount
		);
	
	// bin ranges & offsets

	bool
	GetBinOffset (
		const gui::TCursorPos& Pos,
		size_t* pOffset,
		size_t* pLineOffset,
		size_t* pHexCol,
		size_t* pMergeId
		);

	bool
	GetBinRangePos (
		CBinLine* pLine,
		size_t Offset,
		size_t Length,
		gui::TCursorPos* pPosStart,
		gui::TCursorPos* pPosEnd
		);

	size_t
	GetAsciiCol (CBinLine* pLine)
	{
		return m_FullOffsetWidth + 3 * pLine->m_BinDataConfig.m_BinHexLineSize + m_HexGapSize;
	}

	size_t 
	GetHiliteCol (
		CBinLine* pLine,
		size_t Offset
		);

	// paint

	void
	RedrawLineRange (
		size_t FirstLine,
		size_t LastLine
		);

	void
	RedrawSelChangeLineRange (
		size_t SelStartLine,
		size_t SelEndLine,
		size_t OldSelStartLine,
		size_t OldSelEndLine
		);

	void
	RedrawLine (size_t Line)
	{
		RedrawLineRange (Line, Line);
	}
	
	const char* 
	FormatOffset (size_t Offset);

	size_t 
	GetTimestampString (
		CLine* pLine,
		rtl::CString* pString
		);

	void 
	PaintRect (
		gui::CTextPaint* pPaint,
		const gui::TRect& Rect
		);

	void
	PaintLine (
		gui::CTextPaint* pPaint,
		const gui::TRect& LineRect, 
		CLine* pLine, 
		size_t Line
		);

	void
	PaintTimestamp (
		gui::CTextPaint* pPaint,
		CLine* pLine
		);

	void
	PaintIcon (
		gui::CTextPaint* pPaint,
		size_t Icon
		);

	void
	PaintOffset (
		gui::CTextPaint* pPaint,
		size_t Offset
		);

	void
	PaintDelimiter (
		gui::CTextPaint* pPaint,
		CLine* pLine,
		const gui::TRect& LineRect
		);

	void
	PaintText (
		gui::CTextPaint* pPaint,
		const rtl::CString& Text,
		const gui::CTextAttrAnchorArray* pAttrArray,
		size_t Line,
		size_t BaseCol,
		const gui::TCursorPos* pSelStart,
		const gui::TCursorPos* pSelEnd
		);

	void
	PaintBinHex (
		gui::CTextPaint* pPaint,
		CBinLine* pLine,
		size_t SelStart,
		size_t SelEnd
		);

	void
	PaintBinHexLine (
		gui::CTextPaint* pPaint,
		CBinLine* pLine,
		size_t Line,
		const gui::TCursorPos* pSelStart,
		const gui::TCursorPos* pSelEnd
		);

	// bin blocks

	bool
	GetRangeBinBlock (
		const gui::TCursorPos& PosStart,
		const gui::TCursorPos& PosEnd,
		size_t* pOffset,
		size_t* pSize
		);

	bool
	GetSelectionBinBlock (
		size_t* pOffset,
		size_t* pSize
		)
	{
		return GetRangeBinBlock (m_SelStart, m_SelEnd, pOffset, pSize);
	}

	bool
	IncapsulateOrProcessBinBlock (
		const void* p,
		size_t Size,
		IProcessBinBlock* pProcess
		);

	// copy

	size_t 
	CopyString (rtl::CString* pString);

	// find

	size_t
	IncrementalFindEx (
		CBinLine* pLine,
		size_t LineOffset
		);

	size_t
	IncrementalFind (CBinLine* pLine);

	bool
	FindNextImpl (
		size_t Line, 
		size_t Col, 
		size_t EndLine
		);

	bool
	FindPrevImpl (
		size_t Line, 
		size_t Col, 
		size_t EndLine
		);

protected:
	AXL_GUI_WIDGET_MSG_MAP_BEGIN ()		
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_SetFocus, OnSetFocus)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_KillFocus, OnKillFocus)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_Paint, OnPaint)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_Size, OnSize)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_Scroll, OnScroll)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_MouseButtonDown, OnMouseButtonDown)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_MouseButtonUp, OnMouseButtonUp)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_MouseMove, OnMouseMove)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_MouseWheel, OnMouseWheel)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_MouseCaptureLost, OnMouseCaptureLost)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_KeyDown, OnKeyDown)


/*
	case WM_GETDLGCODE:
		Result = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_THEMECHANGED:
		TStockCtrlPaint_CloseThemes (&m_StockCtrlPaint);
		break;

	case WM_NCPAINT:
		TStockCtrlPaint_NcPaintEdge (&m_StockCtrlPaint, m_hWnd, (HRGN) wParam);
		break;

	case WM_SETCURSOR:
		Result = TLogCtrl_OnSetCursor (wParam, lParam);
		break;
*/


	AXL_GUI_WIDGET_MSG_MAP_END ()

	void
	OnCreate (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnSetFocus (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnKillFocus (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnPaint (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnSize (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnScroll (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnMouseButtonDown (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnMouseButtonUp (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnMouseMove (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnMouseWheel (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnMouseCaptureLost (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

	void
	OnKeyDown (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		);

protected:
	void 
	OnKeyDown_Tab (
		bool Shift, 
		bool Ctrl
		);

	void 
	OnKeyDown_Home (
		bool Shift, 
		bool Ctrl
		);

	void 
	OnKeyDown_End (
		bool Shift, 
		bool Ctrl
		);

	void 
	OnKeyDown_Left (
		bool Shift, 
		bool Ctrl
		);

	void 
	OnKeyDown_Right (
		bool Shift, 
		bool Ctrl
		);

	void 
	OnKeyDown_Up (
		bool Shift, 
		bool Ctrl
		);

	void 
	OnKeyDown_Down (
		bool Shift, 
		bool Ctrl
		);

	void 
	OnKeyDown_PageUp (
		bool Shift, 
		bool Ctrl
		);

	void 
	OnKeyDown_PageDown (
		bool Shift, 
		bool Ctrl
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
