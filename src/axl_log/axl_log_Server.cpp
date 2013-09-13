#include "pch.h"
#include "axl_log_Server.h"

namespace axl {
namespace log {

//.............................................................................

CServer::CServer ()
{
	m_Flags = 0;

	m_pClient = NULL;
	m_pRepresenter = NULL;
	m_pColorizer = NULL;
	m_pHyperlinkHandler = NULL;

	m_IoThread.m_pServer = this;
	m_ColorizeThread.m_pServer = this;

	memset (&m_IndexPrevPacketFileHdrSnapshot, 0, sizeof (m_IndexPrevPacketFileHdrSnapshot));
	memset (&m_IndexLeaf, 0, sizeof (m_IndexLeaf));
	m_IndexLeaf.m_DefBinDataConfig.SetDefaults ();

	m_IndexLeafLineCount = 0;
	m_IndexLeafLineCountLimit = 32;
	m_IndexVolatilePacketCountLimit = 8;
}

bool
CServer::Create (
	IClient* pClient,
	IRepresenter* pRepresenter,
	const char* pPacketFilePath
	)
{
	bool Result;

	Stop ();

	Result = 
		m_PacketFile_w.Open (pPacketFilePath, axl::io::EFileFlag_ShareWrite) &&
		m_PacketFile.Open (pPacketFilePath, axl::io::EFileFlag_ShareWrite);

	if (!Result)
		return false;

	m_Flags = EFlag_Running | EFlag_Rebuild;
	m_pClient = pClient;
	m_pRepresenter = pRepresenter;
	m_IoThreadEvent.Signal ();
	
	return 
		m_IoThread.Start () &&
		m_ColorizeThread.Start ();
}

void
CServer::Stop ()
{
	if (!(m_Flags & EFlag_Running))
		return;

	m_Lock.Lock ();
	m_Flags |= EFlag_TerminateAllThreads;
	m_IoThreadEvent.Signal ();
	m_ColorizeThreadEvent.Signal ();
	m_Lock.Unlock ();

	m_IoThread.WaitAndClose ();
	m_IoThreadEvent.Reset ();

	m_ColorizeThread.WaitAndClose ();
	m_ColorizeThreadEvent.Reset ();

	m_PacketFile.Close ();
	m_PacketFile_w.Close ();
	m_FilterStack.Clear ();

	m_Flags = 0;
}

void
CServer::ProcessSrvMsg (
	ESrvMsg MsgKind,
	const void* p,
	size_t Size
	)
{
	switch (MsgKind)
	{
	case ESrvMsg_Clear:
		Clear ();
		break;

	case ESrvMsg_Rebuild:
		Rebuild ();
		break;

	case ESrvMsg_Update:
		Update ();
		break;

	case ESrvMsg_WritePacket:
		if (Size >= sizeof (TSrvMsg_WritePacket))
		{
			TSrvMsg_WritePacket* pMsg = (TSrvMsg_WritePacket*) p;

			WritePacket (
				pMsg->m_Timestamp, 
				pMsg->m_Code, 
				pMsg->m_DataSize ? pMsg + 1 : NULL, 
				pMsg->m_DataSize
				);
		}

		break;

	case ESrvMsg_Represent:
		if (Size >= sizeof (TSrvMsg_Represent))
		{
			TSrvMsg_Represent* pMsg = (TSrvMsg_Represent*) p;
			Represent (
				pMsg->m_SyncId, 
				pMsg->m_IndexNodeOffset, 
				pMsg->m_IndexNodeLineCount,
				pMsg->m_LineIdx,
				&pMsg->m_IndexLeaf,
				&pMsg->m_PrevIndexLeaf,
				(TIndexVolatilePacket*) (pMsg + 1)
				);
		}

		break;

	case ESrvMsg_Colorize:
		if (Size > sizeof (TSrvMsg_Represent))
		{
			TSrvMsg_Colorize* pMsg = (TSrvMsg_Colorize*) p;
			Colorize (
				pMsg->m_SyncId, 
				pMsg->m_IndexNodeOffset, 
				pMsg->m_LineIdx,
				pMsg->m_LineOffset,
				pMsg + 1,
				pMsg->m_DataSize
				);
		}

		break;
	}
}

void
CServer::Clear ()
{
	m_Lock.Lock ();
	m_Flags |= EFlag_Clear;
	m_RepresentMsgList.Clear (); // not relevant anymore
	m_ColorizeMsgList.Clear (); // not relevant anymore
	m_IoThreadEvent.Signal ();
	m_Lock.Unlock ();
}

void 
CServer::Rebuild ()
{
	m_Lock.Lock ();
	m_Flags |= EFlag_Rebuild;
	m_RepresentMsgList.Clear (); // not relevant anymore
	m_ColorizeMsgList.Clear (); // not relevant anymore
	m_IoThreadEvent.Signal ();
	m_Lock.Unlock ();
}

void 
CServer::SetDefBinDataConfig (const TBinDataConfig* pConfig)
{
	m_Lock.Lock ();
	if (pConfig)
		m_DefBinDataConfig = *pConfig; 
	else
		m_DefBinDataConfig.SetDefaults ();

	m_Flags |= EFlag_Update;
	m_IoThreadEvent.Signal ();
	m_Lock.Unlock ();
}

void 
CServer::Update ()
{
	m_Lock.Lock ();
	m_Flags |= EFlag_Update;
	m_IoThreadEvent.Signal ();
	m_Lock.Unlock ();
}

void 
CServer::WritePacket (
	uint64_t Timestamp,
	uint_t Code,
	const void* p,
	size_t Size
	)
{
	m_Lock.Lock ();
	m_PacketFile_w.Write (Timestamp, Code, p, Size);
	m_Flags |= EFlag_Update;
	m_IoThreadEvent.Signal ();
	m_Lock.Unlock ();
}

void 
CServer::Represent (
	size_t SyncId,
	size_t IndexNodeOffset,
	size_t IndexNodeLineCount,
	size_t LineIdx,
	const TIndexLeaf* pIndexLeaf,
	const TIndexLeaf* pPrevIndexLeaf,
	const TIndexVolatilePacket* pVolatilePacketArray
	)
{
	size_t ExtraSize = pIndexLeaf->m_VolatilePacketCount * sizeof (TIndexVolatilePacket);

	rtl::CBoxListEntryT <TSrvMsg_Represent>* pEntry;
	pEntry = AXL_MEM_NEW_EXTRA (rtl::CBoxListEntryT <TSrvMsg_Represent>, ExtraSize);
	pEntry->m_Value.m_SyncId = SyncId;
	pEntry->m_Value.m_IndexNodeOffset = IndexNodeOffset;
	pEntry->m_Value.m_IndexNodeLineCount = IndexNodeLineCount;
	pEntry->m_Value.m_LineIdx = LineIdx;
	pEntry->m_Value.m_IndexLeaf = *pIndexLeaf;

	if (pPrevIndexLeaf)
		pEntry->m_Value.m_PrevIndexLeaf = *pPrevIndexLeaf;
	else
		memset (&pEntry->m_Value.m_PrevIndexLeaf, 0, sizeof (pEntry->m_Value.m_PrevIndexLeaf));

	if (ExtraSize)
		memcpy (pEntry + 1, pVolatilePacketArray, ExtraSize);

	m_Lock.Lock ();
	m_RepresentMsgList.InsertTailEntry (pEntry);
	m_IoThreadEvent.Signal ();
	m_Lock.Unlock ();
}

void 
CServer::Colorize (
	size_t SyncId,
	size_t IndexNodeOffset,
	size_t LineIdx,
	size_t LineOffset,
	const void* p,
	size_t Size
	)
{
	rtl::CBoxListEntryT <TSrvMsg_Colorize>* pEntry;
	pEntry = AXL_MEM_NEW_EXTRA (rtl::CBoxListEntryT <TSrvMsg_Colorize>, Size);
	pEntry->m_Value.m_SyncId = SyncId;
	pEntry->m_Value.m_IndexNodeOffset = IndexNodeOffset;
	pEntry->m_Value.m_LineIdx = LineIdx;
	pEntry->m_Value.m_LineOffset = LineOffset;
	pEntry->m_Value.m_DataSize = Size;

	if (Size)
		memcpy (pEntry + 1, p, Size);

	m_Lock.Lock ();
	m_ColorizeMsgList.InsertTailEntry (pEntry);
	m_ColorizeThreadEvent.Signal ();
	m_Lock.Unlock ();
}

//.............................................................................

} // namespace log 
} // namespace axl
