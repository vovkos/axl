#include "pch.h"
#include "axl_log_IndexRepresenterTarget.h"

namespace axl {
namespace log {

//.............................................................................

CIndexRepresenterTarget::CIndexRepresenterTarget ()
{
	m_LineCount = 0;
	m_Col = 0;
	m_PartIdx = 0;
	m_FirstPartIdx = 0;
	m_BinOffset = 0;

	m_IsFirstPartMerged = false;
}

void
CIndexRepresenterTarget::StartPacket (const TPacket* pPacket)
{
	m_PacketCode = pPacket->m_Code;
	m_Timestamp = pPacket->m_Timestamp;
	m_FirstPartIdx = m_PartIdx;
	m_IsFirstPartMerged = false;
}

void 
CIndexRepresenterTarget::AddPart (
	EPart PartKind,
	uint_t PartCode,
	const void* p,
	size_t Size
	)
{
	if (Size == 0)
		return;

	ELine LineKind = GetLineKindFromPartKind (PartKind, m_BinDataConfig);

	TMergeCriteria MergeCriteria;
	MergeCriteria.m_PartCode = PartCode;
	MergeCriteria.m_BinDataConfig = m_BinDataConfig;
	MergeCriteria.m_Timestamp = m_Timestamp;
	MergeCriteria.m_LineKind = LineKind;

	bool ShouldMerge = m_MergeCriteria.ShouldMerge (MergeCriteria);
	if (!ShouldMerge)
	{
		m_Col = 0;
		m_LineCount++;
		m_PartIdx = 0;
		m_BinOffset = 0;
	}
	else if (m_PartIdx == m_FirstPartIdx)
	{
		m_IsFirstPartMerged = true;
	}

	m_MergeCriteria = MergeCriteria;

	switch (LineKind)
	{
	case ELine_Text:
		AddText ((const char*) p, Size / sizeof (char));
		break;

	case ELine_BinHex:
		AddBinHex (Size);
		m_BinOffset += Size;
		break;

	case ELine_BinText:
		AddBinText (p, Size);
		m_BinOffset += Size;
		break;
	}

	m_PartIdx++;
}

void 
CIndexRepresenterTarget::AddText (
	const char* pText,
	size_t Length
	)
{
	const char* p = pText;
	const char* pEnd = pText + Length;

	for (; p < pEnd; p++)
		if (*p == '\n')
			m_LineCount++;
}

void 
CIndexRepresenterTarget::AddBinText (
	const void* _p,
	size_t Size
	)
{
	const char* p = (const char*) _p;
	const char* pEnd = p + Size;

	for (; p < pEnd; p++)
		switch (*p)
		{
			size_t TabSize;

		case '\r': 
			// ignore '\r' 
			break; 

		case '\n':
			m_LineCount++, m_Col = 0;
			break;
		
		case '\t':
			if (m_Col >= m_BinDataConfig.m_BinTextLineSize)
				m_LineCount++, m_Col = 0;

			TabSize = m_BinDataConfig.m_BinTextTabSize - m_Col % m_BinDataConfig.m_BinTextTabSize;
			
			if (m_Col + TabSize > m_BinDataConfig.m_BinTextLineSize)
				TabSize = m_BinDataConfig.m_BinTextLineSize - m_Col;

			m_Col += TabSize;
			break;

		default:
			if (m_Col >= m_BinDataConfig.m_BinTextLineSize)
				m_LineCount++, m_Col = 0;

			m_Col++;
		}
}

void 
CIndexRepresenterTarget::AddBinHex (size_t Size)
{
	Size += m_Col - 1;
	m_LineCount += (Size / m_BinDataConfig.m_BinHexLineSize);
	m_Col = (Size % m_BinDataConfig.m_BinHexLineSize) + 1;
}

//.............................................................................

} // namespace log 
} // namespace axl
