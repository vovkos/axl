#include "pch.h"
#include "axl_log_BinLine.h"
#include "axl_log_CachePage.h"

namespace axl {
namespace log { 

//.............................................................................

CBinLine::CBinLine ()
{
	m_BinOffset = 0;
}

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

void 
CBinLine::AddRetroColorizedPart (
	uint64_t BeginOffset, 
	uint64_t EndOffset, 
	const gui::TTextAttr& Attr
	)
{
	TBinLineColorizedPart Part;
	Part.m_BeginOffset = BeginOffset;
	Part.m_EndOffset = EndOffset;
	Part.m_Attr = Attr;
	m_RetroColorizedPartArray.Append (Part);	
}

void 
CBinLine::ApplyRetroColorization (
	const TBinLineColorizedPart* pPartArray,
	size_t PartCount
	)
{
	const TBinLineColorizedPart* pPart = pPartArray;

	for (size_t i = 0; i < PartCount; i++, pPart++)
	{
		if (DoesBinRangeOverlap (pPart->m_BeginOffset, pPart->m_EndOffset))
			Colorize (pPart->m_BeginOffset, pPart->m_EndOffset, pPart->m_Attr);
	}
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
	size_t ColorizedPartArrayOffset = AttrArrayOffset + pBinLine->m_AttrAnchorCount * sizeof (gui::TTextAttrAnchor);
	size_t LineSize = ColorizedPartArrayOffset + pBinLine->m_RetroColorizedPartCount * sizeof (TBinLineColorizedPart);
	
	if (Size < LineSize)
		return 0;

	m_BinOffset = pBinLine->m_BinOffset;
	m_BinDataConfig = pBinLine->m_BinDataConfig;
	m_BinData.Copy ((uchar_t*) ((char*) p + BinDataOffset), pBinLine->m_BinSize);	
	m_BinPartArray.Copy ((TBinLinePart*) ((char*) p + BinPartArrayOffset), pBinLine->m_BinPartCount);
	m_AttrArray.Copy ((gui::TTextAttrAnchor*) ((char*) p + AttrArrayOffset), pBinLine->m_AttrAnchorCount);
	m_RetroColorizedPartArray.Copy ((TBinLineColorizedPart*) ((char*) p + ColorizedPartArrayOffset), pBinLine->m_RetroColorizedPartCount);

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
	BinLine.m_AttrAnchorCount = m_AttrArray.GetCount ();	
	BinLine.m_RetroColorizedPartCount = m_RetroColorizedPartArray.GetCount ();	

	pBuffer->Append ((uint8_t*) &BinLine, sizeof (BinLine));
	pBuffer->Append ((uint8_t*) m_BinData.ca (), m_BinData.GetCount ());
	pBuffer->Append ((uint8_t*) m_BinPartArray.ca (), m_BinPartArray.GetCount () * sizeof (TBinLinePart));
	pBuffer->Append ((uint8_t*) m_AttrArray.ca (), m_AttrArray.GetCount () * sizeof (gui::TTextAttrAnchor));
	pBuffer->Append ((uint8_t*) m_RetroColorizedPartArray.ca (), m_RetroColorizedPartArray.GetCount () * sizeof (TBinLineColorizedPart));

	return pBuffer->GetCount ();
}

void
CBinLine::Clear ()
{
	CLine::Clear ();
	m_BinOffset = 0;
	m_BinDataConfig.SetDefaults ();
	m_BinData.Clear ();
	m_BinPartArray.Clear ();
	m_AttrArray.Clear ();
	m_RetroColorizedPartArray.Clear ();
}

//.............................................................................

} // namespace log {
} // namespace axl {
