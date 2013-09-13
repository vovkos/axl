#include "pch.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

void
CWidget::OnKeyDown (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	gui::TWidgetKeyMsg* pKeyMsg = (gui::TWidgetKeyMsg*) pMsg;

	bool Shift = (pKeyMsg->m_ModifierKeys & gui::EModifierKey_Shift) != 0;
	bool Ctrl = (pKeyMsg->m_ModifierKeys & gui::EModifierKey_Ctrl) != 0;

	switch (pKeyMsg->m_Key)
	{
	case gui::EKey_Tab:
		OnKeyDown_Tab (Shift, Ctrl);
		break;

	case gui::EKey_Home:
		OnKeyDown_Home (Shift, Ctrl);
		break;

	case gui::EKey_End:
		OnKeyDown_End (Shift, Ctrl);
		break;

	case gui::EKey_Left:
		OnKeyDown_Left (Shift, Ctrl);
		break;

	case gui::EKey_Right:
		OnKeyDown_Right (Shift, Ctrl);
		break;

	case gui::EKey_Up:
		OnKeyDown_Up (Shift, Ctrl);
		break;

	case gui::EKey_Down:
		OnKeyDown_Down (Shift, Ctrl);
		break;

	case gui::EKey_PageUp:
		OnKeyDown_PageUp (Shift, Ctrl);
		break;

	case gui::EKey_PageDown:
		OnKeyDown_PageDown (Shift, Ctrl);
		break;
	
	case gui::EKey_Insert:
	case 'C':
		if (Ctrl && !Shift)
			Copy ();

		break;
	};
}

void 
CWidget::OnKeyDown_Tab (
	bool Shift, 
	bool Ctrl
	)
{
	CLine* pLine = GetLineAtCursor ();
	if (pLine->m_LineKind != ELine_BinHex)
		return;

	size_t AsciiCol = GetAsciiCol ((CBinLine*) pLine);

	gui::TCursorPos Pos = m_CursorPos;
	if (Pos.m_Col_u >= AsciiCol)
		Pos.m_Col = m_FullOffsetWidth + (Pos.m_Col - AsciiCol) * 3;
	else
		Pos.m_Col = AsciiCol + (Pos.m_Col - m_FullOffsetWidth) / 3;

	SetCursorPos (Pos, false);
	EnsureVisible (Pos);
}

void 
CWidget::OnKeyDown_Home (
	bool Shift, 
	bool Ctrl
	)
{
	size_t AsciiCol;

	if (Ctrl)
	{
		SetCursorPos (0, 0, Shift);
		EnsureVisible (0, 0);
		return;
	}

	gui::TCursorPos Pos = m_CursorPos;
	CLine* pLine = GetLineAtCursor ();

	switch (pLine->m_LineKind)
	{
	case ELine_Text:
	case ELine_BinText:
		Pos.m_Col = 0;
		break;

	case ELine_BinHex:
		AsciiCol = GetAsciiCol ((CBinLine*) pLine);

		if (Pos.m_Col_u >= AsciiCol)
			Pos.m_Col = AsciiCol;
		else
			Pos.m_Col = 0;

		break;
	}

	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);
}

void 
CWidget::OnKeyDown_End (
	bool Shift, 
	bool Ctrl
	)
{
	size_t AsciiCol;

	if (Ctrl)
	{
		SetCursorPos (-1, -1, Shift);
		EnsureVisible (-1, -1);
		return;
	}

	gui::TCursorPos Pos = m_CursorPos;
	CLine* pLine = GetLineAtCursor ();

	switch (pLine->m_LineKind)
	{
	case ELine_Text:
	case ELine_BinText:
		Pos.m_Col = -1;
		break;

	case ELine_BinHex:
		AsciiCol = GetAsciiCol ((CBinLine*) pLine);
		
		if (Pos.m_Col_u >= AsciiCol)
			Pos.m_Col = -1;
		else
			Pos.m_Col = AsciiCol - m_HexGapSize;

		break;
	}

	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);
}

void 
CWidget::OnKeyDown_Left (
	bool Shift, 
	bool Ctrl
	)
{
	gui::TCursorPos Pos = m_CursorPos;
	CLine* pLine = GetLineAtCursor ();

	CTextLine* pTextLine;
	CBinTextLine* pBinTextLine;
	CBinLine* pBinHexLine;

	size_t AsciiCol;

	switch (pLine->m_LineKind)
	{
	case ELine_Text:
		pTextLine = (CTextLine*) pLine;

		if (Pos.m_Col <= 0)
		{
			pLine = GetPrevLine (pLine);
			if (!pLine)
				return;

			Pos.m_Line--;
			Pos.m_Col = -1;
		}
		else if (Ctrl)
			Pos.m_Col = 0;
		else 
			Pos.m_Col--;
		break;

	case ELine_BinText:
		pBinTextLine = (CBinTextLine*) pLine;

		if (Pos.m_Col_u <= m_FullOffsetWidth)
		{
			pLine = GetPrevLine (pLine);
			if (!pLine)
				return;

			Pos.m_Line--;
			Pos.m_Col = -1;
		}
		else if (Ctrl)
			Pos.m_Col = 0;
		else 
			Pos.m_Col--;
		break;

	case ELine_BinHex:
		pBinHexLine = (CBinLine*) pLine;

		AsciiCol = GetAsciiCol (pBinHexLine);

		if (Pos.m_Col_u >= AsciiCol)
		{
			if (Pos.m_Col == AsciiCol)
			{
				pLine = GetPrevLine (pLine);
				if (!pLine)
					return;

				Pos.m_Line--;
				Pos.m_Col = -1;
			}
			else if (Ctrl)
				Pos.m_Col = AsciiCol;
			else
				Pos.m_Col--;
		}
		else
		{
			if (Pos.m_Col_u <= m_FullOffsetWidth)
			{
				pLine = GetPrevLine (pLine);
				if (!pLine)
					return;

				Pos.m_Line--;
				if (pLine->m_LineKind != ELine_BinHex)
					Pos.m_Col = -1;
				else
					Pos.m_Col = AsciiCol - m_HexGapSize;
			}
			else if (Ctrl)
				Pos.m_Col = 0;
			else
				Pos.m_Col--;
		}

		break;
	}

	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);
}

void 
CWidget::OnKeyDown_Right (
	bool Shift, 
	bool Ctrl
	)
{
	gui::TCursorPos Pos = m_CursorPos;
	CLine* pLine = GetLineAtCursor ();

	CTextLine* pTextLine;
	CBinTextLine* pBinTextLine;
	CBinLine* pBinHexLine;

	size_t LastCol;
	size_t AsciiCol;

	switch (pLine->m_LineKind)
	{
	case ELine_Text:
		pTextLine = (CTextLine*) pLine;
		LastCol = pTextLine->m_HyperText.GetText ().GetLength ();

		if (Pos.m_Col_u >= LastCol)
		{
			pLine = GetNextLine (pLine);
			if (!pLine)
				return;

			Pos.m_Line++;
			Pos.m_Col = 0;
		}
		else if (Ctrl)
			Pos.m_Col = LastCol;
		else 
			Pos.m_Col++;
		break;

	case ELine_BinText:
		pBinTextLine = (CBinTextLine*) pLine;
		LastCol = (m_FullOffsetWidth + pBinTextLine->FindBinTextColByOffset (-1));

		if (Pos.m_Col_u >= LastCol)
		{
			pLine = GetNextLine (pLine);
			if (!pLine)
				return;

			Pos.m_Line++;
			Pos.m_Col = 0;
		}
		else if (Ctrl)
			Pos.m_Col = LastCol;
		else 
			Pos.m_Col++;
		break;

	case ELine_BinHex:
		pBinHexLine = (CBinLine*) pLine;

		AsciiCol = GetAsciiCol (pBinHexLine);
		LastCol = pBinHexLine->m_BinData.GetCount ();

		if (Pos.m_Col_u >= AsciiCol)
		{
			if (Pos.m_Col_u >= AsciiCol + LastCol)
			{
				pLine = GetNextLine (pLine);
				if (!pLine)
					return;

				Pos.m_Line++;

				if (pLine->m_LineKind != ELine_BinHex)
					Pos.m_Col = 0;
				else
					Pos.m_Col = GetAsciiCol ((CBinLine*) pLine);
			}
			else if (Ctrl)
				Pos.m_Col = -1;
			else
				Pos.m_Col++;
		}
		else
		{
			LastCol *= 3;
			LastCol += m_FullOffsetWidth;
			LastCol--;

			if (Pos.m_Col_u >= LastCol)
			{
				pLine = GetNextLine (pLine);
				if (!pLine)
					return;

				Pos.m_Line++;
				Pos.m_Col = 0;
			}
			else if (Ctrl)
				Pos.m_Col = LastCol;
			else
				Pos.m_Col++;
		}

		break;
	}

	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);
}

void 
CWidget::OnKeyDown_Up (
	bool Shift, 
	bool Ctrl
	)
{
	gui::TCursorPos Pos = m_CursorPos;

	if (Ctrl)
	{
		Scroll (-1);
		return;
	}

	if (!Pos.m_Line)
	{
		OnKeyDown_Home (Shift, false);
		return;
	}

	Pos.m_Line--;
	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);
}

void 
CWidget::OnKeyDown_Down (
	bool Shift, 
	bool Ctrl
	)
{
	gui::TCursorPos Pos = m_CursorPos;

	if (Ctrl)
	{
		Scroll (1);
		return;
	}

	if (Pos.m_Line == m_LineCount - 1)
	{
		OnKeyDown_End (Shift, false);
		return;
	}

	Pos.m_Line++;
	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);
}

void 
CWidget::OnKeyDown_PageUp (
	bool Shift, 
	bool Ctrl
	)
{
	gui::TCursorPos Pos = m_CursorPos;

	if (Ctrl)
		Pos.m_Line = m_FirstVisibleLine;
	else
		Pos.m_Line -= m_VisibleLineCount;
	
	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);
}

void 
CWidget::OnKeyDown_PageDown (
	bool Shift, 
	bool Ctrl
	)
{
	gui::TCursorPos Pos = m_CursorPos;

	if (Ctrl)
		Pos.m_Line = m_FirstVisibleLine + m_VisibleLineCount - 1;
	else
		Pos.m_Line += m_VisibleLineCount;

	SetCursorPos (Pos, Shift);
	EnsureVisible (Pos);
}

//.............................................................................

} // namespace log {
} // namespace axl {
