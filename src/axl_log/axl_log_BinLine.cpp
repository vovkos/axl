#include "pch.h"
#include "axl_log_BinLine.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log { 

//.............................................................................

TBinLinePart* 
CBinLine::FindPart (size_t Offset)
{
	TBinLinePart* pResult = NULL;

	size_t Begin = 0;
	size_t End = m_PartArray.GetCount ();

	while (Begin < End)
	{
		size_t Mid = (Begin + End) / 2;

		TBinLinePart* pPart = &m_PartArray [Mid];
		if (pPart->m_Offset == Offset)
			return pPart;

		if (pPart->m_Offset < Offset)
		{
			pResult = pPart;
			Begin = Mid + 1;
		}
		else
		{
			End = Mid;
		}
	}

	return pResult;
}

int
CBinLine::CmpOffset (size_t Offset) // full offset
{
	size_t DataSize = m_BinData.GetCount ();
	return 
		Offset < m_Offset ? -1 :
		Offset > m_Offset + DataSize ? 1 : 0;
}

void 
CBinLine::AddData (
	uint64_t Timestamp,
	const gui::TTextAttr& Attr,
	const void* p, 
	size_t Size
	)
{
	if (!m_BinData.IsEmpty ())
	{
		size_t Offset = m_Offset + m_BinData.GetCount ();
		m_PartArray.Append (TBinLinePart (Offset, Timestamp));
	}

	size_t Offset = m_BinData.GetCount ();

	m_BinData.Append ((uchar_t*) p, Size);
	// m_RepresentorAttrArray.SetAttr (Attr, Offset, Offset + Size);
}

void
CBinLine::Colorize (
	const gui::TTextAttr& Attr,
	size_t OffsetStart,
	size_t OffsetEnd,
	size_t Metric
	)
{
	if (m_LineKind == ELine_BinText) // need to convert offset -> col
	{	
		CBinTextLine* pBinTextLine = (CBinTextLine*) this;
		OffsetStart = pBinTextLine->FindColByOffset (OffsetStart);
		OffsetEnd = pBinTextLine->FindColByOffset (OffsetEnd);
	}
	else
	{
		size_t LineSize = m_BinData.GetCount ();
		if (OffsetStart > LineSize)
			return;

		if (OffsetEnd > LineSize)
			OffsetEnd = LineSize;
	}

	m_AttrArray.SetAttr (Attr, OffsetStart, OffsetEnd, Metric);
}

size_t
CBinLine::GetBinHexLineOffset (
	size_t Col,
	size_t* pHexCol
	)
{
	size_t Offset;
	size_t HexCol;

	size_t Size = m_BinData.GetCount ();

	size_t FullOffsetWidth = GetWidget ()->GetFullOffsetWidth ();
	size_t HexGapSize  = GetWidget ()->GetHexGapSize ();

	if (Col < FullOffsetWidth)
		Col = 0;
	else
		Col -= FullOffsetWidth;
	
	if (Col <= (size_t) m_BinDataConfig.m_BinHexLineSize * 3)
	{
		size_t End = (Size - 1) * 3 + 2;
		
		if (Col >= End)
			Col = End;

		Offset = Col / 3;
		HexCol = Col % 3;
	}
	else
	{
		size_t AsciiCol = m_BinDataConfig.m_BinHexLineSize * 3 + HexGapSize;

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

bool
CBinLine::GetBinLineOffset (
	size_t Col,
	size_t* pOffset,
	size_t* pLineOffset,
	size_t* pHexCol,
	size_t* pMergeId
	)
{
	size_t LineOffset;

	switch (m_LineKind)
	{
	case ELine_BinHex:
		LineOffset = GetBinHexLineOffset (Col, pHexCol);
		break;

	case ELine_BinText:
		LineOffset = ((CBinTextLine*) this)->GetBinTextLineOffset (Col);
		if (pHexCol)
			*pHexCol = 0;
		break;

	default:
		return false;
	}

	if (pLineOffset)
		*pLineOffset = LineOffset;

	if (pOffset)
		*pOffset = m_Offset + LineOffset;

	if (pMergeId)
		*pMergeId = m_MergeId;

	return true;
}

//.............................................................................

size_t
CBinTextLine::GetBinTextLineOffset (size_t Col)
{
	size_t ColCount = m_BinTextMap.GetCount ();
	if (!ColCount)
		return 0;

	size_t FullOffsetWidth = GetWidget ()->GetFullOffsetWidth ();

	if (Col < FullOffsetWidth)
		Col = 0;
	else
		Col -= FullOffsetWidth;

	if (Col >= ColCount)
		return m_BinData.GetCount ();

	return m_BinTextMap [Col].m_Offset;
}

size_t 
CBinTextLine::FindOffsetByCol (size_t Col)
{
	size_t Size = m_BinData.GetCount ();
	size_t Count = m_BinTextMap.GetCount ();
	
	if (!Count)
		return 0;

	if (Col >= Count)
		return Size;

	return m_BinTextMap [Col].m_Offset;
}

size_t 
CBinTextLine::FindColByOffset (size_t Offset)
{
	size_t Size = m_BinData.GetCount ();
	size_t Count = m_BinTextMap.GetCount ();
	size_t Begin = 0;
	size_t End = Count;

	TLogBinTextMapEntry* pMapEntry = NULL;

	if (!Count)
		return 0;

	if (Offset >= Size)
		return Count;

	while (Begin < End)
	{
		size_t Mid = (Begin + End) / 2;
		pMapEntry = &m_BinTextMap [Mid];

		if (pMapEntry->m_Offset == Offset)
			break;

		if (pMapEntry->m_Offset < Offset)
			Begin = Mid + 1;			
		else
			End = Mid;			
	}

	return pMapEntry->m_Col;
}

size_t
CBinTextLine::AddData (
	uint64_t Timestamp,
	const gui::TTextAttr& Attr,
	const void* _p, 
	size_t Size
	)
{
	enum 
	{ 
		UnprintableChar = '.' 
	};

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;
	size_t Offset = m_BinData.GetCount ();

	if (!m_BinData.IsEmpty ())
		m_PartArray.Append (TBinLinePart (m_Offset + Offset, Timestamp));

	for (; p < pEnd; p++, Offset++)
	{
		TLogBinTextMapEntry MapEntry;

		size_t TextLength = m_BinText.GetLength ();

		size_t i;
		size_t TabSize;

		uchar_t x = *p;

		if (x == '\r')
			continue;

		if (x == '\n')
		{
			p++, Offset++; // still add this character to the line
			break;
		}

		if (TextLength >= m_BinDataConfig.m_BinTextLineSize)
			break;

		MapEntry.m_Col = TextLength;
		MapEntry.m_Offset = Offset;

		if (x != '\t')
		{
			char Char = isprint (x) ? (char) x : UnprintableChar;

			m_BinText.Append (Char);
			m_BinTextMap.Append (MapEntry);
			continue;
		}

		TabSize = m_BinDataConfig.m_BinTextTabSize - TextLength % m_BinDataConfig.m_BinTextTabSize;

		if (TextLength + TabSize > m_BinDataConfig.m_BinTextLineSize)
			TabSize = m_BinDataConfig.m_BinTextLineSize - TextLength;

		for (i = 0; i <= TabSize / 2; i++)
			m_BinTextMap.Append (MapEntry);

		MapEntry.m_Col = TextLength + TabSize;
		MapEntry.m_Offset++;

		for (; i < TabSize; i++)
			m_BinTextMap.Append (MapEntry);

		m_BinText.Append (' ', TabSize);
	}

	size_t ActualSize = p - (uchar_t*) _p;
	
	Offset = m_BinData.GetCount ();
	m_BinData.Append ((uchar_t*) _p, ActualSize);
	m_RepresentorAttrArray.SetAttr (Attr, Offset, Offset + ActualSize);
	return ActualSize;
}

//.............................................................................

} // namespace log {
} // namespace axl {
