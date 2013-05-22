#include "pch.h"
#include "axl_log_IndexRepresentorTarget.h"

namespace axl {
namespace log {

//.............................................................................

TIndexRepresentorTargetData::TIndexRepresentorTargetData ()
{
	m_LineCount = 0;
	m_Col = 0;
	m_BinOffset = 0;
	m_PartIndex = 0;
	m_MergeId = 0;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void 
CIndexRepresentorTarget::AddPart (
	EPart PartKind,
	uint_t PartCode,
	uint_t MergeFlags,
	const void* p,
	size_t Size
	)
{
	if (Size == 0)
		return;

	ELine LineKind = GetLineKindFromPartKind (PartKind, m_BinDataConfig);

	TMergeCriteria MergeCriteria;
	MergeCriteria.m_PartCode = PartCode;
	MergeCriteria.m_MergeFlags = MergeFlags;
	MergeCriteria.m_BinDataConfig = m_BinDataConfig;
	MergeCriteria.m_Timestamp = m_Timestamp;
	MergeCriteria.m_LineKind = LineKind;

	bool ShouldMerge = m_MergeCriteria.ShouldMerge (MergeCriteria);
	if (!ShouldMerge)
	{
		m_Col = 0;
		m_BinOffset = 0;
		m_PartIndex = 0;
		m_MergeId++;
		m_LineCount++;
	}

	m_MergeCriteria = MergeCriteria;

	switch (LineKind)
	{
	case ELine_Text:
		AddText ((const char*) p, Size / sizeof (char));
		break;

	case ELine_BinHex:
		AddBinHex (m_BinDataConfig, Size);
		m_BinOffset += Size;
		break;

	case ELine_BinText:
		AddBinText (m_BinDataConfig, p, Size);
		m_BinOffset += Size;
		break;
	}

	m_PartIndex++;
}

void 
CIndexRepresentorTarget::AddText (
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
CIndexRepresentorTarget::AddBinText (
	const TBinDataConfig& BinDataConfig,
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
			if (m_Col >= BinDataConfig.m_BinTextLineSize)
				m_LineCount++, m_Col = 0;

			TabSize = BinDataConfig.m_BinTextTabSize - m_Col % BinDataConfig.m_BinTextTabSize;
			
			if (m_Col + TabSize > BinDataConfig.m_BinTextLineSize)
				TabSize = BinDataConfig.m_BinTextLineSize - m_Col;

			m_Col += TabSize;
			break;

		default:
			if (m_Col >= BinDataConfig.m_BinTextLineSize)
				m_LineCount++, m_Col = 0;

			m_Col++;
		}
}

void 
CIndexRepresentorTarget::AddBinHex (
	const TBinDataConfig& BinDataConfig,
	size_t Size
	)
{
	Size += m_Col - 1;
	m_LineCount += (Size / BinDataConfig.m_BinHexLineSize);
	m_Col = (Size % BinDataConfig.m_BinHexLineSize) + 1;
}

//.............................................................................

} // namespace log 
} // namespace axl
