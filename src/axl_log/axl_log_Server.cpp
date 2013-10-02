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
}

bool
CServer::Create (
	IClient* pClient,
	IRepresenter* pRepresenter,
	IColorizer* pColorizer,
	const char* pPacketFilePath,
	const char* pMergeFilePath,
	const char* pColorizerStateFilePath
	)
{
	bool Result;

	Stop ();

	Result = 
		m_PacketFile.Open (pPacketFilePath, axl::io::EFileFlag_ShareWrite) &&
		m_PacketFile_w.Open (pPacketFilePath, axl::io::EFileFlag_ShareWrite) &&
		m_MergeFile.Open (pMergeFilePath, io::EFileFlag_DeleteOnClose) &&
		m_ColorizerStateFile.Open (pColorizerStateFilePath, io::EFileFlag_DeleteOnClose);

	if (!Result)
		return false;
	
	m_Flags = EFlag_Running | EFlag_Rebuild;
	m_pClient = pClient;
	m_pRepresenter = pRepresenter;
	m_pColorizer = pColorizer;

	CPacketFile* pPacketFile = &m_PacketFile;

	m_FilterMgr.Setup (pPacketFile);
	m_IndexMgr.Setup (pClient, pRepresenter, pPacketFile, &m_MergeFile);
	m_ColorizeMgr.Setup (pRepresenter, pColorizer, pPacketFile, &m_MergeFile, &m_ColorizerStateFile);
	m_LineRepresentMgr.Setup (pClient, pRepresenter, pColorizer, pPacketFile, &m_MergeFile, &m_ColorizerStateFile);

	m_IoThreadEvent.Signal ();
	return m_IoThread.Start ();
}

void
CServer::Stop ()
{
	if (!(m_Flags & EFlag_Running))
		return;

	m_Lock.Lock ();
	m_IoThreadMsgList.Clear ();
	m_Flags |= EFlag_Terminate;
	m_IoThreadEvent.Signal ();
	m_Lock.Unlock ();

	m_IoThread.WaitAndClose ();
	m_IoThreadEvent.Reset ();

	m_PacketFile.Close ();
	m_PacketFile_w.Close ();

	m_Flags = 0;
}

void
CServer::SendMsg (const TMsg* pMsgHdr)
{
	switch (pMsgHdr->m_MsgKind)
	{
	case ESrvMsg_Clear:
		m_Lock.Lock ();
		m_Flags |= EFlag_Clear;
		m_IoThreadMsgList.Clear (); // not relevant anymore
		m_IoThreadEvent.Signal ();
		m_Lock.Unlock ();
		break;

	case ESrvMsg_Rebuild:
		m_Lock.Lock ();
		m_Flags |= EFlag_Rebuild;
		m_IoThreadMsgList.Clear (); // not relevant anymore
		m_IoThreadEvent.Signal ();
		m_Lock.Unlock ();
		break;

	case ESrvMsg_Update:
		m_Lock.Lock ();
		m_Flags |= EFlag_Update;
		m_IoThreadEvent.Signal ();
		m_Lock.Unlock ();
		break;

	case ESrvMsg_SetBinDataConfig:
		if (pMsgHdr->m_MsgSize >= sizeof (TSrvMsg_SetBinDataConfig))
		{
			TSrvMsg_SetBinDataConfig* pMsg = (TSrvMsg_SetBinDataConfig*) pMsgHdr;

			m_Lock.Lock ();
			m_BinDataConfig = pMsg->m_BinDataConfig; 
			m_Flags |= EFlag_Update;
			m_IoThreadEvent.Signal ();
			m_Lock.Unlock ();
		}

		break;

	case ESrvMsg_WritePacket:
		if (pMsgHdr->m_MsgSize >= sizeof (TSrvMsg_WritePacket))
		{
			TSrvMsg_WritePacket* pMsg = (TSrvMsg_WritePacket*) pMsgHdr;
			if (pMsg->m_MsgSize < sizeof (TSrvMsg_WritePacket) + pMsg->m_DataSize)
				break;

			m_Lock.Lock ();
			m_PacketFile_w.AddPacket (pMsg->m_Timestamp, pMsg->m_Code, pMsg + 1, pMsg->m_DataSize);
			m_Flags |= EFlag_Update;
			m_IoThreadEvent.Signal ();
			m_Lock.Unlock ();
		}

		break;

	case ESrvMsg_RepresentPage:
		if (pMsgHdr->m_MsgSize >= sizeof (TSrvMsg_RepresentPage))
		{
			TSrvMsg_RepresentPage* pMsg = (TSrvMsg_RepresentPage*) pMsgHdr;
			size_t FoldFlagArraySize = pMsg->m_IndexLeaf.m_FoldablePacketCount * sizeof (uint64_t);
			
			if (pMsg->m_MsgSize < sizeof (TSrvMsg_RepresentPage) + FoldFlagArraySize)
				break;

			rtl::TListLink* pEntry = AXL_MEM_NEW_EXTRA (rtl::TListLink, pMsg->m_MsgSize);
			memcpy (pEntry + 1, pMsg, pMsg->m_MsgSize);

			m_Lock.Lock ();
			m_IoThreadMsgList.InsertTail (pEntry);
			m_IoThreadEvent.Signal ();
			m_Lock.Unlock ();
		}

		break;

	case ESrvMsg_FoldPacket:
		if (pMsgHdr->m_MsgSize >= sizeof (TSrvMsg_FoldPacket))
		{
			rtl::TListLink* pEntry = AXL_MEM_NEW_EXTRA (rtl::TListLink, pMsgHdr->m_MsgSize);
			memcpy (pEntry + 1, pMsgHdr, pMsgHdr->m_MsgSize);

			m_Lock.Lock ();
			m_IoThreadMsgList.InsertTail (pEntry);
			m_IoThreadEvent.Signal ();
			m_Lock.Unlock ();
		}

		break;
	}
}

void
CServer::IoThreadProc ()
{
	uint64_t RebuildCount = 0;

	for (;;)
	{
		m_IoThreadEvent.Wait ();

		m_Lock.Lock ();

		if (m_Flags & EFlag_Terminate)
		{
			m_Lock.Unlock ();
			return;
		}

		if (m_Flags & EFlag_Clear)
			m_PacketFile.Clear ();
		else if (m_Flags & EFlag_Rebuild)
			RebuildCount++;

		// take snapshots

		uint_t FlagsSnapshot = m_Flags;
		m_Flags &= ~(EFlag_Clear | EFlag_Rebuild | EFlag_Update);

		TPacketFileHdr PacketFileHdrSnapshot = *m_PacketFile.GetHdr ();
		
		rtl::CStdListT <rtl::TListLink> MsgListSnapshot;
		MsgListSnapshot.TakeOver (&m_IoThreadMsgList);

		m_Lock.Unlock ();

		// filter & index

		uint64_t SyncId = PacketFileHdrSnapshot.m_ClearCount + RebuildCount;
		uint64_t TotalPacketSize = PacketFileHdrSnapshot.m_TotalPacketSize;

		if (FlagsSnapshot & (EFlag_Clear | EFlag_Rebuild))
		{
			m_pClient->ClearCache (SyncId);
			m_IndexMgr.ClearIndex ();
			m_ColorizeMgr.Reset ();
			m_LineRepresentMgr.ResetIncrementalRepresent ();
			m_MergeFile.Clear ();
			m_ColorizerStateFile.SetSize (0, true);
		}

		if (FlagsSnapshot & (EFlag_Clear | EFlag_Rebuild | EFlag_Update))
		{
			if (!m_FilterMgr.IsEmpty ())
			{
				m_FilterMgr.Process (SyncId, TotalPacketSize);
				CPacketFile* pPacketFile = m_FilterMgr.GetTargetPacketFile ();
				TotalPacketSize = pPacketFile->GetHdr ()->m_TotalPacketSize;
			}

			m_IndexMgr.UpdateIndex (TotalPacketSize);
		}
		else if (!m_FilterMgr.IsEmpty ())
		{
			CPacketFile* pPacketFile = m_FilterMgr.GetTargetPacketFile ();
			TotalPacketSize = pPacketFile->GetHdr ()->m_TotalPacketSize;
		}

		// update or recalc

		m_ColorizeMgr.UpdateColorizerStateFile ();

		// represent
		
		while (!MsgListSnapshot.IsEmpty ())
		{
			rtl::TListLink* pEntry = MsgListSnapshot.RemoveHead ();
			TMsg* pMsgHdr = (TMsg*) (pEntry + 1);
			switch (pMsgHdr->m_MsgKind)
			{
			case ESrvMsg_RepresentPage:
				if (pMsgHdr->m_MsgSize >= sizeof (TSrvMsg_RepresentPage))
				{
					TSrvMsg_RepresentPage* pMsg = (TSrvMsg_RepresentPage*) pMsgHdr;
					if (pMsg->m_SyncId == SyncId)
						m_LineRepresentMgr.RepresentPage (
							SyncId,
							&pMsg->m_IndexLeaf,
							(uint64_t*) (pMsg + 1)
							);
				}
				break;

			case ESrvMsg_FoldPacket:
				if (pMsgHdr->m_MsgSize >= sizeof (TSrvMsg_FoldPacket))
				{
					TSrvMsg_FoldPacket* pMsg = (TSrvMsg_FoldPacket*) pMsgHdr;
					if (pMsg->m_SyncId == SyncId)
					{
						size_t NewLineCount = m_LineRepresentMgr.RepresentFoldablePacket (
							&m_BinDataConfig,
							pMsg->m_SyncId,
							pMsg->m_IndexLeafOffset,
							pMsg->m_PacketOffset,
							pMsg->m_FoldablePacketIdx,
							pMsg->m_FirstLineIdx,
							pMsg->m_OldLineCount,
							pMsg->m_FoldFlags
							);

						m_IndexMgr.FoldPacketComplete (
							pMsg->m_IndexLeafOffset,
							pMsg->m_OldLineCount,
							NewLineCount
							);
					}
				}
				break;
			}

			AXL_MEM_DELETE (pEntry);
		}
	}
}

//.............................................................................

} // namespace log 
} // namespace axl

