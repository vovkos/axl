#include "pch.h"
#include "HexEditCtrlEx.h"
#include "GuiUtils.h"

//.............................................................................

// inline static void FastRectangle(HDC hdc, const RECT& rc)
//	{ ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL); }

inline static void FastChar(HDC hdc, const RECT& rc, TCHAR ch)
	{ ExtTextOut(hdc, rc.left, rc.top, ETO_OPAQUE, &rc, &ch, 1, NULL); }

static HFONT CreatePointFont(int nPointSize, LPCTSTR pszFaceName, BYTE CharSet = DEFAULT_CHARSET)
{
	LOGFONT lf = {0};
	_tcsncpy(lf.lfFaceName, pszFaceName, sizeof(lf.lfFaceName) / sizeof(TCHAR));
	lf.lfCharSet = CharSet;

	HDC hdc = GetDC(NULL);
	
	POINT pt = {0};
	pt.y = GetDeviceCaps(hdc, LOGPIXELSY) * nPointSize;
	pt.y /= 720; // 72 points/inch, 10 decipoints/point
	DPtoLP(hdc, &pt, 1);
	
	POINT ptOrg = {0};
	DPtoLP(hdc, &ptOrg, 1);
	lf.lfHeight = -abs(pt.y - ptOrg.y);

	ReleaseDC(NULL, hdc);

	return CreateFontIndirect(&lf);
}

static void CalcCharSize(SIZE* pSize, HFONT hFont, TCHAR Char)
{
	HDC hdc = GetDC(NULL);
	HFONT hOldFont = (HFONT) SelectObject(hdc, hFont);
	GetTextExtentPoint32(hdc, &Char, 1, pSize);
	SelectObject(hdc, hOldFont);
	ReleaseDC(NULL, hdc);
}

//.............................................................................

CHexEditCtrl::CHexEditCtrl() 
{
	m_hUpdateRgn = CreateRectRgn(0, 0, 0, 0);
	m_hDefaultFont = CreatePointFont(100, _T("Courier New"));

	m_hFont = m_hDefaultFont;

	CalcCharSize(&m_CharSize, m_hFont, '|');

	m_nVisibleLineCount = 0;
	m_nVisibleColCount = 0;

	m_nLineCount = 0;
	m_nFirstVisibleLine = 0;
	m_nFirstVisibleCol = 0;

	m_bAutoAdjustBytesPerLine = TRUE;
	m_nBytesPerLine = 0;
	m_bUpperCase = TRUE;

	m_nMouseWheelSpeed = 2;

	m_OffsetWidth = 0;
	
	m_nHexGapStep = 0;
	m_nHexCol = 0;
	m_nGapCol = 0;
	m_nAsciiCol = 0;
	m_nSpaceCol = 0;

	m_bOnlyPrintableChars = TRUE;
	m_bReadOnly = FALSE;
	m_bModified = FALSE;
	m_bOverwriteMode = FALSE;
	m_bDragging = FALSE;

	m_StdColorBg.m_Flags = CF_FORE_COLOR | CF_BACK_COLOR;
	m_StdColorBg.m_rgbForeColor = GetSysColor(COLOR_WINDOWTEXT);
	m_StdColorBg.m_rgbBackColor = GetSysColor(COLOR_WINDOW);

	m_StdColorAscii.m_Flags = CF_FORE_COLOR | CF_BACK_COLOR;
	m_StdColorAscii.m_rgbForeColor = GetSysColor(COLOR_WINDOWTEXT);
	m_StdColorAscii.m_rgbBackColor = RGB (
		GetRValue(m_StdColorBg.m_rgbBackColor) / 1.15,
		GetGValue(m_StdColorBg.m_rgbBackColor) / 1.1,
		GetBValue(m_StdColorBg.m_rgbBackColor)
		);

	m_pStdColor = NULL;
	
	m_SelectionColor.m_Flags = CF_FORE_COLOR | CF_BACK_COLOR;
	m_SelectionColor.m_rgbForeColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_SelectionColor.m_rgbBackColor = GetSysColor(COLOR_HIGHLIGHT);
	m_CursorColor.m_Flags = CF_BACK_COLOR;

	m_CursorColor.m_rgbBackColor = RGB(
		GetRValue(m_StdColorBg.m_rgbBackColor) / 1.3,
		GetGValue(m_StdColorBg.m_rgbBackColor) / 1.2,
		GetBValue(m_StdColorBg.m_rgbBackColor)
		);

	m_ModifiedColor.m_Flags = CF_FORE_COLOR;
	m_ModifiedColor.m_rgbForeColor = RGB(255, 0, 0);

	m_bSelecting = FALSE;

	m_pData = NULL;
	m_pDataEnd = NULL;
	m_nDataSize = 0;
	m_nAllocSize = 0;

	m_bIgnoreTab = FALSE;
	m_bIgnoreBackspace = FALSE;

	m_bFindMatchCase = FALSE;
}

CHexEditCtrl::~CHexEditCtrl()
{
	DeleteObject(m_hDefaultFont);
	DeleteObject(m_hUpdateRgn);

	if (m_pData)
		free(m_pData);
}

ATL::CWndClassInfo& CHexEditCtrl::GetWndClassInfo()
{
	static ATL::CWndClassInfo wc =
	{
		{ 
			sizeof(WNDCLASSEX), 
			CS_DBLCLKS, 
			StartWindowProc,
			0, 0, NULL, NULL, NULL, 
			NULL, 
			NULL, 
			_T("tibbo.hexeditctrl"), 
			NULL 
		},

		NULL, 
		NULL, 
		IDC_IBEAM, 
		TRUE, 
		0, 
		_T("")
	};
	return wc;
}

void CHexEditCtrl::ShowOnlyPrintableChars(BOOL bEnable)
{
	if (m_bOnlyPrintableChars == bEnable)
		return;

	m_bOnlyPrintableChars = bEnable;
	Invalidate();
}

void CHexEditCtrl::SetOffsetWidth(BYTE Width)
{
	if (Width > 8)
		Width = 8;

	if (m_OffsetWidth == Width)
		return;

	m_OffsetWidth = Width;

	if (m_bAutoAdjustBytesPerLine)
		AutoAdjustBytesPerLine();

	RecalcColumns();
	RecalcLayout();
	Invalidate();
}

void CHexEditCtrl::SetReadOnly(BOOL bReadOnly)
{
	if (m_bReadOnly == bReadOnly)
		return;

	m_bReadOnly = bReadOnly;
	Invalidate();
}

void CHexEditCtrl::SetModified()
{
	m_bModified = TRUE;
	NotifyParent(HEN_DATA_CHANGE);
}

LRESULT CHexEditCtrl::NotifyParent(UINT nCode, HEN_PARAMS* pParams)
{
	HEN_PARAMS Hdr;

	if (!pParams)
		pParams = &Hdr;

	pParams->hwndFrom = m_hWnd;
    pParams->idFrom = GetDlgCtrlID();
    pParams->code = nCode;

	HWND hwndParent = GetParent();
	ASSERT(::IsWindow(hwndParent));
	
	return SendMessage(hwndParent, WM_NOTIFY, (WPARAM) pParams->idFrom, (LPARAM) pParams);
}

SIZE_T CHexEditCtrl::GetData(PVOID pvBuffer, SIZE_T nBufferSize, SIZE_T nOffset)
{
	if (nOffset >= m_nDataSize)
		return 0;

	SIZE_T nCopySize = m_nDataSize - nOffset;
	if (nCopySize > nBufferSize)
		nCopySize = nBufferSize;

	DATA* pData = m_pData + nOffset;
	DATA* pEnd = pData + nCopySize;
	BYTE* pBuffer = (BYTE*) pvBuffer;
	for (; pData < pEnd; pData++, pBuffer++)
		*pBuffer = pData->m_Data;

	return nCopySize;
}

SIZE_T CHexEditCtrl::GetSelectedData(PVOID pvBuffer, SIZE_T nBufferSize)
{
	SIZE_T nSelectedDataSize = GetSelectedDataSize();

	if (!nSelectedDataSize || nBufferSize < nSelectedDataSize)
		return 0;

	DATA* pData = m_pData + m_Selection.m_nStartOffset;
	DATA* pEnd = m_pData + m_Selection.m_nEndOffset;
	BYTE* pBuffer = (BYTE*) pvBuffer;
	for (; pData < pEnd; pData++, pBuffer++)
		*pBuffer = pData->m_Data;

	return nSelectedDataSize;
}

BOOL CHexEditCtrl::SetData(PVOID pvBuffer, SIZE_T nBufferSize, SIZE_T nOffset)
{
	if (nOffset > m_nDataSize)
		return FALSE;

	SIZE_T nDelete = m_nDataSize - nOffset;
	if (nDelete > nBufferSize)
		nDelete = nBufferSize;

	if (!InsertDeleteData(nOffset, nBufferSize, nDelete))
		return FALSE;

	DATA* pData = m_pData + nOffset;
	DATA* pEnd = pData + nBufferSize;
	BYTE* pBuffer = (BYTE*) pvBuffer;
	
	for (; pData < pEnd; pData++, pBuffer++)
		pData->m_Data = *pBuffer;

	// invalidate has already been called (withing InsertDeleteData)
	return TRUE;
}

BOOL CHexEditCtrl::SetDataSize(SIZE_T nDataSize)
{
	if (nDataSize < m_nDataSize)
		return InsertDeleteData(nDataSize, 0, m_nDataSize - nDataSize);
	else
		return InsertDeleteData(m_nDataSize, nDataSize - m_nDataSize, 0);
}

BOOL CHexEditCtrl::SetFont(HFONT hFont)
{
	HDC hdc = GetDC();
	HFONT hFontOld = (HFONT) SelectObject(hdc, hFont);
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	SelectObject(hdc, hFontOld);
	ReleaseDC(hdc);

	if (!(tm.tmPitchAndFamily & TMPF_FIXED_PITCH)) // how stupid the name is huh? :))
		return FALSE;

	m_hFont = hFont;
	Invalidate();
	return TRUE;
}

BOOL CHexEditCtrl::SetUpperCase(BOOL bUpperCase)
{
	m_bUpperCase = bUpperCase;
	Invalidate();
	return TRUE;
}

BOOL CHexEditCtrl::SetBytesPerLine(int nBytesPerLine)
{
	if (!nBytesPerLine)
	{
		m_bAutoAdjustBytesPerLine = TRUE;
		AutoAdjustBytesPerLine();
		return TRUE;
	}

	if (nBytesPerLine != 2 && 
		nBytesPerLine != 4 && 
		nBytesPerLine != 8 && 
		nBytesPerLine != 16 && 
		nBytesPerLine != 32)
	{
		return FALSE;
	}

	m_bAutoAdjustBytesPerLine = FALSE;
	SetBytesPerLineImpl(nBytesPerLine);
	return TRUE;
}

void CHexEditCtrl::RecalcColumns()
{
	m_nHexCol = m_OffsetWidth ? m_OffsetWidth + 2 : 0;
	m_nGapCol = m_nHexCol + m_nBytesPerLine * 3;
	m_nAsciiCol = m_nGapCol + 1;
	m_nSpaceCol = m_nAsciiCol + m_nBytesPerLine;
}

void CHexEditCtrl::SetBytesPerLineImpl(int nBytesPerLine)
{
	if (m_nBytesPerLine == nBytesPerLine)
		return;

	m_nBytesPerLine = nBytesPerLine;

	m_nLineCount = (int) m_nDataSize / m_nBytesPerLine;
	if (m_nDataSize % m_nBytesPerLine)
		m_nLineCount++;

	RecalcColumns();
	Invalidate();
	UpdateCaretPos();
	RecalcLayout();
}

CHexEditCtrl::CURSOR_POS CHexEditCtrl::SetCursorPos(CURSOR_POS Pos, int Flags)
{
	EnsureVisible(Pos);

	if (Pos.m_Location >= CURSOR_LOCATION_COUNT)
        Pos.m_Location = CURSOR_LOCATION_COUNT - 1;

	if (m_bOverwriteMode && Pos.m_nOffset != (int) m_nDataSize - 1) // in overwrite mode certain locations are unavailable
	{
		if (Pos.m_Location == CURSOR_ASCII_1)
			Pos.m_Location = CURSOR_ASCII_0;
		else if (Pos.m_Location == CURSOR_HEX_2)
			Pos.m_Location = CURSOR_HEX_1;
	}

	if (Pos.m_nOffset >= (int) m_nDataSize)
	{
		Pos.m_nOffset = (int) m_nDataSize - 1;
		if (Pos.m_Location >= CURSOR_ASCII_0)
			Pos.m_Location = CURSOR_ASCII_1;
		else 
			Pos.m_Location = CURSOR_HEX_2;
	}

	if (Pos.m_nOffset < 0)
	{
		Pos.m_nOffset = 0;
		if (Pos.m_Location >= CURSOR_ASCII_0)
			Pos.m_Location = CURSOR_ASCII_0;
		else 
			Pos.m_Location = CURSOR_HEX_0;
	}

	if ((Flags & MCF_FORCE) == 0 &&
		m_CursorPos.m_nOffset == Pos.m_nOffset && 
		m_CursorPos.m_Location == Pos.m_Location)
		return Pos;

	if (m_CursorPos.m_nOffset != Pos.m_nOffset || m_CursorPos.IsHex() && Pos.IsAscii())
		Finalize4Bits();

	if (m_CursorPos.m_nOffset != Pos.m_nOffset)
	{
		InvalidateOffset(m_CursorPos.m_nOffset);
		InvalidateOffset(Pos.m_nOffset);
	}

	if (Flags & MCF_SELECT)
	{
		if (!m_bSelecting)
		{
			m_bSelecting = TRUE;
			m_SelStart = m_CursorPos;		
		}
	}
	else if (!(Flags & MCF_KEEP_SELECTION))
	{
		KillSelection();
	}

	m_CursorPos = Pos;
	UpdateCaretPos();

	if (Flags & MCF_SELECT)
		SetSelEnd(Pos);

	return Pos;
}

BOOL CHexEditCtrl::IsEqualCursorPos(CURSOR_POS& Pos1, CURSOR_POS& Pos2)
{
	static BYTE _EqualityTable1[CURSOR_LOCATION_COUNT][CURSOR_LOCATION_COUNT] = 
	{
		{ 1, 0, 0,  1, 0, }, // CURSOR_HEX_0
		{ 0, 1, 0,  0, 1, }, // CURSOR_HEX_1
		{ 0, 0, 1,  0, 1, }, // CURSOR_HEX_2
		
		{ 1, 0, 0,  1, 0, }, // CURSOR_ASCII_0
		{ 0, 1, 1,  0, 1, }  // CURSOR_ASCII_1
	};

	static BYTE _EqualityTable2[CURSOR_LOCATION_COUNT][CURSOR_LOCATION_COUNT] = 
	{
		{ 0, 0, 0,  0, 0, }, // CURSOR_HEX_0
		{ 0, 0, 0,  0, 0, }, // CURSOR_HEX_1
		{ 1, 0, 0,  1, 0, }, // CURSOR_HEX_2
		
		{ 0, 0, 0,  0, 0, }, // CURSOR_ASCII_0
		{ 1, 0, 0,  1, 0, }  // CURSOR_ASCII_1
	};

	return
		Pos1.m_nOffset == Pos2.m_nOffset && 
		_EqualityTable1[Pos1.m_Location][Pos2.m_Location] ||

		Pos1.m_nOffset + 1 == Pos2.m_nOffset && 
		_EqualityTable2[Pos1.m_Location][Pos2.m_Location] ||

		Pos2.m_nOffset + 1 == Pos1.m_nOffset && 
		_EqualityTable2[Pos2.m_Location][Pos1.m_Location];
}

void CHexEditCtrl::SetSelEnd(CURSOR_POS Pos)
{
	SELECTION_INFO OldSelection = m_Selection;

	m_SelEnd = Pos;

	if (IsEqualCursorPos(Pos, m_SelStart)) // "no selection" shortcut
	{
		m_Selection.m_nStartOffset = 0;
		m_Selection.m_nEndOffset = 0;
		m_Selection.m_bLeadingGap = FALSE;
		m_Selection.m_bTrailingGap = FALSE;
		InvalidateSelection(&OldSelection);
		return;
	}

	if (m_SelStart.m_nOffset == m_SelEnd.m_nOffset) // "single char selection" shortcut
	{
		m_Selection.m_nStartOffset = m_SelStart.m_nOffset;
		m_Selection.m_nEndOffset = m_SelEnd.m_nOffset + 1;
		m_Selection.m_bLeadingGap = FALSE;
		m_Selection.m_bTrailingGap = FALSE;
		InvalidateSelectionDiff(&m_Selection, &OldSelection);
		return;
	}

	// ok, here's the tricky part -- find out ACTUAL selection range

	// first, arrange SelStart and SelEnd according to offsets

	CURSOR_POS SelStart;
	CURSOR_POS SelEnd;

	if (m_SelStart.m_nOffset < m_SelEnd.m_nOffset)
	{
		SelStart = m_SelStart;
		SelEnd = m_SelEnd;
	}
	else
	{
		SelStart = m_SelEnd;
		SelEnd = m_SelStart;
	}

	// now, do a lookup on selection rules

	enum SELECTION_FLAGS { SF_S = 1, SF_E = 2, SF_L = 4, SF_T = 8 };

	static BYTE _SelectionTable[CURSOR_LOCATION_COUNT][CURSOR_LOCATION_COUNT] = 
	{
		// CURSOR_HEX_0 CURSOR_HEX_1 CURSOR_HEX_2   CURSOR_ASCII_0 CURSOR_ASCII_1
		{ SF_S | SF_T,  SF_S | SF_E, SF_S | SF_E,   SF_S | SF_T,   SF_S | SF_E }, // CURSOR_HEX_0
		{ SF_S | SF_T,  SF_S | SF_E, SF_S | SF_E,   SF_S | SF_T,   SF_S | SF_E }, // CURSOR_HEX_1
		{ SF_L | SF_T,  SF_E | SF_L, SF_E | SF_L,   SF_L | SF_T,   SF_E | SF_L }, // CURSOR_HEX_2
		
		{ SF_S | SF_T,  SF_S | SF_E, SF_S | SF_E,   SF_S | SF_T,   SF_S | SF_E }, // CURSOR_ASCII_0
		{ SF_L | SF_T,  SF_E | SF_L, SF_E | SF_L,   SF_L | SF_T,   SF_E | SF_L }  // CURSOR_ASCII_1
	};

	BYTE SelectionFlags = _SelectionTable[SelStart.m_Location][SelEnd.m_Location];

	if (m_bOverwriteMode) 		
	{
		// in overwrite mode end position is always included and no trailing gap
		SelectionFlags |= SF_E;
		SelectionFlags &= ~SF_T;
	}

	if (SelectionFlags & SF_S)
		m_Selection.m_nStartOffset = SelStart.m_nOffset;
	else
		m_Selection.m_nStartOffset = SelStart.m_nOffset + 1;

	if (SelectionFlags & SF_E)
		m_Selection.m_nEndOffset = SelEnd.m_nOffset + 1;
	else
		m_Selection.m_nEndOffset = SelEnd.m_nOffset;

	m_Selection.m_bLeadingGap = (SelectionFlags & SF_L) != 0;
	m_Selection.m_bTrailingGap = (SelectionFlags & SF_T) != 0;

	InvalidateSelectionDiff(&m_Selection, &OldSelection);
}

BOOL CHexEditCtrl::IsDataSelected(SIZE_T nOffset)
{ 
	return 
		nOffset >= m_Selection.m_nStartOffset && 
		nOffset < m_Selection.m_nEndOffset; 
}

BOOL CHexEditCtrl::IsGapSelected(SIZE_T nOffset)
{
	return 
		nOffset == m_Selection.m_nStartOffset - 1 && m_Selection.m_bLeadingGap ||
		nOffset == m_Selection.m_nEndOffset - 1 && m_Selection.m_bTrailingGap;
}

void CHexEditCtrl::InvalidateSelectionDiff(SELECTION_INFO* pOld, SELECTION_INFO* pNew)
{
	InvalidateSelection(pOld);
	InvalidateSelection(pNew);
}

void CHexEditCtrl::InvalidateSelection(SELECTION_INFO* pSel)
{
	if (pSel->IsEmpty())
		return;

	SIZE_T nOffsetFrom = pSel->m_nStartOffset;
	SIZE_T nOffsetTo = pSel->m_nEndOffset;

	if (pSel->m_bLeadingGap && nOffsetFrom)
		nOffsetFrom--;

	InvalidateRange(nOffsetFrom, nOffsetTo);
}

void CHexEditCtrl::InvalidateRange(SIZE_T nOffsetFrom, SIZE_T nOffsetTo)
{
	int nLineFrom = (int) nOffsetFrom / m_nBytesPerLine;
	int nLineTo = (int) nOffsetTo / m_nBytesPerLine;

	if (nLineFrom == nLineTo)
	{
		int nFirstCol = (int) nOffsetFrom % m_nBytesPerLine * 3 + m_nHexCol;
		int nLastCol = (int) nOffsetTo % m_nBytesPerLine + m_nAsciiCol;
		
		InvalidateHelper(nLineFrom,  nFirstCol, 1, nLastCol - nFirstCol);
	}
	else
	{
		InvalidateHelper(
			nLineFrom, m_nHexCol, 
			nLineTo - nLineFrom + 1, m_nSpaceCol - m_nHexCol + 1);
	}
}

void CHexEditCtrl::InvalidateOffset(SIZE_T nOffset)
{
	int nLine = (int) nOffset / m_nBytesPerLine;
	int nCol = (int) nOffset % m_nBytesPerLine;
	
	InvalidateHelper(nLine, nCol * 3 + m_nHexCol, 1, 3);
	InvalidateHelper(nLine, nCol + m_nAsciiCol, 1, 1);
}

void CHexEditCtrl::InvalidateHelper(int nLine, int nCol, int nLineCount, int nColCount)
{
	nLine -= m_nFirstVisibleLine;
	nCol -= m_nFirstVisibleCol;

	RECT rct = 
	{ 
		nCol * m_CharSize.cx, 
		nLine * m_CharSize.cy,  
		(nCol + nColCount) * m_CharSize.cx, 
		(nLine + nLineCount) * m_CharSize.cy,  
	};

	InvalidateRect(&rct);
}

void CHexEditCtrl::EnsureVisible(CURSOR_POS Pos)
{
	int nLine, nCol;
	GetCursorLineCol(Pos, nLine, nCol, FALSE);

	BOOL bInvalidate = FALSE;

	if (nLine >= m_nFirstVisibleLine + m_nVisibleLineCount)
	{
		m_nFirstVisibleLine = nLine - m_nVisibleLineCount + 1;
		bInvalidate = TRUE;
	}

	if (nLine < m_nFirstVisibleLine)
	{
		m_nFirstVisibleLine = nLine;
		bInvalidate = TRUE;
	}

	int nColSpan = Pos.m_Location >= CURSOR_ASCII_0 ? 1 : 2;

	if (nCol + nColSpan >= m_nFirstVisibleCol + m_nVisibleColCount)
	{
		m_nFirstVisibleCol = nCol + nColSpan - m_nVisibleColCount + 1; 
		bInvalidate = TRUE;
	}

	if (nCol < m_nFirstVisibleCol)
	{
		m_nFirstVisibleCol = nCol;
		bInvalidate = TRUE;
	}

	FixupFirstVisibleLineCol();
	RecalcScrollBars();

	if (bInvalidate)
		Invalidate();
}

BOOL CHexEditCtrl::InsertDeleteData(SIZE_T nOffset, SIZE_T nInsert, SIZE_T nDelete)
{
	ASSERT(nOffset <= m_nDataSize);
	ASSERT(nOffset + nDelete <= m_nDataSize);

	SIZE_T nNewSize = m_nDataSize + nInsert - nDelete;
	SIZE_T nLeftOver = m_nDataSize - nOffset - nDelete;

	if (nNewSize <= m_nAllocSize)
	{
		if (nLeftOver)
			memmove(
				m_pData + nOffset + nInsert, 
				m_pData + nOffset + nDelete,
				nLeftOver * sizeof(DATA)
				);
	}
	else
	{
		// reserve at least one more page in buffer
		SIZE_T nNewAllocSize = nNewSize + 2 * BUFFER_PAGE_SIZE - nNewSize % BUFFER_PAGE_SIZE;
		DATA* pNewData = (DATA*) malloc(nNewAllocSize * sizeof(DATA));
		if (!pNewData)
			return FALSE;

		if (m_pData) 
		{
			// copy old data
			memcpy(pNewData, m_pData, nOffset * sizeof(DATA));
			
			if (nLeftOver)
				memcpy(
					pNewData + nOffset + nInsert, 
					m_pData + nOffset + nDelete, 
					nLeftOver * sizeof(DATA)
					);

			free(m_pData);
		}

        m_pData = pNewData;
		m_nAllocSize = nNewAllocSize;
	}

	if (nInsert) // zero out inserted memory
	{
		memset(m_pData + nOffset, 0, nInsert * sizeof(DATA));
		DATA* pData = m_pData + nOffset;
		DATA* pEnd = pData + nInsert;

		for (; pData < pEnd; pData++)
			pData->m_Flags |= DF_MODIFIED;
	}

	int nNewLineCount = (int) nNewSize / m_nBytesPerLine;
	if (nNewSize % m_nBytesPerLine)
		nNewLineCount++;

	int nInvalidateFromLine = (int) nOffset / m_nBytesPerLine;
	int nInvalidateToLine = max(m_nLineCount, nNewLineCount);

	m_pDataEnd = m_pData + nNewSize;
	m_nDataSize = nNewSize;
	m_nLineCount = nNewLineCount;

	RECT rct = 
	{ 
		0, 
		(nInvalidateFromLine - m_nFirstVisibleLine) * m_CharSize.cy,
		(m_nSpaceCol - m_nFirstVisibleCol) * m_CharSize.cx,
		(nInvalidateToLine - m_nFirstVisibleLine) * m_CharSize.cy
	};

	InvalidateRect(&rct);
	RecalcLayout();

	return TRUE;
}

void CHexEditCtrl::TransitCursor(int nOffsetDelta, int NewLocation, int Flags)
{
	CURSOR_POS NewPos;
	NewPos.m_nOffset = m_CursorPos.m_nOffset + nOffsetDelta;
	NewPos.m_Location = NewLocation != -1 ? 
		NewLocation : 
		m_CursorPos.m_Location;

	SetCursorPos(NewPos, Flags);
}

void CHexEditCtrl::KillSelection()
{
	SELECTION_INFO OldSelection = m_Selection;

	m_SelStart.m_nOffset = 0;
	m_SelStart.m_Location = 0;
	m_SelEnd.m_nOffset = 0;
	m_SelEnd.m_Location = 0;
	
	m_Selection.Clear();

	m_bSelecting = FALSE;
	
	InvalidateSelection(&OldSelection);
}

void CHexEditCtrl::Finalize4Bits()
{
	DATA* pData = m_pData + m_CursorPos.m_nOffset;
	if (pData < m_pDataEnd && pData->m_Flags & DF_4_BITS)
	{
		pData->m_Flags &= ~DF_4_BITS;
		InvalidateOffset(m_CursorPos.m_nOffset);
	}
}

BOOL CHexEditCtrl::SetOverwriteMode(BOOL bOverwriteMode)
{
	m_bOverwriteMode = bOverwriteMode;
	UpdateCaret();
	return TRUE;
}

CHexEditCtrl::CURSOR_POS CHexEditCtrl::CursorPosFromMousePos(POINT pt)
{
	int nLine = pt.y / m_CharSize.cy;
	int nCol = pt.x / m_CharSize.cx;

	nLine += m_nFirstVisibleLine;
	nCol += m_nFirstVisibleCol;

	if (nLine < 0)
		nLine = 0;

	CURSOR_POS Pos;

	if (nCol >= m_nGapCol)
	{
		nCol = max(nCol, m_nAsciiCol);
		nCol -= m_nAsciiCol;

		Pos.m_Location = CURSOR_ASCII_0;

		if (nCol >= m_nBytesPerLine)
		{
			nCol = m_nBytesPerLine - 1;
			Pos.m_Location = CURSOR_ASCII_1;
		}
	}
	else 
	{
		nCol = max(nCol, m_nHexCol);
		nCol -= m_nHexCol; 
		
		Pos.m_Location = CURSOR_HEX_0 + nCol % 3;

		nCol /= 3;

		if (nCol >= m_nBytesPerLine)
		{
			nCol = m_nBytesPerLine - 1;
			Pos.m_Location = CURSOR_HEX_2;
		}
	}

	Pos.m_nOffset = nLine * m_nBytesPerLine + nCol;
	return Pos;
}

void CHexEditCtrl::UpdateCaret()
{
	if (GetFocus() != m_hWnd)
		return;

	int cx = m_bOverwriteMode ? m_CharSize.cx : 2;

	if (CreateSolidCaret(cx, m_CharSize.cy))
	{
		UpdateCaretPos();
		ShowCaret();
	}
}

void CHexEditCtrl::GetCursorLineCol(CURSOR_POS Pos, int& nLine, int& nCol, BOOL bAddExtra)
{
	nLine = (int) Pos.m_nOffset / m_nBytesPerLine;
	nCol = (int) Pos.m_nOffset % m_nBytesPerLine;

	if (Pos.m_Location >= CURSOR_ASCII_0)
	{
		nCol += m_nAsciiCol;
	}
	else
	{
		nCol *= 3;
		nCol += m_nHexCol;
	}

	if (bAddExtra)
	{
		static int _ExtraCol[] = { 0, 1, 2, 0, 1, };
		nCol += _ExtraCol[Pos.m_Location];
	}
}

void CHexEditCtrl::UpdateCaretPos()
{
	int nLine, nCol;

	GetCursorLineCol(m_CursorPos, nLine, nCol);

	nLine -= m_nFirstVisibleLine;
	nCol -= m_nFirstVisibleCol;

	if (m_hWnd == GetFocus())
		SetCaretPos(nCol * m_CharSize.cx, nLine * m_CharSize.cy);
}

void CHexEditCtrl::ClearModified()
{
	DATA* pData = m_pData;
	for (; pData < m_pDataEnd; pData++)
		pData->m_Flags &= ~DF_MODIFIED;

	m_bModified = FALSE;
	Invalidate();
}

void CHexEditCtrl::PaintRect(HDC hdc, RECT& rctUpdate)
{
	RECT rct;
	rct.left = rctUpdate.left - rctUpdate.left % m_CharSize.cx;
	rct.top = rctUpdate.top - rctUpdate.top % m_CharSize.cy;
	rct.right = rct.left + m_CharSize.cx;
	rct.bottom = rct.top + m_CharSize.cy;

	int nFirstLine = rctUpdate.top / m_CharSize.cy;
	int nLineCount = rctUpdate.bottom / m_CharSize.cy - nFirstLine;
	if (rctUpdate.bottom % m_CharSize.cy)
		nLineCount++;

	int nFirstCol = rctUpdate.left / m_CharSize.cx;
	int nColCount = rctUpdate.right / m_CharSize.cx - nFirstCol;
	if (rctUpdate.right % m_CharSize.cx)
		nColCount++;

	nFirstLine += m_nFirstVisibleLine;
	nFirstCol += m_nFirstVisibleCol;

	SetBkColor(hdc, m_StdColorBg.m_rgbBackColor);
	m_pStdColor = &m_StdColorBg;

	int nTotalLineCount = m_nLineCount;
	if (!nTotalLineCount)
		nTotalLineCount = 1; // draw at least one

	if (nFirstLine >= nTotalLineCount) // shortcut
	{
		FastRectangle(hdc, rctUpdate);
		return;
	}

	if (nFirstLine + nLineCount >= nTotalLineCount)
		nLineCount = nTotalLineCount - nFirstLine;

	int nVisibilityFlags = 0;

	SIZE_T nOffset = nFirstLine * m_nBytesPerLine;

	if (m_OffsetWidth && nFirstCol < m_OffsetWidth)
	{
		int m = nFirstCol;
		if (m)
		{
			nFirstCol -= m;
			nColCount += m;
			rct.left -= m * m_CharSize.cx;
			rct.right -= m * m_CharSize.cx;
		}

		int nPaintColCount = m_nHexCol - nFirstCol;

		PaintOffs(hdc, rct, nLineCount, nOffset);

		nFirstCol += nPaintColCount;
		nColCount -= nPaintColCount;

		int nPaintWidth = nPaintColCount * m_CharSize.cx;
		rct.left += nPaintWidth;
		rct.right += nPaintWidth;

		ASSERT(nFirstCol == m_nHexCol);
	}

	if (nFirstCol < m_nGapCol &&
		nFirstCol + nColCount >= m_nHexCol)
	{
		int m = (nFirstCol - m_nHexCol) % 3;
		if (m)
		{
			nFirstCol -= m;
			nColCount += m;
			rct.left -= m * m_CharSize.cx;
			rct.right -= m * m_CharSize.cx;
		}

		int nPaintColCount = m_nGapCol - nFirstCol;
		if (nPaintColCount > nColCount)
		{		
			nPaintColCount = nColCount;
			m = nColCount % 3;
			if (m)
				nPaintColCount += 3 - m;
		}

		PaintHex(hdc, rct, nPaintColCount / 3, nLineCount, nOffset + (nFirstCol - m_nHexCol) / 3);

		nFirstCol += nPaintColCount;
		nColCount -= nPaintColCount;

		int nPaintWidth = nPaintColCount * m_CharSize.cx;
		rct.left += nPaintWidth;
		rct.right += nPaintWidth;
	}

	if (nFirstCol < m_nAsciiCol &&
		nFirstCol + nColCount >= m_nGapCol)
	{
		PaintGap(hdc, rct, nLineCount);

		nFirstCol++;
		nColCount--;

		rct.left += m_CharSize.cx;
		rct.right += m_CharSize.cx;
	}

	if (nFirstCol < m_nSpaceCol &&
		nFirstCol + nColCount >= m_nAsciiCol)
	{
		int nPaintColCount = m_nSpaceCol - nFirstCol;
		if (nPaintColCount > nColCount)
			nPaintColCount = nColCount;

		m_pStdColor = &m_StdColorAscii;
		SetBkColor(hdc, m_StdColorAscii.m_rgbBackColor);

		PaintAscii(hdc, rct, nPaintColCount, nLineCount, nOffset + (nFirstCol - m_nAsciiCol));

		nFirstCol += nPaintColCount;
		nColCount -= nPaintColCount;

		int nPaintWidth = nPaintColCount * m_CharSize.cx;
		rct.left += nPaintWidth;
		rct.right += nPaintWidth;

		m_pStdColor = &m_StdColorBg;
		SetBkColor(hdc, m_StdColorBg.m_rgbBackColor);
	}

	int nBottom = rct.top + nLineCount * m_CharSize.cy;

	if (nColCount > 0)
	{
		rct.right = rctUpdate.right;
		rct.bottom = nBottom;

		FastRectangle(hdc, rct);
	}

	if (rctUpdate.bottom > nBottom)
	{
		rct = rctUpdate;
		rct.top = nBottom;

		FastRectangle(hdc, rct);
	}
}

void CHexEditCtrl::ApplyColor(HDC hdc, COLOR_INFO* pColor, int PrevColorFlags)
{
	if (!pColor)
	{
		if (PrevColorFlags & CF_BACK_COLOR)
			SetBkColor(hdc, m_pStdColor->m_rgbBackColor);

		if (PrevColorFlags & CF_FORE_COLOR)
			SetTextColor(hdc, m_pStdColor->m_rgbForeColor);
	}
	else
	{
		if (pColor->m_Flags & CF_BACK_COLOR)
			SetBkColor(hdc, pColor->m_rgbBackColor);
		else if (PrevColorFlags & CF_BACK_COLOR)
			SetBkColor(hdc, m_pStdColor->m_rgbBackColor);

		if (pColor->m_Flags & CF_FORE_COLOR)
			SetTextColor(hdc, pColor->m_rgbForeColor);
		else if (PrevColorFlags & CF_FORE_COLOR)
			SetTextColor(hdc, m_pStdColor->m_rgbForeColor);
	}
}

CHexEditCtrl::COLOR_INFO CHexEditCtrl::GetDataColor(DATA* pData)
{
	if (!pData)
		return COLOR_INFO();

	COLOR_INFO Color;

	Color += pData->m_Color;

	if (pData->m_Flags & DF_MODIFIED)
		Color += m_ModifiedColor;

	if (m_Selection.IsEmpty() && pData == m_pData + m_CursorPos.m_nOffset)
		Color += m_CursorColor;

	if (IsDataSelected(pData - m_pData))
		Color += m_SelectionColor;

	return Color;
}

CHAR CHexEditCtrl::GetHexChar(BYTE Data)
{
	static CHAR _UpperCaseHexChars[] = "0123456789ABCDEF";
	static CHAR _LowerCaseHexChars[] = "0123456789abcdef";
	
	Data &= 0xf;

	return m_bUpperCase ? _UpperCaseHexChars[Data] : _LowerCaseHexChars[Data];
}

void CHexEditCtrl::PaintOffs(HDC hdc, RECT rct, int nLineCount, SIZE_T nOffset)
{
	rct.right = m_nHexCol * m_CharSize.cx;
	rct.bottom = rct.top + m_CharSize.cy;

	COLORREF OldColor = SetTextColor(hdc, GetSysColor(COLOR_GRAYTEXT));

    for (int i = 0; i < nLineCount; i++)
	{
		BYTE b1 = HIBYTE(HIWORD(nOffset));
		BYTE b2 = LOBYTE(HIWORD(nOffset));
		BYTE b3 = HIBYTE(LOWORD(nOffset));
		BYTE b4 = LOBYTE(LOWORD(nOffset));

		CHAR szText[8] = 
		{ 
			GetHexChar(b1 >> 4),
			GetHexChar(b1),
			GetHexChar(b2 >> 4),
			GetHexChar(b2),
			GetHexChar(b3 >> 4),
			GetHexChar(b3),
			GetHexChar(b4 >> 4),
			GetHexChar(b4),
		};

		ExtTextOut(hdc, rct.left, rct.top, ETO_OPAQUE, &rct, szText + 8 - m_OffsetWidth, m_OffsetWidth, NULL);
		
		rct.top += m_CharSize.cy;
		rct.bottom += m_CharSize.cy;

		nOffset += m_nBytesPerLine;
	}

	SetTextColor(hdc, OldColor);
}

void CHexEditCtrl::PaintHex(HDC hdc, RECT rct, int nColCount, int nLineCount, SIZE_T nOffset)
{

	ASSERT(nColCount <= m_nBytesPerLine);

	int left = rct.left;

	DATA* pData = m_pData + nOffset;
	int nExtra = m_nBytesPerLine - nColCount;

	int PrevColorFlags = 0;

    for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nColCount; j++, pData++)
		{
			if (pData >= m_pDataEnd)
			{
				ASSERT(i == nLineCount - 1);
				rct.right = rct.left + (nColCount - j) * m_CharSize.cx * 3;
				ApplyColor(hdc, NULL, PrevColorFlags);
				FastRectangle(hdc, rct);
				return;
			}	

			COLOR_INFO Color = GetDataColor(pData);
			ApplyColor(hdc, &Color, PrevColorFlags);

			if (pData->m_Flags & DF_4_BITS)
			{
				FastChar(hdc, rct, GetHexChar(pData->m_Data));
				rct.left += m_CharSize.cx;
				rct.right += m_CharSize.cx;

				FastRectangle(hdc, rct);
				rct.left += m_CharSize.cx;
				rct.right += m_CharSize.cx;
			}
			else
			{
				FastChar(hdc, rct, GetHexChar(pData->m_Data >> 4));
				rct.left += m_CharSize.cx;
				rct.right += m_CharSize.cx;

				FastChar(hdc, rct, GetHexChar(pData->m_Data));
				rct.left += m_CharSize.cx;
				rct.right += m_CharSize.cx;
			}

			if (IsGapSelected(pData - m_pData))
			{
				// gap shuold be selection color (even though NOT both left and right are selection)
				ApplyColor(hdc, &m_SelectionColor, Color.m_Flags);
				PrevColorFlags = m_SelectionColor.m_Flags;
			} 
			else
			{
				// color of this gap depend on the next data

				DATA* pNextData = pData + 1;

				if (pNextData < m_pDataEnd && GetDataColor(pNextData) == Color)
				{
					// keep color
					PrevColorFlags = Color.m_Flags;
				}
				else
				{
					// reset color
					ApplyColor(hdc, NULL, Color.m_Flags);
					PrevColorFlags = 0;
				}
			}

			if (m_nHexGapStep > 0 && ((pData - m_pData) % m_nHexGapStep) == m_nHexGapStep - 1)
			{
				int width = m_CharSize.cx * 2;
				RECT rctBigGap = { rct.left, rct.top, rct.left + width, rct.bottom };
				FastRectangle(hdc, rctBigGap);
				rct.left += width;
				rct.right += width;
			}
			else
			{
				FastRectangle(hdc, rct);
				rct.left += m_CharSize.cx;
				rct.right += m_CharSize.cx;
			}
		}

		rct.left = left;
		rct.right = rct.left + m_CharSize.cx;
		rct.top += m_CharSize.cy;
		rct.bottom += m_CharSize.cy;

		pData += nExtra;
	}

	ApplyColor(hdc, NULL, PrevColorFlags);
}

void CHexEditCtrl::PaintGap(HDC hdc, RECT rct, int nLineCount)
{
	rct.bottom = rct.top + nLineCount * m_CharSize.cy;
	FastRectangle(hdc, rct);
}

void CHexEditCtrl::PaintAscii(HDC hdc, RECT rct, int nColCount, int nLineCount, SIZE_T nOffset)
{
	ASSERT(nColCount <= m_nBytesPerLine);

	int left = rct.left;

	DATA* pData = m_pData + nOffset;
	int nExtra = m_nBytesPerLine - nColCount;

	int PrevColorFlags = 0;

    for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nColCount; j++, pData++)
		{
			if (pData >= m_pDataEnd)
			{
				ASSERT(i == nLineCount - 1);
				rct.right = rct.left + (nColCount - j) * m_CharSize.cx;
				ApplyColor(hdc, NULL, PrevColorFlags);
				FastRectangle(hdc, rct);
				return;
			}			

			COLOR_INFO Color = GetDataColor(pData);
			ApplyColor(hdc, &Color, PrevColorFlags);
			PrevColorFlags = Color.m_Flags;

			CHAR ch = m_bOnlyPrintableChars ? isprint(pData->m_Data) ? pData->m_Data : '.' : pData->m_Data;
			FastChar(hdc, rct, ch);
			rct.left += m_CharSize.cx;
			rct.right += m_CharSize.cx;
		}

		rct.left = left;
		rct.right = rct.left + m_CharSize.cx;
		rct.top += m_CharSize.cy;
		rct.bottom += m_CharSize.cy;

		pData += nExtra;
	}

	ApplyColor(hdc, NULL, PrevColorFlags);
}

void CHexEditCtrl::FixupFirstVisibleLineCol()
{
	int nNewFirstVisibleLine = m_nFirstVisibleLine;
	int nNewFirstVisibleCol = m_nFirstVisibleCol;

	if (m_nFirstVisibleLine + m_nVisibleLineCount > m_nLineCount)
		nNewFirstVisibleLine = m_nLineCount - m_nVisibleLineCount;

	if (nNewFirstVisibleLine < 0)
		nNewFirstVisibleLine = 0;

	if (m_nFirstVisibleCol + m_nVisibleColCount > m_nSpaceCol)
		nNewFirstVisibleCol = m_nSpaceCol - m_nVisibleColCount;

	if (nNewFirstVisibleCol < 0)
		nNewFirstVisibleCol = 0;

	if (nNewFirstVisibleLine != m_nFirstVisibleLine || nNewFirstVisibleCol != m_nFirstVisibleCol)
	{
		m_nFirstVisibleLine = nNewFirstVisibleLine;
		m_nFirstVisibleCol = nNewFirstVisibleCol;

		Invalidate();
		UpdateCaretPos();
	}
}

void CHexEditCtrl::RecalcScrollBars()
{
	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_ALL;

	si.nMin = 0;
    si.nMax = m_nLineCount - 1;
    si.nPage = m_nVisibleLineCount;
	si.nPos = m_nFirstVisibleLine;
	SetScrollInfo(SB_VERT, &si);

	si.nMin = 0;
    si.nMax = m_nSpaceCol - 1;
    si.nPage = m_nVisibleColCount;
    si.nPos = m_nFirstVisibleCol;
	SetScrollInfo(SB_HORZ, &si);
}

void CHexEditCtrl::RecalcLayout()
{
	RECT rctClient;
	GetClientRect(&rctClient);

	m_nVisibleLineCount = rctClient.bottom / m_CharSize.cy;
	m_nVisibleColCount = rctClient.right / m_CharSize.cx;

	FixupFirstVisibleLineCol();	

	RecalcScrollBars();
}

LRESULT CHexEditCtrl::OnCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetBytesPerLineImpl(8);
	return 0;
}

LRESULT CHexEditCtrl::OnGetDlgCode(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
}

LRESULT CHexEditCtrl::OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(&ps);

	int nResult = GetClipRgn(hdc, m_hUpdateRgn);

	if (nResult == NULLREGION)
	{
		EndPaint(&ps);
		return 0;
	}

	BYTE Buffer[sizeof(RGNDATA) + 2 * sizeof(RECT)];
	RGNDATA* pRgnData = (RGNDATA*) Buffer;
	RECT* pRects = (RECT*) pRgnData->Buffer;

	HFONT hOldFont = (HFONT) SelectObject(hdc, m_hFont);

	nResult = GetRegionData(m_hUpdateRgn, sizeof(Buffer), pRgnData);
	if (!nResult || pRgnData->rdh.nCount != 2) // not enough
	{
		PaintRect(hdc, ps.rcPaint);
	}
	else
	{
		PaintRect(hdc, pRects[0]);
		PaintRect(hdc, pRects[1]);
	}

	SelectObject(hdc, hOldFont);

	EndPaint(&ps);
	return 0;
}

/*

LRESULT CHexEditCtrl::OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nResult = GetUpdateRgn(m_hUpdateRgn);

	PAINTSTRUCT ps;
	if (nResult == NULLREGION)
	{
		BeginPaint(&ps);
		EndPaint(&ps);
		return 0;
	}

	BYTE Buffer[sizeof(RGNDATA) + 2 * sizeof(RECT)];
	RGNDATA* pRgnData = (RGNDATA*) Buffer;
	RECT* pRects = (RECT*) pRgnData->Buffer;
	UINT nCount = 0;

	nResult = GetRegionData(m_hUpdateRgn, sizeof(Buffer), pRgnData);
	if (nResult == 0 || nResult > sizeof(Buffer)) // not enough
	{
		GetUpdateRect(&pRects[0]);
		nCount = 1;
	}
	else if (pRgnData->rdh.nCount != 2)
	{
		pRects[0] = pRgnData->rdh.rcBound;
		nCount = 1;
	}
	else
	{
		nCount = 2;
	}

	HDC hdc = BeginPaint(&ps);

	for (UINT i = 0; i < nCount; i++)
	{
#ifdef _HEXEDIT_USE_OFFSCREEN_BUFFER
		Paint(m_hOffscreenDC, pRects[i]);
		BitBlt(
			hdc, 
			ps.rcPaint.left, ps.rcPaint.top, 
			ps.rcPaint.right - ps.rcPaint.left, 
			ps.rcPaint.bottom - ps.rcPaint.top, 
			m_hOffscreenDC,
			ps.rcPaint.left, ps.rcPaint.top, 
			SRCCOPY
			);
#else
		Paint(hdc, pRects[i]); // paint direct
#endif
	}

#ifdef _HEXEDIT_SHOW_ACTUAL_REPAINT_AREAS 
	int y = m_ClientSize.cy / 2;

	SetViewportOrgEx(hdc, 0, y, NULL);

	RECT rct = { 0, 0, m_ClientSize.cx, y }; 
	FillRect(hdc, &rct, (HBRUSH) GetStockObject(GRAY_BRUSH));

	for (UINT i = 0; i < nCount; i++)
		Paint(hdc, pRects[i]); // paint direct
#endif

	EndPaint(&ps);
	return 0;
}

*/

LRESULT CHexEditCtrl::OnSetFocus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UpdateCaret();
	return 0;
}

LRESULT CHexEditCtrl::OnKillFocus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Finalize4Bits();
	DestroyCaret();
	return 0;
}

int CHexEditCtrl::AutoAdjustBytesPerLine()
{
	static int _BytesPerLine[] = { 32, 16, 8, 4 };

	int nDelta = m_OffsetWidth + 1;

	for (int i = 0; i < sizeof(_BytesPerLine) / sizeof(_BytesPerLine[0]); i++)
		if (m_nVisibleColCount >= _BytesPerLine[i] * 4 + nDelta)
		{
			SetBytesPerLineImpl(_BytesPerLine[i]);
			return _BytesPerLine[i];
		}

	SetBytesPerLineImpl(2);
	return 2;
}

LRESULT CHexEditCtrl::OnSize(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	RecalcLayout();

	if (m_bAutoAdjustBytesPerLine)
		AutoAdjustBytesPerLine();

	return 0;
}

LRESULT CHexEditCtrl::OnScroll(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nBar = nMsg == WM_VSCROLL ? SB_VERT : SB_HORZ;

	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_ALL;
	GetScrollInfo(nBar, &si);

	int nNewPos = si.nPos;
	
	int nCode = LOWORD(wParam);
	switch (nCode)
	{
	case SB_TOP:
		nNewPos = 0;
		break;

	case SB_LINEUP:
		nNewPos--;
		break;

	case SB_LINEDOWN:
		nNewPos++;
		break;

	case SB_PAGEUP:
		nNewPos -= si.nPage; 
		break;

	case SB_PAGEDOWN:
		nNewPos += si.nPage; 
		break;

	case SB_BOTTOM:
		nNewPos = si.nMax; 
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nNewPos = si.nTrackPos; 
		break;
	}

	int nMaxPos = si.nMax - si.nPage + 1;

	if (nNewPos < 0)
		nNewPos = 0;
	else if (nNewPos > nMaxPos)
		nNewPos = nMaxPos;

	if (nNewPos != si.nPos)
	{
		if (nMsg == WM_VSCROLL)
			m_nFirstVisibleLine = nNewPos;
		else
			m_nFirstVisibleCol = nNewPos;

		si.fMask = SIF_POS;
		si.nPos = nNewPos;
		SetScrollInfo(nBar, &si);

		Invalidate();
		UpdateCaretPos();
	}

	return 0;
}

LRESULT CHexEditCtrl::OnMouseWheel(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SHORT zDelta = HIWORD(wParam);

	WPARAM wParamScroll;
	int nLineCount;

	if (zDelta > 0)
	{
		wParamScroll = MAKELONG(SB_LINEUP, 0);
		nLineCount = zDelta / WHEEL_DELTA;
	}
	else
	{
		wParamScroll = MAKELONG(SB_LINEDOWN, 0);
		nLineCount = -zDelta / WHEEL_DELTA;
	}

	nLineCount *= m_nMouseWheelSpeed;
	
	for (int i = 0; i < nLineCount; i++)
		OnScroll(WM_VSCROLL, wParamScroll, 0, bHandled);

	return 0;
}

LRESULT CHexEditCtrl::OnLButtonDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { (SHORT) LOWORD(lParam), (SHORT) HIWORD(lParam) };
	CURSOR_POS pos = CursorPosFromMousePos(pt);
	BOOL bShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

	SetFocus();
	SetCapture();
	SetCursorPos(pos, bShift);

	if (!bShift)
	{
		m_bSelecting = TRUE;
		m_SelStart = m_SelEnd = m_CursorPos;
	}

	m_bDragging = TRUE;

	return 0;
}

LRESULT CHexEditCtrl::OnLButtonUp(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bDragging = FALSE;
	ReleaseCapture();
	return 0;
}

LRESULT CHexEditCtrl::OnCaptureChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bDragging = FALSE;
	return 0;
}

LRESULT CHexEditCtrl::OnMouseMove(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bDragging)
		return 0;

	POINT pt = { (SHORT) LOWORD(lParam), (SHORT) HIWORD(lParam) };
	CURSOR_POS pos = CursorPosFromMousePos(pt);

	if (m_SelStart.m_Location < CURSOR_ASCII_0 && pos.m_Location >= CURSOR_ASCII_0)
	{
		pos.m_nOffset += m_nBytesPerLine - pos.m_nOffset % m_nBytesPerLine - 1;
		pos.m_Location = CURSOR_HEX_2;
	}
	else if (m_SelStart.m_Location >= CURSOR_ASCII_0 && pos.m_Location < CURSOR_ASCII_0)
	{
		pos.m_nOffset -= pos.m_nOffset % m_nBytesPerLine;
		pos.m_Location = CURSOR_ASCII_0;
	}

	SetCursorPos(pos, MCF_SELECT);

	return 0;
}

LRESULT CHexEditCtrl::OnSysKeyDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bAlt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

	switch (wParam)
	{
	case VK_BACK:
		if (bAlt)
		{
			m_bIgnoreBackspace = TRUE;
			Undo();
		};
		break;

	default:
		bHandled = FALSE;
		break;
	};


	return 0;
}

LRESULT CHexEditCtrl::OnKeyDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	BOOL bCtrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	switch (wParam)
	{
	case VK_TAB:
		m_bIgnoreTab = TRUE;
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

	case VK_DELETE:
		if (bShift)
			Cut();
		else
			OnKeyDelete();
		break;

	case VK_BACK:
		m_bIgnoreBackspace = TRUE;
		OnKeyBackspace();
		break;

	case VK_INSERT:
		if (bCtrl)
			Copy();
		else if (bShift)
			Paste();
		else
			SetOverwriteMode(!m_bOverwriteMode);

		break;
	};

	return 0;
}

void CHexEditCtrl::OnKeyTab()
{
	static BYTE _TransitionTable[] = 
	{
		CURSOR_ASCII_0, // from CURSOR_HEX_0
		CURSOR_ASCII_1, // from CURSOR_HEX_1
		CURSOR_ASCII_1, // from CURSOR_HEX_2

		CURSOR_HEX_0, // from CURSOR_ASCII_0
		CURSOR_HEX_2, // from CURSOR_ASCII_1
	};
	
	BYTE NewLocation = _TransitionTable[m_CursorPos.m_Location];
	TransitCursor(0, NewLocation, MCF_KEEP_SELECTION);
}

void CHexEditCtrl::OnKeyLeft(BOOL bShift, BOOL bCtrl)
{
	// begin of ASCII line -- special case
	if (m_CursorPos.m_Location == CURSOR_ASCII_0 && 
		m_CursorPos.m_nOffset % m_nBytesPerLine == 0)
	{
		TransitCursor(-1, CURSOR_ASCII_1, bShift ? MCF_SELECT : 0);
		return;
	}

	static CURSOR_TRANSITION _TransitionTable[] = 
	{
		{ -1, CURSOR_HEX_2 }, // from CURSOR_HEX_0
		{ 0,  CURSOR_HEX_0 }, // from CURSOR_HEX_1
		{ 0,  CURSOR_HEX_1 }, // from CURSOR_HEX_2

		{ -1, CURSOR_ASCII_0 }, // from CURSOR_ASCII_0
		{ 0,  CURSOR_ASCII_0 }, // from CURSOR_ASCII_1
	};

	CURSOR_TRANSITION Transition = _TransitionTable[m_CursorPos.m_Location];

	if (m_bOverwriteMode && Transition.m_NewLocation == CURSOR_HEX_2)
		Transition.m_NewLocation = CURSOR_HEX_1;

	TransitCursor(&Transition, bShift ? MCF_SELECT : 0);
}

void CHexEditCtrl::OnKeyRight(BOOL bShift, BOOL bCtrl)
{
	// end of ASCII line -- special case
	if (m_CursorPos.m_Location == CURSOR_ASCII_1 && 
		m_CursorPos.m_nOffset % m_nBytesPerLine == m_nBytesPerLine - 1)
	{
		TransitCursor(1, CURSOR_ASCII_0, bShift ? MCF_SELECT : 0);
		return;
	}

	static CURSOR_TRANSITION _TransitionTable[] = 
	{
		{ 0, CURSOR_HEX_1 }, // from CURSOR_HEX_0
		{ 0, CURSOR_HEX_2 }, // from CURSOR_HEX_1
		{ 1, CURSOR_HEX_0 }, // from CURSOR_HEX_2

		{ 0, CURSOR_ASCII_1 }, // from CURSOR_ASCII_0
		{ 1, CURSOR_ASCII_1 }, // from CURSOR_ASCII_1
	};

	CURSOR_TRANSITION Transition = _TransitionTable[m_CursorPos.m_Location];

	if (m_bOverwriteMode)
	{
		if (Transition.m_NewLocation == CURSOR_ASCII_1)
		{
			Transition.m_NewLocation = CURSOR_ASCII_0;
			Transition.m_nOffsetDelta = 1;
		}
		else if (Transition.m_NewLocation == CURSOR_HEX_2)
		{
			Transition.m_NewLocation = CURSOR_HEX_0;
			Transition.m_nOffsetDelta = 1;
		}
	}

	TransitCursor(&Transition, bShift ? MCF_SELECT : 0);
}

void CHexEditCtrl::OnKeyUp(BOOL bShift, BOOL bCtrl)
{
	int Flags = bShift ? MCF_SELECT : 0;
	TransitCursor(-m_nBytesPerLine, -1, Flags);
}

void CHexEditCtrl::OnKeyDown(BOOL bShift, BOOL bCtrl)
{
	int Flags = bShift ? MCF_SELECT : 0;
	TransitCursor(m_nBytesPerLine, -1, Flags);
}

void CHexEditCtrl::OnKeyPageUp(BOOL bShift, BOOL bCtrl)
{
	int Flags = bShift ? MCF_SELECT : 0;
	
	if (!bCtrl)
	{
		TransitCursor(-m_nVisibleLineCount * m_nBytesPerLine, -1, Flags);
	}
	else
	{
		int nLine, nCol;
		GetCursorLineCol(m_CursorPos, nLine, nCol, FALSE);
		nLine -= m_nFirstVisibleLine;

		TransitCursor(-nLine * m_nBytesPerLine, -1, Flags);
	}
}


void CHexEditCtrl::OnKeyPageDown(BOOL bShift, BOOL bCtrl)
{
	int Flags = bShift ? MCF_SELECT : 0;
	
	if (!bCtrl)
	{
		TransitCursor(m_nVisibleLineCount * m_nBytesPerLine, -1, Flags);
	}
	else if (m_nVisibleLineCount > 1)
	{		
		int nLine, nCol;
		GetCursorLineCol(m_CursorPos, nLine, nCol, FALSE);
		nLine -= m_nFirstVisibleLine;

		TransitCursor((m_nVisibleLineCount - nLine - 1) * m_nBytesPerLine, -1, Flags);
	}
}

void CHexEditCtrl::OnKeyHome(BOOL bShift, BOOL bCtrl)
{
	int Flags = bShift ? MCF_SELECT : 0;
	int Location = m_CursorPos.m_Location >= CURSOR_ASCII_0 ? CURSOR_ASCII_0 : CURSOR_HEX_0;

	if (bCtrl)
	{
		CURSOR_POS Pos;
		Pos.m_nOffset = 0;
		Pos.m_Location = Location;
		SetCursorPos(Pos, Flags);
	}
	else
	{
		int nDelta = (int) m_CursorPos.m_nOffset % m_nBytesPerLine;
		TransitCursor(-nDelta, Location, Flags);
	}
}

void CHexEditCtrl::OnKeyEnd(BOOL bShift, BOOL bCtrl)
{
	int Flags = bShift ? MCF_SELECT : 0;
	int Location = m_CursorPos.m_Location >= CURSOR_ASCII_0 ? CURSOR_ASCII_1 : CURSOR_HEX_2;

	if (bCtrl)
	{
		CURSOR_POS Pos;
		Pos.m_nOffset = (int) m_nDataSize - 1;
		Pos.m_Location = Location;
		SetCursorPos(Pos, Flags);
	}
	else
	{
		int nDelta = m_nBytesPerLine - (int) m_CursorPos.m_nOffset % m_nBytesPerLine - 1;
		TransitCursor(nDelta, Location, Flags);
	}
}

void CHexEditCtrl::OnKeyDelete()
{
	if (!m_Selection.IsEmpty())
	{
		ReplaceSelection(NULL, 0);
		return;
	}

	switch (m_CursorPos.m_Location)
	{
	case CURSOR_ASCII_0:
		if (m_CursorPos.m_nOffset >= (int) m_nDataSize)
			break;

		BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);
		InsertDeleteData(m_CursorPos.m_nOffset, 0, 1);		
		EndUndoableTransaction(0);

		break;

	case CURSOR_ASCII_1:
		{
		SIZE_T nOffset = m_CursorPos.m_nOffset + 1;
		if (nOffset >= m_nDataSize)
			break;

		BeginUndoableTransaction(nOffset, 1);
		InsertDeleteData(nOffset, 0, 1);
		EndUndoableTransaction(0);
		}
		break;

	case CURSOR_HEX_0:
		{
		if (m_CursorPos.m_nOffset >= (LONG) m_nDataSize)
			break;

		DATA* pData = m_pData + m_CursorPos.m_nOffset;

		if (pData->m_Flags & DF_4_BITS)
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);
			InsertDeleteData(m_CursorPos.m_nOffset, 0, 1);
			EndUndoableTransaction(0);
		}
		else
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);
			pData->m_Data &= 0xf;
			pData->m_Flags |= DF_4_BITS | DF_MODIFIED;
			SetModified();
			InvalidateOffset(m_CursorPos.m_nOffset);
			EndUndoableTransaction(1);
		}

		}
		break;

	case CURSOR_HEX_1:
		{
		DATA* pData = m_pData + m_CursorPos.m_nOffset;

		if (pData->m_Flags & DF_4_BITS)
		{
			if (m_CursorPos.m_nOffset >= (LONG) m_nDataSize -1)
				break;

			OnKeyRight(FALSE, FALSE);
			OnKeyDelete();
		}
		else
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);
			pData->m_Data >>= 4;
			pData->m_Data &= 0xf;
			pData->m_Flags |= DF_4_BITS | DF_MODIFIED;
			SetModified();
			InvalidateOffset(m_CursorPos.m_nOffset);
			EndUndoableTransaction(1);
		}
		}
		break;

	case CURSOR_HEX_2:
		if (m_CursorPos.m_nOffset >= (LONG) m_nDataSize -1)
			break;

		OnKeyRight(FALSE, FALSE);
		OnKeyDelete();
		break;
	}
}

void CHexEditCtrl::OnKeyBackspace()
{
	if (!m_Selection.IsEmpty())
	{
		ReplaceSelection(NULL, 0);
		return;
	}

	switch (m_CursorPos.m_Location)
	{
	case CURSOR_ASCII_0:
		if (m_CursorPos.m_nOffset == 0)
			break;

		BeginUndoableTransaction(m_CursorPos.m_nOffset - 1, 1);
		InsertDeleteData(m_CursorPos.m_nOffset - 1, 0, 1);
		TransitCursor(-1, -1);
		EndUndoableTransaction(0);

		break;

	case CURSOR_ASCII_1:
		BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);
		InsertDeleteData(m_CursorPos.m_nOffset, 0, 1);
		TransitCursor(-1, -1);
		EndUndoableTransaction(0);

		break;

	case CURSOR_HEX_0:
		if (m_CursorPos.m_nOffset == 0)
			break;

		OnKeyLeft(FALSE, FALSE);
		OnKeyBackspace();
		break;

	case CURSOR_HEX_1:
		{
		DATA* pData = m_pData + m_CursorPos.m_nOffset;
		
		if (pData->m_Flags & DF_4_BITS)
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);
			InsertDeleteData(m_CursorPos.m_nOffset, 0, 1);
			TransitCursor(-1, CURSOR_HEX_2);
			EndUndoableTransaction(0);
		}
		else
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);
			pData->m_Data &= 0xf;
			pData->m_Flags |= DF_4_BITS | DF_MODIFIED;
			SetModified();
			InvalidateOffset(m_CursorPos.m_nOffset);
			TransitCursor(0, CURSOR_HEX_0);
			EndUndoableTransaction(1);
		}

		}
		break;

	case CURSOR_HEX_2:
		{
		DATA* pData = m_pData + m_CursorPos.m_nOffset;

		BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);
		pData->m_Data >>= 4;
		pData->m_Data &= 0xf;
		pData->m_Flags |= DF_4_BITS | DF_MODIFIED;
		SetModified();
		InvalidateOffset(m_CursorPos.m_nOffset);
		TransitCursor(0, CURSOR_HEX_1);
		EndUndoableTransaction(1);
		}
		break;
		
	}
}

static int GetHex(CHAR ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (ch >= 'a' && ch <= 'f')
		return ch + 10 - 'a';

	if (ch >= 'A' && ch <= 'F')
		return ch + 10 - 'A';

	return -1; // error
}

void CHexEditCtrl::ModifyData(DATA* pData, BYTE Data, BOOL b4Bits)
{
	pData->m_Data = Data;
	if (b4Bits)
		pData->m_Flags |= DF_4_BITS;
	else
		pData->m_Flags &= ~DF_4_BITS;

	pData->m_Flags |= DF_MODIFIED;
	SetModified();
	InvalidateData(pData);
}

void CHexEditCtrl::SelectAll()
{
	m_bSelecting = TRUE;
	m_SelStart.m_nOffset = 0;
	m_SelStart.m_Location = m_CursorPos.m_Location >= CURSOR_ASCII_0 ? CURSOR_ASCII_0 : CURSOR_HEX_0;
	
	CURSOR_POS Pos;
	Pos.m_nOffset = (int) m_nDataSize;
	SetCursorPos(Pos, MCF_SELECT | MCF_FORCE);
}

LRESULT CHexEditCtrl::OnChar(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CHAR ch = (CHAR) wParam;

	if (ch == VK_TAB && m_bIgnoreTab)
	{
		m_bIgnoreTab = FALSE;
		return 0;
	}

	if (ch == VK_BACK && m_bIgnoreBackspace)
	{
		m_bIgnoreBackspace = FALSE;
		return 0;
	}

	switch (ch)
	{
	case 1 + 'C' - 'A':
		Copy();
		return 0;

	case 1 + 'V' - 'A':
		Paste();
		return 0;

	case 1 + 'X' - 'A':
		Cut();
		return 0;
	}

	if (!m_Selection.IsEmpty())
	{
		if (m_CursorPos.m_Location >= CURSOR_ASCII_0)
		{
			ReplaceSelection(&ch, 1);
			return 0;
		}

		int x = GetHex(ch);
		if (x == -1)
			return 0;

		ReplaceSelection(&ch, 1, FALSE);

		BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);

		DATA* pData = m_pData + m_CursorPos.m_nOffset;
		ModifyData(pData, x, TRUE);
		TransitCursor(0, CURSOR_HEX_1);

		EndUndoableTransaction(1);

		return 0;
	}

	switch (m_CursorPos.m_Location)
	{
	case CURSOR_ASCII_0:
		{

		BOOL bInsert = !m_bOverwriteMode || m_CursorPos.m_nOffset == (int) m_nDataSize;

		if (bInsert)
			InsertDeleteData(m_CursorPos.m_nOffset, 1, 0);

		BeginUndoableTransaction(m_CursorPos.m_nOffset, bInsert ? 0 : 1);

		DATA* pData = m_pData + m_CursorPos.m_nOffset;
		ModifyData(pData, ch);

		TransitCursor(1, -1);
		
		EndUndoableTransaction(1);

		}
		break;

	case CURSOR_ASCII_1:
		{
		SIZE_T nOffset = m_CursorPos.m_nOffset + 1;
		BOOL bInsert = !m_bOverwriteMode || nOffset == m_nDataSize;

		if (bInsert)
			InsertDeleteData(nOffset, 1, 0);

		BeginUndoableTransaction(nOffset, bInsert ? 0 : 1);

		DATA* pData = m_pData + nOffset;
		ModifyData(pData, ch);

		TransitCursor(1, -1);

		EndUndoableTransaction(1);

		}
		break;

	case CURSOR_HEX_0:
		{
		int x = GetHex(ch);
		if (x == -1)
			break;

		DATA* pData = m_pData + m_CursorPos.m_nOffset;

		if (m_bOverwriteMode)
		{
			ASSERT(pData < m_pDataEnd);

			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);

			BOOL b4Bits = (pData->m_Flags & DF_4_BITS) != 0;
			BYTE Value = b4Bits ? (x & 0xf) : ((pData->m_Data & 0xf) | ((x << 4) & 0xf0));
			ModifyData(pData, Value, b4Bits);
		}
		else if (pData < m_pDataEnd && (pData->m_Flags & DF_4_BITS))
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);

			BYTE Value = (pData->m_Data & 0xf) | ((x << 4) & 0xf0);
			ModifyData(pData, Value, FALSE);
		}
		else
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 0);

			InsertDeleteData(m_CursorPos.m_nOffset, 1, 0);

			pData = m_pData + m_CursorPos.m_nOffset; // cause insert/delete can reallocate
			ModifyData(pData, x & 0xf, TRUE);
		}

		TransitCursor(0, CURSOR_HEX_1);
		EndUndoableTransaction(1);

		}
		break;

	case CURSOR_HEX_1:
		{
		int x = GetHex(ch);
		if (x == -1)
			break;

		DATA* pData = m_pData + m_CursorPos.m_nOffset;
		BOOL b4Bits = (pData->m_Flags & DF_4_BITS) != 0;

		if (m_bOverwriteMode)
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);

			BYTE Value = ((b4Bits ? pData->m_Data << 4 : pData->m_Data) & 0xf0) | (x & 0xf);
			ModifyData(pData, Value, FALSE);

			TransitCursor(1, CURSOR_HEX_0);
		}
		else if (b4Bits)
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 1);

			BYTE Value = ((b4Bits ? pData->m_Data << 4 : pData->m_Data) & 0xf0) | (x & 0xf);
			ModifyData(pData, Value, FALSE);

			TransitCursor(0, CURSOR_HEX_2);
		}
		else
		{
			BeginUndoableTransaction(m_CursorPos.m_nOffset, 0);

			InsertDeleteData(m_CursorPos.m_nOffset, 1, 0);
			
			pData = m_pData + m_CursorPos.m_nOffset; // cause insert/delete can reallocate
			DATA* pNextData = pData + 1;
	
			ModifyData(pData, (pNextData->m_Data & 0xf0) | (x & 0xf), FALSE);
			ModifyData(pNextData, pNextData->m_Data & 0xf, TRUE);

			TransitCursor(1, CURSOR_HEX_0);
		}

		EndUndoableTransaction(1);

		}
		break;	

	case CURSOR_HEX_2:
		{
		int x = GetHex(ch);
		if (x == -1)
			break;

		SIZE_T nOffset = m_CursorPos.m_nOffset + 1;

		BeginUndoableTransaction(nOffset, 0);

		InsertDeleteData(nOffset, 1, 0);
		DATA* pData = m_pData + nOffset; // cause insert/delete can reallocate
		
		ModifyData(pData, x & 0xf, TRUE);

		TransitCursor(1, CURSOR_HEX_1);

		EndUndoableTransaction(1);

		}
		break;	
	}

	return 0;
}

BOOL CHexEditCtrl::ReplaceData(SIZE_T nOffset, SIZE_T nDelete, DATA* pData, SIZE_T nSize, BOOL bCursorToEnd)
{	
	BOOL bResult = InsertDeleteData(nOffset, nSize, nDelete);
	if (!bResult)
		return FALSE;

	memcpy(m_pData + nOffset, pData, nSize * sizeof(DATA));

	SetModified();

	SIZE_T nInvalidateSize = max(nDelete, nSize);
	ASSERT(nInvalidateSize);

	InvalidateRange(nOffset, nOffset + nInvalidateSize - 1);

	CURSOR_POS Pos;
	Pos.m_nOffset = (int) nOffset; 
	if (bCursorToEnd)
		Pos.m_nOffset += (int) nSize;

	if (m_CursorPos.m_Location >= CURSOR_ASCII_0)
		Pos.m_Location = CURSOR_ASCII_0;
	else
		Pos.m_Location = CURSOR_HEX_0;

	SetCursorPos(Pos);

	return TRUE;
}

#if (_AXL_VER >= 0x0200)
void CHexEditCtrl::UndoImpl(axl::obj::TData* _pParams)
#else
void CHexEditCtrl::UndoImpl(AXL_PTR* _pParams)
#endif
{
	UNDO_PARAMS* pParams = (UNDO_PARAMS*) _pParams->m_p;
	if (pParams->m_OldData.size())
		ReplaceData(pParams->m_nOffset, pParams->m_NewData.size(), &pParams->m_OldData.front(), pParams->m_OldData.size(), FALSE);
	else
		ReplaceData(pParams->m_nOffset, pParams->m_NewData.size(), NULL, 0, FALSE);

	SetCursorPos(pParams->m_OldCursorPos);
}

#if (_AXL_VER >= 0x0200)
void CHexEditCtrl::RedoImpl(axl::obj::TData* _pParams)
#else
void CHexEditCtrl::RedoImpl(AXL_PTR* _pParams)
#endif
{
	UNDO_PARAMS* pParams = (UNDO_PARAMS*) _pParams->m_p;
	if (pParams->m_NewData.size())
		ReplaceData(pParams->m_nOffset, pParams->m_OldData.size(), &pParams->m_NewData.front(), pParams->m_NewData.size(), FALSE);
	else
		ReplaceData(pParams->m_nOffset, pParams->m_OldData.size(), NULL, 0, FALSE);

	SetCursorPos(pParams->m_NewCursorPos);
}

void CHexEditCtrl::BeginUndoableTransaction(SIZE_T nOffset, SIZE_T nOldDataSize)
{
	m_UndoParams.Create(_AXLTAG("replace_sel_undo"));
	m_UndoParams->m_nOffset = nOffset;
	m_UndoParams->m_OldCursorPos = m_CursorPos;

	if (nOldDataSize)
	{
		m_UndoParams->m_OldData.resize(nOldDataSize);
		memcpy(&m_UndoParams->m_OldData.front(), m_pData + nOffset, nOldDataSize * sizeof(DATA));
	}
}

void CHexEditCtrl::EndUndoableTransaction(SIZE_T nNewDataSize)
{
	ASSERT(m_UndoParams->m_nOffset != -1);

	if (nNewDataSize)
	{
		m_UndoParams->m_NewData.resize(nNewDataSize);
		memcpy(&m_UndoParams->m_NewData.front(), m_pData + m_UndoParams->m_nOffset, nNewDataSize * sizeof(DATA));
	}

	ASSERT(m_UndoParams->m_NewData.size() != 0 || m_UndoParams->m_OldData.size() != 0);

	m_UndoParams->m_NewCursorPos = m_CursorPos;

#if (_AXL_VER >= 0x0200)
	axl::call::CSillyThisCallT<CHexEditCtrl> Undo(this, &CHexEditCtrl::UndoImpl, NULL);
	axl::call::CSillyThisCallT<CHexEditCtrl> Redo(this, &CHexEditCtrl::RedoImpl, NULL);
#else
	axl::CSillyThisCallT<CHexEditCtrl> Undo(this, &CHexEditCtrl::UndoImpl, NULL);
	axl::CSillyThisCallT<CHexEditCtrl> Redo(this, &CHexEditCtrl::RedoImpl, NULL);
#endif

	m_UndoRedo.AddUndoableAction(Undo, Redo, m_UndoParams, m_UndoParams->m_OldData.size() + m_UndoParams->m_NewData.size());
	m_UndoParams = NULL;
}

BOOL CHexEditCtrl::ReplaceSelection(PVOID pvData, SIZE_T nSize, BOOL bCursorToEnd)
{ 
	SIZE_T nOffset;
	SIZE_T nDelete; 
	
	if (m_Selection.m_nStartOffset != m_Selection.m_nEndOffset)
	{
		nOffset = m_Selection.m_nStartOffset;
		nDelete = m_Selection.m_nEndOffset - m_Selection.m_nStartOffset;
	}
	else
	{
		nOffset = m_CursorPos.m_nOffset;
		if (m_CursorPos.m_Location != CURSOR_ASCII_0 && m_CursorPos.m_Location != CURSOR_HEX_0 )
			nOffset++;
		nDelete = 0;
	}

	m_Selection.Clear();

	BeginUndoableTransaction(nOffset, nDelete);

	m_UndoParams->m_NewData.resize(nSize);

	BOOL bResult;

	if (!nSize)
	{
		bResult = ReplaceData(nOffset, nDelete, NULL, 0, bCursorToEnd);
		EndUndoableTransaction(0);
		return bResult;
	}

	DATA* pDst = &m_UndoParams->m_NewData.front();
	BYTE* pSrc = (BYTE*) pvData;
	BYTE* pEnd = pSrc + nSize;

	for (; pSrc < pEnd; pSrc++, pDst++)
	{
		pDst->m_Flags = DF_MODIFIED;
		pDst->m_Data = *pSrc;
	}

	bResult = ReplaceData(nOffset, nDelete, &m_UndoParams->m_NewData.front(), nSize, bCursorToEnd);
	EndUndoableTransaction(0);
	return bResult;
}

void CHexEditCtrl::Copy()
{
	SIZE_T nSize = m_Selection.m_nEndOffset - m_Selection.m_nStartOffset;

	if (!nSize)
		return;

	BOOL bResult = OpenClipboard();
	if (!bResult)
		return;

	BOOL bAsciiFormat = m_CursorPos.m_Location >= CURSOR_ASCII_0;
	
	if (!bAsciiFormat)
		nSize *= 3;

	HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, nSize + 1);

	if (!hData)
	{
		CloseClipboard();
		return;
	}

	PBYTE pDst = (PBYTE) GlobalLock(hData);
	ASSERT(pDst);

	pDst[nSize] = 0;

	DATA* pSrc = m_pData + m_Selection.m_nStartOffset;
	DATA* pEnd = m_pData + m_Selection.m_nEndOffset;

	if (bAsciiFormat)
	{
		for (; pSrc < pEnd; pSrc++, pDst++)
			*pDst = pSrc->m_Data;
	}
	else
	{
		for (; pSrc < pEnd; pSrc++)		
		{
			*pDst++ = GetHexChar(pSrc->m_Data >> 4);
			*pDst++ = GetHexChar(pSrc->m_Data);
			*pDst++ = ' ';
		}
	}

	GlobalUnlock(hData);

	EmptyClipboard();
	SetClipboardData(CF_TEXT, hData);
	CloseClipboard();
}

void CHexEditCtrl::Cut()
{
	if (m_Selection.IsEmpty())
		return;

	Copy();
	ReplaceSelection(NULL, 0);
}

static SIZE_T ScanHexString(
	PBYTE pDst, SIZE_T nDstBufferSize, 
	PBYTE pSrc, SIZE_T nSrcBufferSize
	)
{
	SIZE_T nActualSize = 0;

	PBYTE pEnd = pSrc + nSrcBufferSize;
	
	enum STATE
	{
		STATE_IDLE = 0,
		STATE_SCAN,
	} State = STATE_IDLE;

	for(; pSrc < pEnd; pSrc++)
	{
		if (isspace(*pSrc))
		{
			if (State == STATE_SCAN)
			{
				pDst++;
				State = STATE_IDLE;
			}

			continue;
		}

		int x = GetHex(*pSrc);
		if (x == -1) 
			break;

		if (State == STATE_SCAN)
		{
			*pDst = ((*pDst << 4) & 0xf0) | (x & 0xf);
			pDst++;
			State = STATE_IDLE;
		}
		else 
		{
			if (nActualSize + 1 > nDstBufferSize)
				break;

			nActualSize++;
			*pDst = (x & 0xf);
			State = STATE_SCAN;
		}
	}

	return nActualSize;
}

void CHexEditCtrl::Paste()
{
	BOOL bResult = OpenClipboard();
	if (!bResult)
		return;

	HANDLE hData = GetClipboardData(CF_TEXT);

	if (!hData)
	{
		CloseClipboard();
		return;
	}

	SIZE_T nSize = GlobalSize(hData);
	if (!nSize)
	{
		CloseClipboard();
		return;
	}

	BOOL bAsciiFormat = m_CursorPos.m_Location >= CURSOR_ASCII_0;

	PBYTE pData = (PBYTE) GlobalLock(hData);
	ASSERT(pData);

	nSize = strlen((CHAR*) pData);
	
	if (bAsciiFormat)
	{
		ReplaceSelection(pData, nSize);
	}
	else
	{
		SIZE_T nAllocSize = nSize / 2; // no more than this (probably less)
		PBYTE pActualData = (PBYTE) malloc(nAllocSize); 
		if (!pActualData)
		{
			GlobalUnlock(hData);
			CloseClipboard();
			return;
		}

		SIZE_T nActualSize = ScanHexString(pActualData, nAllocSize, pData, nSize);
		ReplaceSelection(pActualData, nActualSize);

		free(pActualData);
	}
	
	GlobalUnlock(pData);

	CloseClipboard();
}

void CHexEditCtrl::SetFindPattern(PVOID p, SIZE_T nSize, BOOL bMatchCase)
{
	m_FindPattern.resize(nSize);
	memcpy(&m_FindPattern.front(), p, nSize);

	SIZE_T i;
	for (i = 0; i < 256; i++)
	{
		m_FindNextBadCharTable[i] = nSize;
		m_FindPrevBadCharTable[i] = nSize;
	}

	SIZE_T m = nSize - 1;

	BYTE* pData = (BYTE*) p;

	if (bMatchCase)
	{
		for (i = 0; i < m; i++) 
			m_FindNextBadCharTable[pData[i]] = m - i;

		for (i = m; i > 0; i--) 
			m_FindPrevBadCharTable[pData[i]] = i;
	}
	else
	{
		for (i = 0; i < m; i++) 
		{
			m_FindNextBadCharTable[tolower(pData[i])] = m - i;
			m_FindNextBadCharTable[toupper(pData[i])] = m - i;
		}

		for (i = m; i > 0; i--) 
		{
			m_FindPrevBadCharTable[tolower(pData[i])] = i;
			m_FindPrevBadCharTable[toupper(pData[i])] = i;
		}
	}

	m_bFindMatchCase = bMatchCase;
}

inline BOOL IsMatchChar(BYTE b1, BYTE b2, BOOL bMatchCase)
	{ return bMatchCase ? b1 == b2 : toupper(b1) == toupper(b2);}

BOOL CHexEditCtrl::IsMatch(DATA* pData, BYTE* pPattern, SIZE_T nSize, BOOL bMatchCase)
{
	BYTE* p = pPattern;
	BYTE* pEnd = p + nSize;

	for (; p != pEnd; p++, pData++)
		if (!IsMatchChar(*p, pData->m_Data, bMatchCase))
			return FALSE;

	return TRUE;
}		

void CHexEditCtrl::SelectRange(SIZE_T nOffset, SIZE_T nSize, int Flags)
{
	CURSOR_POS Pos1, Pos2;
	Pos1.m_nOffset = (long) nOffset;
	Pos2.m_nOffset = (long) (nOffset + nSize - 1);
	
	if (Flags & SRF_CURSOR_TO_HEX)
	{
		Pos1.m_Location = CURSOR_HEX_0;
		Pos2.m_Location = CURSOR_HEX_2;
	}
	else
	{
		Pos1.m_Location = CURSOR_ASCII_0;
		Pos2.m_Location = CURSOR_ASCII_1;
	}

	if (Flags & SRF_CURSOR_TO_BEGIN)
	{
		SetCursorPos(Pos2);
		SetCursorPos(Pos1, MCF_SELECT);
	}
	else
	{
		SetCursorPos(Pos1);
		SetCursorPos(Pos2, MCF_SELECT);
	}
}

BOOL CHexEditCtrl::FindNext()
{
	SIZE_T nSize = m_FindPattern.size();
	SIZE_T m = nSize - 1;

	BYTE Last = m_FindPattern[m];

	DATA* p = m_pData + m_CursorPos.m_nOffset;
	DATA* pEnd = m_pDataEnd - nSize;

	while (p <= pEnd) 
	{
		BYTE b = p[m].m_Data;

		if (IsMatchChar(b, Last, m_bFindMatchCase) && IsMatch(p, &m_FindPattern.front(), m, m_bFindMatchCase))
		{
			int Flags = 0;
			if (m_CursorPos.m_Location < CURSOR_ASCII_0)
				Flags |= SRF_CURSOR_TO_HEX;

			SelectRange(p - m_pData, nSize);
			return TRUE;
		}

		p += m_FindNextBadCharTable[b];
	}

	return FALSE;

}

BOOL CHexEditCtrl::FindPrev()
{
	SIZE_T nSize = m_FindPattern.size();
	SIZE_T m = nSize - 1;

	if ((LONG) nSize > m_CursorPos.m_nOffset)
		return FALSE;

	BYTE First = m_FindPattern[0];

	DATA* p = m_pData + m_CursorPos.m_nOffset - nSize;

	while (p >= m_pData)
	{
		BYTE b = p[0].m_Data;

		if (IsMatchChar(b, First, m_bFindMatchCase) && IsMatch(p, &m_FindPattern.front(), m, m_bFindMatchCase))
		{
			int Flags = SRF_CURSOR_TO_BEGIN;
			if (m_CursorPos.m_Location < CURSOR_ASCII_0)
				Flags |= SRF_CURSOR_TO_HEX;

			SelectRange(p - m_pData, nSize);
			return TRUE;
		}

		p -= m_FindPrevBadCharTable[b];
	}

	return FALSE;
}
