// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_REPRESENTMGR_H

#include "axl_log_TextLine.h"
#include "axl_log_BinHexLine.h"
#include "axl_log_BinTextLine.h"
#include "axl_log_PacketFile.h"
#include "axl_log_IndexFile.h"
#include "axl_log_MergeFile.h"
#include "axl_log_Protocol.h"
#include "axl_log_Representer.h"
#include "axl_log_Colorizer.h"

namespace axl {
namespace log {

//.............................................................................

struct TLinePool
{
	CTextLine m_TextLine;
	CBinHexLine m_BinHexLine;
	CBinTextLine m_BinTextLine;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CLineRepresentMgr
{ 	
protected:	
	CClientPeer* m_pClientPeer;
	CRepresenter* m_pRepresenter;
	CColorizer* m_pColorizer;
	CPacketFile* m_pPacketFile;
	CMergeFile* m_pMergeFile;
	io::CMappedFile* m_pColorizerStateFile;

	TLinePool m_LinePool;
	TLinePool m_LastLinePool;

	rtl::CArrayT <uchar_t> m_ColorizerState;
	rtl::CArrayT <uchar_t> m_LastLineColorizerState;

	rtl::CArrayT <uchar_t> m_LineBuffer;
	rtl::CArrayT <uchar_t> m_PageBuffer;
	rtl::CArrayT <TFoldablePacket> m_FoldablePacketBuffer;

	uint64_t m_LastLeafOffset;
	uint64_t m_LastLinePacketOffset;
	size_t m_LastLinePacketIdx;
	size_t m_LastLineFoldablePacketIdx;
	size_t m_LastLineIdx;
	uint64_t m_LastLinePartIdx;
	uint64_t m_LastLineBinOffset;
	TMergeCriteria m_LastLineMergeCriteria;
	CLine* m_pLastLine;

public:
	CLineRepresentMgr ();

public:
	void
	ResetIncrementalRepresent ()
	{
		m_LastLeafOffset = -1;
	}
		
	void
	Setup (
		CClientPeer* pClientPeer,
		CRepresenter* pRepresenter,
		CColorizer* pColorizer,
		CPacketFile* pPacketFile,
		CMergeFile* pMergeFile,
		io::CMappedFile* pColorizerStateFile
		);

	void
	RepresentPage (
		uint64_t SyncId,
		const TIndexLeaf* pIndexLeaf,
		const uint64_t* pFoldFlagArray
		);

	size_t
	RepresentFoldablePacket (
		const TBinDataConfig* pBinDataConfig,
		uint64_t SyncId,
		uint64_t IndexLeafOffset,
		uint64_t PacketOffset,
		size_t FoldablePacketIdx,
		size_t FirstLineIdx,
		size_t OldLineCount,
		uint64_t FoldFlags
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {

