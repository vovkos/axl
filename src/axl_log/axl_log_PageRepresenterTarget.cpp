#include "pch.h"
#include "axl_log_PageRepresenterTarget.h"

namespace axl {
namespace log {

//.............................................................................

CPageRepresenterTarget::CPageRepresenterTarget ()
{
	m_pPage = NULL;
	m_pPrevPageMergeLine = NULL;
	m_VolatilePacketIdx = -1;
	m_PacketOffset = 0;
	m_Timestamp = 0;
	m_BinOffset = 0;
	m_PartIdx = 0;
	m_MergeId = 0;
	m_NextLineIdx = 0;
	m_EndLineIdx = 0;
	m_LongestTextLineLength = 0;
	m_LongestBinHexLineSize = 0;
	m_LongestBinTextLineSize = 0;
	m_IsFull = false;
}

CPageRepresenterTarget::~CPageRepresenterTarget ()
{
	if (m_pPrevPageMergeLine)
		AXL_MEM_DELETE (m_pPrevPageMergeLine);
}

void 
CPageRepresenterTarget::AddPart (
	EPart PartKind,
	uint_t PartCode,
	uint_t MergeFlags,
	const void* p,
	size_t Size
	)
{
	if (Size == 0 || m_IsFull)
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
		MergeFlags = 0;
		m_BinOffset = 0;
		m_PartIdx = 0;
		m_MergeId++;
	}
	else
	{
		CLine* pLine = GetCurrentLine ();
		pLine->m_LastTimestamp = m_Timestamp;
	}

	m_MergeCriteria = MergeCriteria;

	switch (LineKind)
	{
	case ELine_Text:
		AddText (PartCode, MergeFlags, (const char*) p, Size / sizeof (char), PartKind == EPart_HyperText);
		break;

	case ELine_BinHex:
		AddBinHex (PartCode, MergeFlags, p, Size);
		break;

	case ELine_BinText:
		AddBinText (PartCode, MergeFlags, p, Size);
		break;
	}

	m_PartIdx++;
}

void 
CPageRepresenterTarget::AddText (
	uint_t PartCode,
	uint_t MergeFlags,
	const char* pText,
	size_t Length,
	bool IsHyperText
	)
{
	CTextLine* pLine;

	const char* p = pText;
	const char* pEnd = pText + Length;
	const char* pCr;

	if (MergeFlags)
	{
		size_t LineLength;

		pLine = (CTextLine*) GetCurrentLine ();
		ASSERT (pLine && pLine->m_LineKind == ELine_Text);

		if (MergeFlags & EMergeFlag_OverrideLineAttr)
			pLine->m_LineAttr = m_LineAttr;

		pCr = strchr_e (p, pEnd, '\n');
		
		LineLength = IsHyperText ?
			pLine->m_HyperText.AppendHyperText (p, pCr ? pCr - p : pEnd - p) :
			pLine->m_HyperText.AppendPlainText (p, pCr ? pCr - p : pEnd - p);

		if (LineLength > m_LongestTextLineLength)
			m_LongestTextLineLength = LineLength;

		if (!pCr)
			return;

		p = pCr + 1;
	}

	for (;;)
	{
		size_t LineLength;

		pLine = AddTextLine (PartCode);
		if (!pLine)
			return;

		pCr = strchr_e (p, pEnd, '\n');

		LineLength = IsHyperText ?
			pLine->m_HyperText.AppendHyperText (p, pCr ? pCr - p : pEnd - p) :
			pLine->m_HyperText.AppendPlainText (p, pCr ? pCr - p : pEnd - p);

		if (LineLength > m_LongestTextLineLength)
			m_LongestTextLineLength = LineLength;

		if (!pCr)
			return;

		p = pCr + 1;
	}
}

void 
CPageRepresenterTarget::AddBinText (
	uint_t PartCode,
	uint_t MergeFlags,
	const void* _p,
	size_t Size
	)
{
	CBinTextLine* pLine;

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;

	size_t ChunkSize;
	size_t Length;

	if (MergeFlags)
	{
		pLine = (CBinTextLine*) GetCurrentLine ();
		ASSERT (pLine && pLine->m_LineKind == ELine_BinText);

		if (MergeFlags & EMergeFlag_OverrideLineAttr)
			pLine->m_LineAttr = m_LineAttr;

		ChunkSize = pLine->AddBinData (m_Timestamp, m_LineAttr, p, Size);
		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;

		Length = pLine->m_BinText.GetLength ();
		if (Length > m_LongestTextLineLength)
			m_LongestTextLineLength = Length;
	}

	while (p < pEnd)
	{
		pLine = AddBinTextLine (PartCode);
		if (!pLine)
			break;

		ChunkSize = pLine->AddBinData (m_Timestamp, m_LineAttr, p, Size);
		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;

		Length = pLine->m_BinText.GetLength ();
		if (Length > m_LongestTextLineLength)
			m_LongestTextLineLength = Length;
	}

	// special case: data block is ended on LF, have to add extra line

	if (*(p - 1) == '\n')
		AddBinTextLine (PartCode);

	if (m_BinDataConfig.m_BinTextLineSize > m_LongestBinTextLineSize)
		m_LongestBinTextLineSize = m_BinDataConfig.m_BinTextLineSize;
}

void 
CPageRepresenterTarget::AddBinHex (
	uint_t PartCode,
	uint_t MergeFlags,
	const void* _p,
	size_t Size
	)
{
	CBinLine* pLine;

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;
	uchar_t* ppp = (uchar_t*) _p;

	size_t ChunkSize;

	if (MergeFlags)
	{
		size_t LastLineSize;

		pLine = (CBinLine*) GetCurrentLine ();
		ASSERT (pLine && pLine->m_LineKind == ELine_BinHex);

		if (MergeFlags & EMergeFlag_OverrideLineAttr)
			pLine->m_LineAttr = m_LineAttr;

		LastLineSize = pLine->m_BinData.GetCount ();
		if (LastLineSize < m_BinDataConfig.m_BinHexLineSize)
		{
			ChunkSize = m_BinDataConfig.m_BinHexLineSize - LastLineSize;
			ChunkSize = AXL_MIN (ChunkSize, Size);
			pLine->AddBinData (m_Timestamp, m_LineAttr, p, ChunkSize);
			p += ChunkSize;
			Size -= ChunkSize;
			m_BinOffset += ChunkSize;
		}
	}	

	while (p < pEnd)
	{
		pLine = AddBinHexLine (PartCode);
		if (!pLine)
			break;

		ChunkSize = AXL_MIN (m_BinDataConfig.m_BinHexLineSize, Size);
		pLine->AddBinData (m_Timestamp, m_LineAttr, p, ChunkSize);
		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;
	}

	if (m_BinDataConfig.m_BinHexLineSize > m_LongestBinHexLineSize)
		m_LongestBinHexLineSize = m_BinDataConfig.m_BinHexLineSize;
}

void
CPageRepresenterTarget::AddLine (
	CLine* pLine,
	uint_t PartCode
	)
{
	ASSERT (CanAddLines ());

	CLine** ppLine = &m_pPage->m_LineArray [m_NextLineIdx];
	ASSERT (!*ppLine);

	*ppLine = pLine;

	pLine->m_pPage = m_pPage;
	pLine->m_IsFirstLineOfPacket = m_IsFirstLineOfPacket;
	pLine->m_LineIdx = m_NextLineIdx;
	pLine->m_VolatilePacketIdx = m_VolatilePacketIdx;
	pLine->m_PartCode = PartCode;
	pLine->m_MergeId = m_MergeId;
	pLine->m_PartIdx = m_PartIdx;
	pLine->m_FirstPacketOffset = m_PacketOffset;
	pLine->m_FirstTimestamp = m_Timestamp;
	pLine->m_LastTimestamp = m_Timestamp;
	pLine->m_LineAttr = m_LineAttr;
	if (!m_IsFirstLineOfPacket && !(m_LineAttr.m_Flags & ELineAttrFlag_TileIcon))
		pLine->m_LineAttr.m_Icon = -1;

	m_IsFirstLineOfPacket = false;

	// link line and volatile packet

	if (pLine->m_VolatilePacketIdx != -1)
	{
		TPageVolatilePacket* pVolatilePacket = &m_pPage->m_VolatilePacketArray [pLine->m_VolatilePacketIdx];
		if (pVolatilePacket->m_FirstLineIdx == -1)
			pVolatilePacket->m_FirstLineIdx = m_NextLineIdx;

		pVolatilePacket->m_LineCount++;
	}

	m_NextLineIdx++;
}

CTextLine* 
CPageRepresenterTarget::AddTextLine (uint_t PartCode)
{
	if (!CanAddLines ())
	{
		m_IsFull = true;
		return NULL;
	}

	CTextLine* pLine = AXL_MEM_NEW (CTextLine);
	AddLine (pLine, PartCode);
	return pLine;
}

CBinTextLine* 
CPageRepresenterTarget::AddBinTextLine (uint_t PartCode)
{
	if (!CanAddLines ())
	{
		m_IsFull = true;
		return NULL;
	}

	CBinTextLine* pLine = AXL_MEM_NEW (CBinTextLine);
	AddLine (pLine, PartCode);
	pLine->m_BinOffset = m_BinOffset;
	pLine->m_BinDataConfig = m_BinDataConfig;
	return pLine;
}

CBinLine* 
CPageRepresenterTarget::AddBinHexLine (uint_t PartCode)
{
	if (!CanAddLines ())
	{
		m_IsFull = true;
		return NULL;
	}

	CBinHexLine* pLine = AXL_MEM_NEW (CBinHexLine);
	AddLine (pLine, PartCode);
	pLine->m_BinOffset = m_BinOffset;	
	pLine->m_BinDataConfig = m_BinDataConfig;
	return pLine;
}

CLine* 
CPageRepresenterTarget::CreatePrevPageMergeLine (
	const TMergeCriteria& MergeCriteria,
	size_t MergeTrailCol
	)
{
	if (!MergeCriteria.m_MergeFlags) // it would not get merged anyway
		return NULL;

	ASSERT (!m_pPrevPageMergeLine);

	switch (MergeCriteria.m_LineKind)
	{
	case ELine_Text:
		{
		CTextLine* pLine = AXL_MEM_NEW (CTextLine);
		pLine->m_HyperText.SetPlainText ('.', MergeTrailCol);
		m_pPrevPageMergeLine = pLine;
		break;
		}

	case ELine_BinText:
		{
		CBinTextLine* pLine = AXL_MEM_NEW (CBinTextLine);
		pLine->m_BinData.SetCount (MergeTrailCol);
		pLine->m_BinText.Copy ('.', MergeTrailCol);
		m_pPrevPageMergeLine = pLine;
		break;
		}

	case ELine_BinHex:
		{
		CBinHexLine* pLine = AXL_MEM_NEW (CBinHexLine);
		pLine->m_BinData.SetCount (MergeTrailCol);
		m_pPrevPageMergeLine = pLine;
		break;
		}

	default:
		ASSERT (false);
	}

	m_pPrevPageMergeLine->m_pPage = m_pPage;
	m_pPrevPageMergeLine->m_LineIdx = -1;
	return m_pPrevPageMergeLine;
}

//.............................................................................

} // namespace log {
} // namespace axl {
