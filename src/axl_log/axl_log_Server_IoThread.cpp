#include "pch.h"
#include "axl_log_Server.h"
#include "axl_log_IndexRepresenterTarget.h"
#include "axl_log_PageRepresenterTarget.h"

namespace axl {
namespace log {

//.............................................................................

void
CServer::IoThreadProc ()
{
	for (;;)
	{
		m_IoThreadEvent.Wait ();

		m_Lock.Lock ();

		if (m_Flags & EFlag_TerminateIoThread)
		{
			m_Lock.Unlock ();
			return;
		}

		if (m_Flags & EFlag_Clear)
			m_PacketFile.Clear ();
		else if (m_Flags & EFlag_Rebuild)
			m_PacketFile.GetHdr ()->m_SyncId++;

		// take snapshots

		uint_t FlagsSnapshot = m_Flags;
		m_Flags &= ~(EFlag_Clear | EFlag_Rebuild | EFlag_Update);

		TPacketFileHdr PacketFileHdrSnapshot = *m_PacketFile.GetHdr ();
		TBinDataConfig DefBinDataConfigSnapshot = m_DefBinDataConfig;
		
		rtl::CBoxListT <TSrvMsg_Represent> RepresentMsgListSnapshot;
		RepresentMsgListSnapshot.TakeOver (&m_RepresentMsgList);

		m_Lock.Unlock ();	

		if (FlagsSnapshot & (EFlag_Clear | EFlag_Rebuild))
		{
			m_pClient->ClearCache (PacketFileHdrSnapshot.m_SyncId);
			memset (&m_IndexPrevPacketFileHdrSnapshot, 0, sizeof (m_IndexPrevPacketFileHdrSnapshot));
			memset (&m_IndexLeaf, 0, sizeof (m_IndexLeaf));
			m_IndexLeafLineCount = 0;
			m_IndexVolatilePacketArray.Clear ();
		}

		CPacketFile* pPacketFile = &m_PacketFile;

		if (FlagsSnapshot & (EFlag_Clear | EFlag_Rebuild | EFlag_Update))
		{
			rtl::CIteratorT <TFilterEntry> It = m_FilterStack.GetHead ();
			for (; It; It++)
			{	
				FilterFile (*It, pPacketFile, &PacketFileHdrSnapshot);
				pPacketFile = &It->m_PacketFile;
				PacketFileHdrSnapshot = *pPacketFile->GetHdr ();
			}

			UpdateIndex (
				pPacketFile,
				&PacketFileHdrSnapshot,
				&DefBinDataConfigSnapshot
				);
		}
		else if (!m_FilterStack.IsEmpty ())
		{
			rtl::CIteratorT <TFilterEntry> It = m_FilterStack.GetTail ();
			pPacketFile = &It->m_PacketFile;
			PacketFileHdrSnapshot = *pPacketFile->GetHdr ();
		}
		
		while (!RepresentMsgListSnapshot.IsEmpty ())
		{
			rtl::CBoxListEntryT <TSrvMsg_Represent>* pEntry = RepresentMsgListSnapshot.RemoveHeadEntry ();

			if (pEntry->m_Value.m_SyncId == PacketFileHdrSnapshot.m_SyncId)
				RepresentImpl (
					pPacketFile,
					&PacketFileHdrSnapshot,
					pEntry->m_Value.m_IndexNodeOffset,
					pEntry->m_Value.m_IndexNodeLineCount,
					pEntry->m_Value.m_LineIdx,
					&pEntry->m_Value.m_IndexLeaf,
					&pEntry->m_Value.m_PrevIndexLeaf,
					(TIndexVolatilePacket*) (pEntry + 1)
					);

			AXL_MEM_DELETE (pEntry);
		}
	}
}

void
CServer::FilterFile (
	TFilterEntry* pFilterEntry,
	CPacketFile* pPacketFile,
	const TPacketFileHdr* pPacketFileHdrSnapshot
	)
{
	size_t End = pPacketFileHdrSnapshot->m_TotalPacketSize;
	size_t Offset = 0;

	if (pFilterEntry->m_PrevPacketFileHdrSnapshot.m_SyncId == pPacketFileHdrSnapshot->m_SyncId)
	{
		Offset = pFilterEntry->m_PrevPacketFileHdrSnapshot.m_TotalPacketSize;
	}
	else
	{
		pFilterEntry->m_PacketFile.Clear ();
		Offset = 0;
	}

	while (Offset < End)
	{
		TPacket* pPacket = pPacketFile->GetPacket (Offset);
		
		pFilterEntry->m_pFilter->Filter (
			&pFilterEntry->m_PacketFile, 
			pPacket->m_Timestamp,
			pPacket->m_Code, 
			pPacket + 1, 
			pPacket->m_DataSize
			);

		Offset += sizeof (TPacket) + pPacket->m_DataSize;
	}

	pFilterEntry->m_PrevPacketFileHdrSnapshot = *pPacketFileHdrSnapshot;
}

void
CServer::UpdateIndex (
	CPacketFile* pPacketFile,
	const TPacketFileHdr* pPacketFileHdrSnapshot,
	const TBinDataConfig* pDefBinDataConfig
	)
{
	bool IsNewLeaf = false;

	if (m_IndexLeaf.m_DefBinDataConfig.Cmp (*pDefBinDataConfig) != 0)
	{
		IsNewLeaf = m_IndexLeafLineCount != 0;

		memset (&m_IndexLeaf, 0, sizeof (m_IndexLeaf));
		m_IndexLeaf.m_DefBinDataConfig = *pDefBinDataConfig;
		m_IndexLeafLineCount = 0;
	}

	size_t Offset = m_IndexPrevPacketFileHdrSnapshot.m_TotalPacketSize;
	size_t PacketEnd = pPacketFileHdrSnapshot->m_TotalPacketSize;
	size_t SyncId = pPacketFileHdrSnapshot->m_SyncId;

	ASSERT (m_pRepresenter);

	CIndexRepresenterTarget RepresenterTarget;
	RepresenterTarget.m_LineCount = m_IndexLeafLineCount;
	RepresenterTarget.m_Col = m_IndexLeaf.m_Col;
	RepresenterTarget.m_BinOffset = m_IndexLeaf.m_BinOffset;
	RepresenterTarget.m_PartIdx = m_IndexLeaf.m_PartIdx;
	RepresenterTarget.m_MergeId = m_IndexLeaf.m_MergeId;
	RepresenterTarget.m_MergeCriteria = m_IndexLeaf.m_MergeCriteria;

	size_t PacketCount = m_IndexLeaf.m_PacketCount;

	while (Offset < PacketEnd)
	{
		TPacket* pPacket = pPacketFile->GetPacket (Offset);

		RepresenterTarget.m_PacketCode = pPacket->m_Code;
		RepresenterTarget.m_Timestamp = pPacket->m_Timestamp;
		RepresenterTarget.m_BinDataConfig = *pDefBinDataConfig;

		if (pPacket->m_Code & EPacketCodeFlag_Mergeable)
		{
			TIndexRepresenterTargetData RepresenterTargetSnapshot = RepresenterTarget;

			m_pRepresenter->Represent (
				&RepresenterTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				pPacket->m_DataSize,
				0
				);

			// first add node using snapshot (if needed)...

			if (RepresenterTargetSnapshot.m_LineCount >= m_IndexLeafLineCountLimit && 
				RepresenterTarget.m_LineCount > RepresenterTargetSnapshot.m_LineCount)
			{
				FinalizeIndexLeaf (&RepresenterTargetSnapshot, PacketCount + 1, IsNewLeaf);

				IsNewLeaf = true;
				m_IndexVolatilePacketArray.Clear ();
				m_IndexLeaf.m_PacketOffset = Offset;	
				RepresenterTarget.m_LineCount -= RepresenterTargetSnapshot.m_LineCount;
				PacketCount = 0;
			}

			// ...then advance

			Offset += sizeof (TPacket) + pPacket->m_DataSize;
			PacketCount++;
		}
		else if (pPacket->m_Code & EPacketCodeFlag_Volatile)
		{	
			uint_t VolatileFlags = 0;
			size_t OldLineCount = RepresenterTarget.m_LineCount;

			RepresenterTarget.m_MergeCriteria.Break ();

			m_pRepresenter->Represent (
				&RepresenterTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				pPacket->m_DataSize,
				VolatileFlags
				);

			TIndexVolatilePacket VolatilePacketData;
			VolatilePacketData.m_VolatileFlags = VolatileFlags;
			VolatilePacketData.m_Offset = (uint32_t) Offset;
			VolatilePacketData.m_LineCount = (uint32_t) (RepresenterTarget.m_LineCount - OldLineCount);
			m_IndexVolatilePacketArray.Append (VolatilePacketData);

			RepresenterTarget.m_MergeCriteria.Break ();

			// first advance...

			Offset += sizeof (TPacket) + pPacket->m_DataSize;
			PacketCount++;

			// then add new empty node (if needed)
			
			size_t VolatilePacketCount = m_IndexVolatilePacketArray.GetCount ();

			if (RepresenterTarget.m_LineCount >= m_IndexLeafLineCountLimit || 
				VolatilePacketCount >= m_IndexVolatilePacketCountLimit)
			{
				FinalizeIndexLeaf (&RepresenterTarget, PacketCount - 1, IsNewLeaf);

				IsNewLeaf = true;
				m_IndexVolatilePacketArray.Clear ();
				m_IndexLeaf.m_PacketOffset = Offset;
				RepresenterTarget.m_LineCount = 0;
				PacketCount = 0;
			}
		}
		else // normal: non-mergeable and non-volatile
		{	
			RepresenterTarget.m_MergeCriteria.Break ();

			m_pRepresenter->Represent (
				&RepresenterTarget, 
				pPacket->m_Code, 
				pPacket + 1,
				pPacket->m_DataSize,
				0
				);

			RepresenterTarget.m_MergeCriteria.Break ();

			// first advance...

			Offset += sizeof (TPacket) + pPacket->m_DataSize;
			PacketCount++;

			// then add new empty node (if needed)
			
			if (RepresenterTarget.m_LineCount >= m_IndexLeafLineCountLimit)
			{
				FinalizeIndexLeaf (&RepresenterTarget, PacketCount - 1, IsNewLeaf);

				IsNewLeaf = true;
				m_IndexLeaf.m_PacketOffset = Offset;
				m_IndexVolatilePacketArray.Clear ();
				RepresenterTarget.m_LineCount = 0;
				PacketCount = 0;
			}
		}
	}

	FinalizeIndexLeaf (&RepresenterTarget, PacketCount, IsNewLeaf);

	m_IndexLeafLineCount = RepresenterTarget.m_LineCount;
	m_IndexPrevPacketFileHdrSnapshot = *pPacketFileHdrSnapshot;
}

void
CServer::FinalizeIndexLeaf (
	const TIndexRepresenterTargetData* pRepresenterTarget,
	size_t PacketCount,
	bool IsNewLeaf
	)
{
	m_IndexLeaf.m_PacketCount = PacketCount;
	m_IndexLeaf.m_VolatilePacketCount = m_IndexVolatilePacketArray.GetCount ();
	m_IndexLeaf.m_Col = (uint32_t) pRepresenterTarget->m_Col;
	m_IndexLeaf.m_BinOffset = (uint32_t) pRepresenterTarget->m_BinOffset;
	m_IndexLeaf.m_PartIdx = (uint32_t) pRepresenterTarget->m_PartIdx;
	m_IndexLeaf.m_MergeId = (uint32_t) pRepresenterTarget->m_MergeId;
	m_IndexLeaf.m_MergeCriteria = pRepresenterTarget->m_MergeCriteria;

	m_pClient->SetIndexTailLeaf (IsNewLeaf, pRepresenterTarget->m_LineCount, &m_IndexLeaf, m_IndexVolatilePacketArray);
}

void
CServer::RepresentImpl (
	CPacketFile* pPacketFile,
	const TPacketFileHdr* pPacketFileHdrSnapshot,
	size_t IndexNodeOffset,
	size_t IndexNodeLineCount,
	size_t LineIdx,
	const TIndexLeaf* pIndexLeaf,
	const TIndexLeaf* pPrevIndexLeaf,
	const TIndexVolatilePacket* pVolatilePacketArray
	)
{
	CPage* pPage;
	CPage TmpPage;

	if (IndexNodeOffset == m_LastPage.m_IndexNodeOffset)
	{
		pPage = &m_LastPage;
	}
	else if (m_LastPage.m_IndexNodeOffset == -1 || IndexNodeOffset > m_LastPage.m_IndexNodeOffset)
	{
		m_LastPage.Clear ();
		pPage = &m_LastPage;
	}
	else
	{
		pPage = &TmpPage;
	}

	size_t OldLineCount = pPage->m_LineArray.GetCount ();
	size_t PacketCount = pIndexLeaf->m_PacketCount;
	size_t PacketIdx = pPage->m_IndexLeaf.m_PacketCount;
	size_t VolatilePacketIdx = pPage->m_VolatilePacketArray.GetCount ();

	const TIndexVolatilePacket* pVolatilePacket = pVolatilePacketArray;
	const TIndexVolatilePacket* pVolatilePacketEnd = pVolatilePacket + pIndexLeaf->m_VolatilePacketCount;

	pVolatilePacket += VolatilePacketIdx;

	// append lines and volatile packets to the page

	ASSERT (IndexNodeLineCount >= OldLineCount);

	if (IndexNodeLineCount > OldLineCount)
		pPage->m_LineArray.Append (NULL, IndexNodeLineCount - OldLineCount);

	pPage->m_VolatilePacketArray.Reserve (pIndexLeaf->m_VolatilePacketCount);

//  uncomment if you want a break at the new page caching
//
//	if (!OldLineCount)
//		_asm int 3

	CPageRepresenterTarget RepresenterTarget;
	RepresenterTarget.m_pPage = pPage;
	RepresenterTarget.m_BinDataConfig = pIndexLeaf->m_DefBinDataConfig;
	RepresenterTarget.m_BinOffset = pPage->m_IndexLeaf.m_BinOffset;
	RepresenterTarget.m_MergeId = pPage->m_IndexLeaf.m_MergeId;
	RepresenterTarget.m_PartIdx = pPage->m_IndexLeaf.m_PartIdx;
	RepresenterTarget.m_MergeCriteria = pPage->m_IndexLeaf.m_MergeCriteria;
	RepresenterTarget.m_NextLineIdx = OldLineCount;
	RepresenterTarget.m_EndLineIdx = IndexNodeLineCount;

	ASSERT (RepresenterTarget.m_NextLineIdx <= RepresenterTarget.m_EndLineIdx);
	
	size_t Offset;
		
	if (OldLineCount)
	{
		Offset = pPage->m_PacketOffset;
	}
	else // merging with previous node
	{		
		Offset = pIndexLeaf->m_PacketOffset;

		RepresenterTarget.m_MergeCriteria = pPrevIndexLeaf->m_MergeCriteria;
		RepresenterTarget.m_BinOffset = pPrevIndexLeaf->m_BinOffset;
		RepresenterTarget.m_PartIdx = pPrevIndexLeaf->m_PartIdx;
		RepresenterTarget.m_MergeId = pPrevIndexLeaf->m_MergeId;
		RepresenterTarget.CreatePrevPageMergeLine (pPrevIndexLeaf->m_MergeCriteria, pPrevIndexLeaf->m_Col);
	}

	for (; PacketIdx < PacketCount; PacketIdx++)
	{
		TPacket* pPacket = pPacketFile->GetPacket (Offset);
		ASSERT (pPacket);

		RepresenterTarget.m_IsFirstLineOfPacket = true;
		RepresenterTarget.m_PacketCode = pPacket->m_Code;
		RepresenterTarget.m_PacketOffset = Offset;
		RepresenterTarget.m_Timestamp = pPacket->m_Timestamp;
		RepresenterTarget.m_BinDataConfig = pPage->m_IndexLeaf.m_DefBinDataConfig;
		RepresenterTarget.m_LineAttr.Clear ();

		if (pPacket->m_Code & EPacketCodeFlag_Mergeable)
		{
			RepresenterTarget.m_VolatilePacketIdx = -1;

			m_pRepresenter->Represent (
				&RepresenterTarget,
				pPacket->m_Code,
				pPacket + 1,
				pPacket->m_DataSize,
				0
				);
		}		
		else if (pPacket->m_Code & EPacketCodeFlag_Volatile)
		{
			ASSERT (pVolatilePacket < pVolatilePacketEnd);

			pPage->m_VolatilePacketArray.SetCount (VolatilePacketIdx + 1);
			TPageVolatilePacket* pPageVolatilePacket = &pPage->m_VolatilePacketArray [VolatilePacketIdx];
			pPageVolatilePacket->m_Offset = Offset;
			pPageVolatilePacket->m_Code = pPacket->m_Code;
			pPageVolatilePacket->m_Timestamp = pPacket->m_Timestamp;
			pPageVolatilePacket->m_DataSize = pPacket->m_DataSize;
			pPageVolatilePacket->m_VolatileIdx = VolatilePacketIdx;
			pPageVolatilePacket->m_VolatileFlags = pVolatilePacket->m_VolatileFlags;
			pPageVolatilePacket->m_FirstLineIdx = -1;
			pPageVolatilePacket->m_LineCount = 0;

			OldLineCount = RepresenterTarget.m_NextLineIdx;

			RepresenterTarget.m_VolatilePacketIdx = VolatilePacketIdx;
			RepresenterTarget.m_MergeCriteria.Break ();

			m_pRepresenter->Represent (
				&RepresenterTarget,
				pPacket->m_Code,
				pPacket + 1,
				pPacket->m_DataSize,
				pVolatilePacket->m_VolatileFlags
				);

			ASSERT (OldLineCount + pPageVolatilePacket->m_LineCount == RepresenterTarget.m_NextLineIdx);

			RepresenterTarget.m_MergeCriteria.Break ();

			VolatilePacketIdx++;
			pVolatilePacket++;
		}
		else // normal: non-mergeable and non-volatile
		{
			RepresenterTarget.m_MergeCriteria.Break ();

			m_pRepresenter->Represent (
				&RepresenterTarget,
				pPacket->m_Code,
				pPacket + 1,
				pPacket->m_DataSize,
				0
				);

			RepresenterTarget.m_MergeCriteria.Break ();
		}

		// done, go to next packet

		Offset += sizeof (TPacket) + pPacket->m_DataSize;
	}

	ASSERT (!pPage->m_LineArray.IsEmpty ());

	if (RepresenterTarget.m_LongestTextLineLength > pPage->m_LongestTextLineLength)
		pPage->m_LongestTextLineLength = RepresenterTarget.m_LongestTextLineLength;

	if (RepresenterTarget.m_LongestBinHexLineSize > pPage->m_LongestBinHexLineSize)
		pPage->m_LongestBinHexLineSize = RepresenterTarget.m_LongestBinHexLineSize;

	if (RepresenterTarget.m_LongestBinTextLineSize > pPage->m_LongestBinTextLineSize)
		pPage->m_LongestBinTextLineSize = RepresenterTarget.m_LongestBinTextLineSize;

	pPage->m_IndexLeaf = *pIndexLeaf;
	pPage->m_PacketOffset = Offset;

	m_LineBuffer.Clear ();
	pPage->SaveLines (&m_LineBuffer, LineIdx);

	m_pClient->RepresentComplete (
		pPacketFileHdrSnapshot->m_SyncId,
		IndexNodeOffset,
		IndexNodeLineCount,
		LineIdx,
		m_LineBuffer, 
		m_LineBuffer.GetCount ()
		);
}

//.............................................................................

} // namespace log 
} // namespace axl