#include "pch.h"
#include "axl_log_Widget_Copy.h"
#include "axl_rtl_HexEncoding.h"
#include "axl_gui_Engine.h"

#if FDFDFDFDfdsdffdsJHJ
#else
#endif


namespace axl {
namespace log {

//.............................................................................

bool
CWidget::Copy ()
{
	rtl::CString String;

	size_t LineCount = CopyString (&String);
	if (!LineCount || String.IsEmpty ())
	{
#if (_AXL_ENV == AXL_ENV_WIN)
		::MessageBeep (MB_OK);
#endif
		return true;
	}

	if (LineCount >= 2) 
		String.Append ("\r\n", 2);

	return m_pEngine->WriteClipboard (String);
}

size_t 
CWidget::CopyString (rtl::CString* pString)
{
	bool_t IsSelectionBinBlock = GetSelectionBinBlock (NULL, NULL);
	if (!IsSelectionBinBlock)
	{
		CProcessCopy Process;
		Process.m_pString = pString;
		return ProcessSelection (&Process);
	}

	CBinLine* pStartLine = (CBinLine*) m_CacheMgr.GetLine (m_SelStart.m_Line);
	CBinLine* pEndLine = (CBinLine*) m_CacheMgr.GetLine (m_SelEnd.m_Line);

	ASSERT (pStartLine->IsBin () && pEndLine->IsBin ());

	if (pStartLine->m_LineKind == ELine_BinHex)
	{
		size_t AsciiCol = GetAsciiCol (pStartLine);
		if (m_SelStart.m_Col_u < AsciiCol && m_SelEnd.m_Col_u < AsciiCol) // hex only
		{
			CProcessBinHexCopy Process;
			Process.m_pString = pString;
			return ProcessSelection (&Process);
		}
		
		if (m_SelStart.m_Col_u < AsciiCol && m_SelEnd.m_Col_u >= AsciiCol ||
			m_SelStart.m_Col_u >= AsciiCol && m_SelEnd.m_Col_u < AsciiCol) // combo
		{
			CProcessBinHexAsciiCopy Process;
			Process.m_pString = pString;
			return ProcessSelection (&Process);
		}
	}

	CProcessBinCopy Process;
	Process.m_pString = pString;
	ProcessSelectionAsBinBlock (&Process);
	return 1; // we copy it as binary block
}

//.............................................................................

void 
CProcessCopy::Process (
	CLine* pLine, 
	CLine* pPrevLine, 
	size_t FromCol, 
	size_t ToCol
	)
{
	ASSERT (FromCol >= 0);
	ASSERT (ToCol >= FromCol || ToCol == -1);

	switch (pLine->GetLineKind ())
	{
	case ELine_Text:
		if (pPrevLine)
			m_pString->Append ("\r\n", 2);

		TextCopy ((CTextLine*) pLine, FromCol, ToCol);
		break;

	case ELine_BinHex:
		if (pPrevLine)
			m_pString->Append ("\r\n", 2);

		BinHexAsciiCopy ((CBinLine*) pLine, FromCol, ToCol, false);
		break;

	case ELine_BinText:
		if (pPrevLine && pPrevLine->GetLineKind () != ELine_BinText)
			m_pString->Append ("\r\n", 2);

		BinTextCopy ((CBinTextLine*) pLine, FromCol, ToCol);
		break;
	}
}

void 
CProcessCopy::TextCopy (
	CTextLine* pLine, 
	size_t FromCol, 
	size_t ToCol
	)
{
	rtl::CString Text = pLine->GetHyperText ()->GetText ();
	size_t Length = Text.GetLength ();

	if (ToCol > Length)
		ToCol = Length;

	if (FromCol < ToCol)
		m_pString->Append (Text.cc () + FromCol, ToCol - FromCol);
}

void 
CProcessCopy::BinHexAsciiCopy (
	CBinLine* pLine,
	size_t FromCol, 
	size_t ToCol,
	bool HexOnly
	)
{
	size_t FromOffset;
	size_t ToOffset;
	size_t FromHexCol;
	size_t ToHexCol;

	FromOffset = pLine->GetBinHexLineOffset (FromCol, &FromHexCol);
	ToOffset = pLine->GetBinHexLineOffset (ToCol, &ToHexCol);

	if (FromHexCol == 2)
		FromOffset++;

	if (ToHexCol)
		ToOffset++;

	if (ToCol == -1)
		m_pString->Append (' ', FromOffset * 3);

	if (FromOffset < ToOffset)
	{
		rtl::CArrayT <uchar_t> BinData = pLine->GetBinData ();

		if (HexOnly)
		{
			m_pString->Append (rtl::CHexEncoding::Encode ((uchar_t*) BinData  + FromOffset, ToOffset - FromOffset));
		}
		else
		{
			const TBinDataConfig* pBinDataConfig = pLine->GetBinDataConfig ();
			size_t HexGapSize = pLine->GetWidget ()->GetHexGapSize ();

			m_pString->Append (rtl::CHexEncoding::Encode ((uchar_t*) BinData + FromOffset, ToOffset - FromOffset));

			if (ToOffset < pBinDataConfig->m_BinHexLineSize)
				m_pString->Append (' ', (pBinDataConfig->m_BinHexLineSize - ToOffset) * 3);

			m_pString->Append (' ', 1 + HexGapSize + FromOffset);

			rtl::CString AsciiString ((char*) (uchar_t*) BinData + FromOffset, ToOffset - FromOffset);
			// AsciiString.ReplaceUnprintable ('.');
			m_pString->Append (AsciiString);
		}
	}
}

void 
CProcessCopy::BinTextCopy (
	CBinTextLine* pLine, 
	size_t FromCol, 
	size_t ToCol
	)
{
	rtl::CArrayT <uchar_t> BinData = pLine->GetBinData ();

	size_t Size = BinData.GetCount ();

	size_t FromOffset = pLine->FindOffsetByCol (FromCol);
	size_t ToOffset = pLine->FindOffsetByCol (ToCol);

	if (FromOffset < ToOffset)
		m_pString->Append ((char*) ((uchar_t*) BinData + FromOffset), ToOffset - FromOffset);
}

//.............................................................................

void 
CProcessBinHexCopy::Process (
	CLine* pLine, 
	CLine* pPrevLine, 
	size_t FromCol, 
	size_t ToCol
	)
{
	ASSERT (pLine->GetLineKind () == ELine_BinHex);

	if (pPrevLine)
		m_pString->Append ("\r\n", 2);

	BinHexAsciiCopy ((CBinLine*) pLine, FromCol, ToCol, true);
}

//.............................................................................

void 
CProcessBinHexAsciiCopy::Process (
	CLine* pLine, 
	CLine* pPrevLine, 
	size_t FromCol, 
	size_t ToCol
	)
{
	ASSERT (pLine->GetLineKind () == ELine_BinHex);

	if (pPrevLine)
		m_pString->Append ("\r\n", 2);

	BinHexAsciiCopy ((CBinLine*) pLine, FromCol, ToCol, false);
}

//.............................................................................

} // namespace log {
} // namespace axl {
