#include "pch.h"
#include "axl_log_IndexMgr.h"
#include "axl_log_IndexRepresenterTarget.h"

namespace axl {
namespace log {

//.............................................................................

CIndexMgr::CIndexMgr ()
{
	m_pClient = NULL;
	m_pRepresenter = NULL;
	m_pPacketFile = NULL;
	m_pMergeFile = NULL;

	m_LeafLineCountLimit = 4;
	m_LeafFoldablePacketCountLimit = 8;

	m_LeafOffset = 0;
	m_IsLeafCreated = false;
	m_FoldFlags = 0;

	m_PacketOffset = 0;
	m_PacketCount = 0;
	m_FoldablePacketCount = 0;
	m_LineCount = 0;
	m_Col = 0;
	m_PartIdx = 0;
	m_BinOffset = 0;	

	m_PrevLeafPacketOffset = 0;
	m_PrevLeafPacketCount = 0;
}

void
CIndexMgr::Setup (
	CClient* pClient,
	CRepresenter* pRepresenter,
	CPacketFile* pPacketFile,
	CMergeFile* pMergeFile
	)
{
	m_pClient = pClient;
	m_pRepresenter = pRepresenter;
	m_pPacketFile = pPacketFile;
	m_pMergeFile = pMergeFile;

	ClearIndex ();
}

void 
CIndexMgr::SetBinDataConfig (const TBinDataConfig* pBinDataConfig)
{
	if (m_BinDataConfig.Cmp (m_BinDataConfig) == 0)
		return;

	m_BinDataConfig = *pBinDataConfig;

	if (m_IsLeafCreated || m_LineCount != 0)
		FinalizeLeaf (
			m_PacketOffset, 
			m_PacketCount, 
			m_FoldablePacketCount, 
			m_LineCount
			);
}

void
CIndexMgr::ClearIndex ()
{
	m_LeafOffset = 0;
	m_IsLeafCreated = false;

	m_PacketOffset = 0;
	m_PacketCount = 0;
	m_FoldablePacketCount = 0;
	m_LineCount = 0;
	m_Col = 0;
	m_PartIdx = 0;
	m_BinOffset = 0;
	m_MergeCriteria.Clear ();

	m_PrevLeafPacketOffset = 0;
	m_PrevLeafPacketCount = 0;
}

void
CIndexMgr::UpdateIndex (uint64_t TotalPacketSize)
{
	bool IsNewLeaf = false;

	uint64_t PacketOffset = m_PacketOffset;

	ASSERT (m_pRepresenter);

	CIndexRepresenterTarget RepresenterTarget;
	RepresenterTarget.m_BinDataConfig = m_BinDataConfig;
	RepresenterTarget.m_LineCount = m_LineCount;

	if (!m_MergeCriteria.IsEmpty ())
	{
		RepresenterTarget.m_Col = m_Col;
		RepresenterTarget.m_PartIdx = m_PartIdx;
		RepresenterTarget.m_BinOffset = m_BinOffset;
		RepresenterTarget.m_MergeCriteria = m_MergeCriteria;
	}

	size_t PacketCount = m_PacketCount;
	size_t FoldablePacketCount = m_FoldablePacketCount;

	while (PacketOffset < TotalPacketSize)
	{
		const TPacket* pPacket = m_pPacketFile->GetPacket (PacketOffset);

		RepresenterTarget.StartPacket (pPacket);

		if (pPacket->m_Code & EPacketCodeFlag_Mergeable)
		{
			uint64_t PrevPacketOffset = PacketOffset;
			size_t PrevLineCount = RepresenterTarget.m_LineCount;
			size_t PrevCol = RepresenterTarget.m_Col;
			uint64_t PrevPartIdx = RepresenterTarget.m_PartIdx;
			uint64_t PrevBinOffset = RepresenterTarget.m_BinOffset;
			TMergeCriteria PrevMergeCriteria = RepresenterTarget.m_MergeCriteria;

			m_pRepresenter->Represent (
				&RepresenterTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				(size_t) pPacket->m_DataSize,
				0
				);

			PacketOffset += sizeof (TPacket) + pPacket->m_DataSize;
			PacketCount++;

			if (PrevLineCount >= m_LeafLineCountLimit && 
				RepresenterTarget.m_LineCount > PrevLineCount)
			{
				FinalizeLeaf (
					PrevPacketOffset,
					PacketCount,
					FoldablePacketCount,
					PrevLineCount
					);

				if (RepresenterTarget.m_IsFirstPartMerged)
				{					
					m_Col = PrevCol;
					m_PartIdx = PrevPartIdx; 
					m_BinOffset = PrevBinOffset;
					m_MergeCriteria = PrevMergeCriteria;
				}
			
				RepresenterTarget.m_LineCount -= PrevLineCount;
				PacketCount = 1;
				FoldablePacketCount = 0;
			}
		}
		else if (pPacket->m_Code & EPacketCodeFlag_Foldable)
		{	
			size_t OldLineCount = RepresenterTarget.m_LineCount;

			RepresenterTarget.m_MergeCriteria.Break ();

			m_pRepresenter->Represent (
				&RepresenterTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				(size_t) pPacket->m_DataSize,
				m_FoldFlags
				);

			RepresenterTarget.m_MergeCriteria.Break ();

			PacketOffset += sizeof (TPacket) + pPacket->m_DataSize;
			PacketCount++;
			FoldablePacketCount++;
		
			if (RepresenterTarget.m_LineCount >= m_LeafLineCountLimit || 
				FoldablePacketCount >= m_LeafFoldablePacketCountLimit)
			{
				FinalizeLeaf (
					PacketOffset,
					PacketCount,
					FoldablePacketCount,
					RepresenterTarget.m_LineCount
					);

				RepresenterTarget.m_LineCount = 0;
				PacketCount = 0;
				FoldablePacketCount = 0;
			}
		}
		else // normal: non-mergeable and non-volatile
		{	
			RepresenterTarget.m_MergeCriteria.Break ();

			m_pRepresenter->Represent (
				&RepresenterTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				(size_t) pPacket->m_DataSize,
				0
				);

			RepresenterTarget.m_MergeCriteria.Break ();

			PacketOffset += sizeof (TPacket) + pPacket->m_DataSize;
			PacketCount++;
		
			if (RepresenterTarget.m_LineCount >= m_LeafLineCountLimit)
			{
				FinalizeLeaf (
					PacketOffset,
					PacketCount,
					FoldablePacketCount,
					RepresenterTarget.m_LineCount
					);
			
				RepresenterTarget.m_LineCount = 0;
				PacketCount = 0;
				FoldablePacketCount = 0;
			}
		}
	}

	UpdateLeaf (
		PacketCount,
		FoldablePacketCount,
		RepresenterTarget.m_LineCount
		);

	m_PacketOffset = PacketOffset;
	m_PacketCount = PacketCount;
	m_FoldablePacketCount = FoldablePacketCount;
	m_LineCount = RepresenterTarget.m_LineCount;

	if (!RepresenterTarget.m_MergeCriteria.IsEmpty ())
	{
		m_Col = RepresenterTarget.m_Col;
		m_PartIdx = RepresenterTarget.m_PartIdx;
		m_BinOffset = RepresenterTarget.m_BinOffset;
		m_MergeCriteria = RepresenterTarget.m_MergeCriteria;
	}
}

void
CIndexMgr::UpdateLeaf (
	size_t PacketCount,
	size_t FoldablePacketCount,
	size_t LineCount
	)
{
	if (!m_IsLeafCreated)
	{
		uint64_t MergePointIdx;

		if (m_MergeCriteria.IsEmpty ())
		{
			MergePointIdx = -1;
		}
		else
		{
			ASSERT (m_PrevLeafPacketCount);

			MergePointIdx = m_pMergeFile->GetHdr ()->m_MergePointCount;
			m_pMergeFile->AddMergePoint (
				m_MergeCriteria.m_LineKind,
				m_Col,
				m_PartIdx,
				m_BinOffset,
				m_PrevLeafPacketOffset,   // TODO: calculate proper offset to optimize colorizer state file update
				m_PrevLeafPacketCount - 1 // don't include merging packet
				);
		}

		m_pClient->CreateIndexLeaf (
			m_LeafOffset, 
			&m_BinDataConfig,
			LineCount,
			m_PacketOffset,
			PacketCount,
			FoldablePacketCount,
			MergePointIdx,
			m_FoldFlags
			);

		m_PrevLeafPacketOffset = m_PacketOffset;
		m_PrevLeafPacketCount = PacketCount;
		m_IsLeafCreated = true;
	}
	else
	{
		m_pClient->UpdateIndexTailLeaf (
			m_LeafOffset, 
			LineCount,
			PacketCount,
			FoldablePacketCount,
			m_FoldFlags
			);

		m_PrevLeafPacketCount = PacketCount;
	}	
}

void
CIndexMgr::FinalizeLeaf (
	uint64_t PacketOffset,
	size_t PacketCount,
	size_t FoldablePacketCount,
	size_t LineCount
	)
{
	UpdateLeaf (PacketCount, FoldablePacketCount, LineCount);

	size_t CurrentLeafSize = sizeof (TIndexLeaf) + FoldablePacketCount * sizeof (uint64_t);

	m_LeafOffset += CurrentLeafSize + sizeof (TIndexNode);
	m_IsLeafCreated = false;

	m_PacketOffset = PacketOffset;
	m_Col = 0;
	m_PartIdx = 0;
	m_BinOffset = 0;
	m_MergeCriteria.Clear ();
}

//.............................................................................

} // namespace log 
} // namespace axl
