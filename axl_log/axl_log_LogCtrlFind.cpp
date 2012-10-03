#include "axl_Precomp.h"
#include "axl_log_LogCtrl.h"

//.............................................................................

void 
AXL_API
axl_log_TLogCtrl_SetCursorPosDirect(
	axl_log_TLogCtrl* pLogCtrl,
	long Line,
	long Col,
	bool_t Select
	)
{
	axl_log_TCursorPos Pos;
	Pos.m_Line = Line;
	Pos.m_Col = Col;
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Select);
}

//.............................................................................

__inline
size_t
axl_log_TLogCtrl_IncrementalFindEx(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine,
	size_t LineOffset
	)
{
	size_t Size = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
	return LineOffset < Size ? axl_rtl_TBmhFind_IncrementalFind(&pLogCtrl->m_Find, (uchar_t*) pLine->m_BinData.m_p + LineOffset, Size - LineOffset) : -1;
}

__inline
size_t
axl_log_TLogCtrl_IncrementalFind(
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TBinLine* pLine
	)
{
	size_t Size = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
	return axl_rtl_TBmhFind_IncrementalFind(&pLogCtrl->m_Find, pLine->m_BinData.m_p, Size);
}

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

	pLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, Line);
	while (Line < EndLine)
	{
		if (pLine->m_Type == axl_log_ELine_Text)
		{
			axl_log_TTextLine* pTextLine = (axl_log_TTextLine*) pLine;
			size_t Length = axl_rtl_TString_GetLength(&pTextLine->m_HyperText.m_Text);

			size_t Offset = axl_rtl_TBmhFind_Find(&pLogCtrl->m_Find, pTextLine->m_HyperText.m_Text.m_p + Col, Length - Col);
			if (Offset != -1)
			{
				Col += (long) Offset;
				axl_log_TLogCtrl_SetCursorPosDirect(pLogCtrl, Line, Col, 0);
				axl_log_TLogCtrl_SetCursorPosDirect(pLogCtrl, Line, Col + (long) pLogCtrl->m_Find.m_PatternSize, true);
				return true;
			}
		}
		else
		{
			size_t StartOffset;
			size_t StartLineOffset;
			size_t Offset;

			long HexCol;
			ASSERT(axl_log_TLine_IsBin(pLine));

			axl_log_TLogCtrl_GetBinLineOffset(pLogCtrl, pLine, Col, &StartOffset, &StartLineOffset, &HexCol, NULL);
			if (HexCol == 2)
				StartOffset++;

			axl_rtl_TBmhFind_ResetIncrementalFind(&pLogCtrl->m_Find);
			pLogCtrl->m_Find.m_IncFindOffset = StartOffset;

			Offset = axl_log_TLogCtrl_IncrementalFindEx(pLogCtrl, (axl_log_TBinLine*) pLine, StartLineOffset);
			if (Offset != -1) // found
			{
				axl_log_TLogCtrl_SelectBinRange(pLogCtrl, (axl_log_TBinLine*) pLine, Offset, pLogCtrl->m_Find.m_PatternSize);
				return true;
			}

			Line++;

			while (Line < EndLine)
			{
				axl_log_TLine* pPrevLine = pLine;

				pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
				if (!pLine || !axl_log_TLine_IsMerged(pPrevLine, pLine))
					break;

				Offset = axl_log_TLogCtrl_IncrementalFind(pLogCtrl, (axl_log_TBinLine*) pLine);
				if (Offset != -1) // found
				{
					axl_log_TLogCtrl_SelectBinRange(pLogCtrl, (axl_log_TBinLine*) pLine, Offset, pLogCtrl->m_Find.m_PatternSize);
					return true;
				}

				Line++;
			} 			
		}

		pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
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

	return false;

	if (!axl_rtl_TBmhFind_CanFind(&pLogCtrl->m_Find))
		return false;

	ASSERT(EndLine >= 0);

	pLine = axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, Line);
	while (Line >= EndLine)
	{
/*		if (pLine->m_Type == axl_log_ELine_Text)
		{
			axl_log_TTextLine* pTextLine = (axl_log_TTextLine*) pLine;
			size_t Offset;

			if (Col == -1)
				Col = (long) axl_rtl_TString_GetLength(&pTextLine->m_HyperText.m_Text);

			Offset = axl_rtl_TBmhFind_ReverseFind(&pLogCtrl->m_Find, pTextLine->m_HyperText.m_Text.m_p, Col);
			if (Offset != -1)
			{
				Col -= (long) Offset;
				axl_log_TLogCtrl_SetCursorPosDirect(pLogCtrl, Line, Col, 0);
				axl_log_TLogCtrl_SetCursorPosDirect(pLogCtrl, Line, Col + (long) pLogCtrl->m_Find.m_PatternSize, true);
				return true;
			}
		}
		else
		{
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
		} */

		pLine = axl_log_TCacheMgr_GetPrevLine(&pLogCtrl->m_CacheMgr, pLine);
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

//.............................................................................
