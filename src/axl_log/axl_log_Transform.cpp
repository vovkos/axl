#include "pch.h"
#include "axl_log_Transform.h"

namespace axl {
namespace log {

//.............................................................................

void
CTransformMgr::SetSrcFile (CPacketFile* pFile)
{
	TEntry* pHead = *m_Stack.GetHead ();
	if (pHead)
		pHead->m_PrevFileHdrSnapshot.m_ClearCount = -1; // this will effectively force re-processing

	m_pSrcFile = pFile;
}

CPacketFile*
CTransformMgr::AddTransform (
	ITransform* pTransform,
	const char* pFileName,
	uint_t OpenFlags
	)
{
	TEntry* pEntry = AXL_MEM_NEW (TEntry);

	bool Result = pEntry->m_TargetFile.Open (pFileName, OpenFlags);
	if (!Result)
	{
		AXL_MEM_DELETE (pEntry);
		return NULL;
	}

	pEntry->m_pTransform = pTransform;
	memset (&pEntry->m_PrevFileHdrSnapshot, 0, sizeof (TPacketFileHdr));
	pEntry->m_PrevFileHdrSnapshot.m_ClearCount = -1;  // this will effectively force re-processing
	m_Stack.InsertTail (pEntry);
	return &pEntry->m_TargetFile;
}

bool
CTransformMgr::Transform ()
{
	CPacketFile* pSrcFile = m_pSrcFile;

	rtl::CIteratorT <TEntry> It = m_Stack.GetHead ();
	for (; It; It++)
	{	
		bool Result = TransformFile (
			It->m_pTransform, 
			&It->m_TargetFile, 
			pSrcFile, 
			&It->m_PrevFileHdrSnapshot
			);

		if (!Result)
			return false;

		pSrcFile = &It->m_TargetFile;
	}

	return true;
}

bool
CTransformMgr::TransformFile (
	ITransform* pTransform,
	CPacketFile* pDstFile,
	CPacketFile* pSrcFile,
	TPacketFileHdr* pSrcFileHdrSnapshot
	)
{
	TPacketFileHdr* pDstFileHdr = pDstFile->GetHdr ();
	TPacketFileHdr* pSrcFileHdr = pSrcFile->GetHdr ();

	size_t End = pSrcFileHdr->m_TotalPacketSize;

	size_t Offset;

	if (pSrcFileHdr->m_ClearCount != pSrcFileHdrSnapshot->m_ClearCount)
	{
		Offset = 0;
		pDstFile->Clear ();
	}
	else
	{
		Offset = pSrcFileHdrSnapshot->m_TotalPacketSize;
	}

	*pSrcFileHdrSnapshot = *pSrcFileHdr;

	while (Offset < End)
	{
		bool_t Result;
		TPacket* pPacket = pSrcFile->GetPacket (Offset);
		
		pDstFile->m_TimestampOverride = pPacket->m_Timestamp;

		Result = pTransform->Transform (pDstFile, pPacket->m_Code, pPacket + 1, pPacket->m_DataSize);
		if (!Result)
		{
			pDstFile->m_TimestampOverride = 0;
			return false;
		}

		Offset += sizeof (TPacket) + pPacket->m_DataSize;
	}

	pDstFile->m_TimestampOverride = 0;
	return true;
}

//.............................................................................

} // namespace log 
} // namespace axl
