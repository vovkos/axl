#include "pch.h"
#include "axl_log_BinTextLine.h"

namespace axl {
namespace log { 

//.............................................................................

size_t 
CBinTextLine::FindOffsetByBinTextCol (size_t Col)
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
CBinTextLine::FindBinTextColByOffset (size_t Offset)
{
	size_t Size = m_BinData.GetCount ();
	size_t Count = m_BinTextMap.GetCount ();
	size_t Begin = 0;
	size_t End = Count;

	TBinTextMapEntry* pMapEntry = NULL;

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
CBinTextLine::AddBinData (
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
	{
		TBinLinePart Part;
		Part.m_Offset = m_BinOffset + Offset;
		Part.m_Timestamp = Timestamp;

		m_BinPartArray.Append (Part);
	}

	const TBinDataConfig* pBinDataConfig = GetBinDataConfig ();

	for (; p < pEnd; p++, Offset++)
	{
		TBinTextMapEntry MapEntry;

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

		if (TextLength >= pBinDataConfig->m_BinTextLineSize)
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

		TabSize = pBinDataConfig->m_BinTextTabSize - TextLength % pBinDataConfig->m_BinTextTabSize;

		if (TextLength + TabSize > pBinDataConfig->m_BinTextLineSize)
			TabSize = pBinDataConfig->m_BinTextLineSize - TextLength;

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
	
	m_AttrArray.SetAttr (Offset, Offset + ActualSize, Attr);

	return ActualSize;
}

void
CBinTextLine::Colorize (
	uint64_t BeginOffset,
	uint64_t EndOffset,
	const gui::TTextAttr& Attr
	)
{
	if (BeginOffset < m_BinOffset)
		m_BinOffset = BeginOffset;

	size_t ColStart = FindBinTextColByOffset ((size_t) (BeginOffset - m_BinOffset));
	size_t ColEnd = FindBinTextColByOffset ((size_t) (EndOffset - m_BinOffset));

	m_AttrArray.SetAttr (ColStart, ColEnd, Attr, 1);
}

size_t
CBinTextLine::Load (
	const void* p,
	size_t Size
	)
{
	size_t Offset = CBinLine::Load (p, Size);
	if (!Offset || Size < Offset + sizeof (TBinTextLine))
		return 0;	
	
	TBinTextLine* pBinTextLine = (TBinTextLine*) ((char*) p + Offset);

	size_t BinTextOffset = Offset + sizeof (TBinTextLine);
	size_t BinTextMapOffset = BinTextOffset + pBinTextLine->m_BinTextLength;
	size_t LineSize = BinTextMapOffset + pBinTextLine->m_BinTextMapEntryCount * sizeof (TBinTextMapEntry);
	
	if (Size < LineSize)
		return 0;

	m_BinText.Copy ((char*) p + BinTextOffset, pBinTextLine->m_BinTextLength);	
	m_BinTextMap.Copy ((TBinTextMapEntry*) ((char*) p + BinTextMapOffset), pBinTextLine->m_BinTextMapEntryCount);

	return LineSize;
}

size_t
CBinTextLine::Save (rtl::CArrayT <uint8_t>* pBuffer)
{
	CBinLine::Save (pBuffer);

	TBinTextLine BinTextLine;
	BinTextLine.m_BinTextLength = m_BinText.GetLength ();
	BinTextLine.m_BinTextMapEntryCount = m_BinTextMap.GetCount ();	

	pBuffer->Append ((uint8_t*) &BinTextLine, sizeof (BinTextLine));
	pBuffer->Append ((uint8_t*) m_BinText.cc (), m_BinText.GetLength ());
	pBuffer->Append ((uint8_t*) m_BinTextMap.ca (), m_BinTextMap.GetCount () * sizeof (TBinTextMapEntry));

	return pBuffer->GetCount ();
}

void
CBinTextLine::Clear ()
{
	CBinLine::Clear ();
	m_BinText.Clear ();
	m_BinTextMap.Clear ();
}

void
CBinTextLine::UpdateLongestLineLength (TLongestLineLength* pLength)
{
	if (m_BinDataConfig.m_BinTextLineSize > pLength->m_BinTextLineSize)
		pLength->m_BinTextLineSize = m_BinDataConfig.m_BinTextLineSize;
}

//.............................................................................

} // namespace log {
} // namespace axl {
