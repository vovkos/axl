#include "pch.h"
#include "axl_log_CachePage.h"
#include "axl_log_CacheMgr.h"

namespace axl {
namespace log {

//.............................................................................

CCachePage::CCachePage ()
{
	m_pCacheMgr = NULL;
	m_Flags = 0;
}

/*

CCachePage::CCachePage ()
{
	m_Flags = 0;
	m_PacketOffset = -1;
	m_IndexNodeOffset = -1;
	memset (&m_IndexLeaf, 0, sizeof (m_IndexLeaf));
	m_IndexLeaf.m_DefBinDataConfig.SetDefaults ();

	m_LongestTextLineLength = 0;
	m_LongestBinHexLineSize = 0;
	m_LongestBinTextLineSize = 0;

	m_ColorizedLineCount = 0;
	m_ColorizerLine = -1;
	m_ColorizerOffset = 0;
	m_ColorizerState = 0;
}

 */

void
CCachePage::Clear ()
{
	FreeLines (0, m_LineArray.GetCount ());

	m_Flags = 0;
	m_LineArray.Clear ();
	m_FoldablePacketArray.Clear ();
	
/*
	m_PacketOffset = -1;
	m_IndexNodeOffset = -1;
	memset (&m_IndexLeaf, 0, sizeof (m_IndexLeaf));
	m_IndexLeaf.m_DefBinDataConfig.SetDefaults ();

	m_IndexVolatilePacketArray.Clear ();

	m_LongestTextLineLength = 0;
	m_LongestBinHexLineSize = 0;
	m_LongestBinTextLineSize = 0;

	m_ColorizedLineCount = 0;
	m_ColorizerLine = -1;
	m_ColorizerOffset = 0;
	m_ColorizerState = 0;
	m_ColorizerStateEx = ref::EPtr_Null;
 */
}

/*

size_t
CCachePage::GetVolatilePacketIdx (const TPageVolatilePacket* pPacket)
{
	size_t Count = m_VolatilePacketArray.GetCount ();
	if (Count && m_VolatilePacketArray [Count - 1].m_Offset == pPacket->m_Offset)
		return Count - 1;
	
	m_VolatilePacketArray.Append (*pPacket);
	return Count;
}

*/

CBinLine*
CCachePage::FindFirstBinLine (size_t i)
{
	size_t Count = m_LineArray.GetCount ();
	for (; i < Count; i++)
	{
		CLine* pLine = m_LineArray [i];
		if (pLine->IsBin ())
			return (CBinLine*) pLine;
	}

	return NULL;
}

void
CCachePage::AddLines (
	size_t LineIdx,
	intptr_t LineCount
	)
{
	if (!LineCount)
		return;
	
	m_IndexLeaf.m_LineCount += LineCount;

	if (LineCount < 0)
	{
		m_LineArray.Remove (LineIdx, -LineCount);
	}
	else
	{
		m_LineArray.Insert (LineIdx, NULL, LineCount);
		LineIdx += LineCount;
	}

	// fixup following line indices

	size_t PageLineCount = m_LineArray.GetCount ();
	for (size_t i = LineIdx; i < PageLineCount; i++)
	{
		CLine* pLine = m_LineArray [i];

		if (pLine)
			pLine->m_LineIdx = i;
	}
}

void
CCachePage::FreeLines (
	size_t LineIdx,
	size_t LineCount
	)
{	
	size_t PageLineCount = m_LineArray.GetCount ();	
	if (LineIdx >= PageLineCount)
		return;

	size_t EndIdx = LineIdx + LineCount;

	if (EndIdx > PageLineCount)
		EndIdx = PageLineCount;

	for (size_t i = LineIdx; i < EndIdx; i++)
	{
		CLine* pLine = m_LineArray [i];
		if (pLine)
		{
			AXL_MEM_DELETE (pLine);		
			m_LineArray [i] = NULL;
		}
	}	
}

void
CCachePage::LoadLines (
	size_t LineIdx,
	size_t LineCount,
	const void* pLineBuffer,
	size_t LineBufferSize,
	TLongestLineLength* pLength,
	bool* pIsRetroColorized
	)
{
	const char* p = (const char*) pLineBuffer;
	size_t Size = LineBufferSize;

	// fill the page starting with LineIdx to IndexNodeLineCount with data from p

	ASSERT (LineIdx + LineCount <= m_IndexLeaf.m_LineCount);

	size_t i = LineIdx;
	size_t j = 0;
	for (; i < m_IndexLeaf.m_LineCount && j < LineCount; i++, j++)
	{
		if (Size < sizeof (TLine))
			break;

		CLine* pLine = NULL;

		ELine LineKind = ((TLine*) p)->m_LineKind;
		switch (LineKind)
		{
		case ELine_Text:
			pLine = AXL_MEM_NEW (CTextLine);
			break;

		case ELine_BinHex:
			pLine = AXL_MEM_NEW (CBinHexLine);
			break;

		case ELine_BinText:
			pLine = AXL_MEM_NEW (CBinTextLine);
			break;
		}

		if (!pLine)
			break;

		CLine* pOldLine = m_LineArray [i];
		if (pOldLine)
			AXL_MEM_DELETE (pOldLine);

		m_LineArray [i] = pLine;
		pLine->m_pPage = this;
		pLine->m_LineIdx = i;

		size_t LineSize = pLine->Load (p, Size);
		if (LineSize == -1)
			break;

		if (pLine->IsBin ())
		{
			CBinLine* pBinLine = (CBinLine*) pLine;
			if (pBinLine->HasRetroColorizedParts ())
			{
				CLine* pPrevLine = m_pCacheMgr->GetPrevLine (pBinLine);
				
				if (pPrevLine && pPrevLine->IsBin ())
				{
					CBinLine* pPrevBinLine = (CBinLine*) pPrevLine;

					pPrevBinLine->ApplyRetroColorization (
						pBinLine->m_RetroColorizedPartArray,
						pBinLine->m_RetroColorizedPartArray.GetCount ()
						);

					if (pIsRetroColorized && i == LineIdx)
						*pIsRetroColorized = true;
				}

			}
		}

		if (pLength)
			pLine->UpdateLongestLineLength (pLength);

		ASSERT (LineSize <= Size);

		p += LineSize;
		Size -= LineSize;
	}
}

//.............................................................................

} // namespace log {
} // namespace axl {
