// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_RepresentSink.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TCacheSink  axl_log_TCacheSink;

void 
AXL_API
axl_log_TCacheSink_Construct(axl_log_TCacheSink* pSink);

void
AXL_API
axl_log_TCacheSink_Destruct(axl_log_TCacheSink* pSink);

#define axl_log_TCacheSink_CanAddLines(pSink) \
	((pSink)->m_Line < (pSink)->m_EndLine)

#define axl_log_TCacheSink_GetCurrentLine(pSink) \
	((pSink)->m_Line ? ((axl_log_TLine**) (pSink)->m_pPage->m_LineArray.m_p)[(pSink)->m_Line - 1] : (pSink)->m_pPrevPageMergeLine)

axl_log_TLine* 
AXL_API
axl_log_TCacheSink_CreatePrevPageMergeLine(
	axl_log_TCacheSink* pSink,
	axl_log_TMergeCriteria* pMergeCriteria,
	ulong_t MergeTrailCol
	);

void 
AXL_VFT
axl_log_TCacheSink_AddData(
	axl_log_TCacheSink* pSink, 
	ulong_t Mode,
	const void* p,
	size_t Size
	);

#ifdef __cplusplus
struct _axl_log_TCacheSink: axl_log_TRepresentSink {
#else
struct _axl_log_TCacheSink { axl_log_TRepresentSink;
#endif
	axl_log_TCachePage* m_pPage;
	axl_log_TCacheVolatileMessage* m_pVolatileMessage;
	axl_log_TMergeCriteria m_MergeCriteria;
	axl_log_TLine* m_pPrevPageMergeLine;
	bool_t m_IsFirstLineOfMessage;
	ulonglong_t m_Timestamp;
	size_t m_MessageOffset;
	size_t m_BinOffset;
	size_t m_PartIndex;
	ulong_t m_MergeId;
	ulong_t m_LongestTextLine;
	ulong_t m_LongestBinHexLine;
	ulong_t m_LongestBinTextLine;
	bool_t m_IsFull;
	size_t m_Line;
	size_t m_EndLine;
};

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TCacheSink TCacheSink;

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
