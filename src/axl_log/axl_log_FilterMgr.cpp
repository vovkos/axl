#include "pch.h"
#include "axl_log_FilterMgr.h"

namespace axl {
namespace log {

//.............................................................................

uint64_t
CFilterMgr::Process (
	uint64_t SyncId,
	uint64_t TotalPacketSize
	)
{	
	CPacketFile* pPacketFile = m_pSourcePacketFile;

	rtl::CIteratorT <TFilterEntry> It = m_FilterStack.GetHead ();
	for (; It; It++)
	{	
		FilterFile (SyncId, *It, pPacketFile, TotalPacketSize);
		pPacketFile = &It->m_PacketFile;
		TotalPacketSize = pPacketFile->GetHdr ()->m_TotalPacketSize;
	}

	return TotalPacketSize;
}

void
CFilterMgr::FilterFile (
	uint64_t SyncId,
	TFilterEntry* pFilterEntry,
	CPacketFile* pPacketFile,
	uint64_t EndOffset
	)
{
	uint64_t Offset;

	if (pFilterEntry->m_SyncId == SyncId)
	{
		Offset = pFilterEntry->m_LastOffset;
	}
	else
	{
		pFilterEntry->m_PacketFile.Clear ();
		pFilterEntry->m_SyncId = SyncId;
		Offset = 0;
	}

	while (Offset < EndOffset)
	{
		const TPacket* pPacket = pPacketFile->GetPacket (Offset);
		
		pFilterEntry->m_pFilter->Filter (
			&pFilterEntry->m_PacketFile, 
			pPacket->m_Timestamp,
			pPacket->m_Code, 
			pPacket + 1, 
			(size_t) pPacket->m_DataSize
			);

		Offset += sizeof (TPacket) + pPacket->m_DataSize;
	}

	pFilterEntry->m_LastOffset = EndOffset;
}

//.............................................................................

} // namespace log 
} // namespace axl
