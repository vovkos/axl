#include "pch.h"
#include "axl_log_Line.h"
#include "axl_log_CachePage.h"

namespace axl {
namespace log {

//.............................................................................

CLine::CLine ()
{
	m_LineKind = ELine_Undefined;
	m_Flags = 0;
	m_pPage = NULL;
	m_LineIdx = -1;
	m_FoldablePacketIdx = -1;
	m_PartCode = 0;
	m_FirstPacketOffset = -1;
	m_FirstTimestamp = 0;
	m_LastTimestamp = 0;
	m_PartIdx = -1;

	m_UserParam = 0;
}

size_t
CLine::Load (
	const void* p,
	size_t Size
	)
{
	if (Size < sizeof (TLine))
		return 0;

	TLine* pLine = (TLine*) p;

	ASSERT (m_LineKind == pLine->m_LineKind);
	m_Flags = pLine->m_Flags;
	m_FoldablePacketIdx = pLine->m_FoldablePacketIdx;
	m_LineAttr = pLine->m_LineAttr;
	m_PartCode = pLine->m_PartCode;
	m_FirstPacketOffset = pLine->m_FirstPacketOffset;
	m_FirstTimestamp = pLine->m_FirstTimestamp;
	m_LastTimestamp = pLine->m_LastTimestamp;
	m_PartIdx = pLine->m_PartIdx;
	
	return sizeof (TLine);
}

size_t
CLine::Save (rtl::CArrayT <uint8_t>* pBuffer)
{
	TLine Line;
	memset (&Line, 0, sizeof (Line));

	Line.m_LineKind = m_LineKind;
	Line.m_Flags = m_Flags;
	Line.m_FoldablePacketIdx = m_FoldablePacketIdx;
	Line.m_LineAttr = m_LineAttr;
	Line.m_PartCode = m_PartCode; 
	Line.m_FirstPacketOffset = m_FirstPacketOffset;
	Line.m_FirstTimestamp = m_FirstTimestamp;
	Line.m_LastTimestamp = m_LastTimestamp;
	Line.m_PartIdx = m_PartIdx;

	pBuffer->Copy ((uint8_t*) &Line, sizeof (Line));
	return sizeof (Line);
}

void
CLine::Clear ()
{
	m_Flags = 0;
	m_pPage = NULL;
	m_LineIdx = -1;
	m_FoldablePacketIdx = -1;
	m_LineAttr.Clear ();
	m_PartCode = 0;
	m_FirstPacketOffset = -1;
	m_FirstTimestamp = 0;
	m_LastTimestamp = 0;
	m_PartIdx = -1;

	m_UserParam = 0;
}

//.............................................................................

} // namespace log {
} // namespace axl {
