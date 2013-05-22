#include "pch.h"
#include "axl_log_Widget.h"
#include "axl_mt_Process.h"

namespace axl {
namespace log {

//.............................................................................

bool
CWidget::GetHyperlinkFromMousePos (
	int x,
	int y,
	CLine** ppLine,
	rtl::CString* pHyperlink
	)
{
	gui::TCursorPos Pos = GetCursorPosFromMousePos (x, y, false);

	CLine* pLine = m_CacheMgr.GetLine (Pos.m_Line);
	if (!pLine || pLine->m_LineKind != ELine_Text)
		return false;

	CTextLine* pTextLine = (CTextLine*) pLine;

	gui::THyperlinkAnchor* pAnchor = pTextLine->m_HyperText.FindHyperlinkByOffset (Pos.m_Col);
	if (!pAnchor || pAnchor->m_Hyperlink.IsEmpty ())
		return false;

	if (ppLine)
		*ppLine = pLine;

	if (pHyperlink)
		*pHyperlink = pAnchor->m_Hyperlink;

	return true;
}

void
CWidget::OnHyperlink (
	CLine* pLine,
	const char* pHyperlink
	)
{
	if (m_pHyperlinkHandler)
	{
		bool Result = m_pHyperlinkHandler->OnHyperlink (pLine, pHyperlink);
		if (Result)
			return;
	}

	switch (*pHyperlink)
	{
	case '+': // add/remove bits to volatile flags
	case '-': 
		{
		uint_t Modify;
		uint_t NewVolatileFlags;

		if (!pLine->m_pVolatilePacket)
			break;

		Modify = strtoul (pHyperlink + 1, NULL, 10);
		if (!Modify)
			break;

		NewVolatileFlags = *pHyperlink == '+' ? 
			pLine->m_pVolatilePacket->m_VolatileFlags | Modify :
			pLine->m_pVolatilePacket->m_VolatileFlags & ~Modify;

		ModifyLineVolatileFlags (pLine, NewVolatileFlags);
		break;
		}

	case '!': // hilite bin range
		{
		char* pEnd;
		size_t Offset;
		size_t Length;

		Offset = strtoul (pHyperlink + 1, &pEnd, 10);
		if (!pEnd)
			break;

		if (*pEnd != ',' && *pEnd != ';')
			break;

		Length = strtoul (pEnd + 1, &pEnd, 10);
		if (!pEnd)
			break;

		CBinLine* pBinLine = pLine->GetCachePage ()->ScanUntilBinLine (pLine);
		if (pBinLine)
			HiliteBinRange (pBinLine, Offset, Length);
		break;
		}

	default:
		mt::CreateProcess (pHyperlink);
	}	
}

//.............................................................................

} // namespace log {
} // namespace axl {
