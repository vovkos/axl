#include "axl_Precomp.h"
#include "axl_log_LogCtrl.h"
#include "axl_rtl_Bin2Str.h" 

//.............................................................................

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

size_t 
AXL_API
axl_log_TLogCtrl_GetTimestampString(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	axl_rtl_TString* pString
	)
{
	ulonglong_t Timestamp = pLine->m_IsFirstLineOfMessage || pLine->m_FirstTimestamp != pLine->m_LastTimestamp ? 
		pLine->m_LastTimestamp : 0;

	FILETIME LocalTimestamp;
	SYSTEMTIME SysTime = {0};

	if (!Timestamp)
	{
		axl_rtl_TString_Clear(pString);
		return 0;
	}

	FileTimeToLocalFileTime((FILETIME*) &Timestamp, &LocalTimestamp);
	FileTimeToSystemTime(&LocalTimestamp, &SysTime);

	ASSERT(!axl_rtl_TString_IsEmpty(&pLogCtrl->m_TimestampFormat));
	axl_log_TLogCtrl_FormatTime(pString, &SysTime, pLogCtrl->m_TimestampFormat.m_p);
	return axl_rtl_TString_GetLength(pString);
}

long
AXL_API
axl_log_TLogCtrl_PaintTimestamp(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine
	)
{
	axl_gr_TTextPaint* pPaint = &pLogCtrl->m_TextPaint;
	size_t Length;

	axl_log_TLogCtrl_GetTimestampString(pLogCtrl, pLine, &pLogCtrl->m_TimestampString);
	Length = axl_rtl_TString_GetLength(&pLogCtrl->m_TimestampString);

	axl_gr_TTextPaint_SetTextColor(pPaint, axl_log_EColor_Timestamp);
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

tchar_t* 
AXL_API
axl_log_TLogCtrl_FormatOffset(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine,
	tchar_t OffsetBuffer [16]
	)
{
	tchar_t* pOffset = OffsetBuffer;
	ASSERT(pLogCtrl->m_OffsetWidth);

	if (pLogCtrl->m_OffsetWidth < 8)
		pOffset += 8 - pLogCtrl->m_OffsetWidth;
	
	_stprintf(OffsetBuffer, _T("%08x"), pLine->m_Offset);
	return pOffset;
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
	tchar_t* pOffset = axl_log_TLogCtrl_FormatOffset(pLogCtrl, pLine, OffsetBuffer);

	axl_gr_TTextPaint_SetTextColor(pPaint, axl_log_EColor_Offset);
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
			pAttrArray
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
		pAttrArray, 
		pSelAttr, SelStart, SelEnd
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
	size_t GapLength = (pLine->m_DataConfig.m_BinHexLineSize - Size) * 3 + axl_log_EDef_HexGapSize;

	if (SelStart >= SelEnd)
	{
		axl_gr_TTextPaint_PaintHyperBinHex(
			pPaint, 
			pLine->m_BinData.m_p, Size, 
			&pLine->m_AttrArray
			);

		axl_gr_TTextPaint_SetStdBackColor(pPaint);
		axl_gr_TTextPaint_PaintSpace(pPaint, GapLength);

		axl_gr_TTextPaint_PaintHyperBinAscii(
			pPaint, 
			pLine->m_BinData.m_p, Size, 
			&pLine->m_AttrArray
			);
	}
	else
	{
		axl_gr_TTextPaint_PaintSelHyperBinHex(
			pPaint, 
			pLine->m_BinData.m_p, Size, 
			&pLine->m_AttrArray,
			pSelAttr, SelStart, SelEnd
			);

		axl_gr_TTextPaint_SetStdBackColor(pPaint);
		axl_gr_TTextPaint_PaintSpace(pPaint, GapLength);

		axl_gr_TTextPaint_PaintSelHyperBinAscii(
			pPaint, 
			pLine->m_BinData.m_p, Size, 
			&pLine->m_AttrArray,
			pSelAttr, SelStart, SelEnd			
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

	int SelCmp = axl_log_TCursorPos_Cmp(&pLogCtrl->m_SelStart, &pLogCtrl->m_SelEnd);
	int HiliteCmp = axl_log_TCursorPos_Cmp(&pLogCtrl->m_HiliteStart, &pLogCtrl->m_HiliteEnd);

	axl_log_TCursorPos* pSelStart = NULL;
	axl_log_TCursorPos* pSelEnd = NULL;

	axl_gr_TTextAttr SelAttr = g_axl_gr_TTextAttr_NullAttr;

	if (SelCmp > 0)
	{
		pSelStart = &pLogCtrl->m_SelEnd;
		pSelEnd = &pLogCtrl->m_SelStart;
		SelAttr.m_TextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
		SelAttr.m_BackColor = GetSysColor(COLOR_HIGHLIGHT);
	}
	else if (SelCmp < 0)
	{
		pSelStart = &pLogCtrl->m_SelStart;
		pSelEnd = &pLogCtrl->m_SelEnd;
		SelAttr.m_TextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
		SelAttr.m_BackColor = GetSysColor(COLOR_HIGHLIGHT);
	}
	else if (HiliteCmp != 0)
	{
		pSelStart = &pLogCtrl->m_HiliteStart; 
		pSelEnd = &pLogCtrl->m_HiliteEnd;
		SelAttr.m_BackColor = axl_log_EColor_HiliteBack;
	}

	if (axl_log_TLine_IsBin(pLine))
		axl_log_TColorizeMgr_EnsureColorized(&pLogCtrl->m_ColorizeMgr, (axl_log_TBinLine*) pLine);

	// reset 

	pPaint->m_StdAttr.m_TextColor = axl_log_EColor_Text;
	pPaint->m_StdAttr.m_BackColor = axl_log_EColor_Back;
	pPaint->m_StdAttr.m_FontFlags = 0;

	axl_gr_TTextAttr_Overlay(&pPaint->m_StdAttr, &pLine->m_Attr);

#if 0 // define to see repainted areas
	COLORREF Rgb = RGB(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
	axl_gr_TColor_SetRgb(&pPaint->m_StdBackColor, Rgb);
#endif

	axl_gr_TTextPaint_SetStdAttr(pPaint);

	pPaint->m_x = -(long) pLogCtrl->m_FirstVisibleCol * pLogCtrl->m_CharSize.cx;
	pPaint->m_y = pLineRect->top;
	pPaint->m_top = pLineRect->top;
	pPaint->m_bottom = pLineRect->bottom;

	if (pLogCtrl->m_IsTimestampVisible && pLogCtrl->m_TimestampWidth)
		axl_log_TLogCtrl_PaintTimestamp(pLogCtrl, pLine);

	if (pLogCtrl->m_IsIconVisible && pLogCtrl->m_IconWidth)
		axl_log_TLogCtrl_PaintIcon(pLogCtrl, pLine);

	if (axl_log_TLine_IsBin(pLine) && pLogCtrl->m_OffsetWidth)
		axl_log_TLogCtrl_PaintOffset(pLogCtrl, (axl_log_TBinLine*) pLine);

	axl_gr_TTextPaint_SetStdTextColor(pPaint);

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		{
		axl_log_TTextLine* pTextLine = (axl_log_TTextLine*) pLine;
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
			&pBinTextLine->m_AttrArray,
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

	pNextLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
	Delimiter = axl_log_TLine_GetDelimiter(pLine, pNextLine);

	if (Delimiter && Delimiter < 3)
	{
		axl_gr_TTextPaint_SetBackColor(&pLogCtrl->m_TextPaint, axl_log_EColor_DelimiterLight + Delimiter - 1);
		axl_gr_FastHLine(
			pPaint->m_hdc, 
			pLineRect->left, 
			pLineRect->bottom - 1,
			pLineRect->right
			);
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

		axl_log_TLine* pLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, FirstLine);
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

			pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
		}
	}

	if (pRect->bottom > LineRect.top)
	{
		COLORREF Color = pLogCtrl->m_Palette [axl_gr_GetColorIndex(axl_log_EColor_Back)];
		LineRect.bottom = pRect->bottom;
		axl_gr_FastRectangleEx(hdc, &LineRect, Color);
	}

	axl_gr_TTextPaint_Detach(&pLogCtrl->m_TextPaint);
}

size_t
AXL_API
axl_log_TLogCtrl_GetLineString(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	axl_rtl_TString* pString,
	axl_rtl_TBinTree* pIconToCharMap
	)
{
	axl_rtl_TString_Clear(pString);

	if (pLogCtrl->m_IsTimestampVisible && pLogCtrl->m_TimestampWidth)
	{
		size_t Length = axl_log_TLogCtrl_GetTimestampString(pLogCtrl, pLine, pString);
		if (Length < pLogCtrl->m_TimestampWidth)
			axl_rtl_TString_AppendChar(pString, _T(' '), pLogCtrl->m_TimestampWidth - Length);
	}

	if (pLogCtrl->m_IconWidth) 
	{
		tchar_t IconChar = _T(' ');

		if (pIconToCharMap && pLine->m_Icon != -1)
		{
			axl_rtl_TBinTreeNode* p = axl_rtl_TBinTree_Find(pIconToCharMap, pLine->m_Icon);
			if (p)
				IconChar = (tchar_t) p->m_Value;
		}

		axl_rtl_TString_AppendChar(pString, IconChar, 1);
		axl_rtl_TString_AppendChar(pString, _T(' '), axl_log_EDef_IconGapSize);
	}

	if (axl_log_TLine_IsBin(pLine) && pLogCtrl->m_OffsetWidth)
	{
		tchar_t OffsetBuffer [16] = {0};
		tchar_t* pOffset = axl_log_TLogCtrl_FormatOffset(pLogCtrl, (axl_log_TBinLine*) pLine, OffsetBuffer);
		axl_rtl_TString_AppendString(pString, pOffset, -1);
		axl_rtl_TString_AppendChar(pString, _T(' '), axl_log_EDef_OffsetGapSize);
	}

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		{
		axl_log_TTextLine* pTextLine = (axl_log_TTextLine*) pLine;
		size_t Length = axl_rtl_TString_GetLength(&pTextLine->m_HyperText.m_Text);
		axl_rtl_TString_AppendString(pString, pTextLine->m_HyperText.m_Text.m_p, -1);
		}
		break;

	case axl_log_ELine_BinHex:
		{
		axl_log_TBinLine* pBinLine = (axl_log_TBinLine*) pLine;
		size_t Size = axl_rtl_TByteArray_GetCount(&pBinLine->m_BinData);

		axl_rtl_AppendEncodeHexString(pString, pBinLine->m_BinData.m_p, Size, 0);

		if (Size < pBinLine->m_DataConfig.m_BinHexLineSize)
			axl_rtl_TString_AppendChar(pString, _T(' '), (pBinLine->m_DataConfig.m_BinHexLineSize - Size) * 3);

		axl_rtl_TString_AppendChar(pString, _T(' '), 1 + axl_log_EDef_HexGapSize);
		axl_rtl_AppendEncodeAsciiString(pString, pBinLine->m_BinData.m_p, Size, '.');
		}
		break;

	case axl_log_ELine_BinText:
		{
		axl_log_TBinTextLine* pBinLine = (axl_log_TBinTextLine*) pLine;
		size_t Size = axl_rtl_TByteArray_GetCount(&pBinLine->m_BinData);
		axl_rtl_AppendEncodeAsciiString(pString, pBinLine->m_BinData.m_p, Size, '.');
		}
	}

	return axl_rtl_TString_GetLength(pString);
}

//.............................................................................
