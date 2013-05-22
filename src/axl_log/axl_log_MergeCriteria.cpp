#include "pch.h"
#include "axl_log_MergeCriteria.h"
#include "axl_log_Line.h"

namespace axl {
namespace log {

//.............................................................................

TMergeCriteria::TMergeCriteria ()
{
	m_PartCode = 0;
	m_MergeFlags = 0;
	m_LineKind = ELine_Text;
	m_Timestamp = 0;
}

bool 
TMergeCriteria::ShouldMerge (const TMergeCriteria& Criteria2) const
{
	if (m_PartCode != Criteria2.m_PartCode ||
		m_LineKind != Criteria2.m_LineKind ||
		!(m_MergeFlags & EMergeFlag_MergeableForward) ||
		!(Criteria2.m_MergeFlags & EMergeFlag_MergeableBackward))
		return false;

	if (m_LineKind == ELine_Text)
		return true;

	uint64_t TimeSpan = Criteria2.m_Timestamp - m_Timestamp;
	return 
		TimeSpan <= Criteria2.m_BinDataConfig.m_BinMergeThreshold &&
		m_BinDataConfig.Cmp (Criteria2.m_BinDataConfig) == 0;
}

//.............................................................................

} // namespace log
} // namespace axl
