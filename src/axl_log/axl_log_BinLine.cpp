#include "pch.h"
#include "axl_log_BinLine.h"
#include "axl_log_Page.h"

namespace axl {
namespace log { 

//.............................................................................

const TBinLinePart* 
CBinLine::FindBinPart (size_t Offset)
{
	const TBinLinePart* pResult = NULL;

	size_t Begin = 0;
	size_t End = m_BinPartArray.GetCount ();

	while (Begin < End)
	{
		size_t Mid = (Begin + End) / 2;

		const TBinLinePart* pPart = &m_BinPartArray [Mid];
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
CBinLine::CmpBinOffset (size_t Offset) // full offset
{
	size_t DataSize = m_BinData.GetCount ();
	return 
		Offset < m_BinOffset ? -1 :
		Offset > m_BinOffset + DataSize ? 1 : 0;
}

size_t
CBinLine::Load (
	const void* p,
	size_t Size
	)
{
	size_t Offset = CLine::Load (p, Size);
	if (!Offset || Size < Offset + sizeof (TBinLine))
		return 0;	
	
	TBinLine* pBinLine = (TBinLine*) ((char*) p + Offset);

	size_t BinDataOffset = Offset + sizeof (TBinLine);
	size_t BinPartArrayOffset = BinDataOffset + pBinLine->m_BinSize;
	size_t AttrArrayOffset = BinPartArrayOffset + pBinLine->m_BinPartCount * sizeof (TBinLinePart);
	size_t LineSize = AttrArrayOffset + 	pBinLine->m_AttrAnchorCount * sizeof (gui::TTextAttrAnchor);
	
	if (Size < LineSize)
		return 0;

	m_BinOffset = pBinLine->m_BinOffset;
	m_BinDataConfig = pBinLine->m_BinDataConfig;
	m_BinData.Copy ((uchar_t*) ((char*) p + BinDataOffset), pBinLine->m_BinSize);	
	m_BinPartArray.Copy ((TBinLinePart*) ((char*) p + BinPartArrayOffset), pBinLine->m_BinPartCount);
	m_OriginalAttrArray.Copy ((gui::TTextAttrAnchor*) ((char*) p + BinPartArrayOffset), pBinLine->m_AttrAnchorCount);
	m_AttrArray = m_OriginalAttrArray;

	return LineSize;
}

size_t
CBinLine::Save (rtl::CArrayT <uint8_t>* pBuffer)
{
	CLine::Save (pBuffer);

	TBinLine BinLine;
	BinLine.m_BinOffset = m_BinOffset;
	BinLine.m_BinDataConfig = m_BinDataConfig;
	BinLine.m_BinSize = m_BinData.GetCount ();
	BinLine.m_BinPartCount = m_BinPartArray.GetCount ();
	BinLine.m_AttrAnchorCount = m_OriginalAttrArray.GetCount ();	

	pBuffer->Append ((uint8_t*) &BinLine, sizeof (BinLine));
	pBuffer->Append ((uint8_t*) m_BinData.ca (), m_BinData.GetCount ());
	pBuffer->Append ((uint8_t*) m_BinPartArray.ca (), m_BinPartArray.GetCount () * sizeof (TBinLinePart));
	pBuffer->Append ((uint8_t*) m_OriginalAttrArray.ca (), m_OriginalAttrArray.GetCount () * sizeof (gui::TTextAttrAnchor));

	return pBuffer->GetCount ();
}

//.............................................................................

} // namespace log {
} // namespace axl {
