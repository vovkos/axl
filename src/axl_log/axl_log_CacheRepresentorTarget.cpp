#include "pch.h"
#include "axl_log_CacheRepresentorTarget.h"

namespace axl {
namespace log {

//.............................................................................

CCacheRepresentorTarget::CCacheRepresentorTarget ()
{
	m_pPage = NULL;
	m_pVolatilePacket = NULL;
	m_pPrevPageMergeLine = NULL;
	m_PacketOffset = 0;
	m_Timestamp = 0;
	m_BinOffset = 0;
	m_PartIndex = 0;
	m_MergeId = 0;
	m_Line = 0;
	m_EndLine = 0;
	m_LongestTextLine = 0;
	m_LongestBinHexLine = 0;
	m_LongestBinTextLine = 0;
	m_IsFull = false;
}

CCacheRepresentorTarget::~CCacheRepresentorTarget ()
{
	if (m_pPrevPageMergeLine)
		AXL_MEM_DELETE (m_pPrevPageMergeLine);
}

void 
CCacheRepresentorTarget::AddPart (
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
		m_PartIndex = 0;
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

	m_PartIndex++;
}

void 
CCacheRepresentorTarget::AddText (
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

		if (LineLength > m_LongestTextLine)
			m_LongestTextLine = LineLength;

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

		if (LineLength > m_LongestTextLine)
			m_LongestTextLine = LineLength;

		if (!pCr)
			return;

		p = pCr + 1;
	}
}

void 
CCacheRepresentorTarget::AddBinText (
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

		ChunkSize = pLine->AddData (m_Timestamp, m_LineAttr, p, Size);
		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;

		Length = pLine->m_BinText.GetLength ();
		if (Length > m_LongestTextLine)
			m_LongestTextLine = Length;
	}

	while (p < pEnd)
	{
		pLine = AddBinTextLine (PartCode);
		if (!pLine)
			break;

		ChunkSize = pLine->AddData (m_Timestamp, m_LineAttr, p, Size);
		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;

		Length = pLine->m_BinText.GetLength ();
		if (Length > m_LongestTextLine)
			m_LongestTextLine = Length;
	}

	// special case: data block is ended on LF, have to add extra line

	if (*(p - 1) == '\n')
		AddBinTextLine (PartCode);

	if (m_BinDataConfig.m_BinTextLineSize > m_LongestBinTextLine)
		m_LongestBinTextLine = m_BinDataConfig.m_BinTextLineSize;
}

void 
CCacheRepresentorTarget::AddBinHex (
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
			pLine->AddData (m_Timestamp, m_LineAttr, p, ChunkSize);
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
		pLine->AddData (m_Timestamp, m_LineAttr, p, ChunkSize);
		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;
	}

	if (m_BinDataConfig.m_BinHexLineSize > m_LongestBinHexLine)
		m_LongestBinHexLine = m_BinDataConfig.m_BinHexLineSize;
}

void
CCacheRepresentorTarget::AddLine (
	CLine* pLine,
	uint_t PartCode
	)
{
	CLine** ppLine = (CLine**) m_pPage->m_LineArray + m_Line;

	ASSERT (CanAddLines ());
	ASSERT (m_Line < m_pPage->m_LineArray.GetCount ());
	ASSERT (!*ppLine);

	*ppLine = pLine;

	pLine->m_pPage = m_pPage;
	pLine->m_IsFirstLineOfPacket = m_IsFirstLineOfPacket;
	pLine->m_pVolatilePacket = m_pVolatilePacket;
	pLine->m_PageIndex = m_Line;
	pLine->m_PartCode = PartCode;
	pLine->m_MergeId = m_MergeId;
	pLine->m_PartIndex = m_PartIndex;
	pLine->m_FirstPacketOffset = m_PacketOffset;
	pLine->m_FirstTimestamp = m_Timestamp;
	pLine->m_LastTimestamp = m_Timestamp;
	pLine->m_LineAttr = m_LineAttr;
	if (!m_IsFirstLineOfPacket && !(m_LineAttr.m_Flags & ELineAttrFlag_TileIcon))
		pLine->m_LineAttr.m_Icon = -1;

	m_IsFirstLineOfPacket = false;
	m_Line++;

	// link line and volatile packet

	if (pLine->m_pVolatilePacket)
	{
		if (!pLine->m_pVolatilePacket->m_pFirstLine)
			pLine->m_pVolatilePacket->m_pFirstLine = pLine;

		pLine->m_pVolatilePacket->m_LineCount++;
	}
}

CTextLine* 
CCacheRepresentorTarget::AddTextLine (uint_t PartCode)
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
CCacheRepresentorTarget::AddBinTextLine (uint_t PartCode)
{
	if (!CanAddLines ())
	{
		m_IsFull = true;
		return NULL;
	}

	CBinTextLine* pLine = AXL_MEM_NEW (CBinTextLine);
	AddLine (pLine, PartCode);
	pLine->m_Offset = m_BinOffset;
	pLine->m_BinDataConfig = m_BinDataConfig;
	return pLine;
}

CBinLine* 
CCacheRepresentorTarget::AddBinHexLine (uint_t PartCode)
{
	if (!CanAddLines ())
	{
		m_IsFull = true;
		return NULL;
	}

	CBinLine* pLine = AXL_MEM_NEW (CBinLine);
	AddLine (pLine, PartCode);
	pLine->m_Offset = m_BinOffset;	
	pLine->m_BinDataConfig = m_BinDataConfig;
	return pLine;
}

CLine* 
CCacheRepresentorTarget::CreatePrevPageMergeLine (
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
		pLine->m_BinText.Copy ('.', MergeTrailCol);
		m_pPrevPageMergeLine = pLine;
		break;
		}

	case ELine_BinHex:
		{
		CBinLine* pLine = AXL_MEM_NEW (CBinLine);
		pLine->m_BinData.SetCount (MergeTrailCol);
		m_pPrevPageMergeLine = pLine;
		break;
		}

	default:
		ASSERT (false);
	}

	m_pPrevPageMergeLine->m_pPage = m_pPage;
	m_pPrevPageMergeLine->m_PageIndex = -1;
	return m_pPrevPageMergeLine;
}

//.............................................................................

} // namespace log {
} // namespace axl {
