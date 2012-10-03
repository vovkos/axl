#include "axl_Precomp.h"
#include "axl_log_LogCtrl.h"
#include "axl_rtl_Bin2Str.h"

//.............................................................................

void 
AXL_API
axl_log_TLogCtrl_TextCopy(
	axl_rtl_TString* pString, 
	axl_log_TTextLine* pLine, 
	ulong_t FromCol, 
	ulong_t ToCol
	)
{
	size_t Length = axl_rtl_TString_GetLength(&pLine->m_HyperText.m_Text);

	if (ToCol > Length)
		ToCol = (ulong_t) Length;

	if (FromCol < ToCol)
		axl_rtl_TString_AppendString(pString, pLine->m_HyperText.m_Text.m_p + FromCol, ToCol - FromCol);
}

void 
AXL_API
axl_log_TLogCtrl_BinHexAsciiCopy(
	axl_rtl_TString* pString,
	axl_log_TLogCtrl* pLogCtrl, 
	axl_log_TBinLine* pLine,
	long FromCol, 
	long ToCol,
	bool_t HexOnly
	)
{
	size_t FromOffset;
	size_t ToOffset;
	long FromHexCol;
	long ToHexCol;

	if (ToCol == -1)
		ToCol = MAXLONG;

	FromOffset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, pLine, FromCol, &FromHexCol);
	ToOffset = axl_log_TLogCtrl_GetBinHexLineOffset(pLogCtrl, pLine, ToCol, &ToHexCol);

	if (FromHexCol == 2)
		FromOffset++;

	if (ToHexCol)
		ToOffset++;

	if (ToCol == MAXLONG)
		axl_rtl_TString_AppendChar(pString, _T(' '), FromOffset * 3);

	if (FromOffset < ToOffset)
	{
		if (HexOnly)
		{
			axl_rtl_AppendEncodeHexString(pString, (uchar_t*) pLine->m_BinData.m_p + FromOffset, ToOffset - FromOffset, 0);
		}
		else
		{
			axl_rtl_AppendEncodeHexString(pString, (uchar_t*) pLine->m_BinData.m_p + FromOffset, ToOffset - FromOffset, 0);

			if (ToOffset < pLine->m_DataConfig.m_BinHexLineSize)
				axl_rtl_TString_AppendChar(pString, _T(' '), (pLine->m_DataConfig.m_BinHexLineSize - ToOffset) * 3);

			axl_rtl_TString_AppendChar(pString, _T(' '), 1 + axl_log_EDef_HexGapSize + FromOffset);

			axl_rtl_AppendEncodeAsciiString(pString, (uchar_t*) pLine->m_BinData.m_p + FromOffset, ToOffset - FromOffset, '.');
		}
	}
}

void 
AXL_API
axl_log_TLogCtrl_BinTextCopy(
	axl_rtl_TString* pString, 
	axl_log_TBinTextLine* pLine, 
	ulong_t FromCol, 
	ulong_t ToCol
	)
{
	size_t Size;
	size_t FromOffset;
	size_t ToOffset;

	Size = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);

	FromOffset = axl_log_TBinTextLine_FindOffsetByCol(pLine, FromCol);
	ToOffset = axl_log_TBinTextLine_FindOffsetByCol(pLine, ToCol);

	if (FromOffset < ToOffset)
		axl_rtl_TString_AppendString(pString, (tchar_t*) ((uchar_t*) pLine->m_BinData.m_p + FromOffset), ToOffset - FromOffset);
}

void 
AXL_API
axl_log_TLogCtrl_ProcessCopy(
	void* pContext, 
	axl_log_TLogCtrl* pLogCtrl, 
	axl_log_TLine* pLine, 
	axl_log_TLine* pPrevLine, 
	long FromCol, 
	long ToCol
	)
{
	axl_rtl_TString* pString = (axl_rtl_TString*) pContext;

	ASSERT(FromCol >= 0);
	ASSERT(ToCol >= FromCol || ToCol == -1);

	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		if (pPrevLine)
			axl_rtl_TString_AppendString(pString, _T("\r\n"), 2);

		axl_log_TLogCtrl_TextCopy(pString, (axl_log_TTextLine*) pLine, FromCol, ToCol);
		break;

	case axl_log_ELine_BinHex:
		if (pPrevLine)
			axl_rtl_TString_AppendString(pString, _T("\r\n"), 2);

		axl_log_TLogCtrl_BinHexAsciiCopy(pString, pLogCtrl, (axl_log_TBinLine*) pLine, FromCol, ToCol, false);
		break;

	case axl_log_ELine_BinText:
		if (pPrevLine && pPrevLine->m_Type != axl_log_ELine_BinText)
			axl_rtl_TString_AppendString(pString, _T("\r\n"), 2);

		axl_log_TLogCtrl_BinTextCopy(pString, (axl_log_TBinTextLine*) pLine, FromCol, ToCol);
		break;
	}
}

void 
AXL_API
axl_log_TLogCtrl_ProcessBinHexCopy(
	void* pContext, 
	axl_log_TLogCtrl* pLogCtrl, 
	axl_log_TLine* pLine, 
	axl_log_TLine* pPrevLine, 
	long FromCol, 
	long ToCol
	)
{
	axl_rtl_TString* pString = (axl_rtl_TString*) pContext;

	ASSERT(pLine->m_Type == axl_log_ELine_BinHex);

	if (pPrevLine)
		axl_rtl_TString_AppendString(pString, _T("\r\n"), 2);

	axl_log_TLogCtrl_BinHexAsciiCopy(pString, pLogCtrl, (axl_log_TBinLine*) pLine, FromCol, ToCol, true);
}

void 
AXL_API
axl_log_TLogCtrl_ProcessBinHexAsciiCopy(
	void* pContext, 
	axl_log_TLogCtrl* pLogCtrl, 
	axl_log_TLine* pLine, 
	axl_log_TLine* pPrevLine, 
	long FromCol, 
	long ToCol
	)
{
	axl_rtl_TString* pString = (axl_rtl_TString*) pContext;

	ASSERT(pLine->m_Type == axl_log_ELine_BinHex);

	if (pPrevLine)
		axl_rtl_TString_AppendString(pString, _T("\r\n"), 2);

	axl_log_TLogCtrl_BinHexAsciiCopy(pString, pLogCtrl, (axl_log_TBinLine*) pLine, FromCol, ToCol, false);
}

void 
AXL_API
axl_log_TLogCtrl_ProcessBinCopy(
	void* pContext, 
	const void* p,
	size_t Size
	)
{
	axl_rtl_TString* pString = (axl_rtl_TString*) pContext;
	axl_rtl_TString_AppendString(pString, p, Size / sizeof(tchar_t));
}

size_t 
AXL_API
axl_log_TLogCtrl_CopyString(
	axl_log_TLogCtrl* pLogCtrl,
	axl_rtl_TString* pString
	)
{
	axl_log_TBinLine* pStartLine;
	axl_log_TBinLine* pEndLine;

	bool_t IsSelectionBinBlock = axl_log_TLogCtrl_GetSelectionBinBlock(pLogCtrl, NULL, NULL);
	if (!IsSelectionBinBlock)
		return axl_log_TLogCtrl_ProcessSelection(pLogCtrl, axl_log_TLogCtrl_ProcessCopy, pString);

	pStartLine = (axl_log_TBinLine*) axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, pLogCtrl->m_SelStart.m_Line);
	pEndLine = (axl_log_TBinLine*) axl_log_TCacheMgr_GetLine(&pLogCtrl->m_CacheMgr, pLogCtrl->m_SelEnd.m_Line);

	ASSERT(axl_log_TLine_IsBin(pStartLine));
	ASSERT(axl_log_TLine_IsBin(pEndLine));

	if (pStartLine->m_Type == axl_log_ELine_BinHex)
	{
		long AsciiCol = axl_log_TLogCtrl_GetAsciiCol(pLogCtrl, pStartLine);
		if (pLogCtrl->m_SelStart.m_Col < AsciiCol && pLogCtrl->m_SelEnd.m_Col < AsciiCol) // hex only
			return axl_log_TLogCtrl_ProcessSelection(pLogCtrl, axl_log_TLogCtrl_ProcessBinHexCopy, pString);
		
		if (pLogCtrl->m_SelStart.m_Col < AsciiCol && pLogCtrl->m_SelEnd.m_Col >= AsciiCol ||
			pLogCtrl->m_SelStart.m_Col >= AsciiCol && pLogCtrl->m_SelEnd.m_Col < AsciiCol) // combo
			return axl_log_TLogCtrl_ProcessSelection(pLogCtrl, axl_log_TLogCtrl_ProcessBinHexAsciiCopy, pString);
	}

	axl_log_TLogCtrl_ProcessSelectionAsBinBlock(pLogCtrl, axl_log_TLogCtrl_ProcessBinCopy, pString);
	return 1; // we copy it as binary block
}

bool_t
AXL_API
axl_log_TLogCtrl_Copy(axl_log_TLogCtrl* pLogCtrl)
{
	BOOL Result;
	size_t LineCount;
	size_t Size;
	HGLOBAL hData;
	void* p;

	axl_rtl_TString String;
	axl_rtl_TString_Construct(&String);

	LineCount = axl_log_TLogCtrl_CopyString(pLogCtrl, &String);
	if (!LineCount || axl_rtl_TString_IsEmpty(&String))
	{
		axl_rtl_TString_Destruct(&String);
		MessageBeep(MB_OK);
		return true;
	}

	if (LineCount >= 2) 
		axl_rtl_TString_AppendString(&String, _T("\r\n"), 2);

	Result = OpenClipboard(pLogCtrl->m_hWnd);
	if (!Result)
	{
		axl_rtl_TString_Destruct(&String);
		MessageBeep(MB_ICONHAND);
		return false;
	}

	EmptyClipboard();

	Size = (axl_rtl_TString_GetLength(&String) + 1) * sizeof(tchar_t);
	hData = GlobalAlloc(GMEM_MOVEABLE, Size);
	p = GlobalLock(hData);
	memcpy(p, String.m_p, Size);
	SetClipboardData(CF_TEXT, hData);
	
	CloseClipboard(); 
	axl_rtl_TString_Destruct(&String);
	return true;
}

//.............................................................................
