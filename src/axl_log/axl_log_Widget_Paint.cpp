#include "pch.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

void
CWidget::OnPaint (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	gui::TWidgetPaintMsg* pPaintMsg = (gui::TWidgetPaintMsg*) pMsg;
	gui::CTextPaint TextPaint (pPaintMsg->m_pCanvas);
	TextPaint.m_MonospaceCharSize = m_CharSize;
	PaintRect (&TextPaint, pPaintMsg->m_Rect);
}

void
CWidget::RedrawLineRange (
	size_t FirstLine,
	size_t LastLine
	)
{
	if (LastLine < FirstLine) // swap
		rtl::Swap (FirstLine, LastLine);

	// first line validation

	if (FirstLine > m_FirstVisibleLine)
		FirstLine -= m_FirstVisibleLine;
	else
		FirstLine = 0;

	if (FirstLine >= m_VisibleLineCount)
		return;

	// last line validation

	if (LastLine < m_FirstVisibleLine)
		return;

	LastLine -= m_FirstVisibleLine;

	if (LastLine > m_VisibleLineCount)
		LastLine = m_VisibleLineCount;

	// redraw

	int Top = FirstLine * m_CharSize.m_Height;
	int Bottom = (LastLine + 1) * m_CharSize.m_Height;

	Redraw (0, Top, m_Size.m_Width, Bottom);
}

void
CWidget::RedrawSelChangeLineRange (
	size_t SelStartLine,
	size_t SelEndLine,
	size_t OldSelStartLine,
	size_t OldSelEndLine
	)
{
	if (SelStartLine == OldSelStartLine)
		RedrawLineRange (
			AXL_MIN (m_SelEnd.m_Line_u, OldSelEndLine),
			AXL_MAX (m_SelEnd.m_Line_u, OldSelEndLine)
			);
	else if (m_SelEnd.m_Line == OldSelEndLine)
		RedrawLineRange (
			AXL_MIN (m_SelStart.m_Line_u, OldSelStartLine),
			AXL_MAX (m_SelStart.m_Line_u, OldSelStartLine)
			);
	else
		RedrawLineRange (
			AXL_MIN (m_SelStart.m_Line_u, OldSelStartLine),
			AXL_MAX (m_SelEnd.m_Line_u, OldSelEndLine)
			);
}

size_t 
CWidget::GetTimestampString (
	CLine* pLine,
	rtl::CString* pString
	)
{
	uint64_t Timestamp = pLine->m_IsFirstLineOfPacket || pLine->m_FirstTimestamp != pLine->m_LastTimestamp ? 
		pLine->m_LastTimestamp : 0;

	if (!Timestamp)
	{
		pString->Clear ();
		return 0;
	}

	ASSERT (!m_TimestampFormat.IsEmpty ());
	return g::TTime (Timestamp).Format (pString, m_TimestampFormat);
}

void 
CWidget::PaintRect (
	gui::CTextPaint* pPaint,
	const gui::TRect& Rect
	)
{
	size_t FirstLine = Rect.m_Top / m_CharSize.m_Height;
	size_t LastLine = Rect.m_Bottom / m_CharSize.m_Height;

	if (Rect.m_Bottom % m_CharSize.m_Height)
		LastLine++;

	gui::TRect LineRect;
	LineRect.m_Left = Rect.m_Left;
	LineRect.m_Top = FirstLine * m_CharSize.m_Height;
	LineRect.m_Right = Rect.m_Right;
	LineRect.m_Bottom = LineRect.m_Top + m_CharSize.m_Height;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	RECT OffscreenRect;
	OffscreenRect.left = pRect->left;
	OffscreenRect.top = 0;
	OffscreenRect.right = pRect->right;
	OffscreenRect.bottom = m_CharSize.m_Height;

	HDC hdcOffscreen = TOffscreenBuffer_GetBuffer (&m_OffscreenBuffer, hdc, pRect->right - pRect->left, m_CharSize.m_Height);
	SetViewportOrgEx (hdcOffscreen, -pRect->left, 0, NULL);
#endif

	FirstLine += m_FirstVisibleLine;
	LastLine += m_FirstVisibleLine;

	if (LastLine > m_LineCount)
		LastLine = m_LineCount;

	if (FirstLine < LastLine)
	{
		CLine* pLine = m_CacheMgr.GetLine (FirstLine);
		for (size_t i = FirstLine; i < LastLine; i++)
		{	
#ifdef _AXL_USE_OFFSCREEN_BUFFER
			PaintLine (pPaint, &OffscreenRect, pLine, i);
			BitBlt (
				hdc, 
				LineRect.left, LineRect.top, 
				LineRect.right - LineRect.left,
				LineRect.bottom - LineRect.top,
				hdcOffscreen,
				OffscreenRect.left, OffscreenRect.top,
				SRCCOPY
				);
#else
			PaintLine (pPaint, LineRect, pLine, i);
#endif

			LineRect.m_Top += m_CharSize.m_Height;
			LineRect.m_Bottom += m_CharSize.m_Height;

			pLine = m_CacheMgr.GetNextLine (pLine);
		}
	}

	if (Rect.m_Bottom > LineRect.m_Top)
	{
		LineRect.m_Bottom = Rect.m_Bottom;
		pPaint->m_pCanvas->DrawRect (LineRect, gui::EStdPalColor_WidgetBack);
	}
}

void
CWidget::PaintLine (
	gui::CTextPaint* pPaint,
	const gui::TRect& LineRect, 
	CLine* pLine, 
	size_t Line
	)
{
	gui::TCursorPos* pSelStart;
	gui::TCursorPos* pSelEnd;

	pPaint->m_SelAttr.Clear ();

	if (m_SelStart != m_SelEnd)
	{
		pSelStart = &m_SelStart;
		pSelEnd = &m_SelEnd;
		pPaint->m_SelAttr.m_ForeColor = gui::EStdPalColor_SelectionText;
		pPaint->m_SelAttr.m_BackColor = gui::EStdPalColor_SelectionBack;
	}
	else if (m_HiliteStart != m_HiliteEnd)
	{
		pSelStart = &m_HiliteStart; 
		pSelEnd = &m_HiliteEnd;
		pPaint->m_SelAttr.m_BackColor = EColor_HiliteBack;
	}
	else
	{
		pSelStart = NULL; 
		pSelEnd = NULL;
	}

	if (pLine->IsBin ())
		m_ColorizeMgr.EnsureColorized ((CBinLine*) pLine);

	// reset 

	pPaint->m_pCanvas->m_BaseTextAttr.Setup (gui::EStdPalColor_WidgetText, gui::EStdPalColor_WidgetBack, 0);
	pPaint->m_pCanvas->m_BaseTextAttr.Overlay (pLine->m_LineAttr);
	pPaint->m_pCanvas->m_DefTextAttr.Clear ();

#if 0 // define to see repainted areas
	COLORREF Rgb = RGB (128 + rand () % 128, 128 + rand () % 128, 128 + rand () % 128);
	TColor_SetRgb (&pPaint->m_StdBackColor, Rgb);
#endif

	pPaint->m_Point.m_x = -m_FirstVisibleCol * m_CharSize.m_Width;
	pPaint->m_Point.m_y = LineRect.m_Top;
	pPaint->m_Top = LineRect.m_Top;
	pPaint->m_Bottom = LineRect.m_Bottom;

	if (m_IsTimestampVisible && m_TimestampWidth)
		PaintTimestamp (pPaint, pLine);

	if (m_IsIconVisible && m_IconWidth)
		PaintIcon (pPaint, pLine->m_LineAttr.m_Icon);

	if (pLine->IsBin () && m_OffsetWidth)
		PaintOffset (pPaint, ((CBinLine*) pLine)->m_Offset);

	pPaint->m_pCanvas->m_DefTextAttr.Clear ();

	switch (pLine->m_LineKind)
	{
	case ELine_Text:
		{
		CTextLine* pTextLine = (CTextLine*) pLine;
		PaintText (
			pPaint,
			pTextLine->m_HyperText.GetText (), 
			pTextLine->m_HyperText.GetAttrArray (), 
			Line, 0,
			pSelStart, 
			pSelEnd
			);
		break;
		}

	case ELine_BinHex:
		PaintBinHexLine (			
			pPaint,
			(CBinLine*) pLine, 
			Line,
			pSelStart, 
			pSelEnd
			);
		break;

	case ELine_BinText:
		{
		CBinTextLine* pBinTextLine = (CBinTextLine*) pLine;
		PaintText (
			pPaint,
			pBinTextLine->m_BinText, 
			&pBinTextLine->m_AttrArray,
			Line,
			m_FullOffsetWidth,
			pSelStart, 
			pSelEnd
			);
		break;
		}
	}

	if (pPaint->m_Point.m_x < LineRect.m_Right)
		pPaint->PaintSpace_p (LineRect.m_Right - pPaint->m_Point.m_x, gui::EColorFlag_Transparent);

	PaintDelimiter (pPaint, pLine, LineRect);
}

void
CWidget::PaintTimestamp (
	gui::CTextPaint* pPaint,
	CLine* pLine
	)
{
	size_t Length = GetTimestampString (pLine, &m_StringBuffer);

	pPaint->m_pCanvas->m_DefTextAttr.m_ForeColor = EColor_Timestamp;
	pPaint->PaintText (m_StringBuffer, Length);

	if (Length < m_TimestampWidth)
		pPaint->PaintSpace (m_TimestampWidth - Length);
}

void
CWidget::PaintIcon (
	gui::CTextPaint* pPaint,
	size_t Icon
	)
{
	ASSERT (m_pImageList);

	gui::TPoint Point = pPaint->GetPoint ();

	pPaint->PaintSpace (m_IconWidth);

	if (Icon != -1)
		pPaint->m_pCanvas->DrawImage (
			Point + m_IconOrigin,
			m_pImageList,
			Icon
			);
}

const char* 
CWidget::FormatOffset (size_t Offset)
{
	ASSERT (m_OffsetWidth);

	m_StringBuffer.Format ("%08x", Offset);
		
	const char* pOffset = m_StringBuffer;

	if (m_OffsetWidth < 8)
		pOffset += 8 - m_OffsetWidth;
	
	return pOffset;
}

void
CWidget::PaintOffset (
	gui::CTextPaint* pPaint,
	size_t Offset
	)
{
	const char* pOffset = FormatOffset (Offset);

	pPaint->m_pCanvas->m_DefTextAttr.m_ForeColor = EColor_Offset;
	pPaint->PaintText (pOffset, m_OffsetWidth);
	pPaint->PaintSpace (m_OffsetGapSize);
}

void
CWidget::PaintDelimiter (
	gui::CTextPaint* pPaint,
	CLine* pLine,
	const gui::TRect& LineRect
	)
{
	int Left = LineRect.m_Left;

	CLine* pNextLine = m_CacheMgr.GetNextLine (pLine);
	ELineDelimiter Delimiter = 
		!pNextLine ? pLine->m_LineAttr.m_LowerDelimiter :
		pLine->IsMerged (pNextLine) ? ELineDelimiter_None :
		AXL_MAX (pLine->m_LineAttr.m_LowerDelimiter, pNextLine->m_LineAttr.m_UpperDelimiter);
	
	if (!Delimiter)
		return;

	if (pNextLine && pLine->m_FirstPacketOffset == pNextLine->m_FirstPacketOffset)
		Left = m_BaseCol * m_CharSize.m_Width;

	uint_t Color = EColor_DelimiterLight + Delimiter - 1;
	pPaint->m_pCanvas->DrawRect (
		Left, 
		LineRect.m_Bottom - 1,
		LineRect.m_Right,
		LineRect.m_Bottom,
		Color
		);
}

void
CWidget::PaintText (
	gui::CTextPaint* pPaint,
	const rtl::CString& Text,
	const gui::CTextAttrAnchorArray* pAttrArray,
	size_t Line,
	size_t BaseCol,
	const gui::TCursorPos* pSelStart,
	const gui::TCursorPos* pSelEnd
	)
{
	size_t Length = Text.GetLength ();

	size_t SelStart;
	size_t SelEnd;

	if (!pSelStart || !pSelEnd || Line < pSelStart->m_Line_u || Line > pSelEnd->m_Line_u) // no selection
	{
		pPaint->PaintHyperText (pAttrArray, Text);
		return;
	}

	if (Line > pSelStart->m_Line_u && Line < pSelEnd->m_Line_u)
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

	pPaint->PaintSelHyperText (pAttrArray, SelStart, SelEnd, Text);
}

void
CWidget::PaintBinHex (
	gui::CTextPaint* pPaint,
	CBinLine* pLine,
	size_t SelStart,
	size_t SelEnd
	)
{
	size_t Size = pLine->m_BinData.GetCount ();
	size_t GapLength = (pLine->m_BinDataConfig.m_BinHexLineSize - Size) * 3 + m_HexGapSize;

	if (SelStart >= SelEnd)
	{
		pPaint->PaintHyperBinHex (&pLine->m_AttrArray, pLine->m_BinData, Size);
		pPaint->PaintSpace (GapLength, gui::EColorFlag_Transparent);
		pPaint->PaintHyperBinAscii (&pLine->m_AttrArray, pLine->m_BinData, Size);
	}
	else
	{
		pPaint->PaintSelHyperBinHex (&pLine->m_AttrArray, SelStart, SelEnd, pLine->m_BinData, Size);
		pPaint->PaintSpace (GapLength, gui::EColorFlag_Transparent);
		pPaint->PaintSelHyperBinAscii (&pLine->m_AttrArray, SelStart, SelEnd, pLine->m_BinData, Size);
	}
}

void
CWidget::PaintBinHexLine (
	gui::CTextPaint* pPaint,
	CBinLine* pLine,
	size_t Line,
	const gui::TCursorPos* pSelStart,
	const gui::TCursorPos* pSelEnd
	)
{
	size_t SelStartOffset;
	size_t SelEndOffset;
	size_t SelStartHexCol;
	size_t SelEndHexCol;

	if (!pSelStart || !pSelEnd || Line < pSelStart->m_Line_u || Line > pSelEnd->m_Line_u) // no selection
	{
		PaintBinHex (pPaint, pLine, 0, 0);
	}
	else if (Line > pSelStart->m_Line_u && Line < pSelEnd->m_Line_u)
	{
		PaintBinHex (pPaint, pLine, 0, -1);
	}
	else if (Line == pSelStart->m_Line && Line != pSelEnd->m_Line)
	{
		SelStartOffset = pLine->GetBinHexLineOffset (pSelStart->m_Col, &SelStartHexCol);
		if (SelStartHexCol == 2)
			SelStartOffset++;

		PaintBinHex (pPaint, pLine, SelStartOffset, -1);
	}
	else if (Line == pSelEnd->m_Line && Line != pSelStart->m_Line)
	{
		SelEndOffset = pLine->GetBinHexLineOffset (pSelEnd->m_Col, &SelEndHexCol);
		if (SelEndHexCol)
			SelEndOffset++;

		PaintBinHex (pPaint, pLine, 0, SelEndOffset);
	}
	else 
	{
		ASSERT (Line == pSelStart->m_Line && Line == pSelEnd->m_Line);

		SelStartOffset = pLine->GetBinHexLineOffset (pSelStart->m_Col, &SelStartHexCol);
		SelEndOffset = pLine->GetBinHexLineOffset (pSelEnd->m_Col, &SelEndHexCol);

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

		PaintBinHex (pPaint, pLine, SelStartOffset, SelEndOffset);
	}
}

//.............................................................................

} // namespace log {
} // namespace axl {
