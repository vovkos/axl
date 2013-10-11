// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_FILTERMGR_H

#include "axl_log_Protocol.h"
#include "axl_log_Filter.h"
#include "axl_log_PacketFile.h"

namespace axl {
namespace log {

//.............................................................................

class CFilterMgr
{
protected:
	struct TFilterEntry: rtl::TListLink
	{
		CFilter* m_pFilter;
		CPacketFile m_PacketFile;
		uint64_t m_SyncId;
		uint64_t m_LastOffset;
	};

protected:
	CPacketFile* m_pSourcePacketFile;
	rtl::CStdListT <TFilterEntry> m_FilterStack;
	
public:
	bool 
	IsEmpty ()
	{
		return m_FilterStack.IsEmpty ();
	}

	void
	Setup (CPacketFile* pPacketFile)
	{
		m_pSourcePacketFile = pPacketFile;
	}

	CPacketFile* 
	GetSourcePacketFile ()
	{
		return m_pSourcePacketFile;
	}
		
	CPacketFile*
	GetTargetPacketFile ()
	{
		ASSERT (!m_FilterStack.IsEmpty ());
		return &m_FilterStack.GetTail ()->m_PacketFile;
	}

	uint64_t
	Process (
		uint64_t SyncId,
		uint64_t TotalPacketSize
		);

protected:
	void
	FilterFile (
		uint64_t SyncId,
		TFilterEntry* pFilterEntry,
		CPacketFile* pPacketFile,
		uint64_t EndOffset
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
