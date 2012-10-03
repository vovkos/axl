// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_sys_Window.h"
#include "axl_log_MessageFile.h"
#include "axl_log_IndexMgr.h"
#include "axl_log_CacheMgr.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_log_LogCtrl_msg.h"

#include "axl_log_Filter.h"
#include "axl_log_Representor.h"
#include "axl_log_HyperlinkHandler.h"

#include "axl_rtl_Array.h"
#include "axl_rtl_BmhFind.h"

#include "axl_gr_TextPaint.h"
#include "axl_gr_StockCtrlPaint.h"
#include "axl_gr_OffscreenBuffer.h"

#ifdef __cplusplus
#error C compiler required
#endif

//.............................................................................

enum
{
	axl_log_EColor_Text = axl_gr_EStdPalette_Size | axl_gr_EColorFlags_Index,
	axl_log_EColor_Back,
	axl_log_EColor_SelectionText,
	axl_log_EColor_SelectionBack,
	axl_log_EColor_HiliteText,
	axl_log_EColor_HiliteBack,
	axl_log_EColor_DelimiterLight,
	axl_log_EColor_DelimiterMedium,
	axl_log_EColor_DelimiterBold,
	axl_log_EColor_Timestamp,
	axl_log_EColor_Offset,
	
	axl_log_EColor_Count = ((axl_log_EColor_Offset + 1) & ~axl_gr_EColorFlags_Index)
};

//.............................................................................

typedef struct _axl_log_TLogCtrl axl_log_TLogCtrl;

void
AXL_API
axl_log_TLogCtrl_Construct(axl_log_TLogCtrl* pLogCtrl);

void
AXL_VFT
axl_log_TLogCtrl_Destruct(axl_log_TLogCtrl* pLogCtrl);

LRESULT 
AXL_VFT
axl_log_TLogCtrl_WindowProc(
	axl_log_TLogCtrl* pLogCtrl,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam,
	bool_t* pIsHandled
	);

void 
AXL_API
axl_log_TLogCtrl_RecalcColCount(axl_log_TLogCtrl* pLogCtrl);

void 
AXL_API
axl_log_TLogCtrl_RecalcBaseCol(axl_log_TLogCtrl* pLogCtrl);

void 
AXL_API
axl_log_TLogCtrl_RecalcHScroll(axl_log_TLogCtrl* pLogCtrl);

void 
AXL_API
axl_log_TLogCtrl_RecalcVScroll(axl_log_TLogCtrl* pLogCtrl);

bool_t
AXL_API
axl_log_TLogCtrl_FixupFirstVisibleLine(axl_log_TLogCtrl* pLogCtrl);

bool_t
AXL_API
axl_log_TLogCtrl_FixupFirstVisibleCol(axl_log_TLogCtrl* pLogCtrl);

void 
AXL_API
axl_log_TLogCtrl_FullUpdateLog(axl_log_TLogCtrl* pLogCtrl);

LRESULT
AXL_API
axl_log_TLogCtrl_NotifyParent(
	axl_log_TLogCtrl* pLogCtrl,
	uint_t Code,
	NMHDR* pNotify
	);

bool_t
AXL_API
axl_log_TLogCtrl_SetMessageFile(
	axl_log_TLogCtrl* pLogCtrl, 
	axl_log_TMessageFile* pFile,
	axl_log_TRepresentor* pRepresentor
	);

void
AXL_API
axl_log_TLogCtrl_SetRepresentor(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TRepresentor* pRepresentor
	);

void
AXL_API
axl_log_TLogCtrl_SetColorizer(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TColorizer* pColorizer
	);

#define axl_log_TLogCtrl_SetFilter(pLogCtrl, pFilter) \
	axl_ref_TObject__CopyPtr( \
		(axl_ref_TObject**) &(pLogCtrl)->m_pFilter, \
		(axl_ref_TObject*) (pFilter) \
		)

#define axl_log_TLogCtrl_SetHyperlinkHandler(pLogCtrl, pHandler) \
	axl_ref_TObject__CopyPtr( \
		(axl_ref_TObject**) &(pLogCtrl)->m_pHyperlinkHandler, \
		(axl_ref_TObject*) (pHandler) \
		)

void 
AXL_API
axl_log_TLogCtrl_ShowIcon(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t IsVisible
	);

void 
AXL_API
axl_log_TLogCtrl_SetImageList(
	axl_log_TLogCtrl* pLogCtrl,
	HIMAGELIST hImageList
	);

void 
AXL_API
axl_log_TLogCtrl_ShowTimestamp(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t IsVisible
	);

void 
AXL_API
axl_log_TLogCtrl_SetTimestampFormat(
	axl_log_TLogCtrl* pLogCtrl,
	const tchar_t* pFormat
	);

void 
AXL_API
axl_log_TLogCtrl_SetOffsetWidth(
	axl_log_TLogCtrl* pLogCtrl,
	ulong_t Width
	);

void
AXL_API
axl_log_TLogCtrl_UpdateLog(axl_log_TLogCtrl* pLogCtrl);

bool_t
AXL_API
axl_log_TLogCtrl_ModifyMessageVolatileFlags(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCachePage* pPage,
	axl_log_TCacheVolatileMessage* pVolatileMsg,
	ulong_t VolatileFlags
	);

bool_t
AXL_API
axl_log_TLogCtrl_ModifyLineVolatileFlags(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	ulong_t VolatileFlags
	);

void
AXL_API
axl_log_TLogCtrl_ReRepresentAll(axl_log_TLogCtrl* pLogCtrl);

void
AXL_API
axl_log_TLogCtrl_ReFilterAll(axl_log_TLogCtrl* pLogCtrl);

void
AXL_API
axl_log_TLogCtrl_ReColorizeAll(axl_log_TLogCtrl* pLogCtrl);

bool_t
AXL_API
axl_log_TLogCtrl_IsVScrollAtBottom(axl_log_TLogCtrl* pLogCtrl);

void 
AXL_API
axl_log_TLogCtrl_UpdateCaretPos(axl_log_TLogCtrl* pLogCtrl);

#define axl_log_TLogCtrl_IsCursorAtLastLine(pLogCtrl) \
	((pLogCtrl)->m_LineCount == 0 || (pLogCtrl)->m_CursorPos.m_Line == (pLogCtrl)->m_LineCount - 1)

#define axl_log_TLogCtrl_GetAsciiCol(pLogCtrl, pLine) \
	((long) ((pLogCtrl)->m_FullOffsetWidth + 3 * (pLine)->m_DataConfig.m_BinHexLineSize + axl_log_EDef_HexGapSize))

size_t
AXL_API
axl_log_TLogCtrl_GetBinHexLineOffset(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine,
	long Col,
	int* pHexCol
	);

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
	);

void 
AXL_API
axl_log_TLogCtrl_ValidateCursorPos(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	);

void 
AXL_API
axl_log_TLogCtrl_SetCursorPos(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos,
	bool_t Select
	);

bool_t
AXL_API
axl_log_TLogCtrl_GetRangeBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd,
	size_t* pOffset,
	size_t* pSize
	);

bool_t
AXL_API
axl_log_TLogCtrl_GetSelectionBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	size_t* pOffset,
	size_t* pSize
	);

size_t
AXL_API
axl_log_TLogCtrl_ProcessRange(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd,
	axl_log_FProcessLine pfnProcess, 
	void* pContext
	);

size_t
AXL_API
axl_log_TLogCtrl_ProcessSelection(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_FProcessLine pfnProcess, 
	void* pContext
	);

size_t
AXL_API
axl_log_TLogCtrl_ProcessRangeAsBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd,
	axl_log_FProcessBinBlock pfnProcess, 
	void* pContext
	);

size_t
AXL_API
axl_log_TLogCtrl_ProcessSelectionAsBinBlock(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_FProcessBinBlock pfnProcess, 
	void* pContext
	);

void 
AXL_API
axl_log_TLogCtrl_SetHilite(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd
	);

bool_t
AXL_API
axl_log_TLogCtrl_HiliteBinRange(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pAnchorLine,
	size_t Offset,
	size_t Length
	);

bool_t
AXL_API
axl_log_TLogCtrl_SelectBinRange(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pAnchorLine,
	size_t Offset,
	size_t Length
	);

void 
AXL_API
axl_log_TLogCtrl_KillHilite(axl_log_TLogCtrl* pLogCtrl);

void 
AXL_API
axl_log_TLogCtrl_SetSelStart(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	);

void 
AXL_API
axl_log_TLogCtrl_SetSelEnd(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	);

#define axl_log_TLogCtrl_SetSelection(pLogCtrl, pStartPos, pEndPos) \
( \
	axl_log_TLogCtrl_SetSelStart(pLogCtrl, pStartPos), \
	axl_log_TLogCtrl_SetSelEnd(pLogCtrl, pEndPos) \
)

bool_t 
AXL_API
axl_log_TLogCtrl_GetSelection(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pSelStart, 
	axl_log_TCursorPos* pSelEnd
	);

void 
AXL_API
axl_log_TLogCtrl_KillSelection(axl_log_TLogCtrl* pLogCtrl);

void 
AXL_API
axl_log_TLogCtrl_EnsureVisible(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos
	);

void 
AXL_API
axl_log_TLogCtrl_EnsureVisibleRange(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPosStart,
	axl_log_TCursorPos* pPosEnd
	);

bool_t
AXL_API
axl_log_TLogCtrl_GetHyperlinkFromMousePos(
	axl_log_TLogCtrl* pLogCtrl,
	long x,
	long y,
	axl_log_TLine** ppLine,
	const tchar_t** ppHyperlink
	);

void
AXL_API
axl_log_TLogCtrl_GetCursorPosFromMousePos(
	axl_log_TLogCtrl* pLogCtrl,
	long x, 
	long y,
	axl_log_TCursorPos* pPos,
	bool_t AdjustX
	);

void
AXL_API
axl_log_TLogCtrl_GetMousePosFromCursorPos(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TCursorPos* pPos,
	POINT* pPoint
	);

#define axl_log_TLogCtrl_GetLineAtCursor(pLogCtrl) \
	axl_log_TCacheMgr_GetLine(&(pLogCtrl)->m_CacheMgr, (pLogCtrl)->m_CursorPos.m_Line)

#define axl_log_TLogCtrl_ScrollToBottom(pLogCtrl) \
	axl_log_TLogCtrl_OnScroll(pLogCtrl, SB_VERT, SB_BOTTOM)

#define axl_log_TLogCtrl_IsSelectionEmpty(pLogCtrl) \
	(axl_log_TCursorPos_Cmp(&(pLogCtrl)->m_SelStart, &(pLogCtrl)->m_SelEnd) == 0)

#ifdef __cplusplus

#define axl_log_TLogCtrl_SetFocus(pLogCtrl) \
	(::SetFocus((pLogCtrl)->m_hWnd))

#define axl_log_TLogCtrl_Invalidate(pLogCtrl) \
	(::InvalidateRect((pLogCtrl)->m_hWnd, NULL, FALSE))

#define axl_log_TLogCtrl_ShowCaret(pLogCtrl) \
	(::ShowCaret((pLogCtrl)->m_hWnd))

#define axl_log_TLogCtrl_HideCaret(pLogCtrl) \
	(::HideCaret((pLogCtrl)->m_hWnd))

#define axl_log_TLogCtrl_ClientToScreen(pLogCtrl, pPoint) \
	(::ClientToScreen((pLogCtrl)->m_hWnd, pPoint))

#else

#define axl_log_TLogCtrl_SetFocus(pLogCtrl) \
	SetFocus((pLogCtrl)->m_hWnd)

#define axl_log_TLogCtrl_Invalidate(pLogCtrl) \
	InvalidateRect((pLogCtrl)->m_hWnd, NULL, FALSE)

#define axl_log_TLogCtrl_ShowCaret(pLogCtrl) \
	ShowCaret((pLogCtrl)->m_hWnd)

#define axl_log_TLogCtrl_HideCaret(pLogCtrl) \
	HideCaret((pLogCtrl)->m_hWnd)

#define axl_log_TLogCtrl_ClientToScreen(pLogCtrl, pPoint) \
	ClientToScreen((pLogCtrl)->m_hWnd, pPoint)

#endif

void
AXL_API
axl_log_TLogCtrl_InvalidateLineRange(
	axl_log_TLogCtrl* pLogCtrl,
	ulong_t FirstLine,
	ulong_t LastLine
	);

#define axl_log_TLogCtrl_InvalidateLine(pLogCtrl, Line) \
	axl_log_TLogCtrl_InvalidateLineRange(pLogCtrl, Line, Line)

#define axl_log_TLogCtrl_GetLineCount(pLogCtrl) \
	((pLogCtrl)->m_LineCount)

size_t
AXL_API
axl_log_TLogCtrl_FormatTime(
	axl_rtl_TString* pString,
	SYSTEMTIME* pSysTime,
	const tchar_t* pFormat
	);

#define axl_log_TLogCtrl_SetFindPattern(pLogCtrl, p, Size, DoMatchCase) \
	axl_rtl_TBmhFind_SetPattern(&(pLogCtrl)->m_Find, p, Size, DoMatchCase)

#define axl_log_TLogCtrl_CanFindNextPrev(pLogCtrl) \
	axl_rtl_TBmhFind_CanFind(&(pLogCtrl)->m_Find)

bool_t
AXL_API
axl_log_TLogCtrl_FindNext(axl_log_TLogCtrl* pLogCtrl);

bool_t
AXL_API
axl_log_TLogCtrl_FindPrev(axl_log_TLogCtrl* pLogCtrl);

bool_t
AXL_API
axl_log_TLogCtrl_Copy(axl_log_TLogCtrl* pLogCtrl);

size_t 
AXL_API
axl_log_TLogCtrl_CopyString(
	axl_log_TLogCtrl* pLogCtrl,
	axl_rtl_TString* pString
	);

bool_t
AXL_API
axl_log_TLogCtrl_SaveAsTextFile(
	axl_log_TLogCtrl* pLogCtrl,
	const tchar_t* pFileName,
	axl_rtl_TBinTree* pIconToCharMap
	);

void 
AXL_API
axl_log_TLogCtrl_OnScroll(
	axl_log_TLogCtrl* pLogCtrl, 
	int Bar,
	int Code
	);

void
AXL_API
axl_log_TLogCtrl_OnKeyDown(
	axl_log_TLogCtrl* pLogCtrl,
	int VkCode
	);

void 
AXL_API
axl_log_TLogCtrl_PaintRect(
	axl_log_TLogCtrl* pLogCtrl,
	HDC hdc,
	const RECT* pRect
	);

size_t
AXL_API
axl_log_TLogCtrl_GetLineString(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	axl_rtl_TString* pString,
	axl_rtl_TBinTree* pIconToCharMap
	);

struct _axl_log_TLogCtrl 
{
	axl_sys_TWindow;

	HWND m_hWndToolTip;

	axl_gr_TTextPaint m_TextPaint;
	axl_gr_TStockCtrlPaint m_StockCtrlPaint;
	axl_gr_TOffscreenBuffer m_OffscreenBuffer;

//	HRGN m_hUpdateRgn;

	axl_log_TMessageFile* m_pMessageFile;

	axl_log_TIndexFile m_IndexFile;
	axl_log_TIndexMgr m_IndexMgr;
	axl_log_TCacheMgr m_CacheMgr;
	axl_log_TColorizeMgr m_ColorizeMgr;

	axl_log_TRepresentor* m_pRepresentor;
	axl_log_THyperlinkHandler* m_pHyperlinkHandler;
	axl_log_TFilter* m_pFilter;
	axl_log_TColorizer* m_pColorizer;

	bool_t m_IsTimestampVisible;
	axl_rtl_TString m_TimestampFormat;
	axl_rtl_TString m_TimestampString;
	ulong_t m_TimestampWidth;

	ulong_t m_OffsetWidth;
	ulong_t m_FullOffsetWidth;

	axl_gr_TFont m_DefaultFont;
	SIZE m_CharSize;

	bool_t m_IsIconVisible;
	HIMAGELIST m_hImageList;
	POINT m_IconOrigin;
	ulong_t m_IconWidth;

	COLORREF m_Palette [axl_log_EColor_Count];

	HCURSOR m_hArrowCursor;
	HCURSOR m_hBeamCursor;
	HCURSOR m_hHyperlinkCursor;

	ulong_t m_MouseWheelSpeed;

	ulong_t m_LineCount;
	ulong_t m_FirstVisibleLine;
	ulong_t m_VisibleLineCount;

	ulong_t m_BaseCol;
	ulong_t m_ColCount;
	ulong_t m_FirstVisibleCol;
	ulong_t m_VisibleColCount;

	bool_t m_IsDragging;
	bool_t m_IsSelecting;

	axl_log_TCursorPos m_CursorPos;
	axl_log_TCursorPos m_HiliteStart;
	axl_log_TCursorPos m_HiliteEnd;
	axl_log_TCursorPos m_SelStart;
	axl_log_TCursorPos m_SelEnd;
	axl_log_TCursorPos m_LastToolTipPos;

	axl_rtl_TByteArray m_BinBlockBuffer;	
	axl_rtl_TBmhFind m_Find; // Boyer-Moore-Horspool find
};

//.............................................................................
