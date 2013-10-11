// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_INDEXMGR_H

#include "axl_log_Protocol.h"
#include "axl_log_Representer.h"
#include "axl_log_PacketFile.h"
#include "axl_log_MergeFile.h"

namespace axl {
namespace log {

//.............................................................................

class CIndexMgr
{
protected:
	CClient* m_pClient;
	CRepresenter* m_pRepresenter;
	CPacketFile* m_pPacketFile;
	CMergeFile* m_pMergeFile;

	size_t m_LeafLineCountLimit;
	size_t m_LeafFoldablePacketCountLimit;

	uint64_t m_LeafOffset;
	bool m_IsLeafCreated;
	
	TBinDataConfig m_BinDataConfig;
	uint64_t m_FoldFlags;

	uint64_t m_PacketOffset;
	size_t m_PacketCount;
	size_t m_FoldablePacketCount;
	size_t m_LineCount;
	uint32_t m_Col;
	uint64_t m_PartIdx;
	uint64_t m_BinOffset;
	TMergeCriteria m_MergeCriteria;

	uint64_t m_PrevLeafPacketOffset;
	size_t m_PrevLeafPacketCount;

public:
	CIndexMgr ();

	void
	Setup (
		CClient* pClient,
		CRepresenter* pRepresenter,
		CPacketFile* pPacketFile,
		CMergeFile* pMergeFile
		);

	void 
	SetBinDataConfig (const TBinDataConfig* pBinDataConfig);

	void
	ClearIndex ();

	void
	UpdateIndex (uint64_t TotalPacketSize);

	void
	FoldPacketComplete (
		uint64_t LeafOffset,	
		size_t OldLineCount,
		size_t NewLineCount
		)
	{
		if (LeafOffset == m_LeafOffset)
			m_LineCount += NewLineCount - OldLineCount;
	}

protected:	
	void
	UpdateLeaf (
		size_t PacketCount,
		size_t FoldablePacketCount,
		size_t LineCount
		);

	void
	FinalizeLeaf (
		uint64_t PacketOffset,
		size_t PacketCount,
		size_t FoldablePacketCount,
		size_t LineCount
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
