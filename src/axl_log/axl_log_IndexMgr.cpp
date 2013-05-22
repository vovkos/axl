#include "pch.h"
#include "axl_log_IndexMgr.h"

namespace axl {
namespace log {

//.............................................................................

CIndexMgr::CIndexMgr ()
{
	// add index node every 32 lines (on average, because packets are never split between nodes)
	// also, add index node every 32 volatile packets (whichever comes first)
	
	m_LineCountLimit = 32;
	m_VolatilePacketCountLimit = 32;

	m_pPacketFile = NULL;
	m_pRepresentor = NULL;
	m_pVolatilePacketFilter = NULL;
}

bool
CIndexMgr::Create (
	CPacketFile* pPacketFile,
	IRepresentor* pRepresentor,
	const char* pIndexFileName,
	uint_t OpenFlags
	)
{
	Close ();

	bool Result = m_IndexFile.Open (pIndexFileName, OpenFlags);
	if (!Result)
		return false;

	AddIndexRootNode ();

	m_pPacketFile = pPacketFile;
	m_pRepresentor = pRepresentor;
	return true;
}

void
CIndexMgr::Close ()
{
	m_IndexFile.Close ();
	m_pPacketFile = NULL;
	m_pRepresentor = NULL;
	m_pVolatilePacketFilter = NULL;
}

void
CIndexMgr::SetRepresentor (IRepresentor* pRepresentor)
{
	if (pRepresentor == m_pRepresentor)
		return;

	m_pRepresentor = pRepresentor;

	ClearIndex ();
	UpdateIndex (true);
}

void
CIndexMgr::SetVolatilePacketFilter (IVolatilePacketFilter* pFilter)
{
	if (pFilter == m_pVolatilePacketFilter)
		return;

	m_pVolatilePacketFilter = pFilter;
	FilterVolatilePackets ();
}

void
CIndexMgr::SetDefBinDataConfig (const TBinDataConfig& Config)
{
	TIndexNode* pNode = m_IndexFile.GetTailLeaf ();
	TIndexLeafData* pNodeData = (TIndexLeafData*) (pNode + 1);

	m_DefBinDataConfig = Config;

	if (!pNode || Config.Cmp (pNodeData->m_DefBinDataConfig) == 0)
		return;

	if (pNode->m_LineCount == 0) // node is empty! can simply change data config
		pNodeData->m_DefBinDataConfig = Config;
	else
		AddIndexNode ();
}


TIndexNode*
CIndexMgr::AddIndexRootNode ()
{
	ASSERT (m_IndexFile.IsEmpty ());

	TIndexNode* pNode = m_IndexFile.AddLeaf ();
	TIndexLeafData* pNodeData = (TIndexLeafData*) (pNode + 1);
	pNodeData->m_DefBinDataConfig = m_DefBinDataConfig;
	return pNode;
}

TIndexNode*
CIndexMgr::AddIndexNode ()
{
	ASSERT (!m_IndexFile.IsEmpty ());

	TIndexFileHdr* pIndexFileHdr = m_IndexFile.GetHdr ();
	TIndexNode* pLastNode = m_IndexFile.GetTailLeaf ();
	TIndexLeafData LastNodeData = *(TIndexLeafData*) (pLastNode + 1);

	TIndexNode* pNewNode = m_IndexFile.AddLeaf ();
	TIndexLeafData* pNewNodeData = (TIndexLeafData*) (pNewNode + 1);

	pNewNodeData->m_PacketOffset = pIndexFileHdr->m_TotalPacketSize;
	pNewNodeData->m_Col = LastNodeData.m_Col;
	pNewNodeData->m_BinOffset = LastNodeData.m_BinOffset;
	pNewNodeData->m_PartIndex = LastNodeData.m_PartIndex;
	pNewNodeData->m_MergeId = LastNodeData.m_MergeId;
	pNewNodeData->m_MergeCriteria = LastNodeData.m_MergeCriteria;
	pNewNodeData->m_DefBinDataConfig = m_DefBinDataConfig;

	return pNewNode;
}

void
CIndexMgr::FinalizeIndexNode (
	TIndexNode* pNode,
	const TIndexRepresentorTargetData& RepresentorTargetData,
	size_t PacketCount
	)
{
	TIndexLeafData* pNodeData = (TIndexLeafData*) (pNode + 1);
	
	size_t VolatilePacketCount = m_VolatilePacketDataBuffer.GetCount ();

	pNodeData->m_Col = (uint32_t) RepresentorTargetData.m_Col;
	pNodeData->m_BinOffset = (uint32_t) RepresentorTargetData.m_BinOffset;
	pNodeData->m_PartIndex = (uint32_t) RepresentorTargetData.m_PartIndex;
	pNodeData->m_MergeId = (uint32_t) RepresentorTargetData.m_MergeId;
	pNodeData->m_MergeCriteria = RepresentorTargetData.m_MergeCriteria;
	pNodeData->m_VolatilePacketCount += (uint32_t) VolatilePacketCount;
	pNodeData->m_PacketCount += (uint32_t) PacketCount;

	if (VolatilePacketCount)
		m_IndexFile.AddTailLeafData (
			m_VolatilePacketDataBuffer, 
			VolatilePacketCount * sizeof (TVolatilePacketData)
			);

	if (RepresentorTargetData.m_LineCount > pNode->m_LineCount)
		m_IndexFile.AddLeafLines (pNode, RepresentorTargetData.m_LineCount - pNode->m_LineCount);

	m_VolatilePacketDataBuffer.Clear ();
}

size_t
CIndexMgr::IncrementalUpdateIndex ()
{
	TPacketFileHdr* pPacketFileHdr = m_pPacketFile->GetHdr ();
	TIndexFileHdr* pIndexFileHdr = m_IndexFile.GetHdr ();
	TIndexNode* pNode = m_IndexFile.GetTailLeaf ();
	TIndexLeafData* pNodeData = (TIndexLeafData*) (pNode + 1);
	
	size_t Offset = pIndexFileHdr->m_TotalPacketSize;
	size_t PacketEnd = pPacketFileHdr->m_TotalPacketSize;

	size_t Anchor = pIndexFileHdr->m_TotalPacketSize;
	size_t PacketCount = 0;

	ASSERT (m_pRepresentor);
	ASSERT (pIndexFileHdr->m_ClearCount == pPacketFileHdr->m_ClearCount);

	CIndexRepresentorTarget RepresentorTarget;
	RepresentorTarget.m_LineCount = pNode->m_LineCount;
	RepresentorTarget.m_Col = pNodeData->m_Col;
	RepresentorTarget.m_BinOffset = pNodeData->m_BinOffset;
	RepresentorTarget.m_PartIndex = pNodeData->m_PartIndex;
	RepresentorTarget.m_MergeId = pNodeData->m_MergeId;
	RepresentorTarget.m_MergeCriteria = pNodeData->m_MergeCriteria;

	TBinDataConfig DefBinDataConfig = pNodeData->m_DefBinDataConfig;

	m_VolatilePacketDataBuffer.Clear ();

	while (Offset < PacketEnd)
	{
		TPacket* pPacket = m_pPacketFile->GetPacket (Offset);

		RepresentorTarget.m_PacketCode = pPacket->m_Code;
		RepresentorTarget.m_Timestamp = pPacket->m_Timestamp;
		RepresentorTarget.m_BinDataConfig = DefBinDataConfig;

		if (pPacket->m_Code & EPacketCodeFlag_Mergeable)
		{
			TIndexRepresentorTargetData RepresentorTargetSnapshot = RepresentorTarget;

			m_pRepresentor->Represent (
				&RepresentorTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				pPacket->m_DataSize,
				0
				);

			// first add node (if needed)...

			if (RepresentorTargetSnapshot.m_LineCount >= m_LineCountLimit && 
				RepresentorTarget.m_LineCount > RepresentorTargetSnapshot.m_LineCount)
			{
				FinalizeIndexNode (pNode, RepresentorTargetSnapshot, PacketCount);

				pNode = AddIndexNode ();
				pNodeData = (TIndexLeafData*) (pNode + 1);

				RepresentorTarget.m_LineCount -= RepresentorTargetSnapshot.m_LineCount;
				PacketCount = 0;
			}

			// ...then advance

			PacketCount++;

			pIndexFileHdr->m_LastPacketOffset = (uint32_t) Offset;

			Offset += sizeof (TPacket) + pPacket->m_DataSize;

			pIndexFileHdr->m_TotalPacketSize = (uint32_t) Offset;
			pIndexFileHdr->m_PacketCount++;
		}
		else if (pPacket->m_Code & EPacketCodeFlag_Volatile)
		{	
			uint_t VolatileFlags = 0;
			size_t OldLineCount = RepresentorTarget.m_LineCount;

			if (m_pVolatilePacketFilter)
				m_pVolatilePacketFilter->Filter (
					pPacket->m_Code, 
					pPacket + 1, 
					pPacket->m_DataSize, 
					&VolatileFlags
					);

			RepresentorTarget.m_MergeCriteria.Break ();

			m_pRepresentor->Represent (
				&RepresentorTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				pPacket->m_DataSize,
				VolatileFlags
				);

			TVolatilePacketData VolatilePacketData;
			VolatilePacketData.m_VolatileFlags = VolatileFlags;
			VolatilePacketData.m_Offset = (uint32_t) Offset;
			VolatilePacketData.m_LineCount = (uint32_t) (RepresentorTarget.m_LineCount - OldLineCount);
			m_VolatilePacketDataBuffer.Append (VolatilePacketData);

			RepresentorTarget.m_MergeCriteria.Break ();

			// first advance...

			PacketCount++;

			pIndexFileHdr->m_LastPacketOffset = (uint32_t) Offset;

			Offset += sizeof (TPacket) + pPacket->m_DataSize;

			pIndexFileHdr->m_TotalPacketSize = (uint32_t) Offset;
			pIndexFileHdr->m_PacketCount++;

			size_t VolatilePacketCount = m_VolatilePacketDataBuffer.GetCount ();

			// then add new empty node
			
			if (RepresentorTarget.m_LineCount >= m_LineCountLimit || 
				VolatilePacketCount >= m_VolatilePacketCountLimit)
			{
				FinalizeIndexNode (pNode, RepresentorTarget, PacketCount);

				pNode = AddIndexNode ();
				pNodeData = (TIndexLeafData*) (pNode + 1);

				RepresentorTarget.m_LineCount = 0;
				PacketCount = 0;
			}
		}
		else // normal: non-mergeable and non-volatile
		{	
			RepresentorTarget.m_MergeCriteria.Break ();

			m_pRepresentor->Represent (
				&RepresentorTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				pPacket->m_DataSize,
				0
				);

			RepresentorTarget.m_MergeCriteria.Break ();

			// first advance...

			PacketCount++;

			pIndexFileHdr->m_LastPacketOffset = (uint32_t) Offset;

			Offset += sizeof (TPacket) + pPacket->m_DataSize;

			pIndexFileHdr->m_TotalPacketSize = (uint32_t) Offset;
			pIndexFileHdr->m_PacketCount++;

			// then add new empty node
			
			if (RepresentorTarget.m_LineCount >= m_LineCountLimit)
			{
				FinalizeIndexNode (pNode, RepresentorTarget, PacketCount);

				pNode = AddIndexNode ();
				pNodeData = (TIndexLeafData*) (pNode + 1);

				RepresentorTarget.m_LineCount = 0;
				PacketCount = 0;
			}
		}
	}

	FinalizeIndexNode (pNode, RepresentorTarget, PacketCount);

	return Anchor;
}

size_t
CIndexMgr::UpdateIndex (bool ForceClear)
{
	TPacketFileHdr* pPacketFileHdr = m_pPacketFile->GetHdr ();
	TIndexFileHdr* pIndexFileHdr = m_IndexFile.GetHdr ();

	if (pIndexFileHdr->m_ClearCount != pPacketFileHdr->m_ClearCount) 
	{
		pIndexFileHdr->m_ClearCount = pPacketFileHdr->m_ClearCount;
		ForceClear = true;
	} 

	if (ForceClear)
		ClearIndex ();
	
	return IncrementalUpdateIndex ();
}

size_t
CIndexMgr::FilterVolatilePackets ()
{
	if (!m_pVolatilePacketFilter)
		return 0;

	size_t ChangeCount = 0;

	TIndexNode* pNode = m_IndexFile.GetHeadLeaf ();

	for (; pNode; pNode = m_IndexFile.GetRightNode (pNode))
	{
		intptr_t NodeLineDelta = 0;

		TIndexLeafData* pNodeData = (TIndexLeafData*) (pNode + 1);
		TVolatilePacketData* pVolatilePacketData = (TVolatilePacketData*) (pNodeData + 1);
		TVolatilePacketData* pVolatilePacketDataEnd = pVolatilePacketData + pNodeData->m_VolatilePacketCount;

		ASSERT (!pNode->m_Depth); // must be a leaf

		for (; pVolatilePacketData < pVolatilePacketDataEnd; pVolatilePacketData++)
		{
			uint_t VolatileFlags = pVolatilePacketData->m_VolatileFlags;

			TPacket* pPacket = m_pPacketFile->GetPacket (pVolatilePacketData->m_Offset);
			
			if (pPacket->m_Code & EPacketCodeFlag_Mergeable)
			{
				ASSERT (false);
				continue;
			}
			
			m_pVolatilePacketFilter->Filter (pPacket->m_Code, pPacket + 1, pPacket->m_DataSize, &VolatileFlags);

			if (pVolatilePacketData->m_VolatileFlags != VolatileFlags)
			{
				size_t NewLineCount = CalcPacketLineCount (pPacket, pNodeData->m_DefBinDataConfig, VolatileFlags);
				NodeLineDelta += NewLineCount - pVolatilePacketData->m_LineCount;
				
				pVolatilePacketData->m_VolatileFlags = VolatileFlags;
				pVolatilePacketData->m_LineCount = (uint32_t) NewLineCount;
				ChangeCount++;
			}			
		}

		m_IndexFile.AddLeafLines (pNode, NodeLineDelta);
	}
	
	return ChangeCount;
}

size_t
CIndexMgr::CalcPacketLineCount (
	TPacket* pPacket,
	const TBinDataConfig& DefBinDataConfig,
	uint_t VolatileFlags
	)
{
	CIndexRepresentorTarget RepresentorTarget;
	RepresentorTarget.m_BinDataConfig = DefBinDataConfig;
	
	m_pRepresentor->Represent (
		&RepresentorTarget,
		pPacket->m_Code, 
		pPacket + 1,
		pPacket->m_DataSize,
		VolatileFlags
		);

	return RepresentorTarget.m_LineCount;
}

//.............................................................................

} // namespace log 
} // namespace axl
