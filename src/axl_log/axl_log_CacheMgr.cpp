#include "pch.h"
#include "axl_log_CacheMgr.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

CCacheMgr::CCacheMgr ()
{
	m_pServer = NULL;
	m_CachePageCountLimit = 32;
	m_SyncId = -1;
}

bool
CCacheMgr::Create (
	CWidget* pWidget,
	IServer* pServer,
	const char* pIndexFileName,
	const char* pColorFileName
	)
{
	bool Result = m_IndexFile.Open (pIndexFileName, io::EFileFlag_DeleteOnClose);
	if (!Result)
		return false;

	m_pWidget = pWidget;
	m_pServer = pServer;

	ClearCache (-1);
	return true;
}

void 
CCacheMgr::ClearCache (uint64_t SyncId)
{
	m_IndexFile.Clear ();
	m_CachePageList.Clear ();
	m_CachePageMap.Clear ();
	m_LongestLineLength.Clear ();

	m_SyncId = SyncId;

	m_pWidget->SetLineCount (0);
	m_pWidget->Redraw ();
}

CCachePage* 
CCacheMgr::GetOrCreateCachePage (const TIndexLeaf* pLeaf)
{
	rtl::CHashTableMapIteratorT <uint64_t, CCachePage*> It = m_CachePageMap.Goto (pLeaf->m_Offset);
	if (It->m_Value)
		return It->m_Value;

	if (m_CachePageList.GetCount () > m_CachePageCountLimit)
	{
		CCachePage* pOldPage = m_CachePageList.RemoveHead ();
		m_CachePageMap.DeleteByKey (pOldPage->m_IndexLeaf.m_Offset);
		AXL_MEM_DELETE (pOldPage);
	}

	CCachePage* pPage = AXL_MEM_NEW (CCachePage);
	pPage->m_pCacheMgr = this;
	pPage->m_IndexLeaf = *pLeaf;
	m_CachePageList.InsertTail (pPage);
	It->m_Value = pPage;
	return pPage;
}

CCachePage* 
CCacheMgr::GetCachePageByIndexLeaf (const TIndexLeaf* pLeaf)
{
	CCachePage* pPage = GetOrCreateCachePage (pLeaf);
	if (pPage->m_Flags & EPageFlag_Represented)
		return pPage;

	if (pPage->m_Flags & EPageFlag_Representing)
		return !pPage->IsEmpty () ? pPage : NULL;

	pPage->m_Flags |= EPageFlag_Representing;

	m_pServer->RepresentPage (m_SyncId, pLeaf, (uint64_t*) (pLeaf + 1));

	return !pPage->IsEmpty () ? pPage : NULL;
}

CCachePage* 
CCacheMgr::GetCachePageByLineIdx (
	uint64_t LineIdx,
	uint64_t* pStartLineIdx
	)
{
	const TIndexLeaf* pIndexLeaf = m_IndexFile.FindLeaf (LineIdx, pStartLineIdx);
	if (!pIndexLeaf)
		return NULL;

	return GetCachePageByIndexLeaf (pIndexLeaf);
}

uint64_t
CCacheMgr::GetCachePageStartLineIdx (CCachePage* pPage)
{
	const TIndexLeaf* pIndexLeaf = m_IndexFile.GetLeaf (pPage->m_IndexLeaf.m_Offset);
	return m_IndexFile.GetLeafStartLine (pIndexLeaf);
}

CCachePage* 
CCacheMgr::GetNextCachePage (CCachePage* pPage)
{
	const TIndexLeaf* pIndexLeaf = m_IndexFile.GetLeaf (pPage->m_IndexLeaf.m_Offset);
	const TIndexLeaf* pNextIndexLeaf = m_IndexFile.GetNextLeaf (pIndexLeaf);

	return pNextIndexLeaf ? GetCachePageByIndexLeaf (pNextIndexLeaf) : NULL;
}

CCachePage* 
CCacheMgr::GetPrevCachePage (CCachePage* pPage)
{
	const TIndexLeaf* pIndexLeaf = m_IndexFile.GetLeaf (pPage->m_IndexLeaf.m_Offset);
	const TIndexLeaf* pPrevIndexLeaf = m_IndexFile.GetPrevLeaf (pIndexLeaf);

	return pPrevIndexLeaf ? GetCachePageByIndexLeaf (pPrevIndexLeaf) : NULL;
}

CLine* 
CCacheMgr::GetLine (uint64_t LineIdx)
{
	uint64_t StartLineIdx;

	CCachePage* pPage = GetCachePageByLineIdx (LineIdx, &StartLineIdx); 
	if (!pPage)
		return NULL;

	ASSERT (LineIdx >= StartLineIdx);
	return pPage->m_LineArray [LineIdx - StartLineIdx];
}

CLine* 
CCacheMgr::GetNextLine (CLine* pLine)
{
	size_t PageLineCount = pLine->m_pPage->m_LineArray.GetCount ();

	if (pLine->m_LineIdx + 1 < PageLineCount)
		return pLine->m_pPage->m_LineArray [pLine->m_LineIdx + 1];

	CCachePage* pNextPage = GetNextCachePage (pLine->m_pPage);
	return pNextPage && !pNextPage->IsEmpty () ? pNextPage->m_LineArray [0] : NULL;
}

CLine* 
CCacheMgr::GetPrevLine (CLine* pLine)
{
	if (pLine->m_LineIdx > 0)
		return pLine->m_pPage->m_LineArray [pLine->m_LineIdx - 1];

	CCachePage* pPrevPage = GetPrevCachePage (pLine->m_pPage);
	return pPrevPage && !pPrevPage->IsEmpty () ? pPrevPage->m_LineArray.GetBack () : NULL;
}

void 
CCacheMgr::CreateIndexLeaf (
	const TBinDataConfig* pBinDataConfig,
	uint64_t LineCount,
	uint64_t PacketOffset,
	uint32_t PacketCount,
	uint32_t FoldablePacketCount,
	uint64_t MergePointIdx,
	uint64_t FoldFlags
	)
{
	m_IndexFile.AddLeaf (
		pBinDataConfig,
		PacketOffset,
		MergePointIdx
		);

	UpdateIndexTailLeaf (LineCount, PacketCount, FoldablePacketCount, FoldFlags);
}

void 
CCacheMgr::UpdateIndexTailLeaf (
	uint64_t LineCount,
	uint32_t PacketCount,
	uint32_t FoldablePacketCount,
	uint64_t FoldFlags
	)
{
	const TIndexLeaf* pLeaf = m_IndexFile.GetTailLeaf ();

	if (PacketCount != pLeaf->m_PacketCount)
		m_IndexFile.AddTailLeafPackets (
			PacketCount - pLeaf->m_PacketCount, 
			FoldablePacketCount - pLeaf->m_FoldablePacketCount, 
			FoldFlags
			);
	
	if (pLeaf->m_LineCount != LineCount)
	{
		m_IndexFile.AddLeafLines (pLeaf, (intptr_t) (LineCount - pLeaf->m_LineCount));
		m_pWidget->SetLineCount ((size_t) m_IndexFile.GetLineCount ());
	}

	CCachePage* pPage = FindCachePageByIndexLeaf (pLeaf->m_Offset);
	if (pPage)
	{
		pPage->m_IndexLeaf = *pLeaf;
		pPage->m_Flags &= ~(EPageFlag_Represented | EPageFlag_Representing | EPageFlag_Colorized | EPageFlag_Colorizing);
	}
	
	uint64_t StartLineIdx = m_IndexFile.GetLeafStartLine (pLeaf);
	uint64_t EndLineIdx = StartLineIdx + pLeaf->m_LineCount;

	if (pPage && !pPage->IsEmpty ())
		StartLineIdx += pPage->m_LineArray.GetCount () - 1;

	m_pWidget->RedrawLineRange ((size_t) StartLineIdx, (size_t) EndLineIdx);
}

void 
CCacheMgr::RepresentPageComplete (
	uint64_t IndexLeafOffset,
	size_t LineIdx,
	size_t LineCount,
	size_t FoldablePacketIdx,
	const TFoldablePacket* pFoldablePacketArray,
	size_t FoldablePacketCount,
	const void* pLineBuffer,
	size_t LineBufferSize
	)
{
	CCachePage* pPage = FindCachePageByIndexLeaf (IndexLeafOffset);
	if (!pPage)
		return;
		
	pPage->m_LineArray.SetCount ((size_t) pPage->m_IndexLeaf.m_LineCount);
	pPage->m_FoldablePacketArray.SetCount (pPage->m_IndexLeaf.m_FoldablePacketCount);
	pPage->m_Flags &= ~EPageFlag_Representing;
	pPage->m_Flags |= EPageFlag_Represented;

	bool IsRetroColorized;
	pPage->LoadLines (LineIdx, LineCount, pLineBuffer, LineBufferSize, &m_LongestLineLength, &IsRetroColorized);

	// foldable packets

	ASSERT (FoldablePacketIdx + FoldablePacketCount <= pPage->m_IndexLeaf.m_FoldablePacketCount);
	memcpy (
		&pPage->m_FoldablePacketArray [FoldablePacketIdx], 
		pFoldablePacketArray, 
		FoldablePacketCount * sizeof (TFoldablePacket)
		);

	// redraw

	uint64_t StartLineIdx = GetCachePageStartLineIdx (pPage);
	uint64_t EndLineIdx = StartLineIdx + pPage->m_IndexLeaf.m_LineCount;

	StartLineIdx += LineIdx;

	if (IsRetroColorized && StartLineIdx)
		StartLineIdx--;

	m_pWidget->RecalcColCount (&m_LongestLineLength);
	m_pWidget->RedrawLineRange ((size_t) StartLineIdx, (size_t) EndLineIdx);
}

void 
CCacheMgr::FoldPacketComplete (
	uint64_t IndexLeafOffset,
	size_t LineIdx,
	size_t OldLineCount,
	size_t NewLineCount,
	size_t FoldablePacketIdx,
	uint64_t FoldFlags,
	const void* pLineBuffer,
	size_t LineBufferSize
	)
{
	CCachePage* pPage = FindCachePageByIndexLeaf (IndexLeafOffset);
	if (!pPage || FoldablePacketIdx >= pPage->m_IndexLeaf.m_FoldablePacketCount)
		return;

	// save foldable packet info

	TFoldablePacket* pFoldablePacket = &pPage->m_FoldablePacketArray [FoldablePacketIdx];
	ASSERT (pFoldablePacket->m_LineCount == OldLineCount);
	pFoldablePacket->m_LineCount = NewLineCount;
	pFoldablePacket->m_FoldFlags = FoldFlags;

	const TIndexLeaf* pLeaf = m_IndexFile.GetLeaf (pPage->m_IndexLeaf.m_Offset);
	((uint64_t*) (pLeaf + 1)) [FoldablePacketIdx] = FoldFlags;

	// add & load lines

	intptr_t LineCountDelta = NewLineCount - OldLineCount;
	if (LineCountDelta)
	{
		m_IndexFile.AddLeafLines (pLeaf, LineCountDelta);
		m_pWidget->SetLineCount ((size_t) m_IndexFile.GetLineCount ());

		pPage->AddLines (LineIdx, LineCountDelta);
	}
	
	pPage->LoadLines (LineIdx, NewLineCount, pLineBuffer, LineBufferSize, &m_LongestLineLength, NULL);

	// redraw

	uint64_t StartLineIdx = GetCachePageStartLineIdx (pPage);
	uint64_t EndLineIdx = LineCountDelta ? -1 : StartLineIdx + NewLineCount;

	m_pWidget->RecalcColCount (&m_LongestLineLength);
	m_pWidget->RedrawLineRange ((size_t) StartLineIdx, (size_t) EndLineIdx);
}

//.............................................................................

} // namespace log {
} // namespace axl {
