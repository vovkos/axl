// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_WIDGET_H

#include "axl_g_Time.h"
#include "axl_log_Protocol.h"
#include "axl_log_Page.h"
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

enum EColor
{
	EColor_HiliteBack = gui::EColorFlag_Index | gui::EStdPalColor__Count,
	EColor_DelimiterLight,
	EColor_DelimiterMedium,
	EColor_DelimiterBold,
	EColor_Timestamp,
	EColor_Offset,
	EColor_CacheMissBack,
	EColor_Last,

	EColor__Count = ~gui::EColorFlag_Index & EColor_Last
};


//.............................................................................

class CWidget: 
	public gui::IWidget,
	public IClient
{
	friend class CCacheMgr;
	friend class CColorizeMgr;
	
protected:
	IServer* m_pServer;
		
	// index

	CIndexFile m_IndexFile;
	size_t m_SyncId;
	
	// cache 

	rtl::CStdListT <CPage> m_CachePageList;
	rtl::CHashTableMapT <size_t, CPage*, rtl::CHashIdT <size_t> > m_CachePageMap;
	size_t m_CachePageCountLimit;

	// CColorizeMgr m_ColorizeMgr;

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

	size_t m_LongestTextLineLength;
	size_t m_LongestBinHexLineSize;
	size_t m_LongestBinTextLineSize;

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
	CWidget (gui::IEngine* pEngine);

	bool
	Create (
		IServer* pServer,
		const char* pIndexFileName
		);

	void
	ProcessCliMsg (
		ECliMsg MsgKind,
		const void* p,
		size_t Size
		);

	virtual 
	void 
	ClearCache (size_t SyncId);

	virtual 
	void 
	FilterProgress (uint_t Percentage); // -1 = complete

	virtual 
	void 
	IndexProgress (uint_t Percentage); // -1 = complete

	virtual 
	void 
	SetIndexTailLeaf (
		bool IsNewLeaf,
		size_t LineCount,
		const TIndexLeaf* pLeaf,
		const TIndexVolatilePacket* pVolatilePacketArray
		);

	virtual 
	void 
	RepresentComplete (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t IndexNodeLineCount,
		size_t LineIdx,
		const void* p,
		size_t Size
		);

	virtual 
	void 
	ColorizeComplete (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t LineIdx,
		size_t LineOffset,
		const void* p,
		size_t Size
		);

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
/*
	bool
	SetPacketFile (
		CPacketFile* pPacketFile,
		IRepresenter* pRepresenter
		);

	CPacketFile* 
	GetPacketFile ()
	{
		return m_IndexMgr.GetPacketFile ();
	}

	IRepresenter* 
	GetRepresenter ()
	{
		return m_IndexMgr.GetRepresenter ();
	}

	void
	SetRepresenter (IRepresenter* pRepresenter)
	{
		m_IndexMgr.SetRepresenter (pRepresenter);
		FullUpdateLog ();
	}
*/

	// cache

	size_t 
	GetLineCount ()
	{
		return m_LineCount;
	}
/*
	void
	UpdateLog ();

	void
	ReRepresentAll ()
	{
		m_IndexMgr.ClearIdx ();
		m_IndexMgr.UpdateIndex ();
		FullUpdateLog ();
	}

	void
	ReFilterAllVolatilePackets ()
	{
		m_IndexMgr.FilterVolatilePackets ();
		FullUpdateLog ();
	}
*/
	// colorizer
/*
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
*/

	CPage* 
	FindCachePageByIndexNode (TIndexNode* pNode);

	CPage* 
	GetCachePageByIndexNode (TIndexNode* pNode);

	CPage* 
	GetCachePageByLineIdx (
		size_t LineIdx,
		size_t* pStartLineIndex
		);

	CPage* 
	GetNextCachePage (CPage* pPage);

	CPage* 
	GetPrevCachePage (CPage* pPage);

	size_t
	GetCachePageStartLineIdx (CPage* pPage);

	CLine* 
	GetLine (size_t LineIdx);

	CLine* 
	GetNextLine (CLine* pLine);

	CLine* 
	GetPrevLine (CLine* pLine);

	size_t
	GetLineIdx (CLine* pLine)
	{
		return GetCachePageStartLineIdx (pLine->m_pPage) + pLine->m_LineIdx;
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
		return GetLine (m_CursorPos.m_Line);
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

	void
	UpdateCursorPosOnReRepresentPacket (
		size_t AnchorLine,
		size_t OldLineCount,
		size_t NewLineCount
		);

	// bin line sel / hilite

	size_t
	GetBinHexLineOffsetFromCol (
		CBinHexLine* pLine,
		size_t Col,
		size_t* pHexCol
		);

	size_t
	GetBinTextLineOffsetFromCol (
		CBinTextLine* pLine,
		size_t Col
		);

	bool
	GetBinLineOffset (
		CBinLine* pLine,
		size_t Col,
		size_t* pOffset,
		size_t* pLineOffset,
		size_t* pHexCol,
		size_t* pMergeId
		);

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
/*
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
*/
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
	// protocol impl

	CPage* 
	GetOrCreateCachePage (size_t IndexNodeOffset);

	void 
	ClearCacheImpl (size_t SyncId);

	void 
	FilterProgressImpl (uint_t Percentage); // -1 = complete

	void 
	IndexProgressImpl (uint_t Percentage); // -1 = complete

	void 
	SetIndexTailLeafImpl (
		bool IsNewLeaf,
		size_t LineCount,
		const TIndexLeaf* pLeaf,
		const TIndexVolatilePacket* pVolatilePacketArray
		);

	void 
	RepresentCompleteImpl (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t IndexNodeLineCount,
		size_t LineIdx,
		const void* p,
		size_t Size
		);

	void 
	ColorizeCompleteImpl (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t LineIdx,
		size_t LineOffset,
		const void* p,
		size_t Size
		);

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
/*
	void
	OnReRepresentPacket (
		CPage* pPage,
		TReRepresent* pReRepresent
		);

	bool
	ModifyPacketVolatileFlags (
		CPage* pPage,
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
*/	
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
		return m_FullOffsetWidth + 3 * pLine->GetBinDataConfig ()->m_BinHexLineSize + m_HexGapSize;
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
	PaintBinOffset (
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
		CBinHexLine* pLine,
		size_t SelStart,
		size_t SelEnd
		);

	void
	PaintBinHexLine (
		gui::CTextPaint* pPaint,
		CBinHexLine* pLine,
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
/*
	bool
	IncapsulateOrProcessBinBlock (
		const void* p,
		size_t Size,
		IProcessBinBlock* pProcess
		);
*/
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
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_ThreadMsg, OnThreadMsg)


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

	void
	OnThreadMsg (
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
