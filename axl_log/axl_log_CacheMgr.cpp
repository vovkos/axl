#include "axl_Precomp.h"
#include "axl_log_CacheMgr.h"
#include "axl_log_CacheSink.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_log_IndexMgr.h"
#include "axl_log_LogCtrl.h"

//.............................................................................

void
AXL_API
axl_log_TCacheMgr_Construct(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLogCtrl* pLogCtrl
	)
{
	axl_rtl_TList_Construct(&pCacheMgr->m_Cache);
	axl_rtl_TRbTree_Construct(&pCacheMgr->m_CacheMap, axl_rtl_CmpULong);

	pCacheMgr->m_pLogCtrl = pLogCtrl;
	pCacheMgr->m_CacheMaxSize = axl_log_EDef_CacheMaxSize;
	pCacheMgr->m_LongestTextLine = 0;
	pCacheMgr->m_LongestBinHexLine = 0;
	pCacheMgr->m_LongestBinTextLine = 0;
}

void
AXL_API
axl_log_TCacheMgr_Destruct(axl_log_TCacheMgr* pCacheMgr)
{
	axl_log_TCacheMgr_ClearCache(pCacheMgr);
	axl_rtl_TRbTree_Destruct(&pCacheMgr->m_CacheMap);
}

void
AXL_API
axl_log_TCacheMgr_ClearCache(axl_log_TCacheMgr* pCacheMgr)
{
	while (!axl_rtl_TList_IsEmpty(&pCacheMgr->m_Cache))
	{
		axl_log_TCachePage* pPage = (axl_log_TCachePage*) axl_rtl_TList_RemoveHead(&pCacheMgr->m_Cache);
		axl_log_TCachePage_Destruct(pPage);
		axl_mem_Free(pPage);
	}

	axl_rtl_TBinTree_Clear(&pCacheMgr->m_CacheMap);

	pCacheMgr->m_LongestTextLine = 0;
	pCacheMgr->m_LongestBinHexLine = 0;
	pCacheMgr->m_LongestBinTextLine = 0;
}

void
AXL_API
axl_log_TCacheMgr_ClearCacheColorization(axl_log_TCacheMgr* pCacheMgr)
{
	axl_log_TCachePage* pPage = (axl_log_TCachePage*) axl_rtl_TList_GetHead(&pCacheMgr->m_Cache);

	while (pPage)
	{
		axl_log_TCachePage_ClearColorization(pPage);
		pPage = (axl_log_TCachePage*) axl_rtl_TList_GetNext(&pCacheMgr->m_Cache, (axl_rtl_TListEntry*) pPage);
	}
}

axl_log_TIndexNode* 
AXL_API
axl_log_TCacheMgr_GetCachePageIndexNode(
	axl_log_TCacheMgr* pCacheMgr, 
	axl_log_TCachePage* pPage
	)
{
	axl_log_TIndexFile* pIndexFile = &pCacheMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pNode = axl_log_TIndexFile_GetNode(pIndexFile, (ulong_t) (pPage)->m_pCacheMapNode->m_Key);
	axl_log_TIndexNodeData* pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
	ASSERT(pNode->m_DataSize >= sizeof(axl_log_TIndexNodeData) + pNodeData->m_VolatileMessageCount * sizeof(axl_log_TVolatileMessageData));
	return pNode;
}

void
AXL_API
axl_log_TCacheMgr_RemoveCachePage(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage
	)
{
	axl_rtl_TList_Remove(&pCacheMgr->m_Cache, (axl_rtl_TListEntry*) pPage);
	axl_rtl_TBinTree_Delete(&pCacheMgr->m_CacheMap, pPage->m_pCacheMapNode);
	axl_log_TCachePage_Destruct(pPage);
	axl_mem_Free(pPage);
}

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_FindCachePageByIndexNode(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TIndexNode* pIndexNode
	)
{
	axl_log_TCachePage* pPage;
	axl_rtl_TBinTreeNode* pCacheMapNode = axl_rtl_TBinTree_Find(&pCacheMgr->m_CacheMap, pIndexNode->m_Offset);
	if (!pCacheMapNode)
		return NULL;

	pPage = (axl_log_TCachePage*) pCacheMapNode->m_Value;
	return !pPage->m_IsRepresentNeeded ? pPage : NULL;
}

ulong_t 
AXL_API
axl_log_TCacheMgr_GetCachePageStartLine(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage
	)
{
	axl_log_TIndexFile* pIndexFile = &pCacheMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pIndexNode = axl_log_TIndexFile_GetNode(pIndexFile, (ulong_t) pPage->m_pCacheMapNode->m_Key);
	return axl_log_TIndexFile_GetLeafStartLine(pIndexFile, pIndexNode);
}

ulong_t 
AXL_API
axl_log_TCacheMgr_GetAbsoluteLine(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine
	)
{
	return axl_log_TCacheMgr_GetCachePageStartLine(pCacheMgr, pLine->m_pPage) + (ulong_t) pLine->m_PageIndex;
}

bool_t
AXL_API
axl_log_TCacheMgr_CachePage(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage,
	axl_log_TIndexNode* pIndexNode
	)
{
	axl_log_TMessageFile* pMessageFile = pCacheMgr->m_pLogCtrl->m_pMessageFile;
	axl_log_TIndexFile* pIndexFile = &pCacheMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TRepresentor* pRepresentor = pCacheMgr->m_pLogCtrl->m_pRepresentor;

	bool_t IsMergeable;
	axl_log_TCacheSink CacheSink;

	size_t Offset = pPage->m_MessageOffset;
	size_t MessageEnd = pMessageFile->m_pHdr->m_MessageSize;
	size_t OldLineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
	size_t MessageIndex = pPage->m_MessageCount;
	size_t VolatileMessageIndex = axl_rtl_TPtrArray_GetCount(&pPage->m_VolatileMessageArray);

	axl_log_TIndexNodeData* pIndexNodeData = (axl_log_TIndexNodeData*) (pIndexNode + 1);
	axl_log_TVolatileMessageData* pVolatileMessageData = (axl_log_TVolatileMessageData*) (pIndexNodeData + 1);
	axl_log_TVolatileMessageData* pVolatileMessageDataEnd = pVolatileMessageData + pIndexNodeData->m_VolatileMessageCount;

	pVolatileMessageData += VolatileMessageIndex;

	if (!pPage->m_IsRepresentNeeded)
		return true;

	if (!pIndexNode->m_LineCount) // empty page
	{
		ASSERT(OldLineCount == 0);
		pPage->m_IsRepresentNeeded = false;
		return true;
	}

	// append lines and messages to the page

	if (pIndexNode->m_LineCount > OldLineCount)
		axl_rtl_TPtrArray_Append(&pPage->m_LineArray, NULL, pIndexNode->m_LineCount - OldLineCount);

	axl_rtl_TPtrArray_Reserve(&pPage->m_VolatileMessageArray, pIndexNodeData->m_VolatileMessageCount);

//  uncomment if you want a break at the new page caching
//
//	if (!OldLineCount)
//		_asm int 3

	axl_log_TCacheSink_Construct(&CacheSink);
	CacheSink.m_pPage = pPage;
	CacheSink.m_DataConfig = pPage->m_PreferredDataConfig;
	CacheSink.m_BinOffset = pPage->m_BinOffset;
	CacheSink.m_MergeId = pPage->m_MergeId;
	CacheSink.m_PartIndex = pPage->m_PartIndex;
	CacheSink.m_MergeCriteria = pPage->m_MergeCriteria;
	CacheSink.m_Line = OldLineCount;
	CacheSink.m_EndLine = pIndexNode->m_LineCount;

	ASSERT(CacheSink.m_Line <= CacheSink.m_EndLine);
    
	// merging with previous node

	if (!CacheSink.m_Line && !CacheSink.m_pPrevPageMergeLine)
	{
		axl_log_TIndexNode* pIndexPrevNode = axl_log_TIndexFile_GetLeftNode(pIndexFile, pIndexNode);
		axl_log_TIndexNodeData* pIndexPrevNodeData = (axl_log_TIndexNodeData*) (pIndexPrevNode + 1);

		if (pIndexPrevNode)
		{
			CacheSink.m_MergeCriteria = pIndexPrevNodeData->m_MergeCriteria;
			CacheSink.m_BinOffset = pIndexPrevNodeData->m_BinOffset;
			CacheSink.m_PartIndex = pIndexPrevNodeData->m_PartIndex;
			CacheSink.m_MergeId = pIndexPrevNodeData->m_MergeId;

			axl_log_TCacheSink_CreatePrevPageMergeLine(
				&CacheSink,
				&pIndexPrevNodeData->m_MergeCriteria,
				pIndexPrevNodeData->m_Col
				);
		}
	}

	while (MessageIndex < pIndexNodeData->m_MessageCount)
	{
		axl_log_TMessage* pMsg = axl_log_TMessageFile_GetMessage(pMessageFile, Offset);
		ASSERT(pMsg);

		IsMergeable = (pMsg->m_Code & axl_log_EMessageCode_Mergeable) != 0;

		CacheSink.m_IsFirstLineOfMessage = true;
		CacheSink.m_PartCode = pMsg->m_Code;
		CacheSink.m_MessageOffset = Offset;
		CacheSink.m_Timestamp = pMsg->m_Timestamp;
		CacheSink.m_Icon = -1;
		CacheSink.m_TileIcon = false;
		axl_gr_TTextAttr_Clear(&CacheSink.m_Attr);

		if (IsMergeable)
		{
			CacheSink.m_Merge = axl_log_EMerge_Normal;
			CacheSink.m_pVolatileMessage = NULL;

			axl_log_TRepresentor_Represent(
				pRepresentor,
				(axl_log_TRepresentSink*) &CacheSink,
				pMsg->m_Code,
				pMsg + 1,
				pMsg->m_DataSize,
				0
				);
		}
		else
		{
			axl_log_TCacheVolatileMessage* pVolatileMsg = axl_mem_Allocate(sizeof(axl_log_TCacheVolatileMessage));

			ASSERT(pVolatileMessageData < pVolatileMessageDataEnd);

			pVolatileMsg->m_Offset = Offset;
			pVolatileMsg->m_Code = pMsg->m_Code;
			pVolatileMsg->m_Timestamp = pMsg->m_Timestamp;
			pVolatileMsg->m_DataSize = pMsg->m_DataSize;
			pVolatileMsg->m_VolatileIndex = VolatileMessageIndex;
			pVolatileMsg->m_VolatileFlags = pVolatileMessageData->m_VolatileFlags;
			pVolatileMsg->m_pFirstLine = NULL;
			pVolatileMsg->m_LineCount = 0;

			axl_rtl_TPtrArray_AppendElement(&pPage->m_VolatileMessageArray, pVolatileMsg);

			axl_log_TMergeCriteria_Clear(&CacheSink.m_MergeCriteria);

			OldLineCount = CacheSink.m_Line;

			CacheSink.m_Merge = 0;
			CacheSink.m_pVolatileMessage = pVolatileMsg;

			axl_log_TRepresentor_Represent(
				pRepresentor,
				(axl_log_TRepresentSink*) &CacheSink,
				pMsg->m_Code,
				pMsg + 1,
				pMsg->m_DataSize,
				pVolatileMessageData->m_VolatileFlags
				);

			ASSERT(OldLineCount + pVolatileMsg->m_LineCount == CacheSink.m_Line);

			axl_log_TMergeCriteria_Clear(&CacheSink.m_MergeCriteria);

			VolatileMessageIndex++;
			pVolatileMessageData++;
		}

		// done, go to next message

		Offset += sizeof(axl_log_TMessage);
		Offset += pMsg->m_DataSize;
		MessageIndex++;
	}

	if (!CacheSink.m_IsFull) // maybe merge with one (!) message from the next node
	{
		pIndexNode = axl_log_TIndexFile_GetRightNode(pIndexFile, pIndexNode);
		pIndexNodeData = (axl_log_TIndexNodeData*) (pIndexNode + 1);

		ASSERT(
			!pIndexNode ||
			pIndexNodeData->m_MessageCount > 0 && Offset < MessageEnd ||
			pIndexNodeData->m_MessageCount == 0 && Offset == MessageEnd
			);

		if (pIndexNode && Offset < MessageEnd)
		{
			axl_log_TMessage* pMsg = axl_log_TMessageFile_GetMessage(pMessageFile, Offset);
			ASSERT(pMsg);

			IsMergeable = (pMsg->m_Code & axl_log_EMessageCode_Mergeable) != 0;
			if (IsMergeable)
			{
				CacheSink.m_DataConfig = pIndexNodeData->m_PreferredDataConfig;
				CacheSink.m_IsFirstLineOfMessage = true;
				CacheSink.m_PartCode = pMsg->m_Code;
				CacheSink.m_MessageOffset = Offset;
				CacheSink.m_Timestamp = pMsg->m_Timestamp;
				CacheSink.m_Icon = -1;
				CacheSink.m_TileIcon = false;
				axl_gr_TTextAttr_Clear(&CacheSink.m_Attr);

				CacheSink.m_Merge = axl_log_EMerge_Normal;
				CacheSink.m_pVolatileMessage = NULL;

				axl_log_TRepresentor_Represent(
					pRepresentor,
					(axl_log_TRepresentSink*) &CacheSink,
					pMsg->m_Code,
					pMsg + 1,
					pMsg->m_DataSize,
					0
					);
			}
		}
	}

	ASSERT(!axl_rtl_TPtrArray_IsEmpty(&pPage->m_LineArray));

	if (CacheSink.m_LongestTextLine > pCacheMgr->m_LongestTextLine)
		pCacheMgr->m_LongestTextLine = CacheSink.m_LongestTextLine;

	if (CacheSink.m_LongestBinHexLine > pCacheMgr->m_LongestBinHexLine)
		pCacheMgr->m_LongestBinHexLine = CacheSink.m_LongestBinHexLine;

	if (CacheSink.m_LongestBinTextLine > pCacheMgr->m_LongestBinTextLine)
		pCacheMgr->m_LongestBinTextLine = CacheSink.m_LongestBinTextLine;

	pPage->m_MessageCount = MessageIndex;
	pPage->m_MergeCriteria = CacheSink.m_MergeCriteria;
	pPage->m_MergeId = CacheSink.m_MergeId;
	pPage->m_PartIndex = CacheSink.m_PartIndex;
	pPage->m_BinOffset = CacheSink.m_BinOffset;
	pPage->m_MessageOffset = Offset;
	pPage->m_IsRepresentNeeded = false;

	axl_log_TLogCtrl_RecalcColCount(pCacheMgr->m_pLogCtrl);
	
	axl_log_TCacheSink_Destruct(&CacheSink);
	return true;
}

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_GetCachePageByIndexNode(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TIndexNode* pIndexNode
	)
{
	axl_log_TCachePage* pPage;
	axl_rtl_TBinTreeNode* pCacheMapNode;
	axl_log_TIndexNodeData* pIndexNodeData;

	// if this index node already cached?

	pCacheMapNode = axl_rtl_TBinTree_Find(&pCacheMgr->m_CacheMap, pIndexNode->m_Offset);
	if (pCacheMapNode)
	{
		pPage = (axl_log_TCachePage*) pCacheMapNode->m_Value;
	
		// move it to the tail (so it's marked as 'recently used')

		axl_rtl_TList_Remove(&pCacheMgr->m_Cache, (axl_rtl_TListEntry*) pPage);
		axl_rtl_TList_InsertTail(&pCacheMgr->m_Cache, (axl_rtl_TListEntry*) pPage);

		if (pPage->m_IsRepresentNeeded)
			axl_log_TCacheMgr_CachePage(pCacheMgr, pPage, pIndexNode);

		return pPage;
	}

	// maintain cache max size

	if (pCacheMgr->m_Cache.m_Count > pCacheMgr->m_CacheMaxSize)
	{
		// dispose the oldest page 

		pPage = (axl_log_TCachePage*) axl_rtl_TList_GetHead(&pCacheMgr->m_Cache);
		axl_log_TCacheMgr_RemoveCachePage(pCacheMgr, pPage);
	}

	// create and cache new page!

	pIndexNodeData = (axl_log_TIndexNodeData*) (pIndexNode + 1);

	pPage = axl_mem_Allocate(sizeof(axl_log_TCachePage));
	axl_log_TCachePage_Construct(pPage);
	axl_rtl_TList_InsertTail(&pCacheMgr->m_Cache, (axl_rtl_TListEntry*) pPage);
	
	pCacheMapNode = axl_rtl_TBinTree_Goto(&pCacheMgr->m_CacheMap, pIndexNode->m_Offset);
	ASSERT(pCacheMapNode->m_Value == 0);
	pCacheMapNode->m_Value = (intptr_t) pPage;

	pPage->m_pCacheMapNode = pCacheMapNode;
	pPage->m_MessageOffset = pIndexNodeData->m_MessageOffset;
	pPage->m_PreferredDataConfig = pIndexNodeData->m_PreferredDataConfig;

	axl_log_TCacheMgr_CachePage(pCacheMgr, pPage, pIndexNode);
	return pPage;
}

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_FindCachePageByLine(
	axl_log_TCacheMgr* pCacheMgr,
	ulong_t Line,
	ulong_t* pStartLine
	)
{
	axl_log_TIndexFile* pIndexFile = &pCacheMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pIndexNode;
	ulong_t StartLine;

	pIndexNode = axl_log_TIndexFile_FindLeafByLine(pIndexFile, Line, &StartLine);
	if (!pIndexNode)
		return NULL;

	if (pStartLine)
		*pStartLine = StartLine;

	return axl_log_TCacheMgr_FindCachePageByIndexNode(pCacheMgr, pIndexNode);
}

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_GetCachePageByLine(
	axl_log_TCacheMgr* pCacheMgr,
	ulong_t Line,
	ulong_t* pStartLine
	)
{
	axl_log_TIndexFile* pIndexFile = &pCacheMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pIndexNode;
	ulong_t StartLine;

	pIndexNode = axl_log_TIndexFile_FindLeafByLine(pIndexFile, Line, &StartLine);
	if (!pIndexNode)
		return NULL;

	if (pStartLine)
		*pStartLine = StartLine;

	return axl_log_TCacheMgr_GetCachePageByIndexNode(pCacheMgr, pIndexNode);
}

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_GetNextCachePage(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage
	)
{
	axl_log_TIndexFile* pIndexFile = &pCacheMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pIndexNode = axl_log_TCacheMgr_GetCachePageIndexNode(pCacheMgr, pPage);

	pIndexNode = axl_log_TIndexFile_GetRightNode(pIndexFile, pIndexNode);
	if (!pIndexNode)
		return NULL;

	return axl_log_TCacheMgr_GetCachePageByIndexNode(pCacheMgr, pIndexNode);
}

axl_log_TCachePage* 
AXL_API
axl_log_TCacheMgr_GetPrevCachePage(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage
	)
{
	axl_log_TIndexFile* pIndexFile = &pCacheMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pIndexNode = axl_log_TCacheMgr_GetCachePageIndexNode(pCacheMgr, pPage);
	
	pIndexNode = axl_log_TIndexFile_GetLeftNode(pIndexFile, pIndexNode);
	if (!pIndexNode)
		return NULL;

	return axl_log_TCacheMgr_GetCachePageByIndexNode(pCacheMgr, pIndexNode);
}

axl_log_TLine*
AXL_API
axl_log_TCacheMgr_FindLine(
	axl_log_TCacheMgr* pCacheMgr,
	ulong_t Line
	)
{
	axl_log_TCachePage* pPage;
	ulong_t StartLine;

	pPage = axl_log_TCacheMgr_FindCachePageByLine(pCacheMgr, Line, &StartLine); 
	if (!pPage)
		return NULL;

	return axl_log_TCachePage_GetLine(pPage, Line - StartLine);
}

axl_log_TLine*
AXL_API
axl_log_TCacheMgr_GetLine(
	axl_log_TCacheMgr* pCacheMgr,
	ulong_t Line
	)
{
	axl_log_TCachePage* pPage;
	ulong_t StartLine;

	pPage = axl_log_TCacheMgr_GetCachePageByLine(pCacheMgr, Line, &StartLine); 
	if (!pPage)
		return NULL;

	return axl_log_TCachePage_GetLine(pPage, Line - StartLine);
}

axl_log_TLine*
AXL_API
axl_log_TCacheMgr_GetLineIncrement(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine,
	size_t Delta
	)
{
	axl_log_TCachePage* pPage = pLine->m_pPage;
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);

	if (pLine->m_PageIndex + Delta < LineCount)
		return axl_log_TCachePage_GetLine(pPage, pLine->m_PageIndex + Delta);

	Delta -= LineCount - pLine->m_PageIndex;

	for (;;)
	{
		pPage = axl_log_TCacheMgr_GetNextCachePage(pCacheMgr, pPage);
		if (!pPage)
			break;

		LineCount = (ulong_t) axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
		if (Delta < LineCount)
			return axl_log_TCachePage_GetLine(pPage, Delta);

		Delta -= LineCount;
	}

	return NULL;
}

axl_log_TLine*
AXL_API
axl_log_TCacheMgr_GetLineDecrement(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TLine* pLine,
	size_t Delta
	)
{
	axl_log_TCachePage* pPage = pLine->m_pPage;
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);

	if (Delta <= pLine->m_PageIndex)
		return axl_log_TCachePage_GetLine(pPage, pLine->m_PageIndex - Delta);

	Delta -= pLine->m_PageIndex;

	for (;;)
	{
		pPage = axl_log_TCacheMgr_GetPrevCachePage(pCacheMgr, pPage);
		if (!pPage)
			break;

		LineCount = (ulong_t) axl_rtl_TPtrArray_GetCount(&pPage->m_LineArray);
		if (Delta <= LineCount)
			return axl_log_TCachePage_GetLine(pPage, LineCount - Delta);

		Delta -= LineCount;
	}

	return NULL;
}

void
AXL_API
axl_log_TCacheMgr_ReRepresentMessage(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage,
	axl_log_TCacheVolatileMessage* pVolatileMsg,
	axl_log_TReRepresent* pReRepresent
	)
{
	axl_log_TMessageFile* pMessageFile = pCacheMgr->m_pLogCtrl->m_pMessageFile;
	axl_log_TIndexFile* pIndexFile = &pCacheMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexMgr* pIndexMgr = &pCacheMgr->m_pLogCtrl->m_IndexMgr;
	axl_log_TRepresentor* pRepresentor = pCacheMgr->m_pLogCtrl->m_pRepresentor;

	axl_log_TMessage* pMsg;
	axl_log_TIndexNode* pNode;
	axl_log_TIndexNodeData* pNodeData;
	axl_log_TVolatileMessageData* pVolatileMsgData;

	ulong_t VolatileFlags;
	size_t AnchorLine;
	size_t OldLineCount;
	size_t NewLineCount;
	long LineDelta;

	// get message from message file

	pMsg = axl_log_TMessageFile_GetMessage(pMessageFile, pVolatileMsg->m_Offset);
	ASSERT(!(pMsg->m_Code & axl_log_EMessageCode_Mergeable)); 

	// get index node data from index file

	pNode = axl_log_TCacheMgr_GetCachePageIndexNode(pCacheMgr, pPage);
	pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
	pVolatileMsgData = (axl_log_TVolatileMessageData*) (pNodeData + 1) + pVolatileMsg->m_VolatileIndex;

	ASSERT(pVolatileMsg->m_VolatileIndex < pNodeData->m_VolatileMessageCount);
	ASSERT(pVolatileMsg->m_LineCount == pVolatileMsgData->m_LineCount);

	// prepare page

	AnchorLine = axl_log_TCachePage_GetVolatileMessageAnchorLine(pPage, pVolatileMsg);
	OldLineCount = pVolatileMsg->m_LineCount;
	VolatileFlags = pVolatileMsgData->m_VolatileFlags;
	
	NewLineCount = axl_log_TIndexMgr_CalcMessageLineCount(pIndexMgr, pMsg, &pPage->m_PreferredDataConfig, VolatileFlags);

	pVolatileMsgData->m_LineCount = (ulong_t) NewLineCount;

	LineDelta = (long) (NewLineCount - OldLineCount);

	axl_log_TCachePage_FreeLines(pPage, AnchorLine, OldLineCount);
	axl_log_TCachePage_AddLines(pPage, AnchorLine, LineDelta);
	axl_log_TIndexFile_AddLeafLines(pIndexFile, pNode, LineDelta);

	// re-cache message

	pVolatileMsg->m_VolatileFlags = VolatileFlags;
	pVolatileMsg->m_pFirstLine = NULL;
	pVolatileMsg->m_LineCount = 0;

	if (NewLineCount)
	{
		axl_log_TCacheSink CacheSink;
		axl_log_TCacheSink_Construct(&CacheSink);

		CacheSink.m_IsFirstLineOfMessage = true;
		CacheSink.m_MessageOffset = pVolatileMsg->m_Offset;
		CacheSink.m_Timestamp = pVolatileMsg->m_Timestamp;
		CacheSink.m_pPage = pPage;
		CacheSink.m_pVolatileMessage = pVolatileMsg;
		CacheSink.m_Line = AnchorLine;
		CacheSink.m_EndLine = AnchorLine + NewLineCount;
		CacheSink.m_PartCode = pMsg->m_Code;
		CacheSink.m_DataConfig = pPage->m_PreferredDataConfig;

		axl_log_TRepresentor_Represent(
			pRepresentor,
			(axl_log_TRepresentSink*) &CacheSink,
			pMsg->m_Code,
			pMsg + 1,
			pMsg->m_DataSize,
			VolatileFlags
			);

		if (CacheSink.m_LongestTextLine > pCacheMgr->m_LongestTextLine)
			pCacheMgr->m_LongestTextLine = CacheSink.m_LongestTextLine;

		if (CacheSink.m_LongestBinHexLine > pCacheMgr->m_LongestBinHexLine)
			pCacheMgr->m_LongestBinHexLine = CacheSink.m_LongestBinHexLine;

		if (CacheSink.m_LongestBinTextLine > pCacheMgr->m_LongestBinTextLine)
			pCacheMgr->m_LongestBinTextLine = CacheSink.m_LongestBinTextLine;

		axl_log_TCacheSink_Destruct(&CacheSink);
	}

	ASSERT(pVolatileMsg->m_LineCount == NewLineCount);

	// fill the results

	pReRepresent->m_AnchorAbsoluteLine = axl_log_TCacheMgr_GetCachePageStartLine(pCacheMgr, pPage) + (ulong_t) AnchorLine;
	pReRepresent->m_AnchorPageLine = (ulong_t) AnchorLine;
	pReRepresent->m_OldLineCount = (ulong_t) OldLineCount;
	pReRepresent->m_NewLineCount = (ulong_t) NewLineCount;	
}

bool_t
AXL_API
axl_log_TCacheMgr_ModifyMessageVolatileFlags(
	axl_log_TCacheMgr* pCacheMgr,
	axl_log_TCachePage* pPage,
	axl_log_TCacheVolatileMessage* pVolatileMsg,
	ulong_t VolatileFlags,
	axl_log_TReRepresent* pReRepresent
	)
{
	axl_log_TIndexNode* pNode = axl_log_TCacheMgr_GetCachePageIndexNode(pCacheMgr, pPage);
	axl_log_TIndexNodeData* pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
	axl_log_TVolatileMessageData* pVolatileMsgData = (axl_log_TVolatileMessageData*) (pNodeData + 1) + pVolatileMsg->m_VolatileIndex;

	if (pVolatileMsg->m_VolatileFlags == VolatileFlags && 
		pVolatileMsgData->m_VolatileFlags == VolatileFlags)
		return false;

	pVolatileMsgData->m_VolatileFlags = VolatileFlags;
	axl_log_TCacheMgr_ReRepresentMessage(pCacheMgr, pPage, pVolatileMsg, pReRepresent);
	return true;
}

//.............................................................................
