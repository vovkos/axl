#include "pch.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_log_CacheMgr.h"

namespace axl {
namespace log {

//.............................................................................

CColorizeMgr::CColorizeMgr ()
{
	m_pCacheMgr = NULL;
	m_pColorizer = NULL;
	m_CrossoverLimit = 4; // don't look further than 4 lines back/forth on page borders
	m_IsInvalidateRequired = true;
}

void
CColorizeMgr::SetColorizer (IColorizer* pColorizer)
{
	m_pColorizer = pColorizer;
	m_pCacheMgr->ClearCacheColorization ();
}

bool
CColorizeMgr::ColorizePage (CCachePage* pPage)
{
	CLine* pLine;

	if (!pPage->m_IsColorizeNeeded || !m_pColorizer)
		return true;

	m_IsInvalidateRequired = false;

	if (pPage->m_pColorizerLine) // incremental colorize
		pLine = ColorizeBlock (pPage, pPage->m_pColorizerLine, pPage->m_ColorizerOffset);
	else 
		pLine = pPage->GetLine (pPage->m_ColorizedLineCount);

	if (!pPage->m_ColorizedLineCount && pLine->IsBin ())
	{
		pLine = Backtrack (pLine);
		ASSERT (pLine->IsBin ());
		pLine = ColorizeBlock (pPage, (CBinLine*) pLine, 0);
	}

	while (pLine && pLine->m_pPage == pPage)
	{
		if (!pLine->IsBin ())
			pLine = m_pCacheMgr->GetNextLine (pLine);
		else
			pLine = ColorizeBlock (pPage, (CBinLine*) pLine, 0);
	}

	pPage->m_ColorizedLineCount = pPage->GetLineCount ();
	pPage->m_IsColorizeNeeded = false;

	//if (m_IsInvalidateRequired)
	//	Invalidate (m_pLogCtrl);

	return true;
}

bool
CColorizeMgr::ReColorizeLineRange (
	CCachePage* pPage,
	size_t FirstLine,
	size_t LineCount
	)
{
	CLine* pLine = pPage->GetLine (FirstLine);

	size_t LastLine = FirstLine + LineCount;

	if (!m_pColorizer)
		return true;

	while (pLine && pLine->m_pPage == pPage && pLine->m_PageIndex < LastLine)
	{
		if (!pLine->IsBin ())
			pLine = m_pCacheMgr->GetNextLine (pLine);
		else
			pLine = ColorizeBlock (pPage, (CBinLine*) pLine, 0);
	}

	return true;
}

void
CColorizeMgr::CollectLine (
	TColorizeBlock* pBlock,
	CBinLine* pLine
	)
{
	size_t PartIndex = pBlock->m_PartIndex + m_PartBuffer.GetCount (); 

	if (pLine->m_PartIndex != PartIndex)
		m_PartBuffer.Append (TBinLinePart (pLine->m_Offset, pLine->m_FirstTimestamp));

	m_DataBuffer.Append (pLine->m_BinData);
	m_PartBuffer.Append (pLine->m_PartArray);
}

void
CColorizeMgr::CollectFirstLine (
	TColorizeBlock* pBlock,
	CBinLine* pLine,
	size_t LineOffset
	)
{
	size_t DataSize = pLine->m_BinData.GetCount ();
	size_t PartCount = pLine->m_PartArray.GetCount ();
	size_t PartIndex;

	TBinLinePart* pPart;

	pBlock->m_PartCode = pLine->m_PartCode;
	pBlock->m_Offset = pLine->m_Offset + LineOffset;
	pBlock->m_PartIndex = pLine->m_PartIndex; 
	pBlock->m_Timestamp = pLine->m_FirstTimestamp;

	if (!LineOffset)
	{
		CollectLine (pBlock, pLine);
		return;
	}

	if (LineOffset >= DataSize) 
	{
		pBlock->m_PartIndex = pLine->m_PartIndex + pLine->m_PartArray.GetCount (); 
		pBlock->m_Timestamp = pLine->m_LastTimestamp;
		return;
	}

	m_DataBuffer.Append (
		pLine->m_BinData + LineOffset, 
		DataSize - LineOffset
		);

	pPart = pLine->FindPart (pLine->m_Offset + LineOffset);
	
	if (!pPart)
	{
		// should never be here : base offset always should be in between parts
		// however, otherwise situation doesnt't logically contradict anything, so
		ASSERT (false); 
		m_PartBuffer.Append (pLine->m_PartArray);
		return;
	}

	PartIndex = pPart - pLine->m_PartArray;
	ASSERT (PartIndex < PartCount);

	pBlock->m_PartIndex = pLine->m_PartIndex + PartIndex + 1; 	
	pBlock->m_Timestamp = pPart->m_Timestamp;

	pPart++;
	PartIndex++;

	m_PartBuffer.Append (pPart, PartCount - PartIndex);
}

CBinLine*
CColorizeMgr::CollectBlock (
	TColorizeBlock* pBlock,
	CCachePage* pPage,
	CBinLine* pLine,
	size_t LineOffset
	)
{
	CLine* pNextLine = m_pCacheMgr->GetNextLine (pLine);

	m_DataBuffer.Clear ();
	m_PartBuffer.Clear ();

	pBlock->m_PartCode = pLine->m_PartCode;
	pBlock->m_Offset = pLine->m_Offset + LineOffset;
	pBlock->m_PartIndex = pLine->m_PartIndex; 
	pBlock->m_Timestamp = pLine->m_FirstTimestamp;

	CollectFirstLine (pBlock, pLine, LineOffset);

	if (pLine->m_pPage != pPage) // backtracked line
	{
	    // collect crossover data till we get back to our page

		while (pNextLine && pNextLine->m_pPage != pPage)
		{
			ASSERT (pLine->IsMerged (pNextLine));

			pLine = (CBinLine*) pNextLine;		
			CollectLine (pBlock, pLine);

			pNextLine = m_pCacheMgr->GetNextLine (pLine);
		}

		ASSERT (pLine->IsMerged (pNextLine));
		ASSERT (pNextLine->m_pPage == pPage);
		ASSERT (pNextLine == pPage->GetLine (0));

		pLine = (CBinLine*) pNextLine;
	}
 
	// data on the page

	while (
		pNextLine && 
		pNextLine->m_pPage == pPage &&
		pLine->IsMerged (pNextLine)
		)
	{
		pLine = (CBinLine*) pNextLine;		
		CollectLine (pBlock, pLine);

		pNextLine = m_pCacheMgr->GetNextLine (pLine);
	}

	if (pNextLine && pNextLine->m_pPage != pPage)
	{
	    // crossover data on the next page 

		size_t i = 0;

		while (
			i < m_CrossoverLimit &&
			pNextLine && 
			pLine->IsMerged (pNextLine)
			)
		{
			pLine = (CBinLine*) pNextLine;
			CollectLine (pBlock, pLine);

			pNextLine = m_pCacheMgr->GetNextLine (pLine);
			i++;
		}
	}

	pBlock->m_p = m_DataBuffer;
	pBlock->m_Size = m_DataBuffer.GetCount ();
	pBlock->m_pPartArray = m_PartBuffer;
	pBlock->m_PartCount = m_PartBuffer.GetCount ();

	return pLine;
}

// fills LineCount line AFTER pAnchorLine

void
CColorizeMgr::FillLineAttr (
	const gui::TTextAttr& Attr,
	size_t Metric,
	CBinLine* pAnchorLine,
	size_t LineCount
	)
{
	CBinLine* pLine = (CBinLine*) m_pCacheMgr->GetNextLine (pAnchorLine);

	size_t i;
	
	for (i = 0; i < LineCount && pLine && pLine->m_pPage == pAnchorLine->m_pPage; i++)
	{
		ASSERT (pLine->m_LineKind == pAnchorLine->m_LineKind);
		ASSERT (pLine->m_PartCode == pAnchorLine->m_PartCode);

		pLine->Colorize (Attr, 0, -1, Metric);
		pLine = (CBinLine*) m_pCacheMgr->GetNextLine (pLine);
	}
}

/*

void TraceAttrAnchorArray (TTextAttrAnchorArray* pArray)
{
	TTextAttrAnchor* pAnchor = pArray->m_p;
	TTextAttrAnchor* pEnd = pAnchor + axl_rtl_TArray_GetCount (pArray);

	axl_dbg_Trace ("TraceAttrAnchorArray (%08x)...\n", pArray);

	for (; pAnchor < pEnd; pAnchor++)
	{
		axl_dbg_Trace ("    %04x -- back (%08x)\n", pAnchor->m_Offset, pAnchor->m_Attr.m_BackColor);
	}
}

*/

CBinLine*
CColorizeMgr::FindLine (
	CCachePage* pPage,
	CBinLine* pLine,
	size_t Offset
	)
{		
	size_t i = 0;

	int Cmp = pLine->CmpOffset (Offset);
	if (Cmp < 0)
	{
		m_IsInvalidateRequired = true;

		while (Cmp < 0 && i < m_CrossoverLimit)
		{
			CLine* pPrevLine = m_pCacheMgr->GetPrevLine (pLine);
			if (!pPrevLine || !pPrevLine->IsMerged (pLine))
				break;
			
			pLine = (CBinLine*) pPrevLine;

			if (pLine->m_pPage != pPage)
				i++;

			Cmp = pLine->CmpOffset (Offset);
		}
	}
	else
	{
		while (Cmp > 0 && i < m_CrossoverLimit)
		{
			CLine* pNextLine = m_pCacheMgr->GetNextLine (pLine);
			if (!pNextLine || !pLine->IsMerged (pNextLine))
				break;

			pLine = (CBinLine*) pNextLine;

			if (pLine->m_pPage != pPage)
				i++;

			Cmp = pLine->CmpOffset (Offset);
		}
	}

	return pLine;
}

void
CColorizeMgr::ApplyColorization (
	CCachePage* pPage,
	CBinLine* pFirstLine,
	CBinLine* pLastLine,
	const gui::CTextAttrAnchorArray* pAttrArray
	)
{
	size_t AnchorCount = pAttrArray->GetCount ();
	const gui::TTextAttrAnchor* pAnchor = *pAttrArray;
	const gui::TTextAttrAnchor* pEnd = pAnchor + AnchorCount;

	CBinLine* pLine = pFirstLine;
	CBinLine* pPrevLine = NULL;
	gui::TTextAttr PrevAttr;
	size_t PrevMetric = 0;
	size_t PrevOffset = 0;

	for (; pAnchor < pEnd; pAnchor++)
	{
		size_t Offset = pAnchor->m_Offset;

		pLine = FindLine (pPage, pLine, Offset);
		ASSERT (pLine);

		if (Offset < pLine->m_Offset)
			Offset = pLine->m_Offset;
		else if (Offset > pLine->m_Offset + pLine->m_BinData.GetCount ())
			Offset = pLine->m_Offset + pLine->m_BinData.GetCount ();

		ASSERT (pLine->m_LineKind == pFirstLine->m_LineKind);
		ASSERT (pLine->m_PartCode == pFirstLine->m_PartCode);

		Offset -= pLine->m_Offset;

		if (pPrevLine)
		{
			if (pLine->m_PageIndex == pPrevLine->m_PageIndex)
			{
				pPrevLine->Colorize (PrevAttr, PrevOffset, Offset, PrevMetric);
			}
			else
			{
				size_t FullLineCount = pLine->m_PageIndex - pPrevLine->m_PageIndex - 1;
				pPrevLine->Colorize (PrevAttr, PrevOffset, -1, PrevMetric);
				FillLineAttr (PrevAttr, PrevMetric, pPrevLine, FullLineCount);
				pLine->Colorize (PrevAttr, 0, Offset, PrevMetric);
			}		
		}

		pPrevLine = pLine;
		PrevAttr = pAnchor->m_Attr;
		PrevMetric = pAnchor->m_Metric;
		PrevOffset = Offset;
	}
}

CLine*
CColorizeMgr::ColorizeBlock (
	CCachePage* pPage,
	CBinLine* pFirstLine,
	size_t LineOffset
	)
{
	TColorizeBlock Block;

	CBinLine* pLastLine = CollectBlock (&Block, pPage, pFirstLine, LineOffset);
	CLine* pNextLine = m_pCacheMgr->GetNextLine (pLastLine);

	bool SaveState = pLastLine->m_pPage == pPage && pNextLine == NULL;
	size_t DataSize = m_DataBuffer.GetCount ();
	size_t PartCount = m_PartBuffer.GetCount ();

	CColorizeTarget ColorizeTarget;

	if (!Block.m_Size)
		return pNextLine;

	ColorizeTarget.m_ColorizerState = pPage->m_ColorizerState;
	ColorizeTarget.m_ColorizerStateEx = pPage->m_ColorizerStateEx;

	m_pColorizer->Colorize (&ColorizeTarget, &Block, SaveState);

	// apply attr array

	ApplyColorization (
		pPage,
		pFirstLine,
		pLastLine,
		&ColorizeTarget.m_AttrArray 
		);

	if (SaveState)
	{
		pPage->m_ColorizerState = ColorizeTarget.m_ColorizerState;
		pPage->m_ColorizerStateEx = ColorizeTarget.m_ColorizerStateEx;
		pPage->m_pColorizerLine = pLastLine;
		pPage->m_ColorizerOffset = pLastLine->m_BinData.GetCount ();
	}
	else
	{
		pPage->m_ColorizerState = 0;
		pPage->m_ColorizerStateEx = ref::EPtr_Null;	
		pPage->m_pColorizerLine = NULL;
		pPage->m_ColorizerOffset = 0;
	}

	return pNextLine;
}

CLine*
CColorizeMgr::Backtrack (CLine* pLine)
{
	size_t i = 0;	
	CLine* pPrevLine = m_pCacheMgr->GetPrevLine (pLine);

	while (
		i < m_CrossoverLimit &&
		pPrevLine && 
		pPrevLine->IsMerged (pLine)
		)
	{
		pLine = pPrevLine;
		pPrevLine = m_pCacheMgr->GetPrevLine (pLine);
		i++;
	}

	return pLine;
}

//.............................................................................

} // namespace log {
} // namespace axl {

