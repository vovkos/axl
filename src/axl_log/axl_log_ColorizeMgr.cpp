#include "pch.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_log_ColorizerStateCalcTarget.h"

namespace axl {
namespace log {

//.............................................................................

CColorizeMgr::CColorizeMgr ()
{
	m_pRepresenter = NULL;
	m_pColorizer = NULL;
	m_pPacketFile = NULL;
	m_pMergeFile = NULL;
	m_pColorizerStateFile = NULL;

	m_PacketOffset = 0;
	m_MergePointIdx = 0;
}

void
CColorizeMgr::Reset ()
{
	m_PacketOffset = 0;
	m_MergePointIdx = 0;
	m_MergeCriteria.Clear ();
	memset (m_ColorizerState, 0, m_ColorizerState.GetCount ());
}

void
CColorizeMgr::Setup (
	IRepresenter* pRepresenter,
	IColorizer* pColorizer,
	CPacketFile* pPacketFile,
	CMergeFile* pMergeFile,
	io::CMappedFile* pColorizerStateFile
	)
{
	Reset ();

	m_pRepresenter = pRepresenter;
	m_pColorizer = pColorizer;
	m_pPacketFile = pPacketFile;
	m_pMergeFile = pMergeFile;
	m_pColorizerStateFile = pColorizerStateFile;

	size_t StateSize = m_pColorizer->GetStateSize ();
	m_ColorizerState.SetCount (StateSize);
}

void
CColorizeMgr::UpdateColorizerStateFile ()
{
	uint64_t PacketOffset = m_PacketOffset;
	uint64_t MergePointIdx = m_MergePointIdx;
	uint64_t MergePointCount = m_pMergeFile->GetHdr ()->m_MergePointCount;

	if (MergePointIdx >= MergePointCount) // no new merge points to update
		return;

	size_t StateSize = m_pColorizer->GetStateSize ();

	ASSERT (m_pRepresenter);
	ASSERT (m_pColorizer);

	CColorizerStateCalcTarget Target;
	Target.m_pColorizer = m_pColorizer;
	Target.m_pColorizerState = m_ColorizerState;
	Target.m_MergeCriteria = m_MergeCriteria;

	for (; MergePointIdx < MergePointCount; MergePointIdx++)
	{
		const TMergePoint* pMergePoint = m_pMergeFile->GetMergePoint (MergePointIdx);

		if (pMergePoint->m_ColorizerStateCalcPacketOffset != PacketOffset)
		{
			ASSERT (pMergePoint->m_ColorizerStateCalcPacketOffset > PacketOffset);
			Target.m_MergeCriteria.Break ();
			PacketOffset = pMergePoint->m_ColorizerStateCalcPacketOffset;
		}

		pMergePoint->m_ColorizerStateCalcPacketOffset;
		for (size_t i = 0; i < pMergePoint->m_ColorizerStateCalcPacketCount; i++)
		{
			const TPacket* pPacket = m_pPacketFile->GetPacket (PacketOffset);
			if (!(pPacket->m_Code & EPacketCodeFlag_Mergeable))
			{
				PacketOffset += sizeof (TPacket) + pPacket->m_DataSize;
				continue;
			}

			Target.StartPacket (pPacket);

			m_pRepresenter->Represent (
				&Target,
				pPacket->m_Code,
				pPacket + 1,
				(size_t) pPacket->m_DataSize,
				0
				);

			PacketOffset += sizeof (TPacket) + pPacket->m_DataSize;
		}

		void* p = m_pColorizerStateFile->View (MergePointIdx * StateSize, StateSize);
		memcpy (p, m_ColorizerState, StateSize);
	}

	m_PacketOffset = PacketOffset;
	m_MergePointIdx = MergePointCount;
	m_MergeCriteria = Target.m_MergeCriteria;
}

//.............................................................................

} // namespace log {
} // namespace axl {

