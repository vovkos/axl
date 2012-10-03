// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_gr_TextPaint.h"
#include "axl_ref_Object.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

enum
{
	axl_log_EBinView_Hex  = 0,
	axl_log_EBinView_Text = 1,
};

enum
{
	axl_log_EMergeThreshold_Always = (ulonglong_t) -1,
	axl_log_EMergeThreshold_Never  = 0,
};

enum
{
	axl_log_EDef_BinView          = axl_log_EBinView_Hex,
	axl_log_EDef_BinHexLineSize   = 16,     // 16 bytes per line in hex/merged hex by default
	axl_log_EDef_BinTextLineSize  = 80,     // force line wrap every 80 characters by default
	axl_log_EDef_BinTextTabSize   = 4,      // unfortunately, tab size cannot dynamically be changed 
	axl_log_EDef_CacheMaxSize     = 64,     // 64 max pages in cache
	axl_log_EDef_IconGapSize      = 1,
	axl_log_EDef_TimestampGapSize = 1,
	axl_log_EDef_OffsetGapSize    = 2,
	axl_log_EDef_HexGapSize       = 1,
	axl_log_EDef_CaretWidth       = 2,

	axl_log_EDef_ColorizeCrossoverLimit         = 4,  // don't look further than 4 lines back/forth on page borders
	axl_log_EDef_IndexLineCountLimit            = 32, // add index node every 32 lines*
	axl_log_EDef_IndexVolatileMessageCountLimit = 32, // or every 32 volatile messages*
	                                                  // * - on average, because messages are never split between nodes
	
	axl_log_EDef_MaxBinBlockBuffer = 1024,  // incapsulating 1K before passing to bin block processor
};

enum
{
	axl_log_EMerge_None     = 0,
	axl_log_EMerge_Backward = 1,
	axl_log_EMerge_Forward  = 2,
	axl_log_EMerge_Normal   = 3,
};

//.............................................................................

typedef struct _axl_log_TDataConfig
{
	ushort_t m_BinView:1;
	ushort_t m_BinTextTabSize:15;
	ushort_t m_BinHexLineSize; // binary data is auto-wrapped
	ushort_t m_BinTextLineSize; // binary data is auto-wrapped
	ulonglong_t m_TextMergeThreshold;
	ulonglong_t m_BinMergeThreshold;
} axl_log_TDataConfig;

#define axl_log_TDataConfig_Compare(pConfig, pConfig2) \
	(memcmp(pConfig, pConfig2, sizeof(axl_log_TDataConfig)))

#define axl_log_TDataConfig_GetBinLineSize(pConfig) \
	((pConfig)->m_BinView ? (pConfig)->m_BinTextLineSize : (pConfig)->m_BinHexLineSize)

extern axl_log_TDataConfig g_axl_log_TDataConfig_Default;

//.............................................................................

typedef struct _axl_log_TMergeCriteria axl_log_TMergeCriteria;

#define axl_log_TMergeCriteria_Construct(pCriteria) \
	memset(pCriteria, 0, sizeof(axl_log_TMergeCriteria))

#define axl_log_TMergeCriteria_Clear(pCriteria) \
	((pCriteria)->m_Merge = 0)

bool_t
AXL_API
axl_log_TMergeCriteria_ShouldMerge(
	axl_log_TMergeCriteria* pCriteria,
	axl_log_TMergeCriteria* pCriteria2
	);

typedef struct _axl_log_TMergeCriteria
{
	ulong_t m_PartCode;
	ulong_t m_Merge:8;
	ulong_t m_LineType:8;
	axl_log_TDataConfig m_DataConfig;
	ulonglong_t m_Timestamp;

#ifdef __cpluplus
	void Clear()
		{ axl_log_TMergeCriteria_Clear(this); }

	bool ShouldMerge(axl_log_TMergeCriteria* pCriteria2)
		{ return axl_log_TMergeCriteria_ShouldMerge(this, pCriteria2) != 0; }
#endif
} axl_log_TMergeCriteria;

//.............................................................................

typedef struct _axl_log_TIndexNodeData
{
	ulong_t m_MessageOffset;
	ulong_t m_MessageCount;
	ulong_t m_VolatileMessageCount;
	ulong_t m_BinOffset;
	ulong_t m_PartIndex;
	ulong_t m_MergeId;
	ulong_t m_Col;
	axl_log_TMergeCriteria m_MergeCriteria;
	axl_log_TDataConfig m_PreferredDataConfig;
} axl_log_TIndexNodeData;

//.............................................................................

// every volatile message in the node has axl_log_TVolatileMessageData

typedef struct _axl_log_TVolatileMessageData
{
	ulong_t m_VolatileFlags; // hidden, expanded, etc.
	ulong_t m_Offset;        // for effective re-filtering all messages (this way we can iterate through volatile msg array)
	ulong_t m_LineCount;     // for effective re-representing of messages that are not in cache
} axl_log_TVolatileMessageData;

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

struct EBinView { enum
{
	Hex  = axl_log_EBinView_Hex,
	Text = axl_log_EBinView_Text,
};};

struct EMergeThreshold { enum
{
	Always = axl_log_EMergeThreshold_Always,
	Never  = axl_log_EMergeThreshold_Never,
};};

struct EDef { enum
{
	BinView           = axl_log_EDef_BinView,
	BinHexLineSize    = axl_log_EDef_BinHexLineSize, 
	BinTextLineSize   = axl_log_EDef_BinTextLineSize,
	BinTextTabSize    = axl_log_EDef_BinTextTabSize,
	CacheMaxSize      = axl_log_EDef_CacheMaxSize,
	IconGapSize       = axl_log_EDef_IconGapSize,
	TimestampGapSize  = axl_log_EDef_TimestampGapSize,
	HexGapSize        = axl_log_EDef_HexGapSize,
	CaretWidth        = axl_log_EDef_CaretWidth,

	ColorizeCrossoverLimit         = axl_log_EDef_ColorizeCrossoverLimit,
	IndexLineCountLimit            = axl_log_EDef_IndexLineCountLimit,
	IndexVolatileMessageCountLimit = axl_log_EDef_IndexVolatileMessageCountLimit,
};};

struct EMerge { enum
{
	None     = axl_log_EMerge_None,
	Backward = axl_log_EMerge_Backward,
	Forward  = axl_log_EMerge_Forward,
	Normal   = axl_log_EMerge_Normal,
};};

//.............................................................................

typedef axl_log_TDataConfig           TDataConfig;
typedef axl_log_TMergeCriteria        TMergeCriteria;
typedef axl_log_TIndexNodeData        TIndexNodeData;
typedef axl_log_TVolatileMessageData TIndexNodeMessageData;

//.............................................................................

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
