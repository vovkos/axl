#include "pch.h"
#include "axl_log_Widget.h"
#include "axl_gui_Engine.h"
#include "axl_io_FilePathUtils.h"

namespace axl {
namespace log {

//.............................................................................

CWidget::CWidget (gui::IEngine* pEngine):
	gui::IWidget (pEngine)
{
	m_CacheMgr.m_pWidget = this;
	m_CacheMgr.m_pIndexMgr = &m_IndexMgr;
	m_ColorizeMgr.m_pCacheMgr = &m_CacheMgr;

	m_pHyperlinkHandler = NULL;
	m_pVolatilePacketFilter = NULL;

	m_MaxBinBlockBuffer = 1024;

	m_IconGapSize = 1;
	m_TimestampGapSize = 1;
	m_OffsetGapSize = 2;
	m_HexGapSize = 1;
	m_CaretWidth = 2;

	m_LineCount = 0;
	m_ColCount = 0;
	m_FirstVisibleLine = 0;
	m_FirstVisibleCol = 0;
	m_VisibleLineCount = 0;
	m_VisibleColCount = 0;

	m_IsIconVisible = true;
	m_IsTimestampVisible = true;
	m_IsOffsetVisible = true;

	m_IsDragging = false;
	m_IsSelecting = false;

	m_TimestampFormat = "%h:%m:%s";
	m_TimestampWidth = CalcTimestampWidth (m_TimestampFormat);
	m_BaseCol = m_TimestampWidth;

	m_OffsetWidth = 4;
	m_FullOffsetWidth = m_OffsetWidth + m_OffsetGapSize;

	m_pBaseFont = m_pEngine->GetDefaultMonospaceFont ();
	ASSERT (m_pBaseFont->IsMonospace ());

	m_CharSize = m_pBaseFont->CalcTextSize ('X');

	m_CharSize.m_Height += 1; // separator line

	m_pImageList = NULL;
	m_IconWidth = 0;
	m_IconOrigin.m_x = (2 * m_CharSize.m_Width - 16) / 2;
	m_IconOrigin.m_y = (m_CharSize.m_Height - 1 - 16) / 2;

	memcpy (m_ColorArray, gui::GetStdPalColorArray (), gui::EStdPalColor__Count * sizeof (uint_t));
	
	m_ColorArray [~gui::EColorFlag_Index & EColor_HiliteBack]      = 0xdcdcff;
	m_ColorArray [~gui::EColorFlag_Index & EColor_DelimiterLight]  = 0xdcdcdc;
	m_ColorArray [~gui::EColorFlag_Index & EColor_DelimiterMedium] = 0xc0c0c0;
	m_ColorArray [~gui::EColorFlag_Index & EColor_DelimiterBold]   = 0x808080;
	m_ColorArray [~gui::EColorFlag_Index & EColor_Timestamp]       = gui::EStdPalColor_GrayText;
	m_ColorArray [~gui::EColorFlag_Index & EColor_Offset]          = gui::EStdPalColor_GrayText;

	m_Palette.m_pColorArray = m_ColorArray;
	m_Palette.m_Count = countof (m_ColorArray);

/*
	TStockCtrlPaint_Construct (&m_StockCtrlPaint);

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	TOffscreenBuffer_Construct (&m_OffscreenBuffer);
#endif

//	m_hUpdateRgn = CreateRectRgn (0, 0, 0, 0);

*/
}

bool
CWidget::SetPacketFile (
	CPacketFile* pPacketFile,
	IRepresentor* pRepresentor
	)
{
	bool Result;

	rtl::CString IndexFileName = io::CreateTempFileName ();
	
	Result = m_IndexMgr.Create (
		pPacketFile, 
		pRepresentor, 
		rtl::CString (IndexFileName), 
		io::EFileFlag_DeleteOnClose | io::EFileFlag_Exclusive
		);

	if (!Result)
		return false;

#if (_AXL_ENV == AXL_ENV_WIN)
	::SetFileAttributesW (
		IndexFileName.cc2 (), 
		FILE_ATTRIBUTE_HIDDEN | 
		FILE_ATTRIBUTE_TEMPORARY |
		FILE_ATTRIBUTE_NOT_CONTENT_INDEXED
		);
#endif

	ReRepresentAll ();
	return true;
}

void 
CWidget::SetImageList (gui::IImageList* pImageList)
{
	m_pImageList = pImageList;

	if (pImageList)
	{
		gui::TSize Size = pImageList->GetImageSize ();
		size_t IconColCount = Size.m_Width / m_CharSize.m_Width;
		if (Size.m_Width % m_CharSize.m_Width)
			IconColCount++;

		m_IconWidth = IconColCount + m_IconGapSize;
	}
	else
	{
		m_IconWidth = 0;
	}

	RecalcBaseCol ();
	Redraw ();
}

void 
CWidget::ShowIcon (bool IsVisible)
{
	m_IsIconVisible = IsVisible;
	RecalcBaseCol ();
	Redraw ();
}

void 
CWidget::SetTimestampFormat (const rtl::CString& Format)
{
	m_TimestampFormat = Format;
	m_TimestampWidth = CalcTimestampWidth (Format);

	RecalcBaseCol ();
	Redraw ();
}

void 
CWidget::ShowTimestamp (bool IsVisible)
{
	m_IsTimestampVisible = IsVisible;
	RecalcBaseCol ();
	Redraw ();
}

void 
CWidget::SetOffsetWidth (size_t Width)
{
	if (Width > 8)
		Width = 8;

	m_OffsetWidth = Width;
	m_FullOffsetWidth = m_IsOffsetVisible ? m_OffsetWidth + m_OffsetGapSize : 0;

	Redraw ();
}

void 
CWidget::ShowOffset (bool IsVisible)
{
	m_IsOffsetVisible = IsVisible;
	m_FullOffsetWidth = m_IsOffsetVisible ? m_OffsetWidth + m_OffsetGapSize : 0;

	Redraw ();
}

size_t 
CWidget::CalcTimestampWidth (const char* pFormat)
{
	if (!pFormat || !*pFormat)
		return 0;
	
	size_t Length = g::TTime ().Format (&m_StringBuffer, m_TimestampFormat);
	return Length + m_TimestampGapSize;
}

void 
CWidget::RecalcBaseCol ()
{
	size_t BaseCol = 0;

	if (m_IsTimestampVisible)
		BaseCol += m_TimestampWidth;
	
	if (m_IsIconVisible)
		BaseCol += m_IconWidth;

	if (BaseCol != m_BaseCol)
	{
		m_BaseCol = BaseCol;
		RecalcHScroll ();
	}
}

void 
CWidget::RecalcColCount ()
{
	size_t BinHexColCount = m_CacheMgr.GetLongestBinHexLine () * 4 + m_HexGapSize + m_FullOffsetWidth;
	size_t BinTextColCount = m_CacheMgr.GetLongestBinTextLine () + m_FullOffsetWidth;
	size_t ColCount = m_CacheMgr.GetLongestTextLine ();
	
	if (BinHexColCount > ColCount)
		ColCount = BinHexColCount;

	if (BinTextColCount > ColCount)
		ColCount = BinTextColCount;

	if (ColCount != m_ColCount)
	{
		m_ColCount = ColCount;
		RecalcHScroll ();
	}
}

void
CWidget::OnReRepresentPacket (
	CCachePage* pPage,
	TReRepresent* pReRepresent
	)
{
	intptr_t LineDelta;

	bool DidFirstVisibleLineChange;
	bool DidFirstVisibleColChange;

	if (pReRepresent->m_NewLineCount)
		m_ColorizeMgr.ReColorizeLineRange (pPage, pReRepresent->m_AnchorPageLine, pReRepresent->m_NewLineCount); 

	LineDelta = pReRepresent->m_NewLineCount - pReRepresent->m_OldLineCount;

	m_LineCount += LineDelta;
	ASSERT (m_LineCount == m_IndexMgr.GetIndexFile ()->GetLineCount ());

	DidFirstVisibleLineChange = FixupFirstVisibleLine ();
	DidFirstVisibleColChange = FixupFirstVisibleCol ();

	RecalcColCount ();
	RecalcVScroll ();
	RecalcHScroll ();

	if (DidFirstVisibleLineChange || DidFirstVisibleColChange)
		Redraw ();
	else if (LineDelta == 0)
		RedrawLineRange (pReRepresent->m_AnchorAbsoluteLine, pReRepresent->m_AnchorAbsoluteLine + pReRepresent->m_OldLineCount - 1);
	else
		RedrawLineRange (pReRepresent->m_AnchorAbsoluteLine, -1);

	UpdateCursorPosOnReRepresentPacket (pReRepresent->m_AnchorAbsoluteLine, pReRepresent->m_OldLineCount, pReRepresent->m_NewLineCount);
}

bool
CWidget::ModifyPacketVolatileFlags (
	CCachePage* pPage,
	TCacheVolatilePacket* pVolatileMsg,
	uint_t VolatileFlags
	)
{
	TReRepresent ReRepresent;

	if (VolatileFlags == pVolatileMsg->m_VolatileFlags)
		return false;

	m_CacheMgr.ModifyPacketVolatileFlags (pPage, pVolatileMsg, VolatileFlags, &ReRepresent);
	OnReRepresentPacket (pPage, &ReRepresent);
	return true;
}

bool
CWidget::ModifyLineVolatileFlags (
	CLine* pLine,
	uint_t VolatileFlags
	)
{
	return pLine->m_pVolatilePacket ? 
		ModifyPacketVolatileFlags (pLine->GetCachePage (), pLine->m_pVolatilePacket, VolatileFlags) :
		false;
}

void
CWidget::UpdateLog ()
{
	size_t Anchor = m_IndexMgr.UpdateIndex ();

	if (Anchor)
		IncrementalUpdateLog ();
	else
		FullUpdateLog ();
}

void 
CWidget::IncrementalUpdateLog ()
{
	bool ShouldScrollToBottom = m_ScrollBarArray [gui::EWidgetOrientation_Vertical].IsMaxed ();
	bool ShouldPutCursorToEnd = ShouldScrollToBottom && IsCursorAtLastLine ();

	size_t AnchorLine = m_LineCount ? m_LineCount - 1 : 0;

	// mark cache pages for update starting with anchor line
	// there might be more than one: could be empty pages

	TIndexNode* pNode = m_IndexMgr.GetIndexFile ()->FindLeafByLine (AnchorLine, NULL);
	while (pNode)
	{
		CCachePage* pPage = m_CacheMgr.FindCachePageByIndexNode (pNode);
		if (pPage)
		{
			if (pPage->IsEmpty ())
				m_CacheMgr.RemoveCachePage (pPage);
			else
				pPage->IncrementalUpdate ();
		}

		pNode = m_IndexMgr.GetIndexFile ()->GetRightNode (pNode);
	}

	m_LineCount = m_IndexMgr.GetIndexFile ()->GetLineCount ();

	FixupFirstVisibleLine ();
	RecalcVScroll ();
	RedrawLineRange (AnchorLine, m_LineCount - 1);

	if (ShouldScrollToBottom)
		ScrollToBottom ();

	if (ShouldPutCursorToEnd)
		SetCursorPos (-1, -1, false); 
}

void 
CWidget::FullUpdateLog ()
{
	m_CacheMgr.ClearCache ();
	
	m_LineCount = m_IndexMgr.GetIndexFile ()->GetLineCount ();
	m_FirstVisibleLine = 0;
	m_FirstVisibleCol = 0;
	m_ColCount = 0;

	RecalcVScroll ();
	RecalcHScroll ();
	SetCursorPos (m_CursorPos); // this will effectively update cursor pos

	Redraw ();
}

void
CWidget::OnSetFocus (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	ShowCaret (
		GetMousePosFromCursorPos (m_CursorPos), 
		gui::TSize (m_CaretWidth, m_CharSize.m_Height)
		);
}

void
CWidget::OnKillFocus (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	HideCaret ();
}

void
CWidget::OnSize (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	m_VisibleLineCount = m_Size.m_Height / m_CharSize.m_Height;
	m_VisibleColCount = m_Size.m_Width / m_CharSize.m_Width;

	bool ShouldScrollToBottom = IsCursorAtLastLine (); 
	bool DidFirstVisibleLineChange = FixupFirstVisibleLine ();
	bool DidFirstVisibleColChange = FixupFirstVisibleCol ();

	RecalcVScroll ();
	RecalcHScroll ();

	if (ShouldScrollToBottom)
		ScrollToBottom ();

	if (DidFirstVisibleLineChange || DidFirstVisibleColChange)
	{
		Redraw ();
		UpdateCaretPos ();
	}
}

/*

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// message handlers

LRESULT
CWidget::NotifyParent (
	uint_t Code,
	NMHDR* pNotify
	)
{
	HWND hWndParent = GetParent (m_hWnd);

	NMHDR Notify;
	if (!pNotify)
		pNotify = &Notify;

	pNotify->code = Code;
	pNotify->hwndFrom = m_hWnd;
	pNotify->idFrom = GetDlgCtrlID(m_hWnd);
	
	return SendPacket (hWndParent, WM_NOTIFY, pNotify->idFrom, (LPARAM) pNotify);
}

LRESULT 
AXL_VFT
CWidget::WindowProc (
	TLogCtrl* 
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam,
	bool_t* pIsHandled
	)
{
	LRESULT Result = 0;

	if (Msg >= WM_MOUSEFIRST && Msg <= WM_MOUSELAST)
	{
		MSG MsgStruct = { m_hWnd, Msg, wParam, lParam };
		SendPacket (m_hWndToolTip, TTM_RELAYEVENT, 0, (LPARAM) &MsgStruct);
	}

	if (Msg >= WM_NCMOUSEFIRST && Msg <= WM_NCMOUSELAST)
		SendPacket (m_hWndToolTip, TTM_ACTIVATE, false, 0);

	switch (Msg)
	{
		// std windows msg

	case WM_CREATE:
		Result = OnCreate ();
		break;

	case WM_DESTROY:
		Result = OnDestroy ();
		break;

	case WM_GETDLGCODE:
		Result = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_SETFOCUS:
		Result = OnSetFocus ();
		break;

	case WM_KILLFOCUS:
		Result = OnKillFocus ();
		break;

	case WM_THEMECHANGED:
		TStockCtrlPaint_CloseThemes (&m_StockCtrlPaint);
		break;

	case WM_NCPAINT:
		TStockCtrlPaint_NcPaintEdge (&m_StockCtrlPaint, m_hWnd, (HRGN) wParam);
		break;

	case WM_PAINT:
		Result = OnPaint ();
		break;

	case WM_SIZE:
		Result = OnSize ();
		break;

	case WM_HSCROLL:
		OnScroll (SB_HORZ, LOWORD(wParam));
		break;

	case WM_VSCROLL:
		OnScroll (SB_VERT, LOWORD(wParam));
		break;

	case WM_SETCURSOR:
		Result = OnSetCursor (wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		Result = OnLButtonDown ((short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_RBUTTONDOWN:
		Result = OnRButtonDown ((short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSEMOVE:
		Result = OnMouseMove ((short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSEWHEEL:
		Result = OnMouseWheel (HIWORD(wParam));
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		m_IsDragging = false;
		ReleaseCapture ();
		break;

	case WM_CAPTURECHANGED:
		m_IsDragging = false;
		break;

	case WM_KEYDOWN:
		OnKeyDown ((int) wParam);
		break;

	// log ctrl msg

	case EMsg_GetPacketFile:
		Result = (LRESULT) m_pPacketFile;
		break;

	case EMsg_SetPacketFile:
		{
		TMsgSetPacketFile* pSetPacketFile = (TMsgSetPacketFile*) lParam;
		Result = SetPacketFile (pSetPacketFile->m_pFile, pSetPacketFile->m_pRepresentor);
		break;
		}

	case EMsg_GetRepresentor:
		Result = (LRESULT) m_pRepresentor;
		break;

	case EMsg_SetRepresentor:
		SetRepresentor ((TRepresentor*) lParam);
		Result = TRUE;
		break;

	case EMsg_GetFilter:
		Result = (LRESULT) m_pFilter;
		break;

	case EMsg_SetFilter:
		SetFilter ((TFilter*) lParam);
		Result = TRUE;
		break;

	case EMsg_GetColorizer:
		Result = (LRESULT) m_pFilter;
		break;

	case EMsg_SetColorizer:
		SetColorizer ((TColorizer*) lParam);
		Result = TRUE;
		break;

	case EMsg_GetHyperlinkHandler:
		Result = (LRESULT) m_pHyperlinkHandler;
		break;

	case EMsg_SetHyperlinkHandler:
		SetHyperlinkHandler ((THyperlinkHandler*) lParam);
		Result = TRUE;
		break;

	case EMsg_GetImageList:
		Result = (LRESULT) m_hImageList;
		break;

	case EMsg_SetImageList:
		SetImageList ((HIMAGELIST) lParam);
		Result = TRUE;
		break;

	case EMsg_IsIconVisible:
		Result = (LRESULT) m_IsIconVisible;
		break;

	case EMsg_ShowIcon:
		ShowIcon ((BOOL) wParam);
		Result = TRUE;
		break;

	case EMsg_GetOffsetWidth:
		Result = (LRESULT) m_OffsetWidth;
		break;

	case EMsg_SetOffsetWidth:
		SetOffsetWidth ((size_t) wParam);
		Result = TRUE;
		break;

	case EMsg_IsTimestampVisible:
		Result = (LRESULT) m_IsTimestampVisible;
		break;

	case EMsg_ShowTimestamp:
		ShowTimestamp ((BOOL) wParam);
		Result = TRUE;
		break;

	case EMsg_SetTimestampFormat:
		SetTimestampFormat ((const char*) lParam);
		break;

	case EMsg_GetTimestampFormat:
		Result = (LRESULT) m_TimestampFormat.m_p;
		break;

	case EMsg_GetDefBinDataConfig:
		Result = (LRESULT) TIndexMgr_GetDefBinDataConfig (&m_IndexMgr);
		break;

	case EMsg_SetDefBinDataConfig:
		TIndexMgr_SetDefBinDataConfig (&m_IndexMgr, (const TBinDataConfig*) lParam);
		Result = TRUE;
		break;

	case EMsg_UpdateLog:
		UpdateLog ();
		Result = TRUE;
		break;

	case EMsg_ReRepresentAll:
		ReRepresentAll ();
		Result = TRUE;
		break;

	case EMsg_ReFilterAll:
		ReFilterAll ();
		Result = TRUE;
		break;

	case EMsg_ReColorizeAll:
		ReColorizeAll ();
		Result = TRUE;
		break;

	case EMsg_ModifyLineVolatileFlags:
		Result = ModifyLineVolatileFlags ((CLine*) lParam, (size_t) wParam);
		break;

	case EMsg_GetLineCount:
		Result = (LRESULT) m_LineCount;
		break;

	case EMsg_GetLine:
		Result = (LRESULT) m_CacheMgr.GetLine ((size_t) wParam);
		break;

	case EMsg_GetNextLine:
		Result = (LRESULT) m_CacheMgr.GetNextLine ((CLine*) lParam);
		break;

	case EMsg_GetPrevLine:
		Result = (LRESULT) m_CacheMgr.GetPrevLine ((CLine*) lParam);
		break;

	case EMsg_GetLineString:
		break;

	case EMsg_GetCursorPos:
		*(TCursorPos*) lParam = m_CursorPos;
		Result = TRUE;
		break;

	case EMsg_SetCursorPos:
		SetCursorPos ((TCursorPos*) lParam, false);
		Result = TRUE;
		break;

	case EMsg_GetSelection:
		*(TCursorPos*) wParam = m_SelStart;
		*(TCursorPos*) lParam = m_SelEnd;
		Result = TRUE;
		break;

	case EMsg_IsSelectionEmpty:
		Result = (LRESULT) IsSelectionEmpty ();
		break;

	case EMsg_SetSelection:
		SetSelection ((TCursorPos*) wParam, (TCursorPos*) lParam);
		Result = TRUE;
		break;

	case EMsg_GetHilite:
		break;

	case EMsg_SetHilite:
		break;

	case EMsg_SelectBinRange:
		break;

	case EMsg_HiliteBinRange:
		break;

	case EMsg_EnsureVisible:
		break;

	case EMsg_EnsureVisibleRange:
		break;

	case EMsg_GetRangeBinBlock:
		break;

	case EMsg_ProcessRange:
		break;

	case EMsg_ProcessRangeAsBinBlock:
		break;

	case EMsg_GetSelectionBinBlock:
		Result = (LPARAM) GetSelectionBinBlock ((size_t*) wParam, (size_t*) lParam);
		break;

	case EMsg_ProcessSelection:
		Result = (LPARAM) ProcessSelection ((FProcessLine) wParam, (void*) lParam);
		break;

	case EMsg_ProcessSelectionAsBinBlock:
		Result = (LPARAM) ProcessSelectionAsBinBlock ((FProcessBinBlock) wParam, (void*) lParam);
		break;

	case EMsg_SetFindPattern:
		{
		TMsgSetFindPattern* pSetPattern = (TMsgSetFindPattern*) lParam;
		Result = (LRESULT) axl_rtl_TBmhFind_SetPattern (
			&m_Find, 
			pSetPattern->m_p, 
			pSetPattern->m_Size, 
			pSetPattern->m_DoMatchCase
			);
		}
		break;

	case EMsg_CanFind:
		Result = (LRESULT) axl_rtl_TBmhFind_CanFind (&m_Find);
		break;

	case EMsg_FindNext:
		Result = (LRESULT) FindNext ();
		break;

	case EMsg_FindPrev:
		Result = (LRESULT) FindPrev ();
		break;

	case EMsg_CanCopy:
		Result = (LRESULT) !IsSelectionEmpty ();
		break;

	case EMsg_Copy:
		Result = (LRESULT) Copy ();
		break;

	case EMsg_CopyString:
		Result = (LRESULT) CopyString ((axl_rtl_TString*) lParam);
		break;

	case EMsg_SaveAsTextFile:
		Result = (LRESULT) SaveAsTextFile ((const char*) lParam, (axl_rtl_TBinTree*) wParam);
		break;

	case EMsg_HitTest:
		break;

	default:
		*pIsHandled = false;
	}
 
	return Result;
}

*/

//.............................................................................

} // namespace log {
} // namespace axl {
