#include "pch.h"
#include "axl_log_IndexFile.h"

namespace axl {
namespace log {

//.............................................................................

/*

turned out checking depth diff is not necessary
it's enough to check:
	NewDepth < Depth on ADD
	NewDepth <= Depth on REMOVE

testing showed that the more sophisticated check of depth diff 
results in worse balance than simple depth check

bool
CIndexFile::ShouldRotate (
	uint32_t OldDepthL,
	uint32_t OldDepthR,
	uint32_t NewDepthL,
	uint32_t NewDepthR
	)
{
	uint32_t OldDepth;
	uint32_t OldDepthDiff;
	uint32_t NewDepth;
	uint32_t NewDepthDiff;

	if (OldDepthL > OldDepthR)
	{
		OldDepth = OldDepthL;
		OldDepthDiff = OldDepthL - OldDepthR;
	}
	else
	{
		OldDepth = OldDepthR;
		OldDepthDiff = OldDepthR - OldDepthL;
	}

	if (NewDepthL > NewDepthR)
	{
		NewDepth = NewDepthL;
		NewDepthDiff = NewDepthL - NewDepthR;
	}
	else
	{
		NewDepth = NewDepthR;
		NewDepthDiff = NewDepthR - NewDepthL;
	}

	return NewDepth < OldDepth || NewDepthDiff < OldDepthDiff;
}

*/

//.............................................................................

bool
CIndexFile::Open (
	const char* pFileName,
	uint_t Flags
	)
{
	bool Result;

	Close ();

	Result = m_File.Open (pFileName, Flags);
	if (!Result)
		return false;

	m_pHdr = (TIndexFileHdr*) m_File.View (0, sizeof (TIndexFileHdr), true);

	if (m_pHdr->m_Signature == EIndexFile_FileSignature && 
		m_pHdr->m_Version == EIndexFile_CurrentVersion)
		return true; // existing file is OK

	m_pHdr->m_Signature = EIndexFile_FileSignature;
	m_pHdr->m_Version = EIndexFile_CurrentVersion;
	m_pHdr->m_PacketCount = 0;
	m_pHdr->m_TotalPacketSize = 0;
	m_pHdr->m_LastPacketOffset = 0;
	m_pHdr->m_ClearCount = 0;
	m_pHdr->m_NodeCount = 0;
	m_pHdr->m_RootNode = 0;
	m_pHdr->m_HeadLeaf = 0;
	m_pHdr->m_TailLeaf = 0;
	m_pHdr->m_NodeEnd = sizeof (TIndexFileHdr);

	m_File.SetSize (sizeof (TIndexFileHdr));
	return true;
}

void
CIndexFile::Clear ()
{
	ASSERT (m_pHdr);

	m_pHdr->m_PacketCount = 0;
	m_pHdr->m_TotalPacketSize = 0;
	m_pHdr->m_LastPacketOffset = 0;
	m_pHdr->m_NodeCount = 0;
	m_pHdr->m_RootNode = 0;
	m_pHdr->m_HeadLeaf = 0;
	m_pHdr->m_TailLeaf = 0;
	m_pHdr->m_NodeEnd = sizeof (TIndexFileHdr);

	// don't touch ClearCount!

	m_File.SetSize (sizeof (TIndexFileHdr));
}

TIndexNode*
CIndexFile::GetNode (size_t Offset) const
{
	if (!Offset)
		return NULL;

	ASSERT (m_pHdr);
	ASSERT (Offset >= sizeof (TIndexFileHdr) && Offset <= m_pHdr->m_TailLeaf);

	TIndexNode* pNode = (TIndexNode*) m_File.View (Offset, sizeof (TIndexNode));

	ASSERT (pNode);
	ASSERT (pNode->m_Offset == Offset);

	ASSERT (
		pNode->m_Signature == EIndexFile_LeafSignature && pNode->m_Depth == 0||
		pNode->m_Signature == EIndexFile_NodeSignature && pNode->m_Depth != 0
		);

	if (pNode->m_DataSize) // make sure mapping covers leaf data as well
		pNode = (TIndexNode*) m_File.View (Offset, sizeof (TIndexNode) + pNode->m_DataSize);

	return pNode;
}

TIndexNode*
CIndexFile::AddLeaf ()
{
	size_t Offset;
	size_t Size = sizeof (TIndexNode) + sizeof (TIndexLeafData);	

	TIndexNode* pLeaf;
	TIndexNode* pParent;
	TIndexNode* pTail;
	TIndexNode* pNode;

	ASSERT (m_pHdr);

	if (!m_pHdr->m_RootNode)
	{
		Offset = sizeof (TIndexFileHdr);
		ASSERT (Offset == m_pHdr->m_NodeEnd);

		pLeaf = (TIndexNode*) m_File.View (Offset, Size);
		memset (pLeaf, 0, Size);

		pLeaf->m_Signature = EIndexFile_LeafSignature;
		pLeaf->m_Offset = (uint32_t) Offset;
		pLeaf->m_DataSize  = sizeof (TIndexLeafData);

		m_pHdr->m_RootNode  = (uint32_t) Offset;
		m_pHdr->m_HeadLeaf  = (uint32_t) Offset;
		m_pHdr->m_TailLeaf  = (uint32_t) Offset;
		m_pHdr->m_NodeEnd   = (uint32_t) (Offset + Size);
		m_pHdr->m_NodeCount = 1;

		m_File.SetSize (m_pHdr->m_NodeEnd);
		return pLeaf;
	}

	pTail = GetTailLeaf ();
	pNode = GetParentNode (pTail);

	Offset = m_pHdr->m_NodeEnd + sizeof (TIndexNode);
	
	pParent = (TIndexNode*) m_File.View (m_pHdr->m_NodeEnd, sizeof (TIndexNode));
	pParent->m_Signature = EIndexFile_NodeSignature;
	pParent->m_Offset    = m_pHdr->m_NodeEnd;
	pParent->m_Depth     = 1;
	pParent->m_Parent    = pTail->m_Parent; 
	pParent->m_Left      = pTail->m_Offset;
	pParent->m_Right     = (uint32_t) Offset;
	pParent->m_LineCount = pTail->m_LineCount;
	pParent->m_DataSize  = 0;
	
#ifdef _DEBUG
	pParent->m_RotationCount = 0;
#endif

	pLeaf = (TIndexNode*) m_File.View (Offset, Size);
	pLeaf->m_Signature = EIndexFile_LeafSignature;
	pLeaf->m_Offset    = (uint32_t) Offset;
	pLeaf->m_Depth     = 0;
	pLeaf->m_Parent    = pParent->m_Offset;
	pLeaf->m_Left      = pTail->m_Offset;
	pLeaf->m_Right     = 0;
	pLeaf->m_LineCount = 0;
	pLeaf->m_DataSize  = sizeof (TIndexLeafData);

	pTail->m_Parent = pParent->m_Offset;
	pTail->m_Right  = (uint32_t) Offset;

	m_pHdr->m_TailLeaf  = (uint32_t) Offset;
	m_pHdr->m_NodeEnd   = (uint32_t) (Offset + Size);
	m_pHdr->m_NodeCount += 2;

	m_File.SetSize (m_pHdr->m_NodeEnd);

	if (!pNode)
	{
		ASSERT (m_pHdr->m_NodeCount == 3 && m_pHdr->m_RootNode == pTail->m_Offset); 
		m_pHdr->m_RootNode = pParent->m_Offset; // fixup root -- must do this only once during tree grow
		return pLeaf;
	}

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

	// make sure new node is still mapped

	return GetNode (Offset);
}

void
CIndexFile::CopyNode (
	TIndexNode* pDst,
	TIndexNode* pSrc
	)
{
	TIndexNode* pParent = GetParentNode (pSrc);
	TIndexNode* pLeft   = GetLeftNode (pSrc);
	TIndexNode* pRight  = GetRightNode (pSrc);

	ASSERT (pDst->m_Depth && pSrc->m_Depth); // not leaves!!

	pLeft->m_Parent = pDst->m_Offset;
	pRight->m_Parent = pDst->m_Offset;

	if (pParent)
	{
		if (pParent->m_Left == pSrc->m_Offset)
			pParent->m_Left = pDst->m_Offset;
		else if (pParent->m_Right == pSrc->m_Offset)
			pParent->m_Right = pDst->m_Offset;
		else
			ASSERT (false);
	}

	if (m_pHdr->m_RootNode == pSrc->m_Offset)
		m_pHdr->m_RootNode = pDst->m_Offset;

	pDst->m_Parent    = pSrc->m_Parent;
	pDst->m_Left      = pSrc->m_Left; 
	pDst->m_Right     = pSrc->m_Right;
	pDst->m_Depth     = pSrc->m_Depth;
	pDst->m_LineCount = pSrc->m_LineCount;
}

bool
CIndexFile::RemoveTailLeaf ()
{
	TIndexNode* pTail;
	TIndexNode* pPrev;
	TIndexNode* pParent;
	TIndexNode* pLeft;
	TIndexNode* pNode;

	size_t AdjNodeOffset;

	ASSERT (m_pHdr);

	if (!m_pHdr->m_NodeCount)
		return false;

	if (m_pHdr->m_NodeCount == 1)
	{
		Clear ();
		return true;
	}

	pTail = GetTailLeaf ();
	pPrev = GetLeftNode (pTail);
	pParent = GetParentNode (pTail);
	pLeft = GetLeftNode (pParent);
	pNode = GetParentNode (pParent);

	pPrev->m_Right  = 0;
	pLeft->m_Parent = pParent->m_Parent;

	if (pNode)
		pNode->m_Right = pLeft->m_Offset;

	// ok, we are about to delete leaf and it's adjacent node (originally, its parent)
	// however, due to rotations logical parent might be not in adjacent node
	// therefore, we need to back up contents of adjacent node
	
	AdjNodeOffset = pTail->m_Offset - sizeof (TIndexNode);
	if (pParent->m_Offset != AdjNodeOffset)
	{
		TIndexNode* pAdjNode = GetNode (AdjNodeOffset);
		CopyNode (pParent, pAdjNode);

		// pLeft & pNode are referenced later, so fix them up if needed

		if (pLeft->m_Offset == AdjNodeOffset)
			pLeft = pParent;

		if (pNode && pNode->m_Offset == AdjNodeOffset)
			pNode = pParent;
	}

	// delete 2 nodes

	m_pHdr->m_TailLeaf  = pPrev->m_Offset;
	m_pHdr->m_NodeEnd   = pPrev->m_Offset + sizeof (TIndexNode) + pPrev->m_DataSize;
	m_pHdr->m_NodeCount -= 2;

	m_File.SetSize (m_pHdr->m_NodeEnd);

	if (!pNode)
	{
		m_pHdr->m_RootNode = pLeft->m_Offset; // fixup root
		return true;
	}

	// go up to the root

	for (; pNode; pNode = GetParentNode (pNode))
	{
		TIndexNode* pL = GetNode (pNode->m_Left);
		TIndexNode* pR = GetNode (pNode->m_Right);

		pNode->m_Depth = AXL_MAX (pL->m_Depth, pR->m_Depth) + 1;
		pNode->m_LineCount = pL->m_LineCount + pR->m_LineCount;

		if (!pL->m_Depth)
			break;

		TIndexNode* pLL = GetNode (pL->m_Left);
		TIndexNode* pLR = GetNode (pL->m_Right);

		uint32_t NewRightDepth = AXL_MAX (pLR->m_Depth, pR->m_Depth) + 1;
		uint32_t NewDepth = AXL_MAX (NewRightDepth, pLL->m_Depth) + 1;

		// check if we could improve tree balance by rotation:
		//
		//      N           N
		//     / \         / \
		//    L   R  ==>  LL  L
		//   / \             / \
		//  LL LR           LR  R
		//

		if (NewDepth <= pNode->m_Depth) // important: on deletion we do rotate if it's equal OR event!
		{
			pNode->m_Left = pLL->m_Offset;
			pNode->m_Right = pL->m_Offset;
			pL->m_Left = pLR->m_Offset;
			pL->m_Right = pR->m_Offset;

			pLL->m_Parent = pNode->m_Offset;
			pLR->m_Parent = pL->m_Offset;
			pR->m_Parent = pL->m_Offset;

			pL->m_LineCount = pLR->m_LineCount + pR->m_LineCount;
			pL->m_Depth = NewRightDepth; 

			pNode->m_LineCount = pLL->m_LineCount + pL->m_LineCount;
			pNode->m_Depth = NewDepth;
		
#ifdef _DEBUG
			pNode->m_RotationCount++;
#endif
		}
	}

	return true;
}

size_t
CIndexFile::AddTailLeafData (
	const void* p,
	size_t DataSize
	)
{
	TIndexNode* pNode = GetTailLeaf ();

	size_t OldDataSize = pNode->m_DataSize;
	size_t NewDataSize = pNode->m_DataSize + DataSize;

	pNode->m_DataSize = (uint32_t) NewDataSize; 

	pNode = GetTailLeaf (); // get it one more time to include added data
	
	if (p)
		memcpy ((char*) (pNode + 1) + OldDataSize, p, DataSize);
	else
		memset ((char*) (pNode + 1) + OldDataSize, 0, DataSize);

	m_pHdr->m_NodeEnd = pNode->m_Offset + sizeof (TIndexNode) + pNode->m_DataSize;
	m_File.SetSize (m_pHdr->m_NodeEnd);
	
	return NewDataSize;
}

size_t
CIndexFile::RemoveTailLeafData (size_t DataSize)
{
	TIndexNode* pNode = GetTailLeaf ();

	size_t NewDataSize = pNode->m_DataSize > DataSize ? pNode->m_DataSize - DataSize : 0;

	pNode->m_DataSize = (uint32_t) NewDataSize;

	m_pHdr->m_NodeEnd = pNode->m_Offset + sizeof (TIndexNode) + pNode->m_DataSize;
	m_File.SetSize (m_pHdr->m_NodeEnd);
	
	return NewDataSize;
}

void
CIndexFile::AddLeafLines (
	TIndexNode* pNode,
	intptr_t LineCount
	)
{
	size_t Offset = pNode->m_Offset;

	ASSERT (!pNode->m_Depth);

	if (LineCount == 0)
		return;

	pNode->m_LineCount += (int32_t) LineCount;

	pNode = GetParentNode (pNode);
	for (; pNode; pNode = GetParentNode (pNode))
		pNode->m_LineCount += (int32_t) LineCount;
	
	GetNode (Offset); // make sure the node is still mapped
}

size_t
CIndexFile::GetLeafStartLine (TIndexNode* pNode) const
{
	size_t Offset = pNode->m_Offset;

	TIndexNode* pParent = GetParentNode (pNode);
	size_t StartLine = 0;

	while (pParent)
	{
		if (pNode->m_Offset != pParent->m_Left)
		{
			TIndexNode* pLeft = GetLeftNode (pParent);
			StartLine += pLeft->m_LineCount;
		}

		pNode = pParent;
		pParent = GetParentNode (pNode);
	}

	GetNode (Offset); // make sure the node is still mapped
	return StartLine;
}

TIndexNode* 
CIndexFile::FindLeafByLine (
	size_t Line,
	size_t* pStartLine
	) const
{
	ASSERT (m_pHdr);

	if (!m_pHdr->m_NodeCount)
		return NULL;

	TIndexNode* pNode = GetRootNode ();
	if (Line >= pNode->m_LineCount)
		return NULL;

	size_t StartLine = 0;
	while (pNode->m_Depth)
	{
		TIndexNode* pLeft = GetLeftNode (pNode);

		ASSERT (pLeft != NULL);
		ASSERT (Line < pNode->m_LineCount);

		if (Line < pLeft->m_LineCount)
		{
			pNode = pLeft;
		}
		else
		{
			pNode = GetRightNode (pNode);
			Line -= pLeft->m_LineCount;
			StartLine += pLeft->m_LineCount;
		}
	}

	if (pStartLine)
		*pStartLine = StartLine;

	return pNode;
}

//.............................................................................

} // namespace log 
} // namespace axl
