#include "pch.h"
#include "axl_log_MergeCriteria.h"
#include "axl_log_PacketFile.h"

namespace axl {
namespace log {

//.............................................................................

void 
TMergeCriteria::Clear ()
{
	m_LineKind = ELine_Undefined;
	m_PartCode = 0;
	m_Timestamp = 0;
	m_BinDataConfig.SetDefaults ();
}

bool 
TMergeCriteria::ShouldMerge (const TMergeCriteria& Criteria2) const
{
	if (m_Timestamp == -1) // forced merge
		return true;

	if (m_LineKind != Criteria2.m_LineKind ||
		(m_PartCode & ~EPacketCodeFlag_Mergeable) != (Criteria2.m_PartCode & ~EPacketCodeFlag_Mergeable) ||
		!(m_PartCode & EPacketCodeFlag_MergeableForward) ||
		!(Criteria2.m_PartCode & EPacketCodeFlag_MergeableBackward))
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
