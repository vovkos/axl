#include "pch.h"
#include "axl_log_Widget.h"
#include "axl_gui_Engine.h"
#include "axl_io_FilePathUtils.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace log {

//.............................................................................

CWidget::CWidget (gui::CEngine* pEngine):
	gui::CWidget (pEngine)
{
	m_pServer = NULL;

/*
	m_CacheMgr.m_pWidget = this;
	m_CacheMgr.m_pIndexMgr = &m_IndexMgr;
	m_ColorizeMgr.m_pCacheMgr = &m_CacheMgr;

	m_pHyperlinkHandler = NULL;
	m_pVolatilePacketFilter = NULL;
*/
	m_MaxBinBlockBuffer = 1024;

	m_IconGapSize = 1;
	m_TimestampGapSize = 2;
	m_OffsetGapSize = 2;
	m_HexGapSize = 1;
	m_CaretWidth = 2;

	m_LineCount = 0;
	m_ColCount = 0;
	m_FirstVisibleLine = 0;
	m_FirstVisibleCol = 0;
	m_VisibleLineCount = 0;
	m_VisibleColCount = 0;

	m_IsIconVisible = true;
	m_IsTimestampVisible = true;
	m_IsOffsetVisible = true;

	m_IsDragging = false;
	m_IsSelecting = false;

	m_TimestampFormat = "%h:%m:%s";
	m_TimestampWidth = CalcTimestampWidth (m_TimestampFormat);
	m_BaseCol = m_TimestampWidth;

	m_OffsetWidth = 4;
	m_FullOffsetWidth = m_OffsetWidth + m_OffsetGapSize;

	m_pBaseFont = m_pEngine->GetDefaultMonospaceFont ();
	ASSERT (m_pBaseFont->IsMonospace ());

	m_CharSize = m_pBaseFont->CalcTextSize ('X');

	m_CharSize.m_Height += 1; // separator line

	m_pImageList = NULL;
	m_IconWidth = 0;
	m_IconOrigin.m_x = (2 * m_CharSize.m_Width - 16) / 2;
	m_IconOrigin.m_y = (m_CharSize.m_Height - 1 - 16) / 2;

	memcpy (m_ColorArray, gui::GetStdPalColorArray (), gui::EStdPalColor__Count * sizeof (uint_t));
	
	m_ColorArray [~gui::EColorFlag_Index & EColor_HiliteBack]           = 0xdcdcff;
	m_ColorArray [~gui::EColorFlag_Index & EColor_IntraPacketDelimiter] = 0xdcdcdc;
	m_ColorArray [~gui::EColorFlag_Index & EColor_InterPacketDelimiter] = 0xc0c0c0;
	m_ColorArray [~gui::EColorFlag_Index & EColor_Timestamp]            = gui::EStdPalColor_GrayText;
	m_ColorArray [~gui::EColorFlag_Index & EColor_Offset]               = gui::EStdPalColor_GrayText;
	m_ColorArray [~gui::EColorFlag_Index & EColor_CacheMissBack]        = gui::EStdPalColor_GrayText;

	m_Palette.m_pColorArray = m_ColorArray;
	m_Palette.m_Count = countof (m_ColorArray);

/*
	TStockCtrlPaint_Construct (&m_StockCtrlPaint);

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	TOffscreenBuffer_Construct (&m_OffscreenBuffer);
#endif

//	m_hUpdateRgn = CreateRectRgn (0, 0, 0, 0);

*/
}

bool
CWidget::Create (
	ÑServer* pServer,
	const char* pIndexFileName
	)
{
	ClearCache (-1);

	bool Result = m_CacheMgr.Create (this, pServer, pIndexFileName);
	if (!Result)
		return false;

	m_pServer = pServer;
	return true;
}

void
CWidget::SendMsg (const TMsg* pMsgHdr)
{
	switch (pMsgHdr->m_MsgKind)
	{
	case ECliMsg_ClearCache:
		if (pMsgHdr->m_MsgSize >= sizeof (TCliMsg_ClearCache))
		{
			TCliMsg_ClearCache* pMsg = (TCliMsg_ClearCache*) pMsgHdr;
			PostThreadMsg (ECliMsg_ClearCache, ref::CPtrT <void> ((void*) (size_t) pMsg->m_SyncId, (ref::CRefCount*) NULL));
		}

		break;

	case ECliMsg_FilterProgress:
		if (pMsgHdr->m_MsgSize >= sizeof (TCliMsg_Progress))
		{
			TCliMsg_Progress* pMsg = (TCliMsg_Progress*) pMsgHdr;
			PostThreadMsg (ECliMsg_FilterProgress, ref::CPtrT <void> ((void*) pMsg->m_Percentage, (ref::CRefCount*) NULL));
		}

		break;

	case ECliMsg_IndexProgress:
		if (pMsgHdr->m_MsgSize >= sizeof (TCliMsg_Progress))
		{
			TCliMsg_Progress* pMsg = (TCliMsg_Progress*) pMsgHdr;
			PostThreadMsg (ECliMsg_IndexProgress, ref::CPtrT <void> ((void*) pMsg->m_Percentage, (ref::CRefCount*) NULL));
		}

		break;

	case ECliMsg_ColorizeProgress:
		if (pMsgHdr->m_MsgSize >= sizeof (TCliMsg_Progress))
		{
			TCliMsg_Progress* pMsg = (TCliMsg_Progress*) pMsgHdr;
			PostThreadMsg (ECliMsg_ColorizeProgress, ref::CPtrT <void> ((void*) pMsg->m_Percentage, (ref::CRefCount*) NULL));
		}

		break;

	case ECliMsg_CreateIndexLeaf:
		if (pMsgHdr->m_MsgSize >= sizeof (TCliMsg_CreateIndexLeaf))
		{
			TCliMsg_CreateIndexLeaf* pMsg = (TCliMsg_CreateIndexLeaf*) pMsgHdr;
			ref::CPtrT <TCliMsg_CreateIndexLeaf> Params = AXL_REF_NEW (ref::CBoxT <TCliMsg_CreateIndexLeaf>);
			*Params = *pMsg;
			PostThreadMsg (ECliMsg_CreateIndexLeaf, Params);
		}

		break;

	case ECliMsg_UpdateIndexTailLeaf:
		if (pMsgHdr->m_MsgSize >= sizeof (TCliMsg_UpdateIndexTailLeaf))
		{
			TCliMsg_UpdateIndexTailLeaf* pMsg = (TCliMsg_UpdateIndexTailLeaf*) pMsgHdr;
			ref::CPtrT <TCliMsg_UpdateIndexTailLeaf> Params = AXL_REF_NEW (ref::CBoxT <TCliMsg_UpdateIndexTailLeaf>);
			*Params = *pMsg;
			PostThreadMsg (ECliMsg_UpdateIndexTailLeaf, Params);
		}

		break;

	case ECliMsg_RepresentPageComplete:
		if (pMsgHdr->m_MsgSize >= sizeof (TCliMsg_RepresentPageComplete))
		{
			TCliMsg_RepresentPageComplete* pMsg = (TCliMsg_RepresentPageComplete*) pMsgHdr;
			size_t ExtraSize = pMsg->m_FoldablePacketCount * sizeof (TFoldablePacket) + pMsg->m_LineBufferSize;
			
			if (pMsg->m_MsgSize < sizeof (TCliMsg_RepresentPageComplete) + ExtraSize)
				break;

			ref::CPtrT <TCliMsg_RepresentPageComplete> Params = AXL_REF_NEW_EXTRA (ref::CBoxT <TCliMsg_RepresentPageComplete>, ExtraSize);
			*Params = *pMsg;
			memcpy (Params + 1, pMsg + 1, ExtraSize);
			PostThreadMsg (ECliMsg_RepresentPageComplete, Params);
		}

		break;

	case ECliMsg_FoldPacketComplete:
		if (pMsgHdr->m_MsgSize >= sizeof (TCliMsg_FoldPacketComplete))
		{
			TCliMsg_FoldPacketComplete* pMsg = (TCliMsg_FoldPacketComplete*) pMsgHdr;
			size_t ExtraSize = pMsg->m_LineBufferSize;

			if (pMsg->m_MsgSize < sizeof (TCliMsg_RepresentPageComplete) + ExtraSize)
				break;

			ref::CPtrT <TCliMsg_FoldPacketComplete> Params = AXL_REF_NEW_EXTRA (ref::CBoxT <TCliMsg_FoldPacketComplete>, ExtraSize);
			*Params = *pMsg;
			memcpy (Params + 1, pMsg + 1, ExtraSize);
			PostThreadMsg (ECliMsg_FoldPacketComplete, Params);
		}

		break;
	}
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
		m_CacheMgr.ClearCache ((size_t) p);
		break;

	case ECliMsg_FilterProgress:
	case ECliMsg_IndexProgress:
	case ECliMsg_ColorizeProgress:
		// notify parent
		break;

	case ECliMsg_CreateIndexLeaf:
		{
		TCliMsg_CreateIndexLeaf* pMsg = (TCliMsg_CreateIndexLeaf*) p;
		ASSERT (pMsg->m_LeafOffset == m_CacheMgr.GetIndexNextLeafOffset ()); 

		if (pMsg->m_LeafOffset == m_CacheMgr.GetIndexNextLeafOffset ())
			m_CacheMgr.CreateIndexLeaf (
				&pMsg->m_BinDataConfig,
				pMsg->m_LineCount,
				pMsg->m_PacketOffset,
				pMsg->m_PacketCount,
				pMsg->m_FoldablePacketCount,
				pMsg->m_MergePointIdx,
				pMsg->m_FoldFlags
				);
		}
		break;

	case ECliMsg_UpdateIndexTailLeaf:
		{
		TCliMsg_UpdateIndexTailLeaf* pMsg = (TCliMsg_UpdateIndexTailLeaf*) p;
		ASSERT (pMsg->m_LeafOffset == m_CacheMgr.GetIndexTailLeafOffset ()); 

		if (pMsg->m_LeafOffset == m_CacheMgr.GetIndexTailLeafOffset ())
			m_CacheMgr.UpdateIndexTailLeaf (
				pMsg->m_LineCount,
				pMsg->m_PacketCount,
				pMsg->m_FoldablePacketCount,
				pMsg->m_FoldFlags
				);
		}
		break;

	case ECliMsg_RepresentPageComplete:
		{
		TCliMsg_RepresentPageComplete* pMsg = (TCliMsg_RepresentPageComplete*) p;
		if (pMsg->m_SyncId == m_CacheMgr.GetSyncId ())
			m_CacheMgr.RepresentPageComplete (
				pMsg->m_IndexLeafOffset,
				pMsg->m_LineIdx,
				pMsg->m_LineCount,
				pMsg->m_FoldablePacketIdx,
				(TFoldablePacket*) (pMsg + 1),
				pMsg->m_FoldablePacketCount,
				(TFoldablePacket*) (pMsg + 1) + pMsg->m_FoldablePacketCount,
				pMsg->m_LineBufferSize
				);
		}
		break;

	case ECliMsg_FoldPacketComplete:
		{
		TCliMsg_FoldPacketComplete* pMsg = (TCliMsg_FoldPacketComplete*) p;
		if (pMsg->m_SyncId == m_CacheMgr.GetSyncId ())
			m_CacheMgr.FoldPacketComplete (
				pMsg->m_IndexLeafOffset,
				pMsg->m_LineIdx,
				pMsg->m_OldLineCount,
				pMsg->m_NewLineCount,
				pMsg->m_FoldablePacketIdx,
				pMsg->m_FoldFlags,
				pMsg + 1,
				pMsg->m_LineBufferSize
				);
		}
		break;
	}
}

bool
CWidget::Copy ()
{
	return true;
}

/*

bool
CWidget::SetPacketFile (
	CPacketFile* pPacketFile,
	CRepresenter* pRepresenter
	)
{
	bool Result;

	rtl::CString IndexFileName = io::CreateTempFileName ();
	
	Result = m_IndexMgr.Create (
		pPacketFile, 
		pRepresenter, 
		rtl::CString (IndexFileName), 
		io::EFileFlag_DeleteOnClose | io::EFileFlag_Exclusive
		);

	if (!Result)
		return false;

#if (_AXL_ENV == AXL_ENV_WIN)
	::SetFileAttributesW (
		IndexFileName.cc2 (), 
		FILE_ATTRIBUTE_HIDDEN | 
		FILE_ATTRIBUTE_TEMPORARY |
		FILE_ATTRIBUTE_NOT_CONTENT_INDEXED
		);
#endif

	ReRepresentAll ();
	return true;
}

*/

void 
CWidget::SetImageList (gui::CImageList* pImageList)
{
	m_pImageList = pImageList;

	if (pImageList)
	{
		gui::TSize Size = pImageList->GetImageSize ();
		size_t IconColCount = Size.m_Width / m_CharSize.m_Width;
		if (Size.m_Width % m_CharSize.m_Width)
			IconColCount++;

		m_IconWidth = IconColCount + m_IconGapSize;
	}
	else
	{
		m_IconWidth = 0;
	}

	RecalcBaseCol ();
	Redraw ();
}

void 
CWidget::ShowIcon (bool IsVisible)
{
	m_IsIconVisible = IsVisible;
	RecalcBaseCol ();
	Redraw ();
}

void 
CWidget::SetTimestampFormat (const rtl::CString& Format)
{
	m_TimestampFormat = Format;
	m_TimestampWidth = CalcTimestampWidth (Format);

	RecalcBaseCol ();
	Redraw ();
}

void 
CWidget::ShowTimestamp (bool IsVisible)
{
	m_IsTimestampVisible = IsVisible;
	RecalcBaseCol ();
	Redraw ();
}

void 
CWidget::SetOffsetWidth (size_t Width)
{
	if (Width > 8)
		Width = 8;

	m_OffsetWidth = Width;
	m_FullOffsetWidth = m_IsOffsetVisible ? m_OffsetWidth + m_OffsetGapSize : 0;

	Redraw ();
}

void 
CWidget::ShowOffset (bool IsVisible)
{
	m_IsOffsetVisible = IsVisible;
	m_FullOffsetWidth = m_IsOffsetVisible ? m_OffsetWidth + m_OffsetGapSize : 0;

	Redraw ();
}

size_t 
CWidget::CalcTimestampWidth (const char* pFormat)
{
	if (!pFormat || !*pFormat)
		return 0;
	
	size_t Length = g::TTime ().Format (&m_StringBuffer, m_TimestampFormat);
	return Length + m_TimestampGapSize;
}

void
CWidget::SetLineCount (size_t LineCount)
{
	if (LineCount == m_LineCount)
		return;

	m_LineCount = LineCount;
	RecalcVScroll ();
}

void 
CWidget::RecalcBaseCol ()
{
	size_t BaseCol = 0;

	if (m_IsTimestampVisible)
		BaseCol += m_TimestampWidth;
	
	if (m_IsIconVisible)
		BaseCol += m_IconWidth;

	if (BaseCol != m_BaseCol)
	{
		m_BaseCol = BaseCol;
		RecalcHScroll ();
	}
}

void 
CWidget::RecalcColCount (const TLongestLineLength* pLength)
{
	size_t BinHexColCount = pLength->m_BinHexLineSize * 4 + m_HexGapSize + m_FullOffsetWidth;
	size_t BinTextColCount = pLength->m_BinTextLineSize + m_FullOffsetWidth;
	size_t ColCount = pLength->m_TextLineLength;
	
	if (BinHexColCount > ColCount)
		ColCount = BinHexColCount;

	if (BinTextColCount > ColCount)
		ColCount = BinTextColCount;

	if (ColCount != m_ColCount)
	{
		m_ColCount = ColCount;
		RecalcHScroll ();
	}
}
/*
void
CWidget::OnReRepresentPacket (
	CCachePage* pPage,
	TReRepresent* pReRepresent
	)
{
	intptr_t LineDelta;

	bool DidFirstVisibleLineChange;
	bool DidFirstVisibleColChange;

	if (pReRepresent->m_NewLineCount)
		m_ColorizeMgr.ReColorizeLineRange (pPage, pReRepresent->m_AnchorPageLine, pReRepresent->m_NewLineCount); 

	LineDelta = pReRepresent->m_NewLineCount - pReRepresent->m_OldLineCount;

	m_LineCount += LineDelta;
	ASSERT (m_LineCount == m_IndexMgr.GetIndexFile ()->GetLineCount ());

	DidFirstVisibleLineChange = FixupFirstVisibleLine ();
	DidFirstVisibleColChange = FixupFirstVisibleCol ();

	RecalcColCount ();
	RecalcVScroll ();
	RecalcHScroll ();

	if (DidFirstVisibleLineChange || DidFirstVisibleColChange)
		Redraw ();
	else if (LineDelta == 0)
		RedrawLineRange (pReRepresent->m_AnchorAbsoluteLine, pReRepresent->m_AnchorAbsoluteLine + pReRepresent->m_OldLineCount - 1);
	else
		RedrawLineRange (pReRepresent->m_AnchorAbsoluteLine, -1);

	UpdateCursorPosOnReRepresentPacket (pReRepresent->m_AnchorAbsoluteLine, pReRepresent->m_OldLineCount, pReRepresent->m_NewLineCount);
}

bool
CWidget::ModifyPacketVolatileFlags (
	CCachePage* pPage,
	TCacheVolatilePacket* pVolatileMsg,
	uint_t VolatileFlags
	)
{
	TReRepresent ReRepresent;

	if (VolatileFlags == pVolatileMsg->m_VolatileFlags)
		return false;

	m_CacheMgr.ModifyPacketVolatileFlags (pPage, pVolatileMsg, VolatileFlags, &ReRepresent);
	OnReRepresentPacket (pPage, &ReRepresent);
	return true;
}

bool
CWidget::ModifyLineVolatileFlags (
	CLine* pLine,
	uint_t VolatileFlags
	)
{
	return pLine->m_pVolatilePacket ? 
		ModifyPacketVolatileFlags (pLine->GetCachePage (), pLine->m_pVolatilePacket, VolatileFlags) :
		false;
}

void
CWidget::UpdateLog ()
{
	size_t Anchor = m_IndexMgr.UpdateIndex ();

	if (Anchor)
		IncrementalUpdateLog ();
	else
		FullUpdateLog ();
}

void 
CWidget::IncrementalUpdateLog ()
{
	bool ShouldScrollToBottom = m_ScrollBarArray [gui::EWidgetOrientation_Vertical].IsMaxed ();
	bool ShouldPutCursorToEnd = ShouldScrollToBottom && IsCursorAtLastLine ();

	size_t AnchorLine = m_LineCount ? m_LineCount - 1 : 0;

	// mark cache pages for update starting with anchor line
	// there might be more than one: could be empty pages

	TIndexNode* pNode = m_IndexMgr.GetIndexFile ()->FindLeafByLine (AnchorLine, NULL);
	while (pNode)
	{
		CCachePage* pPage = m_CacheMgr.FindCachePageByIndexNode (pNode);
		if (pPage)
		{
			if (pPage->IsEmpty ())
				m_CacheMgr.RemoveCachePage (pPage);
			else
				pPage->IncrementalUpdate ();
		}

		pNode = m_IndexMgr.GetIndexFile ()->GetRightNode (pNode);
	}

	m_LineCount = m_IndexMgr.GetIndexFile ()->GetLineCount ();

	FixupFirstVisibleLine ();
	RecalcVScroll ();
	RedrawLineRange (AnchorLine, m_LineCount - 1);

	if (ShouldScrollToBottom)
		ScrollToBottom ();

	if (ShouldPutCursorToEnd)
		SetCursorPos (-1, -1, false); 
}

void 
CWidget::FullUpdateLog ()
{
	m_CacheMgr.ClearCache ();
	
	m_LineCount = m_IndexMgr.GetIndexFile ()->GetLineCount ();
	m_FirstVisibleLine = 0;
	m_FirstVisibleCol = 0;
	m_ColCount = 0;

	RecalcVScroll ();
	RecalcHScroll ();
	SetCursorPos (m_CursorPos); // this will effectively update cursor pos

	Redraw ();
}
*/

void
CWidget::OnSetFocus (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	ShowCaret (
		GetMousePosFromCursorPos (m_CursorPos), 
		gui::TSize (m_CaretWidth, m_CharSize.m_Height)
		);
}

void
CWidget::OnKillFocus (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	HideCaret ();
}

void
CWidget::OnSize (
	gui::TWidgetMsg* pMsg,
	bool* pIsHandled
	)
{
	m_VisibleLineCount = m_Size.m_Height / m_CharSize.m_Height;
	m_VisibleColCount = m_Size.m_Width / m_CharSize.m_Width;

	bool ShouldScrollToBottom = IsCursorAtLastLine (); 
	bool DidFirstVisibleLineChange = FixupFirstVisibleLine ();
	bool DidFirstVisibleColChange = FixupFirstVisibleCol ();

	RecalcVScroll ();
	RecalcHScroll ();

	if (ShouldScrollToBottom)
		ScrollToBottom ();

	if (DidFirstVisibleLineChange || DidFirstVisibleColChange)
	{
		Redraw ();
		UpdateCaretPos ();
	}
}

//.............................................................................

} // namespace log {
} // namespace axl {
