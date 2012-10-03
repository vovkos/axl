// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_RepresentSink.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TIndexSink axl_log_TIndexSink;

#ifdef __cplusplus
struct _axl_log_TIndexSink: axl_log_TRepresentSink {
#else
struct _axl_log_TIndexSink { axl_log_TRepresentSink;
#endif
	ulonglong_t m_Timestamp;
	ulong_t m_LineCount;
	ulong_t m_Col;
	ulong_t m_BinOffset;
	ulong_t m_PartIndex;
	ulong_t m_MergeId;
	axl_log_TMergeCriteria m_MergeCriteria;
};

void
AXL_API
axl_log_TIndexSink_Construct(axl_log_TIndexSink* pSink);

void
AXL_API
axl_log_TIndexSink_Destruct(axl_log_TIndexSink* pSink);

void
AXL_VFT
axl_log_TIndexSink_AddData(
	axl_log_TIndexSink* pSink, 
	ulong_t Mode,
	const void* p,
	size_t Size
	);

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TIndexSink TIndexSink;

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
