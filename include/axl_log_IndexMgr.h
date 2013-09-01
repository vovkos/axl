// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_INDEXMGR_H

#include "axl_log_IndexFile.h"
#include "axl_log_IndexRepresentorTarget.h"
#include "axl_log_PacketFile.h"
#include "axl_log_Represent.h"
#include "axl_log_VolatilePacketFilter.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace log {

//.............................................................................

class CIndexMgr
{
protected:
	CIndexFile m_IndexFile;
	TBinDataConfig m_DefBinDataConfig;

	size_t m_LineCountLimit;
	size_t m_VolatilePacketCountLimit;

	CPacketFile* m_pPacketFile;
	IRepresentor* m_pRepresentor;
	IVolatilePacketFilter* m_pVolatilePacketFilter;

	rtl::CArrayT <TVolatilePacketData> m_VolatilePacketDataBuffer;

public:
	CIndexMgr ();

	CPacketFile*
	GetPacketFile ()
	{
		return m_pPacketFile;
	}

	CIndexFile*
	GetIndexFile ()
	{
		return &m_IndexFile;
	}

	bool
	IsOpen ()
	{	
		return m_IndexFile.IsOpen ();
	}

	bool
	Create (
		CPacketFile* pPacketFile,
		IRepresentor* pRepresentor,
		const char* pIndexFileName,
		uint_t OpenFlags = 0
		);

	void
	Close ();

	IRepresentor*
	GetRepresentor ()
	{
		return m_pRepresentor;
	}

	void
	SetRepresentor (IRepresentor* pRepresentor);

	IVolatilePacketFilter*
	GetVolatilePacketFilter ()
	{
		return m_pVolatilePacketFilter;
	}

	void
	SetVolatilePacketFilter (IVolatilePacketFilter* pFilter);

	size_t 
	FilterVolatilePackets ();

	const TBinDataConfig&
	GetDefBinDataConfig ()
	{
		return m_DefBinDataConfig;
	}

	void 
	SetDefBinDataConfig (const TBinDataConfig& Config);

	void 
	ClearIndex ()
	{
		m_IndexFile.Clear ();
		AddIndexRootNode ();
	}

	size_t 
	UpdateIndex (bool ForceClear = false); // returns anchor packet offset 

	size_t 
	CalcPacketLineCount (
		TPacket* pPacket, 
		const TBinDataConfig& DefBinDataConfig, 
		uint_t VolatileFlags = 0
		);

	size_t 
	CalcPacketLineCount (
		TPacket* pPacket, 
		uint_t VolatileFlags = 0
		)
	{
		return CalcPacketLineCount (pPacket, m_DefBinDataConfig, VolatileFlags);
	}

protected:
	TIndexNode*
	AddIndexRootNode ();

	TIndexNode*
	AddIndexNode ();

	void
	FinalizeIndexNode (
		TIndexNode* pNode,
		const TIndexRepresentorTargetData& RepresentorTargetData,
		size_t PacketCount
		);

	size_t
	IncrementalUpdateIndex ();
};

//.............................................................................

} // namespace log 
} // namespace axl
