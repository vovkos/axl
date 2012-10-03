#include "axl_Precomp.h"
#include "axl_log_IndexFile.h"
#include "axl_err_Error.h"

/*

turned out checking depth diff is not necessary
it's enough to check:
	NewDepth < Depth on ADD
	NewDepth <= Depth on REMOVE

testing showed that the more sophisticated check of depth diff 
results in worse balance than simple depth check

bool_t 
AXL_API
axl_log_TIndexFile_ShouldRotate(
	ulong_t OldDepthL,
	ulong_t OldDepthR,
	ulong_t NewDepthL,
	ulong_t NewDepthR
	)
{
	ulong_t OldDepth;
	ulong_t OldDepthDiff;
	ulong_t NewDepth;
	ulong_t NewDepthDiff;

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

bool_t
AXL_API
axl_log_TIndexFile_Open(
	axl_log_TIndexFile* pFile,
	const tchar_t* pFileName,
	int Flags
	)
{
	bool_t Result;

	axl_log_TIndexFile_Close(pFile);

	Result = axl_sys_TMappedFile_Open(&pFile->m_File, pFileName, Flags);
	if (!Result)
		return false;

	pFile->m_pHdr = (axl_log_TIndexFileHdr*) axl_sys_TMappedFile_ViewEx(
		&pFile->m_File, 
		0, sizeof(axl_log_TIndexFileHdr), 
		true
		);

	if (pFile->m_pHdr->m_Signature == axl_log_EIndexFile_FileSignature && 
		pFile->m_pHdr->m_Version == axl_log_EIndexFile_CurrentVersion)
		return true; // existing file is OK

	pFile->m_pHdr->m_Signature = axl_log_EIndexFile_FileSignature;
	pFile->m_pHdr->m_Version = axl_log_EIndexFile_CurrentVersion;
	pFile->m_pHdr->m_MessageCount = 0;
	pFile->m_pHdr->m_MessageSize = 0;
	pFile->m_pHdr->m_LastMessageOffset = 0;
	pFile->m_pHdr->m_ClearCount = 0;
	pFile->m_pHdr->m_NodeCount = 0;
	pFile->m_pHdr->m_RootNode = 0;
	pFile->m_pHdr->m_HeadLeaf = 0;
	pFile->m_pHdr->m_TailLeaf = 0;
	pFile->m_pHdr->m_NodeEnd = sizeof(axl_log_TIndexFileHdr);

	axl_sys_TMappedFile_SetSize(&pFile->m_File, sizeof(axl_log_TIndexFileHdr));
	return true;
}

void
AXL_API
axl_log_TIndexFile_Clear(axl_log_TIndexFile* pFile)
{
	ASSERT(pFile->m_pHdr);

	pFile->m_pHdr->m_MessageCount = 0;
	pFile->m_pHdr->m_MessageSize = 0;
	pFile->m_pHdr->m_NodeCount = 0;
	pFile->m_pHdr->m_RootNode = 0;
	pFile->m_pHdr->m_HeadLeaf = 0;
	pFile->m_pHdr->m_TailLeaf = 0;
	pFile->m_pHdr->m_NodeEnd = sizeof(axl_log_TIndexFileHdr);

	axl_sys_TMappedFile_SetSize(&pFile->m_File, sizeof(axl_log_TIndexFileHdr));
}

axl_log_TIndexNode*
AXL_API
axl_log_TIndexFile_GetNode(
	axl_log_TIndexFile* pFile, 
	size_t Offset
	)
{
	axl_log_TIndexNode* pNode;

	if (!Offset)
		return NULL;

	ASSERT(pFile->m_pHdr);
	ASSERT(Offset >= sizeof(axl_log_TIndexFileHdr) && Offset <= pFile->m_pHdr->m_TailLeaf);

	pNode = (axl_log_TIndexNode*) axl_sys_TMappedFile_View(&pFile->m_File, Offset, sizeof(axl_log_TIndexNode));

	ASSERT(pNode);
	ASSERT(pNode->m_Offset == Offset);

	ASSERT(
		pNode->m_Signature == axl_log_EIndexFile_LeafSignature && pNode->m_Depth == 0||
		pNode->m_Signature == axl_log_EIndexFile_NodeSignature && pNode->m_Depth != 0
		);

	if (pNode->m_DataSize) // make sure mapping covers leaf data as well
		pNode = (axl_log_TIndexNode*) axl_sys_TMappedFile_View(&pFile->m_File, Offset, sizeof(axl_log_TIndexNode) + pNode->m_DataSize);

	return pNode;
}

axl_log_TIndexNode*
AXL_API
axl_log_TIndexFile_AddLeaf(
	axl_log_TIndexFile* pFile,	
	const void* p,
	size_t DataSize
	)
{
	size_t Offset;
	size_t Size = sizeof(axl_log_TIndexNode) + DataSize;	

	axl_log_TIndexNode* pLeaf;
	axl_log_TIndexNode* pParent;
	axl_log_TIndexNode* pTail;
	axl_log_TIndexNode* pNode;

	ASSERT(pFile->m_pHdr);

	if (!pFile->m_pHdr->m_RootNode)
	{
		Offset = sizeof(axl_log_TIndexFileHdr);
		ASSERT(Offset == pFile->m_pHdr->m_NodeEnd);

		pLeaf = (axl_log_TIndexNode*) axl_sys_TMappedFile_View(&pFile->m_File, Offset, Size);
		memset(pLeaf, 0, Size);

		pLeaf->m_Signature = axl_log_EIndexFile_LeafSignature;
		pLeaf->m_Offset = (ulong_t) Offset;
		pLeaf->m_DataSize  = (ulong_t) DataSize;

		if (p && DataSize)
			memcpy(pLeaf + 1, p, DataSize);

		pFile->m_pHdr->m_RootNode  = (ulong_t) Offset;
		pFile->m_pHdr->m_HeadLeaf  = (ulong_t) Offset;
		pFile->m_pHdr->m_TailLeaf  = (ulong_t) Offset;
		pFile->m_pHdr->m_NodeEnd   = (ulong_t) (Offset + Size);
		pFile->m_pHdr->m_NodeCount = 1;

		axl_sys_TMappedFile_SetSize(&pFile->m_File, pFile->m_pHdr->m_NodeEnd);
		return pLeaf;
	}

	pTail = axl_log_TIndexFile_GetTailLeaf(pFile);
	pNode = axl_log_TIndexFile_GetParentNode(pFile, pTail);

	Offset = pFile->m_pHdr->m_NodeEnd + sizeof(axl_log_TIndexNode);
	
	pParent = (axl_log_TIndexNode*) axl_sys_TMappedFile_View(&pFile->m_File, pFile->m_pHdr->m_NodeEnd, sizeof(axl_log_TIndexNode));
	pParent->m_Signature = axl_log_EIndexFile_NodeSignature;
	pParent->m_Offset    = pFile->m_pHdr->m_NodeEnd;
	pParent->m_Depth     = 1;
	pParent->m_Parent    = pTail->m_Parent; 
	pParent->m_Left      = pTail->m_Offset;
	pParent->m_Right     = (ulong_t) Offset;
	pParent->m_LineCount = pTail->m_LineCount;
	pParent->m_DataSize  = 0;
	
#ifdef _DEBUG
	pParent->m_RotationCount = 0;
#endif

	pLeaf = (axl_log_TIndexNode*) axl_sys_TMappedFile_View(&pFile->m_File, Offset, Size);
	pLeaf->m_Signature = axl_log_EIndexFile_LeafSignature;
	pLeaf->m_Offset    = (ulong_t) Offset;
	pLeaf->m_Depth     = 0;
	pLeaf->m_Parent    = pParent->m_Offset;
	pLeaf->m_Left      = pTail->m_Offset;
	pLeaf->m_Right     = 0;
	pLeaf->m_LineCount = 0;
	pLeaf->m_DataSize  = (ulong_t) DataSize;

	if (p && DataSize)
		memcpy(pLeaf + 1, p, DataSize);

	pTail->m_Parent = pParent->m_Offset;
	pTail->m_Right  = (ulong_t) Offset;

	pFile->m_pHdr->m_TailLeaf  = (ulong_t) Offset;
	pFile->m_pHdr->m_NodeEnd   = (ulong_t) (Offset + Size);
	pFile->m_pHdr->m_NodeCount += 2;

	axl_sys_TMappedFile_SetSize(&pFile->m_File, pFile->m_pHdr->m_NodeEnd);

	if (!pNode)
	{
		ASSERT(pFile->m_pHdr->m_NodeCount == 3 && pFile->m_pHdr->m_RootNode == pTail->m_Offset); 
		pFile->m_pHdr->m_RootNode = pParent->m_Offset; // fixup root -- must do this only once during tree grow
		return pLeaf;
	}

	pNode->m_Right = pParent->m_Offset;

	// go up to the root

	for (; pNode; pNode = axl_log_TIndexFile_GetParentNode(pFile, pNode))
	{
		axl_log_TIndexNode* pL;
		axl_log_TIndexNode* pR;
		axl_log_TIndexNode* pRL;
		axl_log_TIndexNode* pRR;

		ulong_t NewLeftDepth;
		ulong_t NewDepth;

		pL = axl_log_TIndexFile_GetNode(pFile, pNode->m_Left);
		pR = axl_log_TIndexFile_GetNode(pFile, pNode->m_Right);
		ASSERT(pR->m_Depth);

		pRL = axl_log_TIndexFile_GetNode(pFile, pR->m_Left);
		pRR = axl_log_TIndexFile_GetNode(pFile, pR->m_Right);

		NewLeftDepth = max(pL->m_Depth, pRL->m_Depth) + 1;
		NewDepth = max(NewLeftDepth, pRR->m_Depth) + 1;

		pNode->m_Depth = max(pL->m_Depth, pR->m_Depth) + 1;

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

	return axl_log_TIndexFile_GetNode(pFile, Offset);
}

void
AXL_API
axl_log_TIndexFile_CopyNode(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pDst,
	axl_log_TIndexNode* pSrc
	)
{
	axl_log_TIndexNode* pParent = axl_log_TIndexFile_GetParentNode(pFile, pSrc);
	axl_log_TIndexNode* pLeft   = axl_log_TIndexFile_GetLeftNode(pFile, pSrc);
	axl_log_TIndexNode* pRight  = axl_log_TIndexFile_GetRightNode(pFile, pSrc);

	ASSERT(pDst->m_Depth && pSrc->m_Depth); // not leaves!!

	pLeft->m_Parent = pDst->m_Offset;
	pRight->m_Parent = pDst->m_Offset;

	if (pParent)
	{
		if (pParent->m_Left == pSrc->m_Offset)
			pParent->m_Left = pDst->m_Offset;
		else if (pParent->m_Right == pSrc->m_Offset)
			pParent->m_Right = pDst->m_Offset;
		else
			ASSERT(false);
	}

	if (pFile->m_pHdr->m_RootNode == pSrc->m_Offset)
		pFile->m_pHdr->m_RootNode = pDst->m_Offset;

	pDst->m_Parent    = pSrc->m_Parent;
	pDst->m_Left      = pSrc->m_Left; 
	pDst->m_Right     = pSrc->m_Right;
	pDst->m_Depth     = pSrc->m_Depth;
	pDst->m_LineCount = pSrc->m_LineCount;
}

bool_t
AXL_API
axl_log_TIndexFile_RemoveTailLeaf(axl_log_TIndexFile* pFile)
{
	axl_log_TIndexNode* pTail;
	axl_log_TIndexNode* pPrev;
	axl_log_TIndexNode* pParent;
	axl_log_TIndexNode* pLeft;
	axl_log_TIndexNode* pNode;

	size_t AdjNodeOffset;

	ASSERT(pFile->m_pHdr);

	if (!pFile->m_pHdr->m_NodeCount)
		return false;

	if (pFile->m_pHdr->m_NodeCount == 1)
	{
		axl_log_TIndexFile_Clear(pFile);
		return true;
	}

	pTail = axl_log_TIndexFile_GetTailLeaf(pFile);
	pPrev = axl_log_TIndexFile_GetLeftNode(pFile, pTail);
	pParent = axl_log_TIndexFile_GetParentNode(pFile, pTail);
	pLeft = axl_log_TIndexFile_GetLeftNode(pFile, pParent);
	pNode = axl_log_TIndexFile_GetParentNode(pFile, pParent);

	pPrev->m_Right  = 0;
	pLeft->m_Parent = pParent->m_Parent;

	if (pNode)
		pNode->m_Right = pLeft->m_Offset;

	// ok, we are about to delete leaf and it's adjacent node (originally, its parent)
	// however, due to rotations logical parent might be not in adjacent node
	// therefore, we need to back up contents of adjacent node
	
	AdjNodeOffset = pTail->m_Offset - sizeof(axl_log_TIndexNode);
	if (pParent->m_Offset != AdjNodeOffset)
	{
		axl_log_TIndexNode* pAdjNode = axl_log_TIndexFile_GetNode(pFile, AdjNodeOffset);
		axl_log_TIndexFile_CopyNode(pFile, pParent, pAdjNode);

		// pLeft & pNode are referenced later, so fix them up if needed

		if (pLeft->m_Offset == AdjNodeOffset)
			pLeft = pParent;

		if (pNode && pNode->m_Offset == AdjNodeOffset)
			pNode = pParent;
	}

	// delete 2 nodes

	pFile->m_pHdr->m_TailLeaf  = pPrev->m_Offset;
	pFile->m_pHdr->m_NodeEnd   = pPrev->m_Offset + sizeof(axl_log_TIndexNode) + pPrev->m_DataSize;
	pFile->m_pHdr->m_NodeCount -= 2;

	axl_sys_TMappedFile_SetSize(&pFile->m_File, pFile->m_pHdr->m_NodeEnd);

	if (!pNode)
	{
		pFile->m_pHdr->m_RootNode = pLeft->m_Offset; // fixup root
		return true;
	}

	// go up to the root

	for (; pNode; pNode = axl_log_TIndexFile_GetParentNode(pFile, pNode))
	{
		axl_log_TIndexNode* pL = axl_log_TIndexFile_GetNode(pFile, pNode->m_Left);
		axl_log_TIndexNode* pR = axl_log_TIndexFile_GetNode(pFile, pNode->m_Right);
		axl_log_TIndexNode* pLL;
		axl_log_TIndexNode* pLR;

		ulong_t NewRightDepth;
		ulong_t NewDepth;

		pNode->m_Depth = max(pL->m_Depth, pR->m_Depth) + 1;
		pNode->m_LineCount = pL->m_LineCount + pR->m_LineCount;

		if (!pL->m_Depth)
			break;

		pLL = axl_log_TIndexFile_GetNode(pFile, pL->m_Left);
		pLR = axl_log_TIndexFile_GetNode(pFile, pL->m_Right);

		NewRightDepth = max(pLR->m_Depth, pR->m_Depth) + 1;
		NewDepth = max(NewRightDepth, pLL->m_Depth) + 1;

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
AXL_API
axl_log_TIndexFile_AddTailLeafData(
	axl_log_TIndexFile* pFile,	
	const void* p,
	size_t DataSize
	)
{
	axl_log_TIndexNode* pNode = axl_log_TIndexFile_GetTailLeaf(pFile);

	size_t OldDataSize = pNode->m_DataSize;
	size_t NewDataSize = pNode->m_DataSize + DataSize;

	pNode->m_DataSize = (ulong_t) NewDataSize; 

	pNode = axl_log_TIndexFile_GetTailLeaf(pFile); // get it one more time to include added data
	
	if (p)
		memcpy((char*) (pNode + 1) + OldDataSize, p, DataSize);
	else
		memset((char*) (pNode + 1) + OldDataSize, 0, DataSize);

	pFile->m_pHdr->m_NodeEnd = pNode->m_Offset + sizeof(axl_log_TIndexNode) + pNode->m_DataSize;
	axl_sys_TMappedFile_SetSize(&pFile->m_File, pFile->m_pHdr->m_NodeEnd);
	
	return NewDataSize;
}

size_t
AXL_API
axl_log_TIndexFile_RemoveTailLeafData(
	axl_log_TIndexFile* pFile,	
	size_t DataSize
	)
{
	axl_log_TIndexNode* pNode = axl_log_TIndexFile_GetTailLeaf(pFile);

	size_t NewDataSize = pNode->m_DataSize > DataSize ? pNode->m_DataSize - DataSize : 0;

	pNode->m_DataSize = (ulong_t) NewDataSize;

	pFile->m_pHdr->m_NodeEnd = pNode->m_Offset + sizeof(axl_log_TIndexNode) + pNode->m_DataSize;
	axl_sys_TMappedFile_SetSize(&pFile->m_File, pFile->m_pHdr->m_NodeEnd);
	
	return NewDataSize;
}

void
AXL_API
axl_log_TIndexFile_AddLeafLines(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode,
	long LineCount
	)
{
	ulong_t Offset = pNode->m_Offset;

	ASSERT(!pNode->m_Depth);

	if (LineCount == 0)
		return;

	pNode->m_LineCount += LineCount;

	pNode = axl_log_TIndexFile_GetParentNode(pFile, pNode);
	for (; pNode; pNode = axl_log_TIndexFile_GetParentNode(pFile, pNode))
		pNode->m_LineCount += LineCount;
	
	axl_log_TIndexFile_GetNode(pFile, Offset); // make sure the node is still mapped
}

ulong_t
AXL_API
axl_log_TIndexFile_GetLeafStartLine(
	axl_log_TIndexFile* pFile,
	axl_log_TIndexNode* pNode
	)
{
	ulong_t Offset = pNode->m_Offset;

	axl_log_TIndexNode* pParent = axl_log_TIndexFile_GetParentNode(pFile, pNode);
	ulong_t StartLine = 0;

	while (pParent)
	{
		if (pNode->m_Offset != pParent->m_Left)
		{
			axl_log_TIndexNode* pLeft = axl_log_TIndexFile_GetLeftNode(pFile, pParent);
			StartLine += pLeft->m_LineCount;
		}

		pNode = pParent;
		pParent = axl_log_TIndexFile_GetParentNode(pFile, pNode);
	}

	axl_log_TIndexFile_GetNode(pFile, Offset); // make sure the node is still mapped
	return StartLine;
}

axl_log_TIndexNode* 
AXL_API
axl_log_TIndexFile_FindLeafByLine(
	axl_log_TIndexFile* pFile,
	ulong_t Line,
	ulong_t* pStartLine
	)
{
	axl_log_TIndexNode* pNode;
	ulong_t StartLine = 0;

	ASSERT(pFile->m_pHdr);

	if (!pFile->m_pHdr->m_NodeCount)
		return NULL;

	pNode = axl_log_TIndexFile_GetRootNode(pFile);
	if (Line >= pNode->m_LineCount)
		return NULL;

	while (pNode->m_Depth)
	{
		axl_log_TIndexNode* pLeft = axl_log_TIndexFile_GetLeftNode(pFile, pNode);

		ASSERT(pLeft != NULL);
		ASSERT(Line < pNode->m_LineCount);

		if (Line < pLeft->m_LineCount)
		{
			pNode = pLeft;
		}
		else
		{
			pNode = axl_log_TIndexFile_GetRightNode(pFile, pNode);
			Line -= pLeft->m_LineCount;
			StartLine += pLeft->m_LineCount;
		}
	}

	if (pStartLine)
		*pStartLine = StartLine;

	return pNode;
}

//.............................................................................
