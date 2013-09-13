#include "pch.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

CPage* 
CWidget::FindCachePageByIndexNode (TIndexNode* pNode)
{
	rtl::CHashTableMapIteratorT <size_t, CPage*> It = m_CachePageMap.Find (pNode->m_Offset);
	return It ? It->m_Value : NULL;
}

CPage* 
CWidget::GetOrCreateCachePage (size_t IndexNodeOffset)
{
	rtl::CHashTableMapIteratorT <size_t, CPage*> It = m_CachePageMap.Goto (IndexNodeOffset);
	if (It->m_Value)
		return It->m_Value;

	if (m_CachePageList.GetCount () > m_CachePageCountLimit)
	{
		CPage* pOldPage = m_CachePageList.RemoveHead ();
		m_CachePageMap.DeleteByKey (pOldPage->m_IndexNodeOffset);
		AXL_MEM_DELETE (pOldPage);
	}

	CPage* pPage = AXL_MEM_NEW (CPage);
	pPage->m_IndexNodeOffset = IndexNodeOffset;
	m_CachePageList.InsertTail (pPage);
	It->m_Value = pPage;
	return pPage;
}

CPage* 
CWidget::GetCachePageByIndexNode (TIndexNode* pNode)
{
	CPage* pPage = GetOrCreateCachePage (pNode->m_Offset);
	if (pPage->m_Flags & EPageFlag_Represented)
		return pPage;

	if (pPage->m_Flags & EPageFlag_Representing)
		return !pPage->IsEmpty () ? pPage : NULL;

	pPage->m_Flags |= EPageFlag_Representing;

	TIndexLeaf* pLeaf = (TIndexLeaf*) (pNode + 1);
	TIndexVolatilePacket* pVolatilePacketArray = (TIndexVolatilePacket*) (pLeaf + 1);

	TIndexNode* pPrevNode = m_IndexFile.GetLeftNode (pNode);
	TIndexLeaf* pPrevLeaf = NULL;

	if (pPrevNode)
		pPrevLeaf = (TIndexLeaf*) (pPrevNode + 1);

	m_pServer->Represent (
		m_SyncId, 
		pNode->m_Offset, 
		pNode->m_LineCount, 0, 
		pLeaf, 
		pPrevLeaf,
		pVolatilePacketArray
		);

	return !pPage->IsEmpty () ? pPage : NULL;
}

CPage* 
CWidget::GetCachePageByLineIdx (
	size_t LineIdx,
	size_t* pStartLineIdx
	)
{
	if (LineIdx >= m_LineCount)
		return NULL;

	TIndexNode* pIndexNode = m_IndexFile.FindLeaf (LineIdx, pStartLineIdx);
	ASSERT (pIndexNode);

	return GetCachePageByIndexNode (pIndexNode);
}

size_t
CWidget::GetCachePageStartLineIdx (CPage* pPage)
{
	TIndexNode* pIndexNode = m_IndexFile.GetNode (pPage->m_IndexNodeOffset);
	return m_IndexFile.GetLeafStartLine (pIndexNode);
}

CPage* 
CWidget::GetNextCachePage (CPage* pPage)
{
	TIndexNode* pIndexNode = m_IndexFile.GetNode (pPage->m_IndexNodeOffset);
	TIndexNode* pNextIndexNode = m_IndexFile.GetRightNode (pIndexNode);

	return pNextIndexNode ? GetCachePageByIndexNode (pNextIndexNode) : NULL;
}

CPage* 
CWidget::GetPrevCachePage (CPage* pPage)
{
	TIndexNode* pIndexNode = m_IndexFile.GetNode (pPage->m_IndexNodeOffset);
	TIndexNode* pPrevIndexNode = m_IndexFile.GetLeftNode (pIndexNode);

	return pPrevIndexNode ? GetCachePageByIndexNode (pPrevIndexNode) : NULL;
}

CLine* 
CWidget::GetLine (size_t LineIdx)
{
	size_t StartLineIdx;

	CPage* pPage = GetCachePageByLineIdx (LineIdx, &StartLineIdx); 
	if (!pPage)
		return NULL;

	ASSERT (LineIdx >= StartLineIdx);
	return pPage->m_LineArray [LineIdx - StartLineIdx];
}

CLine* 
CWidget::GetNextLine (CLine* pLine)
{
	size_t PageLineCount = pLine->m_pPage->m_LineArray.GetCount ();

	if (pLine->m_LineIdx + 1 < PageLineCount)
		return pLine->m_pPage->m_LineArray [pLine->m_LineIdx + 1];

	CPage* pNextPage = GetNextCachePage (pLine->m_pPage);
	return pNextPage && !pNextPage->IsEmpty () ? pNextPage->m_LineArray [0] : NULL;
}

CLine* 
CWidget::GetPrevLine (CLine* pLine)
{
	if (pLine->m_LineIdx > 0)
		return pLine->m_pPage->m_LineArray [pLine->m_LineIdx - 1];

	CPage* pPrevPage = GetPrevCachePage (pLine->m_pPage);
	return pPrevPage && !pPrevPage->IsEmpty () ? pPrevPage->m_LineArray.GetBack () : NULL;
}

//.............................................................................

} // namespace log {
} // namespace axl {
