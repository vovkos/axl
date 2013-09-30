#include "pch.h"
#include "axl_log_IndexFile.h"

namespace axl {
namespace log {

//.............................................................................

bool
CIndexFile::Open (
	const char* pFileName,
	uint_t Flags
	)
{
	Close ();

	bool Result = m_File.Open (pFileName, Flags);
	if (!Result)
		return false;

	m_pHdr = (TIndexFileHdr*) m_File.View (0, sizeof (TIndexFileHdr), true);
	m_pHdr->m_Signature = EIndexFile_FileSignature;
	m_pHdr->m_Version = EIndexFile_CurrentVersion;
	m_pHdr->m_NodeCount = 0;
	m_pHdr->m_LeafCount = 0;
	m_pHdr->m_TotalNodeSize = 0;
	m_pHdr->m_RootNode = -1;
	m_pHdr->m_HeadLeaf = -1;
	m_pHdr->m_TailLeaf = -1;
	m_pHdr->m_LineCount = 0;

	m_File.SetSize (sizeof (TIndexFileHdr));
	return true;
}

void
CIndexFile::Clear ()
{
	ASSERT (m_pHdr);

	m_pHdr->m_NodeCount = 0;
	m_pHdr->m_LeafCount = 0;
	m_pHdr->m_TotalNodeSize = 0;
	m_pHdr->m_RootNode = -1;
	m_pHdr->m_HeadLeaf = -1;
	m_pHdr->m_TailLeaf = -1;
	m_pHdr->m_LineCount = 0;

	m_File.SetSize (sizeof (TIndexFileHdr));
}

const TIndexLeaf*
CIndexFile::GetLeaf (uint64_t Offset) const
{
	ASSERT (Offset <= m_pHdr->m_TailLeaf);

	uint64_t FileOffset = Offset + sizeof (TIndexFileHdr);

	const TIndexLeaf* pLeaf = (const TIndexLeaf*) m_File.View (FileOffset, sizeof (TIndexLeaf));
	ASSERT (pLeaf && pLeaf->m_Signature == EIndexFile_LeafSignature && pLeaf->m_Offset == Offset);

	if (pLeaf->m_FoldablePacketCount) // map fold flag array
	{
		size_t LeafSize = sizeof (TIndexLeaf) + (size_t) pLeaf->m_FoldablePacketCount * sizeof (uint64_t);
		pLeaf = (const TIndexLeaf*) m_File.View (FileOffset, LeafSize);
	}
	
	return pLeaf;
}

TIndexNode*
CIndexFile::GetNode (uint64_t Offset) const
{
	ASSERT (Offset <= m_pHdr->m_TailLeaf);

	uint64_t FileOffset = Offset + sizeof (TIndexFileHdr);

	TIndexNode* pNode = (TIndexNode*) m_File.View (FileOffset, sizeof (TIndexNode));
	ASSERT (
		pNode && 
		(pNode->m_Signature == EIndexFile_NodeSignature || pNode->m_Signature == EIndexFile_LeafSignature) && 
		pNode->m_Offset == Offset
		);

	return pNode;
}

TIndexNode* 
CIndexFile::GetRootNode () const
{
	if (m_pHdr->m_RootNode == -1)
		return NULL;

	TIndexNode* pRoot = GetNode (m_pHdr->m_RootNode);
	ASSERT (pRoot->m_Parent == -1 && pRoot->m_LineCount == m_pHdr->m_LineCount);

	return pRoot;
}

TIndexNode* 
CIndexFile::GetParentNode (const TIndexNode* pNode) const
{
	if (pNode->m_Parent == -1)
	{
		ASSERT (pNode->m_Offset == m_pHdr->m_RootNode);
		return NULL;
	}

	TIndexNode* pParent = GetNode (pNode->m_Parent);
	ASSERT (pParent->m_Left == pNode->m_Offset || pParent->m_Right == pNode->m_Offset);

	return pParent;
}

TIndexNode* 
CIndexFile::GetLeftNode (const TIndexNode* pNode) const
{
	TIndexNode* pLeft = GetNode (pNode->m_Left);
	ASSERT (pLeft->m_Parent == pNode->m_Offset);

	return pLeft;
}

TIndexNode* 
CIndexFile::GetRightNode (const TIndexNode* pNode) const
{
	TIndexNode* pRight = GetNode (pNode->m_Right);
	ASSERT (pRight->m_Parent == pNode->m_Offset);

	return pRight;
}

TIndexNode* 
CIndexFile::CreateNode (
	uint32_t Signature,
	size_t Size
	)
{	
	uint64_t Offset = m_pHdr->m_TotalNodeSize;
	uint64_t FileOffset = sizeof (TIndexFileHdr) + Offset;

	TIndexNode* pNode = (TIndexNode*) m_File.View (FileOffset, Size);
	pNode->m_Signature = Signature;
	pNode->m_Offset = Offset;
	pNode->m_Depth = 0;
	pNode->m_Parent = -1;
	pNode->m_Left = -1;
	pNode->m_Right = -1;
	pNode->m_LineCount = 0;

#ifdef _DEBUG
	pNode->m_RotationCount = 0;
#endif
	
	m_pHdr->m_TotalNodeSize += Size;
	m_pHdr->m_NodeCount++;

	m_File.SetSize (FileOffset + Size);
	return pNode;
}

TIndexLeaf* 
CIndexFile::CreateLeaf (
	const TBinDataConfig* pBinDataConfig,
	uint64_t PacketOffset,
	uint64_t MergePointIdx
	)
{
	TIndexLeaf* pLeaf = (TIndexLeaf*) CreateNode (EIndexFile_LeafSignature, sizeof (TIndexLeaf));
	pLeaf->m_BinDataConfig = *pBinDataConfig;
	pLeaf->m_PacketOffset = PacketOffset;
	pLeaf->m_PacketCount = 0;
	pLeaf->m_FoldablePacketCount = 0;
	pLeaf->m_MergePointIdx = MergePointIdx;

	m_pHdr->m_LeafCount++;
	m_pHdr->m_TailLeaf = pLeaf->m_Offset;
	return pLeaf;
}

const TIndexLeaf*
CIndexFile::AddLeaf (
	const TBinDataConfig* pBinDataConfig,
	uint64_t PacketOffset,
	uint64_t MergePointIdx
	)
{
	ASSERT (m_pHdr);

	if (m_pHdr->m_RootNode == -1)
	{
		TIndexLeaf* pLeaf = CreateLeaf (
			pBinDataConfig, 
			PacketOffset, 
			MergePointIdx
			);

		ASSERT (pLeaf->m_Offset == 0 && m_pHdr->m_TailLeaf == 0 && m_pHdr->m_NodeCount == 1);

		m_pHdr->m_RootNode = 0;
		m_pHdr->m_HeadLeaf = 0;
		return pLeaf;
	}

	TIndexNode* pTail = GetNode (m_pHdr->m_TailLeaf);
	TIndexNode* pOldParent = GetParentNode (pTail);
	TIndexNode* pParent = CreateNode ();
	TIndexLeaf* pLeaf = CreateLeaf (pBinDataConfig, PacketOffset, MergePointIdx);

	pParent->m_Depth     = 1;
	pParent->m_Parent    = pTail->m_Parent; 
	pParent->m_Left      = pTail->m_Offset;
	pParent->m_Right     = pLeaf->m_Offset;
	pParent->m_LineCount = pTail->m_LineCount;

	pTail->m_Parent = pParent->m_Offset;
	pTail->m_Right  = pLeaf->m_Offset;

	pLeaf->m_Parent    = pParent->m_Offset;
	pLeaf->m_Left      = pTail->m_Offset;

	TIndexNode* pNode = pOldParent;
	if (!pNode)
	{
		ASSERT (m_pHdr->m_NodeCount == 3 && m_pHdr->m_RootNode == pTail->m_Offset); 
		m_pHdr->m_RootNode = pParent->m_Offset; // fixup root -- must do this only once during tree grow
		return pLeaf;
	}

	ASSERT (pNode->m_Right == pTail->m_Offset);
	pNode->m_Right = pParent->m_Offset;

	// go up to the root

	for (; pNode; pNode = GetParentNode (pNode))
	{
		TIndexNode* pL = GetNode (pNode->m_Left);
		TIndexNode* pR = GetNode (pNode->m_Right);
		ASSERT (pR->m_Depth);

		TIndexNode* pRL = GetNode (pR->m_Left);
		TIndexNode* pRR = GetNode (pR->m_Right);

		uint32_t NewLeftDepth = AXL_MAX (pL->m_Depth, pRL->m_Depth) + 1;
		uint32_t NewDepth = AXL_MAX (NewLeftDepth, pRR->m_Depth) + 1;

		pNode->m_Depth = AXL_MAX (pL->m_Depth, pR->m_Depth) + 1;

		// check if we could improve tree balance by rotation:
		//
		//    N             N
		//   / \           / \
		//  L   R   ==>   R   RR
		//     / \       / \
		//    RL  RR    L   RL
		//

		if (NewDepth < pNode->m_Depth)
		{
			pNode->m_Left = pR->m_Offset;
			pNode->m_Right = pRR->m_Offset;
			pR->m_Left = pL->m_Offset;
			pR->m_Right = pRL->m_Offset;

			pL->m_Parent = pR->m_Offset;
			pRL->m_Parent = pR->m_Offset;
			pRR->m_Parent = pNode->m_Offset;

			pR->m_LineCount = pL->m_LineCount + pRL->m_LineCount;
			pR->m_Depth = NewLeftDepth; 

			pNode->m_LineCount = pRR->m_LineCount + pR->m_LineCount;
			pNode->m_Depth = NewDepth;

#ifdef _DEBUG
			pNode->m_RotationCount++;
#endif
		}
	}

	// cannot return pLeaf here (might have been unmapped during walking up)

	return GetTailLeaf ();
}

uint64_t
CIndexFile::GetLeafStartLine (const TIndexLeaf* pLeaf) const
{
	uint64_t Offset = pLeaf->m_Offset;

	const TIndexNode* pNode = pLeaf;

	TIndexNode* pParent = GetParentNode (pNode);
	uint64_t StartLineIdx = 0;

	while (pParent)
	{
		if (pNode->m_Offset != pParent->m_Left)
		{
			TIndexNode* pLeft = GetLeftNode (pParent);
			StartLineIdx += pLeft->m_LineCount;
		}

		pNode = pParent;
		pParent = GetParentNode (pNode);
	}

	GetLeaf (Offset); // make sure the leaf is still mapped
	return StartLineIdx;
}

const TIndexLeaf* 
CIndexFile::FindLeaf (
	uint64_t LineIdx,
	uint64_t* pStartLineIdx
	) const
{
	ASSERT (m_pHdr);

	if (LineIdx >= m_pHdr->m_LineCount)
		return NULL;

	TIndexNode* pNode = GetRootNode ();
	uint64_t StartLineIdx = 0;

	while (pNode->m_Depth)
	{
		TIndexNode* pLeft = GetLeftNode (pNode);

		ASSERT (pLeft != NULL);
		ASSERT (LineIdx < pNode->m_LineCount);

		if (LineIdx < pLeft->m_LineCount)
		{
			pNode = pLeft;
		}
		else
		{
			pNode = GetRightNode (pNode);
			LineIdx -= pLeft->m_LineCount;
			StartLineIdx += pLeft->m_LineCount;
		}
	}

	if (pStartLineIdx)
		*pStartLineIdx = StartLineIdx;

	return GetLeaf (pNode->m_Offset);
}

void
CIndexFile::AddLeafLines (
	const TIndexLeaf* pLeaf,
	intptr_t LineCount
	)
{
	if (LineCount == 0)
		return;

	uint64_t Offset = pLeaf->m_Offset;

	TIndexNode* pNode = (TIndexNode*) pLeaf;
	pNode->m_LineCount += LineCount;

	pNode = GetParentNode (pNode);
	for (; pNode; pNode = GetParentNode (pNode))
		pNode->m_LineCount += LineCount;

	m_pHdr->m_LineCount += LineCount;

	GetLeaf (Offset); // make sure the leaf is still mapped
}

void
CIndexFile::AddTailLeafPackets (
	size_t PacketCount,
	size_t FoldablePacketCount,
	uint64_t FoldFlags
	)
{
	TIndexLeaf* pLeaf = (TIndexLeaf*) GetTailLeaf ();

	pLeaf->m_PacketCount += PacketCount;
	
	if (!FoldablePacketCount)
		return;

	uint64_t FileOffset = 
		sizeof (TIndexFileHdr) + pLeaf->m_Offset + 
		sizeof (TIndexLeaf) + pLeaf->m_FoldablePacketCount * sizeof (uint64_t);

	size_t Size = FoldablePacketCount * sizeof (uint64_t);
	
	void* p = m_File.View (FileOffset, Size);
	if (FoldFlags == 0)
	{
		memset (p, 0, Size);
	}
	else
	{
		uint64_t* pFlags = (uint64_t*) p;
		uint64_t* pEnd = pFlags + FoldablePacketCount;

		for (; pFlags < pEnd; pFlags++)
			*pFlags = FoldFlags;
	}
		
	pLeaf->m_FoldablePacketCount += FoldablePacketCount;
	m_pHdr->m_TotalNodeSize += Size;

	m_File.SetSize (FileOffset + Size);
}

//.............................................................................

} // namespace log 
} // namespace axl
