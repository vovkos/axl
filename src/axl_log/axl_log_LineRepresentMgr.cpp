#include "pch.h"
#include "axl_log_LineRepresentMgr.h"
#include "axl_log_LineRepresenterTarget.h"

namespace axl {
namespace log {

//.............................................................................

CLineRepresentMgr::CLineRepresentMgr ()
{
	m_pClient = NULL;
	m_pRepresenter = NULL;
	m_pColorizer = NULL;
	m_pPacketFile = NULL;
	m_pMergeFile = NULL;
	m_pColorizerStateFile = NULL;

	m_LastLeafOffset = -1;
	m_LastLinePacketOffset = -1;
	m_LastLinePacketIdx = -1;
	m_LastLineFoldablePacketIdx = -1;
	m_LastLineIdx = -1;
	m_LastLinePartIdx = -1;
	m_LastLineBinOffset = -1;
	m_pLastLine = NULL;
}

void
CLineRepresentMgr::Setup (
	CClient* pClient,
	CRepresenter* pRepresenter,
	CColorizer* pColorizer,
	CPacketFile* pPacketFile,
	CMergeFile* pMergeFile,
	io::CMappedFile* pColorizerStateFile
	)
{
	m_pClient = pClient;
	m_pRepresenter = pRepresenter;
	m_pColorizer = pColorizer;
	m_pPacketFile = pPacketFile;
	m_pMergeFile = pMergeFile;
	m_pColorizerStateFile = pColorizerStateFile;
	m_LastLeafOffset = -1; // effectively clears last line

	size_t StateSize = pColorizer->GetStateSize ();
	m_ColorizerState.SetCount (StateSize);
	m_LastLineColorizerState.SetCount (StateSize);
}

void
CLineRepresentMgr::RepresentPage (
	uint64_t SyncId,
	const TIndexLeaf* pIndexLeaf,
	const uint64_t* pFoldFlagArray
	)
{
	bool IsIncremental = pIndexLeaf->m_Right == -1;
		
	CLineRepresenterTarget RepresenterTarget;
	RepresenterTarget.m_pColorizer = m_pColorizer;
	RepresenterTarget.m_BinDataConfig = pIndexLeaf->m_BinDataConfig;
	RepresenterTarget.m_LineCount = (size_t) pIndexLeaf->m_LineCount;
	RepresenterTarget.m_pLineBuffer = &m_LineBuffer;
	RepresenterTarget.m_pPageBuffer = &m_PageBuffer;

	uint64_t Offset;
	size_t FirstPacketIdx;
	size_t FirstFoldablePacketIdx;
	size_t FirstLineIdx;

	if (IsIncremental && m_LastLeafOffset == pIndexLeaf->m_Offset)
	{
		ASSERT (m_pLastLine);

		Offset = m_LastLinePacketOffset;
		FirstPacketIdx = m_LastLinePacketIdx;
		FirstFoldablePacketIdx = m_LastLineFoldablePacketIdx;
		FirstLineIdx = m_LastLineIdx;

		RepresenterTarget.m_pLinePool = &m_LastLinePool;
		RepresenterTarget.m_pCurrentLine = m_pLastLine;
		RepresenterTarget.m_pColorizerState = m_LastLineColorizerState;
		RepresenterTarget.m_PartIdx = m_LastLinePartIdx;
		RepresenterTarget.m_BinOffset = m_LastLineBinOffset;
		RepresenterTarget.m_MergeCriteria = m_LastLineMergeCriteria;
	}
	else
	{
		Offset = pIndexLeaf->m_PacketOffset;
		FirstPacketIdx = 0;
		FirstFoldablePacketIdx = 0;
		FirstLineIdx = 0;

		RepresenterTarget.m_pLinePool = IsIncremental ? &m_LastLinePool : &m_LinePool;
		RepresenterTarget.m_pColorizerState = IsIncremental ? m_LastLineColorizerState : m_ColorizerState;

		if (pIndexLeaf->m_MergePointIdx != -1)
		{
			const TMergePoint* pMergePoint = m_pMergeFile->GetMergePoint (pIndexLeaf->m_MergePointIdx);
			ASSERT (pMergePoint);

			ELine LineKind = (ELine) pMergePoint->m_LineKind;

			RepresenterTarget.m_PartIdx = pMergePoint->m_PartIdx;
			RepresenterTarget.m_BinOffset = pMergePoint->m_BinOffset;
			RepresenterTarget.m_MergeCriteria.m_Timestamp = -1; // forced merge
			RepresenterTarget.CreatePrevPageMergeLine (LineKind, pMergePoint->m_Col);

			size_t StateSize = m_pColorizer->GetStateSize ();
			const void* p = m_pColorizerStateFile->View (pIndexLeaf->m_MergePointIdx * StateSize, StateSize);
			memcpy (RepresenterTarget.m_pColorizerState, p, StateSize);
		}
	}

	m_PageBuffer.Clear ();
	m_FoldablePacketBuffer.Clear ();
	m_FoldablePacketBuffer.Reserve (pIndexLeaf->m_FoldablePacketCount);

	size_t PacketIdx = FirstPacketIdx;
	size_t FoldablePacketIdx = FirstFoldablePacketIdx;

	for (; PacketIdx < pIndexLeaf->m_PacketCount; PacketIdx++)
	{
		const TPacket* pPacket = m_pPacketFile->GetPacket (Offset);
		ASSERT (pPacket);

		RepresenterTarget.StartPacket (pPacket, Offset);

		if (pPacket->m_Code & EPacketCodeFlag_Mergeable)
		{
			m_pRepresenter->Represent (
				&RepresenterTarget,
				pPacket->m_Code,
				pPacket + 1,
				(size_t) pPacket->m_DataSize,
				0
				);
		}		
		else if (pPacket->m_Code & EPacketCodeFlag_Foldable)
		{
			ASSERT (FoldablePacketIdx < pIndexLeaf->m_FoldablePacketCount);
			ASSERT (RepresenterTarget.m_LineIdx != -1);

			intptr_t PrevLineIdx = RepresenterTarget.m_LineIdx;

			TFoldablePacket FoldablePacket;
			FoldablePacket.m_Offset = Offset;
			FoldablePacket.m_FoldFlags = pFoldFlagArray [FoldablePacketIdx];

			RepresenterTarget.m_FoldablePacketIdx = FoldablePacketIdx;
			RepresenterTarget.m_MergeCriteria.Break ();

			m_pRepresenter->Represent (
				&RepresenterTarget,
				pPacket->m_Code,
				pPacket + 1,
				(size_t) pPacket->m_DataSize,
				FoldablePacket.m_FoldFlags
				);

			FoldablePacket.m_LineCount = RepresenterTarget.m_LineIdx - PrevLineIdx;
			m_FoldablePacketBuffer.Append (FoldablePacket);			
			FoldablePacketIdx++;

			RepresenterTarget.m_FoldablePacketIdx = -1;
			RepresenterTarget.m_MergeCriteria.Break ();
		}
		else // normal: non-mergeable and non-volatile
		{
			RepresenterTarget.m_MergeCriteria.Break ();

			m_pRepresenter->Represent (
				&RepresenterTarget,
				pPacket->m_Code,
				pPacket + 1,
				(size_t) pPacket->m_DataSize,
				0
				);

			RepresenterTarget.m_MergeCriteria.Break ();
		}

		if (RepresenterTarget.m_pCurrentLine)
			RepresenterTarget.m_pCurrentLine->m_Flags |= ELineFlag_LastLineOfPacket;

		// done, go to next packet

		Offset += sizeof (TPacket) + pPacket->m_DataSize;
	}
	
	RepresenterTarget.SaveCurrentLine ();

	if (IsIncremental)
	{
		// save last position for incremental represent

		m_LastLeafOffset = pIndexLeaf->m_Offset;
		m_LastLinePacketOffset = Offset;
		m_LastLinePacketIdx = PacketIdx;
		m_LastLineFoldablePacketIdx = FoldablePacketIdx;
		m_LastLineIdx = (size_t) pIndexLeaf->m_LineCount - 1;

		m_pLastLine = RepresenterTarget.m_pCurrentLine;
		m_LastLinePartIdx = RepresenterTarget.m_PartIdx;
		m_LastLineBinOffset = RepresenterTarget.m_BinOffset;
		m_LastLineMergeCriteria = RepresenterTarget.m_MergeCriteria;
	}

	size_t LineCount = (size_t) pIndexLeaf->m_LineCount - FirstLineIdx;

	m_pClient->RepresentPageComplete (
		SyncId,
		pIndexLeaf->m_Offset,
		FirstLineIdx,
		LineCount,
		FirstFoldablePacketIdx,
		m_FoldablePacketBuffer,
		m_FoldablePacketBuffer.GetCount (),
		m_PageBuffer, 
		m_PageBuffer.GetCount ()
		);
}

size_t
CLineRepresentMgr::RepresentFoldablePacket (
	const TBinDataConfig* pBinDataConfig,
	uint64_t SyncId,
	uint64_t IndexLeafOffset,
	uint64_t PacketOffset,
	size_t FoldablePacketIdx,
	size_t FirstLineIdx,
	size_t OldLineCount,
	uint64_t FoldFlags
	)
{
	bool IsLastLine = 
		IndexLeafOffset == m_LastLeafOffset &&
		FirstLineIdx + OldLineCount - 1 == m_LastLineIdx;

	CLineRepresenterTarget RepresenterTarget;
	RepresenterTarget.m_pColorizer = m_pColorizer;
	RepresenterTarget.m_BinDataConfig = *pBinDataConfig;
	RepresenterTarget.m_pLineBuffer = &m_LineBuffer;
	RepresenterTarget.m_pPageBuffer = &m_PageBuffer;
	RepresenterTarget.m_pLinePool = IsLastLine ? &m_LastLinePool : &m_LinePool;
	RepresenterTarget.m_pColorizerState = IsLastLine ? m_LastLineColorizerState : m_ColorizerState;
	RepresenterTarget.m_FoldablePacketIdx = FoldablePacketIdx;
	RepresenterTarget.m_LineCount = -1;

	m_PageBuffer.Clear ();

	const TPacket* pPacket = m_pPacketFile->GetPacket (PacketOffset);
	ASSERT (pPacket);

	RepresenterTarget.StartPacket (pPacket, PacketOffset);

	m_pRepresenter->Represent (
		&RepresenterTarget,
		pPacket->m_Code,
		pPacket + 1,
		(size_t) pPacket->m_DataSize,
		FoldFlags
		);

	if (RepresenterTarget.m_pCurrentLine)
		RepresenterTarget.m_pCurrentLine->m_Flags |= ELineFlag_LastLineOfPacket;
	
	RepresenterTarget.SaveCurrentLine ();

	size_t NewLineCount = RepresenterTarget.m_LineIdx;

	m_pClient->FoldPacketComplete (
		SyncId,
		IndexLeafOffset,
		FirstLineIdx,
		OldLineCount,
		NewLineCount,
		FoldablePacketIdx,
		FoldFlags,
		m_PageBuffer,
		m_PageBuffer.GetCount ()
		);

	if (IndexLeafOffset == m_LastLeafOffset) // last page
	{
		m_LastLineIdx += NewLineCount - OldLineCount;

		if (IsLastLine)
		{
			 // the rest is not important cause it's not gonna get merged anyway
			m_pLastLine = RepresenterTarget.m_pCurrentLine;
		}
	}

	return NewLineCount;
}

//.............................................................................

} // namespace log {
} // namespace axl {
