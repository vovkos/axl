#include "pch.h"
#include "axl_log_PacketFile.h"
#include "axl_err_Error.h"
#include "axl_g_Time.h"

namespace axl {
namespace log {

//.............................................................................

bool
CPacketFile::Open (
	const char* pFileName,
	uint_t Flags
	)
{
	Close ();

	bool Result = m_File.Open (pFileName, Flags);
	if (!Result)
		return false;

	m_pHdr = (TPacketFileHdr*) m_File.View (0, sizeof (TPacketFileHdr), true);
	
	uint64_t FileSize = sizeof (TPacketFileHdr) + m_pHdr->m_TotalPacketSize;
	uint64_t ActualSize = m_File.GetSize ();

	if (m_pHdr->m_Signature == EPacketFile_FileSignature && 
		m_pHdr->m_Version == EPacketFile_CurrentVersion &&
		FileSize <= ActualSize)
	{
		if (FileSize != ActualSize)
			m_File.SetSize (FileSize);

		return true; // existing file is OK
	}

	if (Flags & io::EFileFlag_ReadOnly)
	{
		err::SetFormatStringError ("'%s' is not a valid log packet file", pFileName);
		return false;
	}

	m_pHdr->m_Signature = EPacketFile_FileSignature;
	m_pHdr->m_Version = EPacketFile_CurrentVersion;
	m_pHdr->m_PacketCount = 0;
	m_pHdr->m_TotalPacketSize = 0;
	m_pHdr->m_ClearCount = 0;
	m_pHdr->m_ClassGuid = rtl::GUID_Null;

	m_File.SetSize (sizeof (TPacketFileHdr));
	return true;
}

void
CPacketFile::Close ()
{
	m_File.Close ();
	m_pHdr = NULL;
}

void
CPacketFile::Clear ()
{
	ASSERT (m_pHdr);

	m_pHdr->m_PacketCount = 0;
	m_pHdr->m_TotalPacketSize = 0;
	m_pHdr->m_ClearCount++;

	m_File.SetSize (sizeof (TPacketFileHdr));
}

const TPacket*
CPacketFile::GetPacket (uint64_t Offset) const
{
	ASSERT (m_pHdr);

	if (Offset >= m_pHdr->m_TotalPacketSize)
		return NULL;

	Offset += sizeof (TPacketFileHdr);

	TPacket* pPacket = (TPacket*) m_File.View (Offset, sizeof (TPacket));
	ASSERT (pPacket && pPacket->m_Signature == EPacketFile_PacketSignature); 

	if (pPacket->m_DataSize) // make sure mapping covers data as well
		pPacket = (TPacket*) m_File.View (Offset, sizeof (TPacket) + (size_t) pPacket->m_DataSize);
	
	return pPacket;
}

const TPacket*
CPacketFile::AddPacket (
	uint64_t Timestamp,
	uint_t Code,
	const void* p,
	size_t Size
	)
{
	ASSERT (m_pHdr);

	uint64_t FileOffset = sizeof (TPacketFileHdr) + m_pHdr->m_TotalPacketSize;
	size_t FullSize = sizeof (TPacket) + Size;

	TPacket* pPacket = (TPacket*) m_File.View (FileOffset, FullSize); 
	if (!pPacket)
		return NULL;

	pPacket->m_Signature = EPacketFile_PacketSignature;
	pPacket->m_Code = Code;
	pPacket->m_DataSize = (uint32_t) Size;
	pPacket->m_Timestamp = Timestamp;
	
	if (Size)
		memcpy (pPacket + 1, p, Size);

	m_pHdr->m_PacketCount++;
	m_pHdr->m_TotalPacketSize += FullSize;

	m_File.SetSize (FileOffset + FullSize);
	return pPacket;
}

//.............................................................................

} // namespace log 
} // namespace axl
