// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_Base.h"
#include "axl_gr_HyperText.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

enum
{
	axl_log_ELine_Text = 0,
	axl_log_ELine_BinHex,
	axl_log_ELine_BinText,
};

typedef struct _axl_log_TCachePage            axl_log_TCachePage;
typedef struct _axl_log_TCacheVolatileMessage axl_log_TCacheVolatileMessage;

typedef struct _axl_log_TBinTextLine  axl_log_TBinTextLine;

//.............................................................................

typedef struct _axl_log_TLine axl_log_TLine;

void 
AXL_API
axl_log_TLine_Construct(
	axl_log_TLine* pLine, 
	ulong_t Type
	);

void 
AXL_API
axl_log_TLine_VDestruct(axl_log_TLine* pLine);

#define axl_log_TLine_IsBin(pLine) \
	((pLine)->m_Type == axl_log_ELine_BinHex || (pLine)->m_Type == axl_log_ELine_BinText)

bool_t
AXL_API
axl_log_TLine_IsMerged(
	axl_log_TLine* pLine, 
	axl_log_TLine* pLine2
	);

int 
AXL_API
axl_log_TLine_GetDelimiter(
	axl_log_TLine* pLine, 
	axl_log_TLine* pNextLine
	);

struct _axl_log_TLine
{
	axl_log_TCachePage* m_pPage;
	size_t m_PageIndex;

	ulong_t m_Type;
	intptr_t m_UserParam;

	long m_Icon;
	axl_gr_TTextAttr m_Attr;

	axl_log_TCacheVolatileMessage* m_pVolatileMessage;

	bool_t m_IsFirstLineOfMessage;
	size_t m_FirstMessageOffset;
	ulonglong_t m_FirstTimestamp;
	ulonglong_t m_LastTimestamp;

	ulong_t m_PartCode; 
	ulong_t m_MergeId; // all the lines in the same merged block have the same merge id
	size_t m_PartIndex;

	axl_log_TDataConfig m_DataConfig;

	int m_UpperDelimiter:3;
	int m_LowerDelimiter:3;
};

//.............................................................................

typedef struct _axl_log_TTextLine axl_log_TTextLine;

#ifdef __cplusplus
struct _axl_log_TTextLine: axl_log_TLine {
#else
struct _axl_log_TTextLine { axl_log_TLine;
#endif
	axl_gr_THyperText m_HyperText;
};

AXL_INLINE
void
axl_log_TTextLine_Construct(axl_log_TTextLine* pLine)
{
	axl_log_TLine_Construct((axl_log_TLine*) pLine, axl_log_ELine_Text);
	axl_gr_THyperText_Construct(&pLine->m_HyperText);
}

AXL_INLINE
void
axl_log_TTextLine_Destruct(axl_log_TTextLine* pLine)
	{ axl_gr_THyperText_Destruct(&pLine->m_HyperText); }

//.............................................................................

typedef struct _axl_log_TBinLinePart axl_log_TBinLinePart;

struct _axl_log_TBinLinePart 
{
	size_t m_Offset;
	ulonglong_t m_Timestamp;
};

size_t
AXL_API
axl_log_TBinLinePartArray_AddPart(
	axl_rtl_TArray* pArray,
	size_t Offset,
	ulonglong_t Timestamp
	);

AXL_INLINE
size_t
axl_log_TBinLinePartArray_Append(
	axl_rtl_TArray* pArray,
	axl_log_TBinLinePart* pPart,
	size_t Count
	)
	{ return axl_rtl_TArray_Append(pArray, pPart, Count, sizeof(axl_log_TBinLinePart)); }

AXL_INLINE
size_t
axl_log_TBinLinePartArray_AppendArray(
	axl_rtl_TArray* pArray,
	axl_rtl_TArray* pSrc
	)
	{ return axl_rtl_TArray_AppendArray(pArray, pSrc, sizeof(axl_log_TBinLinePart)); }

//.............................................................................

typedef struct _axl_log_TBinLine axl_log_TBinLine;

void
AXL_API
axl_log_TBinLine_Construct(axl_log_TBinLine* pLine);

void
AXL_API
axl_log_TBinLine_Destruct(axl_log_TBinLine* pLine);

void 
AXL_API
axl_log_TBinLine_AddData(
	axl_log_TBinLine* pLine,
	const void* p, 
	size_t Size,
	ulonglong_t Timestamp,
	axl_gr_TTextAttr* pAttr
	);

void
AXL_API
axl_log_TBinLine_Colorize(
	axl_log_TBinLine* pLine, 
	axl_gr_TTextAttr* pAttr,
	size_t OffsetStart,
	size_t OffsetEnd,
	ulong_t Metric
	);

int
AXL_API
axl_log_TBinLine_CmpOffset(
	axl_log_TBinLine* pLine,
	size_t Offset // full offset
	);

axl_log_TBinLinePart* // if NULL, then it's first part
AXL_API
axl_log_TBinLine_FindPart(
	axl_log_TBinLine* pLine,
	size_t Offset // full offset
	);

AXL_INLINE
size_t 
axl_log_TBinLine_GetSize(axl_log_TBinLine* pLine);

#ifdef __cplusplus
struct _axl_log_TBinLine: axl_log_TLine {
#else
struct _axl_log_TBinLine { axl_log_TLine;
#endif
	size_t m_Offset;
	axl_rtl_TByteArray m_BinData;
	axl_rtl_TArray m_PartArray;
	
	axl_gr_TTextAttrAnchorArray m_RepresentorAttrArray; 
	axl_gr_TTextAttrAnchorArray m_AttrArray;
};

AXL_INLINE
size_t 
axl_log_TBinLine_GetSize(axl_log_TBinLine* pLine)
{
	return pLine->m_Type == axl_log_ELine_BinText ? 
		pLine->m_DataConfig.m_BinTextLineSize :
		pLine->m_DataConfig.m_BinHexLineSize;
}

//.............................................................................

// because of the tabs col<->offset mapping is not linear

typedef struct _axl_log_TLogBinTextMapEntry
{
	size_t m_Col;
	size_t m_Offset;
} axl_log_TLogBinTextMapEntry;

//.............................................................................

typedef struct _axl_log_TBinTextLine axl_log_TBinTextLine;

void
AXL_API
axl_log_TBinTextLine_Construct(axl_log_TBinTextLine* pLine);

void
AXL_API
axl_log_TBinTextLine_Destruct(axl_log_TBinTextLine* pLine);

size_t
AXL_API
axl_log_TBinTextLine_AddData(
	axl_log_TBinTextLine* pLine,
	const void* p, 
	size_t Size,
	ulonglong_t Timestamp,
	axl_gr_TTextAttr* pAttr
	);

size_t 
AXL_API
axl_log_TBinTextLine_FindOffsetByCol(
	axl_log_TBinTextLine* pLine, 
	size_t Col
	);

size_t 
AXL_API
axl_log_TBinTextLine_FindColByOffset(
	axl_log_TBinTextLine* pLine, 
	size_t Offset
	);

#ifdef __cplusplus
struct _axl_log_TBinTextLine: axl_log_TBinLine {
#else
struct _axl_log_TBinTextLine { axl_log_TBinLine;
#endif
	axl_rtl_TString m_BinText;
	axl_rtl_TArray m_BinTextMap;
};

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

//.............................................................................

struct ELine { enum 
{
	Text    = axl_log_ELine_Text,
	BinHex  = axl_log_ELine_BinHex,
	BinText = axl_log_ELine_BinText,
};};

//.............................................................................

typedef axl_log_TLine         TLine;
typedef axl_log_TTextLine     TTextLine;
typedef axl_log_TBinLine      TBinLine;
typedef axl_log_TBinLinePart  TBinLinePart;
typedef axl_log_TBinTextLine  TBinTextLine;

//.............................................................................

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
