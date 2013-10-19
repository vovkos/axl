// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_SERVER_H

#include "axl_log_Protocol.h"
#include "axl_log_PacketFile.h"
#include "axl_log_Filter.h"
#include "axl_log_Representer.h"
#include "axl_log_Colorizer.h"
#include "axl_log_HyperlinkHandler.h"
#include "axl_log_CachePage.h"
#include "axl_log_FilterMgr.h"
#include "axl_log_IndexMgr.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_log_LineRepresentMgr.h"
#include "axl_mt_Lock.h"
#include "axl_mt_Thread.h"
#include "axl_mt_Event.h"
#include "axl_rtl_BoxList.h"

namespace axl {
namespace log {

struct TIndexRepresenterTargetData;

//.............................................................................

class CServer: public CServerPeer
{
	friend class CIoThread;

protected:
	enum EFlag
	{
		EFlag_Running   = 0x0001,
		EFlag_Terminate = 0x0002,
		EFlag_Update    = 0x0010,
		EFlag_Rebuild   = 0x0020, 
		EFlag_Clear     = 0x0040,
	};

	class CIoThread: public mt::CThreadImplT <CIoThread>
	{
	public:
		void
		ThreadProc ()
		{	
			AXL_CONTAINING_RECORD (this, CServer, m_IoThread)->IoThreadProc ();
		}
	};
	
public:
	int m_x;

protected:
	mt::CLock m_Lock;
	uint_t m_Flags;

	CClientPeer* m_pClientPeer;
	CRepresenter* m_pRepresenter;
	CColorizer* m_pColorizer;
	CHyperlinkHandler* m_pHyperlinkHandler;

	CPacketFile m_PacketFile;   // accessed from io thread only
	CPacketFile m_PacketFile_w; // only for ESrvMsg_WritePacket
	CMergeFile m_MergeFile; // shared between IndexMgr, ColorizeMgr & LineRepresentMgr
	io::CMappedFile m_ColorizerStateFile; // shared between ColorizeMgr & LineRepresentMgr

	CFilterMgr m_FilterMgr;
	CIndexMgr m_IndexMgr;
	CColorizeMgr m_ColorizeMgr;
	CLineRepresentMgr m_LineRepresentMgr;

	TBinDataConfig m_BinDataConfig;

	// io thread

	CIoThread m_IoThread;
	mt::CEvent m_IoThreadEvent;
	rtl::CStdListT <rtl::TListLink> m_IoThreadMsgList;
	
public:
	CServer ();

	~CServer ()
	{
		Stop ();
	}

	bool
	Create (
		CClientPeer* pClientPeer,
		CRepresenter* pRepresenter,
		CColorizer* pColorizer,
		const char* pPacketFilePath,
		const char* pMergeFilePath,
		const char* pColorFilePath
		);

	void
	Stop ();

	virtual
	void
	SendMsg (const TMsg* pMsg);

protected:
	void
	IoThreadProc ();
};

//.............................................................................

} // namespace log {
} // namespace axl {
