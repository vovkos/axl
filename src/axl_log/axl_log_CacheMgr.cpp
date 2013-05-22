#include "pch.h"
#include "axl_log_CacheMgr.h"
#include "axl_log_CacheRepresentorTarget.h"
#include "axl_log_IndexMgr.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

CCacheMgr::CCacheMgr ()
{
	m_pWidget = NULL;
	m_pIndexMgr = NULL;
	m_CacheMaxSize = 64;
	m_LongestTextLine = 0;
	m_LongestBinHexLine = 0;
	m_LongestBinTextLine = 0;
}

void
CCacheMgr::ClearCache ()
{
	m_CachePageList.Clear ();
	m_CachePageMap.Clear ();

	m_LongestTextLine = 0;
	m_LongestBinHexLine = 0;
	m_LongestBinTextLine = 0;
}

void
CCacheMgr::ClearCacheColorization ()
{
	rtl::CIteratorT <CCachePage> It = m_CachePageList.GetHead ();
	for (; It; It++)
		It->ClearColorization ();
}

TIndexNode* 
CCacheMgr::GetCachePageIndexNode (CCachePage* pPage)
{
	CIndexFile* pIndexFile = m_pIndexMgr->GetIndexFile ();
	TIndexNode* pNode = pIndexFile->GetNode (pPage->m_CacheMapIt->m_Key);
	TIndexLeafData* pNodeData = (TIndexLeafData*) (pNode + 1);
	ASSERT (pNode->m_DataSize >= sizeof (TIndexLeafData) + pNodeData->m_VolatilePacketCount * sizeof (TVolatilePacketData));
	return pNode;
}

void
CCacheMgr::RemoveCachePage (CCachePage* pPage)
{
	m_CachePageList.Remove (pPage);
	m_CachePageMap.Delete (pPage->m_CacheMapIt);
	AXL_MEM_DELETE (pPage);
}

CCachePage* 
CCacheMgr::FindCachePageByIndexNode (TIndexNode* pIndexNode)
{
	rtl::CHashTableMapIteratorT <size_t, CCachePage*> It = m_CachePageMap.Find (pIndexNode->m_Offset);
	if (!It)
		return NULL;

	CCachePage* pPage = It->m_Value;
	return !pPage->m_IsRepresentNeeded ? pPage : NULL;
}

size_t
CCacheMgr::GetCachePageStartLine (CCachePage* pPage)
{
	CIndexFile* pIndexFile = m_pIndexMgr->GetIndexFile ();
	TIndexNode* pIndexNode = pIndexFile->GetNode (pPage->m_CacheMapIt->m_Key);
	return pIndexFile->GetLeafStartLine (pIndexNode);
}

bool
CCacheMgr::CachePage (
	CCachePage* pPage,
	TIndexNode* pIndexNode
	)
{
	CPacketFile* pPacketFile = m_pIndexMgr->GetPacketFile ();
	CIndexFile* pIndexFile = m_pIndexMgr->GetIndexFile ();
	IRepresentor* pRepresentor = m_pIndexMgr->GetRepresentor ();

	size_t Offset = pPage->m_PacketOffset;
	size_t PacketEnd = pPacketFile->GetHdr ()->m_TotalPacketSize;
	size_t OldLineCount = pPage->m_LineArray.GetCount ();
	size_t PacketIndex = pPage->m_PacketCount;
	size_t VolatilePacketIndex = pPage->m_VolatilePacketArray.GetCount ();

	TIndexLeafData* pIndexNodeData = (TIndexLeafData*) (pIndexNode + 1);
	TVolatilePacketData* pVolatilePacketData = (TVolatilePacketData*) (pIndexNodeData + 1);
	TVolatilePacketData* pVolatilePacketDataEnd = pVolatilePacketData + pIndexNodeData->m_VolatilePacketCount;

	pVolatilePacketData += VolatilePacketIndex;

	if (!pPage->m_IsRepresentNeeded)
		return true;

	if (!pIndexNode->m_LineCount) // empty page
	{
		ASSERT (OldLineCount == 0);
		pPage->m_IsRepresentNeeded = false;
		return true;
	}

	// append lines and packets to the page

	if (pIndexNode->m_LineCount > OldLineCount)
		pPage->m_LineArray.Append (NULL, pIndexNode->m_LineCount - OldLineCount);

	pPage->m_VolatilePacketArray.Reserve (pIndexNodeData->m_VolatilePacketCount);

//  uncomment if you want a break at the new page caching
//
//	if (!OldLineCount)
//		_asm int 3

	CCacheRepresentorTarget RepresentorTarget;

	RepresentorTarget.m_pPage = pPage;
	RepresentorTarget.m_BinDataConfig = pPage->m_DefBinDataConfig;
	RepresentorTarget.m_BinOffset = pPage->m_BinOffset;
	RepresentorTarget.m_MergeId = pPage->m_MergeId;
	RepresentorTarget.m_PartIndex = pPage->m_PartIndex;
	RepresentorTarget.m_MergeCriteria = pPage->m_MergeCriteria;
	RepresentorTarget.m_Line = OldLineCount;
	RepresentorTarget.m_EndLine = pIndexNode->m_LineCount;

	ASSERT (RepresentorTarget.m_Line <= RepresentorTarget.m_EndLine);
	
	// merging with previous node

	if (!RepresentorTarget.m_Line && !RepresentorTarget.m_pPrevPageMergeLine)
	{
		TIndexNode* pIndexPrevNode = pIndexFile->GetLeftNode (pIndexNode);
		TIndexLeafData* pIndexPrevNodeData = (TIndexLeafData*) (pIndexPrevNode + 1);

		if (pIndexPrevNode)
		{
			RepresentorTarget.m_MergeCriteria = pIndexPrevNodeData->m_MergeCriteria;
			RepresentorTarget.m_BinOffset = pIndexPrevNodeData->m_BinOffset;
			RepresentorTarget.m_PartIndex = pIndexPrevNodeData->m_PartIndex;
			RepresentorTarget.m_MergeId = pIndexPrevNodeData->m_MergeId;

			RepresentorTarget.CreatePrevPageMergeLine (
				pIndexPrevNodeData->m_MergeCriteria,
				pIndexPrevNodeData->m_Col
				);
		}
	}

	while (PacketIndex < pIndexNodeData->m_PacketCount)
	{
		TPacket* pPacket = pPacketFile->GetPacket (Offset);
		ASSERT (pPacket);

		RepresentorTarget.m_IsFirstLineOfPacket = true;
		RepresentorTarget.m_PacketCode = pPacket->m_Code;
		RepresentorTarget.m_PacketOffset = Offset;
		RepresentorTarget.m_Timestamp = pPacket->m_Timestamp;
		RepresentorTarget.m_BinDataConfig = pPage->m_DefBinDataConfig;
		RepresentorTarget.m_LineAttr.Clear ();

		if (pPacket->m_Code & EPacketCodeFlag_Mergeable)
		{
			RepresentorTarget.m_pVolatilePacket = NULL;

			pRepresentor->Represent (
				&RepresentorTarget,
				pPacket->m_Code,
				pPacket + 1,
				pPacket->m_DataSize,
				0
				);
		}		
		else if (pPacket->m_Code & EPacketCodeFlag_Volatile)
		{
			TCacheVolatilePacket* pVolatileMsg = AXL_MEM_NEW (TCacheVolatilePacket);

			ASSERT (pVolatilePacketData < pVolatilePacketDataEnd);

			pVolatileMsg->m_Offset = Offset;
			pVolatileMsg->m_Code = pPacket->m_Code;
			pVolatileMsg->m_Timestamp = pPacket->m_Timestamp;
			pVolatileMsg->m_DataSize = pPacket->m_DataSize;
			pVolatileMsg->m_VolatileIndex = VolatilePacketIndex;
			pVolatileMsg->m_VolatileFlags = pVolatilePacketData->m_VolatileFlags;
			pVolatileMsg->m_pFirstLine = NULL;
			pVolatileMsg->m_LineCount = 0;

			pPage->m_VolatilePacketArray.Append (pVolatileMsg);

			RepresentorTarget.m_MergeCriteria.Break ();

			OldLineCount = RepresentorTarget.m_Line;

			RepresentorTarget.m_pVolatilePacket = pVolatileMsg;

			pRepresentor->Represent (
				&RepresentorTarget,
				pPacket->m_Code,
				pPacket + 1,
				pPacket->m_DataSize,
				pVolatilePacketData->m_VolatileFlags
				);

			ASSERT (OldLineCount + pVolatileMsg->m_LineCount == RepresentorTarget.m_Line);

			RepresentorTarget.m_MergeCriteria.Break ();

			VolatilePacketIndex++;
			pVolatilePacketData++;
		}
		else // normal: non-mergeable and non-volatile
		{
			RepresentorTarget.m_MergeCriteria.Break ();

			pRepresentor->Represent (
				&RepresentorTarget,
				pPacket->m_Code,
				pPacket + 1,
				pPacket->m_DataSize,
				0
				);

			RepresentorTarget.m_MergeCriteria.Break ();
		}

		// done, go to next packet

		Offset += sizeof (TPacket);
		Offset += pPacket->m_DataSize;
		PacketIndex++;
	}

	if (!RepresentorTarget.m_IsFull) // maybe merge with one (!) packet from the next node
	{
		pIndexNode = pIndexFile->GetRightNode (pIndexNode);
		pIndexNodeData = (TIndexLeafData*) (pIndexNode + 1);

		ASSERT (
			!pIndexNode ||
			pIndexNodeData->m_PacketCount > 0 && Offset < PacketEnd ||
			pIndexNodeData->m_PacketCount == 0 && Offset == PacketEnd
			);

		if (pIndexNode && Offset < PacketEnd)
		{
			TPacket* pPacket = pPacketFile->GetPacket (Offset);
			ASSERT (pPacket);

			if (pPacket->m_Code & EPacketCodeFlag_Mergeable)
			{
				RepresentorTarget.m_IsFirstLineOfPacket = true;
				RepresentorTarget.m_PacketCode = pPacket->m_Code;
				RepresentorTarget.m_PacketOffset = Offset;
				RepresentorTarget.m_Timestamp = pPacket->m_Timestamp;
				RepresentorTarget.m_BinDataConfig = pIndexNodeData->m_DefBinDataConfig;
				RepresentorTarget.m_LineAttr.Clear ();
				RepresentorTarget.m_pVolatilePacket = NULL;

				pRepresentor->Represent (
					&RepresentorTarget,
					pPacket->m_Code,
					pPacket + 1,
					pPacket->m_DataSize,
					0
					);
			}
		}
	}

	ASSERT (!pPage->m_LineArray.IsEmpty ());

	if (RepresentorTarget.m_LongestTextLine > m_LongestTextLine)
		m_LongestTextLine = RepresentorTarget.m_LongestTextLine;

	if (RepresentorTarget.m_LongestBinHexLine > m_LongestBinHexLine)
		m_LongestBinHexLine = RepresentorTarget.m_LongestBinHexLine;

	if (RepresentorTarget.m_LongestBinTextLine > m_LongestBinTextLine)
		m_LongestBinTextLine = RepresentorTarget.m_LongestBinTextLine;

	pPage->m_PacketCount = PacketIndex;
	pPage->m_MergeCriteria = RepresentorTarget.m_MergeCriteria;
	pPage->m_MergeId = RepresentorTarget.m_MergeId;
	pPage->m_PartIndex = RepresentorTarget.m_PartIndex;
	pPage->m_BinOffset = RepresentorTarget.m_BinOffset;
	pPage->m_PacketOffset = Offset;
	pPage->m_IsRepresentNeeded = false;

	m_pWidget->RecalcColCount ();
	
	return true;
}

CCachePage* 
CCacheMgr::GetCachePageByIndexNode (TIndexNode* pIndexNode)
{
	CCachePage* pPage;

	// if this index node already cached?

	rtl::CHashTableMapIteratorT <size_t, CCachePage*> It = m_CachePageMap.Find (pIndexNode->m_Offset);
	if (It)
	{
		pPage = It->m_Value;
	
		// move it to the tail (so it's marked as 'recently used')

		m_CachePageList.MoveToTail (pPage);

		if (pPage->m_IsRepresentNeeded)
			CachePage (pPage, pIndexNode);

		return pPage;
	}

	// maintain cache max size

	if (m_CachePageList.GetCount () > m_CacheMaxSize)
	{
		// dispose the oldest page 

		pPage = *m_CachePageList.GetHead ();
		RemoveCachePage (pPage);
	}

	// create and cache new page!

	TIndexLeafData* pIndexNodeData = (TIndexLeafData*) (pIndexNode + 1);

	pPage = AXL_MEM_NEW (CCachePage);
	pPage->m_pCacheMgr = this;
	m_CachePageList.InsertTail (pPage);
	
	It = m_CachePageMap.Goto (pIndexNode->m_Offset);
	It->m_Value = pPage;

	pPage->m_CacheMapIt = It;
	pPage->m_PacketOffset = pIndexNodeData->m_PacketOffset;
	pPage->m_DefBinDataConfig = pIndexNodeData->m_DefBinDataConfig;

	CachePage (pPage, pIndexNode);
	return pPage;
}

CCachePage* 
CCacheMgr::FindCachePageByLine (
	size_t Line,
	size_t* pStartLine
	)
{
	CIndexFile* pIndexFile = m_pIndexMgr->GetIndexFile ();

	size_t StartLine;

	TIndexNode* pIndexNode = pIndexFile->FindLeafByLine (Line, &StartLine);
	if (!pIndexNode)
		return NULL;

	if (pStartLine)
		*pStartLine = StartLine;

	return FindCachePageByIndexNode (pIndexNode);
}

CCachePage* 
CCacheMgr::GetCachePageByLine (
	size_t Line,
	size_t* pStartLine
	)
{
	CIndexFile* pIndexFile = m_pIndexMgr->GetIndexFile ();

	size_t StartLine;

	TIndexNode* pIndexNode = pIndexFile->FindLeafByLine (Line, &StartLine);
	if (!pIndexNode)
		return NULL;

	if (pStartLine)
		*pStartLine = StartLine;

	return GetCachePageByIndexNode (pIndexNode);
}

CCachePage* 
CCacheMgr::GetNextCachePage (CCachePage* pPage)
{
	CIndexFile* pIndexFile = m_pIndexMgr->GetIndexFile ();
	TIndexNode* pIndexNode = GetCachePageIndexNode (pPage);

	pIndexNode = pIndexFile->GetRightNode (pIndexNode);
	if (!pIndexNode)
		return NULL;

	return GetCachePageByIndexNode (pIndexNode);
}

CCachePage* 
CCacheMgr::GetPrevCachePage (CCachePage* pPage)
{
	CIndexFile* pIndexFile = m_pIndexMgr->GetIndexFile ();
	TIndexNode* pIndexNode = GetCachePageIndexNode (pPage);
	
	pIndexNode = pIndexFile->GetLeftNode (pIndexNode);
	if (!pIndexNode)
		return NULL;

	return GetCachePageByIndexNode (pIndexNode);
}

CLine*
CCacheMgr::FindLine (size_t Line)
{
	size_t StartLine;

	CCachePage* pPage = FindCachePageByLine (Line, &StartLine); 
	if (!pPage)
		return NULL;

	return pPage->m_LineArray [Line - StartLine];
}

CLine*
CCacheMgr::GetLine (size_t Line)
{
	size_t StartLine;

	CCachePage* pPage = GetCachePageByLine (Line, &StartLine); 
	if (!pPage)
		return NULL;

	return pPage->m_LineArray [Line - StartLine];
}

CLine*
CCacheMgr::GetLineIncrement (
	CLine* pLine,
	size_t Delta
	)
{
	CCachePage* pPage = pLine->m_pPage;
	size_t LineCount = pPage->m_LineArray.GetCount ();

	if (pLine->m_PageIndex + Delta < LineCount)
		return pPage->m_LineArray [pLine->m_PageIndex + Delta];

	Delta -= LineCount - pLine->m_PageIndex;

	for (;;)
	{
		pPage = GetNextCachePage (pPage);
		if (!pPage)
			break;

		LineCount = pPage->m_LineArray.GetCount ();
		if (Delta < LineCount)
			return pPage->m_LineArray [Delta];

		Delta -= LineCount;
	}

	return NULL;
}

CLine*
CCacheMgr::GetLineDecrement (
	CLine* pLine,
	size_t Delta
	)
{
	CCachePage* pPage = pLine->m_pPage;
	size_t LineCount = pPage->m_LineArray.GetCount ();

	if (Delta <= pLine->m_PageIndex)
		return pPage->m_LineArray [pLine->m_PageIndex - Delta];

	Delta -= pLine->m_PageIndex;

	for (;;)
	{
		pPage = GetPrevCachePage (pPage);
		if (!pPage)
			break;

		LineCount = pPage->m_LineArray.GetCount ();
		if (Delta <= LineCount)
			return pPage->m_LineArray [LineCount - Delta];

		Delta -= LineCount;
	}

	return NULL;
}

void
CCacheMgr::ReRepresentPacket (
	CCachePage* pPage,
	TCacheVolatilePacket* pVolatileMsg,
	TReRepresent* pReRepresent
	)
{
	CPacketFile* pPacketFile = m_pIndexMgr->GetPacketFile ();
	CIndexFile* pIndexFile = m_pIndexMgr->GetIndexFile ();
	IRepresentor* pRepresentor = m_pIndexMgr->GetRepresentor ();

	TPacket* pPacket;
	TIndexNode* pNode;
	TIndexLeafData* pNodeData;
	TVolatilePacketData* pVolatileMsgData;

	uint_t VolatileFlags;
	size_t AnchorLine;
	size_t OldLineCount;
	size_t NewLineCount;
	intptr_t LineDelta;

	// get packet from packet file

	pPacket = pPacketFile->GetPacket (pVolatileMsg->m_Offset);
	ASSERT (!(pPacket->m_Code & EPacketCodeFlag_Mergeable)); 

	// get index node data from index file

	pNode = GetCachePageIndexNode (pPage);
	pNodeData = (TIndexLeafData*) (pNode + 1);
	pVolatileMsgData = (TVolatilePacketData*) (pNodeData + 1) + pVolatileMsg->m_VolatileIndex;

	ASSERT (pVolatileMsg->m_VolatileIndex < pNodeData->m_VolatilePacketCount);
	ASSERT (pVolatileMsg->m_LineCount == pVolatileMsgData->m_LineCount);

	// prepare page

	AnchorLine = pPage->GetVolatilePacketAnchorLine (pVolatileMsg);
	OldLineCount = pVolatileMsg->m_LineCount;
	VolatileFlags = pVolatileMsgData->m_VolatileFlags;
	
	NewLineCount = m_pIndexMgr->CalcPacketLineCount (pPacket, pPage->m_DefBinDataConfig, VolatileFlags);

	pVolatileMsgData->m_LineCount = (size_t) NewLineCount;

	LineDelta = NewLineCount - OldLineCount;

	pPage->FreeLines (AnchorLine, OldLineCount);
	pPage->AddLines (AnchorLine, LineDelta);
	pIndexFile->AddLeafLines (pNode, LineDelta);

	// re-cache packet

	pVolatileMsg->m_VolatileFlags = VolatileFlags;
	pVolatileMsg->m_pFirstLine = NULL;
	pVolatileMsg->m_LineCount = 0;

	if (NewLineCount)
	{
		CCacheRepresentorTarget RepresentorTarget;

		RepresentorTarget.m_IsFirstLineOfPacket = true;
		RepresentorTarget.m_PacketOffset = pVolatileMsg->m_Offset;
		RepresentorTarget.m_Timestamp = pVolatileMsg->m_Timestamp;
		RepresentorTarget.m_pPage = pPage;
		RepresentorTarget.m_pVolatilePacket = pVolatileMsg;
		RepresentorTarget.m_Line = AnchorLine;
		RepresentorTarget.m_EndLine = AnchorLine + NewLineCount;
		RepresentorTarget.m_PacketCode = pPacket->m_Code;
		RepresentorTarget.m_BinDataConfig = pPage->m_DefBinDataConfig;

		pRepresentor->Represent (
			&RepresentorTarget,
			pPacket->m_Code,
			pPacket + 1,
			pPacket->m_DataSize,
			VolatileFlags
			);

		if (RepresentorTarget.m_LongestTextLine > m_LongestTextLine)
			m_LongestTextLine = RepresentorTarget.m_LongestTextLine;

		if (RepresentorTarget.m_LongestBinHexLine > m_LongestBinHexLine)
			m_LongestBinHexLine = RepresentorTarget.m_LongestBinHexLine;

		if (RepresentorTarget.m_LongestBinTextLine > m_LongestBinTextLine)
			m_LongestBinTextLine = RepresentorTarget.m_LongestBinTextLine;
	}

	ASSERT (pVolatileMsg->m_LineCount == NewLineCount);

	// fill the results

	pReRepresent->m_AnchorAbsoluteLine = GetCachePageStartLine (pPage) + AnchorLine;
	pReRepresent->m_AnchorPageLine = AnchorLine;
	pReRepresent->m_OldLineCount = OldLineCount;
	pReRepresent->m_NewLineCount = NewLineCount;	
}

bool
CCacheMgr::ModifyPacketVolatileFlags (
	CCachePage* pPage,
	TCacheVolatilePacket* pVolatileMsg,
	uint_t VolatileFlags,
	TReRepresent* pReRepresent
	)
{
	TIndexNode* pNode = GetCachePageIndexNode (pPage);
	TIndexLeafData* pNodeData = (TIndexLeafData*) (pNode + 1);
	TVolatilePacketData* pVolatileMsgData = (TVolatilePacketData*) (pNodeData + 1) + pVolatileMsg->m_VolatileIndex;

	if (pVolatileMsg->m_VolatileFlags == VolatileFlags && 
		pVolatileMsgData->m_VolatileFlags == VolatileFlags)
		return false;

	pVolatileMsgData->m_VolatileFlags = VolatileFlags;
	ReRepresentPacket (pPage, pVolatileMsg, pReRepresent);
	return true;
}

//.............................................................................

} // namespace log {
} // namespace axl {
