#include "pch.h"
#include "axl_log_Widget.h"

namespace axl {
namespace log {

//.............................................................................

void
CWidget::ProcessCliMsg (
	ECliMsg MsgKind,
	const void* p,
	size_t Size
	)
{
	switch (MsgKind)
	{
	case ECliMsg_ClearCache:
		if (Size >= sizeof (TCliMsg_ClearCache))
			ClearCache (((TCliMsg_ClearCache*) p)->m_SyncId);
		break;

	case ECliMsg_FilterProgress:
		if (Size >= sizeof (TCliMsg_Progress))
			FilterProgress (((TCliMsg_Progress*) p)->m_Percentage);
		break;

	case ECliMsg_IndexProgress:
		if (Size >= sizeof (TCliMsg_Progress))
			IndexProgress (((TCliMsg_Progress*) p)->m_Percentage);
		break;

	case ECliMsg_SetIndexTailLeaf:
		if (Size >= sizeof (TCliMsg_SetIndexTailLeaf))
		{
			TCliMsg_SetIndexTailLeaf* pMsg = (TCliMsg_SetIndexTailLeaf*) p;
			SetIndexTailLeaf (
				pMsg->m_IsNewLeaf != 0,
				pMsg->m_LineCount,
				&pMsg->m_Leaf,
				pMsg->m_Leaf.m_VolatilePacketCount ? (TIndexVolatilePacket*) (pMsg + 1) : NULL
				);
		}

		break;

	case ECliMsg_RepresentComplete:
		if (Size >= sizeof (TCliMsg_RepresentComplete))
		{
			TCliMsg_RepresentComplete* pMsg = (TCliMsg_RepresentComplete*) p;
			RepresentComplete (
				pMsg->m_SyncId,
				pMsg->m_IndexNodeOffset,
				pMsg->m_IndexNodeLineCount,
				pMsg->m_LineIdx,
				pMsg->m_LineDataSize ? pMsg + 1 : NULL,
				pMsg->m_LineDataSize
				);
		}

		break;

	case ECliMsg_ColorizeComplete:
		if (Size >= sizeof (TCliMsg_ColorizeComplete))
		{
			TCliMsg_ColorizeComplete* pMsg = (TCliMsg_ColorizeComplete*) p;
			ColorizeComplete (
				pMsg->m_SyncId,
				pMsg->m_IndexNodeOffset,
				pMsg->m_LineIdx,
				pMsg->m_LineOffset,
				pMsg->m_ColorizeDataSize ? pMsg + 1 : NULL,
				pMsg->m_ColorizeDataSize
				);
		}

		break;
	}
}

void 
CWidget::ClearCache (size_t SyncId)
{
	PostThreadMsg (ECliMsg_ClearCache, ref::CPtrT <void> ((void*) SyncId, (ref::IRefCount*) NULL));
}

void 
CWidget::FilterProgress (uint_t Percentage)
{
	PostThreadMsg (ECliMsg_FilterProgress, ref::CPtrT <void> ((void*) Percentage, (ref::IRefCount*) NULL));
}

void 
CWidget::IndexProgress (uint_t Percentage)
{
	PostThreadMsg (ECliMsg_IndexProgress, ref::CPtrT <void> ((void*) Percentage, (ref::IRefCount*) NULL));
}

void 
CWidget::SetIndexTailLeaf (
	bool IsNewLeaf,
	size_t LineCount,
	const TIndexLeaf* pLeaf,
	const TIndexVolatilePacket* pVolatilePacketArray
	)
{
	size_t VolatileSize = pLeaf->m_VolatilePacketCount * sizeof (TIndexVolatilePacket);

	ref::CPtrT <TCliMsg_SetIndexTailLeaf> Params = AXL_REF_NEW_EXTRA (ref::CBoxT <TCliMsg_SetIndexTailLeaf>, VolatileSize);
	Params->m_IsNewLeaf = IsNewLeaf;
	Params->m_LineCount = LineCount;
	Params->m_Leaf = *pLeaf;

	if (VolatileSize)
		memcpy (Params + 1, pVolatilePacketArray, VolatileSize);

	PostThreadMsg (ECliMsg_SetIndexTailLeaf, Params);
}

void 
CWidget::RepresentComplete (
	size_t SyncId,
	size_t IndexNodeOffset,
	size_t IndexNodeLineCount,
	size_t LineIdx,
	const void* p,
	size_t Size
	)
{
	ref::CPtrT <TCliMsg_RepresentComplete> Params = AXL_REF_NEW_EXTRA (ref::CBoxT <TCliMsg_RepresentComplete>, Size);
	Params->m_SyncId = SyncId;
	Params->m_IndexNodeOffset = IndexNodeOffset;
	Params->m_IndexNodeLineCount = IndexNodeLineCount;
	Params->m_LineIdx = LineIdx;
	Params->m_LineDataSize = Size;

	if (Size)
		memcpy (Params + 1, p, Size);

	PostThreadMsg (ECliMsg_RepresentComplete, Params);
}

void 
CWidget::ColorizeComplete (
	size_t SyncId,
	size_t IndexNodeOffset,
	size_t LineIdx,
	size_t LineOffset,
	const void* p,
	size_t Size
	)
{
	ref::CPtrT <TCliMsg_ColorizeComplete> Params = AXL_REF_NEW_EXTRA (ref::CBoxT <TCliMsg_ColorizeComplete>, Size);
	Params->m_SyncId = SyncId;
	Params->m_IndexNodeOffset = IndexNodeOffset;
	Params->m_LineIdx = LineIdx;
	Params->m_LineOffset = LineOffset;
	Params->m_ColorizeDataSize = Size;

	if (Size)
		memcpy (Params + 1, p, Size);

	PostThreadMsg (ECliMsg_ColorizeComplete, Params);
}

void
CWidget::OnThreadMsg (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	gui::TWidgetThreadMsg* pThreadMsg = (gui::TWidgetThreadMsg*) pMsg;
	void* p = pThreadMsg->m_Params.p ();

	switch (pThreadMsg->m_Code)
	{
	case ECliMsg_ClearCache:
		ClearCacheImpl ((size_t) p);
		break;

	case ECliMsg_FilterProgress:
		FilterProgressImpl ((uint_t) p);
		break;

	case ECliMsg_IndexProgress:
		IndexProgressImpl ((uint_t) p);
		break;

	case ECliMsg_SetIndexTailLeaf:
		{
		TCliMsg_SetIndexTailLeaf* pMsg = (TCliMsg_SetIndexTailLeaf*) p;
		SetIndexTailLeafImpl (
			pMsg->m_IsNewLeaf != 0,
			pMsg->m_LineCount,
			&pMsg->m_Leaf,
			pMsg->m_Leaf.m_VolatilePacketCount ? (TIndexVolatilePacket*) (pMsg + 1) : NULL
			);
		}
		break;

	case ECliMsg_RepresentComplete:
		{
		TCliMsg_RepresentComplete* pMsg = (TCliMsg_RepresentComplete*) p;
		RepresentCompleteImpl (
			pMsg->m_SyncId,
			pMsg->m_IndexNodeOffset,
			pMsg->m_IndexNodeLineCount,
			pMsg->m_LineIdx,
			pMsg->m_LineDataSize ? pMsg + 1 : NULL,
			pMsg->m_LineDataSize
			);
		}
		break;

	case ECliMsg_ColorizeComplete:
		{
		TCliMsg_ColorizeComplete* pMsg = (TCliMsg_ColorizeComplete*) p;
		ColorizeCompleteImpl (
			pMsg->m_SyncId,
			pMsg->m_IndexNodeOffset,
			pMsg->m_LineIdx,
			pMsg->m_LineOffset,
			pMsg->m_ColorizeDataSize ? pMsg + 1 : NULL,
			pMsg->m_ColorizeDataSize
			);
		}
		break;
	}
}

void 
CWidget::ClearCacheImpl (size_t SyncId)
{
	m_SyncId = SyncId;
	m_LineCount = 0;
	m_IndexFile.Clear ();
	m_CachePageList.Clear ();
	m_CachePageMap.Clear ();

	Redraw ();
}

void 
CWidget::FilterProgressImpl (uint_t Percentage)
{
	// notify parent
}

void 
CWidget::IndexProgressImpl (uint_t Percentage)
{
	// notify parent
}

void 
CWidget::SetIndexTailLeafImpl (
	bool IsNewLeaf,
	size_t LineCount,
	const TIndexLeaf* pLeaf,
	const TIndexVolatilePacket* pVolatilePacketArray
	)
{
	TIndexNode* pNode;
	
	if (IsNewLeaf)
	{
		pNode = m_IndexFile.AddLeaf ();
	}
	else 
	{
		pNode = m_IndexFile.GetTailLeaf ();
		if (!pNode)
			pNode = m_IndexFile.AddLeaf ();
	}
	
	if (pNode->m_LineCount != LineCount)
		m_IndexFile.AddLeafLines (pNode, LineCount - pNode->m_LineCount);

	m_IndexFile.SetTailLeafData (pLeaf, sizeof (TIndexLeaf));
	
	if (pLeaf->m_VolatilePacketCount)
		m_IndexFile.AddTailLeafData (pVolatilePacketArray, pLeaf->m_VolatilePacketCount * sizeof (TIndexVolatilePacket));

	m_LineCount = m_IndexFile.GetLineCount ();

	CPage* pPage = FindCachePageByIndexNode (pNode);
	if (pPage)
		pPage->m_Flags &= ~(EPageFlag_Represented | EPageFlag_Representing | EPageFlag_Colorized | EPageFlag_Colorizing);

	RecalcVScroll ();

	size_t StartLineIdx = m_IndexFile.GetLeafStartLine (pNode);
	size_t EndLineIdx = StartLineIdx + pNode->m_LineCount;

	if (pPage && !pPage->IsEmpty ())
		StartLineIdx += pPage->m_LineArray.GetCount () - 1;

	RedrawLineRange (StartLineIdx, EndLineIdx);
}

void 
CWidget::RepresentCompleteImpl (
	size_t SyncId,
	size_t IndexNodeOffset,
	size_t IndexNodeLineCount,
	size_t LineIdx,
	const void* _p,
	size_t Size
	)
{
	if (SyncId != m_SyncId)
		return;

	CPage* pPage = GetOrCreateCachePage (IndexNodeOffset);
	pPage->m_LineArray.SetCount (IndexNodeLineCount);
	pPage->m_Flags &= ~EPageFlag_Representing;
	pPage->m_Flags |= EPageFlag_Represented;

	const char* p = (const char*) _p;

	// fill the page starting with LineIdx to IndexNodeLineCount with data from p

	for (size_t i = LineIdx; i < IndexNodeLineCount; i++)
	{
		if (Size < sizeof (TLine))
			break;

		CLine* pLine = NULL;

		ELine LineKind = ((TLine*) p)->m_LineKind;
		switch (LineKind)
		{
		case ELine_Text:
			pLine = AXL_MEM_NEW (CTextLine);
			break;

		case ELine_BinHex:
			pLine = AXL_MEM_NEW (CBinHexLine);
			break;

		case ELine_BinText:
			pLine = AXL_MEM_NEW (CBinTextLine);
			break;
		}

		if (!pLine)
			break;

		pPage->m_LineArray [i] = pLine;
		pLine->m_pPage = pPage;
		pLine->m_LineIdx = i;

		size_t LineSize = pLine->Load (p, Size);
		if (LineSize == -1)
			break;

		ASSERT (LineSize <= Size);

		p += LineSize;
		Size -= LineSize;
	}

	size_t StartLineIdx = GetCachePageStartLineIdx (pPage);
	size_t EndLineIdx = StartLineIdx + IndexNodeLineCount;

	RedrawLineRange (StartLineIdx + LineIdx, EndLineIdx);
}

void 
CWidget::ColorizeCompleteImpl (
	size_t SyncId,
	size_t IndexNodeOffset,
	size_t LineIdx,
	size_t LineOffset,
	const void* p,
	size_t Size
	)
{
	if (SyncId != m_SyncId)
		return;
}

//.............................................................................

} // namespace log {
} // namespace axl {
