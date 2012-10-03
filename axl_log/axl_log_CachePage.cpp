#include "axl_Precomp.h"
#include "axl_log_CachePage.h"
#include "axl_log_ColorizeSink.h"
#include "axl_log_Colorizer.h"
#include "axl_log_Line.h"

//.............................................................................

void
AXL_API
axl_log_TCachePage_Construct(axl_log_TCachePage* pPage)
{
	axl_rtl_TPtrArray_Construct(&pPage->m_LineArray);
	axl_rtl_TPtrArray_Construct(&pPage->m_VolatileMessageArray);

	pPage->m_pCacheMapNode = NULL;

	pPage->m_MessageCount = 0;
	pPage->m_MessageOffset = 0;
	pPage->m_BinOffset = 0;
	pPage->m_PartIndex = 0;
	pPage->m_MergeId = 0;
	pPage->m_PreferredDataConfig = g_axl_log_TDataConfig_Default;
	axl_log_TMergeCriteria_Construct(&pPage->m_MergeCriteria);

	pPage->m_ColorizedLineCount = 0;
	pPage->m_ColorizerOffset = 0;
	pPage->m_pColorizerLine = NULL;
	pPage->m_ColorizerState = 0;
	pPage->m_pColorizerStateEx = NULL;

	pPage->m_IsRepresentNeeded = true;
	pPage->m_IsColorizeNeeded = true;
}

void
AXL_API
axl_log_TCachePage_Destruct(axl_log_TCachePage* pPage)
{
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
	axl_log_TLine** ppLine = (axl_log_TLine**) pPage->m_LineArray.m_p;
	axl_log_TLine** ppLineEnd = ppLine + LineCount;

	size_t MsgCount = axl_rtl_TPtrArray_GetCount(&pPage->m_VolatileMessageArray);
	axl_log_TCacheVolatileMessage** ppMsg = (axl_log_TCacheVolatileMessage**) pPage->m_VolatileMessageArray.m_p;
	axl_log_TCacheVolatileMessage** ppMsgEnd = ppMsg + MsgCount;

	for (; ppLine < ppLineEnd; ppLine++)
	{
		axl_log_TLine* pLine = *ppLine;
		axl_log_TLine_VDestruct(pLine);
		axl_mem_Free(pLine);
	}

	for (; ppMsg < ppMsgEnd; ppMsg++)
		axl_mem_Free(*ppMsg);

	axl_ref_TObject__ClearPtr(&pPage->m_pColorizerStateEx);
	axl_rtl_TPtrArray_Destruct(&pPage->m_VolatileMessageArray);
	axl_rtl_TPtrArray_Destruct(&pPage->m_LineArray);
}

axl_log_TBinLine*
AXL_API
axl_log_TCachePage_FindBinLineByOffset(
	axl_log_TCachePage* pPage,
	axl_log_TBinLine* pLine,
	size_t Offset
	)
{
	// we cannot simply do Offset / LineSize because:
	// - it won't work for bin text lines 
	// - we dont want to leave the current page

	for (;;) 
	{
		axl_log_TLine* pNextLine;

		size_t DataSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
		if (Offset >= pLine->m_Offset && Offset <= pLine->m_Offset + DataSize)
			return pLine;

		pNextLine = axl_log_TCachePage_GetLine(pPage, pLine->m_PageIndex + 1);
		if (!pNextLine || !axl_log_TLine_IsMerged((axl_log_TLine*) pLine, pNextLine))
			break;

		pLine = (axl_log_TBinLine*) pNextLine;
	} 

	return NULL;
}

size_t 
AXL_API
axl_log_TCachePage_GetVolatileMessageAnchorLine(
	axl_log_TCachePage* pPage,
	axl_log_TCacheVolatileMessage* pVolatileMsg
	)
{
	size_t LineCount;
	axl_log_TLine** ppLine;
	axl_log_TLine** ppEnd;

	if (pVolatileMsg->m_pFirstLine) // shortcut
		return pVolatileMsg->m_pFirstLine->m_PageIndex;

	LineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
	ppLine = (axl_log_TLine**) pPage->m_LineArray.m_p;
	ppEnd = ppLine + LineCount;

	for (; ppLine < ppEnd; ppLine++)
	{
		axl_log_TLine* pLine = *ppLine;
		if (pLine->m_FirstMessageOffset >= pVolatileMsg->m_Offset)
			return pLine->m_PageIndex;
	}

	return LineCount;
}

void
AXL_API
axl_log_TCachePage_ClearColorization(axl_log_TCachePage* pPage)
{
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
	axl_log_TLine** ppLine = (axl_log_TLine**) pPage->m_LineArray.m_p;
	axl_log_TLine** ppEnd = ppLine + LineCount;

	for (; ppLine < ppEnd; ppLine++)
	{
		axl_log_TLine* pLine = *ppLine;
		if (axl_log_TLine_IsBin(pLine))
		{
			axl_log_TBinLine* pBinLine = (axl_log_TBinLine*) pLine;
			axl_gr_TTextAttrAnchorArray_Copy(&pBinLine->m_AttrArray, &pBinLine->m_RepresentorAttrArray);
		}
	}

	pPage->m_ColorizerState = 0;
	axl_ref_TObject__ClearPtr(&pPage->m_pColorizerStateEx);
	pPage->m_pColorizerLine = NULL;
	pPage->m_ColorizedLineCount = 0;
	pPage->m_ColorizerOffset = 0;
	pPage->m_IsColorizeNeeded = true;
}

axl_log_TLine*
AXL_API
axl_log_TCachePage_GetLine(
	axl_log_TCachePage* pPage,
	size_t Line
	)
{
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
	return Line < LineCount ? ((axl_log_TLine**) pPage->m_LineArray.m_p)[Line] : NULL;
}

void
AXL_API
axl_log_TCachePage_FreeLines(
	axl_log_TCachePage* pPage,
	size_t Line,
	size_t LineCount
	)
{	
	axl_log_TLine** ppLine;
	axl_log_TLine** ppEnd;

	size_t PageLineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
	
	if (Line >= PageLineCount)
		return;

	if (Line + LineCount > PageLineCount)
		LineCount = PageLineCount - Line;

	ppLine = (axl_log_TLine**) pPage->m_LineArray.m_p + Line;
	ppEnd = ppLine + LineCount;	

	while (ppLine < ppEnd)
	{
		axl_log_TLine* pLine = *ppLine;

		axl_log_TLine_VDestruct(pLine);
		axl_mem_Free(pLine);

		*ppLine = NULL;
		ppLine++;
	}	
}

void
AXL_API
axl_log_TCachePage_AddLines(
	axl_log_TCachePage* pPage,
	size_t Line,
	long LineDelta
	)
{
	size_t PageLineCount;
	
	if (!LineDelta)
		return;
	
	if (LineDelta < 0)
		axl_rtl_TPtrArray_Remove(&pPage->m_LineArray, Line, -LineDelta);
	else
		axl_rtl_TPtrArray_Insert(&pPage->m_LineArray, Line, NULL, LineDelta);

	// fixup following line indices

	PageLineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
	for (; Line < PageLineCount; Line++)
	{
		axl_log_TLine* pLine = ((axl_log_TLine**) pPage->m_LineArray.m_p)[Line];
		if (pLine)
			pLine->m_PageIndex = Line;
	}
}

//.............................................................................

