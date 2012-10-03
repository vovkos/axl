#include "axl_Precomp.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_log_ColorizeSink.h"
#include "axl_log_CacheMgr.h"
#include "axl_log_LogCtrl.h"
#include "axl_base_Trace.h"

//.............................................................................

void
AXL_API
axl_log_TColorizeMgr_Construct(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TLogCtrl* pLogCtrl
	)
{
	pColorizeMgr->m_pLogCtrl = pLogCtrl;
	pColorizeMgr->m_CrossoverLimit = axl_log_EDef_ColorizeCrossoverLimit;
	pColorizeMgr->m_IsInvalidateRequired = true;
	axl_rtl_TByteArray_Construct(&pColorizeMgr->m_DataBuffer);
	axl_rtl_TArray_Construct(&pColorizeMgr->m_PartBuffer);
}

void
AXL_API
axl_log_TColorizeMgr_Destruct(axl_log_TColorizeMgr* pColorizeMgr)
{
	axl_rtl_TByteArray_Destruct(&pColorizeMgr->m_DataBuffer);
	axl_rtl_TArray_Destruct(&pColorizeMgr->m_PartBuffer);
}

AXL_INLINE
void
axl_log_TColorizeMgr_CollectLine(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TColorizeBlock* pBlock,
	axl_log_TBinLine* pLine
	)
{
	size_t PartIndex = pBlock->m_PartIndex + axl_rtl_TArray_GetCount(&pColorizeMgr->m_PartBuffer); 

	if (pLine->m_PartIndex != PartIndex)
		axl_log_TBinLinePartArray_AddPart(&pColorizeMgr->m_PartBuffer, pLine->m_Offset, pLine->m_FirstTimestamp);

	axl_rtl_TByteArray_AppendArray(&pColorizeMgr->m_DataBuffer, &pLine->m_BinData);
	axl_log_TBinLinePartArray_AppendArray(&pColorizeMgr->m_PartBuffer, &pLine->m_PartArray);
}

void
AXL_API
axl_log_TColorizeMgr_CollectFirstLine(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TColorizeBlock* pBlock,
	axl_log_TBinLine* pLine,
	size_t LineOffset
	)
{
	size_t DataSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
	size_t PartCount = axl_rtl_TArray_GetCount(&pLine->m_PartArray);
	size_t PartIndex;

	axl_log_TBinLinePart* pPart;

	pBlock->m_PartCode = pLine->m_PartCode;
	pBlock->m_Offset = pLine->m_Offset + LineOffset;
	pBlock->m_PartIndex = pLine->m_PartIndex; 
	pBlock->m_Timestamp = pLine->m_FirstTimestamp;

	if (!LineOffset)
	{
		axl_log_TColorizeMgr_CollectLine(pColorizeMgr, pBlock, pLine);
		return;
	}

	if (LineOffset >= DataSize) 
	{
		pBlock->m_PartIndex = pLine->m_PartIndex + axl_rtl_TArray_GetCount(&pLine->m_PartArray); 
		pBlock->m_Timestamp = pLine->m_LastTimestamp;
		return;
	}

	axl_rtl_TByteArray_Append(
		&pColorizeMgr->m_DataBuffer, 
		(uchar_t*) pLine->m_BinData.m_p + LineOffset, 
		DataSize - LineOffset
		);

	pPart = axl_log_TBinLine_FindPart(pLine, pLine->m_Offset + LineOffset);
	
	if (!pPart)
	{
		// should never be here : base offset always should be in between parts
		// however, otherwise situation doesnt't logically contradict anything, so
		ASSERT(false); 
		axl_log_TBinLinePartArray_AppendArray(&pColorizeMgr->m_PartBuffer, &pLine->m_PartArray);
		return;
	}

	PartIndex = pPart - (axl_log_TBinLinePart*) pLine->m_PartArray.m_p;
	ASSERT(PartIndex < PartCount);

	pBlock->m_PartIndex = pLine->m_PartIndex + PartIndex + 1; 	
	pBlock->m_Timestamp = pPart->m_Timestamp;

	pPart++;
	PartIndex++;

	axl_log_TBinLinePartArray_Append(&pColorizeMgr->m_PartBuffer, pPart, PartCount - PartIndex);
}

axl_log_TBinLine*
AXL_API
axl_log_TColorizeMgr_CollectBlock(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TColorizeBlock* pBlock,
	axl_log_TCachePage* pPage,
	axl_log_TBinLine* pLine,
	size_t LineOffset
	)
{
	axl_log_TCacheMgr* pCacheMgr = &pColorizeMgr->m_pLogCtrl->m_CacheMgr;
	axl_log_TLine* pNextLine = axl_log_TCacheMgr_GetNextLine(pCacheMgr, (axl_log_TLine*) pLine);

	axl_rtl_TByteArray_Clear(&pColorizeMgr->m_DataBuffer);
	axl_rtl_TByteArray_Clear(&pColorizeMgr->m_PartBuffer);

	pBlock->m_PartCode = pLine->m_PartCode;
	pBlock->m_Offset = pLine->m_Offset + LineOffset;
	pBlock->m_PartIndex = pLine->m_PartIndex; 
	pBlock->m_Timestamp = pLine->m_FirstTimestamp;

	axl_log_TColorizeMgr_CollectFirstLine(pColorizeMgr, pBlock, pLine, LineOffset);

	if (pLine->m_pPage != pPage) // backtracked line
	{
	    // collect crossover data till we get back to our page

		while (pNextLine && pNextLine->m_pPage != pPage)
		{
			ASSERT(axl_log_TLine_IsMerged((axl_log_TLine*) pLine, pNextLine));

			pLine = (axl_log_TBinLine*) pNextLine;		
			axl_log_TColorizeMgr_CollectLine(pColorizeMgr, pBlock, pLine);

			pNextLine = axl_log_TCacheMgr_GetNextLine(pCacheMgr, (axl_log_TLine*) pLine);
		}

		ASSERT(axl_log_TLine_IsMerged((axl_log_TLine*) pLine, pNextLine));
		ASSERT(pNextLine->m_pPage == pPage);
		ASSERT(pNextLine == axl_log_TCachePage_GetLine(pPage, 0));

		pLine = (axl_log_TBinLine*) pNextLine;
	}
 
	// data on the page

	while (
		pNextLine && pNextLine->m_pPage == pPage &&
		axl_log_TLine_IsMerged((axl_log_TLine*) pLine, pNextLine)
		)
	{
		pLine = (axl_log_TBinLine*) pNextLine;		
		axl_log_TColorizeMgr_CollectLine(pColorizeMgr, pBlock, pLine);

		pNextLine = axl_log_TCacheMgr_GetNextLine(pCacheMgr, (axl_log_TLine*) pLine);
	}

	if (pNextLine && pNextLine->m_pPage != pPage)
	{
	    // crossover data on the next page 

		size_t i = 0;

		while (
			i < pColorizeMgr->m_CrossoverLimit &&
			pNextLine && 
			axl_log_TLine_IsMerged((axl_log_TLine*) pLine, pNextLine)
			)
		{
			pLine = (axl_log_TBinLine*) pNextLine;
			axl_log_TColorizeMgr_CollectLine(pColorizeMgr, pBlock, pLine);

			pNextLine = axl_log_TCacheMgr_GetNextLine(pCacheMgr, (axl_log_TLine*) pLine);
			i++;
		}
	}

	pBlock->m_p = pColorizeMgr->m_DataBuffer.m_p;
	pBlock->m_Size = axl_rtl_TByteArray_GetCount(&pColorizeMgr->m_DataBuffer);
	pBlock->m_pPartArray = pColorizeMgr->m_PartBuffer.m_p;
	pBlock->m_PartCount = axl_rtl_TArray_GetCount(&pColorizeMgr->m_PartBuffer);

	return pLine;
}

// fills LineCount line AFTER pAnchorLine

void
AXL_API
axl_log_TColorizeMgr_FillLineAttr(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_gr_TTextAttr* pAttr,
	ulong_t Metric,
	axl_log_TBinLine* pAnchorLine,
	size_t LineCount
	)
{
	axl_log_TCacheMgr* pCacheMgr = &pColorizeMgr->m_pLogCtrl->m_CacheMgr;

	axl_log_TBinLine* pLine = (axl_log_TBinLine*) axl_log_TCacheMgr_GetNextLine(pCacheMgr, (axl_log_TLine*) pAnchorLine);

	size_t i;
	
	for (i = 0; i < LineCount && pLine && pLine->m_pPage == pAnchorLine->m_pPage; i++)
	{
		ASSERT(pLine->m_Type == pAnchorLine->m_Type);
		ASSERT(pLine->m_PartCode == pAnchorLine->m_PartCode);

		axl_log_TBinLine_Colorize(pLine, pAttr, 0, -1, Metric);
		pLine = (axl_log_TBinLine*) axl_log_TCacheMgr_GetNextLine(pCacheMgr, (axl_log_TLine*) pLine);
	}
}

/*

void TraceAttrAnchorArray(axl_gr_TTextAttrAnchorArray* pArray)
{
	axl_gr_TTextAttrAnchor* pAnchor = pArray->m_p;
	axl_gr_TTextAttrAnchor* pEnd = pAnchor + axl_rtl_TArray_GetCount(pArray);

	axl_dbg_Trace("TraceAttrAnchorArray(%08x)...\n", pArray);

	for (; pAnchor < pEnd; pAnchor++)
	{
		axl_dbg_Trace("    %04x -- back(%08x)\n", pAnchor->m_Offset, pAnchor->m_Attr.m_BackColor);
	}
}

*/

axl_log_TBinLine*
AXL_API
axl_log_TColorizeMgr_FindLine(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TCachePage* pPage,
	axl_log_TBinLine* pLine,
	size_t Offset
	)
{		
	axl_log_TCacheMgr* pCacheMgr = &pColorizeMgr->m_pLogCtrl->m_CacheMgr;

	size_t i = 0;

	int Cmp = axl_log_TBinLine_CmpOffset(pLine, Offset);

	if (Cmp < 0)
	{
		pColorizeMgr->m_IsInvalidateRequired = true;

		while (Cmp < 0 && i < pColorizeMgr->m_CrossoverLimit)
		{
			axl_log_TLine* pPrevLine = axl_log_TCacheMgr_GetPrevLine(pCacheMgr, (axl_log_TLine*) pLine);
			if (!pPrevLine || !axl_log_TLine_IsMerged(pPrevLine, (axl_log_TLine*) pLine))
				break;
			
			pLine = (axl_log_TBinLine*) pPrevLine;

			if (pLine->m_pPage != pPage)
				i++;

			Cmp = axl_log_TBinLine_CmpOffset(pLine, Offset);
		}
	}
	else
	{
		while (Cmp > 0 && i < pColorizeMgr->m_CrossoverLimit)
		{
			axl_log_TLine* pNextLine = axl_log_TCacheMgr_GetNextLine(pCacheMgr, (axl_log_TLine*) pLine);
			if (!pNextLine || !axl_log_TLine_IsMerged((axl_log_TLine*) pLine, pNextLine))
				break;

			pLine = (axl_log_TBinLine*) pNextLine;

			if (pLine->m_pPage != pPage)
				i++;

			Cmp = axl_log_TBinLine_CmpOffset(pLine, Offset);
		}
	}

	return pLine;
}

void
AXL_API
axl_log_TColorizeMgr_ApplyColorization(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TCachePage* pPage,
	axl_log_TBinLine* pFirstLine,
	axl_log_TBinLine* pLastLine,
	axl_gr_TTextAttrAnchorArray* pAttrArray
	)
{
	size_t AnchorCount = axl_rtl_TArray_GetCount(pAttrArray);
	axl_gr_TTextAttrAnchor* pAnchor = (axl_gr_TTextAttrAnchor*) pAttrArray->m_p;
	axl_gr_TTextAttrAnchor* pEnd = pAnchor + AnchorCount;

	axl_log_TBinLine* pLine = pFirstLine;
	axl_log_TBinLine* pPrevLine = NULL;
	axl_gr_TTextAttr* pPrevAttr = NULL;
	ulong_t PrevMetric = 0;
	size_t PrevOffset = 0;

	for (; pAnchor < pEnd; pAnchor++)
	{
		size_t Offset = pAnchor->m_Offset;

		pLine = axl_log_TColorizeMgr_FindLine(pColorizeMgr, pPage, pLine, Offset);
		ASSERT(pLine);

		if (Offset < pLine->m_Offset)
			Offset = pLine->m_Offset;
		else if (Offset > pLine->m_Offset + axl_rtl_TByteArray_GetCount(&pLine->m_BinData))
			Offset = pLine->m_Offset + axl_rtl_TByteArray_GetCount(&pLine->m_BinData);

		ASSERT(pLine->m_Type == pFirstLine->m_Type);
		ASSERT(pLine->m_PartCode == pFirstLine->m_PartCode);

		Offset -= pLine->m_Offset;

		if (pPrevLine)
		{
			if (pLine->m_PageIndex == pPrevLine->m_PageIndex)
			{
				axl_log_TBinLine_Colorize(pPrevLine, pPrevAttr, PrevOffset, Offset, PrevMetric);
			}
			else
			{
				size_t FullLineCount = pLine->m_PageIndex - pPrevLine->m_PageIndex - 1;
				axl_log_TBinLine_Colorize(pPrevLine, pPrevAttr, PrevOffset, -1, PrevMetric);
				axl_log_TColorizeMgr_FillLineAttr(pColorizeMgr, pPrevAttr, PrevMetric, pPrevLine, FullLineCount);
				axl_log_TBinLine_Colorize(pLine, pPrevAttr, 0, Offset, PrevMetric);
			}		
		}

		pPrevLine = pLine;
		pPrevAttr = &pAnchor->m_Attr;
		PrevMetric = pAnchor->m_Metric;
		PrevOffset = Offset;
	}
}

axl_log_TLine*
AXL_API
axl_log_TColorizeMgr_ColorizeBlock(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TCachePage* pPage,
	axl_log_TColorizer* pColorizer,
	axl_log_TBinLine* pFirstLine,
	size_t LineOffset
	)
{
	axl_log_TCacheMgr* pCacheMgr = &pColorizeMgr->m_pLogCtrl->m_CacheMgr;

	axl_log_TColorizeBlock Block;

	axl_log_TBinLine* pLastLine = axl_log_TColorizeMgr_CollectBlock(pColorizeMgr, &Block, pPage, pFirstLine, LineOffset);
	axl_log_TLine* pNextLine = axl_log_TCacheMgr_GetNextLine(pCacheMgr, (axl_log_TLine*) pLastLine);

	bool_t SaveState = pLastLine->m_pPage == pPage && pNextLine == NULL;
	size_t DataSize = axl_rtl_TByteArray_GetCount(&pColorizeMgr->m_DataBuffer);
	size_t PartCount = axl_rtl_TByteArray_GetCount(&pColorizeMgr->m_PartBuffer);

	axl_log_TColorizeSink Sink;

	if (!Block.m_Size)
		return pNextLine;

	axl_log_TColorizeSink_Construct(&Sink);
	axl_ref_TObject__CopyPtr(&Sink.m_pColorizerStateEx, pPage->m_pColorizerStateEx);
	Sink.m_ColorizerState = pPage->m_ColorizerState;

	axl_log_TColorizer_Colorize(pColorizer, &Sink, &Block, SaveState);

	// apply attr array

	axl_log_TColorizeMgr_ApplyColorization(
		pColorizeMgr,
		pPage,
		pFirstLine,
		pLastLine,
		&Sink.m_AttrArray 
		);

	if (SaveState)
	{
		axl_ref_TObject__CopyPtr(&pPage->m_pColorizerStateEx, Sink.m_pColorizerStateEx);
		pPage->m_ColorizerState = Sink.m_ColorizerState;
		pPage->m_pColorizerLine = pLastLine;
		pPage->m_ColorizerOffset = axl_rtl_TByteArray_GetCount(&pLastLine->m_BinData);
	}
	else
	{
		axl_ref_TObject__ClearPtr(&pPage->m_pColorizerStateEx);
		pPage->m_ColorizerState = 0;
		pPage->m_pColorizerLine = NULL;
		pPage->m_ColorizerOffset = 0;
	}

	axl_log_TColorizeSink_Destruct(&Sink);

	return pNextLine;
}

axl_log_TLine*
AXL_API
axl_log_TColorizeMgr_Backtrack(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TLine* pLine
	)
{
	size_t i = 0;	
	axl_log_TCacheMgr* pCacheMgr = &pColorizeMgr->m_pLogCtrl->m_CacheMgr;
	axl_log_TLine* pPrevLine = axl_log_TCacheMgr_GetPrevLine(pCacheMgr, pLine);

	while (
		i < pColorizeMgr->m_CrossoverLimit &&
		pPrevLine && 
		axl_log_TLine_IsMerged(pPrevLine, pLine)
		)
	{
		pLine = pPrevLine;
		pPrevLine = axl_log_TCacheMgr_GetPrevLine(pCacheMgr, pLine);
		i++;
	}

	return pLine;
}

bool_t
AXL_API
axl_log_TColorizeMgr_ColorizePage(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TCachePage* pPage
	)
{
	axl_log_TLine* pLine;
	axl_log_TColorizer* pColorizer = pColorizeMgr->m_pLogCtrl->m_pColorizer;
	axl_log_TCacheMgr* pCacheMgr = &pColorizeMgr->m_pLogCtrl->m_CacheMgr;

	if (!pPage->m_IsColorizeNeeded || !pColorizer)
		return true;

	pColorizeMgr->m_IsInvalidateRequired = false;

	if (pPage->m_pColorizerLine) // incremental colorize
		pLine = axl_log_TColorizeMgr_ColorizeBlock(pColorizeMgr, pPage, pColorizer, pPage->m_pColorizerLine, pPage->m_ColorizerOffset);
	else 
		pLine = axl_log_TCachePage_GetLine(pPage, pPage->m_ColorizedLineCount);

	if (!pPage->m_ColorizedLineCount && axl_log_TLine_IsBin(pLine))
	{
		pLine = axl_log_TColorizeMgr_Backtrack(pColorizeMgr, pLine);
		ASSERT(axl_log_TLine_IsBin(pLine));
		pLine = axl_log_TColorizeMgr_ColorizeBlock(pColorizeMgr, pPage, pColorizer, (axl_log_TBinLine*) pLine, 0);
	}

	while (pLine && pLine->m_pPage == pPage)
	{
		if (!axl_log_TLine_IsBin(pLine))
			pLine = axl_log_TCacheMgr_GetNextLine(pCacheMgr, pLine);
		else
			pLine = axl_log_TColorizeMgr_ColorizeBlock(
				pColorizeMgr, 
				pPage, 
				pColorizeMgr->m_pLogCtrl->m_pColorizer, 
				(axl_log_TBinLine*) pLine, 
				0
				);
	}

	pPage->m_ColorizedLineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
	pPage->m_IsColorizeNeeded = false;

	if (pColorizeMgr->m_IsInvalidateRequired)
		axl_log_TLogCtrl_Invalidate( pColorizeMgr->m_pLogCtrl);

	return true;
}

bool_t
AXL_API
axl_log_TColorizeMgr_ReColorizeLineRange(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TCachePage* pPage,
	size_t FirstLine,
	size_t LineCount
	)
{
	axl_log_TLine* pLine  = axl_log_TCachePage_GetLine(pPage, FirstLine);
	axl_log_TColorizer* pColorizer = pColorizeMgr->m_pLogCtrl->m_pColorizer;
	axl_log_TCacheMgr* pCacheMgr = &pColorizeMgr->m_pLogCtrl->m_CacheMgr;

	size_t LastLine = FirstLine + LineCount;

	if (!pColorizer)
		return true;

	while (pLine && pLine->m_pPage == pPage && pLine->m_PageIndex < LastLine)
	{
		if (!axl_log_TLine_IsBin(pLine))
			pLine = axl_log_TCacheMgr_GetNextLine(pCacheMgr, pLine);
		else
			pLine = axl_log_TColorizeMgr_ColorizeBlock(
				pColorizeMgr, 
				pPage, 
				pColorizeMgr->m_pLogCtrl->m_pColorizer, 
				(axl_log_TBinLine*) pLine, 
				0
				);
	}

	return true;
}

bool_t
AXL_API
axl_log_TColorizeMgr_EnsureColorized(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TBinLine* pLine
	)
{
	return axl_log_TColorizeMgr_ColorizePage(pColorizeMgr, pLine->m_pPage);
}

//.............................................................................
