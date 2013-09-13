#include "pch.h"
#include "axl_log_TextLine.h"

namespace axl {
namespace log {

//.............................................................................

size_t
CTextLine::Load (
	const void* p,
	size_t Size
	)
{
	size_t Offset = CLine::Load (p, Size);
	if (!Offset || Size < Offset + sizeof (TTextLine))
		return 0;	
	
	TTextLine* pTextLine = (TTextLine*) ((char*) p + Offset);

	size_t SourceOffset = Offset + sizeof (TTextLine);
	size_t LineSize = SourceOffset + pTextLine->m_SourceLength;
	
	if (Size < LineSize)
		return 0;

	m_HyperText.SetHyperText ((char*) p + SourceOffset, pTextLine->m_SourceLength);
	return LineSize;
}

size_t
CTextLine::Save (rtl::CArrayT <uint8_t>* pBuffer)
{
	CLine::Save (pBuffer);

	rtl::CString Source = m_HyperText.GetSource ();
	size_t SourceLength = Source.GetLength ();

	TTextLine TextLine;
	TextLine.m_SourceLength = SourceLength;

	pBuffer->Append ((uint8_t*) &TextLine, sizeof (TextLine));
	pBuffer->Append ((uint8_t*) Source.cc (), SourceLength);

	return pBuffer->GetCount ();
}

//.............................................................................

} // namespace log {
} // namespace axl {
