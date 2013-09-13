#include "pch.h"
#include "axl_log_Line.h"
#include "axl_log_Page.h"

namespace axl {
namespace log {

//.............................................................................

TLineAttr::TLineAttr ()
{
	m_Flags = 0;
	m_Icon = -1;

	m_UpperDelimiter = ELineDelimiter_Normal;
	m_LowerDelimiter = ELineDelimiter_Normal;
}

//.............................................................................

CLine::CLine ()
{
	m_LineKind = ELine_Undefined;
	m_MergeId = -1;
	m_IsFirstLineOfPacket = false;
	m_FirstPacketOffset = 0;
	m_FirstTimestamp = 0;
	m_LastTimestamp = 0;
	m_pPage = NULL;
	m_LineIdx = 0;
	m_VolatilePacketIdx = -1;
	m_UserParam = 0;
	m_PartCode = 0;
	m_PartIdx = 0;
	m_MergeId = 0;
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
	m_LineAttr = pLine->m_LineAttr;

	m_VolatilePacketIdx = (pLine->m_VolatilePacket.m_Code & EPacketCodeFlag_Volatile) ? 
		m_pPage->GetVolatilePacketIdx (&pLine->m_VolatilePacket) : 
		-1;

	m_IsFirstLineOfPacket = pLine->m_IsFirstLineOfPacket != 0;
	m_FirstPacketOffset = pLine->m_FirstPacketOffset;
	m_FirstTimestamp = pLine->m_FirstTimestamp;
	m_LastTimestamp = pLine->m_LastTimestamp;

	m_PartCode = pLine->m_PartCode;
	m_PartIdx = pLine->m_PartIdx;
	m_MergeId = pLine->m_MergeId; 
	
	return sizeof (TLine);
}

size_t
CLine::Save (rtl::CArrayT <uint8_t>* pBuffer)
{
	TLine Line;
	memset (&Line, 0, sizeof (Line));

	Line.m_LineKind = m_LineKind;
	Line.m_LineAttr = m_LineAttr;

	if (m_VolatilePacketIdx != -1)
		Line.m_VolatilePacket = m_pPage->GetVolatilePacketArray () [m_VolatilePacketIdx];

	Line.m_IsFirstLineOfPacket = m_IsFirstLineOfPacket;
	Line.m_FirstPacketOffset = m_FirstPacketOffset;
	Line.m_FirstTimestamp = m_FirstTimestamp;
	Line.m_LastTimestamp = m_LastTimestamp;

	Line.m_PartCode = m_PartCode; 
	Line.m_PartIdx = m_PartIdx;
	Line.m_MergeId = m_MergeId;

	pBuffer->Copy ((uint8_t*) &Line, sizeof (Line));
	return sizeof (Line);
}

//.............................................................................

} // namespace log {
} // namespace axl {
