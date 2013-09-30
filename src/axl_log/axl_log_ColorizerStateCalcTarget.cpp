#include "pch.h"
#include "axl_log_ColorizerStateCalcTarget.h"

namespace axl {
namespace log {

//.............................................................................

CColorizerStateCalcTarget::CColorizerStateCalcTarget ()
{
	m_pColorizer = NULL;
	m_pColorizerState = NULL;
}

void
CColorizerStateCalcTarget::StartPacket (const TPacket* pPacket)
{
	m_PacketCode = pPacket->m_Code;
	m_Timestamp = pPacket->m_Timestamp;
}

void 
CColorizerStateCalcTarget::AddPart (
	EPart PartKind,
	uint_t PartCode,
	const void* p,
	size_t Size
	)
{
	if (PartKind != EPart_Bin || 
		!(PartCode & EPacketCodeFlag_Mergeable) || 
		Size == 0)
	{
		m_MergeCriteria.Clear ();
		return;
	}

	TMergeCriteria MergeCriteria;
	MergeCriteria.m_PartCode = PartCode;
	MergeCriteria.m_BinDataConfig = m_BinDataConfig;
	MergeCriteria.m_Timestamp = m_Timestamp;
	MergeCriteria.m_LineKind = ELine_BinHex;

	bool ShouldMerge = m_MergeCriteria.ShouldMerge (MergeCriteria);
	if (!ShouldMerge)
		m_pColorizer->Finalize (this, m_pColorizerState);

	m_MergeCriteria = MergeCriteria;
		
	bool IsFinal = !(PartCode & EPacketCodeFlag_MergeableForward);
	m_pColorizer->Colorize (this, p, Size, IsFinal, m_pColorizerState);
}

//.............................................................................

} // namespace log 
} // namespace axl
