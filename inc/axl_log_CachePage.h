// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_Base.h"
#include "axl_rtl_BinTree.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TColorizer axl_log_TColorizer;
typedef struct _axl_log_TLine      axl_log_TLine;
typedef struct _axl_log_TBinLine   axl_log_TBinLine;

//.............................................................................

typedef struct _axl_log_TCacheVolatileMessage axl_log_TCacheVolatileMessage;

struct _axl_log_TCacheVolatileMessage 
{ 
	size_t m_Offset;
	ulong_t m_Code;
	ulonglong_t m_Timestamp;
	size_t m_DataSize; 
	size_t m_VolatileIndex;
	ulong_t m_VolatileFlags;
	axl_log_TLine* m_pFirstLine;
	size_t m_LineCount;
};

//.............................................................................

typedef struct _axl_log_TCachePage            axl_log_TCachePage;

void
AXL_API
axl_log_TCachePage_Construct(axl_log_TCachePage* pPage);

void
AXL_API
axl_log_TCachePage_Destruct(axl_log_TCachePage* pPage);

void
AXL_API
axl_log_TCachePage_ClearColorization(axl_log_TCachePage* pPage);

size_t 
AXL_API
axl_log_TCachePage_GetVolatileMessageAnchorLine(
	axl_log_TCachePage* pPage,
	axl_log_TCacheVolatileMessage* pVolatileMsg
	);

axl_log_TLine*
AXL_API
axl_log_TCachePage_GetLine(
	axl_log_TCachePage* pPage,
	size_t Line
	);

void
AXL_API
axl_log_TCachePage_AddLines(
	axl_log_TCachePage* pPage,
	size_t Line,
	long LineCount
	);

void
AXL_API
axl_log_TCachePage_FreeLines(
	axl_log_TCachePage* pPage,
	size_t Line,
	size_t LineCount
	);

axl_log_TBinLine*
AXL_API
axl_log_TCachePage_FindBinLineByOffset(
	axl_log_TCachePage* pPage,
	axl_log_TBinLine* pLine,
	size_t Offset
	);

#ifdef __cplusplus
struct _axl_log_TCachePage: axl_rtl_TListEntry {
#else
struct _axl_log_TCachePage { axl_rtl_TListEntry;
#endif
	axl_rtl_TBinTreeNode* m_pCacheMapNode; // log index node -> cache page
	axl_rtl_TPtrArray m_LineArray;
	axl_rtl_TPtrArray m_VolatileMessageArray; 

	// last representor cycle final state

	size_t m_MessageCount;
	size_t m_MessageOffset;
	size_t m_BinOffset;
	size_t m_PartIndex;
	ulong_t m_MergeId;
	axl_log_TDataConfig m_PreferredDataConfig;
	axl_log_TMergeCriteria m_MergeCriteria;

	// last colorizer cycle final state

	size_t m_ColorizedLineCount;
	axl_log_TBinLine* m_pColorizerLine;
	intptr_t m_ColorizerState;
	axl_ref_TObject* m_pColorizerStateEx;
	size_t m_ColorizerOffset;

	// update on add

	bool_t m_IsRepresentNeeded:1;
	bool_t m_IsColorizeNeeded:1;
};

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TCachePage TCachePage;

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
