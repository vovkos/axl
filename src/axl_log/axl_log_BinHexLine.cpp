#include "pch.h"
#include "axl_log_BinHexLine.h"
#include "axl_log_Page.h"

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
	size_t ChunkSize = min (Size, MaxSize);
	size_t End = Offset + ChunkSize;

	m_BinData.Append ((uchar_t*) p, ChunkSize);
	
	m_OriginalAttrArray.SetAttr (Attr, Offset, End);
	m_AttrArray.SetAttr (Attr, Offset, End);

	return ChunkSize;
}

void
CBinHexLine::Colorize (
	const gui::TTextAttr& Attr,
	size_t OffsetStart,
	size_t OffsetEnd,
	size_t Metric
	)
{
	size_t Size = m_BinData.GetCount ();
	if (OffsetStart > Size)
		return;

	if (OffsetEnd > Size)
		OffsetEnd = Size;

	m_AttrArray.SetAttr (Attr, OffsetStart, OffsetEnd, Metric);
}

//.............................................................................

} // namespace log {
} // namespace axl {
