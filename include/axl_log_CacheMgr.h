// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_CACHEMGR_H

#include "axl_log_CachePage.h"
#include "axl_log_Protocol.h"

namespace axl {
namespace log {

class CWidget;

//.............................................................................

class CCacheMgr
{ 
protected:
	CWidget* m_pWidget;
	IServer* m_pServer;
	uint64_t m_SyncId;

	TLongestLineLength m_LongestLineLength;

	CIndexFile m_IndexFile;

	rtl::CStdListT <CCachePage> m_CachePageList;
	rtl::CHashTableMapT <uint64_t, CCachePage*, rtl::CHashIdT <uint64_t> > m_CachePageMap;
	size_t m_CachePageCountLimit;

public:
	CCacheMgr ();

	uint64_t 
	GetSyncId ()
	{
		return m_SyncId;
	}

	uint64_t
	GetIndexNextLeafOffset ()
	{
		return !m_IndexFile.IsEmpty () ? m_IndexFile.GetHdr ()->m_TotalNodeSize + sizeof (TIndexNode) : 0;
	}

	uint64_t
	GetIndexTailLeafOffset ()
	{
		return m_IndexFile.GetHdr ()->m_TailLeaf;
	}

	const TIndexLeaf* 
	FindIndexLeaf (
		uint64_t LineIdx,
		uint64_t* pStartLineIdx
		)
	{
		return m_IndexFile.FindLeaf (LineIdx, pStartLineIdx);
	}

	const TIndexLeaf* 
	GetNextIndexLeaf (const TIndexLeaf* pLeaf)
	{
		return m_IndexFile.GetNextLeaf (pLeaf);
	}

	const TIndexLeaf* 
	GetPrevIndexLeaf (const TIndexLeaf* pLeaf)
	{
		return m_IndexFile.GetPrevLeaf (pLeaf);
	}

	bool
	Create (
		CWidget* pWidget,
		IServer* pServer,
		const char* pIndexFileName
		);

	void 
	ClearCache (uint64_t SyncId);

	CCachePage* 
	FindCachePageByIndexLeaf (uint64_t IndexLeafOffset)
	{
		rtl::CHashTableMapIteratorT <uint64_t, CCachePage*> It = m_CachePageMap.Find (IndexLeafOffset);
		return It ? It->m_Value : NULL;
	}

	CCachePage* 
	GetCachePageByIndexLeaf (const TIndexLeaf* pLeaf);

	CCachePage* 
	GetCachePageByLineIdx (
		uint64_t LineIdx,
		uint64_t* pStartLineIndex
		);

	CCachePage* 
	GetNextCachePage (CCachePage* pPage);

	CCachePage* 
	GetPrevCachePage (CCachePage* pPage);

	uint64_t
	GetCachePageStartLineIdx (CCachePage* pPage);

	CLine* 
	GetLine (uint64_t LineIdx);

	CLine* 
	GetNextLine (CLine* pLine);

	CLine* 
	GetPrevLine (CLine* pLine);

	uint64_t
	GetLineIdx (CLine* pLine)
	{
		return GetCachePageStartLineIdx (pLine->m_pPage) + pLine->m_LineIdx;
	}

	const TLongestLineLength*
	GetLongestLineLength ()
	{
		return &m_LongestLineLength;
	}
	
	void 
	CreateIndexLeaf (
		const TBinDataConfig* pBinDataConfig,
		uint64_t LineCount,
		uint64_t PacketOffset,
		uint32_t PacketCount,
		uint32_t FoldablePacketCount,
		uint64_t MergePointIdx,
		uint64_t FoldFlags
		);

	void 
	UpdateIndexTailLeaf (
		uint64_t LineCount,
		uint32_t PacketCount,
		uint32_t FoldablePacketCount,
		uint64_t FoldFlags
		);

	void 
	RepresentPageComplete (
		uint64_t IndexLeafOffset,
		size_t LineIdx,
		size_t LineCount,
		size_t FoldablePacketIdx,
		const TFoldablePacket* pFoldablePacketArray,
		size_t FoldablePacketCount,
		const void* pLineBuffer,
		size_t LineBufferSize
		);

	void 
	FoldPacketComplete (
		uint64_t IndexLeafOffset,
		size_t LineIdx,
		size_t OldLineCount,
		size_t NewLineCount,
		size_t FoldablePacketIdx,
		uint64_t FoldFlags,
		const void* pLineBuffer,
		size_t LineBufferSize
		);

protected:
	CCachePage* 
	GetOrCreateCachePage (const TIndexLeaf* pLeaf);
};

//.............................................................................

} // namespace log {
} // namespace axl {
