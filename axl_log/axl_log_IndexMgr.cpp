#include "axl_Precomp.h"
#include "axl_log_IndexMgr.h"
#include "axl_log_IndexSink.h"
#include "axl_log_LogCtrl.h"

//.............................................................................

void 
AXL_API
axl_log_TIndexMgr_Construct(
	axl_log_TIndexMgr* pIndexMgr,
	axl_log_TLogCtrl* pLogCtrl
	)
{
	pIndexMgr->m_PreferredDataConfig = g_axl_log_TDataConfig_Default;
	pIndexMgr->m_pLogCtrl = pLogCtrl;
	pIndexMgr->m_LineCountLimit = axl_log_EDef_IndexLineCountLimit;
	pIndexMgr->m_VolatileMessageCountLimit = axl_log_EDef_IndexVolatileMessageCountLimit;
	axl_rtl_TArray_Construct(&pIndexMgr->m_VolatileMessageDataBuffer);
}

void 
AXL_API
axl_log_TIndexMgr_Destruct(axl_log_TIndexMgr* pIndexMgr)
{
	axl_rtl_TArray_Destruct(&pIndexMgr->m_VolatileMessageDataBuffer);
}

axl_log_TIndexNode*
AXL_API
axl_log_TIndexMgr_AddIndexRootNode(
	axl_log_TIndexMgr* pIndexMgr,
	const axl_log_TDataConfig* pConfig
	)
{
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pNode = axl_log_TIndexFile_AddLeaf(pIdxFile, NULL, sizeof(axl_log_TIndexNodeData));
	axl_log_TIndexNodeData* pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
	memset(pNodeData, 0, sizeof(axl_log_TIndexNodeData));
	pNodeData->m_PreferredDataConfig = *pConfig;
	return pNode;
}

axl_log_TIndexNode*
AXL_API
axl_log_TIndexMgr_AddIndexNode(
	axl_log_TIndexMgr* pIndexMgr,
	const axl_log_TDataConfig* pConfig
	)
{
	axl_log_TMessageFile* pMsgFile = pIndexMgr->m_pLogCtrl->m_pMessageFile;
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;

	axl_log_TIndexNode* pLastNode = axl_log_TIndexFile_GetTailLeaf(pIdxFile);
	axl_log_TIndexNodeData LastNodeData = *(axl_log_TIndexNodeData*) (pLastNode + 1); // have to save it! (adding new node might unmap last node)

	axl_log_TIndexNode* pNewNode = axl_log_TIndexFile_AddLeaf(pIdxFile, NULL, sizeof(axl_log_TIndexNodeData));
	axl_log_TIndexNodeData* pNewNodeData = (axl_log_TIndexNodeData*) (pNewNode + 1);
	
	memset(pNewNodeData, 0, sizeof(axl_log_TIndexNodeData));

	pNewNodeData->m_MessageOffset = pIdxFile->m_pHdr->m_MessageSize;
	pNewNodeData->m_Col = LastNodeData.m_Col;
	pNewNodeData->m_BinOffset = LastNodeData.m_BinOffset;
	pNewNodeData->m_PartIndex = LastNodeData.m_PartIndex;
	pNewNodeData->m_MergeId = LastNodeData.m_MergeId;
	pNewNodeData->m_MergeCriteria = LastNodeData.m_MergeCriteria;
	pNewNodeData->m_PreferredDataConfig = pConfig ? *pConfig : LastNodeData.m_PreferredDataConfig;

	return pNewNode;
}

void
AXL_API
axl_log_TIndexMgr_FinalizeIndexNode(
	axl_log_TIndexMgr* pIndexMgr,
	axl_log_TIndexNode* pNode,
	axl_log_TIndexSink* pSink,
	size_t MessageCount
	)
{
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNodeData* pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
	
	size_t VolatileMessageCount = axl_rtl_TArray_GetCount(&pIndexMgr->m_VolatileMessageDataBuffer);

	// finalize last index node

	pNodeData->m_Col = pSink->m_Col;
	pNodeData->m_BinOffset = pSink->m_BinOffset;
	pNodeData->m_PartIndex = pSink->m_PartIndex;
	pNodeData->m_MergeId = pSink->m_MergeId;
	pNodeData->m_MergeCriteria = pSink->m_MergeCriteria;
	pNodeData->m_VolatileMessageCount += (ulong_t) VolatileMessageCount;
	pNodeData->m_MessageCount += (ulong_t) MessageCount;

	if (VolatileMessageCount)
		axl_log_TIndexFile_AddTailLeafData(
			pIdxFile, 
			pIndexMgr->m_VolatileMessageDataBuffer.m_p, 
			VolatileMessageCount * sizeof(axl_log_TVolatileMessageData)
			);

	if (pSink->m_LineCount > pNode->m_LineCount)
		axl_log_TIndexFile_AddLeafLines(pIdxFile, pNode, pSink->m_LineCount - pNode->m_LineCount);

	axl_rtl_TArray_Clear(&pIndexMgr->m_VolatileMessageDataBuffer);
}

axl_log_TDataConfig* 
AXL_API
axl_log_TIndexMgr_GetPreferredDataConfig(axl_log_TIndexMgr* pIndexMgr)
{
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pNode = axl_log_TIndexFile_GetTailLeaf(pIdxFile);
	axl_log_TIndexNodeData* pNodeData;

	if (!pNode)
		return &pIndexMgr->m_PreferredDataConfig;

	pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
	return &pNodeData->m_PreferredDataConfig;
}

void
AXL_API
axl_log_TIndexMgr_SetPreferredDataConfig(
	axl_log_TIndexMgr* pIndexMgr,
	const axl_log_TDataConfig* pConfig
	)
{
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TIndexNode* pNode = axl_log_TIndexFile_GetTailLeaf(pIdxFile);
	axl_log_TIndexNodeData* pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);

	pIndexMgr->m_PreferredDataConfig = *pConfig;

	if (!pNode)
		return;

	if (axl_log_TDataConfig_Compare(pConfig, &pNodeData->m_PreferredDataConfig) == 0)
		return;

	if (pNode->m_LineCount == 0) // node is empty! can simply change data config
		pNodeData->m_PreferredDataConfig = *pConfig;
	else
		axl_log_TIndexMgr_AddIndexNode(pIndexMgr, pConfig);
}

void
AXL_API
axl_log_TIndexMgr_ClearIndex(axl_log_TIndexMgr* pIndexMgr)
{
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;
	axl_log_TDataConfig Config = *axl_log_TIndexMgr_GetPreferredDataConfig(pIndexMgr);
	axl_log_TIndexFile_Clear(pIdxFile);
	axl_log_TIndexMgr_AddIndexRootNode(pIndexMgr, &Config);
}

bool_t
AXL_API
axl_log_TIndexMgr_UpdateLastMessage(axl_log_TIndexMgr* pIndexMgr)
{
	axl_log_TFilter* pFilter = pIndexMgr->m_pLogCtrl->m_pFilter;
	axl_log_TMessageFile* pMsgFile = pIndexMgr->m_pLogCtrl->m_pMessageFile;
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;

	axl_log_TMessage* pMsg;
	axl_log_TIndexNode* pNode = axl_log_TIndexFile_GetTailLeaf(pIdxFile);
	axl_log_TIndexNodeData* pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
	axl_log_TVolatileMessageData* pVolatileMessageData;

	size_t OldLineCount;
	size_t NewLineCount;
	long LineDelta;
	ulong_t VolatileFlags;

	if (!pIdxFile->m_pHdr->m_MessageCount ||
		!pNodeData->m_VolatileMessageCount)
		return false;

	pVolatileMessageData = (axl_log_TVolatileMessageData*) (pNodeData + 1) + pNodeData->m_VolatileMessageCount - 1;

	pMsg = axl_log_TMessageFile_GetMessage(pMsgFile, pVolatileMessageData->m_Offset);
	if (!pMsg || (pMsg->m_Code & axl_log_EMessageCode_Mergeable))
		return false;

	VolatileFlags = pVolatileMessageData->m_VolatileFlags;
	OldLineCount = pVolatileMessageData->m_LineCount;

	if (pFilter)
		axl_log_TFilter_Filter(pFilter, pMsg->m_Code, pMsg + 1, pMsg->m_DataSize, &VolatileFlags);

	NewLineCount = axl_log_TIndexMgr_CalcMessageLineCount(pIndexMgr, pMsg, &pNodeData->m_PreferredDataConfig, VolatileFlags);

	pVolatileMessageData->m_VolatileFlags = VolatileFlags;
	pVolatileMessageData->m_LineCount = (ulong_t) NewLineCount;
	LineDelta = (long) (NewLineCount - OldLineCount);

	axl_log_TIndexFile_AddLeafLines(pIdxFile, pNode, LineDelta);
	return true;
}

size_t
AXL_API
axl_log_TIndexMgr_IncrementalUpdateIndex(axl_log_TIndexMgr* pIndexMgr)
{
	axl_log_TRepresentor* pRepresentor = pIndexMgr->m_pLogCtrl->m_pRepresentor;
	axl_log_TFilter* pFilter = pIndexMgr->m_pLogCtrl->m_pFilter;
	
	axl_log_TMessageFile* pMsgFile = pIndexMgr->m_pLogCtrl->m_pMessageFile;
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;

	axl_log_TIndexNode* pNode = axl_log_TIndexFile_GetTailLeaf(pIdxFile);
	axl_log_TIndexNodeData* pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
	axl_log_TDataConfig PreferredDataConfig;
	
	ulong_t Offset = pIdxFile->m_pHdr->m_MessageSize;
	ulong_t MessageEnd = pMsgFile->m_pHdr->m_MessageSize;

	size_t Anchor = pIdxFile->m_pHdr->m_MessageSize;
	size_t MessageCount = 0;

	axl_log_TIndexSink IndexSink;

	ASSERT(pRepresentor);
	ASSERT(pIdxFile->m_pHdr->m_ClearCount == pMsgFile->m_pHdr->m_ClearCount);

	axl_log_TIndexSink_Construct(&IndexSink);
	IndexSink.m_LineCount = pNode->m_LineCount;
	IndexSink.m_Col = pNodeData->m_Col;
	IndexSink.m_BinOffset = pNodeData->m_BinOffset;
	IndexSink.m_PartIndex = pNodeData->m_PartIndex;
	IndexSink.m_MergeId = pNodeData->m_MergeId;
	IndexSink.m_MergeCriteria = pNodeData->m_MergeCriteria;

	PreferredDataConfig = pNodeData->m_PreferredDataConfig;

	axl_rtl_TArray_Clear(&pIndexMgr->m_VolatileMessageDataBuffer);

	while (Offset < MessageEnd)
	{
		axl_log_TMessage* pMsg = axl_log_TMessageFile_GetMessage(pMsgFile, Offset);

		IndexSink.m_PartCode = pMsg->m_Code;
		IndexSink.m_Timestamp = pMsg->m_Timestamp;
		IndexSink.m_DataConfig = PreferredDataConfig;

		if (pMsg->m_Code & axl_log_EMessageCode_Mergeable)
		{
			axl_log_TIndexSink IndexSinkSnapshot = IndexSink;

			IndexSink.m_Merge = axl_log_EMerge_Normal;

			axl_log_TRepresentor_Represent(
				pRepresentor, 
				(axl_log_TRepresentSink*) &IndexSink, 
				pMsg->m_Code, 
				pMsg + 1,
				pMsg->m_DataSize,
				0
				);

			// first add node (if needed)...

			if (IndexSinkSnapshot.m_LineCount >= pIndexMgr->m_LineCountLimit && 
				IndexSink.m_LineCount > IndexSinkSnapshot.m_LineCount)
			{
				axl_log_TIndexMgr_FinalizeIndexNode(pIndexMgr, pNode, &IndexSinkSnapshot, MessageCount);

				pNode = axl_log_TIndexMgr_AddIndexNode(pIndexMgr, NULL);
				pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);

				IndexSink.m_LineCount -= IndexSinkSnapshot.m_LineCount;
				MessageCount = 0;
			}

			// ...then advance

			MessageCount++;

			pIdxFile->m_pHdr->m_LastMessageOffset = Offset;

			Offset += sizeof(axl_log_TMessage) + pMsg->m_DataSize;

			pIdxFile->m_pHdr->m_MessageSize = Offset;
			pIdxFile->m_pHdr->m_MessageCount++;
		}
		else // volatile
		{	
			axl_log_TVolatileMessageData VolatileMessageData;

			size_t VolatileMessageCount;
			ulong_t VolatileFlags = 0;
			ulong_t OldLineCount = IndexSink.m_LineCount;

			if (pFilter)
				axl_log_TFilter_Filter(pFilter, pMsg->m_Code, pMsg + 1, pMsg->m_DataSize, &VolatileFlags);

			IndexSink.m_Merge = 0;
			axl_log_TMergeCriteria_Clear(&IndexSink.m_MergeCriteria);

			axl_log_TRepresentor_Represent(
				pRepresentor, 
				(axl_log_TRepresentSink*) &IndexSink, 
				pMsg->m_Code, 
				pMsg + 1,
				pMsg->m_DataSize,
				VolatileFlags
				);

			VolatileMessageData.m_VolatileFlags = VolatileFlags;
			VolatileMessageData.m_Offset = Offset;
			VolatileMessageData.m_LineCount = IndexSink.m_LineCount - OldLineCount;
			axl_rtl_TArray_Append(&pIndexMgr->m_VolatileMessageDataBuffer, &VolatileMessageData, 1, sizeof(axl_log_TVolatileMessageData));

			axl_log_TMergeCriteria_Clear(&IndexSink.m_MergeCriteria);

			// first advance...

			MessageCount++;

			pIdxFile->m_pHdr->m_LastMessageOffset = Offset;

			Offset += sizeof(axl_log_TMessage) + pMsg->m_DataSize;

			pIdxFile->m_pHdr->m_MessageSize = Offset;
			pIdxFile->m_pHdr->m_MessageCount++;

			VolatileMessageCount = axl_rtl_TArray_GetCount(&pIndexMgr->m_VolatileMessageDataBuffer);

			// then add new empty node
			
			if (IndexSink.m_LineCount >= pIndexMgr->m_LineCountLimit || 
				VolatileMessageCount >= pIndexMgr->m_VolatileMessageCountLimit)
			{
				axl_log_TIndexMgr_FinalizeIndexNode(pIndexMgr, pNode, &IndexSink, MessageCount);

				pNode = axl_log_TIndexMgr_AddIndexNode(pIndexMgr, NULL);
				pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);

				IndexSink.m_LineCount = 0;
				MessageCount = 0;
			}
		}
	}

	axl_log_TIndexMgr_FinalizeIndexNode(pIndexMgr, pNode, &IndexSink, MessageCount);

	axl_log_TIndexSink_Destruct(&IndexSink);

	return Anchor;
}

size_t
AXL_API
axl_log_TIndexMgr_UpdateIndex(axl_log_TIndexMgr* pIndexMgr)
{
	axl_log_TMessageFile* pMsgFile = pIndexMgr->m_pLogCtrl->m_pMessageFile;
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;
	
	if (pIdxFile->m_pHdr->m_ClearCount != pMsgFile->m_pHdr->m_ClearCount) 
	{
		pIdxFile->m_pHdr->m_ClearCount = pMsgFile->m_pHdr->m_ClearCount;
		axl_log_TIndexMgr_ClearIndex(pIndexMgr);
	} 
	
	return axl_log_TIndexMgr_IncrementalUpdateIndex(pIndexMgr);
}

size_t
AXL_API
axl_log_TIndexMgr_ReFilterAll(axl_log_TIndexMgr* pIndexMgr)
{
	size_t ChangeCount = 0;

	axl_log_TFilter* pFilter = pIndexMgr->m_pLogCtrl->m_pFilter;
	axl_log_TMessageFile* pMsgFile = pIndexMgr->m_pLogCtrl->m_pMessageFile;
	axl_log_TIndexFile* pIdxFile = &pIndexMgr->m_pLogCtrl->m_IndexFile;

	axl_log_TIndexNode* pNode = axl_log_TIndexFile_GetHeadLeaf(pIdxFile);

	if (!pFilter)
		return 0;

	for (; pNode; pNode = axl_log_TIndexFile_GetRightNode(pIdxFile, pNode))
	{
		long NodeLineDelta = 0;

		axl_log_TIndexNodeData* pNodeData = (axl_log_TIndexNodeData*) (pNode + 1);
		axl_log_TVolatileMessageData* pVolatileMessageData = (axl_log_TVolatileMessageData*) (pNodeData + 1);
		axl_log_TVolatileMessageData* pVolatileMessageDataEnd = pVolatileMessageData + pNodeData->m_VolatileMessageCount;

		ASSERT(!pNode->m_Depth); // must be a leaf

		for (; pVolatileMessageData < pVolatileMessageDataEnd; pVolatileMessageData++)
		{
			ulong_t VolatileFlags = pVolatileMessageData->m_VolatileFlags;

			axl_log_TMessage* pMsg = axl_log_TMessageFile_GetMessage(pMsgFile, pVolatileMessageData->m_Offset);
			
			if (pMsg->m_Code & axl_log_EMessageCode_Mergeable)
			{
				ASSERT(false);
				continue;
			}
			
			axl_log_TFilter_Filter(pFilter, pMsg->m_Code, pMsg + 1, pMsg->m_DataSize, &VolatileFlags);

			if (pVolatileMessageData->m_VolatileFlags != VolatileFlags)
			{
				size_t NewLineCount = axl_log_TIndexMgr_CalcMessageLineCount(pIndexMgr, pMsg, &pNodeData->m_PreferredDataConfig, VolatileFlags);
				NodeLineDelta += (long) (NewLineCount - pVolatileMessageData->m_LineCount);
				
				pVolatileMessageData->m_VolatileFlags = VolatileFlags;
				pVolatileMessageData->m_LineCount = (ulong_t) NewLineCount;
				ChangeCount++;
			}			
		}

		axl_log_TIndexFile_AddLeafLines(pIdxFile, pNode, NodeLineDelta);
	}
	
	return ChangeCount;
}

size_t
AXL_API
axl_log_TIndexMgr_CalcMessageLineCount(
	axl_log_TIndexMgr* pIndexMgr,
	axl_log_TMessage* pMsg,
	axl_log_TDataConfig* pPreferredDataConfig,
	ulong_t VolatileFlags
	)
{
	axl_log_TRepresentor* pRepresentor = pIndexMgr->m_pLogCtrl->m_pRepresentor;

	size_t LineCount;
	axl_log_TIndexSink IndexSink;

	axl_log_TIndexSink_Construct(&IndexSink);
	IndexSink.m_DataConfig = *pPreferredDataConfig;
	
	axl_log_TRepresentor_Represent(
		pRepresentor, 
		(axl_log_TRepresentSink*) &IndexSink,
		pMsg->m_Code, 
		pMsg + 1,
		pMsg->m_DataSize,
		VolatileFlags
		);

	LineCount = IndexSink.m_LineCount;
	axl_log_TIndexSink_Destruct(&IndexSink);

	return LineCount;
}

//.............................................................................
