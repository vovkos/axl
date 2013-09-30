#include "pch.h"
#include "axl_log_MergeFile.h"
#include "axl_err_Error.h"
#include "axl_g_Time.h"

namespace axl {
namespace log {

//.............................................................................

bool
CMergeFile::Open (
	const char* pFileName,
	uint_t Flags
	)
{
	Close ();

	bool Result = m_File.Open (pFileName, Flags);
	if (!Result)
		return false;

	m_pHdr = (TMergeFileHdr*) m_File.View (0, sizeof (TMergeFileHdr), true);
	m_pHdr->m_Signature = EMergeFile_FileSignature;
	m_pHdr->m_Version = EMergeFile_CurrentVersion;
	m_pHdr->m_MergePointCount = 0;

	m_File.SetSize (sizeof (TMergeFileHdr));
	return true;
}

void
CMergeFile::Close ()
{
	m_File.Close ();
	m_pHdr = NULL;
}

void
CMergeFile::Clear ()
{
	ASSERT (m_pHdr);

	m_pHdr->m_MergePointCount = 0;

	m_File.SetSize (sizeof (TMergeFileHdr));
}

const TMergePoint*
CMergeFile::GetMergePoint (uint64_t Index) const
{
	ASSERT (m_pHdr);

	if (Index >= m_pHdr->m_MergePointCount)
		return NULL;

	uint64_t Offset = sizeof (TMergeFileHdr) + Index * sizeof (TMergePoint);

	TMergePoint* pMergePoint = (TMergePoint*) m_File.View (Offset, sizeof (TMergePoint));
	ASSERT (pMergePoint && pMergePoint->m_Signature == EMergeFile_MergePointSignature);
	
	return pMergePoint;
}

const TMergePoint* 
CMergeFile::AddMergePoint (
	ELine LineKind,
	uint32_t Col,
	uint64_t PartIdx,
	uint64_t BinOffset,
	uint64_t ColorizerStateCalcPacketOffset,
	uint32_t ColorizerStateCalcPacketCount
	)
{
	ASSERT (m_pHdr);

	uint64_t Offset = sizeof (TMergeFileHdr) + m_pHdr->m_MergePointCount * sizeof (TMergePoint);

	TMergePoint* pMergPoint = (TMergePoint*) m_File.View (Offset, sizeof (TMergePoint)); 
	if (!pMergPoint)
		return NULL;

	pMergPoint->m_Signature = EMergeFile_MergePointSignature;
	pMergPoint->m_LineKind = LineKind;
	pMergPoint->m_Col = Col;
	pMergPoint->m_PartIdx = PartIdx;
	pMergPoint->m_BinOffset = BinOffset;
	pMergPoint->m_ColorizerStateCalcPacketOffset = ColorizerStateCalcPacketOffset;
	pMergPoint->m_ColorizerStateCalcPacketCount = ColorizerStateCalcPacketCount;

	m_pHdr->m_MergePointCount++;

	m_File.SetSize (Offset + sizeof (TMergePoint));
	return pMergPoint;
}

//.............................................................................

} // namespace log 
} // namespace axl
