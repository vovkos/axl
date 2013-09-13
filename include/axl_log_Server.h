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
#include "axl_log_Page.h"
#include "axl_mt_Lock.h"
#include "axl_mt_Thread.h"
#include "axl_mt_Event.h"
#include "axl_rtl_BoxList.h"

namespace axl {
namespace log {

struct TIndexRepresenterTargetData;

//.............................................................................

class CServer: public IServer
{
	friend class CIndexThread;
	friend class CIndexThread;
	friend class CIndexThread;

protected:
	enum EFlag
	{
		EFlag_Running                  = 0x0001,
		EFlag_TerminateIoThread        = 0x0002,
		EFlag_TerminateColorizeThread  = 0x0004,
		EFlag_TerminateAllThreads      = 0x0006,
		EFlag_Update                   = 0x0010,
		EFlag_Rebuild                  = 0x0020, 
		EFlag_Clear                    = 0x0040,
	};

	class CIoThread: public mt::CThreadImplT <CIoThread>
	{
	public:
		CServer* m_pServer;

		void
		ThreadProc ()
		{
			m_pServer->IoThreadProc ();
		}
	};

	class CColorizeThread: public mt::CThreadImplT <CColorizeThread>
	{
	public:
		CServer* m_pServer;

		void
		ThreadProc ()
		{
			m_pServer->ColorizeThreadProc ();
		}
	};

	struct TFilterEntry: rtl::TListLink
	{
		IFilter* m_pFilter;
		CPacketFile m_PacketFile;
		TPacketFileHdr m_PrevPacketFileHdrSnapshot;
	};

protected:
	mt::CLock m_Lock;
	uint_t m_Flags;

	IClient* m_pClient;
	IRepresenter* m_pRepresenter;
	IColorizer* m_pColorizer;
	IHyperlinkHandler* m_pHyperlinkHandler;

	CPacketFile m_PacketFile_w; // only for ESrvMsg_WritePacket

	CIoThread m_IoThread;
	mt::CEvent m_IoThreadEvent;
	CPacketFile m_PacketFile; 
	rtl::CStdListT <TFilterEntry> m_FilterStack;
	TBinDataConfig m_DefBinDataConfig;
	TPacketFileHdr m_IndexPrevPacketFileHdrSnapshot;
	TIndexLeaf m_IndexLeaf;
	size_t m_IndexLeafLineCount;
	size_t m_IndexLeafLineCountLimit;
	size_t m_IndexVolatilePacketCountLimit;
	rtl::CArrayT <TIndexVolatilePacket> m_IndexVolatilePacketArray;
	rtl::CBoxListT <TSrvMsg_Represent> m_RepresentMsgList;
	CPage m_LastPage;
	rtl::CArrayT <uchar_t> m_LineBuffer;

	CColorizeThread m_ColorizeThread; 
	mt::CEvent m_ColorizeThreadEvent;
	rtl::CBoxListT <TSrvMsg_Colorize> m_ColorizeMsgList;
		
public:
	CServer ();

	~CServer ()
	{
		Stop ();
	}

	bool
	Create (
		IClient* pClient,
		IRepresenter* pRepresenter,
		const char* pPacketFilePath
		);

	void
	Stop ();

	void
	ProcessSrvMsg (
		ESrvMsg MsgKind,
		const void* p,
		size_t Size
		);

	virtual 
	void 
	Clear ();

	virtual 
	void 
	Rebuild ();

	virtual 
	void 
	SetDefBinDataConfig (const TBinDataConfig* pConfig);

	virtual 
	void 
	Update ();

	virtual 
	void 
	WritePacket (
		uint64_t Timestamp,
		uint_t Code,
		const void* p,
		size_t Size
		);

	virtual 
	void 
	Represent (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t IndexNodeLineCount,
		size_t LineIdx,
		const TIndexLeaf* pIndexLeaf,
		const TIndexLeaf* pPrevIndexLeaf,
		const TIndexVolatilePacket* pVolatilePacketArray
		);

	virtual 
	void 
	Colorize (
		size_t SyncId,
		size_t IndexNodeOffset,
		size_t LineIdx,
		size_t LineOffset,
		const void* p,
		size_t Size
		);

protected:
	void
	IoThreadProc ();

	void
	RepresentThreadProc ();

	void
	ColorizeThreadProc ();

	void
	FilterFile (
		TFilterEntry* pFilterEntry,
		CPacketFile* pPacketFile,
		const TPacketFileHdr* pPacketFileHdrSnapshot
		);

	void 
	UpdateIndex (
		CPacketFile* pPacketFile,
		const TPacketFileHdr* pPacketFileHdrSnapshot,
		const TBinDataConfig* pDefBinDataConfig
		);

	void
	FinalizeIndexLeaf (
		const TIndexRepresenterTargetData* pRepresenterTarget,
		size_t PacketCount,
		bool IsNewLeaf
		);

	void
	RepresentImpl (
		CPacketFile* pPacketFile,
		const TPacketFileHdr* pPacketFileHdrSnapshot,
		size_t IndexNodeOffset,
		size_t IndexNodeLineCount,
		size_t LineIdx,
		const TIndexLeaf* pIndexLeaf,
		const TIndexLeaf* pPrevIndexLeaf,
		const TIndexVolatilePacket* pVolatilePacketArray
		);
};

//.............................................................................

} // namespace log {
} // namespace axl {
