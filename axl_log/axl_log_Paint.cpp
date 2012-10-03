#include "axl_Precomp.h"
#include "axl_log_LogCtrl.h"

//.............................................................................

void
AXL_API
axl_log_TLogCtrl_Construct(axl_log_TLogCtrl* pLogCtrl)
{
	axl_gr_TStockCtrlPaint_Construct(&pLogCtrl->m_StockCtrlPaint);
	axl_gr_TTextPaint_Construct(&pLogCtrl->m_TextPaint);

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	axl_gr_TOffscreenBuffer_Construct(&pLogCtrl->m_OffscreenBuffer);
#endif

//	pLogCtrl->m_hUpdateRgn = CreateRectRgn(0, 0, 0, 0);

	pLogCtrl->m_pMessageFile = NULL;
	axl_log_TIndexFile_Construct(&pLogCtrl->m_IndexFile);
	
	axl_log_TIndexManager_Construct(&pLogCtrl->m_IndexMgr, pLogCtrl);
	axl_log_TCacheManager_Construct(&pLogCtrl->m_CacheMgr, pLogCtrl);
	axl_log_TColorizeManager_Construct(&pLogCtrl->m_ColorizeMgr, pLogCtrl);

	pLogCtrl->m_pRepresentor = NULL;
	pLogCtrl->m_pHyperLinkHandler = NULL;
	pLogCtrl->m_pFilter = NULL;

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
	axl_log_TCursorPos_ConstructSetup(&pLogCtrl->m_LastToolTipPos, -1, -1);
	
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

	pLogCtrl->m_hImageList = NULL;
	pLogCtrl->m_hArrowCursor = LoadCursor(NULL, IDC_ARROW);
	pLogCtrl->m_hBeamCursor = LoadCursor(NULL, IDC_IBEAM);
	pLogCtrl->m_hHyperLinkCursor = LoadCursor(NULL, IDC_HAND);

	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_Black]        = axl_gr_EColorRgb_Black;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_White]        = axl_gr_EColorRgb_White;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_Gray]         = axl_gr_EColorRgb_Gray;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_PastelGray]   = axl_gr_EColorRgb_PastelGray;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_Red]          = axl_gr_EColorRgb_Red;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_LightRed]     = axl_gr_EColorRgb_LightRed;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_PastelRed]    = axl_gr_EColorRgb_PastelRed;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_Orange]       = axl_gr_EColorRgb_Orange;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_PastelOrange] = axl_gr_EColorRgb_PastelOrange;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_PastelYellow] = axl_gr_EColorRgb_PastelYellow;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_Green]        = axl_gr_EColorRgb_Green;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_PastelGreen]  = axl_gr_EColorRgb_PastelGreen;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_Cyan]         = axl_gr_EColorRgb_Cyan;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_PastelCyan]   = axl_gr_EColorRgb_PastelCyan;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_Blue]         = axl_gr_EColorRgb_Blue;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_LightBlue]    = axl_gr_EColorRgb_LightBlue;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_PastelBlue]   = axl_gr_EColorRgb_PastelBlue;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_Purple]       = axl_gr_EColorRgb_Purple;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_LightPurple]  = axl_gr_EColorRgb_LightPurple;
	pLogCtrl->m_ColorScheme [axl_gr_EColorIndex_PastelPurple] = axl_gr_EColorRgb_PastelPurple;
	
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_Text]            = GetSysColor(COLOR_WINDOWTEXT);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_Back]            = GetSysColor(COLOR_WINDOW);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_SelectionText]   = GetSysColor(COLOR_HIGHLIGHTTEXT);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_SelectionBack]   = GetSysColor(COLOR_HIGHLIGHT);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_HiliteText]      = GetSysColor(COLOR_WINDOWTEXT);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_HiliteBack]      = RGB(220, 220, 255);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_DelimiterLight]  = RGB(220, 220, 220);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_DelimiterMedium] = RGB(192, 192, 192);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_DelimiterBold]   = RGB(128, 128, 128);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_Timestamp]       = GetSysColor(COLOR_GRAYTEXT);
	pLogCtrl->m_ColorScheme [axl_log_EColorIndex_Offset]          = GetSysColor(COLOR_GRAYTEXT);

	pLogCtrl->m_TextPaint.m_pColorScheme = pLogCtrl->m_ColorScheme;
	pLogCtrl->m_TextPaint.m_ColorCount = sizeof(pLogCtrl->m_ColorScheme) / sizeof(COLORREF);
}

void
AXL_API
axl_log_TLogCtrl_Destruct(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TLogCtrl_Destroy(pLogCtrl);

	axl_obj_TObject_Dereference(pLogCtrl->m_pRepresentor);
	axl_obj_TObject_Dereference(pLogCtrl->m_pHyperLinkHandler);
	axl_obj_TObject_Dereference(pLogCtrl->m_pFilter);

	axl_gr_TStockCtrlPaint_Destruct(&pLogCtrl->m_StockCtrlPaint);
	axl_gr_TTextPaint_Destruct(&pLogCtrl->m_TextPaint);

	axl_log_TIndexFile_Destruct(&pLogCtrl->m_IndexFile);	
	axl_log_TIndexManager_Destruct(&pLogCtrl->m_IndexMgr);
	axl_log_TCacheManager_Destruct(&pLogCtrl->m_CacheMgr);
	axl_log_TColorizeManager_Destruct(&pLogCtrl->m_ColorizeMgr);

	axl_gr_TFont_Destruct(&pLogCtrl->m_DefaultFont);

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	axl_gr_TOffscreenBuffer_Destruct(&pLogCtrl->m_OffscreenBuffer);
#endif

//	DeleteObject(pLogCtrl->m_hUpdateRgn);

	axl_rtl_TBmhFind_Destruct(&pLogCtrl->m_Find);
	axl_rtl_TByteArray_Destruct(&pLogCtrl->m_BinBlockBuffer);

	axl_rtl_TString_Destruct(&pLogCtrl->m_TimestampFormat);
	axl_rtl_TString_Destruct(&pLogCtrl->m_TimestampString);
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
		ulong_t OpenFlags = axl_io_TFile_EFlags_DeleteOnClose | axl_io_TFile_EFlags_Exclusive;

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

		axl_log_TIndexManager_ClearIndex(&pLogCtrl->m_IndexMgr);
	}

	pLogCtrl->m_pMessageFile = pMessageFile;

	if (pRepresentor)
		axl_obj_TObject_CopyPtr(&pLogCtrl->m_pRepresentor, pRepresentor);

	axl_log_TLogCtrl_ReRepresentAll(pLogCtrl);
	return true;
}

void
AXL_API
axl_log_TLogCtrl_SetRepresentor(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TRepresentor* pRepresentor
	)
{
	axl_obj_TObject_CopyPtr(&pLogCtrl->m_pRepresentor, pRepresentor);
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

	axl_log_TIndexManager_ClearIndex(&pLogCtrl->m_IndexMgr);
	axl_log_TIndexManager_UpdateIndex(&pLogCtrl->m_IndexMgr);
	axl_log_TLogCtrl_FullUpdateLog(pLogCtrl);
}

void
AXL_API
axl_log_TLogCtrl_ReFilterAll(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TIndexManager_ReFilterAll(&pLogCtrl->m_IndexMgr);  
	axl_log_TLogCtrl_FullUpdateLog(pLogCtrl);
}

void
AXL_API
axl_log_TLogCtrl_ReColorizeAll(axl_log_TLogCtrl* pLogCtrl)
{
//	axl_log_TCacheManager_ClearCache(&pLogCtrl->m_CacheMgr);
	axl_log_TCacheManager_ClearCacheColorization(&pLogCtrl->m_CacheMgr);
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
axl_log_TLogCtrl_UpdateLogOnAdd(axl_log_TLogCtrl* pLogCtrl)
{
	bool_t ShouldScrollToBottom = axl_log_TLogCtrl_IsVScrollAtBottom(pLogCtrl);
	bool_t ShouldPutCursorToEnd = ShouldScrollToBottom && axl_log_TLogCtrl_IsCursorAtLastLine(pLogCtrl);

	ulong_t AnchorLine = pLogCtrl->m_LineCount ? pLogCtrl->m_LineCount - 1 : 0;

	// mark cache pages for update starting with anchor line

	axl_log_TIndexNode* pNode = axl_log_TIndexFile_FindNodeByLine(&pLogCtrl->m_IndexFile, AnchorLine, NULL);
	while (pNode)
	{
		axl_log_TCachePage* pPage = axl_log_TCacheManager_FindCachePageByIndexNode(&pLogCtrl->m_CacheMgr, pNode);
		if (pPage)
		{
			pPage->m_IsRepresentNeeded = true;
			pPage->m_IsColorizeNeeded = true;
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
		axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos); 
		axl_log_TLogCtrl_NotifyParent(pLogCtrl, axl_log_ECtrlNm_CursorPosChange, NULL);
	}
}

void 
AXL_API
axl_log_TLogCtrl_FullUpdateLog(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TCacheManager_ClearCache(&pLogCtrl->m_CacheMgr);
	
	pLogCtrl->m_LineCount = axl_log_TIndexFile_GetLineCount(&pLogCtrl->m_IndexFile);
	pLogCtrl->m_FirstVisibleLine = 0;
	pLogCtrl->m_FirstVisibleCol = 0;
	pLogCtrl->m_ColCount = 0;

	axl_log_TLogCtrl_RecalcVScroll(pLogCtrl);
	axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);
	axl_log_TLogCtrl_Invalidate(pLogCtrl);
}

bool_t 
AXL_API
axl_log_TLogCtrl_UpdateLog(axl_log_TLogCtrl* pLogCtrl)
{
	bool_t IsFullUpdateNeeded = axl_log_TIndexManager_UpdateIndex(&pLogCtrl->m_IndexMgr);

	if (IsFullUpdateNeeded)
		axl_log_TLogCtrl_FullUpdateLog(pLogCtrl);
	else
		axl_log_TLogCtrl_UpdateLogOnAdd(pLogCtrl);

	return IsFullUpdateNeeded;
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

	if (pPos->m_Line >= (long) (AnchorLine + OldLineCount))
	{
		// cursor was below re-represented message

		pPos->m_Line = pPos->m_Line + LineDelta;
		return axl_log_ECursorPosFixup_Below;
	}
	else if (pPos->m_Line >= (long) AnchorLine)
	{	
		// cursor was within re-represented message itself

		long MessageEndLine = (long) (AnchorLine + NewLineCount);
		if (pPos->m_Line >= MessageEndLine)
			pPos->m_Line = MessageEndLine - 1;

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
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &CursorPos);

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

	axl_log_TLogCtrl_NotifyParent(pLogCtrl, axl_log_ECtrlNm_CursorPosChange, NULL);
}

bool_t
AXL_API
axl_log_TLogCtrl_ReRepresentMessage(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pAnchorLine,
	ulong_t VolatileFlags
	)
{
	bool_t Result;
	axl_log_TReRepresent ReRepresent;
	long LineDelta;

	bool_t DidFirstVisibleLineChange;
	bool_t DidFirstVisibleColChange;

	axl_log_TCachePage* pPage = pAnchorLine->m_pPage;

	Result = axl_log_TCacheManager_ReRepresentMessage(&pLogCtrl->m_CacheMgr, pAnchorLine, VolatileFlags, &ReRepresent);
	if (!Result)
		return false;

	if (ReRepresent.m_NewLineCount)
		axl_log_TColorizeManager_RecolorizeLineRange(&pLogCtrl->m_ColorizeMgr, pPage, ReRepresent.m_AnchorLine, ReRepresent.m_NewLineCount); 

	LineDelta = ReRepresent.m_NewLineCount - ReRepresent.m_OldLineCount;

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
		axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, ReRepresent.m_AnchorLine, ReRepresent.m_AnchorLine + ReRepresent.m_OldLineCount - 1);
	else
		axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, ReRepresent.m_AnchorLine, -1);

	axl_log_TLogCtrl_UpdateCursorPosOnReRepresentMessage(pLogCtrl, ReRepresent.m_AnchorLine, ReRepresent.m_OldLineCount, ReRepresent.m_NewLineCount);

	return true;
}

LRESULT 
CALLBACK 
axl_log_TLogCtrl_WndProc(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
	);

bool_t
AXL_API
axl_log_TLogCtrl_RegisterWndClass(HINSTANCE hInstance)
{
	BOOL Result;
	WNDCLASS WndClass;

	static bool_t _IsRegistered = false;
	if (_IsRegistered)
		return true;

	memset(&WndClass, 0, sizeof(WndClass));
	WndClass.style = 0; // CS_DBLCLKS;
    WndClass.lpfnWndProc = axl_log_TLogCtrl_WndProc;
    WndClass.cbWndExtra = sizeof(axl_log_TLogCtrl*);
    WndClass.hInstance = hInstance;
	WndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);
    WndClass.lpszClassName = axl_log_TLogCtrl_WndClassName;

	Result = RegisterClass(&WndClass);
	if (!Result)
		return false;

	_IsRegistered = true;
	return true;
}

HWND
AXL_API
axl_log_TLogCtrl_CreateEx(
	axl_log_TLogCtrl* pLogCtrl,
	DWORD ExStyle,
	DWORD Style,
	const RECT* pRect,
	HWND hWndParent,
	UINT ID,
	HINSTANCE hInstance
	)
{
	bool_t Result;
	
	ASSERT(!pLogCtrl->m_hWnd);

	Result = axl_log_TLogCtrl_RegisterWndClass(hInstance);
	if (!Result)
		return false;

	CreateWindowEx(
		ExStyle, 
		axl_log_TLogCtrl_WndClassName, NULL, 
		Style, 
		pRect->left, pRect->top, 
		pRect->right - pRect->left, 
		pRect->bottom - pRect->top,
		hWndParent, (HMENU)(uintptr_t) ID,
		hInstance,
		pLogCtrl
		);

	return pLogCtrl->m_hWnd;
}

void
AXL_API
axl_log_TLogCtrl_Destroy(axl_log_TLogCtrl* pLogCtrl)
{
	if (!pLogCtrl->m_hWnd)
		return;

	DestroyWindow(pLogCtrl->m_hWnd);
	pLogCtrl->m_hWnd = NULL;
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

void
AXL_API
axl_log_TLogCtrl_InvalidateLineRange(
	axl_log_TLogCtrl* pLogCtrl,
	ulong_t FirstLine,
	ulong_t LastLine
	)
{
	RECT Rect;

	if (LastLine < FirstLine) // swap
	{
		ulong_t t;		
		t = LastLine, LastLine = FirstLine, FirstLine = t;
	}

	// first line validation

	if (FirstLine > pLogCtrl->m_FirstVisibleLine)
		FirstLine -= pLogCtrl->m_FirstVisibleLine;
	else
		FirstLine = 0;

	if (FirstLine >= pLogCtrl->m_VisibleLineCount)
		return;

	// last line validation

	if (LastLine < pLogCtrl->m_FirstVisibleLine)
		return;

	LastLine -= pLogCtrl->m_FirstVisibleLine;

	if (LastLine > pLogCtrl->m_VisibleLineCount)
		LastLine = pLogCtrl->m_VisibleLineCount;

	// redraw

	GetClientRect(pLogCtrl->m_hWnd, &Rect);

	Rect.top = FirstLine * pLogCtrl->m_CharSize.cy;
	Rect.bottom = (LastLine + 1) * pLogCtrl->m_CharSize.cy;

	InvalidateRect(pLogCtrl->m_hWnd, &Rect, FALSE);
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
	axl_log_TLogTextLine* pTextLine;
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

    pLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, pPos->m_Line);

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		pTextLine = (axl_log_TLogTextLine*) pLine;

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
		Col = (long) ColCount;

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
axl_log_TLogCtrl_GetBinOffset(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos,
	size_t* pOffset,
	size_t* pLineOffset,
	int* pHexCol,
	ulong_t* pMergeId
	)
{
	size_t LineOffset;

	axl_log_TLine* pLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, pPos->m_Line);
	if (!pLine)
		return false;

	switch (pLine->m_Type)
	{
	case axl_log_ELine_BinHex:
		LineOffset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, (axl_log_TBinLine*) pLine, pPos->m_Col, pHexCol);
		break;

	case axl_log_ELine_BinText:
		LineOffset = axl_log_TLogCtrl_GetBinTextLineOffset(pLogCtrl, (axl_log_TBinTextLine*) pLine, pPos->m_Col);
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
axl_log_TLogCtrl_GetSelBinRange(
	axl_log_TLogCtrl* pLogCtrl,
	size_t* pOffset,
	size_t* pSize
	)
{
	axl_log_TCursorPos SelStart;
	axl_log_TCursorPos SelEnd;

	size_t StartOffset = -1;
	ulong_t StartMergeId = 0;

	size_t EndOffset = -1;
	ulong_t EndMergeId = 0;
	int EndHexCol;

	bool_t IsNonEmptySel = axl_log_TLogCtrl_GetSelection(pLogCtrl, &SelStart, &SelEnd);

	axl_log_TLogCtrl_GetBinOffset(pLogCtrl, &SelStart, &StartOffset, NULL, NULL, &StartMergeId);
	axl_log_TLogCtrl_GetBinOffset(pLogCtrl, &SelEnd, &EndOffset, NULL, &EndHexCol, &EndMergeId);

	if (!IsNonEmptySel || StartOffset == -1 || EndOffset == -1 || StartMergeId != EndMergeId)
	{
		*pSize = 0;
		return axl_log_TLogCtrl_GetBinOffset(pLogCtrl, &pLogCtrl->m_CursorPos, pOffset, NULL, NULL, NULL);
	}

	if (EndHexCol)
		EndOffset++;

	if (pOffset)
		*pOffset = StartOffset;

	if (pSize)
		*pSize = EndOffset - StartOffset;

	return true;
}

bool_t
AXL_API
axl_log_TLogCtrl_IsSelBinBlock(axl_log_TLogCtrl* pLogCtrl)
{
	size_t Offset;
	size_t Size;
	axl_log_TLogCtrl_GetSelBinRange(pLogCtrl, &Offset, &Size);
	return Offset != -1 && Size != 0;
}

void 
AXL_API
axl_log_TLogCtrl_SetCursorPos(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	)
{
	axl_log_TLine* pNewLine;
	axl_log_TLine* pOldLine;

	long OldLine = pLogCtrl->m_CursorPos.m_Line;

	if (axl_log_TCursorPos_Compare(pPos, &pLogCtrl->m_CursorPos) == 0)
		return;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPos);

	pOldLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, OldLine);
	pNewLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, pPos->m_Line);

	pLogCtrl->m_CursorPos = *pPos;

	axl_log_TLogCtrl_UpdateCaretPos(pLogCtrl);

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

	if (axl_log_TCursorPos_Compare(pPosStart, &pLogCtrl->m_HiliteStart) == 0 &&
		axl_log_TCursorPos_Compare(pPosEnd, &pLogCtrl->m_HiliteEnd) == 0)
		return;

	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosStart);
	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPosEnd);

	if (axl_log_TCursorPos_Compare(pPosStart, pPosEnd) > 0)
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
	size_t LineSize;

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
		axl_log_TLine* pPrevLine = axl_log_TCachePage_GetLine(pPage, pLine->m_PageIndex - 1);
		if (!pPrevLine || !axl_log_TLine_IsMerged(pPrevLine, pLine))
			break;

		pLine = pPrevLine;
	} 

	pBinLine = (axl_log_TBinLine*) pLine;
	ASSERT(pBinLine->m_Offset == 0);

	Line = axl_log_TCacheManager_GetCachePageStartLine(&pLogCtrl->m_CacheMgr, pPage);

	// we cannot simply do Offset / LineSize because:
	// - it won't work for bin text lines 
	// - we dont want to leave the current block

	// scan forward to the beginning of the range 

	for (;;) 
	{
		axl_log_TLine* pNextLine;
		
		pBinLine = (axl_log_TBinLine*) pLine;
		LineSize = axl_rtl_TByteArray_GetCount(&pBinLine->m_BinData);

		if (Offset >= pBinLine->m_Offset && Offset <= pBinLine->m_Offset + LineSize)
		{
			PosStart.m_Line = Line + (long) pBinLine->m_PageIndex;
			PosStart.m_Col = axl_log_TLogCtrl_GetHiliteCol(pLogCtrl, pBinLine, Offset - pBinLine->m_Offset);
			break;
		}

		pNextLine = axl_log_TCachePage_GetLine(pPage, pLine->m_PageIndex + 1);
		if (!pNextLine || !axl_log_TLine_IsMerged(pLine, pNextLine))
			return false;

		pLine = pNextLine;
	} 

	// scan forward to the end of the range 

	for (;;) 
	{
		axl_log_TLine* pNextLine;
		
		pBinLine = (axl_log_TBinLine*) pLine;
		LineSize = axl_rtl_TByteArray_GetCount(&pBinLine->m_BinData);

		if (EndOffset >= pBinLine->m_Offset && EndOffset <= pBinLine->m_Offset + LineSize)
		{
			PosEnd.m_Line = Line + (long) pBinLine->m_PageIndex;
			PosEnd.m_Col = axl_log_TLogCtrl_GetHiliteCol(pLogCtrl, pBinLine, EndOffset - pBinLine->m_Offset);
			break;
		}

		pNextLine = axl_log_TCachePage_GetLine(pPage, pLine->m_PageIndex + 1);
		if (!pNextLine || !axl_log_TLine_IsMerged(pLine, pNextLine))
			return false;

		pLine = pNextLine;
	} 

	axl_log_TLogCtrl_KillSelection(pLogCtrl);
	axl_log_TLogCtrl_SetHilite(pLogCtrl, &PosStart, &PosEnd);
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

	if (axl_log_TCursorPos_Compare(&OldHiliteStart, &OldHiliteEnd) != 0) 
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

	if (axl_log_TCursorPos_Compare(pPos, &pLogCtrl->m_SelStart) == 0)
		return;
	
	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPos);
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

	if (axl_log_TCursorPos_Compare(pPos, &pLogCtrl->m_SelEnd) == 0)
		return;
	
	axl_log_TLogCtrl_ValidateCursorPos(pLogCtrl, pPos);
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
	int Cmp = axl_log_TCursorPos_Compare(&pLogCtrl->m_SelStart, &pLogCtrl->m_SelEnd);

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

	if (axl_log_TCursorPos_Compare(&OldSelStart, &OldSelEnd) != 0) 
		axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, OldSelStart.m_Line, OldSelEnd.m_Line);
}

void 
AXL_API
axl_log_TLogCtrl_EnsureVisible(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	)
{
	bool_t DoInvalidate = false;

	long Col = pPos->m_Col + pLogCtrl->m_BaseCol;

	if ((ulong_t) pPos->m_Line >= pLogCtrl->m_FirstVisibleLine + pLogCtrl->m_VisibleLineCount)
	{
		pLogCtrl->m_FirstVisibleLine = pPos->m_Line - pLogCtrl->m_VisibleLineCount + 1;
		DoInvalidate = true;
	}

	if ((ulong_t) pPos->m_Line < pLogCtrl->m_FirstVisibleLine)
	{
		pLogCtrl->m_FirstVisibleLine = pPos->m_Line;
		DoInvalidate = true;
	}

	if ((ulong_t) Col + 1 >= pLogCtrl->m_FirstVisibleCol + pLogCtrl->m_VisibleColCount)
	{
		pLogCtrl->m_FirstVisibleCol = Col + 1 - pLogCtrl->m_VisibleColCount + 1; 
		DoInvalidate = true;
	}

	if ((ulong_t) Col < pLogCtrl->m_FirstVisibleCol)
	{
		pLogCtrl->m_FirstVisibleCol = Col;
		DoInvalidate = true;
	}

	axl_log_TLogCtrl_FixupFirstVisibleLine(pLogCtrl);
	axl_log_TLogCtrl_FixupFirstVisibleCol(pLogCtrl);
	axl_log_TLogCtrl_RecalcHScroll(pLogCtrl);
	axl_log_TLogCtrl_RecalcVScroll(pLogCtrl);

	if (DoInvalidate)
		axl_log_TLogCtrl_Invalidate(pLogCtrl);
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

// painting

long
AXL_API
axl_log_TLogCtrl_PaintTimestamp(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;

	ulonglong_t Timestamp = pLine->m_IsFirstLineOfMessage || pLine->m_FirstTimestamp != pLine->m_LastTimestamp ? 
		pLine->m_LastTimestamp : 0;

	FILETIME LocalTimestamp;
	SYSTEMTIME SysTime = {0};
	size_t Length;

	if (!Timestamp)
		return axl_gr_TTextPaint_PaintSpace(pPaint, pLogCtrl->m_TimestampWidth);

	FileTimeToLocalFileTime((FILETIME*) &Timestamp, &LocalTimestamp);
	FileTimeToSystemTime(&LocalTimestamp, &SysTime);

	ASSERT(!axl_rtl_TString_IsEmpty(&pLogCtrl->m_TimestampFormat));
	axl_log_TLogCtrl_FormatTime(&pLogCtrl->m_TimestampString, &SysTime, pLogCtrl->m_TimestampFormat.m_p);
	Length = axl_rtl_TString_GetLength(&pLogCtrl->m_TimestampString);

	axl_gr_TTextPaint_SetTextColorIndex(pPaint, axl_log_EColorIndex_Timestamp);
	axl_gr_TTextPaint_PaintText(pPaint, pLogCtrl->m_TimestampString.m_p, Length);

	if (Length < pLogCtrl->m_TimestampWidth)
		axl_gr_TTextPaint_PaintSpace(pPaint, pLogCtrl->m_TimestampWidth - Length);

	return pPaint->m_x;
}

long 
AXL_API
axl_log_TLogCtrl_PaintIcon(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;

	long x = pPaint->m_x;
	long y = pPaint->m_y;

	axl_gr_TTextPaint_PaintSpace(pPaint, pLogCtrl->m_IconWidth);

	ASSERT(pLogCtrl->m_hImageList != NULL);

	if (pLine->m_Icon != -1)
		ImageList_Draw(
			pLogCtrl->m_hImageList, 
			pLine->m_Icon,
			pPaint->m_hdc, 
			x + pLogCtrl->m_IconOrigin.x, 
			y + pLogCtrl->m_IconOrigin.y, 
			ILD_TRANSPARENT
			);

	return pPaint->m_x;
}

long
AXL_API
axl_log_TLogCtrl_PaintOffset(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;
	tchar_t OffsetBuffer [16] = {0};
	tchar_t* pOffset = OffsetBuffer;

	ASSERT(pLogCtrl->m_OffsetWidth);

	if (pLogCtrl->m_OffsetWidth < 8)
		pOffset += 8 - pLogCtrl->m_OffsetWidth;
	
	_sntprintf(OffsetBuffer, sizeof(OffsetBuffer) / sizeof(tchar_t), _T("%08x"), pLine->m_Offset);

	axl_gr_TTextPaint_SetTextColorIndex(pPaint, axl_log_EColorIndex_Offset);
	axl_gr_TTextPaint_PaintText(pPaint, pOffset, pLogCtrl->m_OffsetWidth);
	axl_gr_TTextPaint_PaintSpace(pPaint, axl_log_EDef_OffsetGapSize);

	return pPaint->m_x;
}

long
AXL_API
axl_log_TLogCtrl_PaintText(
	axl_log_TLogCtrl* pLogCtrl,
	axl_rtl_TString* pText,
	axl_rtl_TArray* pAttrArray,
	long Line,
	long BaseCol,
	axl_log_TCursorPos* pSelStart,
	axl_log_TCursorPos* pSelEnd,
	axl_gr_TTextAttr* pSelAttr
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;

	size_t Length = axl_rtl_TString_GetLength(pText);

	size_t SelStart;
	size_t SelEnd;

	if (!pSelStart || !pSelEnd || Line < pSelStart->m_Line || Line > pSelEnd->m_Line) // no selection
		return axl_gr_TTextPaint_PaintHyperText(
			pPaint, 
			pText->m_p, Length, 
			(axl_gr_TTextAttrAnchor*) pAttrArray->m_p, 
			axl_rtl_TArray_GetCount(pAttrArray)
			);

	if (Line > pSelStart->m_Line && Line < pSelEnd->m_Line)
	{
		SelStart = 0;
		SelEnd = -1;
	}
	else if (Line == pSelStart->m_Line && Line == pSelEnd->m_Line)
	{
		SelStart = pSelStart->m_Col - BaseCol;
		SelEnd = pSelEnd->m_Col - BaseCol;
	}
	else if (Line == pSelStart->m_Line)
	{
		SelStart = pSelStart->m_Col - BaseCol;
		SelEnd = -1;
	}
	else
	{
		SelStart = 0;
		SelEnd = pSelEnd->m_Col - BaseCol;
	}

	return axl_gr_TTextPaint_PaintSelHyperText(
		pPaint, 
		pText->m_p, Length, 
		(axl_gr_TTextAttrAnchor*) pAttrArray->m_p, 
		axl_rtl_TArray_GetCount(pAttrArray),
		SelStart,
		SelEnd,
		pSelAttr
		);
}

long
AXL_API
axl_log_TLogCtrl_PaintBinHex(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine,
	size_t SelStart,
	size_t SelEnd,
	axl_gr_TTextAttr* pSelAttr
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;

	size_t Size = axl_rtl_TArray_GetCount(&pLine->m_BinData);
	size_t GapLength = (pLine->m_DataConfig.m_BinHexLineSize - Size) * 3 + 1 + axl_log_EDef_HexGapSize;

	if (SelStart >= SelEnd)
	{
		axl_gr_TTextPaint_PaintHyperBinHex(
			pPaint, 
			pLine->m_BinData.m_p, Size, 
			(axl_gr_TTextAttrAnchor*) pLine->m_OverlayAttrArray.m_p, 
			axl_rtl_TArray_GetCount(&pLine->m_OverlayAttrArray)
			);

		axl_gr_TTextPaint_SetStdBackColor(pPaint);
		axl_gr_TTextPaint_PaintSpace(pPaint, GapLength);

		axl_gr_TTextPaint_PaintHyperBinAscii(
			pPaint, 
			pLine->m_BinData.m_p, Size, 
			(axl_gr_TTextAttrAnchor*) pLine->m_OverlayAttrArray.m_p, 
			axl_rtl_TArray_GetCount(&pLine->m_OverlayAttrArray)
			);
	}
	else
	{
		axl_gr_TTextPaint_PaintSelHyperBinHex(
			pPaint, 
			pLine->m_BinData.m_p, Size, 
			(axl_gr_TTextAttrAnchor*) pLine->m_OverlayAttrArray.m_p, 
			axl_rtl_TArray_GetCount(&pLine->m_OverlayAttrArray),
			SelStart, SelEnd, 
			pSelAttr
			);

		axl_gr_TTextPaint_SetStdBackColor(pPaint);
		axl_gr_TTextPaint_PaintSpace(pPaint, GapLength);

		axl_gr_TTextPaint_PaintSelHyperBinAscii(
			pPaint, 
			pLine->m_BinData.m_p, Size, 
			(axl_gr_TTextAttrAnchor*) pLine->m_OverlayAttrArray.m_p, 
			axl_rtl_TArray_GetCount(&pLine->m_OverlayAttrArray),
			SelStart, SelEnd, 
			pSelAttr
			);
	}

	return pPaint->m_x;
}

long
AXL_API
axl_log_TLogCtrl_PaintBinHexLine(
	axl_log_TLogCtrl* pLogCtrl,
	const RECT* pLineRect, 
	axl_log_TBinLine* pLine,
	long Line,
	axl_log_TCursorPos* pSelStart,
	axl_log_TCursorPos* pSelEnd,
	axl_gr_TTextAttr* pSelAttr
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;

	size_t SelStartOffset;
	size_t SelEndOffset;
	int SelStartHexCol;
	int SelEndHexCol;

	if (!pSelStart || !pSelEnd || Line < pSelStart->m_Line || Line > pSelEnd->m_Line) // no selection
	{
		return axl_log_TLogCtrl_PaintBinHex(pLogCtrl, pLine, 0, 0, NULL);
	}
	else if (Line > pSelStart->m_Line && Line < pSelEnd->m_Line)
	{
		return axl_log_TLogCtrl_PaintBinHex(pLogCtrl, pLine, 0, -1, pSelAttr);
	}
	else if (Line == pSelStart->m_Line && Line != pSelEnd->m_Line)
	{
		SelStartOffset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, pLine, pSelStart->m_Col, &SelStartHexCol);
		if (SelStartHexCol == 2)
			SelStartOffset++;

		return axl_log_TLogCtrl_PaintBinHex(pLogCtrl, pLine, SelStartOffset, -1, pSelAttr);
	}
	else if (Line == pSelEnd->m_Line && Line != pSelStart->m_Line)
	{
		SelEndOffset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, pLine, pSelEnd->m_Col, &SelEndHexCol);
		if (SelEndHexCol)
			SelEndOffset++;

		return axl_log_TLogCtrl_PaintBinHex(pLogCtrl, pLine, 0, SelEndOffset, pSelAttr);
	}
	else 
	{
		ASSERT(Line == pSelStart->m_Line && Line == pSelEnd->m_Line);

		SelStartOffset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, pLine, pSelStart->m_Col, &SelStartHexCol);
		SelEndOffset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, pLine, pSelEnd->m_Col, &SelEndHexCol);

		if (SelEndOffset < SelStartOffset || 
			SelEndOffset == SelStartOffset && SelEndHexCol < SelStartHexCol)
		{
			size_t TempOffset = SelStartOffset;
			int TempHexCol = SelStartHexCol;

			SelStartOffset = SelEndOffset;
			SelStartHexCol = SelEndHexCol;

			SelEndOffset = TempOffset;
			SelEndHexCol = TempHexCol;
		}

		if (SelStartHexCol == 2)
			SelStartOffset++;

		if (SelEndHexCol)
			SelEndOffset++;

		return axl_log_TLogCtrl_PaintBinHex(pLogCtrl, pLine, SelStartOffset, SelEndOffset, pSelAttr);
	}
}

void
AXL_API
axl_log_TLogCtrl_PaintLine(
	axl_log_TLogCtrl* pLogCtrl,
	const RECT* pLineRect, 
	axl_log_TLine* pLine, 
	long Line
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;

	axl_log_TLine* pNextLine;
	int Delimiter;

	int SelCmp = axl_log_TCursorPos_Compare(&pLogCtrl->m_SelStart, &pLogCtrl->m_SelEnd);
	int HiliteCmp = axl_log_TCursorPos_Compare(&pLogCtrl->m_HiliteStart, &pLogCtrl->m_HiliteEnd);

	axl_log_TCursorPos* pSelStart = NULL;
	axl_log_TCursorPos* pSelEnd = NULL;

	axl_gr_TTextAttr SelAttr = {0};

	if (SelCmp > 0)
	{
		pSelStart = &pLogCtrl->m_SelEnd;
		pSelEnd = &pLogCtrl->m_SelStart;
		axl_gr_TColor_SetRgb(&SelAttr.m_TextColor, GetSysColor(COLOR_HIGHLIGHTTEXT));
		axl_gr_TColor_SetRgb(&SelAttr.m_BackColor, GetSysColor(COLOR_HIGHLIGHT));
	}
	else if (SelCmp < 0)
	{
		pSelStart = &pLogCtrl->m_SelStart;
		pSelEnd = &pLogCtrl->m_SelEnd;
		axl_gr_TColor_SetRgb(&SelAttr.m_TextColor, GetSysColor(COLOR_HIGHLIGHTTEXT));
		axl_gr_TColor_SetRgb(&SelAttr.m_BackColor, GetSysColor(COLOR_HIGHLIGHT));
	}
	else if (HiliteCmp != 0)
	{
		pSelStart = &pLogCtrl->m_HiliteStart; 
		pSelEnd = &pLogCtrl->m_HiliteEnd;
		axl_gr_TColor_SetIndex(&SelAttr.m_BackColor, axl_log_EColorIndex_HiliteBack);
	}

	if (axl_log_TLine_IsBin(pLine))
	{
		axl_log_TBinLine* pBinLine = (axl_log_TBinLine*) pLine;

		axl_log_TColorizeManager_EnsureColorized(&pLogCtrl->m_ColorizeMgr, pBinLine);

		if (pBinLine->m_IsAttrOverlayScheduled)
			axl_log_TBinLine_CreateOverlayAttrArray(pBinLine);
	}

	// reset 

	axl_gr_TColor_SetIndex(&pPaint->m_StdAttr.m_TextColor, axl_log_EColorIndex_Text);
	axl_gr_TColor_SetIndex(&pPaint->m_StdAttr.m_BackColor, axl_log_EColorIndex_Back);

	axl_gr_TColor_Add(&pPaint->m_StdAttr.m_TextColor, &pLine->m_Attr.m_TextColor);
	axl_gr_TColor_Add(&pPaint->m_StdAttr.m_BackColor, &pLine->m_Attr.m_BackColor);

#if 0 // define to see repainted areas
	COLORREF Rgb = RGB(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
	axl_gr_TColor_SetRgb(&pPaint->m_StdBackColor, Rgb);
#endif

	axl_gr_TTextPaint_SetStdBackColor(pPaint);

	pPaint->m_x = -(long) pLogCtrl->m_FirstVisibleCol * pLogCtrl->m_CharSize.cx;
	pPaint->m_y = pLineRect->top;
	pPaint->m_top = pLineRect->top;
	pPaint->m_bottom = pLineRect->bottom;

	if (pLogCtrl->m_IsTimestampVisible && pLogCtrl->m_TimestampWidth)
		axl_log_TLogCtrl_PaintTimestamp(pLogCtrl, pLine);

	if (pLogCtrl->m_IconWidth)
		axl_log_TLogCtrl_PaintIcon(pLogCtrl, pLine);

	if (axl_log_TLine_IsBin(pLine) && pLogCtrl->m_OffsetWidth)
		axl_log_TLogCtrl_PaintOffset(pLogCtrl, (axl_log_TBinLine*) pLine);

	axl_gr_TTextPaint_SetStdTextColor(pPaint);

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		{
		axl_log_TLogTextLine* pTextLine = (axl_log_TLogTextLine*) pLine;
		axl_log_TLogCtrl_PaintText(
			pLogCtrl, 
			&pTextLine->m_HyperText.m_Text, 
			&pTextLine->m_HyperText.m_AttrArray, 
			Line, 0,
			pSelStart, pSelEnd, &SelAttr
			);
		break;
		}

	case axl_log_ELine_BinHex:
		axl_log_TLogCtrl_PaintBinHexLine(
			pLogCtrl, 
			pLineRect, 
			(axl_log_TBinLine*) pLine, 
			Line,
			pSelStart, pSelEnd, &SelAttr
			);
		break;

	case axl_log_ELine_BinText:
		{
		axl_log_TBinTextLine* pBinTextLine = (axl_log_TBinTextLine*) pLine;
		axl_log_TLogCtrl_PaintText(
			pLogCtrl, 
			&pBinTextLine->m_BinText, 
			&pBinTextLine->m_OverlayAttrArray,
			Line,
			pLogCtrl->m_FullOffsetWidth,
			pSelStart, pSelEnd, &SelAttr
			);
		break;
		}
	}

	if (pPaint->m_x < pLineRect->right)
	{
		axl_gr_TTextPaint_SetStdBackColor(pPaint);
		axl_gr_TTextPaint_PaintGap(pPaint, pLineRect->right - pPaint->m_x);
	}

	pNextLine = axl_log_TCacheManager_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
	Delimiter = axl_log_TLine_GetDelimiter(pLine, pNextLine);

	if (Delimiter)
	{
		RECT DelimRect = *pLineRect;
		DelimRect.top = pLineRect->bottom - 1;
		axl_gr_TTextPaint_SetBackColorIndex(pPaint, axl_log_EColorIndex_DelimiterLight + Delimiter - 1);
		axl_gr_FastRectangle(pPaint->m_hdc, &DelimRect);
	}
}

void 
AXL_API
axl_log_TLogCtrl_PaintRect(
	axl_log_TLogCtrl* pLogCtrl,
	HDC hdc,
	const RECT* pRect
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;

	RECT LineRect;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	RECT OffscreenRect;
	HDC hdcOffscreen;
#endif

	long FirstLine = pRect->top / pLogCtrl->m_CharSize.cy;
	long LastLine = pRect->bottom / pLogCtrl->m_CharSize.cy;

	if (pRect->bottom % pLogCtrl->m_CharSize.cy)
		LastLine++;

	LineRect.left = pRect->left;
	LineRect.top = FirstLine * pLogCtrl->m_CharSize.cy;
	LineRect.right = pRect->right;
	LineRect.bottom = LineRect.top + pLogCtrl->m_CharSize.cy;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	OffscreenRect.left = pRect->left;
	OffscreenRect.top = 0;
	OffscreenRect.right = pRect->right;
	OffscreenRect.bottom = pLogCtrl->m_CharSize.cy;

	hdcOffscreen = axl_gr_TOffscreenBuffer_GetBuffer(&pLogCtrl->m_OffscreenBuffer, hdc, pRect->right - pRect->left, pLogCtrl->m_CharSize.cy);
	SetViewportOrgEx(hdcOffscreen, -pRect->left, 0, NULL);

	axl_gr_TTextPaint_Attach(&pLogCtrl->m_TextPaint, hdcOffscreen);
#else
	axl_gr_TTextPaint_Attach(&pLogCtrl->m_TextPaint, hdc);
#endif

	FirstLine += pLogCtrl->m_FirstVisibleLine;
	LastLine += pLogCtrl->m_FirstVisibleLine;

	if (LastLine > (long) pLogCtrl->m_LineCount)
		LastLine = pLogCtrl->m_LineCount;

	if (FirstLine < LastLine)
	{
		long i;

		axl_log_TLine* pLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, FirstLine);
		for (i = FirstLine; i < LastLine; i++)
		{	
#ifdef _AXL_USE_OFFSCREEN_BUFFER
			axl_log_TLogCtrl_PaintLine(pLogCtrl, &OffscreenRect, pLine, i);
			BitBlt(
				hdc, 
				LineRect.left, LineRect.top, 
				LineRect.right - LineRect.left,
				LineRect.bottom - LineRect.top,
				hdcOffscreen,
				OffscreenRect.left, OffscreenRect.top,
				SRCCOPY
				);
#else
			axl_log_TLogCtrl_PaintLine(pLogCtrl, &LineRect, pLine, i);
#endif

			LineRect.top += pLogCtrl->m_CharSize.cy;
			LineRect.bottom += pLogCtrl->m_CharSize.cy;

			pLine = axl_log_TCacheManager_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
		}
	}

	axl_gr_TTextPaint_Detach(&pLogCtrl->m_TextPaint);

	if (pRect->bottom > LineRect.top)
	{
		LineRect.bottom = pRect->bottom;
		axl_gr_FastRectangleEx(hdc, &LineRect, pLogCtrl->m_ColorScheme [axl_log_EColorIndex_Back]);
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// find

bool_t
AXL_API
axl_log_TLogCtrl_FindNextImpl(
	axl_log_TLogCtrl* pLogCtrl,
	long Line, 
	long Col, 
	long EndLine
	)
{
	axl_log_TLine* pLine;

	if (!axl_rtl_TBmhFind_CanFind(&pLogCtrl->m_Find))
		return false;

	ASSERT(EndLine <= (long) pLogCtrl->m_LineCount);

	pLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, Line);
	while (Line < EndLine)
	{
/*			
		if (pLine->m_Type == axl_log_ELine_Text)
		{
			axl_log_TLogTextLine* pTextLine = (axl_log_TLogTextLine*) pLine;
			size_t Length = axl_rtl_TString_GetLength(&pTextLine->m_Text);

			size_t Offset = axl_rtl_TBmhFind_Find(&pLogCtrl->m_Find, pTextLine->m_Text.m_p + Col, Length - Col);
			if (Offset != -1)
			{
				Col += (long) Offset;
				axl_log_TLogCtrl_SetCursorPos(pLogCtrl, Line, Col, 0);
				axl_log_TLogCtrl_SetCursorPos(pLogCtrl, Line, Col + (long) pLogCtrl->m_Find.m_PatternSize, axl_log_ESetCursorPos_Select);
				return true;
			}
		}
		else
		{
		int StartLine = Line;
			int StartOffset = GetHexOffsetFromCol(pLine, Col, true);
			int MsgCode = pLine->pLogCtrl->m_MsgCode;

			pLogCtrl->m_Find.ResetIncrementalFind();

			size_t Offset = pLogCtrl->m_Find.IncrementalFind(pLine->pLogCtrl->m_HexData + StartOffset, HexLineSize - StartOffset);
			
			if (Offset != -1) // found
			{		
				Offset += StartOffset;
				Col = AsciiCol + Offset % HexLineSize;
				SetCursorPos(Line, Col);

				Offset += pLogCtrl->m_Find.pLogCtrl->m_PatternSize;
				Col = AsciiCol + Offset % HexLineSize;
				SetCursorPos(Line, Col, true);
				return true;
			}

			Line++;

			while (Line < EndLine)
			{
				pLine = GetLine(Line);
				if (pLine->pLogCtrl->m_MsgCode != MsgCode)
					break;

				Offset = pLogCtrl->m_Find.IncrementalFind(pLine->pLogCtrl->m_HexData, HexLineSize);

				if (Offset != -1) // found
				{				
					Offset += StartOffset;

					Line = StartLine + Offset / HexLineSize;
					Col = AsciiCol + Offset % HexLineSize;
					SetCursorPos(Line, Col);

					Offset += pLogCtrl->m_Find.pLogCtrl->m_PatternSize;
					Line = StartLine + Offset / HexLineSize;
					Col = AsciiCol + Offset % HexLineSize;
					SetCursorPos(Line, Col, true);
					return true;
				}

				Line++;
			} 			
		}
		*/

		pLine = axl_log_TCacheManager_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
		Line++;
		Col = 0; // col is used for 1st line only
	}

	return false;
}

bool_t
AXL_API
axl_log_TLogCtrl_FindNext(axl_log_TLogCtrl* pLogCtrl)
{
	bool_t Result;
	axl_log_TCursorPos SelStart;
	axl_log_TCursorPos SelEnd;

	if (!pLogCtrl->m_LineCount) // special case of empty log
		return false;

	axl_log_TLogCtrl_GetSelection(pLogCtrl, &SelStart, &SelEnd);

	Result = axl_log_TLogCtrl_FindNextImpl(pLogCtrl, SelEnd.m_Line, SelEnd.m_Col, pLogCtrl->m_LineCount);

	if (Result)
		return true;
	
	// nope, wrap after the end of log...
	return axl_log_TLogCtrl_FindNextImpl(pLogCtrl, 0, 0, SelEnd.m_Line + 1);
}

bool_t
AXL_API
axl_log_TLogCtrl_FindPrevImpl(
	axl_log_TLogCtrl* pLogCtrl,
	long Line, 
	long Col, 
	long EndLine
	)
{
	axl_log_TLine* pLine;

	if (!axl_rtl_TBmhFind_CanFind(&pLogCtrl->m_Find))
		return false;

	ASSERT(EndLine >= 0);

	pLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, Line);
	while (Line >= EndLine)
	{
/*
		if (pLine->m_Type == axl_log_ELine_Text)
		{
			axl_log_TLogTextLine* pTextLine = (axl_log_TLogTextLine*) pLine;
			size_t Offset;

			if (Col == -1)
				Col = (long) axl_rtl_TString_GetLength(&pTextLine->m_Text);

			Offset = axl_rtl_TBmhFind_ReverseFind(&pLogCtrl->m_Find, pTextLine->m_Text.m_p, Col);
			if (Offset != -1)
			{
				Col -= (long) Offset;
				axl_log_TLogCtrl_SetCursorPos(pLogCtrl, Line, Col, 0);
				axl_log_TLogCtrl_SetCursorPos(pLogCtrl, Line, Col + (long) pLogCtrl->m_Find.m_PatternSize, axl_log_ESetCursorPos_Select);
				return true;
			}
		}
		else
		{
			int StartLine = Line;
			int StartOffset = Col == -1 ? HexLineSize : GetHexOffsetFromCol(pLine, Col, true);
			int MsgCode = pLine->pLogCtrl->m_MsgCode;

			pLogCtrl->m_Find.ResetIncrementalFind();

			size_t Offset = pLogCtrl->m_Find.ReverseIncrementalFind(pLine->pLogCtrl->m_HexData, StartOffset);
			
			if (Offset != -1) // found
			{		
				Offset = StartOffset - Offset;
				Col = AsciiCol + Offset % HexLineSize;
				SetCursorPos(Line, Col);

				Offset += pLogCtrl->m_Find.pLogCtrl->m_PatternSize;
				Col = AsciiCol + Offset % HexLineSize;
				SetCursorPos(Line, Col, true);
				return true;
			}

			Line--;

			while (Line >= EndLine)
			{
				pLine = GetLine(Line);
				if (pLine->pLogCtrl->m_MsgCode != MsgCode)
					break;

				Offset = pLogCtrl->m_Find.ReverseIncrementalFind(pLine->pLogCtrl->m_HexData, HexLineSize);

				if (Offset != -1) // found
				{				
					ASSERT(Offset > (size_t) StartOffset); // because we moved to another line already
					Offset -= StartOffset;

					size_t m = Offset % HexLineSize;
					Offset = m ? HexLineSize - m : 0;

					Col = AsciiCol + Offset;
					SetCursorPos(Line, Col);

					Offset += pLogCtrl->m_Find.pLogCtrl->m_PatternSize;
					Line = Line + Offset / HexLineSize;
					Col = AsciiCol + Offset % HexLineSize;
					SetCursorPos(Line, Col, true);
					return true;
				}

				Line--;
			} 			
		} 
		*/

		pLine = axl_log_TCacheManager_GetPrevLine(&pLogCtrl->m_CacheMgr, pLine);
		Line--;
		Col = -1; // col is used for 1st line only
	}

	return false;
}

bool_t
AXL_API
axl_log_TLogCtrl_FindPrev(axl_log_TLogCtrl* pLogCtrl)
{
	bool_t Result;
	axl_log_TCursorPos SelStart;
	axl_log_TCursorPos SelEnd;

	if (!pLogCtrl->m_LineCount) // special case of empty log
		return false;

	axl_log_TLogCtrl_GetSelection(pLogCtrl, &SelStart, &SelEnd);

	Result = axl_log_TLogCtrl_FindPrevImpl(pLogCtrl, SelStart.m_Line, SelStart.m_Col, 0);
	if (Result)
		return true;

	// nope, wrap after the beginning of log...
	return axl_log_TLogCtrl_FindPrevImpl(pLogCtrl, pLogCtrl->m_LineCount - 1, -1, SelStart.m_Line);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// selection processing

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
axl_log_TLogCtrl_ProcessSelAsBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_FProcessBinBlock pfnProcess, 
	void* pContext
	)
{
	axl_log_TBinLine* pLine;
	long i;
	size_t LineSize;

	axl_log_TCursorPos SelStart;
	axl_log_TCursorPos SelEnd;

	size_t StartFullOffset = -1;
	size_t StartLineOffset = -1;
	ulong_t StartMergeId = 0;
	int StartHexCol;

	size_t EndFullOffset = -1;
	size_t EndLineOffset = -1;
	ulong_t EndMergeId = 0;
	int EndHexCol;

	size_t BufferSize;

	bool_t IsNonEmptySel = axl_log_TLogCtrl_GetSelection(pLogCtrl, &SelStart, &SelEnd);
    if (!IsNonEmptySel)
		return 0;

	axl_log_TLogCtrl_GetBinOffset(pLogCtrl, &SelStart, &StartFullOffset, &StartLineOffset, &StartHexCol, &StartMergeId);
	axl_log_TLogCtrl_GetBinOffset(pLogCtrl, &SelEnd, &EndFullOffset, &EndLineOffset, &EndHexCol, &EndMergeId);

	if (StartFullOffset == -1 || EndFullOffset == -1 || StartMergeId != EndMergeId)
		return 0;

	if (EndHexCol)
	{
		EndFullOffset++;
		EndLineOffset++;
	}

    // first line

	pLine = (axl_log_TBinLine*) axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, SelStart.m_Line);
	ASSERT(axl_log_TLine_IsBin(pLine));

	if (SelStart.m_Line == SelEnd.m_Line) // special case -- single line
	{
		pfnProcess(pContext, (uchar_t*) pLine->m_BinData.m_p + StartLineOffset, EndLineOffset - StartLineOffset);
		return EndFullOffset - StartFullOffset;
	}

	axl_rtl_TByteArray_Clear(&pLogCtrl->m_BinBlockBuffer);

	LineSize = axl_log_TBinLine_GetSize(pLine);
	ASSERT(axl_rtl_TByteArray_GetCount(&pLine->m_BinData) == LineSize);

	axl_log_TLogCtrl_IncapsulateBinBlock(pLogCtrl, (uchar_t*) pLine->m_BinData.m_p + StartLineOffset, LineSize - StartLineOffset, pfnProcess, pContext);

	// middle lines

	for (i = SelStart.m_Line + 1; i < SelEnd.m_Line; i++)
	{
		pLine = (axl_log_TBinLine*) axl_log_TCacheManager_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
		ASSERT(axl_log_TLine_IsBin(pLine));
		ASSERT(axl_rtl_TByteArray_GetCount(&pLine->m_BinData) == LineSize);
		axl_log_TLogCtrl_IncapsulateBinBlock(pLogCtrl, pLine->m_BinData.m_p, LineSize, pfnProcess, pContext);
	}

	// last line

	pLine = (axl_log_TBinLine*) axl_log_TCacheManager_GetNextLine(&pLogCtrl->m_CacheMgr, (axl_log_TLine*) pLine);
	ASSERT(axl_log_TLine_IsBin(pLine));
	axl_log_TLogCtrl_IncapsulateBinBlock(pLogCtrl, pLine->m_BinData.m_p, EndLineOffset, pfnProcess, pContext);

	BufferSize = axl_rtl_TByteArray_GetCount(&pLogCtrl->m_BinBlockBuffer);
	if (BufferSize)
		pfnProcess(pContext, pLogCtrl->m_BinBlockBuffer.m_p, BufferSize);

	axl_rtl_TByteArray_Clear(&pLogCtrl->m_BinBlockBuffer);

	return EndFullOffset - StartFullOffset;
}

/*

bool_t
AXL_API
axl_log_TLogCtrl_ProcessSelAsBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_FProcessBinBlock pfnProcess, 
	void* pContext
	)
{
	bool_t Result;
	axl_log_TCursorPos SelStart;
	axl_log_TCursorPos SelEnd;
	axl_log_TLine* pLine;
	long i;

	if (!pLogCtrl->m_LineCount) // special case of empty log
		return false;

	Result = axl_log_TLogCtrl_GetSelection(pLogCtrl, &SelStart, &SelEnd);
	if (!Result)
		return 0;

	// first line

	pLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, SelStart.m_Line);

	if (SelStart.m_Line == SelEnd.m_Line) // special case -- single line
	{
//		pfnProcess(pContext, pLine, SelStart.m_Col, SelEnd.m_Col, false);
		return 1;
	}

//	pfnProcess(pContext, pLine, SelStart.m_Col, MAXLONG, true);

	// middle lines

	for (i = SelStart.m_Line + 1; i < SelEnd.m_Line; i++)
	{
		pLine = axl_log_TCacheManager_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
//		pfnProcess(pContext, pLine, 0, MAXLONG, true);
	}

	// last line

	pLine = axl_log_TCacheManager_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
//	pfnProcess(pContext, pLine, 0, SelEnd.m_Col, true);

	return SelEnd.m_Line - SelStart.m_Line + 1;
}

*/

void 
AXL_API
axl_log_TLogCtrl_CopyText(
	axl_rtl_TString* pString, 
	axl_log_TLogTextLine* pLine, 
	long FromCol, 
	long ToCol
	)
{
	size_t Length = axl_rtl_TString_GetLength(&pLine->m_HyperText.m_Text);

	if (FromCol > (long) Length)
		FromCol = (long) Length;

	if (ToCol > (long) Length)
		ToCol = (long) Length;

	if (FromCol < ToCol)
		axl_rtl_TString_AppendString(pString, pLine->m_HyperText.m_Text.m_p + FromCol, ToCol - FromCol);
}

int 
AXL_API
axl_log_TLogCtrl_GetHexOffsetFromCol(
	axl_log_TBinLine* pLine, 
	long Col, 
	bool_t IsSelBegin
	)
{
	size_t DataSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);

	if (Col <= (long) pLine->m_DataConfig.m_BinHexLineSize * 3)
	{
		long m = Col % 3;

		Col /= 3;

		if (IsSelBegin)
		{
			if (m == 2)
				Col++;
		}
		else
		{
			if (m != 0)
				Col++;
		}
	}
	else
	{
		Col -= (long) pLine->m_DataConfig.m_BinHexLineSize * 3 + axl_log_EDef_HexGapSize;
		if (Col < 0)
			Col = 0;
	}

	if (Col > (long) DataSize)
		Col = (long) DataSize;

	return Col;
}

void 
AXL_API
axl_log_TLogCtrl_CopyBinHex(
	axl_rtl_TString* pString, 
	axl_log_TBinLine* pLine, 
	long FromCol, 
	long ToCol,
	bool_t IsMultiLine
	)
{
/*	FromCol = axl_log_TLogCtrl_GetHexOffsetFromCol(pLine, FromCol, true);
	ToCol = axl_log_TLogCtrl_GetHexOffsetFromCol(pLine, ToCol, false);	

	if (IsMultiLine && FromCol != 0)
		axl_rtl_TString_AppendChar(pString, _T(' '), FromCol * 3);

	axl_rtl_TString_AppendString(pString, pLine->m_HexPart.m_p + FromCol * 3, (ToCol - FromCol) * 3);
	
	if (!IsMultiLine)
		axl_rtl_TString_AppendChar(pString, _T(' '), 2);
	else
		axl_rtl_TString_AppendChar(pString, _T(' '), (pLine->m_DataConfig.m_BinHexLineSize - ToCol) * 3 + FromCol + 2);

	axl_rtl_TString_AppendString(pString, pLine->m_AsciiPart.m_p + FromCol, ToCol - FromCol); */
}

void 
AXL_API
axl_log_TLogCtrl_CopyBinText(
	axl_rtl_TString* pString, 
	axl_log_TBinTextLine* pLine, 
	long FromCol, 
	long ToCol
	)
{
}

void 
AXL_API
axl_log_TLogCtrl_ProcessCopy(
	void* pContext, 
	axl_log_TLine* pLine, 
	long FromCol, 
	long ToCol, 
	bool_t IsMultiLine
	)
{
	axl_rtl_TString* pString = (axl_rtl_TString*) pContext;

	if (!axl_rtl_TString_IsEmpty(pString))
		axl_rtl_TString_AppendString(pString, _T("\r\n"), 2);

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		axl_log_TLogCtrl_CopyText(pString, (axl_log_TLogTextLine*) pLine, FromCol, ToCol);
		break;

	case axl_log_ELine_BinHex:
		axl_log_TLogCtrl_CopyBinHex(pString, (axl_log_TBinLine*) pLine, FromCol, ToCol, IsMultiLine);
		break;

	case axl_log_ELine_BinText:
		axl_log_TLogCtrl_CopyBinText(pString, (axl_log_TBinTextLine*) pLine, FromCol, ToCol);
		break;
	}
}

bool_t
AXL_API
axl_log_TLogCtrl_Copy(axl_log_TLogCtrl* pLogCtrl)
{
	BOOL Result;
	size_t LineCount;
	size_t Size;
	HGLOBAL hData;
	void* p;
	axl_rtl_TString String;
	axl_rtl_TString_Construct(&String);
	
	LineCount = 0; 
	// LineCount = axl_log_TLogCtrl_ProcessSelection(pLogCtrl, axl_log_TLogCtrl_ProcessCopy, &String);
	if (!LineCount)
	{
		axl_rtl_TString_Destruct(&String);
		return true;
	}

	if (LineCount >= 2) 
		axl_rtl_TString_AppendString(&String, _T("\r\n"), 2);

	Result = OpenClipboard(pLogCtrl->m_hWnd);
	if (!Result)
	{
		axl_rtl_TString_Destruct(&String);
		return false;
	}

	EmptyClipboard();

	Size = (axl_rtl_TString_GetLength(&String) + 1) * sizeof(tchar_t);
	hData = GlobalAlloc(GMEM_MOVEABLE, Size);
	p = GlobalLock(hData);
	memcpy(p, String.m_p, Size);
	SetClipboardData(CF_TEXT, hData);
	
	CloseClipboard(); 
	axl_rtl_TString_Destruct(&String);
	return true;
}

bool_t
AXL_API
axl_log_TLogCtrl_ExportToTextFile(
	axl_log_TLogCtrl* pLogCtrl,
	const tchar_t* pFileName
	)
{
/*	axl::io::CFile File;
	File.Open(pFileName);
	File.SetSize(0);

	for (int i = 0; i < m_LineCount; i++)
	{
		CLine* pLine = GetLine(i);
		File.Write(pLine->m_Text, pLine->m_Text.GetLength() * sizeof(tchar_t));
		File.Write(_T("\r\n"), 2 * sizeof(tchar_t));
	}
*/

	return true;
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

	// see http://pluralsight.com/blogs/keith/archive/2005/01/13/4940.aspx
#pragma warning(disable:4244)
	SetWindowLongPtr(pLogCtrl->m_hWnd, 0, 0);
#pragma warning(default:4244)

	pLogCtrl->m_hWnd = NULL;

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

LRESULT 
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
		return 0;

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

	return 0;
}

bool_t
AXL_API
axl_log_TLogCtrl_GetHyperLinkFromMousePos(
	axl_log_TLogCtrl* pLogCtrl,
	long x,
	long y,
	axl_log_TLine** ppLine,
	const tchar_t** ppHyperLink
	)
{
	axl_log_TLine* pLine;
	axl_log_TLogTextLine* pTextLine;
	axl_gr_THyperLinkAnchor* pAnchor;

	axl_log_TCursorPos Pos;
	axl_log_TLogCtrl_GetCursorPosFromMousePos(pLogCtrl, x, y, &Pos, false);

	pLine = axl_log_TCacheManager_GetLine(&pLogCtrl->m_CacheMgr, Pos.m_Line);
	if (!pLine || pLine->m_Type != axl_log_ELine_Text)
		return false;

	pTextLine = (axl_log_TLogTextLine*) pLine;

	pAnchor = axl_gr_THyperLinkAnchorArray_Find(&pTextLine->m_HyperText.m_HyperLinkArray, Pos.m_Col);
	if (!pAnchor || axl_rtl_TString_IsEmpty(&pAnchor->m_HyperLink))
		return false;

	if (ppLine)
		*ppLine = pLine;

	if (ppHyperLink)
		*ppHyperLink = pAnchor->m_HyperLink.m_p;

	return true;
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

	if (!axl_log_TLogCtrl_GetHyperLinkFromMousePos(pLogCtrl, MousePos.x, MousePos.y, NULL, NULL))
		return DefWindowProc(pLogCtrl->m_hWnd, WM_SETCURSOR, wParam, lParam);

	SetCursor(pLogCtrl->m_hHyperLinkCursor);
	return TRUE;
}

void
AXL_API
axl_log_TLogCtrl_OnHyperLink(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	const tchar_t* pHyperLink
	)
{
	if (pLogCtrl->m_pHyperLinkHandler)
	{
		bool_t Result = pLogCtrl->m_pHyperLinkHandler->m_pfnOnHyperLink(pLogCtrl->m_pHyperLinkHandler, pLogCtrl, pLine, pHyperLink);
		if (Result)
			return;
	}

	switch (*pHyperLink)
	{
	case '+': // add/remove bits to volatile flags
	case '-': 
		{
		ulong_t Modify;
		ulong_t NewVolatileFlags;

		if (!pLine->m_pVolatileMessage)
			break;

		Modify = _tcstoul(pHyperLink + 1, NULL, 10);
		if (!Modify)
			break;

		NewVolatileFlags = *pHyperLink == '+' ? 
			pLine->m_pVolatileMessage->m_VolatileFlags | Modify :
			pLine->m_pVolatileMessage->m_VolatileFlags & ~Modify;

		axl_log_TLogCtrl_ReRepresentMessage(pLogCtrl, pLine, NewVolatileFlags);
		break;
		}

	case '!': // hilite bin range
		{
		tchar_t* pEnd;
		size_t Offset;
		size_t Length;

		Offset = _tcstoul(pHyperLink + 1, &pEnd, 10);
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
		ShellExecute(0, _T("open"), pHyperLink, 0, 0, SW_SHOWNORMAL);
	}	
}

LRESULT 
AXL_API
axl_log_TLogCtrl_OnLButtonDown(
	axl_log_TLogCtrl* pLogCtrl,
	long x,
	long y
	)
{
	axl_log_TLine* pHyperLinkLine;
	const tchar_t* pHyperLink;

	axl_log_TCursorPos Pos;
	bool_t IsShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

	if (axl_log_TLogCtrl_GetHyperLinkFromMousePos(pLogCtrl, x, y, &pHyperLinkLine, &pHyperLink))
	{
		axl_log_TLogCtrl_OnHyperLink(pLogCtrl, pHyperLinkLine, pHyperLink);
		return 0;
	}

	axl_log_TLogCtrl_GetCursorPosFromMousePos(pLogCtrl, x, y, &Pos, true);

	// handle selection

	if (!IsShiftPressed)
	{
		axl_log_TLogCtrl_KillSelection(pLogCtrl);
		axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos);

		// SetSelStart(), SetSelEnd() with validations and repainting are not really needed here
		pLogCtrl->m_SelStart = pLogCtrl->m_CursorPos;
		pLogCtrl->m_SelEnd = pLogCtrl->m_CursorPos;
	}
	else 
	{
		if (!pLogCtrl->m_IsSelecting)
			axl_log_TLogCtrl_SetSelStart(pLogCtrl, &pLogCtrl->m_CursorPos);
		
		axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos);
		axl_log_TLogCtrl_SetSelEnd(pLogCtrl, &pLogCtrl->m_CursorPos);
	}
  
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &pLogCtrl->m_CursorPos);

	pLogCtrl->m_IsSelecting = true;
	pLogCtrl->m_IsDragging = true;

	SetFocus(pLogCtrl->m_hWnd);
	SetCapture(pLogCtrl->m_hWnd);

	axl_log_TLogCtrl_NotifyParent(pLogCtrl, axl_log_ECtrlNm_CursorPosChange, NULL);
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
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos);
	axl_log_TLogCtrl_SetSelEnd(pLogCtrl, &pLogCtrl->m_CursorPos);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &pLogCtrl->m_CursorPos);
	axl_log_TLogCtrl_NotifyParent(pLogCtrl, axl_log_ECtrlNm_CursorPosChange, NULL);
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
AXL_API
axl_log_TLogCtrl_OnKeyDown(
	axl_log_TLogCtrl* pLogCtrl,
	int VkCode
	)
{
	bool_t bShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bool_t bCtrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

/*
	switch (VkCode)
	{
	case VK_TAB:
		OnKeyTab();
		break;

	case VK_LEFT:
		OnKeyLeft(bShift, bCtrl);
		break;

	case VK_RIGHT:
		OnKeyRight(bShift, bCtrl);
		break;

	case VK_UP:
		OnKeyUp(bShift, bCtrl);
		break;

	case VK_DOWN:
		OnKeyDown(bShift, bCtrl);
		break;

	case VK_PRIOR:
		OnKeyPageUp(bShift, bCtrl);
		break;

	case VK_NEXT:
		OnKeyPageDown(bShift, bCtrl);
		break;

	case VK_HOME:
		OnKeyHome(bShift, bCtrl);
		break;

	case VK_END:
		OnKeyEnd(bShift, bCtrl);
		break;

	case VK_INSERT:
	case _T('C'):
		if (bCtrl && !bShift)
			Copy();
		break;
	};

*/

	return 0;
}

LRESULT 
CALLBACK 
axl_log_TLogCtrl_WndProc(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	axl_log_TLogCtrl* pLogCtrl;
	LRESULT lResult = 0;

	if (Msg == WM_CREATE) // special handling
	{
		// establish connection axl_log_TLogCtrl <-> HWND

		CREATESTRUCT* p = (CREATESTRUCT*) lParam;
		pLogCtrl = (axl_log_TLogCtrl*) p->lpCreateParams;
		pLogCtrl->m_hWnd = hWnd;

		// see http://pluralsight.com/blogs/keith/archive/2005/01/13/4940.aspx
#pragma warning(disable:4244)
		SetWindowLongPtr(hWnd, 0, (LONG_PTR) pLogCtrl);
#pragma warning(default:4244)

		return axl_log_TLogCtrl_OnCreate(pLogCtrl);
	}

	pLogCtrl = (axl_log_TLogCtrl*) (LONG_PTR) GetWindowLongPtr(hWnd, 0);
	if (!pLogCtrl) // control was detached
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	if (Msg >= WM_MOUSEFIRST && Msg <= WM_MOUSELAST)
	{
		MSG MsgStruct = { hWnd, Msg, wParam, lParam };

		if (pLogCtrl->m_hWndToolTip)
			SendMessage(pLogCtrl->m_hWndToolTip, TTM_RELAYEVENT, 0, (LPARAM) &MsgStruct);
	}

	switch (Msg)
	{
	case WM_DESTROY:
		lResult = axl_log_TLogCtrl_OnDestroy(pLogCtrl);
		break;

	case WM_GETDLGCODE:
		lResult = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_SETFOCUS:
		lResult = axl_log_TLogCtrl_OnSetFocus(pLogCtrl);
		break;

	case WM_KILLFOCUS:
		lResult = axl_log_TLogCtrl_OnKillFocus(pLogCtrl);
		break;

    case WM_THEMECHANGED:
		axl_gr_TStockCtrlPaint_CloseThemes(&pLogCtrl->m_StockCtrlPaint);
		break;

	case WM_NCPAINT:
		axl_gr_TStockCtrlPaint_NcPaintEdge(&pLogCtrl->m_StockCtrlPaint, hWnd, (HRGN) wParam);
		break;

	case WM_PAINT:
		lResult = axl_log_TLogCtrl_OnPaint(pLogCtrl);
		break;

	case WM_SIZE:
		lResult = axl_log_TLogCtrl_OnSize(pLogCtrl);
		break;

	case WM_HSCROLL:
		lResult = axl_log_TLogCtrl_OnScroll(pLogCtrl, SB_HORZ, LOWORD(wParam));
		break;

	case WM_VSCROLL:
		lResult = axl_log_TLogCtrl_OnScroll(pLogCtrl, SB_VERT, LOWORD(wParam));
		break;

	case WM_SETCURSOR:
		lResult = axl_log_TLogCtrl_OnSetCursor(pLogCtrl, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		lResult = axl_log_TLogCtrl_OnLButtonDown(pLogCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_RBUTTONDOWN:
		lResult = axl_log_TLogCtrl_OnRButtonDown(pLogCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSEMOVE:
		lResult = axl_log_TLogCtrl_OnMouseMove(pLogCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSEWHEEL:
		lResult = axl_log_TLogCtrl_OnMouseWheel(pLogCtrl, HIWORD(wParam));
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
		lResult = axl_log_TLogCtrl_OnKeyDown(pLogCtrl, (int) wParam);
		break;

	default:
		lResult = DefWindowProc(hWnd, Msg, wParam, lParam);
	}
 
	return lResult;
}

//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................
//.............................................................................

#if 0

//.............................................................................

// message handlers

void axl_log_TLogCtrl_OnKeyTab()
{
	axl_log_TLine* pLine = GetLineAtCursor();
	if (!pLine->IsHex())
		return;

	if (m_CursorPos.m_Col >= AsciiCol)
		SetCursorPos(m_CursorPos.m_Line, (m_CursorPos.m_Col - AsciiCol) * 3);
	else
		SetCursorPos(m_CursorPos.m_Line, AsciiCol + m_CursorPos.m_Col / 3);
}

void axl_log_TLogCtrl_OnKeyLeft(bool bShift, bool bCtrl)
{
	axl_log_TLine* pLine = GetLineAtCursor();
	if (pLine->IsHex())
	{
		// need some more logic over here

		int Col = bCtrl ? 0 : m_CursorPos.m_Col - 1;
		SetCursorPos(m_CursorPos.m_Line, Col, bShift);
	}
	else
	{
		int Col = bCtrl ? 0 : m_CursorPos.m_Col - 1;
		SetCursorPos(m_CursorPos.m_Line, Col, bShift);
	}
}

void axl_log_TLogCtrl_OnKeyRight(bool bShift, bool bCtrl)
{
	axl_log_TLine* pLine = GetLineAtCursor();
	if (pLine->IsHex())
	{
		// need some more logic over here 

		int Col = bCtrl ? (int) pLine->m_Text.GetLength() : m_CursorPos.m_Col + 1;
		SetCursorPos(m_CursorPos.m_Line, Col, bShift);
	}
	else
	{
		int Col = bCtrl ? (int) pLine->m_Text.GetLength() : m_CursorPos.m_Col + 1;
		SetCursorPos(m_CursorPos.m_Line, Col, bShift);
	}
}

void axl_log_TLogCtrl_OnKeyUp(bool bShift, bool bCtrl)
{
	SetCursorPos(m_CursorPos.m_Line - 1, m_CursorPos.m_Col, bShift);
}

void axl_log_TLogCtrl_OnKeyDown(bool bShift, bool bCtrl)
{
	SetCursorPos(m_CursorPos.m_Line + 1, m_CursorPos.m_Col, bShift);
}

void axl_log_TLogCtrl_OnKeyPageUp(bool bShift, bool bCtrl)
{
	int Line = bCtrl ? 
		m_FirstVisibleLine : 
		m_CursorPos.m_Line - m_VisibleLineCount;
	
	SetCursorPos(Line, m_CursorPos.m_Col, bShift);
}

void axl_log_TLogCtrl_OnKeyPageDown(bool bShift, bool bCtrl)
{
	int Line = bCtrl ? 
		m_FirstVisibleLine + m_VisibleLineCount - 1 : 
		m_CursorPos.m_Line + m_VisibleLineCount;

	SetCursorPos(Line, m_CursorPos.m_Col, bShift);
}

void axl_log_TLogCtrl_OnKeyHome(bool bShift, bool bCtrl)
{
	if (bCtrl)
	{
		SetCursorPos(0, 0, bShift);
		return;
	}

	SetCursorPos(m_CursorPos.m_Line, 0, bShift);
}

void axl_log_TLogCtrl_OnKeyEnd(bool bShift, bool bCtrl)
{
	int Line = bCtrl ? m_LineCount - 1 : m_CursorPos.m_Line;
	axl_log_TLine* pLine = GetLine(Line);
	SetCursorPos(Line, m_BaseCol + (int) pLine->m_Text.GetLength(), bShift);
}

int axl_log_TLogCtrl_GetTextExOffsetFromCol(CLineData* pLine, int Col)
{
	if (Col < 0)
		Col = 0;

	if (Col > (int) pLine->m_Text.GetLength())
		Col = pLine->m_Text.GetLength();

	if (Col >= (int) pLine->m_TextExMapping.GetCount())
		return pLine->m_TextEx.GetCount();

	Col = pLine->m_TextExMapping [Col].m_TextExPosition;
	return Col;
}

int axl_log_TLogCtrl_GetSelRangeArray(
	CSelRange SelRanges [2],
	const CCursorPos& SelStart,
	const CCursorPos& SelEnd,
	axl_log_TLine* pLine,
	int Line
	)
{
	if (!pLine->IsHex())
	{
		if (Line == SelStart.m_Line)
		{
			SelRanges [0].m_From = SelStart.m_Col;
			SelRanges [0].m_To = Line == SelEnd.m_Line ? SelEnd.m_Col : (int) pLine->m_Text.GetLength();
		}
		else 
		{
			SelRanges [0].m_From = Line == SelStart.m_Line ? SelStart.m_Col : 0;
			SelRanges [0].m_To = SelEnd.m_Col;
		}

		return 1;
	}

	int FromOffset;
	int ToOffset;

	if (Line == SelStart.m_Line)
	{
		if (Line == SelEnd.m_Line) // single line
		{
			FromOffset = GetHexOffsetFromCol(pLine, SelStart.m_Col, true);
			ToOffset = GetHexOffsetFromCol(pLine, SelEnd.m_Col, false);

			if (FromOffset > ToOffset)
			{
				FromOffset = GetHexOffsetFromCol(pLine, SelEnd.m_Col, true);
				ToOffset = GetHexOffsetFromCol(pLine, SelStart.m_Col, false);
			}
		}
		else
		{
			FromOffset = GetHexOffsetFromCol(pLine, SelStart.m_Col, true);
			ToOffset = pLine->m_HexSize; // GetHexOffsetFromCol(pLine, LastChar, false);
		}
	}
	else 
	{
		FromOffset = GetHexOffsetFromCol(pLine, 0, true);
		ToOffset = GetHexOffsetFromCol(pLine, SelEnd.m_Col, false);
	}

	if (FromOffset == ToOffset)
		return 0;

	SelRanges [0].m_From = FromOffset * 3;
	SelRanges [0].m_To = ToOffset * 3 - 1;

	SelRanges [1].m_From = HexLineSize * 3 + HexGapSize + FromOffset;
	SelRanges [1].m_To = HexLineSize * 3 + HexGapSize + ToOffset;

	return 2;
}

#endif