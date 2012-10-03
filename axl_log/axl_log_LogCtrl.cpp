#include "axl_Precomp.h"
#include "axl_rtl_RbTree.h"

#include "axl_log_IndexMgr.h"
#include "axl_log_CacheMgr.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_log_LogCtrl.h"

bool_t
AXL_API
axl_log_RegisterLogCtrlWndClass()
{
	static long _Once = false;
	static bool_t _Result = false;

	if (!InterlockedCompareExchange(&_Once, true, false))
	{
		axl_sys_TWindowClass Class =
		{	
			axl_log_WLogCtrl,
			sizeof(axl_log_TLogCtrl),
			axl_log_TLogCtrl_Construct,
			0,
			LoadCursor(NULL, IDC_IBEAM)
		};

		_Result = axl_sys_TWindowClass_Register(&Class);
	}

	return _Result;
}

//.............................................................................

void
AXL_API
axl_log_TLogCtrl_Construct(axl_log_TLogCtrl* pLogCtrl)
{
	static axl_sys_TWindowVTable _VTable = 
	{
		axl_log_TLogCtrl_Destruct,
		axl_log_TLogCtrl_WindowProc,
	};

	axl_sys_TWindow_Construct((axl_sys_TWindow*) pLogCtrl);
	axl_gr_TStockCtrlPaint_Construct(&pLogCtrl->m_StockCtrlPaint);
	axl_gr_TTextPaint_Construct(&pLogCtrl->m_TextPaint);

	pLogCtrl->m_pVTable = &_VTable;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	axl_gr_TOffscreenBuffer_Construct(&pLogCtrl->m_OffscreenBuffer);
#endif

//	pLogCtrl->m_hUpdateRgn = CreateRectRgn(0, 0, 0, 0);

	pLogCtrl->m_pMessageFile = NULL;
	axl_log_TIndexFile_Construct(&pLogCtrl->m_IndexFile);
	
	axl_log_TIndexMgr_Construct(&pLogCtrl->m_IndexMgr, pLogCtrl);
	axl_log_TCacheMgr_Construct(&pLogCtrl->m_CacheMgr, pLogCtrl);
	axl_log_TColorizeMgr_Construct(&pLogCtrl->m_ColorizeMgr, pLogCtrl);

	pLogCtrl->m_pRepresentor = NULL;
	pLogCtrl->m_pHyperlinkHandler = NULL;
	pLogCtrl->m_pFilter = NULL;
	pLogCtrl->m_pColorizer = NULL;

	pLogCtrl->m_hWnd = NULL;
	pLogCtrl->m_hWndToolTip = NULL;

	pLogCtrl->m_LineCount = 0;
	pLogCtrl->m_FirstVisibleLine = 0;
	pLogCtrl->m_FirstVisibleCol = 0;
	pLogCtrl->m_BaseCol = 0;
	pLogCtrl->m_VisibleLineCount = 0;
	pLogCtrl->m_VisibleColCount = 0;
	pLogCtrl->m_ColCount = 0;

	pLogCtrl->m_IsDragging = false;
	pLogCtrl->m_IsSelecting = false;

	axl_log_TCursorPos_Construct(&pLogCtrl->m_CursorPos);
	axl_log_TCursorPos_Construct(&pLogCtrl->m_HiliteStart);
	axl_log_TCursorPos_Construct(&pLogCtrl->m_HiliteEnd);
	axl_log_TCursorPos_Construct(&pLogCtrl->m_SelStart);
	axl_log_TCursorPos_Construct(&pLogCtrl->m_SelEnd);
	axl_log_TCursorPos_ConstructEx(&pLogCtrl->m_LastToolTipPos, -1, -1);
	
	axl_rtl_TBmhFind_Construct(&pLogCtrl->m_Find);
	axl_rtl_TByteArray_Construct(&pLogCtrl->m_BinBlockBuffer);

	axl_rtl_TString_Construct(&pLogCtrl->m_TimestampFormat);
	axl_rtl_TString_Construct(&pLogCtrl->m_TimestampString);
	
	axl_gr_TFont_Construct(&pLogCtrl->m_DefaultFont);
	axl_gr_TFont_CreatePointFont(&pLogCtrl->m_DefaultFont, _T("Courier New"), 100, DEFAULT_CHARSET, NONANTIALIASED_QUALITY);

	axl_gr_TFontMod_SetBaseFont(&pLogCtrl->m_TextPaint.m_Font, pLogCtrl->m_DefaultFont.m_hFont);

	pLogCtrl->m_CharSize = axl_gr_TFont_CalcCharSize(&pLogCtrl->m_DefaultFont, '|');

	if (pLogCtrl->m_CharSize.cx < 8)
		pLogCtrl->m_CharSize.cx = 8;

	if (pLogCtrl->m_CharSize.cy < 16)
		pLogCtrl->m_CharSize.cy = 16;

	pLogCtrl->m_CharSize.cy += 1; // separator line

	pLogCtrl->m_TextPaint.m_CharSize = pLogCtrl->m_CharSize;

	pLogCtrl->m_IconOrigin.x = (2 * pLogCtrl->m_CharSize.cx - 16) / 2;
	pLogCtrl->m_IconOrigin.y = (pLogCtrl->m_CharSize.cy - 1 - 16) / 2;

	pLogCtrl->m_IsTimestampVisible = true;
	pLogCtrl->m_TimestampWidth = 0;
	pLogCtrl->m_OffsetWidth = 4;
	pLogCtrl->m_FullOffsetWidth = pLogCtrl->m_OffsetWidth + axl_log_EDef_OffsetGapSize;
	pLogCtrl->m_IconWidth = 0;
	pLogCtrl->m_MouseWheelSpeed = 3;

	pLogCtrl->m_IsIconVisible = true;
	pLogCtrl->m_hImageList = NULL;
	pLogCtrl->m_hArrowCursor = LoadCursor(NULL, IDC_ARROW);
	pLogCtrl->m_hBeamCursor = LoadCursor(NULL, IDC_IBEAM);
	pLogCtrl->m_hHyperlinkCursor = LoadCursor(NULL, IDC_HAND);

	memcpy(pLogCtrl->m_Palette, g_axl_gr_StdPalette, sizeof(g_axl_gr_StdPalette));
	
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_Text)]            = GetSysColor(COLOR_WINDOWTEXT);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_Back)]            = GetSysColor(COLOR_WINDOW);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_SelectionText)]   = GetSysColor(COLOR_HIGHLIGHTTEXT);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_SelectionBack)]   = GetSysColor(COLOR_HIGHLIGHT);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_HiliteText)]      = GetSysColor(COLOR_WINDOWTEXT);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_HiliteBack)]      = RGB(220, 220, 255);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_DelimiterLight)]  = RGB(220, 220, 220);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_DelimiterMedium)] = RGB(192, 192, 192);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_DelimiterBold)]   = RGB(128, 128, 128);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_Timestamp)]       = GetSysColor(COLOR_GRAYTEXT);
	pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_Offset)]          = GetSysColor(COLOR_GRAYTEXT);

	pLogCtrl->m_TextPaint.m_pPalette = pLogCtrl->m_Palette;
	pLogCtrl->m_TextPaint.m_PaletteSize = axl_log_EColor_Count;
}

void
AXL_VFT
axl_log_TLogCtrl_Destruct(axl_log_TLogCtrl* pLogCtrl)
{
	axl_ref_TObject__ClearPtr((axl_ref_TObject**) &pLogCtrl->m_pRepresentor);
	axl_ref_TObject__ClearPtr((axl_ref_TObject**) &pLogCtrl->m_pHyperlinkHandler);
	axl_ref_TObject__ClearPtr((axl_ref_TObject**) &pLogCtrl->m_pFilter);
	axl_ref_TObject__ClearPtr((axl_ref_TObject**) &pLogCtrl->m_pColorizer);

	axl_gr_TStockCtrlPaint_Destruct(&pLogCtrl->m_StockCtrlPaint);
	axl_gr_TTextPaint_Destruct(&pLogCtrl->m_TextPaint);

	axl_log_TIndexFile_Destruct(&pLogCtrl->m_IndexFile);	
	axl_log_TIndexMgr_Destruct(&pLogCtrl->m_IndexMgr);
	axl_log_TCacheMgr_Destruct(&pLogCtrl->m_CacheMgr);
	axl_log_TColorizeMgr_Destruct(&pLogCtrl->m_ColorizeMgr);

	axl_gr_TFont_Destruct(&pLogCtrl->m_DefaultFont);

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	axl_gr_TOffscreenBuffer_Destruct(&pLogCtrl->m_OffscreenBuffer);
#endif

//	DeleteObject(pLogCtrl->m_hUpdateRgn);

	axl_rtl_TBmhFind_Destruct(&pLogCtrl->m_Find);
	axl_rtl_TByteArray_Destruct(&pLogCtrl->m_BinBlockBuffer);

	axl_rtl_TString_Destruct(&pLogCtrl->m_TimestampFormat);
	axl_rtl_TString_Destruct(&pLogCtrl->m_TimestampString);

	axl_sys_TWindow_Destruct((axl_sys_TWindow*) pLogCtrl);
}

bool_t
AXL_API
axl_log_TLogCtrl_SetMessageFile(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TMessageFile* pMessageFile,
	axl_log_TRepresentor* pRepresentor
	)
{
	if (!axl_log_TIndexFile_IsOpen(&pLogCtrl->m_IndexFile))
	{
		ulong_t OpenFlags = axl_sys_EFile_DeleteOnClose | axl_sys_EFile_Exclusive;

		TCHAR IndexFileName [MAX_PATH];
		GetTempFileName(_T("."), _T("idx"), 0, IndexFileName);

		if (!axl_log_TIndexFile_Open(&pLogCtrl->m_IndexFile, IndexFileName, OpenFlags))
			return false;

		SetFileAttributes(
			IndexFileName, 
			FILE_ATTRIBUTE_HIDDEN | 
			FILE_ATTRIBUTE_TEMPORARY |
			FILE_ATTRIBUTE_NOT_CONTENT_INDEXED
			);

		axl_log_TIndexMgr_ClearIndex(&pLogCtrl->m_IndexMgr);
	}

	pLogCtrl->m_pMessageFile = pMessageFile;

	if (pRepresentor)
		axl_ref_TObject__CopyPtr(
			(axl_ref_TObject**) &pLogCtrl->m_pRepresentor, 
			(axl_ref_TObject*) pRepresentor
			);

	axl_log_TLogCtrl_ReRepresentAll(pLogCtrl);
	return true;
}

void
AXL_API
axl_log_TLogCtrl_SetColorizer(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TColorizer* pColorizer
	)
{
	axl_ref_TObject__CopyPtr(
		(axl_ref_TObject**) &pLogCtrl->m_pColorizer, 
		(axl_ref_TObject*) pColorizer
		);

	axl_log_TLogCtrl_ReColorizeAll(pLogCtrl);
}

void
AXL_API
axl_log_TLogCtrl_SetRepresentor(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TRepresentor* pRepresentor
	)
{
	axl_ref_TObject__CopyPtr(
		(axl_ref_TObject**) &pLogCtrl->m_pRepresentor, 
		(axl_ref_TObject*) pRepresentor
		);

	axl_log_TLogCtrl_ReRepresentAll(pLogCtrl);
}

void
AXL_API
axl_log_TLogCtrl_ReRepresentAll(axl_log_TLogCtrl* pLogCtrl)
{
	if (!pLogCtrl->m_pRepresentor ||
		!pLogCtrl->m_pMessageFile || 
		!axl_log_TMessageFile_IsOpen(pLogCtrl->m_pMessageFile))
		return;

	axl_log_TIndexMgr_ClearIndex(&pLogCtrl->m_IndexMgr);
	axl_log_TIndexMgr_UpdateIndex(&pLogCtrl->m_IndexMgr);
	axl_log_TLogCtrl_FullUpdateLog(pLogCtrl);
}

void
AXL_API
axl_log_TLogCtrl_ReFilterAll(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TIndexMgr_ReFilterAll(&pLogCtrl->m_IndexMgr);  
	axl_log_TLogCtrl_FullUpdateLog(pLogCtrl);
}

void
AXL_API
axl_log_TLogCtrl_ReColorizeAll(axl_log_TLogCtrl* pLogCtrl)
{
//	axl_log_TCacheMgr_ClearCache(&pLogCtrl->m_CacheMgr);
	axl_log_TCacheMgr_ClearCacheColorization(&pLogCtrl->m_CacheMgr);
	axl_log_TLogCtrl_Invalidate(pLogCtrl);
}

void 
AXL_API
axl_log_TLogCtrl_SetImageList(
	axl_log_TLogCtrl* pLogCtrl,
	HIMAGELIST hImageList
	)
{
	pLogCtrl->m_hImageList = hImageList;
	pLogCtrl->m_IconWidth = hImageList ? 2 + axl_log_EDef_IconGapSize : 0;

	axl_log_TLogCtrl_RecalcBaseCol(pLogCtrl);
	axl_log_TLogCtrl_Invalidate(pLogCtrl);
}

void 
AXL_API
axl_log_TLogCtrl_ShowIcon(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t IsVisible
	)
{
	pLogCtrl->m_IsIconVisible = IsVisible;
	axl_log_TLogCtrl_RecalcBaseCol(pLogCtrl);
	axl_log_TLogCtrl_Invalidate(pLogCtrl);
}

void 
AXL_API
axl_log_TLogCtrl_ShowTimestamp(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t IsVisible
	)
{
	pLogCtrl->m_IsTimestampVisible = IsVisible;
	axl_log_TLogCtrl_RecalcBaseCol(pLogCtrl);
	axl_log_TLogCtrl_Invalidate(pLogCtrl);
}

void 
AXL_API
axl_log_TLogCtrl_SetTimestampFormat(
	axl_log_TLogCtrl* pLogCtrl,
	const tchar_t* pFormat
	)
{
	axl_rtl_TString_CopyString(&pLogCtrl->m_TimestampFormat, pFormat, -1);

	if (axl_rtl_TString_IsEmpty(&pLogCtrl->m_TimestampFormat))
	{
		pLogCtrl->m_TimestampWidth = 0;
	}
	else
	{
		SYSTEMTIME SysTime = {0};
		axl_rtl_TString Sample;
		size_t Length;

		axl_rtl_TString_Construct(&Sample);
		axl_log_TLogCtrl_FormatTime(&Sample, &SysTime, pFormat);
		Length = axl_rtl_TString_GetLength(&Sample);
		axl_rtl_TString_Destruct(&Sample);

		pLogCtrl->m_TimestampWidth = (ulong_t) Length + axl_log_EDef_TimestampGapSize;
	}

	axl_log_TLogCtrl_RecalcBaseCol(pLogCtrl);
	axl_log_TLogCtrl_Invalidate(pLogCtrl);
}

void 
AXL_API
axl_log_TLogCtrl_SetOffsetWidth(
	axl_log_TLogCtrl* pLogCtrl,
	ulong_t Width
	)
{
	if (Width > 8)
		Width = 8;

	pLogCtrl->m_OffsetWidth = Width;
	pLogCtrl->m_FullOffsetWidth = Width ? Width + axl_log_EDef_OffsetGapSize: 0;

	axl_log_TLogCtrl_Invalidate(pLogCtrl);
}

void 
AXL_API
axl_log_TLogCtrl_UpdateCursorPos(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TCursorPos Pos = pLogCtrl->m_CursorPos;
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, false);
}

enum
{
	axl_log_ECursorPosFixup_None = 0,
	axl_log_ECursorPosFixup_Below,
	axl_log_ECursorPosFixup_Within,
};

int
AXL_API
axl_log_TCursorPos_FixupOnReRepresentMessage(
	axl_log_TCursorPos* pPos,
	size_t AnchorLine,
	size_t OldLineCount,
	size_t NewLineCount
	)
{
	long LineDelta = (long) (NewLineCount - OldLineCount);

	if (pPos->m_Line >= (long) (AnchorLine + OldLineCount)) // below re-represented message
	{
		pPos->m_Line = pPos->m_Line + LineDelta;
		return axl_log_ECursorPosFixup_Below;
	}
	else if (pPos->m_Line >= (long) AnchorLine) // below re-represented message
	{
		pPos->m_Line = (long) AnchorLine;
		pPos->m_Col = 0;
		return axl_log_ECursorPosFixup_Within;
	}

	return axl_log_ECursorPosFixup_None;
}

void
AXL_API
axl_log_TLogCtrl_UpdateCursorPosOnReRepresentMessage(
	axl_log_TLogCtrl* pLogCtrl,
	size_t AnchorLine,
	size_t OldLineCount,
	size_t NewLineCount
	)
{
	axl_log_TCursorPos CursorPos = pLogCtrl->m_CursorPos;
	axl_log_TCursorPos_FixupOnReRepresentMessage(&CursorPos, AnchorLine, OldLineCount, NewLineCount);

	if (pLogCtrl->m_IsSelecting)
	{
		axl_log_TCursorPos SelStart = pLogCtrl->m_SelStart;
		axl_log_TCursorPos SelEnd = pLogCtrl->m_SelEnd;

		int SelStartFixup = axl_log_TCursorPos_FixupOnReRepresentMessage(&SelStart, AnchorLine, OldLineCount, NewLineCount);
		int SelEndFixup = axl_log_TCursorPos_FixupOnReRepresentMessage(&SelEnd, AnchorLine, OldLineCount, NewLineCount);

		if (SelStartFixup == axl_log_ECursorPosFixup_Within || SelEndFixup == axl_log_ECursorPosFixup_Within)
			axl_log_TLogCtrl_KillSelection(pLogCtrl);
		else if (SelStartFixup != axl_log_ECursorPosFixup_None || SelEndFixup != axl_log_ECursorPosFixup_None)
			axl_log_TLogCtrl_SetSelection(pLogCtrl, &SelStart, &SelEnd);
	}

	if (axl_log_TCursorPos_Cmp(&pLogCtrl->m_HiliteStart, &pLogCtrl->m_HiliteEnd) != 0)
	{
		axl_log_TCursorPos HiliteStart = pLogCtrl->m_HiliteStart;
		axl_log_TCursorPos HiliteEnd = pLogCtrl->m_HiliteEnd;

		int HiliteStartFixup = axl_log_TCursorPos_FixupOnReRepresentMessage(&HiliteStart, AnchorLine, OldLineCount, NewLineCount);
		int HiliteEndFixup = axl_log_TCursorPos_FixupOnReRepresentMessage(&HiliteEnd, AnchorLine, OldLineCount, NewLineCount);

		if (HiliteStartFixup == axl_log_ECursorPosFixup_Within || HiliteEndFixup == axl_log_ECursorPosFixup_Within)
			axl_log_TLogCtrl_KillHilite(pLogCtrl);
		else if (HiliteStartFixup != axl_log_ECursorPosFixup_None || HiliteEndFixup != axl_log_ECursorPosFixup_None)
			axl_log_TLogCtrl_SetHilite(pLogCtrl, &HiliteStart, &HiliteEnd);
	}

	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &CursorPos, pLogCtrl->m_IsSelecting);
}

void
AXL_API
axl_log_TLogCtrl_OnReRepresentMessage(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCachePage* pPage,
	axl_log_TReRepresent* pReRepresent
	)
{
	long LineDelta;

	bool_t DidFirstVisibleLineChange;
	bool_t DidFirstVisibleColChange;

	if (pReRepresent->m_NewLineCount)
		axl_log_TColorizeMgr_ReColorizeLineRange(&pLogCtrl->m_ColorizeMgr, pPage, pReRepresent->m_AnchorPageLine, pReRepresent->m_NewLineCount); 

	LineDelta = pReRepresent->m_NewLineCount - pReRepresent->m_OldLineCount;

	pLogCtrl->m_LineCount += LineDelta;
	ASSERT(pLogCtrl->m_LineCount == axl_log_TIndexFile_GetLineCount(&pLogCtrl->m_IndexFile));

	DidFirstVisibleLineChange = axl_log_TLogCtrl_FixupFirstVisibleLine(pLogCtrl);
	DidFirstVisibleColChange = axl_log_TLogCtrl_FixupFirstVisibleCol(pLogCtrl);

	axl_log_TLogCtrl_RecalcColCount(pLogCtrl);
	axl_log_TLogCtrl_RecalcVScroll(pLogCtrl);
	axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);

	if (DidFirstVisibleLineChange || DidFirstVisibleColChange)
		axl_log_TLogCtrl_Invalidate(pLogCtrl);
	else if (LineDelta == 0)
		axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, pReRepresent->m_AnchorAbsoluteLine, pReRepresent->m_AnchorAbsoluteLine + pReRepresent->m_OldLineCount - 1);
	else
		axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, pReRepresent->m_AnchorAbsoluteLine, -1);

	axl_log_TLogCtrl_UpdateCursorPosOnReRepresentMessage(pLogCtrl, pReRepresent->m_AnchorAbsoluteLine, pReRepresent->m_OldLineCount, pReRepresent->m_NewLineCount);
}

bool_t
AXL_API
axl_log_TLogCtrl_ModifyMessageVolatileFlags(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCachePage* pPage,
	axl_log_TCacheVolatileMessage* pVolatileMsg,
	ulong_t VolatileFlags
	)
{
	axl_log_TReRepresent ReRepresent;

	if (VolatileFlags == pVolatileMsg->m_VolatileFlags)
		return false;

	axl_log_TCacheMgr_ModifyMessageVolatileFlags(&pLogCtrl->m_CacheMgr, pPage, pVolatileMsg, VolatileFlags, &ReRepresent);
	axl_log_TLogCtrl_OnReRepresentMessage(pLogCtrl, pPage, &ReRepresent);
	return true;
}

bool_t
AXL_API
axl_log_TLogCtrl_ModifyLineVolatileFlags(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	ulong_t VolatileFlags
	)
{
	if (!pLine->m_pVolatileMessage)
		return false;

	return axl_log_TLogCtrl_ModifyMessageVolatileFlags(pLogCtrl, pLine->m_pPage, pLine->m_pVolatileMessage, VolatileFlags);
}

void 
AXL_API
axl_log_TLogCtrl_IncrementalUpdateLog(axl_log_TLogCtrl* pLogCtrl)
{
	bool_t ShouldScrollToBottom = axl_log_TLogCtrl_IsVScrollAtBottom(pLogCtrl);
	bool_t ShouldPutCursorToEnd = ShouldScrollToBottom && axl_log_TLogCtrl_IsCursorAtLastLine(pLogCtrl);

	ulong_t AnchorLine = pLogCtrl->m_LineCount ? pLogCtrl->m_LineCount - 1 : 0;

	// mark cache pages for update starting with anchor line
	// there might be more than one: could be empty pages

	axl_log_TIndexNode* pNode = axl_log_TIndexFile_FindLeafByLine(&pLogCtrl->m_IndexFile, AnchorLine, NULL);
	while (pNode)
	{
		axl_log_TCachePage* pPage = axl_log_TCacheMgr_FindCachePageByIndexNode(&pLogCtrl->m_CacheMgr, pNode);
		if (pPage)
		{
			if (axl_rtl_TArray_IsEmpty(&pPage->m_LineArray))
			{
				axl_log_TCacheMgr_RemoveCachePage(&pLogCtrl->m_CacheMgr, pPage);
			}
			else
			{
				pPage->m_IsRepresentNeeded = true;
				pPage->m_IsColorizeNeeded = true;
			}
		}

		pNode = axl_log_TIndexFile_GetRightNode(&pLogCtrl->m_IndexFile, pNode);
	}

	pLogCtrl->m_LineCount = axl_log_TIndexFile_GetLineCount(&pLogCtrl->m_IndexFile);

	axl_log_TLogCtrl_FixupFirstVisibleLine(pLogCtrl);
	axl_log_TLogCtrl_RecalcVScroll(pLogCtrl);
	axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, AnchorLine, pLogCtrl->m_LineCount - 1);

	if (ShouldScrollToBottom)
		axl_log_TLogCtrl_ScrollToBottom(pLogCtrl);

	if (ShouldPutCursorToEnd)
	{
		axl_log_TCursorPos Pos = { MAXLONG, MAXLONG };
		axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, false); 
	}
}

void 
AXL_API
axl_log_TLogCtrl_FullUpdateLog(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TCacheMgr_ClearCache(&pLogCtrl->m_CacheMgr);
	
	pLogCtrl->m_LineCount = axl_log_TIndexFile_GetLineCount(&pLogCtrl->m_IndexFile);
	pLogCtrl->m_FirstVisibleLine = 0;
	pLogCtrl->m_FirstVisibleCol = 0;
	pLogCtrl->m_ColCount = 0;

	axl_log_TLogCtrl_RecalcVScroll(pLogCtrl);
	axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);
	axl_log_TLogCtrl_UpdateCursorPos(pLogCtrl);
	axl_log_TLogCtrl_Invalidate(pLogCtrl);
}

void
AXL_API
axl_log_TLogCtrl_UpdateLog(axl_log_TLogCtrl* pLogCtrl)
{
	size_t Anchor = axl_log_TIndexMgr_UpdateIndex(&pLogCtrl->m_IndexMgr);

	if (Anchor)
		axl_log_TLogCtrl_IncrementalUpdateLog(pLogCtrl);
	else
		axl_log_TLogCtrl_FullUpdateLog(pLogCtrl);
}

size_t
AXL_API
axl_log_TLogCtrl_FormatTime(
	axl_rtl_TString* pString,
	SYSTEMTIME* pSysTime,
	const tchar_t* pFormat
	)
{
	static tchar_t _WeekDayNameArray [][4] = 
	{ 
		_T("Sun"), _T("Mon"), _T("Tue"), _T("Wed"), _T("Thu"), _T("Fri"), _T("Sat")
	};

	static tchar_t _MonthNameArray [][4] = 
	{
		_T("Jan"), _T("Feb"), _T("Mar"), _T("Apr"),
		_T("May"), _T("Jun"), _T("Jul"), _T("Aug"),
		_T("Sep"), _T("Oct"), _T("Nov"), _T("Dec")
	};

	const tchar_t* p = pFormat;

	axl_rtl_TString_Clear(pString);

	for (; *p; p++)
	{
		if (*p != _T('%'))
		{
			axl_rtl_TString_AppendChar(pString, *p, 1);
			continue;
		}

		p++;
		if (!*p)
			break;

		switch (*p)
		{
		case _T('h'):
			axl_rtl_TString_AppendFormat(pString, _T("%02d"), pSysTime->wHour);
			break;

		case _T('H'):
			{
			int h = pSysTime->wHour % 12;
			axl_rtl_TString_AppendFormat(pString, _T("%d"), h ? h : 12);
			}
			break;

		case _T('t'):
			axl_rtl_TString_AppendChar(pString, pSysTime->wHour >= 12 ? _T('p') : _T('a'), 1);
			break;

		case _T('T'):
			axl_rtl_TString_AppendChar(pString, pSysTime->wHour >= 12 ? _T('P') : _T('A'), 1);
			break;

		case _T('m'):
			axl_rtl_TString_AppendFormat(pString, _T("%02d"), pSysTime->wMinute);
			break;

		case _T('s'):
			axl_rtl_TString_AppendFormat(pString, _T("%02d"), pSysTime->wSecond);
			break;

		case _T('l'):
			axl_rtl_TString_AppendFormat(pString, _T("%03d"), pSysTime->wMilliseconds);
			break;

		case _T('Y'):
			axl_rtl_TString_AppendFormat(pString, _T("%04d"), pSysTime->wYear + 1900);
			break;

		case _T('M'):
			axl_rtl_TString_AppendFormat(pString, _T("%02d"), pSysTime->wMonth);
			break;

		case _T('D'):
			axl_rtl_TString_AppendFormat(pString, _T("%02d"), pSysTime->wDay);
			break;

		case _T('W'):
			axl_rtl_TString_AppendString(pString, _WeekDayNameArray [pString, pSysTime->wDayOfWeek % 7], -1);
			break;

		case _T('N'):
			axl_rtl_TString_AppendString(pString, _MonthNameArray [pString, pSysTime->wMonth % 12], -1);
			break;

		default:
			axl_rtl_TString_AppendChar(pString, *p, 1);
		}
	}

	return axl_rtl_TString_GetLength(pString);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// imlpementation

bool_t
AXL_API
axl_log_TLogCtrl_IsVScrollAtBottom(axl_log_TLogCtrl* pLogCtrl)
{
	SCROLLINFO ScrollInfo = { sizeof(ScrollInfo) };
	ScrollInfo.fMask = SIF_ALL;
	GetScrollInfo(pLogCtrl->m_hWnd, SB_VERT, &ScrollInfo);
	return ScrollInfo.nPos + ScrollInfo.nPage == ScrollInfo.nMax + 1;
}

bool_t
AXL_API
axl_log_TLogCtrl_FixupFirstVisibleLine(axl_log_TLogCtrl* pLogCtrl)
{
	ulong_t NewFirstVisibleLine = pLogCtrl->m_FirstVisibleLine;

	if (pLogCtrl->m_FirstVisibleLine + pLogCtrl->m_VisibleLineCount > pLogCtrl->m_LineCount)
	{
		if (pLogCtrl->m_LineCount > pLogCtrl->m_VisibleLineCount)
			NewFirstVisibleLine = pLogCtrl->m_LineCount - pLogCtrl->m_VisibleLineCount;
		else
			NewFirstVisibleLine = 0;
	}

	if (NewFirstVisibleLine == pLogCtrl->m_FirstVisibleLine)
		return false;

	pLogCtrl->m_FirstVisibleLine = NewFirstVisibleLine;
	return true;
}

bool_t
AXL_API
axl_log_TLogCtrl_FixupFirstVisibleCol(axl_log_TLogCtrl* pLogCtrl)
{
	ulong_t NewFirstVisibleCol = pLogCtrl->m_FirstVisibleCol;
	ulong_t LastCol = pLogCtrl->m_BaseCol + pLogCtrl->m_ColCount;

	if (pLogCtrl->m_FirstVisibleCol + pLogCtrl->m_VisibleColCount > LastCol)
	{
		if (LastCol > pLogCtrl->m_VisibleColCount)
			NewFirstVisibleCol = LastCol - pLogCtrl->m_VisibleColCount;
		else
			NewFirstVisibleCol = 0;
	}

	if (NewFirstVisibleCol == pLogCtrl->m_FirstVisibleCol)
		return false;

	pLogCtrl->m_FirstVisibleCol = NewFirstVisibleCol;
	return true;	
}

void 
AXL_API
axl_log_TLogCtrl_RecalcBaseCol(axl_log_TLogCtrl* pLogCtrl)
{
	ulong_t BaseCol = 0;

	if (pLogCtrl->m_IsTimestampVisible)
		BaseCol += pLogCtrl->m_TimestampWidth;
	
	if (pLogCtrl->m_IsIconVisible)
		BaseCol += pLogCtrl->m_IconWidth;

	if (BaseCol != pLogCtrl->m_BaseCol)
	{
		pLogCtrl->m_BaseCol = BaseCol;
		axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);
	}
}

void 
AXL_API
axl_log_TLogCtrl_RecalcColCount(axl_log_TLogCtrl* pLogCtrl)
{
	ulong_t BinHexColCount = pLogCtrl->m_CacheMgr.m_LongestBinHexLine * 4 + axl_log_EDef_HexGapSize + pLogCtrl->m_FullOffsetWidth;
	ulong_t BinTextColCount = pLogCtrl->m_CacheMgr.m_LongestBinTextLine + pLogCtrl->m_FullOffsetWidth;
	ulong_t ColCount = pLogCtrl->m_CacheMgr.m_LongestTextLine;
	
	if (BinHexColCount > ColCount)
		ColCount = BinHexColCount;

	if (BinTextColCount > ColCount)
		ColCount = BinTextColCount;

	if (ColCount != pLogCtrl->m_ColCount)
	{
		pLogCtrl->m_ColCount = ColCount;
		axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);
	}
}

void 
AXL_API
axl_log_TLogCtrl_RecalcHScroll(axl_log_TLogCtrl* pLogCtrl)
{
	SCROLLINFO ScrollInfo = { sizeof(ScrollInfo) };
	ScrollInfo.fMask = SIF_ALL;
	ScrollInfo.nMin  = 0;
    ScrollInfo.nMax  = pLogCtrl->m_BaseCol + pLogCtrl->m_ColCount - 1;
    ScrollInfo.nPage = pLogCtrl->m_VisibleColCount;
    ScrollInfo.nPos  = pLogCtrl->m_FirstVisibleCol;
	SetScrollInfo(pLogCtrl->m_hWnd, SB_HORZ, &ScrollInfo, TRUE);
}

void 
AXL_API
axl_log_TLogCtrl_RecalcVScroll(axl_log_TLogCtrl* pLogCtrl)
{
	SCROLLINFO ScrollInfo = { sizeof(ScrollInfo) };
	ScrollInfo.fMask = SIF_ALL;
	ScrollInfo.nMin  = 0;
    ScrollInfo.nMax  = pLogCtrl->m_LineCount - 1;
    ScrollInfo.nPage = pLogCtrl->m_VisibleLineCount;
	ScrollInfo.nPos  = pLogCtrl->m_FirstVisibleLine;
	SetScrollInfo(pLogCtrl->m_hWnd, SB_VERT, &ScrollInfo, TRUE);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// cursor

void 
AXL_API
axl_log_TLogCtrl_UpdateCaretPos(axl_log_TLogCtrl* pLogCtrl)
{
	long Line;
	long Col;

	if (pLogCtrl->m_hWnd != GetFocus())
		return;

	Line = pLogCtrl->m_CursorPos.m_Line - pLogCtrl->m_FirstVisibleLine;
	Col = pLogCtrl->m_BaseCol + pLogCtrl->m_CursorPos.m_Col - pLogCtrl->m_FirstVisibleCol;
	SetCaretPos(Col * pLogCtrl->m_CharSize.cx, Line * pLogCtrl->m_CharSize.cy);
}

void
AXL_API
axl_log_TLogCtrl_GetCursorPosFromMousePos(
	axl_log_TLogCtrl* pLogCtrl,
	long x, 
	long y,
	axl_log_TCursorPos* pPos,
	bool_t AdjustX
	)
{
	if (AdjustX)
		x += pLogCtrl->m_CharSize.cx / 2;

	pPos->m_Line = y / pLogCtrl->m_CharSize.cy + pLogCtrl->m_FirstVisibleLine;
	pPos->m_Col = x / pLogCtrl->m_CharSize.cx + pLogCtrl->m_FirstVisibleCol - pLogCtrl->m_BaseCol;
}

void
AXL_API
axl_log_TLogCtrl_GetMousePosFromCursorPos(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos,
	POINT* pPoint
	)
{
	pPoint->x = (pLogCtrl->m_BaseCol + pPos->m_Col - pLogCtrl->m_FirstVisibleCol) * pLogCtrl->m_CharSize.cx;
	pPoint->y = (pPos->m_Line - pLogCtrl->m_FirstVisibleLine + 1) * pLogCtrl->m_CharSize.cy;
}

void 
AXL_API
axl_log_TLogCtrl_ValidateCursorPos(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	)
{
	axl_log_TLine* pLine;
	axl_log_TTextLine* pTextLine;
	axl_log_TBinLine* pBinHexLine;
	axl_log_TBinTextLine* pBinTextLine;

	size_t Length;

	if (!pLogCtrl->m_LineCount)
	{
		pPos->m_Line = 0;
		pPos->m_Col = 0;
		return;
	}

	if (pPos->m_Line >= (long) pLogCtrl->m_LineCount)
		pPos->m_Line = pLogCtrl->m_LineCount - 1;

	if (pPos->m_Line < 0)
		pPos->m_Line = 0;

	if (pPos->m_Col < 0)
		pPos->m_Col = 0;

    pLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, pPos->m_Line);

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		pTextLine = (axl_log_TTextLine*) pLine;

		Length = axl_rtl_TString_GetLength(&pTextLine->m_HyperText.m_Text);

		if (pPos->m_Col > (long) Length)
			pPos->m_Col = (long) Length;
		break;

	case axl_log_ELine_BinHex:
		pBinHexLine = (axl_log_TBinLine*) pLine;
		
		Length = axl_rtl_TByteArray_GetCount(&pBinHexLine->m_BinData);

		if (pPos->m_Col < (long) pLogCtrl->m_FullOffsetWidth)
			pPos->m_Col = 0;
		else
			pPos->m_Col -= pLogCtrl->m_FullOffsetWidth;
		
		if (pPos->m_Col <= (long) pBinHexLine->m_DataConfig.m_BinHexLineSize * 3)
		{
			size_t End = (Length - 1) * 3 + 2;
			
			if (pPos->m_Col >= (long) End)
				pPos->m_Col = (long) End;
		}
		else
		{
			long AsciiCol = pBinHexLine->m_DataConfig.m_BinHexLineSize * 3 + axl_log_EDef_HexGapSize;

			if (pPos->m_Col < AsciiCol)
				pPos->m_Col = AsciiCol;

			if (pPos->m_Col - AsciiCol > (long) Length)
				pPos->m_Col = AsciiCol + (long) Length;
		}

		pPos->m_Col += pLogCtrl->m_FullOffsetWidth;

		break;

	case axl_log_ELine_BinText:
		pBinTextLine = (axl_log_TBinTextLine*) pLine;

		Length = axl_rtl_TString_GetLength(&pBinTextLine->m_BinText);
		ASSERT(axl_rtl_TArray_GetCount(&pBinTextLine->m_BinTextMap) == Length);

		if (pPos->m_Col < (long) pLogCtrl->m_FullOffsetWidth)
			pPos->m_Col = 0;
		else
			pPos->m_Col -= pLogCtrl->m_FullOffsetWidth;

		if (pPos->m_Col >= (long) Length)
			pPos->m_Col = (long) Length;
		else		
			pPos->m_Col = (long) ((axl_log_TLogBinTextMapEntry*) pBinTextLine->m_BinTextMap.m_p)[pPos->m_Col].m_Col;

		pPos->m_Col += pLogCtrl->m_FullOffsetWidth;

		break;
	}
}

size_t
AXL_API
axl_log_TLogCtrl_GetBinTextLineOffset(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinTextLine* pLine,
	long Col
	)
{
	size_t ColCount = axl_rtl_TArray_GetCount(&pLine->m_BinTextMap);
	if (!ColCount)
		return 0;

	if (Col < (long) pLogCtrl->m_FullOffsetWidth)
		Col = 0;
	else
		Col -= pLogCtrl->m_FullOffsetWidth;

	if (Col >= (long) ColCount)
		return axl_rtl_TByteArray_GetCount(&pLine->m_BinData);

	return ((axl_log_TLogBinTextMapEntry*) pLine->m_BinTextMap.m_p)[Col].m_Offset;
}

size_t
AXL_API
axl_log_TLogCtrl_GetBinHexLineOffset(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine,
	long Col,
	int* pHexCol
	)
{
	size_t Offset;
	int HexCol;

	size_t Size = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);

	if (Col < (long) pLogCtrl->m_FullOffsetWidth)
		Col = 0;
	else
		Col -= pLogCtrl->m_FullOffsetWidth;
	
	if (Col <= (long) pLine->m_DataConfig.m_BinHexLineSize * 3)
	{
		size_t End = (Size - 1) * 3 + 2;
		
		if (Col >= (long) End)
			Col = (long) End;

		Offset = Col / 3;
		HexCol = Col % 3;
	}
	else
	{
		long AsciiCol = pLine->m_DataConfig.m_BinHexLineSize * 3 + axl_log_EDef_HexGapSize;

		if (Col < AsciiCol)
			Col = AsciiCol;

		if (Col - AsciiCol < (long) Size)
		{
			Offset = Col - AsciiCol;
			HexCol = 0;
		}
		else
		{
			Offset = Size - 1;
			HexCol = 2;
		}
	}

	if (pHexCol)
		*pHexCol = HexCol;

	return Offset;
}

bool_t
AXL_API
axl_log_TLogCtrl_GetBinLineOffset(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	long Col,
	size_t* pOffset,
	size_t* pLineOffset,
	int* pHexCol,
	ulong_t* pMergeId
	)
{
	size_t LineOffset;

	switch (pLine->m_Type)
	{
	case axl_log_ELine_BinHex:
		LineOffset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, (axl_log_TBinLine*) pLine, Col, pHexCol);
		break;

	case axl_log_ELine_BinText:
		LineOffset = axl_log_TLogCtrl_GetBinTextLineOffset(pLogCtrl, (axl_log_TBinTextLine*) pLine, Col);
		if (pHexCol)
			*pHexCol = 0;
		break;

	default:
		return false;
	}

	if (pLineOffset)
		*pLineOffset = LineOffset;

	if (pOffset)
		*pOffset = ((axl_log_TBinLine*) pLine)->m_Offset + LineOffset;

	if (pMergeId)
		*pMergeId = pLine->m_MergeId;

	return true;
}

bool_t
AXL_API
axl_log_TLogCtrl_GetBinOffset(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos,
	size_t* pOffset,
	size_t* pLineOffset,
	int* pHexCol,
	ulong_t* pMergeId
	)
{
	axl_log_TLine* pLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, pPos->m_Line);
	return pLine ? axl_log_TLogCtrl_GetBinLineOffset(pLogCtrl, pLine, pPos->m_Col, pOffset, pLineOffset, pHexCol, pMergeId) : false;
}

bool_t
AXL_API
axl_log_TLogCtrl_GetRangeBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd,
	size_t* pOffset,
	size_t* pSize
	)
{
	size_t StartOffset = -1;
	ulong_t StartMergeId = 0;

	size_t EndOffset = -1;
	ulong_t EndMergeId = 0;
	int EndHexCol = 0;

	int Cmp;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosStart);
	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosEnd);

	Cmp = axl_log_TCursorPos_Cmp(pPosStart, pPosEnd);
	if (Cmp > 0)
	{
		axl_log_TCursorPos* pTmp = pPosStart;
		pPosStart = pPosEnd;
		pPosEnd = pTmp;
	}

	axl_log_TLogCtrl_GetBinOffset(pLogCtrl, pPosStart, &StartOffset, NULL, NULL, &StartMergeId);
	axl_log_TLogCtrl_GetBinOffset(pLogCtrl, pPosEnd, &EndOffset, NULL, &EndHexCol, &EndMergeId);

	if (StartOffset == -1 || EndOffset == -1 || StartMergeId != EndMergeId)
	{
		if (pOffset)
			*pOffset = -1;

		if (pSize)
			*pSize = 0;

		return false;
	}

	if (Cmp != 0 && EndHexCol)
		EndOffset++;

	if (pOffset)
		*pOffset = StartOffset;

	if (pSize)
		*pSize = EndOffset - StartOffset;

	return true;
}

bool_t
AXL_API
axl_log_TLogCtrl_GetSelectionBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	size_t* pOffset,
	size_t* pSize
	)
{
	return axl_log_TLogCtrl_GetRangeBinBlock(pLogCtrl, &pLogCtrl->m_SelStart, &pLogCtrl->m_SelEnd, pOffset, pSize);
}

bool_t
AXL_API
axl_log_TLogCtrl_IncapsulateBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	const void* p,
	size_t Size,
	axl_log_FProcessBinBlock pfnProcess, 
	void* pContext
	)
{
	size_t BufferSize;

	// normally Size (from a single line) should be much less than axl_log_EDef_MaxBinBlockBuffer

	axl_rtl_TByteArray_Append(&pLogCtrl->m_BinBlockBuffer, p, Size);
	BufferSize = axl_rtl_TByteArray_GetCount(&pLogCtrl->m_BinBlockBuffer);

	if (BufferSize < axl_log_EDef_MaxBinBlockBuffer)
		return false;

	pfnProcess(pContext, pLogCtrl->m_BinBlockBuffer.m_p, BufferSize);

	axl_rtl_TByteArray_Clear(&pLogCtrl->m_BinBlockBuffer);
	return true;
}

size_t
AXL_API
axl_log_TLogCtrl_ProcessRangeAsBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd,
	axl_log_FProcessBinBlock pfnProcess, 
	void* pContext
	)
{
	axl_log_TBinLine* pLine;
	long i;
	size_t LineSize;

	size_t StartFullOffset = -1;
	size_t StartLineOffset = -1;
	ulong_t StartMergeId = 0;
	int StartHexCol;

	size_t EndFullOffset = -1;
	size_t EndLineOffset = -1;
	ulong_t EndMergeId = 0;
	int EndHexCol;

	int Cmp;

	size_t BufferSize;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosStart);
	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosEnd);

	Cmp = axl_log_TCursorPos_Cmp(pPosStart, pPosEnd);
	if (Cmp == 0)
		return 0;

	if (Cmp > 0)
	{
		axl_log_TCursorPos* pTmp = pPosStart;
		pPosStart = pPosEnd;
		pPosEnd = pTmp;
	}

	axl_log_TLogCtrl_GetBinOffset(pLogCtrl, pPosStart, &StartFullOffset, &StartLineOffset, &StartHexCol, &StartMergeId);
	axl_log_TLogCtrl_GetBinOffset(pLogCtrl, pPosEnd, &EndFullOffset, &EndLineOffset, &EndHexCol, &EndMergeId);

	if (StartFullOffset == -1 || EndFullOffset == -1 || StartMergeId != EndMergeId)
		return 0;

	if (EndHexCol)
	{
		EndFullOffset++;
		EndLineOffset++;
	}

    // first line

	pLine = (axl_log_TBinLine*) axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, pPosStart->m_Line);
	ASSERT(axl_log_TLine_IsBin(pLine));

	LineSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
	ASSERT(StartLineOffset < LineSize);

	if (pPosStart->m_Line == pPosEnd->m_Line) // special case -- single line
	{
		pfnProcess(pContext, (uchar_t*) pLine->m_BinData.m_p + StartLineOffset, EndLineOffset - StartLineOffset);
		return EndFullOffset - StartFullOffset;
	}

	axl_rtl_TByteArray_Clear(&pLogCtrl->m_BinBlockBuffer);

	axl_log_TLogCtrl_IncapsulateBinBlock(pLogCtrl, (uchar_t*) pLine->m_BinData.m_p + StartLineOffset, LineSize - StartLineOffset, pfnProcess, pContext);

	// middle lines

	for (i = pPosStart->m_Line + 1; i < pPosEnd->m_Line; i++)
	{
		pLine = (axl_log_TBinLine*) axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
		ASSERT(axl_log_TLine_IsBin(pLine));

		LineSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
		axl_log_TLogCtrl_IncapsulateBinBlock(pLogCtrl, pLine->m_BinData.m_p, LineSize, pfnProcess, pContext);
	}

	// last line

	pLine = (axl_log_TBinLine*) axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
	ASSERT(axl_log_TLine_IsBin(pLine));
	axl_log_TLogCtrl_IncapsulateBinBlock(pLogCtrl, pLine->m_BinData.m_p, EndLineOffset, pfnProcess, pContext);

	BufferSize = axl_rtl_TByteArray_GetCount(&pLogCtrl->m_BinBlockBuffer);
	if (BufferSize)
		pfnProcess(pContext, pLogCtrl->m_BinBlockBuffer.m_p, BufferSize);

	axl_rtl_TByteArray_Clear(&pLogCtrl->m_BinBlockBuffer);

	return EndFullOffset - StartFullOffset;
}

size_t
AXL_API
axl_log_TLogCtrl_ProcessRange(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd,
	axl_log_FProcessLine pfnProcess, 
	void* pContext
	)
{
	axl_log_TLine* pLine;
	axl_log_TLine* pPrevLine;
	long i;

	int Cmp;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosStart);
	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosEnd);

	Cmp = axl_log_TCursorPos_Cmp(pPosStart, pPosEnd);
	if (Cmp == 0)
		return 0;

	if (Cmp > 0)
	{
		axl_log_TCursorPos* pTmp = pPosStart;
		pPosStart = pPosEnd;
		pPosEnd = pTmp;
	}

    // first line

	pLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, pPosStart->m_Line);
	if (pPosStart->m_Line == pPosEnd->m_Line) // special case -- single line
	{
		pfnProcess(pContext, pLogCtrl, pLine, NULL, pPosStart->m_Col, pPosEnd->m_Col);
		return 1;
	}

	pfnProcess(pContext, pLogCtrl, pLine, NULL, pPosStart->m_Col, -1);

	// middle lines

	for (i = pPosStart->m_Line + 1; i < pPosEnd->m_Line; i++)
	{
		pPrevLine = pLine;
		pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
		pfnProcess(pContext, pLogCtrl, pLine, pPrevLine, 0, -1);
	}

	// last line

	pPrevLine = pLine;
	pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
	pfnProcess(pContext, pLogCtrl, pLine, pPrevLine, 0, pPosEnd->m_Col);

	return pPosEnd->m_Line - pPosStart->m_Line + 1;
}

size_t
AXL_API
axl_log_TLogCtrl_ProcessSelection(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_FProcessLine pfnProcess, 
	void* pContext
	)
{
	return axl_log_TLogCtrl_ProcessRange(pLogCtrl, &pLogCtrl->m_SelStart, &pLogCtrl->m_SelEnd, pfnProcess, pContext);
}

size_t
AXL_API
axl_log_TLogCtrl_ProcessSelectionAsBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_FProcessBinBlock pfnProcess, 
	void* pContext
	)
{
	return axl_log_TLogCtrl_ProcessRangeAsBinBlock(pLogCtrl, &pLogCtrl->m_SelStart, &pLogCtrl->m_SelEnd, pfnProcess, pContext);
}

bool_t
AXL_API
axl_log_TLogCtrl_SaveAsTextFile(
	axl_log_TLogCtrl* pLogCtrl,
	const tchar_t* pFileName,
	axl_rtl_TBinTree* pIconToCharMap
	)
{
	axl_TStatus Status;
	axl_rtl_TString String;
	size_t Length;
	axl_log_TLine* pLine;

	axl_sys_TFile File;
	axl_sys_TFile_Construct(&File);

	Status = axl_sys_TFile_Open(&File, pFileName, 0);
	if (Status != axl_EStatus_Success)
	{
		axl_sys_TFile_Destruct(&File);
		return false;
	}

	axl_sys_TFile_SetSize(&File, 0);
	axl_rtl_TString_Construct(&String);

	pLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, 0);
	while (pLine)
	{
		axl_log_TLogCtrl_GetLineString(pLogCtrl, pLine, &String, pIconToCharMap);
		Length = axl_rtl_TString_GetLength(&String);
		axl_sys_TFile_Write(&File, String.m_p, Length * sizeof(tchar_t));
		axl_sys_TFile_Write(&File, _T("\r\n"), 2 * sizeof(tchar_t));
		pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
	}

	axl_rtl_TString_Destruct(&String);
	axl_sys_TFile_Destruct(&File);
	return true;
}

void 
AXL_API
axl_log_TLogCtrl_SetCursorPos(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos,
	bool_t Select
	)
{
	axl_log_TLine* pNewLine;
	axl_log_TCursorPos OldPos = pLogCtrl->m_CursorPos;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPos);
	
	if (axl_log_TCursorPos_Cmp(pPos, &pLogCtrl->m_CursorPos) == 0)
		return;

	// handle selection

	OldPos = pLogCtrl->m_CursorPos;
	pLogCtrl->m_CursorPos = *pPos;

	axl_log_TLogCtrl_UpdateCaretPos(pLogCtrl);

	if (Select)
	{
		if (!pLogCtrl->m_IsSelecting)
		{
			pLogCtrl->m_IsSelecting = true;
			axl_log_TLogCtrl_SetSelStart(pLogCtrl, &OldPos);
		}
		
		axl_log_TLogCtrl_SetSelEnd(pLogCtrl, pPos);
	}
	else
	{
		axl_log_TLogCtrl_KillSelection(pLogCtrl);

		pLogCtrl->m_SelStart = *pPos;
		pLogCtrl->m_SelEnd = *pPos;

		pNewLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, pPos->m_Line);
		if (pNewLine && pNewLine->m_Type == axl_log_ELine_BinHex)
		{
			axl_log_TCursorPos PosStart = pLogCtrl->m_CursorPos;
			axl_log_TCursorPos PosEnd = pLogCtrl->m_CursorPos;

			int HexCol;
			size_t Offset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, (axl_log_TBinLine*) pNewLine, PosStart.m_Col, &HexCol);

			if (HexCol == 2)
				PosStart.m_Col--;
			else
				PosEnd.m_Col++;

			axl_log_TLogCtrl_SetHilite(pLogCtrl, &PosStart, &PosEnd);
		}
		else
		{
			axl_log_TLogCtrl_KillHilite(pLogCtrl);
		}
	}

	axl_log_TLogCtrl_NotifyParent(pLogCtrl, axl_log_ENm_CursorPosChange, NULL);
}

void 
AXL_API
axl_log_TLogCtrl_SetHilite(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd
	)
{
	axl_log_TCursorPos OldHiliteStart;
	axl_log_TCursorPos OldHiliteEnd;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosStart);
	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosEnd);

	if (axl_log_TCursorPos_Cmp(pPosStart, &pLogCtrl->m_HiliteStart) == 0 &&
		axl_log_TCursorPos_Cmp(pPosEnd, &pLogCtrl->m_HiliteEnd) == 0)
		return;

	if (axl_log_TCursorPos_Cmp(pPosStart, pPosEnd) > 0)
	{
		axl_log_TCursorPos* pTmp = pPosStart;
		pPosStart = pPosEnd;
		pPosEnd = pTmp;			
	}

	OldHiliteStart = pLogCtrl->m_HiliteStart;
	OldHiliteEnd = pLogCtrl->m_HiliteEnd;

	pLogCtrl->m_HiliteStart = *pPosStart;
	pLogCtrl->m_HiliteEnd = *pPosEnd;

	axl_log_TLogCtrl_InvalidateLineRange(
		pLogCtrl, 
		min(OldHiliteStart.m_Line, pPosStart->m_Line),
		max(OldHiliteEnd.m_Line, pPosEnd->m_Line)
		);
}

ulong_t 
AXL_API
axl_log_TLogCtrl_GetHiliteCol(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine,
	size_t Offset
	)
{
	return pLine->m_Type == axl_log_ELine_BinHex ?
		pLogCtrl->m_FullOffsetWidth + (ulong_t) Offset * 3 :
		pLogCtrl->m_FullOffsetWidth + (ulong_t) axl_log_TBinTextLine_FindColByOffset((axl_log_TBinTextLine*) pLine, Offset);
}

bool_t
AXL_API
axl_log_TLogCtrl_HiliteBinRange(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	size_t Offset,
	size_t Length
	)
{
	ulong_t Line;
	axl_log_TCursorPos PosStart;
	axl_log_TCursorPos PosEnd;

	axl_log_TCachePage* pPage = pLine->m_pPage;
	size_t EndOffset = Offset + Length;
	
	axl_log_TBinLine* pBinLine;

	if (!pLine->m_pVolatileMessage)
        return false;

	if (!axl_log_TLine_IsBin(pLine)) // scan forward until we bump into bin line
	{
		while (pLine && !axl_log_TLine_IsBin(pLine))
			pLine = axl_log_TCachePage_GetLine(pPage, pLine->m_PageIndex + 1);

		if (!pLine)
			return false;
	}

	// scan backward to the beginning of the block

	for (;;) 
	{
		axl_log_TLine* pPrevLine = pLine->m_PageIndex ? axl_log_TCachePage_GetLine(pPage, pLine->m_PageIndex - 1) : 0;
		if (!pPrevLine || !axl_log_TLine_IsMerged(pPrevLine, pLine))
			break;

		pLine = pPrevLine;
	} 

	pBinLine = (axl_log_TBinLine*) pLine;
	ASSERT(pBinLine->m_Offset == 0);

	Line = axl_log_TCacheMgr_GetCachePageStartLine(&pLogCtrl->m_CacheMgr, pPage);

	// scan forward to the beginning of the range 

	pBinLine = axl_log_TCachePage_FindBinLineByOffset(pBinLine->m_pPage, pBinLine, Offset);
	if (!pBinLine)
		return false;

	PosStart.m_Line = Line + (long) pBinLine->m_PageIndex;
	PosStart.m_Col = axl_log_TLogCtrl_GetHiliteCol(pLogCtrl, pBinLine, Offset - pBinLine->m_Offset);

	// scan forward to the end of the range 

	pBinLine = axl_log_TCachePage_FindBinLineByOffset(pBinLine->m_pPage, pBinLine, EndOffset);
	if (!pBinLine)
		return false;

	PosEnd.m_Line = Line + (long) pBinLine->m_PageIndex;
	PosEnd.m_Col = axl_log_TLogCtrl_GetHiliteCol(pLogCtrl, pBinLine, EndOffset - pBinLine->m_Offset);

	axl_log_TLogCtrl_KillSelection(pLogCtrl);
	axl_log_TLogCtrl_SetHilite(pLogCtrl, &PosStart, &PosEnd);
	return true;
}

bool_t
AXL_API
axl_log_TLogCtrl_SelectBinRange(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine,
	size_t Offset,
	size_t Length
	)
{
	ulong_t Line;
	axl_log_TCursorPos PosStart;
	axl_log_TCursorPos PosEnd;

	axl_log_TCachePage* pPage = pLine->m_pPage;
	size_t EndOffset = Offset + Length;
	
	size_t LineSize;

	// scan backward 

	while (pLine->m_Offset > Offset) 
	{
		axl_log_TLine* pPrevLine = axl_log_TCacheMgr_GetPrevLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
		if (!pPrevLine || !axl_log_TLine_IsMerged(pPrevLine, (axl_log_TLine*) pLine))
			return false;

		pLine = (axl_log_TBinLine*) pPrevLine;
	} 

	Line = axl_log_TCacheMgr_GetAbsoluteLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);

	// scan forward to the beginning of the range (if anchor line was given properly, cycle will stop immediatly)

	for (;;) 
	{
		axl_log_TLine* pNextLine;
		
		LineSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
		if (Offset >= pLine->m_Offset && Offset <= pLine->m_Offset + LineSize)
		{
			PosStart.m_Line = Line;
			PosStart.m_Col = axl_log_TLogCtrl_GetHiliteCol(pLogCtrl, pLine, Offset - pLine->m_Offset);
			break;
		}

		pNextLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
		if (!pNextLine || !axl_log_TLine_IsMerged((axl_log_TLine*) pLine, pNextLine))
			return false;

		pLine = (axl_log_TBinLine*) pNextLine;
		Line++;
	} 

	// scan forward to the end of the range 

	for (;;) 
	{
		axl_log_TLine* pNextLine;
		
		LineSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
		if (EndOffset >= pLine->m_Offset && EndOffset <= pLine->m_Offset + LineSize)
		{
			PosEnd.m_Line = Line;
			PosEnd.m_Col = axl_log_TLogCtrl_GetHiliteCol(pLogCtrl, pLine, EndOffset - pLine->m_Offset);
			break;
		}

		pNextLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
		if (!pNextLine || !axl_log_TLine_IsMerged((axl_log_TLine*) pLine, pNextLine))
			return false;

		pLine = (axl_log_TBinLine*) pNextLine;
		Line++;
	} 

	axl_log_TLogCtrl_SetSelection(pLogCtrl, &PosStart, &PosEnd);
	axl_log_TLogCtrl_EnsureVisibleRange(pLogCtrl, &PosStart, &PosEnd);
	
	pLogCtrl->m_CursorPos = PosEnd;
	axl_log_TLogCtrl_UpdateCaretPos(pLogCtrl);
	
	return true;
}

void 
AXL_API
axl_log_TLogCtrl_KillHilite(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TCursorPos OldHiliteStart = pLogCtrl->m_HiliteStart;
	axl_log_TCursorPos OldHiliteEnd = pLogCtrl->m_HiliteEnd;
	
	pLogCtrl->m_HiliteStart.m_Line = 0;
	pLogCtrl->m_HiliteStart.m_Col = 0;
	pLogCtrl->m_HiliteEnd.m_Line = 0;
	pLogCtrl->m_HiliteEnd.m_Col = 0;

	if (axl_log_TCursorPos_Cmp(&OldHiliteStart, &OldHiliteEnd) != 0) 
		axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, OldHiliteStart.m_Line, OldHiliteEnd.m_Line);
}

void 
AXL_API
axl_log_TLogCtrl_SetSelStart(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	)
{
	long OldLine = pLogCtrl->m_SelStart.m_Line;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPos);

	if (axl_log_TCursorPos_Cmp(pPos, &pLogCtrl->m_SelStart) == 0)
		return;
	
	pLogCtrl->m_SelStart = *pPos;

	axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, pPos->m_Line, OldLine);
}

void 
AXL_API
axl_log_TLogCtrl_SetSelEnd(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	)
{
	long OldLine = pLogCtrl->m_SelEnd.m_Line;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPos);
	
	if (axl_log_TCursorPos_Cmp(pPos, &pLogCtrl->m_SelEnd) == 0)
		return;
	
	pLogCtrl->m_SelEnd = *pPos;

	axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, pPos->m_Line, OldLine);
}

bool_t 
AXL_API
axl_log_TLogCtrl_GetSelection(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pSelStart, 
	axl_log_TCursorPos* pSelEnd
	)
{
	int Cmp = axl_log_TCursorPos_Cmp(&pLogCtrl->m_SelStart, &pLogCtrl->m_SelEnd);

	if (Cmp == 0)
	{
		*pSelStart = pLogCtrl->m_CursorPos;
		*pSelEnd = pLogCtrl->m_CursorPos;
		return false;
	}
	
	if (Cmp < 0)
	{
		*pSelStart = pLogCtrl->m_SelStart;
		*pSelEnd = pLogCtrl->m_SelEnd;
	}
	else
	{
		*pSelStart = pLogCtrl->m_SelEnd;
		*pSelEnd = pLogCtrl->m_SelStart;
	}

	return true;
}
void 
AXL_API
axl_log_TLogCtrl_KillSelection(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TCursorPos OldSelStart = pLogCtrl->m_SelStart;
	axl_log_TCursorPos OldSelEnd = pLogCtrl->m_SelEnd;
	
	pLogCtrl->m_SelStart.m_Line = 0;
	pLogCtrl->m_SelStart.m_Col = 0;
	pLogCtrl->m_SelEnd.m_Line = 0;
	pLogCtrl->m_SelEnd.m_Col = 0;
	pLogCtrl->m_IsSelecting = false;

	if (axl_log_TCursorPos_Cmp(&OldSelStart, &OldSelEnd) != 0) 
		axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, OldSelStart.m_Line, OldSelEnd.m_Line);
}

void 
AXL_API
axl_log_TLogCtrl_EnsureVisible(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	)
{
	long Col = pPos->m_Col + pLogCtrl->m_BaseCol;

	ulong_t OldFirstVisibleLine = pLogCtrl->m_FirstVisibleLine;
	ulong_t OldFirstVisibleCol = pLogCtrl->m_FirstVisibleCol;

	if ((ulong_t) pPos->m_Line >= pLogCtrl->m_FirstVisibleLine + pLogCtrl->m_VisibleLineCount)
		pLogCtrl->m_FirstVisibleLine = pPos->m_Line - pLogCtrl->m_VisibleLineCount + 1;

	if ((ulong_t) pPos->m_Line < pLogCtrl->m_FirstVisibleLine)
		pLogCtrl->m_FirstVisibleLine = pPos->m_Line;

	if ((ulong_t) Col + 1 >= pLogCtrl->m_FirstVisibleCol + pLogCtrl->m_VisibleColCount)
		pLogCtrl->m_FirstVisibleCol = Col + 1 - pLogCtrl->m_VisibleColCount + 1; 

	if ((ulong_t) Col < pLogCtrl->m_FirstVisibleCol)
		pLogCtrl->m_FirstVisibleCol = Col;

	axl_log_TLogCtrl_FixupFirstVisibleLine(pLogCtrl);
	axl_log_TLogCtrl_FixupFirstVisibleCol(pLogCtrl);

	if (OldFirstVisibleLine != pLogCtrl->m_FirstVisibleLine ||
		OldFirstVisibleCol != pLogCtrl->m_FirstVisibleCol)
	{
		axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);
		axl_log_TLogCtrl_RecalcVScroll(pLogCtrl);
		axl_log_TLogCtrl_UpdateCaretPos(pLogCtrl);
		axl_log_TLogCtrl_Invalidate(pLogCtrl);
	}
}

void 
AXL_API
axl_log_TLogCtrl_EnsureVisibleRange(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd
	)
{
	long ColStart = pPosStart->m_Col + pLogCtrl->m_BaseCol;
	long ColEnd = pPosEnd->m_Col + pLogCtrl->m_BaseCol;

	ulong_t OldFirstVisibleLine = pLogCtrl->m_FirstVisibleLine;
	ulong_t OldFirstVisibleCol = pLogCtrl->m_FirstVisibleCol;

	// end first

	if ((ulong_t) pPosEnd->m_Line >= pLogCtrl->m_FirstVisibleLine + pLogCtrl->m_VisibleLineCount)
		pLogCtrl->m_FirstVisibleLine = pPosEnd->m_Line - pLogCtrl->m_VisibleLineCount + 1;

	if ((ulong_t) pPosEnd->m_Line < pLogCtrl->m_FirstVisibleLine)
		pLogCtrl->m_FirstVisibleLine = pPosEnd->m_Line;

	if ((ulong_t) ColEnd + 1 >= pLogCtrl->m_FirstVisibleCol + pLogCtrl->m_VisibleColCount)
		pLogCtrl->m_FirstVisibleCol = ColEnd + 1 - pLogCtrl->m_VisibleColCount + 1; 

	if ((ulong_t) ColEnd < pLogCtrl->m_FirstVisibleCol)
		pLogCtrl->m_FirstVisibleCol = ColEnd;

	// then start (so beginning of range is visible)

	if ((ulong_t) pPosStart->m_Line >= pLogCtrl->m_FirstVisibleLine + pLogCtrl->m_VisibleLineCount)
		pLogCtrl->m_FirstVisibleLine = pPosStart->m_Line - pLogCtrl->m_VisibleLineCount + 1;

	if ((ulong_t) pPosStart->m_Line < pLogCtrl->m_FirstVisibleLine)
		pLogCtrl->m_FirstVisibleLine = pPosStart->m_Line;

	if ((ulong_t) ColStart + 1 >= pLogCtrl->m_FirstVisibleCol + pLogCtrl->m_VisibleColCount)
		pLogCtrl->m_FirstVisibleCol = ColStart + 1 - pLogCtrl->m_VisibleColCount + 1; 

	if ((ulong_t) ColStart < pLogCtrl->m_FirstVisibleCol)
		pLogCtrl->m_FirstVisibleCol = ColStart;

	axl_log_TLogCtrl_FixupFirstVisibleLine(pLogCtrl);
	axl_log_TLogCtrl_FixupFirstVisibleCol(pLogCtrl);

	if (OldFirstVisibleLine != pLogCtrl->m_FirstVisibleLine ||
		OldFirstVisibleCol != pLogCtrl->m_FirstVisibleCol)
	{
		axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);
		axl_log_TLogCtrl_RecalcVScroll(pLogCtrl);
		axl_log_TLogCtrl_UpdateCaretPos(pLogCtrl);
		axl_log_TLogCtrl_Invalidate(pLogCtrl);
	}
}

/*

axl::rtl::CString axl_log_TLogCtrl_GetToolTipText(const CCursorPos& Pos)
{
	if (m_IsDragging)
		return axl::rtl::CString();

	if (Pos.m_Line < 0 || Pos.m_Line >= m_LineCount)
		return axl::rtl::CString();

	axl_log_TLine* pLine = GetLine(Pos.m_Line);

	if (!pLine->IsTextEx() || Pos.m_Col < 0 || (size_t) Pos.m_Col >= pLine->m_TextExMapping.GetCount())
		return axl::rtl::CString();

	size_t Position = pLine->m_TextExMapping [Pos.m_Col].m_TextExPosition;
	byte_t x = pLine->m_TextEx [Position];
	if (isprint(x))
		return axl::rtl::CString();

	axl::rtl::CString Text;
	Text.Format(_T("hex %02X (dec %d)"), x, x);
	return Text;
}

*/

void 
AXL_API
axl_log_TLogCtrl_HandleToolTip(
	axl_log_TLogCtrl* pLogCtrl,
	int x, 
	int y
	)
{
/*	CCursorPos Pos;
	Pos.m_Line = Point.y / m_CharSize.cy + m_FirstVisibleLine;
	Pos.m_Col = Point.x / m_CharSize.cx + m_FirstVisibleCol - m_BaseCol;

	if (Pos == m_LastToolTipPos)
		return;

	axl::rtl::CString ToolTipText = GetToolTipText(Pos);
	if (ToolTipText.IsEmpty())
	{
		m_ToolTip.Activate(FALSE);
		m_LastToolTipPos.m_Line = -1, m_LastToolTipPos.m_Col = -1;
	}
	else
	{
		m_LastToolTipPos = Pos;
		m_ToolTip.Activate(TRUE);
		m_ToolTip.UpdateTipText((const tchar_t*) ToolTipText, m_hWnd);
	} */
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Hyperlinks

bool_t
AXL_API
axl_log_TLogCtrl_GetHyperlinkFromMousePos(
	axl_log_TLogCtrl* pLogCtrl,
	long x,
	long y,
	axl_log_TLine** ppLine,
	const tchar_t** ppHyperlink
	)
{
	axl_log_TLine* pLine;
	axl_log_TTextLine* pTextLine;
	axl_gr_THyperlinkAnchor* pAnchor;

	axl_log_TCursorPos Pos;
	axl_log_TLogCtrl_GetCursorPosFromMousePos(pLogCtrl, x, y, &Pos, false);

	pLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, Pos.m_Line);
	if (!pLine || pLine->m_Type != axl_log_ELine_Text)
		return false;

	pTextLine = (axl_log_TTextLine*) pLine;

	pAnchor = axl_gr_THyperlinkAnchorArray_Find(&pTextLine->m_HyperText.m_HyperlinkArray, Pos.m_Col);
	if (!pAnchor || axl_rtl_TString_IsEmpty(&pAnchor->m_Hyperlink))
		return false;

	if (ppLine)
		*ppLine = pLine;

	if (ppHyperlink)
		*ppHyperlink = pAnchor->m_Hyperlink.m_p;

	return true;
}

void
AXL_API
axl_log_TLogCtrl_OnHyperlink(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	const tchar_t* pHyperlink
	)
{
	if (pLogCtrl->m_pHyperlinkHandler)
	{
		bool_t Result = axl_log_THyperlinkHandler_OnHyperlink(pLogCtrl->m_pHyperlinkHandler, pLogCtrl->m_hWnd, pLine, pHyperlink);
		if (Result)
			return;
	}

	switch (*pHyperlink)
	{
	case '+': // add/remove bits to volatile flags
	case '-': 
		{
		ulong_t Modify;
		ulong_t NewVolatileFlags;

		if (!pLine->m_pVolatileMessage)
			break;

		Modify = _tcstoul(pHyperlink + 1, NULL, 10);
		if (!Modify)
			break;

		NewVolatileFlags = *pHyperlink == '+' ? 
			pLine->m_pVolatileMessage->m_VolatileFlags | Modify :
			pLine->m_pVolatileMessage->m_VolatileFlags & ~Modify;

		axl_log_TLogCtrl_ModifyLineVolatileFlags(pLogCtrl, pLine, NewVolatileFlags);
		break;
		}

	case '!': // hilite bin range
		{
		tchar_t* pEnd;
		size_t Offset;
		size_t Length;

		Offset = _tcstoul(pHyperlink + 1, &pEnd, 10);
		if (!pEnd)
			break;

		if (*pEnd != ',' && *pEnd != ';')
			break;

		Length = _tcstoul(pEnd + 1, &pEnd, 10);
		if (!pEnd)
			break;

		axl_log_TLogCtrl_HiliteBinRange(pLogCtrl, pLine, Offset, Length);
		break;
		}

	default:
		ShellExecute(0, _T("open"), pHyperlink, 0, 0, SW_SHOWNORMAL);
	}	
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// message handlers

LRESULT
AXL_API
axl_log_TLogCtrl_NotifyParent(
	axl_log_TLogCtrl* pLogCtrl,
	uint_t Code,
	NMHDR* pNotify
	)
{
	HWND hWndParent = GetParent(pLogCtrl->m_hWnd);

	NMHDR Notify;
	if (!pNotify)
		pNotify = &Notify;

	pNotify->code = Code;
	pNotify->hwndFrom = pLogCtrl->m_hWnd;
	pNotify->idFrom = GetDlgCtrlID(pLogCtrl->m_hWnd);
	
	return SendMessage(hWndParent, WM_NOTIFY, pNotify->idFrom, (LPARAM) pNotify);
}


LRESULT 
AXL_API
axl_log_TLogCtrl_OnCreate(axl_log_TLogCtrl* pLogCtrl)
{	
	HINSTANCE hInstance = (HINSTANCE) (LONG_PTR) GetWindowLongPtr(pLogCtrl->m_hWnd, GWLP_HINSTANCE);
	TOOLINFO ToolInfo = { sizeof(ToolInfo) };

	pLogCtrl->m_hWndToolTip = CreateWindow(TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP, 0,0,0,0, pLogCtrl->m_hWnd, 0, hInstance, NULL);
	
	if (!pLogCtrl->m_hWndToolTip)
		return 0;

	ToolInfo.uFlags   = TTF_IDISHWND;
	ToolInfo.hwnd     = GetParent(pLogCtrl->m_hWnd); 
	ToolInfo.uId      = (UINT_PTR) pLogCtrl->m_hWnd;
    ToolInfo.hinst    = hInstance; 
	ToolInfo.lpszText = LPSTR_TEXTCALLBACK;  

	SendMessage(pLogCtrl->m_hWndToolTip, TTM_ADDTOOL, 0, (LPARAM) &ToolInfo);
	SendMessage(pLogCtrl->m_hWndToolTip, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
	SendMessage(pLogCtrl->m_hWndToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
	SendMessage(pLogCtrl->m_hWndToolTip, TTM_SETDELAYTIME, TTDT_INITIAL, 200);
	SendMessage(pLogCtrl->m_hWndToolTip, TTM_SETDELAYTIME, TTDT_RESHOW, 200); 
			
	return 0;
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnDestroy(axl_log_TLogCtrl* pLogCtrl)
{
	DestroyWindow(pLogCtrl->m_hWndToolTip);
	pLogCtrl->m_hWndToolTip = NULL;
	return 0;
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnSetFocus(axl_log_TLogCtrl* pLogCtrl)
{
	CreateCaret(pLogCtrl->m_hWnd, (HBITMAP) 0, axl_log_EDef_CaretWidth, pLogCtrl->m_CharSize.cy);
	ShowCaret(pLogCtrl->m_hWnd);
	axl_log_TLogCtrl_UpdateCaretPos(pLogCtrl);
	return 0;
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnKillFocus(axl_log_TLogCtrl* pLogCtrl)
{
	HideCaret(pLogCtrl->m_hWnd);
	DestroyCaret();
	return 0;
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnPaint(axl_log_TLogCtrl* pLogCtrl)
{
	PAINTSTRUCT PaintStruct;
	HDC hdc = BeginPaint(pLogCtrl->m_hWnd, &PaintStruct);
	axl_log_TLogCtrl_PaintRect(pLogCtrl, hdc, &PaintStruct.rcPaint);
	EndPaint(pLogCtrl->m_hWnd, &PaintStruct);
	return 0;
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnSize(axl_log_TLogCtrl* pLogCtrl)
{
	bool_t DidFirstVisibleLineChange;
	bool_t DidFirstVisibleColChange;

	bool_t ShouldScrollToBottom = axl_log_TLogCtrl_IsCursorAtLastLine(pLogCtrl); 
	// bool_t ShouldScrollToBottom = axl_log_TLogCtrl_IsVScrollAtBottom(pLogCtrl);
	
	RECT ClientRect;
	GetClientRect(pLogCtrl->m_hWnd, &ClientRect);

	pLogCtrl->m_VisibleLineCount = ClientRect.bottom / pLogCtrl->m_CharSize.cy;
	pLogCtrl->m_VisibleColCount = ClientRect.right / pLogCtrl->m_CharSize.cx;

	DidFirstVisibleLineChange = axl_log_TLogCtrl_FixupFirstVisibleLine(pLogCtrl);
	DidFirstVisibleColChange = axl_log_TLogCtrl_FixupFirstVisibleCol(pLogCtrl);

	axl_log_TLogCtrl_RecalcVScroll(pLogCtrl);
	axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);

	if (ShouldScrollToBottom)
		axl_log_TLogCtrl_ScrollToBottom(pLogCtrl);

	if (DidFirstVisibleLineChange || DidFirstVisibleColChange)
	{
		axl_log_TLogCtrl_Invalidate(pLogCtrl);
		axl_log_TLogCtrl_UpdateCaretPos(pLogCtrl);
	}

	return 0;
}

void
AXL_API
axl_log_TLogCtrl_OnScroll(
	axl_log_TLogCtrl* pLogCtrl, 
	int Bar,
	int Code
	)
{
	int NewPos;
	int MaxPos;

	SCROLLINFO ScrollInfo = { sizeof(ScrollInfo) };
	ScrollInfo.fMask = SIF_ALL;
	
	GetScrollInfo(pLogCtrl->m_hWnd, Bar, &ScrollInfo);

	NewPos = ScrollInfo.nPos;
	MaxPos = ScrollInfo.nMax - ScrollInfo.nPage + 1;
	
	switch (Code)
	{
	case SB_TOP:
		NewPos = 0;
		break;

	case SB_LINEUP:
		NewPos--;
		break;

	case SB_LINEDOWN:
		NewPos++;
		break;

	case SB_PAGEUP:
		NewPos -= ScrollInfo.nPage; 
		break;

	case SB_PAGEDOWN:
		NewPos += ScrollInfo.nPage; 
		break;

	case SB_BOTTOM:
		NewPos = ScrollInfo.nMax; 
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		NewPos = ScrollInfo.nTrackPos; 
		break;
	}

	if (NewPos < 0)
		NewPos = 0;
	else if (NewPos > MaxPos)
		NewPos = MaxPos;

	if (NewPos == ScrollInfo.nPos)
		return;

	if (Bar == SB_VERT)
	{
		pLogCtrl->m_FirstVisibleLine = NewPos;
		axl_log_TLogCtrl_FixupFirstVisibleLine(pLogCtrl);
	}
	else
	{
		pLogCtrl->m_FirstVisibleCol = NewPos;
		axl_log_TLogCtrl_FixupFirstVisibleCol(pLogCtrl);
	}

	ScrollInfo.fMask = SIF_POS;
	ScrollInfo.nPos = NewPos;
	SetScrollInfo(pLogCtrl->m_hWnd, Bar, &ScrollInfo, TRUE);

	axl_log_TLogCtrl_Invalidate(pLogCtrl);
	axl_log_TLogCtrl_UpdateCaretPos(pLogCtrl);
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnSetCursor(
	axl_log_TLogCtrl* pLogCtrl,
	WPARAM wParam,
	LPARAM lParam
	)
{
	POINT MousePos;
	GetCursorPos(&MousePos);
	ScreenToClient(pLogCtrl->m_hWnd, &MousePos);

	if (!axl_log_TLogCtrl_GetHyperlinkFromMousePos(pLogCtrl, MousePos.x, MousePos.y, NULL, NULL))
		return DefWindowProc(pLogCtrl->m_hWnd, WM_SETCURSOR, wParam, lParam);

	SetCursor(pLogCtrl->m_hHyperlinkCursor);
	return TRUE;
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnLButtonDown(
	axl_log_TLogCtrl* pLogCtrl,
	long x,
	long y
	)
{
	axl_log_TLine* pHyperlinkLine;
	const tchar_t* pHyperlink;

	axl_log_TCursorPos Pos;
	bool_t IsShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

	if (axl_log_TLogCtrl_GetHyperlinkFromMousePos(pLogCtrl, x, y, &pHyperlinkLine, &pHyperlink))
	{
		axl_log_TLogCtrl_OnHyperlink(pLogCtrl, pHyperlinkLine, pHyperlink);
		return 0;
	}

	axl_log_TLogCtrl_GetCursorPosFromMousePos(pLogCtrl, x, y, &Pos, true);
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, IsShiftPressed);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);

	pLogCtrl->m_IsDragging = true;

	SetFocus(pLogCtrl->m_hWnd);
	SetCapture(pLogCtrl->m_hWnd);
	return 0;
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnRButtonDown(
	axl_log_TLogCtrl* pLogCtrl,
	long x,
	long y
	)
{
	if (pLogCtrl->m_SelStart.m_Line != pLogCtrl->m_SelEnd.m_Line ||
		pLogCtrl->m_SelStart.m_Col != pLogCtrl->m_SelEnd.m_Col)
		return 0;

	return axl_log_TLogCtrl_OnLButtonDown(pLogCtrl, x, y);
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnMouseMove(
	axl_log_TLogCtrl* pLogCtrl,
	long x,
	long y
	)
{
	axl_log_TCursorPos Pos;

	if (!pLogCtrl->m_IsDragging)
	{
		axl_log_TLogCtrl_HandleToolTip(pLogCtrl, x, y);
		return 0;
	}

	axl_log_TLogCtrl_GetCursorPosFromMousePos(pLogCtrl, x, y, &Pos, true);
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, true);
	return 0;
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnMouseWheel(
	axl_log_TLogCtrl* pLogCtrl, 
	short Delta
	)
{
	int Code;
	int LineCount; 
	int i;

	if (Delta > 0)
	{
		Code = SB_LINEUP;
		LineCount = Delta / WHEEL_DELTA;
	}
	else
	{
		Code = SB_LINEDOWN;
		LineCount = -Delta / WHEEL_DELTA;
	}

	LineCount *= pLogCtrl->m_MouseWheelSpeed;
	
	for (i = 0; i < LineCount; i++)
		axl_log_TLogCtrl_OnScroll(pLogCtrl, SB_VERT, Code);

	return 0;
}

LRESULT 
AXL_VFT
axl_log_TLogCtrl_WindowProc(
	axl_log_TLogCtrl* pLogCtrl, 
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam,
	bool_t* pIsHandled
	)
{
	LRESULT Result = 0;

	if (Msg >= WM_MOUSEFIRST && Msg <= WM_MOUSELAST)
	{
		MSG MsgStruct = { pLogCtrl->m_hWnd, Msg, wParam, lParam };
		SendMessage(pLogCtrl->m_hWndToolTip, TTM_RELAYEVENT, 0, (LPARAM) &MsgStruct);
	}

	if (Msg >= WM_NCMOUSEFIRST && Msg <= WM_NCMOUSELAST)
		SendMessage(pLogCtrl->m_hWndToolTip, TTM_ACTIVATE, false, 0);

	switch (Msg)
	{
		// std windows msg

	case WM_CREATE:
		Result = axl_log_TLogCtrl_OnCreate(pLogCtrl);
		break;

	case WM_DESTROY:
		Result = axl_log_TLogCtrl_OnDestroy(pLogCtrl);
		break;

	case WM_GETDLGCODE:
		Result = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_SETFOCUS:
		Result = axl_log_TLogCtrl_OnSetFocus(pLogCtrl);
		break;

	case WM_KILLFOCUS:
		Result = axl_log_TLogCtrl_OnKillFocus(pLogCtrl);
		break;

    case WM_THEMECHANGED:
		axl_gr_TStockCtrlPaint_CloseThemes(&pLogCtrl->m_StockCtrlPaint);
		break;

	case WM_NCPAINT:
		axl_gr_TStockCtrlPaint_NcPaintEdge(&pLogCtrl->m_StockCtrlPaint, pLogCtrl->m_hWnd, (HRGN) wParam);
		break;

	case WM_PAINT:
		Result = axl_log_TLogCtrl_OnPaint(pLogCtrl);
		break;

	case WM_SIZE:
		Result = axl_log_TLogCtrl_OnSize(pLogCtrl);
		break;

	case WM_HSCROLL:
		axl_log_TLogCtrl_OnScroll(pLogCtrl, SB_HORZ, LOWORD(wParam));
		break;

	case WM_VSCROLL:
		axl_log_TLogCtrl_OnScroll(pLogCtrl, SB_VERT, LOWORD(wParam));
		break;

	case WM_SETCURSOR:
		Result = axl_log_TLogCtrl_OnSetCursor(pLogCtrl, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		Result = axl_log_TLogCtrl_OnLButtonDown(pLogCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_RBUTTONDOWN:
		Result = axl_log_TLogCtrl_OnRButtonDown(pLogCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSEMOVE:
		Result = axl_log_TLogCtrl_OnMouseMove(pLogCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSEWHEEL:
		Result = axl_log_TLogCtrl_OnMouseWheel(pLogCtrl, HIWORD(wParam));
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		pLogCtrl->m_IsDragging = false;
		ReleaseCapture();
		break;

	case WM_CAPTURECHANGED:
		pLogCtrl->m_IsDragging = false;
		break;

	case WM_KEYDOWN:
		axl_log_TLogCtrl_OnKeyDown(pLogCtrl, (int) wParam);
		break;

	// log ctrl msg

	case axl_log_EMsg_GetMessageFile:
		Result = (LRESULT) pLogCtrl->m_pMessageFile;
		break;

	case axl_log_EMsg_SetMessageFile:
		{
		axl_log_TMsgSetMessageFile* pSetMessageFile = (axl_log_TMsgSetMessageFile*) lParam;
		Result = axl_log_TLogCtrl_SetMessageFile(pLogCtrl, pSetMessageFile->m_pFile, pSetMessageFile->m_pRepresentor);
		break;
		}

	case axl_log_EMsg_GetRepresentor:
		Result = (LRESULT) pLogCtrl->m_pRepresentor;
		break;

	case axl_log_EMsg_SetRepresentor:
		axl_log_TLogCtrl_SetRepresentor(pLogCtrl, (axl_log_TRepresentor*) lParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_GetFilter:
		Result = (LRESULT) pLogCtrl->m_pFilter;
		break;

	case axl_log_EMsg_SetFilter:
		axl_log_TLogCtrl_SetFilter(pLogCtrl, (axl_log_TFilter*) lParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_GetColorizer:
		Result = (LRESULT) pLogCtrl->m_pFilter;
		break;

	case axl_log_EMsg_SetColorizer:
		axl_log_TLogCtrl_SetColorizer(pLogCtrl, (axl_log_TColorizer*) lParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_GetHyperlinkHandler:
		Result = (LRESULT) pLogCtrl->m_pHyperlinkHandler;
		break;

	case axl_log_EMsg_SetHyperlinkHandler:
		axl_log_TLogCtrl_SetHyperlinkHandler(pLogCtrl, (axl_log_THyperlinkHandler*) lParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_GetImageList:
		Result = (LRESULT) pLogCtrl->m_hImageList;
		break;

	case axl_log_EMsg_SetImageList:
		axl_log_TLogCtrl_SetImageList(pLogCtrl, (HIMAGELIST) lParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_IsIconVisible:
		Result = (LRESULT) pLogCtrl->m_IsIconVisible;
		break;

	case axl_log_EMsg_ShowIcon:
		axl_log_TLogCtrl_ShowIcon(pLogCtrl, (BOOL) wParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_GetOffsetWidth:
		Result = (LRESULT) pLogCtrl->m_OffsetWidth;
		break;

	case axl_log_EMsg_SetOffsetWidth:
		axl_log_TLogCtrl_SetOffsetWidth(pLogCtrl, (ulong_t) wParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_IsTimestampVisible:
		Result = (LRESULT) pLogCtrl->m_IsTimestampVisible;
		break;

	case axl_log_EMsg_ShowTimestamp:
		axl_log_TLogCtrl_ShowTimestamp(pLogCtrl, (BOOL) wParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_SetTimestampFormat:
		axl_log_TLogCtrl_SetTimestampFormat(pLogCtrl, (const tchar_t*) lParam);
		break;

	case axl_log_EMsg_GetTimestampFormat:
		Result = (LRESULT) pLogCtrl->m_TimestampFormat.m_p;
		break;

	case axl_log_EMsg_GetPreferredDataConfig:
		Result = (LRESULT) axl_log_TIndexMgr_GetPreferredDataConfig(&pLogCtrl->m_IndexMgr);
		break;

	case axl_log_EMsg_SetPreferredDataConfig:
		axl_log_TIndexMgr_SetPreferredDataConfig(&pLogCtrl->m_IndexMgr, (const axl_log_TDataConfig*) lParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_UpdateLog:
		axl_log_TLogCtrl_UpdateLog(pLogCtrl);
		Result = TRUE;
		break;

	case axl_log_EMsg_ReRepresentAll:
		axl_log_TLogCtrl_ReRepresentAll(pLogCtrl);
		Result = TRUE;
		break;

	case axl_log_EMsg_ReFilterAll:
		axl_log_TLogCtrl_ReFilterAll(pLogCtrl);
		Result = TRUE;
		break;

	case axl_log_EMsg_ReColorizeAll:
		axl_log_TLogCtrl_ReColorizeAll(pLogCtrl);
		Result = TRUE;
		break;

	case axl_log_EMsg_ModifyLineVolatileFlags:
		Result = axl_log_TLogCtrl_ModifyLineVolatileFlags(pLogCtrl, (axl_log_TLine*) lParam, (ulong_t) wParam);
		break;

	case axl_log_EMsg_GetLineCount:
		Result = (LRESULT) pLogCtrl->m_LineCount;
		break;

	case axl_log_EMsg_GetLine:
		Result = (LRESULT) axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, (ulong_t) wParam);
		break;

	case axl_log_EMsg_GetNextLine:
		Result = (LRESULT) axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) lParam);
		break;

	case axl_log_EMsg_GetPrevLine:
		Result = (LRESULT) axl_log_TCacheMgr_GetPrevLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) lParam);
		break;

	case axl_log_EMsg_GetLineString:
		break;

	case axl_log_EMsg_GetCursorPos:
		*(axl_log_TCursorPos*) lParam = pLogCtrl->m_CursorPos;
		Result = TRUE;
		break;

	case axl_log_EMsg_SetCursorPos:
		axl_log_TLogCtrl_SetCursorPos(pLogCtrl, (axl_log_TCursorPos*) lParam, false);
		Result = TRUE;
		break;

	case axl_log_EMsg_GetSelection:
		*(axl_log_TCursorPos*) wParam = pLogCtrl->m_SelStart;
		*(axl_log_TCursorPos*) lParam = pLogCtrl->m_SelEnd;
		Result = TRUE;
		break;

	case axl_log_EMsg_IsSelectionEmpty:
		Result = (LRESULT) axl_log_TLogCtrl_IsSelectionEmpty(pLogCtrl);
		break;

	case axl_log_EMsg_SetSelection:
		axl_log_TLogCtrl_SetSelection(pLogCtrl, (axl_log_TCursorPos*) wParam, (axl_log_TCursorPos*) lParam);
		Result = TRUE;
		break;

	case axl_log_EMsg_GetHilite:
		break;

	case axl_log_EMsg_SetHilite:
		break;

	case axl_log_EMsg_SelectBinRange:
		break;

	case axl_log_EMsg_HiliteBinRange:
		break;

	case axl_log_EMsg_EnsureVisible:
		break;

	case axl_log_EMsg_EnsureVisibleRange:
		break;

	case axl_log_EMsg_GetRangeBinBlock:
		break;

	case axl_log_EMsg_ProcessRange:
		break;

	case axl_log_EMsg_ProcessRangeAsBinBlock:
		break;

	case axl_log_EMsg_GetSelectionBinBlock:
		Result = (LPARAM) axl_log_TLogCtrl_GetSelectionBinBlock(pLogCtrl, (size_t*) wParam, (size_t*) lParam);
		break;

	case axl_log_EMsg_ProcessSelection:
		Result = (LPARAM) axl_log_TLogCtrl_ProcessSelection(pLogCtrl, (axl_log_FProcessLine) wParam, (void*) lParam);
		break;

	case axl_log_EMsg_ProcessSelectionAsBinBlock:
		Result = (LPARAM) axl_log_TLogCtrl_ProcessSelectionAsBinBlock(pLogCtrl, (axl_log_FProcessBinBlock) wParam, (void*) lParam);
		break;

	case axl_log_EMsg_SetFindPattern:
		{
		axl_log_TMsgSetFindPattern* pSetPattern = (axl_log_TMsgSetFindPattern*) lParam;
		Result = (LRESULT) axl_rtl_TBmhFind_SetPattern(
			&pLogCtrl->m_Find, 
			pSetPattern->m_p, 
			pSetPattern->m_Size, 
			pSetPattern->m_DoMatchCase
			);
		}
		break;

	case axl_log_EMsg_CanFind:
		Result = (LRESULT) axl_rtl_TBmhFind_CanFind(&pLogCtrl->m_Find);
		break;

	case axl_log_EMsg_FindNext:
		Result = (LRESULT) axl_log_TLogCtrl_FindNext(pLogCtrl);
		break;

	case axl_log_EMsg_FindPrev:
		Result = (LRESULT) axl_log_TLogCtrl_FindPrev(pLogCtrl);
		break;

	case axl_log_EMsg_CanCopy:
		Result = (LRESULT) !axl_log_TLogCtrl_IsSelectionEmpty(pLogCtrl);
		break;

	case axl_log_EMsg_Copy:
		Result = (LRESULT) axl_log_TLogCtrl_Copy(pLogCtrl);
		break;

	case axl_log_EMsg_CopyString:
		Result = (LRESULT) axl_log_TLogCtrl_CopyString(pLogCtrl, (axl_rtl_TString*) lParam);
		break;

	case axl_log_EMsg_SaveAsTextFile:
		Result = (LRESULT) axl_log_TLogCtrl_SaveAsTextFile(pLogCtrl, (const tchar_t*) lParam, (axl_rtl_TBinTree*) wParam);
		break;

	case axl_log_EMsg_HitTest:
		break;

	default:
		*pIsHandled = false;
	}
 
	return Result;
}

//.............................................................................
