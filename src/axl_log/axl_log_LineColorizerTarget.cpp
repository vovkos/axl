#include "pch.h"
#include "axl_log_LineColorizerTarget.h"

namespace axl {
namespace log {

//.............................................................................

void 
CLineColorizerTarget::SetAttr (
	uint64_t BeginOffset, 
	uint64_t EndOffset, 
	const gui::TTextAttr& Attr
	)
{
	if (m_pLine->DoesBinRangeOverlap (BeginOffset, EndOffset))
		m_pLine->Colorize (BeginOffset, EndOffset, Attr);

	if (BeginOffset < m_pLine->GetBinOffset () && (m_pLine->GetFlags () & ELineFlag_MergedBackward))
		m_pLine->AddRetroColorizedPart (BeginOffset, EndOffset, Attr);
}

//.............................................................................

} // namespace log 
} // namespace axl
