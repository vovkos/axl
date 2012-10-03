#include "axl_Precomp.h"
#include "axl_log_IndexSink.h"

//.............................................................................

void
AXL_API
axl_log_TIndexSink_Construct(axl_log_TIndexSink* pSink)
{
	static axl_log_TRepresentSinkVTable _VTable = 
	{
		axl_log_TIndexSink_AddData
	};

	axl_log_TRepresentSink_Construct((axl_log_TRepresentSink*) pSink);
	axl_log_TMergeCriteria_Construct(&pSink->m_MergeCriteria);

	pSink->m_pVTable = &_VTable;
	pSink->m_Timestamp = 0;
	pSink->m_LineCount = 0;
	pSink->m_Col = 0;
	pSink->m_BinOffset = 0;
	pSink->m_PartIndex = 0;
	pSink->m_MergeId = 0;
}

void
AXL_API
axl_log_TIndexSink_Destruct(axl_log_TIndexSink* pSink)
{
	axl_log_TRepresentSink_Destruct((axl_log_TRepresentSink*) pSink);
}

void 
AXL_API
axl_log_TIndexSink_AddText(
	axl_log_TIndexSink* pSink, 
	const tchar_t* pText,
	size_t Length
	)
{
	const tchar_t* p = pText;
	const tchar_t* pEnd = pText + Length;

	for (; p < pEnd; p++)
		if (*p == '\n')
			pSink->m_LineCount++;
}

void 
AXL_API
axl_log_TIndexSink_AddBinText(
	axl_log_TIndexSink* pSink, 
	const void* _p,
	size_t Size
	)
{
	const char* p = (const char*) _p;
	const char* pEnd = p + Size;

	for (; p < pEnd; p++)
		switch (*p)
		{
			ulong_t TabSize;

		case '\r': 
			// ignore '\r' 
			break; 

		case '\n':
			pSink->m_LineCount++, pSink->m_Col = 0;
			break;
		
		case '\t':
			if (pSink->m_Col >= pSink->m_DataConfig.m_BinTextLineSize)
				pSink->m_LineCount++, pSink->m_Col = 0;

			TabSize = pSink->m_DataConfig.m_BinTextTabSize - pSink->m_Col % pSink->m_DataConfig.m_BinTextTabSize;
			
			if (pSink->m_Col + TabSize > pSink->m_DataConfig.m_BinTextLineSize)
				TabSize = pSink->m_DataConfig.m_BinTextLineSize - pSink->m_Col;

			pSink->m_Col += TabSize;
			break;

		default:
			if (pSink->m_Col >= pSink->m_DataConfig.m_BinTextLineSize)
				pSink->m_LineCount++, pSink->m_Col = 0;

			pSink->m_Col++;
		}
}

void 
AXL_API
axl_log_TIndexSink_AddBinHex(
	axl_log_TIndexSink* pSink, 
	size_t Size
	)
{
	Size += pSink->m_Col - 1;
	pSink->m_LineCount += (ulong_t) (Size / pSink->m_DataConfig.m_BinHexLineSize);
	pSink->m_Col = (ulong_t) (Size % pSink->m_DataConfig.m_BinHexLineSize) + 1;
}

void
AXL_VFT
axl_log_TIndexSink_AddData(
	axl_log_TIndexSink* pSink, 
	ulong_t Mode,
	const void* p,
	size_t Size
	)
{
	ulong_t LineType;
	bool_t ShouldMerge;
	axl_log_TMergeCriteria MergeCriteria;

	if (Mode == axl_log_ERepresentMode_Delimiter)
		return;

	if (Size == 0)
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
		pSink->m_Col = 0;
		pSink->m_BinOffset = 0;
		pSink->m_PartIndex = 0;
		pSink->m_MergeId++;
		pSink->m_LineCount++;
	}

	pSink->m_MergeCriteria = MergeCriteria;

	switch (LineType)
	{
	case axl_log_ELine_Text:
		axl_log_TIndexSink_AddText(pSink, (const tchar_t*) p, Size / sizeof(tchar_t));
		break;

	case axl_log_ELine_BinHex:
		axl_log_TIndexSink_AddBinHex(pSink, Size);
		pSink->m_BinOffset += (ulong_t) Size;
		break;

	case axl_log_ELine_BinText:
		axl_log_TIndexSink_AddBinText(pSink, p, Size);
		pSink->m_BinOffset += (ulong_t) Size;
		break;
	}

	pSink->m_PartIndex++;
}

//.............................................................................