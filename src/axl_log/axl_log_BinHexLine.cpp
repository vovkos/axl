#include "pch.h"
#include "axl_log_BinHexLine.h"
#include "axl_log_CachePage.h"

namespace axl {
namespace log { 

//.............................................................................

size_t
CBinHexLine::AddBinData (
	uint64_t Timestamp,
	const gui::TTextAttr& Attr,
	const void* p, 
	size_t Size
	)
{
	if (!m_BinData.IsEmpty ())
	{
		TBinLinePart Part;
		Part.m_Offset = m_BinOffset + m_BinData.GetCount ();
		Part.m_Timestamp = Timestamp;

		m_BinPartArray.Append (Part);
	}

	size_t BinLineSize = GetBinLineSize ();
	size_t Offset = m_BinData.GetCount ();

	ASSERT (Offset < BinLineSize);

	size_t MaxSize = BinLineSize - Offset;
	size_t ChunkSize = AXL_MIN (Size, MaxSize);
	size_t End = Offset + ChunkSize;

	m_BinData.Append ((uchar_t*) p, ChunkSize);
	m_AttrArray.SetAttr (Offset, End, Attr);

	return ChunkSize;
}

void
CBinHexLine::Colorize (
	uint64_t BeginOffset,
	uint64_t EndOffset,
	const gui::TTextAttr& Attr
	)
{
	uint64_t LineEndOffset = m_BinOffset + m_BinData.GetCount ();

	if (BeginOffset < m_BinOffset)
		BeginOffset = m_BinOffset;

	if (EndOffset > LineEndOffset)
		EndOffset = LineEndOffset;

	m_AttrArray.SetAttr (
		(size_t) (BeginOffset - m_BinOffset), 
		(size_t) (EndOffset - m_BinOffset), 
		Attr, 1
		);
}

void
CBinHexLine::UpdateLongestLineLength (TLongestLineLength* pLength)
{
	if (m_BinDataConfig.m_BinHexLineSize > pLength->m_BinHexLineSize)
		pLength->m_BinHexLineSize = m_BinDataConfig.m_BinHexLineSize;
}

//.............................................................................

} // namespace log {
} // namespace axl {
