#include "axl_Precomp.h"
#include "axl_log_Line.h"
#include "axl_log_CachePage.h"
#include "axl_base_Trace.h"

//.............................................................................

void 
AXL_API
axl_log_TLine_Construct(
	axl_log_TLine* pLine, 
	ulong_t Type
	)
{
	pLine->m_pVolatileMessage = NULL;
	pLine->m_IsFirstLineOfMessage = false;
	pLine->m_FirstMessageOffset = 0;
	pLine->m_FirstTimestamp = 0;
	pLine->m_LastTimestamp = 0;
	pLine->m_pPage = NULL;
	pLine->m_PageIndex = 0;
	pLine->m_Type = Type;
	pLine->m_UserParam = 0;
	pLine->m_PartCode = 0;
	pLine->m_PartIndex = 0;
	pLine->m_MergeId = 0;
	pLine->m_DataConfig = g_axl_log_TDataConfig_Default;
	pLine->m_Icon = -1;
	axl_gr_TTextAttr_Construct(&pLine->m_Attr);
	pLine->m_UpperDelimiter = 0;
	pLine->m_LowerDelimiter = 0;
}

void 
AXL_API
axl_log_TLine_VDestruct(axl_log_TLine* pLine)
{
	switch (pLine->m_Type)
	{
	case axl_log_ELine_Text:
		axl_log_TTextLine_Destruct((axl_log_TTextLine*) pLine);
		break;

	case axl_log_ELine_BinHex:
		axl_log_TBinLine_Destruct((axl_log_TBinLine*) pLine);
		break;

	case axl_log_ELine_BinText:
		axl_log_TBinTextLine_Destruct((axl_log_TBinTextLine*) pLine);
		break;
	}
}

int 
AXL_API
axl_log_TLine_GetDelimiter(
	axl_log_TLine* pLine, 
	axl_log_TLine* pNextLine
	)
{
	if (!pNextLine)
		return pLine->m_LowerDelimiter;

	if (axl_log_TLine_IsMerged(pLine, pNextLine))
		return 0;

	return max(pLine->m_LowerDelimiter, pNextLine->m_UpperDelimiter);
}

bool_t
AXL_API
axl_log_TLine_IsMerged(
	axl_log_TLine* pLine, 
	axl_log_TLine* pLine2
	)
{
	return 
		pLine->m_pVolatileMessage == pLine2->m_pVolatileMessage &&
		pLine->m_MergeId == pLine2->m_MergeId;
}

//.............................................................................

size_t
AXL_API
axl_log_TBinLinePartArray_AddPart(
	axl_rtl_TArray* pArray,
	size_t Offset,
	ulonglong_t Timestamp
	)
{
	axl_log_TBinLinePart Part;
	Part.m_Offset = Offset;
	Part.m_Timestamp = Timestamp;
	axl_rtl_TArray_Append(pArray, &Part, 1, sizeof(axl_log_TBinLinePart));

	return axl_rtl_TArray_GetCount(pArray);
}

//.............................................................................

void
AXL_API
axl_log_TBinLine_Construct(axl_log_TBinLine* pLine)
{
	axl_log_TLine_Construct((axl_log_TLine*) pLine, axl_log_ELine_BinHex);
	axl_rtl_TByteArray_Construct(&pLine->m_BinData);
	axl_rtl_TArray_Construct(&pLine->m_PartArray);
	axl_gr_TTextAttrAnchorArray_Construct(&pLine->m_RepresentorAttrArray); 
	axl_gr_TTextAttrAnchorArray_Construct(&pLine->m_AttrArray);
	pLine->m_Offset = 0;
}

void
AXL_API
axl_log_TBinLine_Destruct(axl_log_TBinLine* pLine)
{
	axl_rtl_TByteArray_Destruct(&pLine->m_BinData);
	axl_rtl_TArray_Destruct(&pLine->m_PartArray);
	axl_gr_TTextAttrAnchorArray_Destruct(&pLine->m_RepresentorAttrArray); 
	axl_gr_TTextAttrAnchorArray_Destruct(&pLine->m_AttrArray);
}

axl_log_TBinLinePart* 
AXL_API
axl_log_TBinLine_FindPart(
	axl_log_TBinLine* pLine,
	size_t Offset 
	)
{
	axl_log_TBinLinePart* pResult = NULL;

	size_t Begin = 0;
	size_t End = axl_rtl_TArray_GetCount(&pLine->m_PartArray);

	while (Begin < End)
	{
		size_t Mid = (Begin + End) / 2;

		axl_log_TBinLinePart* pPart = &((axl_log_TBinLinePart*) pLine->m_PartArray.m_p)[Mid];
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
AXL_API
axl_log_TBinLine_CmpOffset(
	axl_log_TBinLine* pLine,
	size_t Offset // full offset
	)
{
	size_t DataSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
	return 
		Offset < pLine->m_Offset ? -1 :
		Offset > pLine->m_Offset + DataSize ? 1 : 0;
}

void
AXL_API
axl_log_TBinLine_Colorize(
	axl_log_TBinLine* pLine, 
	axl_gr_TTextAttr* pAttr,
	size_t OffsetStart,
	size_t OffsetEnd,
	ulong_t Metric
	)
{
	size_t LineSize;

	if (pLine->m_Type == axl_log_ELine_BinText) // need to convert offset -> col
	{	
		OffsetStart = axl_log_TBinTextLine_FindColByOffset((axl_log_TBinTextLine*) pLine, OffsetStart);
		OffsetEnd = axl_log_TBinTextLine_FindColByOffset((axl_log_TBinTextLine*) pLine, OffsetEnd);
	}
	else
	{
		LineSize = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
		if (OffsetStart > LineSize)
			return;

		if (OffsetEnd > LineSize)
			OffsetEnd = LineSize;
	}

	axl_gr_TTextAttrAnchorArray_SetAttr(&pLine->m_AttrArray, pAttr, OffsetStart, OffsetEnd, Metric);
}

void 
AXL_API
axl_log_TBinLine_AddData(
	axl_log_TBinLine* pLine,
	const void* p, 
	size_t Size,
	ulonglong_t Timestamp,
	axl_gr_TTextAttr* pAttr
	)
{
	if (!axl_rtl_TByteArray_IsEmpty(&pLine->m_BinData))
	{
		size_t Offset = pLine->m_Offset + axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
		axl_log_TBinLinePartArray_AddPart(&pLine->m_PartArray, Offset, Timestamp);
	}

	axl_rtl_TByteArray_Append(&pLine->m_BinData, p, Size);
}

//.............................................................................

void
AXL_API
axl_log_TBinTextLine_Construct(axl_log_TBinTextLine* pLine)
{
	axl_log_TBinLine_Construct((axl_log_TBinLine*) pLine);
	pLine->m_Type = axl_log_ELine_BinText;
	axl_rtl_TArray_Construct(&pLine->m_BinTextMap);
	axl_rtl_TString_Construct(&pLine->m_BinText);

}

void
AXL_API
axl_log_TBinTextLine_Destruct(axl_log_TBinTextLine* pLine)
{
	axl_rtl_TArray_Destruct(&pLine->m_BinTextMap);
	axl_rtl_TString_Destruct(&pLine->m_BinText);
	axl_log_TBinLine_Destruct((axl_log_TBinLine*) pLine);
}

size_t 
AXL_API
axl_log_TBinTextLine_FindOffsetByCol(
	axl_log_TBinTextLine* pLine, 
	size_t Col
	)
{
	size_t Size = axl_rtl_TArray_GetCount(&pLine->m_BinData);
	size_t Count = axl_rtl_TArray_GetCount(&pLine->m_BinTextMap);
	axl_log_TLogBinTextMapEntry* pMapEntry = NULL;
	
	if (!Count)
		return 0;

	if (Col >= Count)
		return Size;

	pMapEntry = &((axl_log_TLogBinTextMapEntry*) pLine->m_BinTextMap.m_p) [Col];
	return pMapEntry->m_Offset;
}

size_t 
AXL_API
axl_log_TBinTextLine_FindColByOffset(
	axl_log_TBinTextLine* pLine, 
	size_t Offset
	)
{
	size_t Size = axl_rtl_TArray_GetCount(&pLine->m_BinData);
	size_t Count = axl_rtl_TArray_GetCount(&pLine->m_BinTextMap);
	size_t Begin = 0;
	size_t End = Count;

	axl_log_TLogBinTextMapEntry* pMapEntry = NULL;

	if (Offset >= Size)
		return Count;

	if (!Count)
		return 0;

	while (Begin < End)
	{
		size_t Mid = (Begin + End) / 2;
		pMapEntry = &((axl_log_TLogBinTextMapEntry*) pLine->m_BinTextMap.m_p) [Mid];

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
AXL_API
axl_log_TBinTextLine_AddData(
	axl_log_TBinTextLine* pLine,
	const void* _p, 
	size_t Size,
	ulonglong_t Timestamp,
	axl_gr_TTextAttr* pAttr
	)
{
	enum { SpecialChar = _T('.') };

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;
	size_t Offset = axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
	size_t ActualSize;

	if (!axl_rtl_TByteArray_IsEmpty(&pLine->m_BinData))
	{
		size_t Offset = pLine->m_Offset + axl_rtl_TByteArray_GetCount(&pLine->m_BinData);
		axl_log_TBinLinePartArray_AddPart(&pLine->m_PartArray, Offset, Timestamp);
	}

	for (; p < pEnd; p++, Offset++)
	{
		axl_log_TLogBinTextMapEntry MapEntry;

        size_t TextLength = axl_rtl_TString_GetLength(&pLine->m_BinText);

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

		if (TextLength >= pLine->m_DataConfig.m_BinTextLineSize)
			break;

		MapEntry.m_Col = TextLength;
		MapEntry.m_Offset = Offset;

		if (x != '\t')
		{
			axl_rtl_TArray_Append(&pLine->m_BinTextMap, &MapEntry, 1, sizeof(MapEntry));
			axl_rtl_TString_AppendChar(&pLine->m_BinText, (tchar_t) (isprint(x) ? x : SpecialChar), 1);
			continue;
		}

		TabSize = pLine->m_DataConfig.m_BinTextTabSize - TextLength % pLine->m_DataConfig.m_BinTextTabSize;

		if (TextLength + TabSize > pLine->m_DataConfig.m_BinTextLineSize)
			TabSize = pLine->m_DataConfig.m_BinTextLineSize - TextLength;

		for (i = 0; i <= TabSize / 2; i++)
			axl_rtl_TArray_Append(&pLine->m_BinTextMap, &MapEntry, 1, sizeof(MapEntry));

		MapEntry.m_Col = TextLength + TabSize;
		MapEntry.m_Offset++;

		for (; i < TabSize; i++)
			axl_rtl_TArray_Append(&pLine->m_BinTextMap, &MapEntry, 1, sizeof(MapEntry));

		axl_rtl_TString_AppendChar(&pLine->m_BinText, _T(' '), TabSize);
	}

	ActualSize = p - (uchar_t*) _p;
	axl_rtl_TByteArray_Append(&pLine->m_BinData, _p, ActualSize);

	return ActualSize;
}

//.............................................................................
