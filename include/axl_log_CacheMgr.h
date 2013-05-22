// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_CACHEMGR_H

#include "axl_log_IndexMgr.h"
#include "axl_log_CachePage.h"

namespace axl {
namespace log {

//.............................................................................

struct TReRepresent
{
	size_t m_AnchorPageLine;
	size_t m_AnchorAbsoluteLine;
	size_t m_OldLineCount;
	size_t m_NewLineCount;
};

//.............................................................................

class CCacheMgr 
{ 	
protected:
	friend class CWidget;

	CWidget* m_pWidget;
	CIndexMgr* m_pIndexMgr;

	size_t m_LongestTextLine;
	size_t m_LongestBinHexLine;
	size_t m_LongestBinTextLine;
	size_t m_CacheMaxSize;

	rtl::CStdListT <CCachePage> m_CachePageList;
	rtl::CHashTableMapT <size_t, CCachePage*, rtl::CHashIdT <size_t> > m_CachePageMap;

public:
	CCacheMgr ();

	CWidget*
	GetWidget ()
	{
		return m_pWidget;
	}

	size_t 
	GetLongestTextLine ()
	{
		return m_LongestTextLine;
	}

	size_t 
	GetLongestBinHexLine ()
	{
		return m_LongestBinHexLine;
	}

	size_t 
	GetLongestBinTextLine ()
	{
		return m_LongestBinTextLine;
	}

	void 
	ClearCache ();

	void 
	ClearCacheColorization ();

	CCachePage* 
	FindCachePageByIndexNode (TIndexNode* pIndexNode);

	CCachePage* 
	GetCachePageByIndexNode (TIndexNode* pIndexNode);

	CCachePage* 
	FindCachePageByLine (
		size_t Line, 
		size_t* pStartLine
		);

	CCachePage* 
	GetCachePageByLine (
		size_t Line, 
		size_t* pStartLine
		);

	CCachePage* 
	GetNextCachePage (CCachePage* pPage);

	CCachePage* 
	GetPrevCachePage (CCachePage* pPage);

	void
	RemoveCachePage (CCachePage* pPage);

	CLine* 
	FindLine (size_t Line);

	CLine* 
	GetLine (size_t Line);

	CLine* 
	GetNextLine (CLine* pLine)
	{
		return GetLineIncrement (pLine, 1);
	}

	CLine* 
	GetPrevLine (CLine* pLine)
	{
		return GetLineDecrement (pLine, 1);
	}

	CLine*
	GetLineIncrement (
		CLine* pLine,
		size_t Delta
		);

	CLine*
	GetLineDecrement (
		CLine* pLine,
		size_t Delta
		);

	size_t
	GetAbsoluteLine (CLine* pLine)
	{
		return GetCachePageStartLine (pLine->m_pPage) + pLine->m_PageIndex;
	}

	void 
	ReRepresentPacket (
		CCachePage* pPage, 
		TCacheVolatilePacket* pVolatileMsg, 
		TReRepresent* pReRepresent
		);

	bool 
	ModifyPacketVolatileFlags (
		CCachePage* pPage, 
		TCacheVolatilePacket* pVolatileMsg, 
		uint_t VolatileFlags, 
		TReRepresent* pReRepresent
		);

protected:
	TIndexNode* 
	GetCachePageIndexNode (CCachePage* pPage);

	size_t
	GetCachePageStartLine (CCachePage* pPage);

	bool
	CachePage (
		CCachePage* pPage,
		TIndexNode* pIndexNode
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {

