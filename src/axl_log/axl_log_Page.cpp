#include "pch.h"
#include "axl_log_Page.h"

namespace axl {
namespace log {

//.............................................................................

CPage::CPage ()
{
	m_Flags = 0;

	m_PacketOffset = -1;
	m_IndexNodeOffset = -1;
	memset (&m_IndexLeaf, 0, sizeof (m_IndexLeaf));
	m_IndexLeaf.m_DefBinDataConfig.SetDefaults ();

	m_LongestTextLineLength = 0;
	m_LongestBinHexLineSize = 0;
	m_LongestBinTextLineSize = 0;

	m_ColorizedLineCount = 0;
	m_ColorizerLine = -1;
	m_ColorizerOffset = 0;
	m_ColorizerState = 0;
}

void
CPage::Clear ()
{
	m_Flags = 0;

	size_t Count = m_LineArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
		AXL_MEM_DELETE (m_LineArray [i]);
	
	m_LineArray.Clear ();
	m_VolatilePacketArray.Clear ();
	
	m_PacketOffset = -1;
	m_IndexNodeOffset = -1;
	memset (&m_IndexLeaf, 0, sizeof (m_IndexLeaf));
	m_IndexLeaf.m_DefBinDataConfig.SetDefaults ();

	m_IndexVolatilePacketArray.Clear ();

	m_LongestTextLineLength = 0;
	m_LongestBinHexLineSize = 0;
	m_LongestBinTextLineSize = 0;

	m_ColorizedLineCount = 0;
	m_ColorizerLine = -1;
	m_ColorizerOffset = 0;
	m_ColorizerState = 0;
	m_ColorizerStateEx = ref::EPtr_Null;
}

size_t
CPage::GetVolatilePacketIdx (const TPageVolatilePacket* pPacket)
{
	size_t Count = m_VolatilePacketArray.GetCount ();
	if (Count && m_VolatilePacketArray [Count - 1].m_Offset == pPacket->m_Offset)
		return Count - 1;
	
	m_VolatilePacketArray.Append (*pPacket);
	return Count;
}

CBinLine*
CPage::FindFirstBinLine (size_t i)
{
	size_t Count = m_LineArray.GetCount ();
	for (; i < Count; i++)
	{
		CLine* pLine = m_LineArray [i];
		if (pLine->IsBin ())
			return (CBinLine*) pLine;
	}

	return NULL;
}

size_t 
CPage::GetVolatilePacketAnchorLineIdx (TPageVolatilePacket* pVolatilePacket)
{
	if (pVolatilePacket->m_FirstLineIdx != -1)
		return pVolatilePacket->m_FirstLineIdx;

	// volatile message is in visible

	size_t LineCount = m_LineArray.GetCount ();
	for (size_t i = 0; i < LineCount; i++)
	{
		CLine* pLine = m_LineArray [i];
		if (pLine->m_FirstPacketOffset >= pVolatilePacket->m_Offset)
			return pLine->m_LineIdx;
	}

	return LineCount;
}

void
CPage::ClearColorization ()
{
	size_t LineCount = m_LineArray.GetCount ();
	for (size_t i = 0; i < LineCount; i++)
	{
		CLine* pLine = m_LineArray [i];

		if (pLine->IsBin ())
		{
			CBinLine* pBinLine = (CBinLine*) pLine;
			pBinLine->m_AttrArray = pBinLine->m_OriginalAttrArray;
		}
	}

	m_ColorizedLineCount = 0;
	m_ColorizerLine = -1;
	m_ColorizerOffset = 0;
	m_ColorizerState = 0;
	m_ColorizerStateEx = ref::EPtr_Null;
	m_Flags &= ~EPageFlag_Colorized;
}

void
CPage::AddLines (
	size_t LineIdx,
	intptr_t LineDelta
	)
{
	if (!LineDelta)
		return;
	
	if (LineDelta < 0)
	{
		m_LineArray.Remove (LineIdx, -LineDelta);
	}
	else
	{
		m_LineArray.Insert (LineIdx, NULL, LineDelta);
		LineIdx += LineDelta;
	}

	// fixup following line indices

	size_t LineCount = m_LineArray.GetCount ();
	for (size_t i = LineIdx; i < LineCount; i++)
	{
		CLine* pLine = m_LineArray [i];

		if (pLine)
			pLine->m_LineIdx = i;
	}
}

void
CPage::FreeLines (
	size_t LineIdx,
	size_t LineCount
	)
{	
	size_t PageLineCount = m_LineArray.GetCount ();	
	if (LineIdx >= PageLineCount)
		return;

	if (LineIdx + LineCount > PageLineCount)
		LineCount = PageLineCount - LineIdx;

	size_t End = LineIdx + LineCount;
	for (size_t i = LineIdx; i < End; i++)
	{
		CLine* pLine = m_LineArray [i];
		AXL_MEM_DELETE (pLine);
		m_LineArray [i] = NULL;
	}	
}

bool 
CPage::SaveLines (
	rtl::CArrayT <uchar_t>* pBuffer,
	size_t LineIndex
	)
{
	char Buffer [256];
	rtl::CArrayT <uchar_t> LineBuffer (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	
	size_t LineCount = m_LineArray.GetCount ();
	for (size_t i = LineIndex; i < LineCount; i++)
	{
		CLine* pLine = m_LineArray [i];
		pLine->Save (&LineBuffer);
		pBuffer->Append (LineBuffer);
	}

	return true;
}

//.............................................................................

} // namespace log {
} // namespace axl {
