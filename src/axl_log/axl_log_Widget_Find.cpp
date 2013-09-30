#include "pch.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

bool
CWidget::FindNext ()
{
	if (!m_LineCount) // special case of empty log
		return false;

	gui::TCursorPos SelStart;
	gui::TCursorPos SelEnd;
	GetSelection (&SelStart, &SelEnd);

	bool Result = FindNextImpl (SelEnd.m_Line, SelEnd.m_Col, m_LineCount);
	if (Result)
		return true;
	
	// nope, wrap after the end of log...
	return FindNextImpl (0, 0, SelEnd.m_Line + 1);
}

bool
CWidget::FindPrev ()
{
	gui::TCursorPos SelStart;
	gui::TCursorPos SelEnd;

	if (!m_LineCount) // special case of empty log
		return false;

	GetSelection (&SelStart, &SelEnd);

	bool Result = FindPrevImpl (SelStart.m_Line, SelStart.m_Col, 0);
	if (Result)
		return true;

	// nope, wrap after the beginning of log...
	return FindPrevImpl (m_LineCount - 1, -1, SelStart.m_Line);
}

size_t
CWidget::IncrementalFindEx (
	CBinLine* pLine,
	size_t LineOffset
	)
{
	size_t Size = pLine->m_BinData.GetCount ();
	return LineOffset < Size ? m_Find.IncrementalFind ((uchar_t*) pLine->m_BinData + LineOffset, Size - LineOffset) : -1;
}

size_t
CWidget::IncrementalFind (CBinLine* pLine)
{
	size_t Size = pLine->m_BinData.GetCount ();
	return m_Find.IncrementalFind (pLine->m_BinData, Size);
}

bool
CWidget::FindNextImpl (
	size_t Line, 
	size_t Col, 
	size_t EndLine
	)
{
	size_t PatternSize = m_Find.GetPatternSize ();
	if (!PatternSize)
		return false;

	ASSERT (EndLine <= m_LineCount);

	CLine* pLine = m_CacheMgr.GetLine (Line);
	while (Line < EndLine)
	{
		if (pLine->m_LineKind == ELine_Text)
		{
			CTextLine* pTextLine = (CTextLine*) pLine;
			rtl::CString Text = pTextLine->m_HyperText.GetText ();
			size_t Length = Text.GetLength ();

			size_t Offset = m_Find.Find (Text.cc () + Col, Length - Col);
			if (Offset != -1)
			{
				Col += Offset;
				SetCursorPos (Line, Col);
				SetCursorPos (Line, Col + PatternSize, true);
				return true;
			}
		}
		else
		{
			size_t StartOffset;
			size_t StartLineOffset;
			size_t Offset;

			size_t HexCol;
			ASSERT (pLine->IsBin ());

			GetBinLineOffset ((CBinLine*) pLine, Col, &StartOffset, &StartLineOffset, &HexCol);
			if (HexCol == 2)
				StartOffset++;

			m_Find.ResetIncrementalFind (StartOffset);

			Offset = IncrementalFindEx ((CBinLine*) pLine, StartLineOffset);
			if (Offset != -1) // found
			{
				SelectBinRange ((CBinLine*) pLine, Offset, PatternSize);
				return true;
			}

			Line++;

			while (Line < EndLine)
			{
				CLine* pPrevLine = pLine;

				pLine = m_CacheMgr.GetNextLine (pLine);
				if (!pLine || !(pPrevLine->GetFlags () & ELineFlag_MergedForward))
					break;

				Offset = IncrementalFind ((CBinLine*) pLine);
				if (Offset != -1) // found
				{
					SelectBinRange ((CBinLine*) pLine, Offset, PatternSize);
					return true;
				}

				Line++;
			} 			
		}

		pLine = m_CacheMgr.GetNextLine (pLine);
		Line++;
		Col = 0; // col is used for 1st line only
	}

	return false;
}


bool
CWidget::FindPrevImpl (
	size_t Line, 
	size_t Col, 
	size_t EndLine
	)
{
	size_t PatternSize = m_Find.GetPatternSize ();
	if (!PatternSize)
		return false;

	ASSERT (EndLine >= 0);

	CLine* pLine = m_CacheMgr.GetLine (Line);
	while (Line >= EndLine)
	{
/*		if (pLine->m_Type == ELine_Text)
		{
			TTextLine* pTextLine = (TTextLine*) pLine;
			size_t Offset;

			if (Col == -1)
				Col = axl_rtl_TString_GetLength (&pTextLine->m_HyperText.m_Text);

			Offset = m_Find.ReverseFind (pTextLine->m_HyperText.m_Text.m_p, Col);
			if (Offset != -1)
			{
				Col -= Offset;
				SetCursorPosDirect (Line, Col, 0);
				SetCursorPosDirect (Line, Col + m_Find.m_PatternSize, true);
				return true;
			}
		}
		else
		{
			int StartOffset = Col == -1 ? HexLineSize : GetHexOffsetFromCol (pLine, Col, true);
			int MsgCode = pLine->m_MsgCode;

			m_Find.ResetIncrementalFind ();

			size_t Offset = m_Find.ReverseIncrementalFind (pLine->m_HexData, StartOffset);			
			if (Offset != -1) // found
			{		
				Offset = StartOffset - Offset;
				Col = AsciiCol + Offset % HexLineSize;
				SetCursorPos (Line, Col);

				Offset += m_Find.m_PatternSize;
				Col = AsciiCol + Offset % HexLineSize;
				SetCursorPos (Line, Col, true);
				return true;
			}

			Line--;

			while (Line >= EndLine)
			{
				pLine = GetLine (Line);
				if (pLine->m_MsgCode != MsgCode)
					break;

				Offset = m_Find.ReverseIncrementalFind (pLine->m_HexData, HexLineSize);

				if (Offset != -1) // found
				{				
					ASSERT (Offset > (size_t) StartOffset); // because we moved to another line already
					Offset -= StartOffset;

					size_t m = Offset % HexLineSize;
					Offset = m ? HexLineSize - m : 0;

					Col = AsciiCol + Offset;
					SetCursorPos (Line, Col);

					Offset += m_Find.m_PatternSize;
					Line = Line + Offset / HexLineSize;
					Col = AsciiCol + Offset % HexLineSize;
					SetCursorPos (Line, Col, true);
					return true;
				}

				Line--;
			} 			
		} */

		pLine = m_CacheMgr.GetPrevLine (pLine);
		Line--;
		Col = -1; // col is used for 1st line only
	}

	return false;
}

//.............................................................................

} // namespace log {
} // namespace axl {
