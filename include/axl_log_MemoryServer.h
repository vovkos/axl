// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_MEMORYSERVER_H

#include "axl_log_LineRepresentMgr.h"

namespace axl {
namespace log {

//.............................................................................

class CMemoryServer: public CServerPeer
{
	friend class CIoThread;

protected:
	CClientPeer* m_pClientPeer;
	uint64_t m_SyncId;
	TBinDataConfig m_BinDataConfig;

	TLinePool m_LinePool;
	rtl::CArrayT <uchar_t> m_LineBuffer;
	rtl::CArrayT <uchar_t> m_PageBuffer;

	const void* m_p;
	size_t m_Size;
	uint64_t m_Timestamp;

public:
	CMemoryServer ();

	void
	Create (
		CClientPeer* pClientPeer,
		const TBinDataConfig* pBinDataConfig,
		const void* p,
		size_t Size,
		uint64_t Timestamp = 0
		);

	virtual
	void
	SendMsg (const TMsg* pMsg);

protected:
	void
	ProcessRebuild ();

	void
	ProcessUpdate ();

	void
	ProcessRepresentPage (const log::TIndexLeaf* pIndexLeaf);
};

//.............................................................................

} // namespace log {
} // namespace axl {
