#include "axl_Precomp.h"
#include "axl_log_CacheSink.h"
#include "axl_log_CachePage.h"
#include "axl_base_Trace.h"

static tchar_t* _tcschrx(const tchar_t* p, tchar_t x, const tchar_t* pEnd)
{
	while (p < pEnd)
	{
		if (*p == x)
			return (tchar_t*) p;
		
		p++;
	}

	return NULL;
}

//.............................................................................

void 
AXL_API
axl_log_TCacheSink_Construct(axl_log_TCacheSink* pSink)
{
	static axl_log_TRepresentSinkVTable _VTable = 
	{
		axl_log_TCacheSink_AddData
	};

	axl_log_TRepresentSink_Construct((axl_log_TRepresentSink*) pSink);
	axl_log_TMergeCriteria_Construct(&pSink->m_MergeCriteria);
	
	pSink->m_pVTable = &_VTable;
	pSink->m_pPage = NULL;
	pSink->m_pVolatileMessage = NULL;
	pSink->m_pPrevPageMergeLine = NULL;
	pSink->m_MessageOffset = 0;
	pSink->m_Timestamp = 0;
	pSink->m_BinOffset = 0;
	pSink->m_PartIndex = 0;
	pSink->m_MergeId = 0;
	pSink->m_Line = 0;
	pSink->m_EndLine = 0;
	pSink->m_LongestTextLine = 0;
	pSink->m_LongestBinHexLine = 0;
	pSink->m_LongestBinTextLine = 0;
	pSink->m_IsFull = false;
}

void
AXL_API
axl_log_TCacheSink_Destruct(axl_log_TCacheSink* pSink)
{
	if (pSink->m_pPrevPageMergeLine)
	{
		axl_log_TLine_VDestruct(pSink->m_pPrevPageMergeLine);
		axl_mem_Free(pSink->m_pPrevPageMergeLine);
	}

	axl_log_TRepresentSink_Destruct((axl_log_TRepresentSink*) pSink);
}

void
AXL_API
axl_log_TCacheSink_UpdateLineAttr(
	axl_log_TCacheSink* pSink,
	axl_log_TLine* pLine
	)
{
	pLine->m_Icon = pSink->m_IsFirstLineOfMessage || pSink->m_TileIcon ? pSink->m_Icon : -1;
	pLine->m_Attr = pSink->m_Attr;
}

void
AXL_API
axl_log_TCacheSink_AddLine(
	axl_log_TCacheSink* pSink,
	axl_log_TLine* pLine
	)
{
	axl_log_TLine** ppLine = (axl_log_TLine**) pSink->m_pPage->m_LineArray.m_p + pSink->m_Line;

	ASSERT(axl_log_TCacheSink_CanAddLines(pSink));
	ASSERT(pSink->m_Line < axl_rtl_TPtrArray_GetCount(&pSink->m_pPage->m_LineArray));
	ASSERT(!*ppLine);

	*ppLine = pLine;

	pLine->m_pPage = pSink->m_pPage;
	pLine->m_IsFirstLineOfMessage = pSink->m_IsFirstLineOfMessage;
	pLine->m_pVolatileMessage = pSink->m_pVolatileMessage;
	pLine->m_PageIndex = pSink->m_Line;
	pLine->m_PartCode = pSink->m_PartCode;
	pLine->m_MergeId = pSink->m_MergeId;
	pLine->m_PartIndex = pSink->m_PartIndex;
	pLine->m_FirstMessageOffset = pSink->m_MessageOffset;
	pLine->m_FirstTimestamp = pSink->m_Timestamp;
	pLine->m_LastTimestamp = pSink->m_Timestamp;

	axl_log_TCacheSink_UpdateLineAttr(pSink, pLine);

	pSink->m_IsFirstLineOfMessage = false;
	pSink->m_Line++;

	// link line and volatile message

	if (pLine->m_pVolatileMessage)
	{
		if (!pLine->m_pVolatileMessage->m_pFirstLine)
			pLine->m_pVolatileMessage->m_pFirstLine = pLine;

		pLine->m_pVolatileMessage->m_LineCount++;
	}
}

axl_log_TTextLine* 
AXL_API
axl_log_TCacheSink_AddTextLine(axl_log_TCacheSink* pSink)
{
	axl_log_TTextLine* pLine;

	if (!axl_log_TCacheSink_CanAddLines(pSink))
	{
		pSink->m_IsFull = true;
		return NULL;
	}

	pLine = (axl_log_TTextLine*) axl_mem_Allocate(sizeof(axl_log_TTextLine));
	axl_log_TTextLine_Construct(pLine);	
	axl_log_TCacheSink_AddLine(pSink, (axl_log_TLine*) pLine);
	return pLine;
}

axl_log_TBinTextLine* 
AXL_API
axl_log_TCacheSink_AddBinTextLine(axl_log_TCacheSink* pSink)
{
	axl_log_TBinTextLine* pLine;

	if (!axl_log_TCacheSink_CanAddLines(pSink))
	{
		pSink->m_IsFull = true;
		return NULL;
	}

	pLine = (axl_log_TBinTextLine*) axl_mem_Allocate(sizeof(axl_log_TBinTextLine));
	axl_log_TBinTextLine_Construct(pLine);
	axl_log_TCacheSink_AddLine(pSink, (axl_log_TLine*) pLine);
	pLine->m_Offset = pSink->m_BinOffset;
	pLine->m_DataConfig = pSink->m_DataConfig;
	return pLine;
}

axl_log_TBinLine* 
AXL_API
axl_log_TCacheSink_AddBinHexLine(axl_log_TCacheSink* pSink)
{
	axl_log_TBinLine* pLine;

	if (!axl_log_TCacheSink_CanAddLines(pSink))
	{
		pSink->m_IsFull = true;
		return NULL;
	}

	pLine = (axl_log_TBinLine*) axl_mem_Allocate(sizeof(axl_log_TBinLine));
	axl_log_TBinLine_Construct(pLine);
	axl_log_TCacheSink_AddLine(pSink, (axl_log_TLine*) pLine);
	pLine->m_Offset = pSink->m_BinOffset;
	pLine->m_DataConfig = pSink->m_DataConfig;
	return pLine;
}

axl_log_TLine* 
AXL_API
axl_log_TCacheSink_CreatePrevPageMergeLine(
	axl_log_TCacheSink* pSink,
	axl_log_TMergeCriteria* pMergeCriteria,
	ulong_t MergeTrailCol
	)
{
	if (!pMergeCriteria->m_Merge) // it would not get merged anyway
		return NULL;

	ASSERT(!pSink->m_pPrevPageMergeLine);

	switch (pMergeCriteria->m_LineType)
	{
	case axl_log_ELine_Text:
		{
		axl_log_TTextLine* pLine = axl_mem_Allocate(sizeof(axl_log_TTextLine));
		axl_log_TTextLine_Construct(pLine);
		axl_rtl_TString_CopyChar(&pLine->m_HyperText.m_Text, '.', MergeTrailCol);
		pSink->m_pPrevPageMergeLine = (axl_log_TLine*) pLine;
		break;
		}

	case axl_log_ELine_BinText:
		{
		axl_log_TBinTextLine* pLine = axl_mem_Allocate(sizeof(axl_log_TBinTextLine));
		axl_log_TBinTextLine_Construct(pLine);
		axl_rtl_TString_CopyChar(&pLine->m_BinText, '.', MergeTrailCol);
		pSink->m_pPrevPageMergeLine = (axl_log_TLine*) pLine;
		break;
		}

	case axl_log_ELine_BinHex:
		{
		axl_log_TBinLine* pLine = axl_mem_Allocate(sizeof(axl_log_TBinLine));
		axl_log_TBinLine_Construct(pLine);
		axl_rtl_TByteArray_SetCount(&pLine->m_BinData, MergeTrailCol);
		pSink->m_pPrevPageMergeLine = (axl_log_TLine*) pLine;
		break;
		}

	default:
		ASSERT(FALSE);
	}

	pSink->m_pPrevPageMergeLine->m_pPage = pSink->m_pPage;
	pSink->m_pPrevPageMergeLine->m_PageIndex = -1;
	return pSink->m_pPrevPageMergeLine;
}

void 
AXL_API
axl_log_TCacheSink_AddText(
	axl_log_TCacheSink* pSink, 
	const tchar_t* pText,
	size_t Length,
	bool_t ShouldMerge,
	bool_t IsHyperText
	)
{
	axl_log_TTextLine* pLine;

	const tchar_t* p = pText;
	const tchar_t* pEnd = pText + Length;
	const tchar_t* pCr;

	if (ShouldMerge)
	{
		ulong_t LineLength;

		pLine = (axl_log_TTextLine*) axl_log_TCacheSink_GetCurrentLine(pSink);
		ASSERT(pLine && pLine->m_Type == axl_log_ELine_Text);

		axl_log_TCacheSink_UpdateLineAttr(pSink, (axl_log_TLine*) pLine);

		pCr = _tcschrx(p, _T('\n'), pEnd);

		if (IsHyperText)
			LineLength = (ulong_t) axl_gr_THyperText_AddHyperText(&pLine->m_HyperText, p, pCr ? pCr - p : pEnd - p, &pSink->m_Attr);
		else
			LineLength = (ulong_t) axl_rtl_TString_AppendString(&pLine->m_HyperText.m_Text, p, pCr ? pCr - p : pEnd - p);

		if (LineLength > pSink->m_LongestTextLine)
			pSink->m_LongestTextLine = LineLength;

		if (!pCr)
			return;

		p = pCr + 1;
	}

	for (;;)
	{
		ulong_t LineLength;

		pLine = axl_log_TCacheSink_AddTextLine(pSink);
		if (!pLine)
			return;

		pCr = _tcschrx(p, _T('\n'), pEnd);

		if (IsHyperText)
			LineLength = (ulong_t) axl_gr_THyperText_AddHyperText(&pLine->m_HyperText, p, pCr ? pCr - p : pEnd - p, &pSink->m_Attr);
		else
			LineLength = (ulong_t) axl_rtl_TString_AppendString(&pLine->m_HyperText.m_Text, p, pCr ? pCr - p : pEnd - p);

		if (LineLength > pSink->m_LongestTextLine)
			pSink->m_LongestTextLine = LineLength;

		if (!pCr)
			return;

		p = pCr + 1;
	}
}

void 
AXL_API
axl_log_TCacheSink_AddBinText(
	axl_log_TCacheSink* pSink,
	const void* _p,
	size_t Size,
	bool_t ShouldMerge
	)
{
	axl_log_TBinTextLine* pLine;

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;

	size_t ChunkSize;
	ulong_t Length;

	if (ShouldMerge)
	{
		pLine = (axl_log_TBinTextLine*) axl_log_TCacheSink_GetCurrentLine(pSink);
		ASSERT(pLine && pLine->m_Type == axl_log_ELine_BinText);

		axl_log_TCacheSink_UpdateLineAttr(pSink, (axl_log_TLine*) pLine);

		ChunkSize = axl_log_TBinTextLine_AddData(pLine, p, Size, pSink->m_Timestamp, &pSink->m_Attr);
		p += ChunkSize;
		Size -= ChunkSize;
		pSink->m_BinOffset += ChunkSize;

		Length = (ulong_t) axl_rtl_TString_GetLength(&pLine->m_BinText);
		if (Length > pSink->m_LongestTextLine)
			pSink->m_LongestTextLine = Length;
	}

	while (p < pEnd)
	{
		pLine = axl_log_TCacheSink_AddBinTextLine(pSink);
		if (!pLine)
			break;

		ChunkSize = axl_log_TBinTextLine_AddData(pLine, p, Size, pSink->m_Timestamp, &pSink->m_Attr);
		p += ChunkSize;
		Size -= ChunkSize;
		pSink->m_BinOffset += ChunkSize;

		Length = (ulong_t) axl_rtl_TString_GetLength(&pLine->m_BinText);
		if (Length > pSink->m_LongestTextLine)
			pSink->m_LongestTextLine = Length;
	}

	// special case: data block is ended on LF, have to add extra line

	if (*(p - 1) == '\n')
		axl_log_TCacheSink_AddBinTextLine(pSink);

	if (pSink->m_DataConfig.m_BinTextLineSize > pSink->m_LongestBinTextLine)
		pSink->m_LongestBinTextLine = pSink->m_DataConfig.m_BinTextLineSize;
}

void 
AXL_API
axl_log_TCacheSink_AddBinHex(
	axl_log_TCacheSink* pSink, 
	const void* _p,
	size_t Size,
	bool_t ShouldMerge
	)
{
	axl_log_TBinLine* pLine;

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;
	uchar_t* ppp = (uchar_t*) _p;

	size_t ChunkSize;

	if (ShouldMerge)
	{
		size_t LastLineSize;

		pLine = (axl_log_TBinLine*) axl_log_TCacheSink_GetCurrentLine(pSink);
		ASSERT(pLine && pLine->m_Type == axl_log_ELine_BinHex);

		axl_log_TCacheSink_UpdateLineAttr(pSink, (axl_log_TLine*) pLine);

		LastLineSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
		if (LastLineSize < pSink->m_DataConfig.m_BinHexLineSize)
		{
			ChunkSize = pSink->m_DataConfig.m_BinHexLineSize - LastLineSize;
			ChunkSize = min(ChunkSize, Size);
			axl_log_TBinLine_AddData(pLine, p, ChunkSize, pSink->m_Timestamp, &pSink->m_Attr);
			p += ChunkSize;
			Size -= ChunkSize;
			pSink->m_BinOffset += ChunkSize;
		}
	}	

	while (p < pEnd)
	{
		pLine = axl_log_TCacheSink_AddBinHexLine(pSink);
		if (!pLine)
			break;

		ChunkSize = min(pSink->m_DataConfig.m_BinHexLineSize, Size);
		axl_log_TBinLine_AddData(pLine, p, ChunkSize, pSink->m_Timestamp, &pSink->m_Attr);
		p += ChunkSize;
		Size -= ChunkSize;
		pSink->m_BinOffset += ChunkSize;
	}

	if (pSink->m_DataConfig.m_BinHexLineSize > pSink->m_LongestBinHexLine)
		pSink->m_LongestBinHexLine = pSink->m_DataConfig.m_BinHexLineSize;
}

void 
AXL_API
axl_log_TCacheSink_AddDelimiter(
	axl_log_TCacheSink* pSink, 
	int Delimiter
	)
{
	axl_log_TLine* pLine = axl_log_TCacheSink_GetCurrentLine(pSink);
	if (pLine)
		pLine->m_LowerDelimiter = Delimiter;
}

void 
AXL_VFT
axl_log_TCacheSink_AddData(
	axl_log_TCacheSink* pSink, 
	ulong_t Mode,
	const void* p,
	size_t Size
	)
{
	ulong_t LineType;
	bool_t ShouldMerge;
	axl_log_TMergeCriteria MergeCriteria;

	if (Mode == axl_log_ERepresentMode_Delimiter)
	{
		if (Size)
			axl_log_TCacheSink_AddDelimiter(pSink, *(uchar_t*) p);

		return;
	}

	if (Size == 0 || pSink->m_IsFull)
		return;

	LineType = axl_log_TRepresentSink_GetLineType((axl_log_TRepresentSink*) pSink, Mode);

	MergeCriteria.m_PartCode = pSink->m_PartCode;
	MergeCriteria.m_Merge = pSink->m_Merge;
	MergeCriteria.m_DataConfig = pSink->m_DataConfig;
	MergeCriteria.m_Timestamp = pSink->m_Timestamp;
	MergeCriteria.m_LineType = LineType;

	ShouldMerge = axl_log_TMergeCriteria_ShouldMerge(&pSink->m_MergeCriteria, &MergeCriteria);
	if (!ShouldMerge)
	{
		pSink->m_BinOffset = 0;
		pSink->m_PartIndex = 0;
		pSink->m_MergeId++;
	}
	else
	{
		axl_log_TLine* pLine = axl_log_TCacheSink_GetCurrentLine(pSink);
		pLine->m_LastTimestamp = pSink->m_Timestamp;
	}

	pSink->m_MergeCriteria = MergeCriteria;

	switch (LineType)
	{
	case axl_log_ELine_Text:
		axl_log_TCacheSink_AddText(pSink, (const tchar_t*) p, Size / sizeof(tchar_t), ShouldMerge, Mode == axl_log_ERepresentMode_HyperText);
		break;

	case axl_log_ELine_BinHex:
		axl_log_TCacheSink_AddBinHex(pSink, p, Size, ShouldMerge);
		break;

	case axl_log_ELine_BinText:
		axl_log_TCacheSink_AddBinText(pSink, p, Size, ShouldMerge);
		break;

	}

	pSink->m_PartIndex++;
}

//.............................................................................

