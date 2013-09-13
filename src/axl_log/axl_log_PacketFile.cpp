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
	bool Result;

	uint64_t FileSize;
	uint64_t ActualSize;

	Close ();

	Result = m_File.Open (pFileName, Flags);
	if (!Result)
		return false;

	m_pHdr = (TPacketFileHdr*) m_File.View (0, sizeof (TPacketFileHdr), true);
	
	FileSize = sizeof (TPacketFileHdr) + m_pHdr->m_TotalPacketSize;
	ActualSize = m_File.GetSize ();

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
	m_pHdr->m_SyncId = 0;
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
	m_pHdr->m_SyncId++;

	m_File.SetSize (sizeof (TPacketFileHdr));
}

TPacket*
CPacketFile::GetPacket (size_t Offset) const
{
	TPacket* pPacket;

	ASSERT (m_pHdr);

	if (Offset >= m_pHdr->m_TotalPacketSize)
		return NULL;

	Offset += sizeof (TPacketFileHdr);

	pPacket = (TPacket*) m_File.View (Offset, sizeof (TPacket));

	ASSERT (pPacket); 
	ASSERT (pPacket->m_Signature == EPacketFile_PacketSignature); 

	if (pPacket->m_DataSize) // make sure mapping covers data as well
		pPacket = (TPacket*) m_File.View (Offset, sizeof (TPacket) + pPacket->m_DataSize);
	
	return pPacket;
}

bool
CPacketFile::Write (
	uint64_t Timestamp,
	uint_t Code,
	const void* p,
	size_t Size
	)
{
	TPacket* pPacket;

	size_t Offset;
	size_t FullSize;

	ASSERT (m_pHdr);

	Offset = sizeof (TPacketFileHdr) + m_pHdr->m_TotalPacketSize;
	FullSize = sizeof (TPacket) + Size;
	pPacket = (TPacket*) m_File.View (Offset, FullSize); 
	if (!pPacket)
		return false;

	pPacket->m_Signature = EPacketFile_PacketSignature;
	pPacket->m_Code = Code;
	pPacket->m_DataSize = (uint32_t) Size;
	pPacket->m_Timestamp = Timestamp;
	
	if (Size)
		memcpy (pPacket + 1, p, Size);

	m_pHdr->m_PacketCount++;
	m_pHdr->m_TotalPacketSize += (uint32_t) FullSize;

	m_File.SetSize (Offset + FullSize);
	return true;
}

//.............................................................................

} // namespace log 
} // namespace axl
