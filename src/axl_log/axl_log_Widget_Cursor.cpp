#include "pch.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

size_t
CWidget::GetBinHexLineOffsetFromCol (
	CBinHexLine* pLine,
	size_t Col,
	size_t* pHexCol
	)
{
	size_t Offset;
	size_t HexCol;

	size_t Size = pLine->m_BinData.GetCount ();

	size_t FullOffsetWidth = GetFullOffsetWidth ();
	size_t HexGapSize  = GetHexGapSize ();

	if (Col < FullOffsetWidth)
		Col = 0;
	else
		Col -= FullOffsetWidth;
	
	if (Col <= (size_t) pLine->m_BinDataConfig.m_BinHexLineSize * 3)
	{
		size_t End = (Size - 1) * 3 + 2;
		
		if (Col >= End)
			Col = End;

		Offset = Col / 3;
		HexCol = Col % 3;
	}
	else
	{
		size_t AsciiCol = pLine->m_BinDataConfig.m_BinHexLineSize * 3 + HexGapSize;

		if (Col < AsciiCol)
			Col = AsciiCol;

		if (Col - AsciiCol < Size)
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

size_t
CWidget::GetBinTextLineOffsetFromCol (
	CBinTextLine* pLine,
	size_t Col
	)
{
	size_t ColCount = pLine->m_BinTextMap.GetCount ();
	if (!ColCount)
		return 0;

	size_t FullOffsetWidth = GetFullOffsetWidth ();

	if (Col < FullOffsetWidth)
		Col = 0;
	else
		Col -= FullOffsetWidth;

	if (Col >= ColCount)
		return pLine->m_BinData.GetCount ();

	return pLine->m_BinTextMap [Col].m_Offset;
}


bool
CWidget::GetBinLineOffset (
	CBinLine* pLine,
	size_t Col,
	size_t* pOffset,
	size_t* pLineOffset,
	size_t* pHexCol
	)
{
	size_t LineOffset;

	switch (pLine->m_LineKind)
	{
	case ELine_BinHex:
		LineOffset = GetBinHexLineOffsetFromCol ((CBinHexLine*) pLine, Col, pHexCol);
		break;

	case ELine_BinText:
		LineOffset = GetBinTextLineOffsetFromCol ((CBinTextLine*) pLine, Col);
		if (pHexCol)
			*pHexCol = 0;
		break;

	default:
		ASSERT (false);
		return false;
	}

	if (pLineOffset)
		*pLineOffset = LineOffset;

	if (pOffset)
		*pOffset = (size_t) pLine->m_BinOffset + LineOffset;

	return true;
}

gui::TCursorPos
CWidget::GetCursorPosFromMousePos (
	int x, 
	int y,
	bool AdjustX
	)
{
	if (AdjustX)
		x += m_CharSize.m_Width / 2;

	gui::TCursorPos CursorPos (
		y / m_CharSize.m_Height + m_FirstVisibleLine,
		x / m_CharSize.m_Width + m_FirstVisibleCol - m_BaseCol
		);

	CursorPos.ClipNegative ();
	return CursorPos;
}

gui::TCursorPos
CWidget::ValidateCursorPos (
	uint_t Line,
	uint_t Col
	)
{
	CLine* pLine;
	CTextLine* pTextLine;
	CBinLine* pBinHexLine;
	CBinTextLine* pBinTextLine;

	size_t Length;

	if (!m_LineCount)
		return gui::TCursorPos (0, 0);

	if (Line < 0)
		Line = 0;

	if (Line >= m_LineCount)
		Line = m_LineCount - 1;

	if (Col < 0)
		Col = 0;

	pLine = m_CacheMgr.GetLine (Line);

	switch (pLine->m_LineKind)
	{
	case ELine_Text:
		pTextLine = (CTextLine*) pLine;

		Length = pTextLine->m_HyperText.GetText ().GetLength ();

		if (Col > Length)
			Col = Length;
		break;

	case ELine_BinHex:
		pBinHexLine = (CBinLine*) pLine;
		
		Length = pBinHexLine->m_BinData.GetCount ();

		if (Col < m_FullOffsetWidth)
			Col = 0;
		else
			Col -= m_FullOffsetWidth;
		
		if (Col <= (uint_t) pBinHexLine->m_BinDataConfig.m_BinHexLineSize * 3)
		{
			size_t End = (Length - 1) * 3 + 2;
			
			if (Col >= End)
				Col = End;
		}
		else
		{
			size_t AsciiCol = pBinHexLine->m_BinDataConfig.m_BinHexLineSize * 3 + m_HexGapSize;

			if (Col < AsciiCol)
				Col = AsciiCol;

			if (Col - AsciiCol > Length)
				Col = AsciiCol + Length;
		}

		Col += m_FullOffsetWidth;

		break;

	case ELine_BinText:
		pBinTextLine = (CBinTextLine*) pLine;

		Length = pBinTextLine->m_BinText.GetLength ();
		ASSERT (pBinTextLine->m_BinTextMap.GetCount () == Length);

		if (Col < m_FullOffsetWidth)
			Col = 0;
		else
			Col -= m_FullOffsetWidth;

		if (Col >= Length)
			Col = Length;
		else		
			Col = pBinTextLine->m_BinTextMap [Col].m_Col;

		Col += m_FullOffsetWidth;

		break;
	}

	return gui::TCursorPos (Line, Col);
}

bool 
CWidget::ValidateCursorPosRange (
	gui::TCursorPos* pPosStart,
	gui::TCursorPos* pPosEnd
	)
{
	ValidateCursorPos (pPosStart);
	ValidateCursorPos (pPosEnd);

	int Cmp = pPosStart->Cmp (*pPosEnd);
	if (Cmp > 0)
	{
		gui::TCursorPos Tmp = *pPosStart;
		*pPosStart = *pPosEnd;
		*pPosEnd = Tmp;
	}

	return Cmp != 0;
}

void 
CWidget::SetCursorPos (
	uint_t Line,
	uint_t Col,
	bool Select
	)
{
	CLine* pNewLine;
	gui::TCursorPos OldPos = m_CursorPos;
	gui::TCursorPos Pos = ValidateCursorPos (Line, Col);
	
	if (Pos == m_CursorPos)
		return;

	// handle selection

	OldPos = m_CursorPos;
	m_CursorPos = Pos;

	UpdateCaretPos ();

	if (Select)
	{
		if (!m_IsSelecting)
		{
			m_IsSelecting = true;
			m_SelAnchor = OldPos;
		}

		SetSelection (m_SelAnchor, Pos);
	}
	else
	{
		KillSelection ();

		m_SelStart = Pos;
		m_SelEnd = Pos;

		pNewLine = m_CacheMgr.GetLine (Pos.m_Line);
		if (pNewLine && pNewLine->m_LineKind == ELine_BinHex)
		{
			gui::TCursorPos PosStart = m_CursorPos;
			gui::TCursorPos PosEnd = m_CursorPos;

			size_t HexCol;
			size_t Offset = GetBinHexLineOffsetFromCol ((CBinHexLine*) pNewLine, PosStart.m_Col, &HexCol);

			if (HexCol == 2)
				PosStart.m_Col--;
			else
				PosEnd.m_Col++;

			SetHilite (PosStart, PosEnd);
		}
		else
		{
			KillHilite ();
		}
	}

	NotifyParent (ENm_CursorPosChange);
}

void 
CWidget::EnsureVisible (
	uint_t Line,
	uint_t Col
	)
{
	Col += m_BaseCol;

	size_t OldFirstVisibleLine = m_FirstVisibleLine;
	size_t OldFirstVisibleCol = m_FirstVisibleCol;

	if (Line >= m_FirstVisibleLine + m_VisibleLineCount)
		m_FirstVisibleLine = Line - m_VisibleLineCount + 1;

	if (Line < m_FirstVisibleLine)
		m_FirstVisibleLine = Line;

	if (Col + 1 >= m_FirstVisibleCol + m_VisibleColCount)
		m_FirstVisibleCol = Col + 1 - m_VisibleColCount + 1; 

	if (Col < m_FirstVisibleCol)
		m_FirstVisibleCol = Col;

	FixupFirstVisibleLine ();
	FixupFirstVisibleCol ();

	if (OldFirstVisibleLine != m_FirstVisibleLine ||
		OldFirstVisibleCol != m_FirstVisibleCol)
	{
		RecalcHScroll ();
		RecalcVScroll ();
		UpdateCaretPos ();
		Redraw ();
	}
}

void 
CWidget::EnsureVisibleRange (
	const gui::TCursorPos &PosStart,
	const gui::TCursorPos &PosEnd
	)
{
	size_t ColStart = PosStart.m_Col + m_BaseCol;
	size_t ColEnd = PosEnd.m_Col + m_BaseCol;

	size_t OldFirstVisibleLine = m_FirstVisibleLine;
	size_t OldFirstVisibleCol = m_FirstVisibleCol;

	// end first

	if (PosEnd.m_Line_u >= m_FirstVisibleLine + m_VisibleLineCount)
		m_FirstVisibleLine = PosEnd.m_Line - m_VisibleLineCount + 1;
	 
	if (PosEnd.m_Line_u < m_FirstVisibleLine)
		m_FirstVisibleLine = PosEnd.m_Line;

	if (ColEnd + 1 >= m_FirstVisibleCol + m_VisibleColCount)
		m_FirstVisibleCol = ColEnd + 1 - m_VisibleColCount + 1; 

	if (ColEnd < m_FirstVisibleCol)
		m_FirstVisibleCol = ColEnd;

	// then start (so beginning of range is visible)

	if (PosStart.m_Line_u >= m_FirstVisibleLine + m_VisibleLineCount)
		m_FirstVisibleLine = PosStart.m_Line - m_VisibleLineCount + 1;

	if (PosStart.m_Line_u < m_FirstVisibleLine)
		m_FirstVisibleLine = PosStart.m_Line;

	if (ColStart + 1 >= m_FirstVisibleCol + m_VisibleColCount)
		m_FirstVisibleCol = ColStart + 1 - m_VisibleColCount + 1; 

	if (ColStart < m_FirstVisibleCol)
		m_FirstVisibleCol = ColStart;

	FixupFirstVisibleLine ();
	FixupFirstVisibleCol ();

	if (OldFirstVisibleLine != m_FirstVisibleLine ||
		OldFirstVisibleCol != m_FirstVisibleCol)
	{
		RecalcHScroll ();
		RecalcVScroll ();
		UpdateCaretPos ();
		Redraw ();
	}
}

bool
CWidget::GetSelection (
	gui::TCursorPos* pSelStart, 
	gui::TCursorPos* pSelEnd
	)
{
	*pSelStart = m_SelStart;
	*pSelEnd = m_SelEnd;
	return m_SelStart != m_SelEnd;
}

void
CWidget::SetSelection (
	const gui::TCursorPos& RawSelStart, 
	const gui::TCursorPos& RawSelEnd
	)
{
	gui::TCursorPos SelStart = RawSelStart;
	gui::TCursorPos SelEnd = RawSelEnd;

	ValidateCursorPosRange (&SelStart, &SelEnd);

	bool IsStartPosChanged = m_SelStart != SelStart;
	bool IsEndPosChanged = m_SelEnd != SelEnd;

	if (!IsStartPosChanged && !IsEndPosChanged)
		return;

	size_t OldSelStartLine = m_SelStart.m_Line;
	size_t OldSelEndLine = m_SelEnd.m_Line;

	m_SelStart = SelStart;
	m_SelEnd = SelEnd;

	RedrawSelChangeLineRange (
		m_SelStart.m_Line, 
		m_SelEnd.m_Line,
		OldSelStartLine,
		OldSelEndLine,
		IsStartPosChanged,
		IsEndPosChanged
		);
}

void 
CWidget::KillSelection ()
{
	gui::TCursorPos OldSelStart = m_SelStart;
	gui::TCursorPos OldSelEnd = m_SelEnd;
	
	m_SelAnchor.Setup (0, 0);
	m_SelStart.Setup (0, 0);
	m_SelEnd.Setup (0, 0);

	m_IsSelecting = false;

	if (OldSelStart != OldSelEnd) 
		RedrawLineRange (OldSelStart.m_Line, OldSelEnd.m_Line);
}

bool
CWidget::GetHilite (
	gui::TCursorPos* pHiliteStart, 
	gui::TCursorPos* pHiliteEnd
	)
{
	*pHiliteStart = m_HiliteStart;
	*pHiliteEnd = m_HiliteEnd;
	return m_HiliteStart != m_HiliteEnd;
}

void 
CWidget::SetHilite (
	const gui::TCursorPos& RawPosStart,
	const gui::TCursorPos& RawPosEnd
	)
{
	gui::TCursorPos PosStart = RawPosStart;
	gui::TCursorPos PosEnd = RawPosEnd;

	ValidateCursorPosRange (&PosStart, &PosEnd);

	bool IsStartPosChanged = m_HiliteStart != PosStart;
	bool IsEndPosChanged = m_HiliteEnd != PosEnd;

	if (!IsStartPosChanged && !IsEndPosChanged)
		return;

	size_t OldHiliteStartLine = m_HiliteStart.m_Line;
	size_t OldHiliteEndLine = m_HiliteEnd.m_Line;

	m_HiliteStart = PosStart;
	m_HiliteEnd = PosEnd;

	RedrawSelChangeLineRange (
		m_HiliteStart.m_Line, 
		m_HiliteEnd.m_Line,
		OldHiliteStartLine,
		OldHiliteEndLine,
		IsStartPosChanged,
		IsEndPosChanged
		);
}

void 
CWidget::KillHilite ()
{
	gui::TCursorPos OldHiliteStart = m_HiliteStart;
	gui::TCursorPos OldHiliteEnd = m_HiliteEnd;
	
	m_HiliteStart.Setup (0, 0);
	m_HiliteEnd.Setup (0, 0);

	if (OldHiliteStart != OldHiliteEnd) 
		RedrawLineRange (OldHiliteStart.m_Line, OldHiliteEnd.m_Line);
}

size_t
CWidget::GetHiliteCol (
	CBinLine* pLine,
	size_t Offset
	)
{
	return pLine->m_LineKind == ELine_BinHex ?
		m_FullOffsetWidth + Offset * 3 :
		m_FullOffsetWidth + ((CBinTextLine*) pLine)->FindBinTextColByOffset (Offset);
}

bool
CWidget::GetBinRangePos (
	CBinLine* pLine,
	uint64_t Offset,
	size_t Length,
	gui::TCursorPos* pPosStart,
	gui::TCursorPos* pPosEnd
	)
{
	size_t Line;

	CCachePage* pPage = pLine->m_pPage;
	uint64_t EndOffset = Offset + Length;
	
	size_t LineSize;

	// scan backward 

	while (pLine->m_BinOffset > Offset) 
	{
		CLine* pPrevLine = m_CacheMgr.GetPrevLine (pLine);
		if (!pPrevLine || !(pPrevLine->GetFlags () & ELineFlag_MergedForward))
			return false;

		pLine = (CBinLine*) pPrevLine;
	} 

	Line = (size_t) m_CacheMgr.GetLineIdx (pLine);

	// scan forward to the beginning of the range (if anchor line was given properly, cycle will stop immediatly)

	for (;;) 
	{
		LineSize = pLine->m_BinData.GetCount ();
		if (Offset >= pLine->m_BinOffset && Offset <= pLine->m_BinOffset + LineSize)
		{
			pPosStart->m_Line = Line;
			pPosStart->m_Col = GetHiliteCol (pLine, (size_t) (Offset - pLine->m_BinOffset));
			break;
		}

		CLine* pNextLine = m_CacheMgr.GetNextLine (pLine);
		if (!pNextLine || !(pLine->GetFlags () & ELineFlag_MergedForward))
			return false;

		pLine = (CBinLine*) pNextLine;
		Line++;
	} 

	// scan forward to the end of the range 

	for (;;) 
	{
		LineSize = pLine->m_BinData.GetCount ();
		if (EndOffset >= pLine->m_BinOffset && EndOffset <= pLine->m_BinOffset + LineSize)
		{
			pPosEnd->m_Line = Line;
			pPosEnd->m_Col = GetHiliteCol (pLine, (size_t) (EndOffset - pLine->m_BinOffset));
			break;
		}

		CLine* pNextLine = m_CacheMgr.GetNextLine (pLine);
		if (!pNextLine || !(pLine->GetFlags () & ELineFlag_MergedForward))
			return false;

		pLine = (CBinLine*) pNextLine;
		Line++;
	} 

	return true;
}

bool
CWidget::SelectBinRange (
	CBinLine* pLine,
	size_t Offset,
	size_t Length
	)
{
	gui::TCursorPos PosStart;
	gui::TCursorPos PosEnd;

	bool Result = GetBinRangePos (pLine, Offset, Length, &PosStart, &PosEnd);
	if (!Result)
		return false;

	SetSelection (PosStart, PosEnd);
	EnsureVisibleRange (PosStart, PosEnd);
	
	m_CursorPos = PosEnd;
	UpdateCaretPos ();
	
	return true;
}

bool
CWidget::HiliteBinRange (
	CBinLine* pLine,
	size_t Offset,
	size_t Length
	)
{
	gui::TCursorPos PosStart;
	gui::TCursorPos PosEnd;

	bool Result = GetBinRangePos (pLine, Offset, Length, &PosStart, &PosEnd);
	if (!Result)
		return false;

	KillSelection ();
	SetHilite (PosStart, PosEnd);

	return true;
}

enum ECursorPosFixup
{
	ECursorPosFixup_None = 0,
	ECursorPosFixup_Below,
	ECursorPosFixup_Within,
};

static
ECursorPosFixup
FixupCursorPos (
	gui::TCursorPos* pPos,
	size_t AnchorLine,
	size_t OldLineCount,
	size_t NewLineCount
	)
{
	intptr_t LineDelta = NewLineCount - OldLineCount;

	if (pPos->m_Line_u >= AnchorLine + OldLineCount) // below re-represented packet
	{
		pPos->m_Line = pPos->m_Line + (int) LineDelta;
		return ECursorPosFixup_Below;
	}
	else if (pPos->m_Line_u >= AnchorLine) // below re-represented packet
	{
		pPos->m_Line = AnchorLine;
		pPos->m_Col = 0;
		return ECursorPosFixup_Within;
	}

	return ECursorPosFixup_None;
}

void
CWidget::UpdateCursorPosOnReRepresentPacket (
	size_t AnchorLine,
	size_t OldLineCount,
	size_t NewLineCount
	)
{
	gui::TCursorPos CursorPos = m_CursorPos;
	FixupCursorPos (&CursorPos, AnchorLine, OldLineCount, NewLineCount);

	if (m_IsSelecting)
	{
		gui::TCursorPos SelStart = m_SelStart;
		gui::TCursorPos SelEnd = m_SelEnd;

		ECursorPosFixup SelStartFixup = FixupCursorPos (&SelStart, AnchorLine, OldLineCount, NewLineCount);
		ECursorPosFixup SelEndFixup = FixupCursorPos (&SelEnd, AnchorLine, OldLineCount, NewLineCount);

		if (SelStartFixup == ECursorPosFixup_Within || SelEndFixup == ECursorPosFixup_Within)
			KillSelection ();
		else if (SelStartFixup != ECursorPosFixup_None || SelEndFixup != ECursorPosFixup_None)
			SetSelection (SelStart, SelEnd);
	}

	if (m_HiliteStart != m_HiliteEnd)
	{
		gui::TCursorPos HiliteStart = m_HiliteStart;
		gui::TCursorPos HiliteEnd = m_HiliteEnd;

		ECursorPosFixup HiliteStartFixup = FixupCursorPos (&HiliteStart, AnchorLine, OldLineCount, NewLineCount);
		ECursorPosFixup HiliteEndFixup = FixupCursorPos (&HiliteEnd, AnchorLine, OldLineCount, NewLineCount);

		if (HiliteStartFixup == ECursorPosFixup_Within || HiliteEndFixup == ECursorPosFixup_Within)
			KillHilite ();
		else if (HiliteStartFixup != ECursorPosFixup_None || HiliteEndFixup != ECursorPosFixup_None)
			SetHilite (HiliteStart, HiliteEnd);
	}

	SetCursorPos (CursorPos, m_IsSelecting);
}

//.............................................................................

} // namespace log {
} // namespace axl {
