#include "pch.h"
#include "axl_log_Widget.h"
#include "axl_gui_Engine.h"
#include "axl_io_FilePathUtils.h"

namespace axl {
namespace log {

//.............................................................................

CWidget::CWidget (gui::IEngine* pEngine):
	gui::IWidget (pEngine)
{
	m_pServer = NULL;

	m_SyncId = -1;
/*
	m_CacheMgr.m_pWidget = this;
	m_CacheMgr.m_pIndexMgr = &m_IndexMgr;
	m_ColorizeMgr.m_pCacheMgr = &m_CacheMgr;

	m_pHyperlinkHandler = NULL;
	m_pVolatilePacketFilter = NULL;
*/
	m_CachePageCountLimit = 64;
	m_MaxBinBlockBuffer = 1024;

	m_IconGapSize = 1;
	m_TimestampGapSize = 1;
	m_OffsetGapSize = 2;
	m_HexGapSize = 1;
	m_CaretWidth = 2;

	m_LineCount = 0;
	m_ColCount = 0;
	m_FirstVisibleLine = 0;
	m_FirstVisibleCol = 0;
	m_VisibleLineCount = 0;
	m_VisibleColCount = 0;

	m_LongestTextLineLength = 0;
	m_LongestBinHexLineSize = 0;
	m_LongestBinTextLineSize = 0;

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
	
	m_ColorArray [~gui::EColorFlag_Index & EColor_HiliteBack]      = 0xdcdcff;
	m_ColorArray [~gui::EColorFlag_Index & EColor_DelimiterLight]  = 0xdcdcdc;
	m_ColorArray [~gui::EColorFlag_Index & EColor_DelimiterMedium] = 0xc0c0c0;
	m_ColorArray [~gui::EColorFlag_Index & EColor_DelimiterBold]   = 0x808080;
	m_ColorArray [~gui::EColorFlag_Index & EColor_Timestamp]       = gui::EStdPalColor_GrayText;
	m_ColorArray [~gui::EColorFlag_Index & EColor_Offset]          = gui::EStdPalColor_GrayText;
	m_ColorArray [~gui::EColorFlag_Index & EColor_CacheMissBack]   = gui::EStdPalColor_GrayText;

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
	IServer* pServer,
	const char* pIndexFileName
	)
{
	ClearCache (-1);

	bool Result = m_IndexFile.Open (pIndexFileName, io::EFileFlag_Exclusive | io::EFileFlag_DeleteOnClose);
	if (!Result)
		return false;

	m_pServer = pServer;
	return true;
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
	IRepresenter* pRepresenter
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
CWidget::SetImageList (gui::IImageList* pImageList)
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
CWidget::RecalcColCount ()
{
	size_t BinHexColCount = m_LongestBinHexLineSize * 4 + m_HexGapSize + m_FullOffsetWidth;
	size_t BinTextColCount = m_LongestBinTextLineSize + m_FullOffsetWidth;
	size_t ColCount = m_LongestTextLineLength;
	
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
	CPage* pPage,
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
	CPage* pPage,
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
		CPage* pPage = m_CacheMgr.FindCachePageByIndexNode (pNode);
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
