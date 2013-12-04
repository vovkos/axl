#include "pch.h"
#include "axl_log_MemoryServer.h"
#include "axl_log_IndexRepresenterTarget.h"
#include "axl_log_LineRepresenterTarget.h"

namespace axl {
namespace log {

//.............................................................................

CMemoryServer::CMemoryServer ()
{
	m_pClientPeer = NULL;
	m_SyncId = 0;
	m_p = NULL;
	m_Size = 0;
	m_Timestamp = 0;
}

void
CMemoryServer::Create (
	CClientPeer* pClientPeer,
	const TBinDataConfig* pBinDataConfig,
	const void* p,
	size_t Size,
	uint64_t Timestamp
	)
{
	m_pClientPeer = pClientPeer;
	m_SyncId = 0;
	m_BinDataConfig = *pBinDataConfig;
	m_p = p;
	m_Size = Size;
	m_Timestamp = Timestamp;

	ProcessRebuild ();
}

void
CMemoryServer::SendMsg (const TMsg* pMsg0)
{
	switch (pMsg0->m_MsgKind)
	{
	case ESrvMsg_Clear:
	case ESrvMsg_WritePacket:
	case ESrvMsg_FoldPacket:
	case ESrvMsg_Update:
		// ignored
		break;

	case ESrvMsg_Rebuild:
		ProcessRebuild ();
		break;

	case ESrvMsg_SetBinDataConfig:
		if (pMsg0->m_MsgSize >= sizeof (TSrvMsg_SetBinDataConfig))
		{
			TSrvMsg_SetBinDataConfig* pMsg = (TSrvMsg_SetBinDataConfig*) pMsg0;
			m_BinDataConfig = pMsg->m_BinDataConfig; // wait until rebuild
		}

		break;

	case ESrvMsg_RepresentPage:
		if (pMsg0->m_MsgSize >= sizeof (TSrvMsg_RepresentPage))
		{
			TSrvMsg_RepresentPage* pMsg = (TSrvMsg_RepresentPage*) pMsg0;

			if (pMsg->m_SyncId == m_SyncId)
				ProcessRepresentPage (&pMsg->m_IndexLeaf);
		}

		break;
	}
}

void
CMemoryServer::ProcessRebuild ()
{
	m_SyncId++;

	m_pClientPeer->ClearCache (m_SyncId);

	CIndexRepresenterTarget RepresenterTarget;
	RepresenterTarget.m_BinDataConfig = m_BinDataConfig;
	RepresenterTarget.AddBin (m_p, m_Size);

	m_pClientPeer->CreateIndexLeaf (
		0,
		&m_BinDataConfig,
		RepresenterTarget.m_LineCount,
		0, 1, 0, 0, 0
		);
}

void
CMemoryServer::ProcessRepresentPage (const log::TIndexLeaf* pIndexLeaf)
{
	CLineRepresenterTarget RepresenterTarget;
	RepresenterTarget.m_BinDataConfig = m_BinDataConfig;
	RepresenterTarget.m_LineCount = (size_t) pIndexLeaf->m_LineCount;
	RepresenterTarget.m_pLinePool = &m_LinePool;
	RepresenterTarget.m_pLineBuffer = &m_LineBuffer;
	RepresenterTarget.m_pPageBuffer = &m_PageBuffer;

	RepresenterTarget.m_IsFirstLineOfPacket = true;
	RepresenterTarget.m_Timestamp = m_Timestamp;
	RepresenterTarget.AddBin (m_p, m_Size);
	RepresenterTarget.SaveCurrentLine ();

	m_pClientPeer->RepresentPageComplete (
		m_SyncId,
		0,
		0,
		(size_t) pIndexLeaf->m_LineCount,
		0, NULL, 0, // foldable packets
		m_PageBuffer,
		m_PageBuffer.GetCount ()
		);
}

//.............................................................................

} // namespace log
} // namespace axl

