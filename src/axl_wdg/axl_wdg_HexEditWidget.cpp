#include "pch.h"
#include "HexEditCtrlEx.h"
#include "GuiUtils.h"

//.............................................................................

// inline static void FastRectangle(HDC hdc, const RECT& rc)
//	{ ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL); }

inline static void fastChar(HDC hdc, const RECT& rc, TCHAR ch)
	{ extTextOut(hdc, rc.left, rc.top, ETO_OPAQUE, &rc, &ch, 1, NULL); }

static HFONT createPointFont(int nPointSize, LPCTSTR pszFaceName, BYTE charSet = DEFAULT_CHARSET)
{
	LOGFONT lf = {0};
	_tcsncpy(lf.lfFaceName, pszFaceName, sizeof(lf.lfFaceName) / sizeof(TCHAR));
	lf.lfCharSet = charSet;

	HDC hdc = getDC(NULL);
	
	POINT pt = {0};
	pt.y = getDeviceCaps(hdc, LOGPIXELSY) * nPointSize;
	pt.y /= 720; // 72 points/inch, 10 decipoints/point
	DPtoLP(hdc, &pt, 1);
	
	POINT ptOrg = {0};
	DPtoLP(hdc, &ptOrg, 1);
	lf.lfHeight = -abs(pt.y - ptOrg.y);

	releaseDC(NULL, hdc);

	return createFontIndirect(&lf);
}

static void calcCharSize(SIZE* size, HFONT hFont, TCHAR char)
{
	HDC hdc = getDC(NULL);
	HFONT hOldFont = (HFONT) selectObject(hdc, hFont);
	getTextExtentPoint32(hdc, &char, 1, size);
	selectObject(hdc, hOldFont);
	releaseDC(NULL, hdc);
}

//.............................................................................

CHexEditCtrl::CHexEditCtrl() 
{
	m_hUpdateRgn = createRectRgn(0, 0, 0, 0);
	m_hDefaultFont = createPointFont(100, _T("Courier New"));

	m_hFont = m_hDefaultFont;

	calcCharSize(&m_charSize, m_hFont, '|');

	m_nVisibleLineCount = 0;
	m_nVisibleColCount = 0;

	m_nLineCount = 0;
	m_nFirstVisibleLine = 0;
	m_nFirstVisibleCol = 0;

	m_bAutoAdjustBytesPerLine = TRUE;
	m_nBytesPerLine = 0;
	m_bUpperCase = TRUE;

	m_nMouseWheelSpeed = 2;

	m_offsetWidth = 0;
	
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

	m_stdColorBg.m_flags = CF_FORE_COLOR | CF_BACK_COLOR;
	m_stdColorBg.m_rgbForeColor = getSysColor(COLOR_WINDOWTEXT);
	m_stdColorBg.m_rgbBackColor = getSysColor(COLOR_WINDOW);

	m_stdColorAscii.m_flags = CF_FORE_COLOR | CF_BACK_COLOR;
	m_stdColorAscii.m_rgbForeColor = getSysColor(COLOR_WINDOWTEXT);
	m_stdColorAscii.m_rgbBackColor = RGB (
		getRValue(m_stdColorBg.m_rgbBackColor) / 1.15,
		getGValue(m_stdColorBg.m_rgbBackColor) / 1.1,
		getBValue(m_stdColorBg.m_rgbBackColor)
		);

	m_stdColor = NULL;
	
	m_selectionColor.m_flags = CF_FORE_COLOR | CF_BACK_COLOR;
	m_selectionColor.m_rgbForeColor = getSysColor(COLOR_HIGHLIGHTTEXT);
	m_selectionColor.m_rgbBackColor = getSysColor(COLOR_HIGHLIGHT);
	m_cursorColor.m_flags = CF_BACK_COLOR;

	m_cursorColor.m_rgbBackColor = RGB(
		getRValue(m_stdColorBg.m_rgbBackColor) / 1.3,
		getGValue(m_stdColorBg.m_rgbBackColor) / 1.2,
		getBValue(m_stdColorBg.m_rgbBackColor)
		);

	m_modifiedColor.m_flags = CF_FORE_COLOR;
	m_modifiedColor.m_rgbForeColor = RGB(255, 0, 0);

	m_bSelecting = FALSE;

	m_data = NULL;
	m_dataEnd = NULL;
	m_nDataSize = 0;
	m_nAllocSize = 0;

	m_bIgnoreTab = FALSE;
	m_bIgnoreBackspace = FALSE;

	m_bFindMatchCase = FALSE;
}

CHexEditCtrl::~CHexEditCtrl()
{
	deleteObject(m_hDefaultFont);
	deleteObject(m_hUpdateRgn);

	if (m_data)
		free(m_data);
}

ATL::CWndClassInfo& CHexEditCtrl::getWndClassInfo()
{
	static ATL::CWndClassInfo wc =
	{
		{ 
			sizeof(WNDCLASSEX), 
			CS_DBLCLKS, 
			startWindowProc,
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

void CHexEditCtrl::showOnlyPrintableChars(BOOL bEnable)
{
	if (m_bOnlyPrintableChars == bEnable)
		return;

	m_bOnlyPrintableChars = bEnable;
	invalidate();
}

void CHexEditCtrl::setOffsetWidth(BYTE width)
{
	if (width > 8)
		width = 8;

	if (m_offsetWidth == width)
		return;

	m_offsetWidth = width;

	if (m_bAutoAdjustBytesPerLine)
		autoAdjustBytesPerLine();

	recalcColumns();
	recalcLayout();
	invalidate();
}

void CHexEditCtrl::setReadOnly(BOOL bReadOnly)
{
	if (m_bReadOnly == bReadOnly)
		return;

	m_bReadOnly = bReadOnly;
	invalidate();
}

void CHexEditCtrl::setModified()
{
	m_bModified = TRUE;
	notifyParent(HEN_DATA_CHANGE);
}

LRESULT CHexEditCtrl::notifyParent(UINT nCode, HEN_PARAMS* params)
{
	HEN_PARAMS hdr;

	if (!params)
		params = &hdr;

	params->hwndFrom = m_hWnd;
    params->idFrom = getDlgCtrlID();
    params->code = nCode;

	HWND hwndParent = getParent();
	ASSERT(::IsWindow(hwndParent));
	
	return sendMessage(hwndParent, WM_NOTIFY, (WPARAM) params->idFrom, (LPARAM) params);
}

SIZE_T CHexEditCtrl::getData(PVOID pvBuffer, SIZE_T nBufferSize, SIZE_T nOffset)
{
	if (nOffset >= m_nDataSize)
		return 0;

	SIZE_T nCopySize = m_nDataSize - nOffset;
	if (nCopySize > nBufferSize)
		nCopySize = nBufferSize;

	DATA* data = m_data + nOffset;
	DATA* end = data + nCopySize;
	BYTE* buffer = (BYTE*) pvBuffer;
	for (; data < end; data++, buffer++)
		*buffer = data->m_data;

	return nCopySize;
}

SIZE_T CHexEditCtrl::getSelectedData(PVOID pvBuffer, SIZE_T nBufferSize)
{
	SIZE_T nSelectedDataSize = getSelectedDataSize();

	if (!nSelectedDataSize || nBufferSize < nSelectedDataSize)
		return 0;

	DATA* data = m_data + m_selection.m_nStartOffset;
	DATA* end = m_data + m_selection.m_nEndOffset;
	BYTE* buffer = (BYTE*) pvBuffer;
	for (; data < end; data++, buffer++)
		*buffer = data->m_data;

	return nSelectedDataSize;
}

BOOL CHexEditCtrl::setData(PVOID pvBuffer, SIZE_T nBufferSize, SIZE_T nOffset)
{
	if (nOffset > m_nDataSize)
		return FALSE;

	SIZE_T nDelete = m_nDataSize - nOffset;
	if (nDelete > nBufferSize)
		nDelete = nBufferSize;

	if (!insertDeleteData(nOffset, nBufferSize, nDelete))
		return FALSE;

	DATA* data = m_data + nOffset;
	DATA* end = data + nBufferSize;
	BYTE* buffer = (BYTE*) pvBuffer;
	
	for (; data < end; data++, buffer++)
		data->m_data = *buffer;

	// invalidate has already been called (withing InsertDeleteData)
	return TRUE;
}

BOOL CHexEditCtrl::setDataSize(SIZE_T nDataSize)
{
	if (nDataSize < m_nDataSize)
		return insertDeleteData(nDataSize, 0, m_nDataSize - nDataSize);
	else
		return insertDeleteData(m_nDataSize, nDataSize - m_nDataSize, 0);
}

BOOL CHexEditCtrl::setFont(HFONT hFont)
{
	HDC hdc = getDC();
	HFONT hFontOld = (HFONT) selectObject(hdc, hFont);
	TEXTMETRIC tm;
	getTextMetrics(hdc, &tm);
	selectObject(hdc, hFontOld);
	releaseDC(hdc);

	if (!(tm.tmPitchAndFamily & TMPF_FIXED_PITCH)) // how stupid the name is huh? :))
		return FALSE;

	m_hFont = hFont;
	invalidate();
	return TRUE;
}

BOOL CHexEditCtrl::setUpperCase(BOOL bUpperCase)
{
	m_bUpperCase = bUpperCase;
	invalidate();
	return TRUE;
}

BOOL CHexEditCtrl::setBytesPerLine(int nBytesPerLine)
{
	if (!nBytesPerLine)
	{
		m_bAutoAdjustBytesPerLine = TRUE;
		autoAdjustBytesPerLine();
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
	setBytesPerLineImpl(nBytesPerLine);
	return TRUE;
}

void CHexEditCtrl::recalcColumns()
{
	m_nHexCol = m_offsetWidth ? m_offsetWidth + 2 : 0;
	m_nGapCol = m_nHexCol + m_nBytesPerLine * 3;
	m_nAsciiCol = m_nGapCol + 1;
	m_nSpaceCol = m_nAsciiCol + m_nBytesPerLine;
}

void CHexEditCtrl::setBytesPerLineImpl(int nBytesPerLine)
{
	if (m_nBytesPerLine == nBytesPerLine)
		return;

	m_nBytesPerLine = nBytesPerLine;

	m_nLineCount = (int) m_nDataSize / m_nBytesPerLine;
	if (m_nDataSize % m_nBytesPerLine)
		m_nLineCount++;

	recalcColumns();
	invalidate();
	updateCaretPos();
	recalcLayout();
}

CHexEditCtrl::CURSOR_POS CHexEditCtrl::setCursorPos(CURSOR_POS pos, int flags)
{
	ensureVisible(pos);

	if (pos.m_location >= CURSOR_LOCATION_COUNT)
        pos.m_location = CURSOR_LOCATION_COUNT - 1;

	if (m_bOverwriteMode && pos.m_nOffset != (int) m_nDataSize - 1) // in overwrite mode certain locations are unavailable
	{
		if (pos.m_location == CURSOR_ASCII_1)
			pos.m_location = CURSOR_ASCII_0;
		else if (pos.m_location == CURSOR_HEX_2)
			pos.m_location = CURSOR_HEX_1;
	}

	if (pos.m_nOffset >= (int) m_nDataSize)
	{
		pos.m_nOffset = (int) m_nDataSize - 1;
		if (pos.m_location >= CURSOR_ASCII_0)
			pos.m_location = CURSOR_ASCII_1;
		else 
			pos.m_location = CURSOR_HEX_2;
	}

	if (pos.m_nOffset < 0)
	{
		pos.m_nOffset = 0;
		if (pos.m_location >= CURSOR_ASCII_0)
			pos.m_location = CURSOR_ASCII_0;
		else 
			pos.m_location = CURSOR_HEX_0;
	}

	if ((flags & MCF_FORCE) == 0 &&
		m_cursorPos.m_nOffset == pos.m_nOffset && 
		m_cursorPos.m_location == pos.m_location)
		return pos;

	if (m_cursorPos.m_nOffset != pos.m_nOffset || m_cursorPos.isHex() && pos.isAscii())
		finalize4Bits();

	if (m_cursorPos.m_nOffset != pos.m_nOffset)
	{
		invalidateOffset(m_cursorPos.m_nOffset);
		invalidateOffset(pos.m_nOffset);
	}

	if (flags & MCF_SELECT)
	{
		if (!m_bSelecting)
		{
			m_bSelecting = TRUE;
			m_selStart = m_cursorPos;		
		}
	}
	else if (!(flags & MCF_KEEP_SELECTION))
	{
		killSelection();
	}

	m_cursorPos = pos;
	updateCaretPos();

	if (flags & MCF_SELECT)
		setSelEnd(pos);

	return pos;
}

BOOL CHexEditCtrl::isEqualCursorPos(CURSOR_POS& pos1, CURSOR_POS& pos2)
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
		pos1.m_nOffset == pos2.m_nOffset && 
		_EqualityTable1[pos1.m_location][pos2.m_location] ||

		pos1.m_nOffset + 1 == pos2.m_nOffset && 
		_EqualityTable2[pos1.m_location][pos2.m_location] ||

		pos2.m_nOffset + 1 == pos1.m_nOffset && 
		_EqualityTable2[pos2.m_location][pos1.m_location];
}

void CHexEditCtrl::setSelEnd(CURSOR_POS pos)
{
	SELECTION_INFO oldSelection = m_selection;

	m_selEnd = pos;

	if (isEqualCursorPos(pos, m_selStart)) // "no selection" shortcut
	{
		m_selection.m_nStartOffset = 0;
		m_selection.m_nEndOffset = 0;
		m_selection.m_bLeadingGap = FALSE;
		m_selection.m_bTrailingGap = FALSE;
		invalidateSelection(&oldSelection);
		return;
	}

	if (m_selStart.m_nOffset == m_selEnd.m_nOffset) // "single char selection" shortcut
	{
		m_selection.m_nStartOffset = m_selStart.m_nOffset;
		m_selection.m_nEndOffset = m_selEnd.m_nOffset + 1;
		m_selection.m_bLeadingGap = FALSE;
		m_selection.m_bTrailingGap = FALSE;
		invalidateSelectionDiff(&m_selection, &oldSelection);
		return;
	}

	// ok, here's the tricky part -- find out ACTUAL selection range

	// first, arrange SelStart and SelEnd according to offsets

	CURSOR_POS selStart;
	CURSOR_POS selEnd;

	if (m_selStart.m_nOffset < m_selEnd.m_nOffset)
	{
		selStart = m_selStart;
		selEnd = m_selEnd;
	}
	else
	{
		selStart = m_selEnd;
		selEnd = m_selStart;
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

	BYTE selectionFlags = _SelectionTable[selStart.m_location][selEnd.m_location];

	if (m_bOverwriteMode) 		
	{
		// in overwrite mode end position is always included and no trailing gap
		selectionFlags |= SF_E;
		selectionFlags &= ~SF_T;
	}

	if (selectionFlags & SF_S)
		m_selection.m_nStartOffset = selStart.m_nOffset;
	else
		m_selection.m_nStartOffset = selStart.m_nOffset + 1;

	if (selectionFlags & SF_E)
		m_selection.m_nEndOffset = selEnd.m_nOffset + 1;
	else
		m_selection.m_nEndOffset = selEnd.m_nOffset;

	m_selection.m_bLeadingGap = (selectionFlags & SF_L) != 0;
	m_selection.m_bTrailingGap = (selectionFlags & SF_T) != 0;

	invalidateSelectionDiff(&m_selection, &oldSelection);
}

BOOL CHexEditCtrl::isDataSelected(SIZE_T nOffset)
{ 
	return 
		nOffset >= m_selection.m_nStartOffset && 
		nOffset < m_selection.m_nEndOffset; 
}

BOOL CHexEditCtrl::isGapSelected(SIZE_T nOffset)
{
	return 
		nOffset == m_selection.m_nStartOffset - 1 && m_selection.m_bLeadingGap ||
		nOffset == m_selection.m_nEndOffset - 1 && m_selection.m_bTrailingGap;
}

void CHexEditCtrl::invalidateSelectionDiff(SELECTION_INFO* old, SELECTION_INFO* new)
{
	invalidateSelection(old);
	invalidateSelection(new);
}

void CHexEditCtrl::invalidateSelection(SELECTION_INFO* sel)
{
	if (sel->isEmpty())
		return;

	SIZE_T nOffsetFrom = sel->m_nStartOffset;
	SIZE_T nOffsetTo = sel->m_nEndOffset;

	if (sel->m_bLeadingGap && nOffsetFrom)
		nOffsetFrom--;

	invalidateRange(nOffsetFrom, nOffsetTo);
}

void CHexEditCtrl::invalidateRange(SIZE_T nOffsetFrom, SIZE_T nOffsetTo)
{
	int nLineFrom = (int) nOffsetFrom / m_nBytesPerLine;
	int nLineTo = (int) nOffsetTo / m_nBytesPerLine;

	if (nLineFrom == nLineTo)
	{
		int nFirstCol = (int) nOffsetFrom % m_nBytesPerLine * 3 + m_nHexCol;
		int nLastCol = (int) nOffsetTo % m_nBytesPerLine + m_nAsciiCol;
		
		invalidateHelper(nLineFrom,  nFirstCol, 1, nLastCol - nFirstCol);
	}
	else
	{
		invalidateHelper(
			nLineFrom, m_nHexCol, 
			nLineTo - nLineFrom + 1, m_nSpaceCol - m_nHexCol + 1);
	}
}

void CHexEditCtrl::invalidateOffset(SIZE_T nOffset)
{
	int nLine = (int) nOffset / m_nBytesPerLine;
	int nCol = (int) nOffset % m_nBytesPerLine;
	
	invalidateHelper(nLine, nCol * 3 + m_nHexCol, 1, 3);
	invalidateHelper(nLine, nCol + m_nAsciiCol, 1, 1);
}

void CHexEditCtrl::invalidateHelper(int nLine, int nCol, int nLineCount, int nColCount)
{
	nLine -= m_nFirstVisibleLine;
	nCol -= m_nFirstVisibleCol;

	RECT rct = 
	{ 
		nCol * m_charSize.cx, 
		nLine * m_charSize.cy,  
		(nCol + nColCount) * m_charSize.cx, 
		(nLine + nLineCount) * m_charSize.cy,  
	};

	invalidateRect(&rct);
}

void CHexEditCtrl::ensureVisible(CURSOR_POS pos)
{
	int nLine, nCol;
	getCursorLineCol(pos, nLine, nCol, FALSE);

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

	int nColSpan = pos.m_location >= CURSOR_ASCII_0 ? 1 : 2;

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

	fixupFirstVisibleLineCol();
	recalcScrollBars();

	if (bInvalidate)
		invalidate();
}

BOOL CHexEditCtrl::insertDeleteData(SIZE_T nOffset, SIZE_T nInsert, SIZE_T nDelete)
{
	ASSERT(nOffset <= m_nDataSize);
	ASSERT(nOffset + nDelete <= m_nDataSize);

	SIZE_T nNewSize = m_nDataSize + nInsert - nDelete;
	SIZE_T nLeftOver = m_nDataSize - nOffset - nDelete;

	if (nNewSize <= m_nAllocSize)
	{
		if (nLeftOver)
			memmove(
				m_data + nOffset + nInsert, 
				m_data + nOffset + nDelete,
				nLeftOver * sizeof(DATA)
				);
	}
	else
	{
		// reserve at least one more page in buffer
		SIZE_T nNewAllocSize = nNewSize + 2 * BUFFER_PAGE_SIZE - nNewSize % BUFFER_PAGE_SIZE;
		DATA* newData = (DATA*) malloc(nNewAllocSize * sizeof(DATA));
		if (!newData)
			return FALSE;

		if (m_data) 
		{
			// copy old data
			memcpy(newData, m_data, nOffset * sizeof(DATA));
			
			if (nLeftOver)
				memcpy(
					newData + nOffset + nInsert, 
					m_data + nOffset + nDelete, 
					nLeftOver * sizeof(DATA)
					);

			free(m_data);
		}

        m_data = newData;
		m_nAllocSize = nNewAllocSize;
	}

	if (nInsert) // zero out inserted memory
	{
		memset(m_data + nOffset, 0, nInsert * sizeof(DATA));
		DATA* data = m_data + nOffset;
		DATA* end = data + nInsert;

		for (; data < end; data++)
			data->m_flags |= DF_MODIFIED;
	}

	int nNewLineCount = (int) nNewSize / m_nBytesPerLine;
	if (nNewSize % m_nBytesPerLine)
		nNewLineCount++;

	int nInvalidateFromLine = (int) nOffset / m_nBytesPerLine;
	int nInvalidateToLine = max(m_nLineCount, nNewLineCount);

	m_dataEnd = m_data + nNewSize;
	m_nDataSize = nNewSize;
	m_nLineCount = nNewLineCount;

	RECT rct = 
	{ 
		0, 
		(nInvalidateFromLine - m_nFirstVisibleLine) * m_charSize.cy,
		(m_nSpaceCol - m_nFirstVisibleCol) * m_charSize.cx,
		(nInvalidateToLine - m_nFirstVisibleLine) * m_charSize.cy
	};

	invalidateRect(&rct);
	recalcLayout();

	return TRUE;
}

void CHexEditCtrl::transitCursor(int nOffsetDelta, int newLocation, int flags)
{
	CURSOR_POS newPos;
	newPos.m_nOffset = m_cursorPos.m_nOffset + nOffsetDelta;
	newPos.m_location = newLocation != -1 ? 
		newLocation : 
		m_cursorPos.m_location;

	setCursorPos(newPos, flags);
}

void CHexEditCtrl::killSelection()
{
	SELECTION_INFO oldSelection = m_selection;

	m_selStart.m_nOffset = 0;
	m_selStart.m_location = 0;
	m_selEnd.m_nOffset = 0;
	m_selEnd.m_location = 0;
	
	m_selection.clear();

	m_bSelecting = FALSE;
	
	invalidateSelection(&oldSelection);
}

void CHexEditCtrl::finalize4Bits()
{
	DATA* data = m_data + m_cursorPos.m_nOffset;
	if (data < m_dataEnd && data->m_flags & DF_4_BITS)
	{
		data->m_flags &= ~DF_4_BITS;
		invalidateOffset(m_cursorPos.m_nOffset);
	}
}

BOOL CHexEditCtrl::setOverwriteMode(BOOL bOverwriteMode)
{
	m_bOverwriteMode = bOverwriteMode;
	updateCaret();
	return TRUE;
}

CHexEditCtrl::CURSOR_POS CHexEditCtrl::cursorPosFromMousePos(POINT pt)
{
	int nLine = pt.y / m_charSize.cy;
	int nCol = pt.x / m_charSize.cx;

	nLine += m_nFirstVisibleLine;
	nCol += m_nFirstVisibleCol;

	if (nLine < 0)
		nLine = 0;

	CURSOR_POS pos;

	if (nCol >= m_nGapCol)
	{
		nCol = max(nCol, m_nAsciiCol);
		nCol -= m_nAsciiCol;

		pos.m_location = CURSOR_ASCII_0;

		if (nCol >= m_nBytesPerLine)
		{
			nCol = m_nBytesPerLine - 1;
			pos.m_location = CURSOR_ASCII_1;
		}
	}
	else 
	{
		nCol = max(nCol, m_nHexCol);
		nCol -= m_nHexCol; 
		
		pos.m_location = CURSOR_HEX_0 + nCol % 3;

		nCol /= 3;

		if (nCol >= m_nBytesPerLine)
		{
			nCol = m_nBytesPerLine - 1;
			pos.m_location = CURSOR_HEX_2;
		}
	}

	pos.m_nOffset = nLine * m_nBytesPerLine + nCol;
	return pos;
}

void CHexEditCtrl::updateCaret()
{
	if (getFocus() != m_hWnd)
		return;

	int cx = m_bOverwriteMode ? m_charSize.cx : 2;

	if (createSolidCaret(cx, m_charSize.cy))
	{
		updateCaretPos();
		showCaret();
	}
}

void CHexEditCtrl::getCursorLineCol(CURSOR_POS pos, int& nLine, int& nCol, BOOL bAddExtra)
{
	nLine = (int) pos.m_nOffset / m_nBytesPerLine;
	nCol = (int) pos.m_nOffset % m_nBytesPerLine;

	if (pos.m_location >= CURSOR_ASCII_0)
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
		nCol += _ExtraCol[pos.m_location];
	}
}

void CHexEditCtrl::updateCaretPos()
{
	int nLine, nCol;

	getCursorLineCol(m_cursorPos, nLine, nCol);

	nLine -= m_nFirstVisibleLine;
	nCol -= m_nFirstVisibleCol;

	if (m_hWnd == getFocus())
		setCaretPos(nCol * m_charSize.cx, nLine * m_charSize.cy);
}

void CHexEditCtrl::clearModified()
{
	DATA* data = m_data;
	for (; data < m_dataEnd; data++)
		data->m_flags &= ~DF_MODIFIED;

	m_bModified = FALSE;
	invalidate();
}

void CHexEditCtrl::paintRect(HDC hdc, RECT& rctUpdate)
{
	RECT rct;
	rct.left = rctUpdate.left - rctUpdate.left % m_charSize.cx;
	rct.top = rctUpdate.top - rctUpdate.top % m_charSize.cy;
	rct.right = rct.left + m_charSize.cx;
	rct.bottom = rct.top + m_charSize.cy;

	int nFirstLine = rctUpdate.top / m_charSize.cy;
	int nLineCount = rctUpdate.bottom / m_charSize.cy - nFirstLine;
	if (rctUpdate.bottom % m_charSize.cy)
		nLineCount++;

	int nFirstCol = rctUpdate.left / m_charSize.cx;
	int nColCount = rctUpdate.right / m_charSize.cx - nFirstCol;
	if (rctUpdate.right % m_charSize.cx)
		nColCount++;

	nFirstLine += m_nFirstVisibleLine;
	nFirstCol += m_nFirstVisibleCol;

	setBkColor(hdc, m_stdColorBg.m_rgbBackColor);
	m_stdColor = &m_stdColorBg;

	int nTotalLineCount = m_nLineCount;
	if (!nTotalLineCount)
		nTotalLineCount = 1; // draw at least one

	if (nFirstLine >= nTotalLineCount) // shortcut
	{
		fastRectangle(hdc, rctUpdate);
		return;
	}

	if (nFirstLine + nLineCount >= nTotalLineCount)
		nLineCount = nTotalLineCount - nFirstLine;

	int nVisibilityFlags = 0;

	SIZE_T nOffset = nFirstLine * m_nBytesPerLine;

	if (m_offsetWidth && nFirstCol < m_offsetWidth)
	{
		int m = nFirstCol;
		if (m)
		{
			nFirstCol -= m;
			nColCount += m;
			rct.left -= m * m_charSize.cx;
			rct.right -= m * m_charSize.cx;
		}

		int nPaintColCount = m_nHexCol - nFirstCol;

		paintOffs(hdc, rct, nLineCount, nOffset);

		nFirstCol += nPaintColCount;
		nColCount -= nPaintColCount;

		int nPaintWidth = nPaintColCount * m_charSize.cx;
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
			rct.left -= m * m_charSize.cx;
			rct.right -= m * m_charSize.cx;
		}

		int nPaintColCount = m_nGapCol - nFirstCol;
		if (nPaintColCount > nColCount)
		{		
			nPaintColCount = nColCount;
			m = nColCount % 3;
			if (m)
				nPaintColCount += 3 - m;
		}

		paintHex(hdc, rct, nPaintColCount / 3, nLineCount, nOffset + (nFirstCol - m_nHexCol) / 3);

		nFirstCol += nPaintColCount;
		nColCount -= nPaintColCount;

		int nPaintWidth = nPaintColCount * m_charSize.cx;
		rct.left += nPaintWidth;
		rct.right += nPaintWidth;
	}

	if (nFirstCol < m_nAsciiCol &&
		nFirstCol + nColCount >= m_nGapCol)
	{
		paintGap(hdc, rct, nLineCount);

		nFirstCol++;
		nColCount--;

		rct.left += m_charSize.cx;
		rct.right += m_charSize.cx;
	}

	if (nFirstCol < m_nSpaceCol &&
		nFirstCol + nColCount >= m_nAsciiCol)
	{
		int nPaintColCount = m_nSpaceCol - nFirstCol;
		if (nPaintColCount > nColCount)
			nPaintColCount = nColCount;

		m_stdColor = &m_stdColorAscii;
		setBkColor(hdc, m_stdColorAscii.m_rgbBackColor);

		paintAscii(hdc, rct, nPaintColCount, nLineCount, nOffset + (nFirstCol - m_nAsciiCol));

		nFirstCol += nPaintColCount;
		nColCount -= nPaintColCount;

		int nPaintWidth = nPaintColCount * m_charSize.cx;
		rct.left += nPaintWidth;
		rct.right += nPaintWidth;

		m_stdColor = &m_stdColorBg;
		setBkColor(hdc, m_stdColorBg.m_rgbBackColor);
	}

	int nBottom = rct.top + nLineCount * m_charSize.cy;

	if (nColCount > 0)
	{
		rct.right = rctUpdate.right;
		rct.bottom = nBottom;

		fastRectangle(hdc, rct);
	}

	if (rctUpdate.bottom > nBottom)
	{
		rct = rctUpdate;
		rct.top = nBottom;

		fastRectangle(hdc, rct);
	}
}

void CHexEditCtrl::applyColor(HDC hdc, COLOR_INFO* color, int prevColorFlags)
{
	if (!color)
	{
		if (prevColorFlags & CF_BACK_COLOR)
			setBkColor(hdc, m_stdColor->m_rgbBackColor);

		if (prevColorFlags & CF_FORE_COLOR)
			setTextColor(hdc, m_stdColor->m_rgbForeColor);
	}
	else
	{
		if (color->m_flags & CF_BACK_COLOR)
			setBkColor(hdc, color->m_rgbBackColor);
		else if (prevColorFlags & CF_BACK_COLOR)
			setBkColor(hdc, m_stdColor->m_rgbBackColor);

		if (color->m_flags & CF_FORE_COLOR)
			setTextColor(hdc, color->m_rgbForeColor);
		else if (prevColorFlags & CF_FORE_COLOR)
			setTextColor(hdc, m_stdColor->m_rgbForeColor);
	}
}

CHexEditCtrl::COLOR_INFO CHexEditCtrl::getDataColor(DATA* data)
{
	if (!data)
		return COLOR_INFO();

	COLOR_INFO color;

	color += data->m_color;

	if (data->m_flags & DF_MODIFIED)
		color += m_modifiedColor;

	if (m_selection.isEmpty() && data == m_data + m_cursorPos.m_nOffset)
		color += m_cursorColor;

	if (isDataSelected(data - m_data))
		color += m_selectionColor;

	return color;
}

CHAR CHexEditCtrl::getHexChar(BYTE data)
{
	static CHAR _UpperCaseHexChars[] = "0123456789ABCDEF";
	static CHAR _LowerCaseHexChars[] = "0123456789abcdef";
	
	data &= 0xf;

	return m_bUpperCase ? _UpperCaseHexChars[data] : _LowerCaseHexChars[data];
}

void CHexEditCtrl::paintOffs(HDC hdc, RECT rct, int nLineCount, SIZE_T nOffset)
{
	rct.right = m_nHexCol * m_charSize.cx;
	rct.bottom = rct.top + m_charSize.cy;

	COLORREF oldColor = setTextColor(hdc, getSysColor(COLOR_GRAYTEXT));

    for (int i = 0; i < nLineCount; i++)
	{
		BYTE b1 = HIBYTE(HIWORD(nOffset));
		BYTE b2 = LOBYTE(HIWORD(nOffset));
		BYTE b3 = HIBYTE(LOWORD(nOffset));
		BYTE b4 = LOBYTE(LOWORD(nOffset));

		CHAR szText[8] = 
		{ 
			getHexChar(b1 >> 4),
			getHexChar(b1),
			getHexChar(b2 >> 4),
			getHexChar(b2),
			getHexChar(b3 >> 4),
			getHexChar(b3),
			getHexChar(b4 >> 4),
			getHexChar(b4),
		};

		extTextOut(hdc, rct.left, rct.top, ETO_OPAQUE, &rct, szText + 8 - m_offsetWidth, m_offsetWidth, NULL);
		
		rct.top += m_charSize.cy;
		rct.bottom += m_charSize.cy;

		nOffset += m_nBytesPerLine;
	}

	setTextColor(hdc, oldColor);
}

void CHexEditCtrl::paintHex(HDC hdc, RECT rct, int nColCount, int nLineCount, SIZE_T nOffset)
{

	ASSERT(nColCount <= m_nBytesPerLine);

	int left = rct.left;

	DATA* data = m_data + nOffset;
	int nExtra = m_nBytesPerLine - nColCount;

	int prevColorFlags = 0;

    for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nColCount; j++, data++)
		{
			if (data >= m_dataEnd)
			{
				ASSERT(i == nLineCount - 1);
				rct.right = rct.left + (nColCount - j) * m_charSize.cx * 3;
				applyColor(hdc, NULL, prevColorFlags);
				fastRectangle(hdc, rct);
				return;
			}	

			COLOR_INFO color = getDataColor(data);
			applyColor(hdc, &color, prevColorFlags);

			if (data->m_flags & DF_4_BITS)
			{
				fastChar(hdc, rct, getHexChar(data->m_data));
				rct.left += m_charSize.cx;
				rct.right += m_charSize.cx;

				fastRectangle(hdc, rct);
				rct.left += m_charSize.cx;
				rct.right += m_charSize.cx;
			}
			else
			{
				fastChar(hdc, rct, getHexChar(data->m_data >> 4));
				rct.left += m_charSize.cx;
				rct.right += m_charSize.cx;

				fastChar(hdc, rct, getHexChar(data->m_data));
				rct.left += m_charSize.cx;
				rct.right += m_charSize.cx;
			}

			if (isGapSelected(data - m_data))
			{
				// gap shuold be selection color (even though NOT both left and right are selection)
				applyColor(hdc, &m_selectionColor, color.m_flags);
				prevColorFlags = m_selectionColor.m_flags;
			} 
			else
			{
				// color of this gap depend on the next data

				DATA* nextData = data + 1;

				if (nextData < m_dataEnd && getDataColor(nextData) == color)
				{
					// keep color
					prevColorFlags = color.m_flags;
				}
				else
				{
					// reset color
					applyColor(hdc, NULL, color.m_flags);
					prevColorFlags = 0;
				}
			}

			if (m_nHexGapStep > 0 && ((data - m_data) % m_nHexGapStep) == m_nHexGapStep - 1)
			{
				int width = m_charSize.cx * 2;
				RECT rctBigGap = { rct.left, rct.top, rct.left + width, rct.bottom };
				fastRectangle(hdc, rctBigGap);
				rct.left += width;
				rct.right += width;
			}
			else
			{
				fastRectangle(hdc, rct);
				rct.left += m_charSize.cx;
				rct.right += m_charSize.cx;
			}
		}

		rct.left = left;
		rct.right = rct.left + m_charSize.cx;
		rct.top += m_charSize.cy;
		rct.bottom += m_charSize.cy;

		data += nExtra;
	}

	applyColor(hdc, NULL, prevColorFlags);
}

void CHexEditCtrl::paintGap(HDC hdc, RECT rct, int nLineCount)
{
	rct.bottom = rct.top + nLineCount * m_charSize.cy;
	fastRectangle(hdc, rct);
}

void CHexEditCtrl::paintAscii(HDC hdc, RECT rct, int nColCount, int nLineCount, SIZE_T nOffset)
{
	ASSERT(nColCount <= m_nBytesPerLine);

	int left = rct.left;

	DATA* data = m_data + nOffset;
	int nExtra = m_nBytesPerLine - nColCount;

	int prevColorFlags = 0;

    for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nColCount; j++, data++)
		{
			if (data >= m_dataEnd)
			{
				ASSERT(i == nLineCount - 1);
				rct.right = rct.left + (nColCount - j) * m_charSize.cx;
				applyColor(hdc, NULL, prevColorFlags);
				fastRectangle(hdc, rct);
				return;
			}			

			COLOR_INFO color = getDataColor(data);
			applyColor(hdc, &color, prevColorFlags);
			prevColorFlags = color.m_flags;

			CHAR ch = m_bOnlyPrintableChars ? isprint(data->m_data) ? data->m_data : '.' : data->m_data;
			fastChar(hdc, rct, ch);
			rct.left += m_charSize.cx;
			rct.right += m_charSize.cx;
		}

		rct.left = left;
		rct.right = rct.left + m_charSize.cx;
		rct.top += m_charSize.cy;
		rct.bottom += m_charSize.cy;

		data += nExtra;
	}

	applyColor(hdc, NULL, prevColorFlags);
}

void CHexEditCtrl::fixupFirstVisibleLineCol()
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

		invalidate();
		updateCaretPos();
	}
}

void CHexEditCtrl::recalcScrollBars()
{
	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_ALL;

	si.nMin = 0;
    si.nMax = m_nLineCount - 1;
    si.nPage = m_nVisibleLineCount;
	si.nPos = m_nFirstVisibleLine;
	setScrollInfo(SB_VERT, &si);

	si.nMin = 0;
    si.nMax = m_nSpaceCol - 1;
    si.nPage = m_nVisibleColCount;
    si.nPos = m_nFirstVisibleCol;
	setScrollInfo(SB_HORZ, &si);
}

void CHexEditCtrl::recalcLayout()
{
	RECT rctClient;
	getClientRect(&rctClient);

	m_nVisibleLineCount = rctClient.bottom / m_charSize.cy;
	m_nVisibleColCount = rctClient.right / m_charSize.cx;

	fixupFirstVisibleLineCol();	

	recalcScrollBars();
}

LRESULT CHexEditCtrl::onCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	setBytesPerLineImpl(8);
	return 0;
}

LRESULT CHexEditCtrl::onGetDlgCode(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
}

LRESULT CHexEditCtrl::onPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps;
	HDC hdc = beginPaint(&ps);

	int nResult = getClipRgn(hdc, m_hUpdateRgn);

	if (nResult == NULLREGION)
	{
		endPaint(&ps);
		return 0;
	}

	BYTE buffer[sizeof(RGNDATA) + 2 * sizeof(RECT)];
	RGNDATA* rgnData = (RGNDATA*) buffer;
	RECT* rects = (RECT*) rgnData->Buffer;

	HFONT hOldFont = (HFONT) selectObject(hdc, m_hFont);

	nResult = getRegionData(m_hUpdateRgn, sizeof(buffer), rgnData);
	if (!nResult || rgnData->rdh.nCount != 2) // not enough
	{
		paintRect(hdc, ps.rcPaint);
	}
	else
	{
		paintRect(hdc, rects[0]);
		paintRect(hdc, rects[1]);
	}

	selectObject(hdc, hOldFont);

	endPaint(&ps);
	return 0;
}

/*

LRESULT CHexEditCtrl::onPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nResult = getUpdateRgn(m_hUpdateRgn);

	PAINTSTRUCT ps;
	if (nResult == NULLREGION)
	{
		beginPaint(&ps);
		endPaint(&ps);
		return 0;
	}

	BYTE buffer[sizeof(RGNDATA) + 2 * sizeof(RECT)];
	RGNDATA* rgnData = (RGNDATA*) buffer;
	RECT* rects = (RECT*) rgnData->Buffer;
	UINT nCount = 0;

	nResult = getRegionData(m_hUpdateRgn, sizeof(buffer), rgnData);
	if (nResult == 0 || nResult > sizeof(buffer)) // not enough
	{
		getUpdateRect(&rects[0]);
		nCount = 1;
	}
	else if (rgnData->rdh.nCount != 2)
	{
		rects[0] = rgnData->rdh.rcBound;
		nCount = 1;
	}
	else
	{
		nCount = 2;
	}

	HDC hdc = beginPaint(&ps);

	for (UINT i = 0; i < nCount; i++)
	{
#ifdef _HEXEDIT_USE_OFFSCREEN_BUFFER
		paint(m_hOffscreenDC, rects[i]);
		bitBlt(
			hdc, 
			ps.rcPaint.left, ps.rcPaint.top, 
			ps.rcPaint.right - ps.rcPaint.left, 
			ps.rcPaint.bottom - ps.rcPaint.top, 
			m_hOffscreenDC,
			ps.rcPaint.left, ps.rcPaint.top, 
			SRCCOPY
			);
#else
		paint(hdc, rects[i]); // paint direct
#endif
	}

#ifdef _HEXEDIT_SHOW_ACTUAL_REPAINT_AREAS 
	int y = m_clientSize.cy / 2;

	setViewportOrgEx(hdc, 0, y, NULL);

	RECT rct = { 0, 0, m_clientSize.cx, y }; 
	fillRect(hdc, &rct, (HBRUSH) getStockObject(GRAY_BRUSH));

	for (UINT i = 0; i < nCount; i++)
		paint(hdc, rects[i]); // paint direct
#endif

	endPaint(&ps);
	return 0;
}

*/

LRESULT CHexEditCtrl::onSetFocus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	updateCaret();
	return 0;
}

LRESULT CHexEditCtrl::onKillFocus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	finalize4Bits();
	destroyCaret();
	return 0;
}

int CHexEditCtrl::autoAdjustBytesPerLine()
{
	static int _BytesPerLine[] = { 32, 16, 8, 4 };

	int nDelta = m_offsetWidth + 1;

	for (int i = 0; i < sizeof(_BytesPerLine) / sizeof(_BytesPerLine[0]); i++)
		if (m_nVisibleColCount >= _BytesPerLine[i] * 4 + nDelta)
		{
			setBytesPerLineImpl(_BytesPerLine[i]);
			return _BytesPerLine[i];
		}

	setBytesPerLineImpl(2);
	return 2;
}

LRESULT CHexEditCtrl::onSize(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	recalcLayout();

	if (m_bAutoAdjustBytesPerLine)
		autoAdjustBytesPerLine();

	return 0;
}

LRESULT CHexEditCtrl::onScroll(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nBar = nMsg == WM_VSCROLL ? SB_VERT : SB_HORZ;

	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_ALL;
	getScrollInfo(nBar, &si);

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
		setScrollInfo(nBar, &si);

		invalidate();
		updateCaretPos();
	}

	return 0;
}

LRESULT CHexEditCtrl::onMouseWheel(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		onScroll(WM_VSCROLL, wParamScroll, 0, bHandled);

	return 0;
}

LRESULT CHexEditCtrl::onLButtonDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { (SHORT) LOWORD(lParam), (SHORT) HIWORD(lParam) };
	CURSOR_POS pos = cursorPosFromMousePos(pt);
	BOOL bShift = (getAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

	setFocus();
	setCapture();
	setCursorPos(pos, bShift);

	if (!bShift)
	{
		m_bSelecting = TRUE;
		m_selStart = m_selEnd = m_cursorPos;
	}

	m_bDragging = TRUE;

	return 0;
}

LRESULT CHexEditCtrl::onLButtonUp(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bDragging = FALSE;
	releaseCapture();
	return 0;
}

LRESULT CHexEditCtrl::onCaptureChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bDragging = FALSE;
	return 0;
}

LRESULT CHexEditCtrl::onMouseMove(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bDragging)
		return 0;

	POINT pt = { (SHORT) LOWORD(lParam), (SHORT) HIWORD(lParam) };
	CURSOR_POS pos = cursorPosFromMousePos(pt);

	if (m_selStart.m_location < CURSOR_ASCII_0 && pos.m_location >= CURSOR_ASCII_0)
	{
		pos.m_nOffset += m_nBytesPerLine - pos.m_nOffset % m_nBytesPerLine - 1;
		pos.m_location = CURSOR_HEX_2;
	}
	else if (m_selStart.m_location >= CURSOR_ASCII_0 && pos.m_location < CURSOR_ASCII_0)
	{
		pos.m_nOffset -= pos.m_nOffset % m_nBytesPerLine;
		pos.m_location = CURSOR_ASCII_0;
	}

	setCursorPos(pos, MCF_SELECT);

	return 0;
}

LRESULT CHexEditCtrl::onSysKeyDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bAlt = (getAsyncKeyState(VK_MENU) & 0x8000) != 0;

	switch (wParam)
	{
	case VK_BACK:
		if (bAlt)
		{
			m_bIgnoreBackspace = TRUE;
			undo();
		};
		break;

	default:
		bHandled = FALSE;
		break;
	};


	return 0;
}

LRESULT CHexEditCtrl::onKeyDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bShift = (getAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	BOOL bCtrl = (getAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	switch (wParam)
	{
	case VK_TAB:
		m_bIgnoreTab = TRUE;
		onKeyTab();
		break;

	case VK_LEFT:
		onKeyLeft(bShift, bCtrl);
		break;

	case VK_RIGHT:
		onKeyRight(bShift, bCtrl);
		break;

	case VK_UP:
		onKeyUp(bShift, bCtrl);
		break;

	case VK_DOWN:
		onKeyDown(bShift, bCtrl);
		break;

	case VK_PRIOR:
		onKeyPageUp(bShift, bCtrl);
		break;

	case VK_NEXT:
		onKeyPageDown(bShift, bCtrl);
		break;

	case VK_HOME:
		onKeyHome(bShift, bCtrl);
		break;

	case VK_END:
		onKeyEnd(bShift, bCtrl);
		break;

	case VK_DELETE:
		if (bShift)
			cut();
		else
			onKeyDelete();
		break;

	case VK_BACK:
		m_bIgnoreBackspace = TRUE;
		onKeyBackspace();
		break;

	case VK_INSERT:
		if (bCtrl)
			copy();
		else if (bShift)
			paste();
		else
			setOverwriteMode(!m_bOverwriteMode);

		break;
	};

	return 0;
}

void CHexEditCtrl::onKeyTab()
{
	static BYTE _TransitionTable[] = 
	{
		CURSOR_ASCII_0, // from CURSOR_HEX_0
		CURSOR_ASCII_1, // from CURSOR_HEX_1
		CURSOR_ASCII_1, // from CURSOR_HEX_2

		CURSOR_HEX_0, // from CURSOR_ASCII_0
		CURSOR_HEX_2, // from CURSOR_ASCII_1
	};
	
	BYTE newLocation = _TransitionTable[m_cursorPos.m_location];
	transitCursor(0, newLocation, MCF_KEEP_SELECTION);
}

void CHexEditCtrl::onKeyLeft(BOOL bShift, BOOL bCtrl)
{
	// begin of ASCII line -- special case
	if (m_cursorPos.m_location == CURSOR_ASCII_0 && 
		m_cursorPos.m_nOffset % m_nBytesPerLine == 0)
	{
		transitCursor(-1, CURSOR_ASCII_1, bShift ? MCF_SELECT : 0);
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

	CURSOR_TRANSITION transition = _TransitionTable[m_cursorPos.m_location];

	if (m_bOverwriteMode && transition.m_newLocation == CURSOR_HEX_2)
		transition.m_newLocation = CURSOR_HEX_1;

	transitCursor(&transition, bShift ? MCF_SELECT : 0);
}

void CHexEditCtrl::onKeyRight(BOOL bShift, BOOL bCtrl)
{
	// end of ASCII line -- special case
	if (m_cursorPos.m_location == CURSOR_ASCII_1 && 
		m_cursorPos.m_nOffset % m_nBytesPerLine == m_nBytesPerLine - 1)
	{
		transitCursor(1, CURSOR_ASCII_0, bShift ? MCF_SELECT : 0);
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

	CURSOR_TRANSITION transition = _TransitionTable[m_cursorPos.m_location];

	if (m_bOverwriteMode)
	{
		if (transition.m_newLocation == CURSOR_ASCII_1)
		{
			transition.m_newLocation = CURSOR_ASCII_0;
			transition.m_nOffsetDelta = 1;
		}
		else if (transition.m_newLocation == CURSOR_HEX_2)
		{
			transition.m_newLocation = CURSOR_HEX_0;
			transition.m_nOffsetDelta = 1;
		}
	}

	transitCursor(&transition, bShift ? MCF_SELECT : 0);
}

void CHexEditCtrl::onKeyUp(BOOL bShift, BOOL bCtrl)
{
	int flags = bShift ? MCF_SELECT : 0;
	transitCursor(-m_nBytesPerLine, -1, flags);
}

void CHexEditCtrl::onKeyDown(BOOL bShift, BOOL bCtrl)
{
	int flags = bShift ? MCF_SELECT : 0;
	transitCursor(m_nBytesPerLine, -1, flags);
}

void CHexEditCtrl::onKeyPageUp(BOOL bShift, BOOL bCtrl)
{
	int flags = bShift ? MCF_SELECT : 0;
	
	if (!bCtrl)
	{
		transitCursor(-m_nVisibleLineCount * m_nBytesPerLine, -1, flags);
	}
	else
	{
		int nLine, nCol;
		getCursorLineCol(m_cursorPos, nLine, nCol, FALSE);
		nLine -= m_nFirstVisibleLine;

		transitCursor(-nLine * m_nBytesPerLine, -1, flags);
	}
}


void CHexEditCtrl::onKeyPageDown(BOOL bShift, BOOL bCtrl)
{
	int flags = bShift ? MCF_SELECT : 0;
	
	if (!bCtrl)
	{
		transitCursor(m_nVisibleLineCount * m_nBytesPerLine, -1, flags);
	}
	else if (m_nVisibleLineCount > 1)
	{		
		int nLine, nCol;
		getCursorLineCol(m_cursorPos, nLine, nCol, FALSE);
		nLine -= m_nFirstVisibleLine;

		transitCursor((m_nVisibleLineCount - nLine - 1) * m_nBytesPerLine, -1, flags);
	}
}

void CHexEditCtrl::onKeyHome(BOOL bShift, BOOL bCtrl)
{
	int flags = bShift ? MCF_SELECT : 0;
	int location = m_cursorPos.m_location >= CURSOR_ASCII_0 ? CURSOR_ASCII_0 : CURSOR_HEX_0;

	if (bCtrl)
	{
		CURSOR_POS pos;
		pos.m_nOffset = 0;
		pos.m_location = location;
		setCursorPos(pos, flags);
	}
	else
	{
		int nDelta = (int) m_cursorPos.m_nOffset % m_nBytesPerLine;
		transitCursor(-nDelta, location, flags);
	}
}

void CHexEditCtrl::onKeyEnd(BOOL bShift, BOOL bCtrl)
{
	int flags = bShift ? MCF_SELECT : 0;
	int location = m_cursorPos.m_location >= CURSOR_ASCII_0 ? CURSOR_ASCII_1 : CURSOR_HEX_2;

	if (bCtrl)
	{
		CURSOR_POS pos;
		pos.m_nOffset = (int) m_nDataSize - 1;
		pos.m_location = location;
		setCursorPos(pos, flags);
	}
	else
	{
		int nDelta = m_nBytesPerLine - (int) m_cursorPos.m_nOffset % m_nBytesPerLine - 1;
		transitCursor(nDelta, location, flags);
	}
}

void CHexEditCtrl::onKeyDelete()
{
	if (!m_selection.isEmpty())
	{
		replaceSelection(NULL, 0);
		return;
	}

	switch (m_cursorPos.m_location)
	{
	case CURSOR_ASCII_0:
		if (m_cursorPos.m_nOffset >= (int) m_nDataSize)
			break;

		beginUndoableTransaction(m_cursorPos.m_nOffset, 1);
		insertDeleteData(m_cursorPos.m_nOffset, 0, 1);		
		endUndoableTransaction(0);

		break;

	case CURSOR_ASCII_1:
		{
		SIZE_T nOffset = m_cursorPos.m_nOffset + 1;
		if (nOffset >= m_nDataSize)
			break;

		beginUndoableTransaction(nOffset, 1);
		insertDeleteData(nOffset, 0, 1);
		endUndoableTransaction(0);
		}
		break;

	case CURSOR_HEX_0:
		{
		if (m_cursorPos.m_nOffset >= (LONG) m_nDataSize)
			break;

		DATA* data = m_data + m_cursorPos.m_nOffset;

		if (data->m_flags & DF_4_BITS)
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);
			insertDeleteData(m_cursorPos.m_nOffset, 0, 1);
			endUndoableTransaction(0);
		}
		else
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);
			data->m_data &= 0xf;
			data->m_flags |= DF_4_BITS | DF_MODIFIED;
			setModified();
			invalidateOffset(m_cursorPos.m_nOffset);
			endUndoableTransaction(1);
		}

		}
		break;

	case CURSOR_HEX_1:
		{
		DATA* data = m_data + m_cursorPos.m_nOffset;

		if (data->m_flags & DF_4_BITS)
		{
			if (m_cursorPos.m_nOffset >= (LONG) m_nDataSize -1)
				break;

			onKeyRight(FALSE, FALSE);
			onKeyDelete();
		}
		else
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);
			data->m_data >>= 4;
			data->m_data &= 0xf;
			data->m_flags |= DF_4_BITS | DF_MODIFIED;
			setModified();
			invalidateOffset(m_cursorPos.m_nOffset);
			endUndoableTransaction(1);
		}
		}
		break;

	case CURSOR_HEX_2:
		if (m_cursorPos.m_nOffset >= (LONG) m_nDataSize -1)
			break;

		onKeyRight(FALSE, FALSE);
		onKeyDelete();
		break;
	}
}

void CHexEditCtrl::onKeyBackspace()
{
	if (!m_selection.isEmpty())
	{
		replaceSelection(NULL, 0);
		return;
	}

	switch (m_cursorPos.m_location)
	{
	case CURSOR_ASCII_0:
		if (m_cursorPos.m_nOffset == 0)
			break;

		beginUndoableTransaction(m_cursorPos.m_nOffset - 1, 1);
		insertDeleteData(m_cursorPos.m_nOffset - 1, 0, 1);
		transitCursor(-1, -1);
		endUndoableTransaction(0);

		break;

	case CURSOR_ASCII_1:
		beginUndoableTransaction(m_cursorPos.m_nOffset, 1);
		insertDeleteData(m_cursorPos.m_nOffset, 0, 1);
		transitCursor(-1, -1);
		endUndoableTransaction(0);

		break;

	case CURSOR_HEX_0:
		if (m_cursorPos.m_nOffset == 0)
			break;

		onKeyLeft(FALSE, FALSE);
		onKeyBackspace();
		break;

	case CURSOR_HEX_1:
		{
		DATA* data = m_data + m_cursorPos.m_nOffset;
		
		if (data->m_flags & DF_4_BITS)
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);
			insertDeleteData(m_cursorPos.m_nOffset, 0, 1);
			transitCursor(-1, CURSOR_HEX_2);
			endUndoableTransaction(0);
		}
		else
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);
			data->m_data &= 0xf;
			data->m_flags |= DF_4_BITS | DF_MODIFIED;
			setModified();
			invalidateOffset(m_cursorPos.m_nOffset);
			transitCursor(0, CURSOR_HEX_0);
			endUndoableTransaction(1);
		}

		}
		break;

	case CURSOR_HEX_2:
		{
		DATA* data = m_data + m_cursorPos.m_nOffset;

		beginUndoableTransaction(m_cursorPos.m_nOffset, 1);
		data->m_data >>= 4;
		data->m_data &= 0xf;
		data->m_flags |= DF_4_BITS | DF_MODIFIED;
		setModified();
		invalidateOffset(m_cursorPos.m_nOffset);
		transitCursor(0, CURSOR_HEX_1);
		endUndoableTransaction(1);
		}
		break;
		
	}
}

static int getHex(CHAR ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (ch >= 'a' && ch <= 'f')
		return ch + 10 - 'a';

	if (ch >= 'A' && ch <= 'F')
		return ch + 10 - 'A';

	return -1; // error
}

void CHexEditCtrl::modifyData(DATA* data, BYTE data, BOOL b4Bits)
{
	data->m_data = data;
	if (b4Bits)
		data->m_flags |= DF_4_BITS;
	else
		data->m_flags &= ~DF_4_BITS;

	data->m_flags |= DF_MODIFIED;
	setModified();
	invalidateData(data);
}

void CHexEditCtrl::selectAll()
{
	m_bSelecting = TRUE;
	m_selStart.m_nOffset = 0;
	m_selStart.m_location = m_cursorPos.m_location >= CURSOR_ASCII_0 ? CURSOR_ASCII_0 : CURSOR_HEX_0;
	
	CURSOR_POS pos;
	pos.m_nOffset = (int) m_nDataSize;
	setCursorPos(pos, MCF_SELECT | MCF_FORCE);
}

LRESULT CHexEditCtrl::onChar(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		copy();
		return 0;

	case 1 + 'V' - 'A':
		paste();
		return 0;

	case 1 + 'X' - 'A':
		cut();
		return 0;
	}

	if (!m_selection.isEmpty())
	{
		if (m_cursorPos.m_location >= CURSOR_ASCII_0)
		{
			replaceSelection(&ch, 1);
			return 0;
		}

		int x = getHex(ch);
		if (x == -1)
			return 0;

		replaceSelection(&ch, 1, FALSE);

		beginUndoableTransaction(m_cursorPos.m_nOffset, 1);

		DATA* data = m_data + m_cursorPos.m_nOffset;
		modifyData(data, x, TRUE);
		transitCursor(0, CURSOR_HEX_1);

		endUndoableTransaction(1);

		return 0;
	}

	switch (m_cursorPos.m_location)
	{
	case CURSOR_ASCII_0:
		{

		BOOL bInsert = !m_bOverwriteMode || m_cursorPos.m_nOffset == (int) m_nDataSize;

		if (bInsert)
			insertDeleteData(m_cursorPos.m_nOffset, 1, 0);

		beginUndoableTransaction(m_cursorPos.m_nOffset, bInsert ? 0 : 1);

		DATA* data = m_data + m_cursorPos.m_nOffset;
		modifyData(data, ch);

		transitCursor(1, -1);
		
		endUndoableTransaction(1);

		}
		break;

	case CURSOR_ASCII_1:
		{
		SIZE_T nOffset = m_cursorPos.m_nOffset + 1;
		BOOL bInsert = !m_bOverwriteMode || nOffset == m_nDataSize;

		if (bInsert)
			insertDeleteData(nOffset, 1, 0);

		beginUndoableTransaction(nOffset, bInsert ? 0 : 1);

		DATA* data = m_data + nOffset;
		modifyData(data, ch);

		transitCursor(1, -1);

		endUndoableTransaction(1);

		}
		break;

	case CURSOR_HEX_0:
		{
		int x = getHex(ch);
		if (x == -1)
			break;

		DATA* data = m_data + m_cursorPos.m_nOffset;

		if (m_bOverwriteMode)
		{
			ASSERT(data < m_dataEnd);

			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);

			BOOL b4Bits = (data->m_flags & DF_4_BITS) != 0;
			BYTE value = b4Bits ? (x & 0xf) : ((data->m_data & 0xf) | ((x << 4) & 0xf0));
			modifyData(data, value, b4Bits);
		}
		else if (data < m_dataEnd && (data->m_flags & DF_4_BITS))
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);

			BYTE value = (data->m_data & 0xf) | ((x << 4) & 0xf0);
			modifyData(data, value, FALSE);
		}
		else
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 0);

			insertDeleteData(m_cursorPos.m_nOffset, 1, 0);

			data = m_data + m_cursorPos.m_nOffset; // cause insert/delete can reallocate
			modifyData(data, x & 0xf, TRUE);
		}

		transitCursor(0, CURSOR_HEX_1);
		endUndoableTransaction(1);

		}
		break;

	case CURSOR_HEX_1:
		{
		int x = getHex(ch);
		if (x == -1)
			break;

		DATA* data = m_data + m_cursorPos.m_nOffset;
		BOOL b4Bits = (data->m_flags & DF_4_BITS) != 0;

		if (m_bOverwriteMode)
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);

			BYTE value = ((b4Bits ? data->m_data << 4 : data->m_data) & 0xf0) | (x & 0xf);
			modifyData(data, value, FALSE);

			transitCursor(1, CURSOR_HEX_0);
		}
		else if (b4Bits)
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 1);

			BYTE value = ((b4Bits ? data->m_data << 4 : data->m_data) & 0xf0) | (x & 0xf);
			modifyData(data, value, FALSE);

			transitCursor(0, CURSOR_HEX_2);
		}
		else
		{
			beginUndoableTransaction(m_cursorPos.m_nOffset, 0);

			insertDeleteData(m_cursorPos.m_nOffset, 1, 0);
			
			data = m_data + m_cursorPos.m_nOffset; // cause insert/delete can reallocate
			DATA* nextData = data + 1;
	
			modifyData(data, (nextData->m_data & 0xf0) | (x & 0xf), FALSE);
			modifyData(nextData, nextData->m_data & 0xf, TRUE);

			transitCursor(1, CURSOR_HEX_0);
		}

		endUndoableTransaction(1);

		}
		break;	

	case CURSOR_HEX_2:
		{
		int x = getHex(ch);
		if (x == -1)
			break;

		SIZE_T nOffset = m_cursorPos.m_nOffset + 1;

		beginUndoableTransaction(nOffset, 0);

		insertDeleteData(nOffset, 1, 0);
		DATA* data = m_data + nOffset; // cause insert/delete can reallocate
		
		modifyData(data, x & 0xf, TRUE);

		transitCursor(1, CURSOR_HEX_1);

		endUndoableTransaction(1);

		}
		break;	
	}

	return 0;
}

BOOL CHexEditCtrl::replaceData(SIZE_T nOffset, SIZE_T nDelete, DATA* data, SIZE_T nSize, BOOL bCursorToEnd)
{	
	BOOL bResult = insertDeleteData(nOffset, nSize, nDelete);
	if (!bResult)
		return FALSE;

	memcpy(m_data + nOffset, data, nSize * sizeof(DATA));

	setModified();

	SIZE_T nInvalidateSize = max(nDelete, nSize);
	ASSERT(nInvalidateSize);

	invalidateRange(nOffset, nOffset + nInvalidateSize - 1);

	CURSOR_POS pos;
	pos.m_nOffset = (int) nOffset; 
	if (bCursorToEnd)
		pos.m_nOffset += (int) nSize;

	if (m_cursorPos.m_location >= CURSOR_ASCII_0)
		pos.m_location = CURSOR_ASCII_0;
	else
		pos.m_location = CURSOR_HEX_0;

	setCursorPos(pos);

	return TRUE;
}

#if (_AXL_VER >= 0x0200)
void CHexEditCtrl::undoImpl(axl::obj::TData* _pParams)
#else
void CHexEditCtrl::undoImpl(AXL_PTR* _pParams)
#endif
{
	UNDO_PARAMS* params = (UNDO_PARAMS*) _pParams->m_p;
	if (params->m_oldData.size())
		replaceData(params->m_nOffset, params->m_newData.size(), &params->m_oldData.front(), params->m_oldData.size(), FALSE);
	else
		replaceData(params->m_nOffset, params->m_newData.size(), NULL, 0, FALSE);

	setCursorPos(params->m_oldCursorPos);
}

#if (_AXL_VER >= 0x0200)
void CHexEditCtrl::redoImpl(axl::obj::TData* _pParams)
#else
void CHexEditCtrl::redoImpl(AXL_PTR* _pParams)
#endif
{
	UNDO_PARAMS* params = (UNDO_PARAMS*) _pParams->m_p;
	if (params->m_newData.size())
		replaceData(params->m_nOffset, params->m_oldData.size(), &params->m_newData.front(), params->m_newData.size(), FALSE);
	else
		replaceData(params->m_nOffset, params->m_oldData.size(), NULL, 0, FALSE);

	setCursorPos(params->m_newCursorPos);
}

void CHexEditCtrl::beginUndoableTransaction(SIZE_T nOffset, SIZE_T nOldDataSize)
{
	m_undoParams.create(_AXLTAG("replace_sel_undo"));
	m_undoParams->m_nOffset = nOffset;
	m_undoParams->m_oldCursorPos = m_cursorPos;

	if (nOldDataSize)
	{
		m_undoParams->m_oldData.resize(nOldDataSize);
		memcpy(&m_undoParams->m_oldData.front(), m_data + nOffset, nOldDataSize * sizeof(DATA));
	}
}

void CHexEditCtrl::endUndoableTransaction(SIZE_T nNewDataSize)
{
	ASSERT(m_undoParams->m_nOffset != -1);

	if (nNewDataSize)
	{
		m_undoParams->m_newData.resize(nNewDataSize);
		memcpy(&m_undoParams->m_newData.front(), m_data + m_undoParams->m_nOffset, nNewDataSize * sizeof(DATA));
	}

	ASSERT(m_undoParams->m_newData.size() != 0 || m_undoParams->m_oldData.size() != 0);

	m_undoParams->m_newCursorPos = m_cursorPos;

#if (_AXL_VER >= 0x0200)
	axl::call::CSillyThisCallT<CHexEditCtrl> undo(this, &CHexEditCtrl::undoImpl, NULL);
	axl::call::CSillyThisCallT<CHexEditCtrl> redo(this, &CHexEditCtrl::redoImpl, NULL);
#else
	axl::CSillyThisCallT<CHexEditCtrl> undo(this, &CHexEditCtrl::undoImpl, NULL);
	axl::CSillyThisCallT<CHexEditCtrl> redo(this, &CHexEditCtrl::redoImpl, NULL);
#endif

	m_undoRedo.addUndoableAction(undo, redo, m_undoParams, m_undoParams->m_oldData.size() + m_undoParams->m_newData.size());
	m_undoParams = NULL;
}

BOOL CHexEditCtrl::replaceSelection(PVOID pvData, SIZE_T nSize, BOOL bCursorToEnd)
{ 
	SIZE_T nOffset;
	SIZE_T nDelete; 
	
	if (m_selection.m_nStartOffset != m_selection.m_nEndOffset)
	{
		nOffset = m_selection.m_nStartOffset;
		nDelete = m_selection.m_nEndOffset - m_selection.m_nStartOffset;
	}
	else
	{
		nOffset = m_cursorPos.m_nOffset;
		if (m_cursorPos.m_location != CURSOR_ASCII_0 && m_cursorPos.m_location != CURSOR_HEX_0 )
			nOffset++;
		nDelete = 0;
	}

	m_selection.clear();

	beginUndoableTransaction(nOffset, nDelete);

	m_undoParams->m_newData.resize(nSize);

	BOOL bResult;

	if (!nSize)
	{
		bResult = replaceData(nOffset, nDelete, NULL, 0, bCursorToEnd);
		endUndoableTransaction(0);
		return bResult;
	}

	DATA* dst = &m_undoParams->m_newData.front();
	BYTE* src = (BYTE*) pvData;
	BYTE* end = src + nSize;

	for (; src < end; src++, dst++)
	{
		dst->m_flags = DF_MODIFIED;
		dst->m_data = *src;
	}

	bResult = replaceData(nOffset, nDelete, &m_undoParams->m_newData.front(), nSize, bCursorToEnd);
	endUndoableTransaction(0);
	return bResult;
}

void CHexEditCtrl::copy()
{
	SIZE_T nSize = m_selection.m_nEndOffset - m_selection.m_nStartOffset;

	if (!nSize)
		return;

	BOOL bResult = openClipboard();
	if (!bResult)
		return;

	BOOL bAsciiFormat = m_cursorPos.m_location >= CURSOR_ASCII_0;
	
	if (!bAsciiFormat)
		nSize *= 3;

	HANDLE hData = globalAlloc(GMEM_MOVEABLE, nSize + 1);

	if (!hData)
	{
		closeClipboard();
		return;
	}

	PBYTE dst = (PBYTE) globalLock(hData);
	ASSERT(dst);

	dst[nSize] = 0;

	DATA* src = m_data + m_selection.m_nStartOffset;
	DATA* end = m_data + m_selection.m_nEndOffset;

	if (bAsciiFormat)
	{
		for (; src < end; src++, dst++)
			*dst = src->m_data;
	}
	else
	{
		for (; src < end; src++)		
		{
			*dst++ = getHexChar(src->m_data >> 4);
			*dst++ = getHexChar(src->m_data);
			*dst++ = ' ';
		}
	}

	globalUnlock(hData);

	emptyClipboard();
	setClipboardData(CF_TEXT, hData);
	closeClipboard();
}

void CHexEditCtrl::cut()
{
	if (m_selection.isEmpty())
		return;

	copy();
	replaceSelection(NULL, 0);
}

static SIZE_T scanHexString(
	PBYTE dst, SIZE_T nDstBufferSize, 
	PBYTE src, SIZE_T nSrcBufferSize
	)
{
	SIZE_T nActualSize = 0;

	PBYTE end = src + nSrcBufferSize;
	
	enum STATE
	{
		STATE_IDLE = 0,
		STATE_SCAN,
	} state = STATE_IDLE;

	for(; src < end; src++)
	{
		if (isspace(*src))
		{
			if (state == STATE_SCAN)
			{
				dst++;
				state = STATE_IDLE;
			}

			continue;
		}

		int x = getHex(*src);
		if (x == -1) 
			break;

		if (state == STATE_SCAN)
		{
			*dst = ((*dst << 4) & 0xf0) | (x & 0xf);
			dst++;
			state = STATE_IDLE;
		}
		else 
		{
			if (nActualSize + 1 > nDstBufferSize)
				break;

			nActualSize++;
			*dst = (x & 0xf);
			state = STATE_SCAN;
		}
	}

	return nActualSize;
}

void CHexEditCtrl::paste()
{
	BOOL bResult = openClipboard();
	if (!bResult)
		return;

	HANDLE hData = getClipboardData(CF_TEXT);

	if (!hData)
	{
		closeClipboard();
		return;
	}

	SIZE_T nSize = globalSize(hData);
	if (!nSize)
	{
		closeClipboard();
		return;
	}

	BOOL bAsciiFormat = m_cursorPos.m_location >= CURSOR_ASCII_0;

	PBYTE data = (PBYTE) globalLock(hData);
	ASSERT(data);

	nSize = strlen((CHAR*) data);
	
	if (bAsciiFormat)
	{
		replaceSelection(data, nSize);
	}
	else
	{
		SIZE_T nAllocSize = nSize / 2; // no more than this (probably less)
		PBYTE actualData = (PBYTE) malloc(nAllocSize); 
		if (!actualData)
		{
			globalUnlock(hData);
			closeClipboard();
			return;
		}

		SIZE_T nActualSize = scanHexString(actualData, nAllocSize, data, nSize);
		replaceSelection(actualData, nActualSize);

		free(actualData);
	}
	
	globalUnlock(data);

	closeClipboard();
}

void CHexEditCtrl::setFindPattern(PVOID p, SIZE_T nSize, BOOL bMatchCase)
{
	m_findPattern.resize(nSize);
	memcpy(&m_findPattern.front(), p, nSize);

	SIZE_T i;
	for (i = 0; i < 256; i++)
	{
		m_findNextBadCharTable[i] = nSize;
		m_findPrevBadCharTable[i] = nSize;
	}

	SIZE_T m = nSize - 1;

	BYTE* data = (BYTE*) p;

	if (bMatchCase)
	{
		for (i = 0; i < m; i++) 
			m_findNextBadCharTable[data[i]] = m - i;

		for (i = m; i > 0; i--) 
			m_findPrevBadCharTable[data[i]] = i;
	}
	else
	{
		for (i = 0; i < m; i++) 
		{
			m_findNextBadCharTable[tolower(data[i])] = m - i;
			m_findNextBadCharTable[toupper(data[i])] = m - i;
		}

		for (i = m; i > 0; i--) 
		{
			m_findPrevBadCharTable[tolower(data[i])] = i;
			m_findPrevBadCharTable[toupper(data[i])] = i;
		}
	}

	m_bFindMatchCase = bMatchCase;
}

inline BOOL isMatchChar(BYTE b1, BYTE b2, BOOL bMatchCase)
	{ return bMatchCase ? b1 == b2 : toupper(b1) == toupper(b2);}

BOOL CHexEditCtrl::isMatch(DATA* data, BYTE* pattern, SIZE_T nSize, BOOL bMatchCase)
{
	BYTE* p = pattern;
	BYTE* end = p + nSize;

	for (; p != end; p++, data++)
		if (!isMatchChar(*p, data->m_data, bMatchCase))
			return FALSE;

	return TRUE;
}		

void CHexEditCtrl::selectRange(SIZE_T nOffset, SIZE_T nSize, int flags)
{
	CURSOR_POS pos1, pos2;
	pos1.m_nOffset = (long) nOffset;
	pos2.m_nOffset = (long) (nOffset + nSize - 1);
	
	if (flags & SRF_CURSOR_TO_HEX)
	{
		pos1.m_location = CURSOR_HEX_0;
		pos2.m_location = CURSOR_HEX_2;
	}
	else
	{
		pos1.m_location = CURSOR_ASCII_0;
		pos2.m_location = CURSOR_ASCII_1;
	}

	if (flags & SRF_CURSOR_TO_BEGIN)
	{
		setCursorPos(pos2);
		setCursorPos(pos1, MCF_SELECT);
	}
	else
	{
		setCursorPos(pos1);
		setCursorPos(pos2, MCF_SELECT);
	}
}

BOOL CHexEditCtrl::findNext()
{
	SIZE_T nSize = m_findPattern.size();
	SIZE_T m = nSize - 1;

	BYTE last = m_findPattern[m];

	DATA* p = m_data + m_cursorPos.m_nOffset;
	DATA* end = m_dataEnd - nSize;

	while (p <= end) 
	{
		BYTE b = p[m].m_data;

		if (isMatchChar(b, last, m_bFindMatchCase) && isMatch(p, &m_findPattern.front(), m, m_bFindMatchCase))
		{
			int flags = 0;
			if (m_cursorPos.m_location < CURSOR_ASCII_0)
				flags |= SRF_CURSOR_TO_HEX;

			selectRange(p - m_data, nSize);
			return TRUE;
		}

		p += m_findNextBadCharTable[b];
	}

	return FALSE;

}

BOOL CHexEditCtrl::findPrev()
{
	SIZE_T nSize = m_findPattern.size();
	SIZE_T m = nSize - 1;

	if ((LONG) nSize > m_cursorPos.m_nOffset)
		return FALSE;

	BYTE first = m_findPattern[0];

	DATA* p = m_data + m_cursorPos.m_nOffset - nSize;

	while (p >= m_data)
	{
		BYTE b = p[0].m_data;

		if (isMatchChar(b, first, m_bFindMatchCase) && isMatch(p, &m_findPattern.front(), m, m_bFindMatchCase))
		{
			int flags = SRF_CURSOR_TO_BEGIN;
			if (m_cursorPos.m_location < CURSOR_ASCII_0)
				flags |= SRF_CURSOR_TO_HEX;

			selectRange(p - m_data, nSize);
			return TRUE;
		}

		p -= m_findPrevBadCharTable[b];
	}

	return FALSE;
}
