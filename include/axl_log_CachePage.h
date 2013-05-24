// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_CACHEPAGE_H

#include "axl_log_BinLine.h"
#include "axl_rtl_HashTable.h"
#include "axl_log_MergeCriteria.h"

namespace axl {
namespace log {

class CCacheMgr;

//.............................................................................

struct TCacheVolatilePacket 
{ 
	size_t m_Offset;
	uint_t m_Code;
	uint64_t m_Timestamp;
	size_t m_DataSize; 
	size_t m_VolatileIndex;
	uint_t m_VolatileFlags;
	CLine* m_pFirstLine;
	size_t m_LineCount;

	TCacheVolatilePacket ()
	{
		memset (this, 0, sizeof (TCacheVolatilePacket));
	}
};

//.............................................................................

class CCachePage: public rtl::TListLink
{
	friend class CCacheRepresentorTarget;
	friend class CCacheMgr;
	friend class CColorizeMgr;

protected:
	CCacheMgr* m_pCacheMgr;

	rtl::CHashTableMapIteratorT <size_t, CCachePage*> m_CacheMapIt; // log index node -> cache page
	rtl::CArrayT <CLine*> m_LineArray;
	rtl::CArrayT <TCacheVolatilePacket*> m_VolatilePacketArray;

	// last represent final state

	size_t m_PacketCount;
	size_t m_PacketOffset;
	size_t m_BinOffset;
	size_t m_PartIndex;
	size_t m_MergeId;
	TBinDataConfig m_DefBinDataConfig;
	TMergeCriteria m_MergeCriteria;

	// last colorize final state

	size_t m_ColorizedLineCount;
	CBinLine* m_pColorizerLine;
	intptr_t m_ColorizerState;
	ref::CPtrT <void> m_ColorizerStateEx;
	size_t m_ColorizerOffset;

	// incremental update

	bool m_IsRepresentNeeded;
	bool m_IsColorizeNeeded;

public:
	CCachePage ();

	~CCachePage ();

	bool
	IsEmpty ()
	{
		return m_LineArray.IsEmpty ();
	}

	size_t
	GetLineCount ()
	{
		return m_LineArray.GetCount ();
	}

	CLine*
	GetLine (size_t i)
	{
		return i < m_LineArray.GetCount () ? m_LineArray [i] : NULL;
	}

	CCacheMgr*
	GetCacheMgr ()
	{
		return m_pCacheMgr;
	}

	CBinLine*
	ScanUntilBinLine (CLine* pLine);

	void
	AddLines (
		size_t Line,
		intptr_t LineCount
		);

	void
	FreeLines (
		size_t Line,
		size_t LineCount
		);

	CBinLine*
	FindBinLineByOffset (
		CBinLine* pLine,
		size_t Offset
		);

	void
	ClearColorization ();
	
	void
	IncrementalUpdate ()
	{
		m_IsRepresentNeeded = true;
		m_IsColorizeNeeded = true;
	}

	size_t 
	GetVolatilePacketAnchorLine (TCacheVolatilePacket* pVolatilePacket);
};

//.............................................................................

} // namespace log {
} // namespace axl {
