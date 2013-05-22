#include "pch.h"
#include "axl_log_CachePage.h"
#include "axl_log_BinLine.h"

namespace axl {
namespace log {

//.............................................................................

CCachePage::CCachePage ()
{
	m_pCacheMgr = NULL;

	m_PacketCount = 0;
	m_PacketOffset = 0;
	m_BinOffset = 0;
	m_PartIndex = 0;
	m_MergeId = 0;

	m_ColorizedLineCount = 0;
	m_ColorizerOffset = 0;
	m_pColorizerLine = NULL;
	m_ColorizerState = 0;

	m_IsRepresentNeeded = true;
	m_IsColorizeNeeded = true;
}

CCachePage::~CCachePage ()
{
	size_t Count = m_LineArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CLine* pLine = m_LineArray [i];
		AXL_MEM_DELETE (pLine);
	}

	Count = m_VolatilePacketArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		TCacheVolatilePacket* pMsg = m_VolatilePacketArray [i];
		AXL_MEM_DELETE (pMsg);
	}
}

CBinLine*
CCachePage::ScanUntilBinLine (CLine* pLine)
{
	while (pLine && !pLine->IsBin ())
		pLine = GetLine (pLine->m_PageIndex + 1);

	return (CBinLine*) pLine;
}

CBinLine*
CCachePage::FindBinLineByOffset (
	CBinLine* pLine,
	size_t Offset
	)
{
	// we cannot simply do Offset / LineSize because:
	// - it won't work for bin text lines 
	// - we dont want to leave the current page

	size_t LineCount = m_LineArray.GetCount ();
	for (;;) 
	{
		size_t DataSize = pLine->m_BinData.GetCount ();
		if (Offset >= pLine->m_Offset && Offset <= pLine->m_Offset + DataSize)
			return pLine;

		size_t iNext = pLine->m_PageIndex + 1;
		if (iNext >= LineCount)
			break;

		CLine* pNextLine = m_LineArray [iNext];
		if (!pLine->IsMerged (pNextLine))
			break;

		pLine = (CBinLine*) pNextLine;
	} 

	return NULL;
}

size_t 
CCachePage::GetVolatilePacketAnchorLine (TCacheVolatilePacket* pVolatilePacket)
{
	if (pVolatilePacket->m_pFirstLine) // shortcut
		return pVolatilePacket->m_pFirstLine->m_PageIndex;

	size_t LineCount = m_LineArray.GetCount ();
	for (size_t i = 0; i < LineCount; i++)
	{
		CLine* pLine = m_LineArray [i];
		if (pLine->m_FirstPacketOffset >= pVolatilePacket->m_Offset)
			return pLine->m_PageIndex;
	}

	return LineCount;
}

void
CCachePage::ClearColorization ()
{
	size_t LineCount = m_LineArray.GetCount ();
	for (size_t i = 0; i < LineCount; i++)
	{
		CLine* pLine = m_LineArray [i];

		if (pLine->IsBin ())
		{
			CBinLine* pBinLine = (CBinLine*) pLine;
			pBinLine->m_AttrArray = pBinLine->m_RepresentorAttrArray;
		}
	}

	m_ColorizerState = 0;
	m_ColorizerStateEx = ref::EPtr_Null;
	m_pColorizerLine = NULL;
	m_ColorizedLineCount = 0;
	m_ColorizerOffset = 0;
	m_IsColorizeNeeded = true;
}

void
CCachePage::AddLines (
	size_t Line,
	intptr_t LineDelta
	)
{
	if (!LineDelta)
		return;
	
	if (LineDelta < 0)
	{
		m_LineArray.Remove (Line, -LineDelta);
	}
	else
	{
		m_LineArray.Insert (Line, NULL, LineDelta);
		Line += LineDelta;
	}

	// fixup following line indices

	size_t LineCount = m_LineArray.GetCount ();
	for (; Line < LineCount; Line++)
	{
		CLine* pLine = m_LineArray [Line];

		if (pLine)
			pLine->m_PageIndex = Line;
	}
}

void
CCachePage::FreeLines (
	size_t Line,
	size_t LineCount
	)
{	
	size_t PageLineCount = m_LineArray.GetCount ();	
	if (Line >= PageLineCount)
		return;

	if (Line + LineCount > PageLineCount)
		LineCount = PageLineCount - Line;

	size_t End = Line + LineCount;
	for (size_t i = Line; i < End; i++)
	{
		CLine* pLine = m_LineArray [i];
		AXL_MEM_DELETE (pLine);
		m_LineArray [i] = NULL;
	}	
}

//.............................................................................

} // namespace log {
} // namespace axl {
