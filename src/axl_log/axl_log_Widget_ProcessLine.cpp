#include "pch.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

bool
CWidget::GetBinOffset (
	const gui::TCursorPos& Pos,
	size_t* pOffset,
	size_t* pLineOffset,
	size_t* pHexCol,
	size_t* pMergeId
	)
{
	CLine* pLine = GetLine (Pos.m_Line);
	return pLine && pLine->IsBin () ? 
		((CBinLine*) pLine)->GetBinLineOffset (Pos.m_Col, pOffset, pLineOffset, pHexCol, pMergeId) : 
		false;
}

bool
CWidget::GetRangeBinBlock (
	const gui::TCursorPos& RawPosStart,
	const gui::TCursorPos& RawPosEnd,
	size_t* pOffset,
	size_t* pSize
	)
{
	gui::TCursorPos PosStart = RawPosStart;
	gui::TCursorPos PosEnd = RawPosEnd;	

	bool IsNonEmpty = ValidateCursorPosRange (&PosStart, &PosEnd);

	size_t StartOffset = -1;
	size_t StartMergeId = 0;

	size_t EndOffset = -1;
	size_t EndMergeId = 0;
	size_t EndHexCol = 0;

	GetBinOffset (PosStart, &StartOffset, NULL, NULL, &StartMergeId);
	GetBinOffset (PosEnd, &EndOffset, NULL, &EndHexCol, &EndMergeId);

	if (StartOffset == -1 || EndOffset == -1 || StartMergeId != EndMergeId)
	{
		if (pOffset)
			*pOffset = -1;

		if (pSize)
			*pSize = 0;

		return false;
	}

	if (IsNonEmpty && EndHexCol)
		EndOffset++;

	if (pOffset)
		*pOffset = StartOffset;

	if (pSize)
		*pSize = EndOffset - StartOffset;

	return true;
}

bool
CWidget::IncapsulateOrProcessBinBlock (
	const void* p,
	size_t Size,
	IProcessBinBlock* pProcess
	)
{
	// normally Size (from a single line) should be much less than EDef_MaxBinBlockBuffer

	m_BinBlockBuffer.Append ((uchar_t*) p, Size);
	size_t BufferSize = m_BinBlockBuffer.GetCount ();

	if (BufferSize < m_MaxBinBlockBuffer)
		return false;

	pProcess->Process (m_BinBlockBuffer, BufferSize);

	m_BinBlockBuffer.Clear ();
	return true;
}

size_t
CWidget::ProcessRangeAsBinBlock (
	const gui::TCursorPos& RawPosStart,
	const gui::TCursorPos& RawPosEnd,
	IProcessBinBlock* pProcess
	)
{
	gui::TCursorPos PosStart = RawPosStart;
	gui::TCursorPos PosEnd = RawPosEnd;	

	bool IsNonEmpty = ValidateCursorPosRange (&PosStart, &PosEnd);
	if (!IsNonEmpty)
		return 0;

	size_t StartFullOffset = -1;
	size_t StartLineOffset = -1;
	size_t StartMergeId = 0;
	size_t StartHexCol;

	size_t EndFullOffset = -1;
	size_t EndLineOffset = -1;
	size_t EndMergeId = 0;
	size_t EndHexCol;

	GetBinOffset (PosStart, &StartFullOffset, &StartLineOffset, &StartHexCol, &StartMergeId);
	GetBinOffset (PosEnd, &EndFullOffset, &EndLineOffset, &EndHexCol, &EndMergeId);

	if (StartFullOffset == -1 || EndFullOffset == -1 || StartMergeId != EndMergeId)
		return 0;

	if (EndHexCol)
	{
		EndFullOffset++;
		EndLineOffset++;
	}

	// first line

	CBinLine* pLine = (CBinLine*) GetLine (PosStart.m_Line);
	ASSERT (pLine->IsBin ());

	if (PosStart.m_Line == PosEnd.m_Line) // special case -- single line
	{
		pProcess->Process (pLine->m_BinData + StartLineOffset, EndLineOffset - StartLineOffset);
		return EndFullOffset - StartFullOffset;
	}

	m_BinBlockBuffer.Clear ();

	size_t LineSize = pLine->m_BinData.GetCount ();
	ASSERT (StartLineOffset < LineSize);

	IncapsulateOrProcessBinBlock (pLine->m_BinData + StartLineOffset, LineSize - StartLineOffset, pProcess);

	// middle lines

	for (int i = PosStart.m_Line + 1; i < PosEnd.m_Line; i++)
	{
		pLine = (CBinLine*) GetNextLine ((CLine*) pLine);
		ASSERT (pLine->IsBin ());

		LineSize = pLine->m_BinData.GetCount ();
		IncapsulateOrProcessBinBlock (pLine->m_BinData, LineSize, pProcess);
	}

	// last line

	pLine = (CBinLine*) GetNextLine ((CLine*) pLine);
	ASSERT (pLine->IsBin ());
	IncapsulateOrProcessBinBlock (pLine->m_BinData, EndLineOffset, pProcess);

	size_t BufferSize = m_BinBlockBuffer.GetCount ();
	if (BufferSize)
		pProcess->Process (m_BinBlockBuffer, BufferSize);

	m_BinBlockBuffer.Clear ();

	return EndFullOffset - StartFullOffset;
}

size_t
CWidget::ProcessRange (
	const gui::TCursorPos& RawPosStart,
	const gui::TCursorPos& RawPosEnd,
	IProcessLine* pProcess
	)
{
	gui::TCursorPos PosStart = RawPosStart;
	gui::TCursorPos PosEnd = RawPosEnd;	

	bool IsNonEmpty = ValidateCursorPosRange (&PosStart, &PosEnd);
	if (!IsNonEmpty)
		return 0;

	// first line

	CLine* pLine = GetLine (PosStart.m_Line);
	if (PosStart.m_Line == PosEnd.m_Line) // special case -- single line
	{
		pProcess->Process (pLine, NULL, PosStart.m_Col, PosEnd.m_Col);
		return 1;
	}

	pProcess->Process (pLine, NULL, PosStart.m_Col, -1);

	// middle lines

	for (int i = PosStart.m_Line + 1; i < PosEnd.m_Line; i++)
	{
		CLine* pPrevLine = pLine;
		pLine = GetNextLine ((CLine*) pLine);
		pProcess->Process (pLine, pPrevLine, 0, -1);
	}

	// last line

	CLine* pPrevLine = pLine;
	pLine = GetNextLine (pLine);
	pProcess->Process (pLine, pPrevLine, 0, PosEnd.m_Col);

	return PosEnd.m_Line - PosStart.m_Line + 1;
}

/*

size_t
CWidget::GetLineString (
	CLine* pLine,
	axl_rtl_TString* pString,
	axl_rtl_TBinTree* pIconToCharMap
	)
{
	axl_rtl_TString_Clear (pString);

	if (m_IsTimestampVisible && m_TimestampWidth)
	{
		size_t Length = GetTimestampString (pLine, pString);
		if (Length < m_TimestampWidth)
			pString->Append (' ', m_TimestampWidth - Length);
	}

	if (m_IconWidth) 
	{
		char IconChar = ' ';

		if (pIconToCharMap && pLine->m_Icon != -1)
		{
			axl_rtl_TBinTreeNode* p = axl_rtl_TBinTree_Find (pIconToCharMap, pLine->m_Icon);
			if (p)
				IconChar = (char) p->m_Value;
		}

		pString->Append (IconChar);
		pString->Append (' ', EDef_IconGapSize);
	}

	if (TLine_IsBin (pLine) && m_OffsetWidth)
	{
		const char* pOffset = FormatOffset ((CBinLine*) pLine);
		axl_rtl_TString_AppendString (pString, pOffset, -1);
		pString->Append (' ', EDef_OffsetGapSize);
	}

	switch (pLine->m_Type)
	{
	case ELine_Text:
		{
		TTextLine* pTextLine = (TTextLine*) pLine;
		size_t Length = axl_rtl_TString_GetLength (&pTextLine->m_HyperText.m_Text);
		axl_rtl_TString_AppendString (pString, pTextLine->m_HyperText.m_Text.m_p, -1);
		}
		break;

	case ELine_BinHex:
		{
		CBinLine* pBinLine = (CBinLine*) pLine;
		size_t Size = axl_rtl_TByteArray_GetCount (&pBinLine->m_BinData);

		axl_rtl_AppendEncodeHexString (pString, pBinLine->m_BinData.m_p, Size, 0);

		if (Size < pBinLine->m_BinDataConfig.m_BinHexLineSize)
			pString->Append (' ', (pBinLine->m_BinDataConfig.m_BinHexLineSize - Size) * 3);

		pString->Append (' ', 1 + EDef_HexGapSize);
		axl_rtl_AppendEncodeAsciiString (pString, pBinLine->m_BinData.m_p, Size, '.');
		}
		break;

	case ELine_BinText:
		{
		TBinTextLine* pBinLine = (TBinTextLine*) pLine;
		size_t Size = axl_rtl_TByteArray_GetCount (&pBinLine->m_BinData);
		axl_rtl_AppendEncodeAsciiString (pString, pBinLine->m_BinData.m_p, Size, '.');
		}
	}

	return axl_rtl_TString_GetLength (pString);
}

bool
CWidget::SaveAsTextFile (
	const char* pFileName,
	axl_rtl_TBinTree* pIconToCharMap
	)
{
	axl_TStatus Status;
	axl_rtl_TString String;
	size_t Length;
	CLine* pLine;

	axl_sys_TFile File;
	axl_sys_TFile_Construct (&File);

	Status = axl_sys_TFile_Open (&File, pFileName, 0);
	if (Status != axl_EStatus_Success)
	{
		axl_sys_TFile_Destruct (&File);
		return false;
	}

	axl_sys_TFile_SetSize (&File, 0);
	axl_rtl_TString_Construct (&String);

	pLine = GetLine (0);
	while (pLine)
	{
		GetLineString (pLine, &String, pIconToCharMap);
		Length = axl_rtl_TString_GetLength (&String);
		axl_sys_TFile_Write (&File, String.m_p, Length * sizeof (char));
		axl_sys_TFile_Write (&File, "\r\n", 2 * sizeof (char));
		pLine = GetNextLine (pLine);
	}

	axl_rtl_TString_Destruct (&String);
	axl_sys_TFile_Destruct (&File);
	return true;
}

*/

//.............................................................................

} // namespace log {
} // namespace axl {
