#include "axl_Precomp.h"
#include "axl_log_LogCtrl.h"

//.............................................................................

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_Tab(axl_log_TLogCtrl* pLogCtrl)
{
	axl_log_TCursorPos Pos = pLogCtrl->m_CursorPos;
	axl_log_TLine* pLine = axl_log_TLogCtrl_GetLineAtCursor(pLogCtrl);

	long AsciiCol;

	if (pLine->m_Type != axl_log_ELine_BinHex)
		return;

	AsciiCol = axl_log_TLogCtrl_GetAsciiCol(pLogCtrl, (axl_log_TBinLine*) pLine);

	if (Pos.m_Col >= AsciiCol)
		Pos.m_Col = pLogCtrl->m_FullOffsetWidth + (Pos.m_Col - AsciiCol) * 3;
	else
		Pos.m_Col = AsciiCol + (Pos.m_Col - pLogCtrl->m_FullOffsetWidth) / 3;

	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, false);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_Home(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_log_TCursorPos Pos;
	axl_log_TLine* pLine;
	long AsciiCol;

	if (Ctrl)
	{
		Pos.m_Line = 0;
		Pos.m_Col = 0;
		axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
		axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
		return;
	}

	Pos = pLogCtrl->m_CursorPos;
	pLine = axl_log_TLogCtrl_GetLineAtCursor(pLogCtrl);

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
	case axl_log_ELine_BinText:
		Pos.m_Col = 0;
		break;

	case axl_log_ELine_BinHex:
		AsciiCol = axl_log_TLogCtrl_GetAsciiCol(pLogCtrl, (axl_log_TBinLine*) pLine);

		if (Pos.m_Col >= AsciiCol)
			Pos.m_Col = AsciiCol;
		else
			Pos.m_Col = 0;

		break;
	}

	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_End(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_log_TCursorPos Pos;
	axl_log_TLine* pLine;
	long AsciiCol;

	if (Ctrl)
	{
		Pos.m_Line = MAXLONG;
		Pos.m_Col = MAXLONG;
		axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
		axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
		return;
	}

	Pos = pLogCtrl->m_CursorPos;
	pLine = axl_log_TLogCtrl_GetLineAtCursor(pLogCtrl);

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
	case axl_log_ELine_BinText:
		Pos.m_Col = MAXLONG;
		break;

	case axl_log_ELine_BinHex:
		AsciiCol = axl_log_TLogCtrl_GetAsciiCol(pLogCtrl, (axl_log_TBinLine*) pLine);
		
		if (Pos.m_Col >= AsciiCol)
			Pos.m_Col = MAXLONG;
		else
			Pos.m_Col = AsciiCol - axl_log_EDef_HexGapSize;

		break;
	}

	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_Left(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_log_TCursorPos Pos = pLogCtrl->m_CursorPos;
	axl_log_TLine* pLine = axl_log_TLogCtrl_GetLineAtCursor(pLogCtrl);

	axl_log_TTextLine* pTextLine;
	axl_log_TBinTextLine* pBinTextLine;
	axl_log_TBinLine* pBinHexLine;

	long AsciiCol;

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		pTextLine = (axl_log_TTextLine*) pLine;

		if (Pos.m_Col <= 0)
		{
			pLine = axl_log_TCacheMgr_GetPrevLine(&pLogCtrl->m_CacheMgr, pLine);
			if (!pLine)
				return;

			Pos.m_Line--;
			Pos.m_Col = MAXLONG;
		}
		else if (Ctrl)
			Pos.m_Col = 0;
		else 
			Pos.m_Col--;
		break;

	case axl_log_ELine_BinText:
		pBinTextLine = (axl_log_TBinTextLine*) pLine;

		if (Pos.m_Col <= (long) pLogCtrl->m_FullOffsetWidth)
		{
			pLine = axl_log_TCacheMgr_GetPrevLine(&pLogCtrl->m_CacheMgr, pLine);
			if (!pLine)
				return;

			Pos.m_Line--;
			Pos.m_Col = MAXLONG;
		}
		else if (Ctrl)
			Pos.m_Col = 0;
		else 
			Pos.m_Col--;
		break;

	case axl_log_ELine_BinHex:
		pBinHexLine = (axl_log_TBinLine*) pLine;

		AsciiCol = axl_log_TLogCtrl_GetAsciiCol(pLogCtrl, pBinHexLine);

		if (Pos.m_Col >= AsciiCol)
		{
			if (Pos.m_Col == AsciiCol)
			{
				pLine = axl_log_TCacheMgr_GetPrevLine(&pLogCtrl->m_CacheMgr, pLine);
				if (!pLine)
					return;

				Pos.m_Line--;
				Pos.m_Col = MAXLONG;
			}
			else if (Ctrl)
				Pos.m_Col = AsciiCol;
			else
				Pos.m_Col--;
		}
		else
		{
			if (Pos.m_Col <= (long) pLogCtrl->m_FullOffsetWidth)
			{
				pLine = axl_log_TCacheMgr_GetPrevLine(&pLogCtrl->m_CacheMgr, pLine);
				if (!pLine)
					return;

				Pos.m_Line--;
				if (pLine->m_Type != axl_log_ELine_BinHex)
                    Pos.m_Col = MAXLONG;
				else
					Pos.m_Col = AsciiCol - axl_log_EDef_HexGapSize;
			}
			else if (Ctrl)
				Pos.m_Col = 0;
			else
				Pos.m_Col--;
		}

		break;
	}

	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_Right(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_log_TCursorPos Pos = pLogCtrl->m_CursorPos;
	axl_log_TLine* pLine = axl_log_TLogCtrl_GetLineAtCursor(pLogCtrl);

	axl_log_TTextLine* pTextLine;
	axl_log_TBinTextLine* pBinTextLine;
	axl_log_TBinLine* pBinHexLine;

	long LastCol;
	long AsciiCol;

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		pTextLine = (axl_log_TTextLine*) pLine;
		LastCol = (long) axl_rtl_TString_GetLength(&pTextLine->m_HyperText.m_Text);

		if (Pos.m_Col >= LastCol)
		{
			pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
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

	case axl_log_ELine_BinText:
		pBinTextLine = (axl_log_TBinTextLine*) pLine;
		LastCol = (long) (pLogCtrl->m_FullOffsetWidth + axl_log_TBinTextLine_FindColByOffset(pBinTextLine, -1));

		if (Pos.m_Col >= LastCol)
		{
			pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
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

	case axl_log_ELine_BinHex:
		pBinHexLine = (axl_log_TBinLine*) pLine;

		AsciiCol = axl_log_TLogCtrl_GetAsciiCol(pLogCtrl, pBinHexLine);
		LastCol = (long) axl_rtl_TByteArray_GetCount(&pBinHexLine->m_BinData);

		if (Pos.m_Col >= AsciiCol)
		{
			if (Pos.m_Col >= AsciiCol + LastCol)
			{
				pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
				if (!pLine)
					return;

				Pos.m_Line++;

				if (pLine->m_Type != axl_log_ELine_BinHex)
                    Pos.m_Col = 0;
				else
					Pos.m_Col = axl_log_TLogCtrl_GetAsciiCol(pLogCtrl, (axl_log_TBinLine*) pLine);
			}
			else if (Ctrl)
				Pos.m_Col = MAXLONG;
			else
				Pos.m_Col++;
		}
		else
		{
			LastCol *= 3;
			LastCol += (long) pLogCtrl->m_FullOffsetWidth;
			LastCol--;

			if (Pos.m_Col >= LastCol)
			{
				pLine = axl_log_TCacheMgr_GetNextLine(&pLogCtrl->m_CacheMgr, pLine);
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

	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_Up(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_log_TCursorPos Pos = pLogCtrl->m_CursorPos;

	if (Ctrl)
	{
		axl_log_TLogCtrl_OnScroll(pLogCtrl, SB_VERT, SB_LINEUP);
		return;
	}

	if (!Pos.m_Line)
	{
		axl_log_TLogCtrl_OnKeyDown_Home(pLogCtrl, Shift, false);
		return;
	}

	Pos.m_Line--;
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_Down(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_log_TCursorPos Pos = pLogCtrl->m_CursorPos;

	if (Ctrl)
	{
		axl_log_TLogCtrl_OnScroll(pLogCtrl, SB_VERT, SB_LINEDOWN);
		return;
	}

	if (Pos.m_Line == pLogCtrl->m_LineCount - 1)
	{
		axl_log_TLogCtrl_OnKeyDown_End(pLogCtrl, Shift, false);
		return;
	}

	Pos.m_Line++;
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_PageUp(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_log_TCursorPos Pos = pLogCtrl->m_CursorPos;

	if (Ctrl)
		Pos.m_Line = pLogCtrl->m_FirstVisibleLine;
	else
		Pos.m_Line -= pLogCtrl->m_VisibleLineCount;
	
	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void 
AXL_API
axl_log_TLogCtrl_OnKeyDown_PageDown(
	axl_log_TLogCtrl* pLogCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_log_TCursorPos Pos = pLogCtrl->m_CursorPos;

	if (Ctrl)
		Pos.m_Line = pLogCtrl->m_FirstVisibleLine + pLogCtrl->m_VisibleLineCount - 1;
	else
		Pos.m_Line += pLogCtrl->m_VisibleLineCount;

	axl_log_TLogCtrl_SetCursorPos(pLogCtrl, &Pos, Shift);
	axl_log_TLogCtrl_EnsureVisible(pLogCtrl, &Pos);
}

void
AXL_API
axl_log_TLogCtrl_OnKeyDown(
	axl_log_TLogCtrl* pLogCtrl,
	int VkCode
	)
{
	bool_t Shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bool_t Ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	switch (VkCode)
	{
	case VK_TAB:
		axl_log_TLogCtrl_OnKeyDown_Tab(pLogCtrl);
		break;

	case VK_HOME:
		axl_log_TLogCtrl_OnKeyDown_Home(pLogCtrl, Shift, Ctrl);
		break;

	case VK_END:
		axl_log_TLogCtrl_OnKeyDown_End(pLogCtrl, Shift, Ctrl);
		break;

	case VK_LEFT:
		axl_log_TLogCtrl_OnKeyDown_Left(pLogCtrl, Shift, Ctrl);
		break;

	case VK_RIGHT:
		axl_log_TLogCtrl_OnKeyDown_Right(pLogCtrl, Shift, Ctrl);
		break;

	case VK_UP:
		axl_log_TLogCtrl_OnKeyDown_Up(pLogCtrl, Shift, Ctrl);
		break;

	case VK_DOWN:
		axl_log_TLogCtrl_OnKeyDown_Down(pLogCtrl, Shift, Ctrl);
		break;

	case VK_PRIOR:
		axl_log_TLogCtrl_OnKeyDown_PageUp(pLogCtrl, Shift, Ctrl);
		break;

	case VK_NEXT:
		axl_log_TLogCtrl_OnKeyDown_PageDown(pLogCtrl, Shift, Ctrl);
		break;

	case VK_INSERT:
	case _T('C'):
		if (Ctrl && !Shift)
			axl_log_TLogCtrl_Copy(pLogCtrl);
		break;
	};
}

//.............................................................................
