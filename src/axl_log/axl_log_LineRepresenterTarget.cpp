#include "pch.h"
#include "axl_log_LineRepresenterTarget.h"
#include "axl_log_LineColorizerTarget.h"
#include "axl_log_PacketFile.h"

namespace axl {
namespace log {

//.............................................................................

CLineRepresenterTarget::CLineRepresenterTarget ()
{
	m_pColorizer = NULL;
	m_pLinePool = NULL;
	m_pLineBuffer = NULL;
	m_pPageBuffer = NULL;
	m_pCurrentLine = NULL;
	m_pColorizerState = NULL;
	m_FoldablePacketIdx = -1;
	m_PacketOffset = 0;
	m_Timestamp = 0;
	m_BinOffset = 0;
	m_PartIdx = 0;
	m_LineIdx = 0;
	m_LineCount = 0;
	m_IsFull = false;
	m_IsFirstLineOfPacket = false;
	m_IsCurrentLineSaved = false;
}

void
CLineRepresenterTarget::StartPacket (
	const TPacket* pPacket,
	uint64_t PacketOffset
	)
{
	m_IsFirstLineOfPacket = true;
	m_PacketCode = pPacket->m_Code;
	m_PacketOffset = PacketOffset;
	m_Timestamp = pPacket->m_Timestamp;
	m_LineAttr.Clear ();
}

void
CLineRepresenterTarget::SaveCurrentLine ()
{
	if (!m_pCurrentLine || m_IsCurrentLineSaved)
		return;

	m_pCurrentLine->Save (m_pLineBuffer);
	m_pPageBuffer->Append (*m_pLineBuffer);
	m_IsCurrentLineSaved = true;
}

void 
CLineRepresenterTarget::AddPart (
	EPart PartKind,
	uint_t PartCode,
	const void* p,
	size_t Size
	)
{
	if (Size == 0 || m_IsFull)
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
		m_PartIdx = 0;
		m_BinOffset = 0;
		memset (m_pColorizerState, 0, m_pColorizer->GetStateSize ());
	}
	else
	{
		m_pCurrentLine->m_LastTimestamp = m_Timestamp;
		if (m_LineAttr.m_Flags & ELineAttrFlag_Override)
			m_pCurrentLine->m_LineAttr = m_LineAttr;
	}

	m_MergeCriteria = MergeCriteria;

	switch (LineKind)
	{
	case ELine_Text:
		AddText (ShouldMerge, PartCode, (const char*) p, Size / sizeof (char), PartKind == EPart_HyperText);
		break;

	case ELine_BinHex:
		AddBinHex (ShouldMerge, PartCode, p, Size);
		break;

	case ELine_BinText:
		AddBinText (ShouldMerge, PartCode, p, Size);
		break;
	}

	m_PartIdx++;
}

void 
CLineRepresenterTarget::AddText (
	bool ShouldMerge,
	uint_t PartCode,
	const char* pText,
	size_t Length,
	bool IsHyperText
	)
{
	CTextLine* pLine;

	const char* p = pText;
	const char* pEnd = pText + Length;
	const char* pCr;

	if (ShouldMerge)
	{
		pLine = (CTextLine*) m_pCurrentLine;
		ASSERT (pLine && pLine->m_LineKind == ELine_Text);

		pCr = strchr_e (p, pEnd, '\n');
		
		size_t LineLength = IsHyperText ?
			pLine->m_HyperText.AppendHyperText (p, pCr ? pCr - p : pEnd - p) :
			pLine->m_HyperText.AppendPlainText (p, pCr ? pCr - p : pEnd - p);

		if (!pCr)
			return;

		p = pCr + 1;
	}

	for (;;)
	{
		size_t LineLength;

		pLine = AddTextLine (ShouldMerge, PartCode);
		if (!pLine)
			return;

		ShouldMerge = true;
		pCr = strchr_e (p, pEnd, '\n');

		LineLength = IsHyperText ?
			pLine->m_HyperText.AppendHyperText (p, pCr ? pCr - p : pEnd - p) :
			pLine->m_HyperText.AppendPlainText (p, pCr ? pCr - p : pEnd - p);

		if (!pCr)
			return;

		p = pCr + 1;
	}
}

void 
CLineRepresenterTarget::AddBinText (
	bool ShouldMerge,
	uint_t PartCode,
	const void* _p,
	size_t Size
	)
{
	CBinTextLine* pLine;

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;

	size_t ChunkSize;

	if (ShouldMerge)
	{
		pLine = (CBinTextLine*) m_pCurrentLine;
		ASSERT (pLine && pLine->m_LineKind == ELine_BinText);

		ChunkSize = pLine->AddBinData (m_Timestamp, m_LineAttr, p, Size);
		ColorizeLine (pLine, p, ChunkSize);

		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;
	}

	while (p < pEnd)
	{
		pLine = AddBinTextLine (ShouldMerge, PartCode);
		if (!pLine)
			break;

		ShouldMerge = true;
		ChunkSize = pLine->AddBinData (m_Timestamp, m_LineAttr, p, Size);
		ColorizeLine (pLine, p, ChunkSize);

		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;
	}

	// special case: data block is ended on LF, have to add extra line

	if (*(p - 1) == '\n')
		AddBinTextLine (true, PartCode);
}

void 
CLineRepresenterTarget::AddBinHex (
	bool ShouldMerge,
	uint_t PartCode,
	const void* _p,
	size_t Size
	)
{
	CBinLine* pLine;

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;
	uchar_t* ppp = (uchar_t*) _p;

	size_t ChunkSize;

	if (ShouldMerge)
	{
		pLine = (CBinLine*) m_pCurrentLine;
		ASSERT (pLine && pLine->m_LineKind == ELine_BinHex);

		size_t LastLineSize = pLine->m_BinData.GetCount ();
		if (LastLineSize < m_BinDataConfig.m_BinHexLineSize)
		{
			ChunkSize = m_BinDataConfig.m_BinHexLineSize - LastLineSize;
			ChunkSize = AXL_MIN (ChunkSize, Size);

			pLine->AddBinData (m_Timestamp, m_LineAttr, p, ChunkSize);
			ColorizeLine (pLine, p, ChunkSize);

			p += ChunkSize;
			Size -= ChunkSize;
			m_BinOffset += ChunkSize;
		}
	}	

	while (p < pEnd)
	{
		pLine = AddBinHexLine (ShouldMerge, PartCode);
		if (!pLine)
			break;

		ShouldMerge = true;
		ChunkSize = AXL_MIN (m_BinDataConfig.m_BinHexLineSize, Size);

		pLine->AddBinData (m_Timestamp, m_LineAttr, p, ChunkSize);
		ColorizeLine (pLine, p, ChunkSize);

		p += ChunkSize;
		Size -= ChunkSize;
		m_BinOffset += ChunkSize;
	}
}

bool
CLineRepresenterTarget::AddLine (
	bool ShouldMerge,
	CLine* pLine,
	uint_t PartCode
	)
{
	if (m_pCurrentLine && ShouldMerge)
		m_pCurrentLine->m_Flags |= ELineFlag_MergedForward;

	if (!CanAddLines ())
	{
		m_IsFull = true;
		return false;
	}

	SaveCurrentLine ();

	pLine->Clear ();
	pLine->m_LineIdx = m_LineIdx;
	pLine->m_FoldablePacketIdx = m_FoldablePacketIdx;
	pLine->m_PartCode = PartCode;
	pLine->m_PartIdx = m_PartIdx;
	pLine->m_FirstPacketOffset = m_PacketOffset;
	pLine->m_FirstTimestamp = m_Timestamp;
	pLine->m_LastTimestamp = m_Timestamp;
	pLine->m_LineAttr = m_LineAttr;

	if (m_IsFirstLineOfPacket)
		pLine->m_Flags |= ELineFlag_FirstLineOfPacket;

	if (ShouldMerge)
		pLine->m_Flags |= ELineFlag_MergedBackward;

	m_IsFirstLineOfPacket = false;
	m_IsCurrentLineSaved = false;

	m_pCurrentLine = pLine;
	m_LineIdx++;
	return true;
}

CTextLine* 
CLineRepresenterTarget::AddTextLine (
	bool ShouldMerge,
	uint_t PartCode
	)
{
	CTextLine* pLine = &m_pLinePool->m_TextLine;
	bool Result = AddLine (ShouldMerge, pLine, PartCode);
	return Result ? pLine : NULL;
}

CBinTextLine* 
CLineRepresenterTarget::AddBinTextLine (
	bool ShouldMerge,
	uint_t PartCode
	)
{
	CBinTextLine* pLine = &m_pLinePool->m_BinTextLine;
	bool Result = AddLine (ShouldMerge, pLine, PartCode);
	if (!Result)
		return NULL;

	pLine->m_BinOffset = m_BinOffset;
	pLine->m_BinDataConfig = m_BinDataConfig;
	return pLine;
}

CBinLine* 
CLineRepresenterTarget::AddBinHexLine (
	bool ShouldMerge,
	uint_t PartCode
	)
{
	CBinHexLine* pLine = &m_pLinePool->m_BinHexLine;
	bool Result = AddLine (ShouldMerge, pLine, PartCode);
	if (!Result)
		return NULL;

	pLine->m_BinOffset = m_BinOffset;
	pLine->m_BinDataConfig = m_BinDataConfig;
	return pLine;
}

CLine* 
CLineRepresenterTarget::CreatePrevPageMergeLine (
	ELine MergeLineKind,
	size_t MergeTrailCol
	)
{
	switch (MergeLineKind)
	{
	case ELine_Text:
		{
		CTextLine* pLine = &m_pLinePool->m_TextLine;
		pLine->m_HyperText.SetPlainText ('.', MergeTrailCol);
		m_pCurrentLine = pLine;
		break;
		}

	case ELine_BinText:
		{
		CBinTextLine* pLine = &m_pLinePool->m_BinTextLine;
		pLine->m_BinData.SetCount (MergeTrailCol);
		pLine->m_BinText.Copy ('.', MergeTrailCol);
		m_pCurrentLine = pLine;
		break;
		}

	case ELine_BinHex:
		{
		CBinHexLine* pLine = &m_pLinePool->m_BinHexLine;
		pLine->m_BinData.SetCount (MergeTrailCol);
		m_pCurrentLine = pLine;
		break;
		}

	default:
		ASSERT (false);
	}

	m_pCurrentLine->m_LineIdx = -1;
	m_IsCurrentLineSaved = true; // don't save it
	return m_pCurrentLine;
}

void
CLineRepresenterTarget::ColorizeLine (
	CBinLine* pLine,
	const void* p,
	size_t Size
	)
{
	CLineColorizerTarget ColorizerTarget;
	ColorizerTarget.m_pLine = pLine;
	m_pColorizer->Colorize (&ColorizerTarget, p, Size, false, m_pColorizerState);

}

//.............................................................................

} // namespace log {
} // namespace axl {
