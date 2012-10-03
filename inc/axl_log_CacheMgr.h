// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_MessageFile.h"
#include "axl_log_IndexFile.h"
#include "axl_log_CachePage.h"

typedef struct _axl_log_TLogCtrl axl_log_TLogCtrl;

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TReRepresent
{
	ulong_t m_AnchorPageLine;
	ulong_t m_AnchorAbsoluteLine;
	ulong_t m_OldLineCount;
	ulong_t m_NewLineCount;
} axl_log_TReRepresent;

//.............................................................................

typedef struct _axl_log_TCacheMgr axl_log_TCacheMgr;

void
AXL_API
axl_log_TCacheMgr_Construct(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLogCtrl* pLogCtrl
	);

void
AXL_API
axl_log_TCacheMgr_Destruct(axl_log_TCacheMgr* pCacheMgr);

void
AXL_API
axl_log_TCacheMgr_ClearCache(axl_log_TCacheMgr* pCacheMgr);

void
AXL_API
axl_log_TCacheMgr_ClearCacheColorization(axl_log_TCacheMgr* pCacheMgr);

void
AXL_API
axl_log_TCacheMgr_RemoveCachePage(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage
	);

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_FindCachePageByIndexNode(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TIndexNode* pIndexNode
	);

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_GetCachePageByIndexNode(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TIndexNode* pIndexNode
	);

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_FindCachePageByLine(
	axl_log_TCacheMgr* pCacheMgr,
	ulong_t Line,
	ulong_t* pStartLine
	);

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_GetCachePageByLine(
	axl_log_TCacheMgr* pCacheMgr,
	ulong_t Line,
	ulong_t* pStartLine
	);

ulong_t 
AXL_API
axl_log_TCacheMgr_GetCachePageStartLine(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage
	);

ulong_t 
AXL_API
axl_log_TCacheMgr_GetAbsoluteLine(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine
	);

axl_log_TLine*
AXL_API
axl_log_TCacheMgr_FindLine(
	axl_log_TCacheMgr* pCacheMgr,
	ulong_t Line
	);

axl_log_TLine*
AXL_API
axl_log_TCacheMgr_GetLine(
	axl_log_TCacheMgr* pCacheMgr,
	ulong_t Line
	);

AXL_INLINE
axl_log_TLine*
axl_log_TCacheMgr_GetNextLine(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine
	);

AXL_INLINE
axl_log_TLine*
axl_log_TCacheMgr_GetPrevLine(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine
	);

axl_log_TLine*
AXL_API
axl_log_TCacheMgr_GetLineIncrement(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine,
	size_t Delta
	);

axl_log_TLine*
AXL_API
axl_log_TCacheMgr_GetLineDecrement(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine,
	size_t Delta
	);

void
AXL_API
axl_log_TCacheMgr_ReRepresentMessage(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage,
	axl_log_TCacheVolatileMessage* pVolatileMsg,
	axl_log_TReRepresent* pReRepresent
	);

bool_t
AXL_API
axl_log_TCacheMgr_ModifyMessageVolatileFlags(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage,
	axl_log_TCacheVolatileMessage* pVolatileMsg,
	ulong_t VolatileFlags,
	axl_log_TReRepresent* pReRepresent
	);

typedef struct _axl_log_TCacheMgr 
{ 
	axl_log_TLogCtrl* m_pLogCtrl;

	ulong_t m_LongestTextLine;
	ulong_t m_LongestBinHexLine;
	ulong_t m_LongestBinTextLine;
	size_t m_CacheMaxSize;

	axl_rtl_TList m_Cache;
	axl_rtl_TBinTree m_CacheMap;

#ifdef __cplusplus
	void ClearCache()
		{ axl_log_TCacheMgr_ClearCache(this); }

	void ClearCacheColorization()
		{ axl_log_TCacheMgr_ClearCacheColorization(this); }

	axl_log_TCachePage* FindCachePageByIndexNode(axl_log_TIndexNode* pIndexNode)
		{ return axl_log_TCacheMgr_FindCachePageByIndexNode(this, pIndexNode); }

	axl_log_TCachePage* GetCachePageByIndexNode(axl_log_TIndexNode* pIndexNode)
		{ return axl_log_TCacheMgr_GetCachePageByIndexNode(this, pIndexNode); }

	axl_log_TCachePage* FindCachePageByLine(ulong_t Line, ulong_t* pStartLine)
		{ return axl_log_TCacheMgr_FindCachePageByLine(this, Line, pStartLine); }

	axl_log_TCachePage* GetCachePageByLine(ulong_t Line, ulong_t* pStartLine)
		{ return axl_log_TCacheMgr_GetCachePageByLine(this, Line, pStartLine); }

	axl_log_TLine* FindLine(ulong_t Line)
		{ return axl_log_TCacheMgr_FindLine(this, Line); }

	axl_log_TLine* GetLine(ulong_t Line)
		{ return axl_log_TCacheMgr_GetLine(this, Line); }

	axl_log_TLine* GetNextLine(axl_log_TLine* pLine)
		{ return axl_log_TCacheMgr_GetNextLine(this, pLine); }

	axl_log_TLine* GetPrevLine(axl_log_TLine* pLine)
		{ return axl_log_TCacheMgr_GetPrevLine(this, pLine); }

	void ReRepresentMessage(axl_log_TCachePage* pPage, axl_log_TCacheVolatileMessage* pVolatileMsg, axl_log_TReRepresent* pReRepresent)
		{ axl_log_TCacheMgr_ReRepresentMessage(this, pPage, pVolatileMsg, pReRepresent); }

	bool ModifyMessageVolatileFlags(axl_log_TCachePage* pPage, axl_log_TCacheVolatileMessage* pVolatileMsg, ulong_t VolatileFlags, axl_log_TReRepresent* pReRepresent)
		{ return axl_log_TCacheMgr_ModifyMessageVolatileFlags(this, pPage, pVolatileMsg, VolatileFlags, pReRepresent) != 0; }
#endif
} axl_log_TCacheMgr;

//.............................................................................

AXL_INLINE
axl_log_TLine*
axl_log_TCacheMgr_GetNextLine(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine
	)
{
	return axl_log_TCacheMgr_GetLineIncrement(pCacheMgr, pLine, 1);
}

AXL_INLINE
axl_log_TLine*
axl_log_TCacheMgr_GetPrevLine(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine
	)
{
	return axl_log_TCacheMgr_GetLineDecrement(pCacheMgr, pLine, 1);
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TCacheMgr TCacheMgr;

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
