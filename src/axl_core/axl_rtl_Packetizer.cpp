#include "pch.h"
#include "axl_rtl_Packetizer.h"

namespace axl {
namespace rtl {

//.............................................................................

size_t
CPacketizerRoot::WriteImpl (
	const void* p0,
	size_t Size0
	)
{
	const char* p = (char*) p0;
	size_t Size = Size0;
	size_t BufferSize = m_Buffer.GetCount ();

	static uint32_t Signature = EPacketHdr_Signature;
	const char* pSignature = (char*) &Signature;

	while (BufferSize < sizeof (uint32_t)) // append signature byte-by-byte
	{
		if (!Size)
			return -1; // all is buffered

		char c = *p;

		if (c == pSignature [BufferSize])
		{
			m_Buffer.Append (c);
			BufferSize++;
		}
		else if (BufferSize)
		{
			m_Buffer.Clear ();
			BufferSize = 0;

			if (c == pSignature [0])
			{
				m_Buffer.Copy (c);
				BufferSize = 1;
			}
		}

		p++;
		Size--;
	}

	if (BufferSize < sizeof (uint64_t)) // packet size
	{
		size_t ChunkSize = sizeof (uint64_t) - BufferSize;
		if (Size < ChunkSize)
		{
			m_Buffer.Append (p, Size);
			return -1; // all is buffered
		}

		m_Buffer.Append (p, ChunkSize);
		p += ChunkSize;
		Size -= ChunkSize;

		BufferSize = sizeof (uint64_t);
	}

	uint32_t DataSize = ((const uint32_t*) m_Buffer.ca ()) [1];
	uint32_t PacketSize = sizeof (uint64_t) + DataSize;

	if (BufferSize < PacketSize)
	{
		size_t ChunkSize = PacketSize - BufferSize;
		if (Size < ChunkSize)
		{
			m_Buffer.Append (p, Size);
			return -1; // all is buffered
		}

		m_Buffer.Append (p, ChunkSize);
		p += ChunkSize;
		Size -= ChunkSize;
	}

	ASSERT (Size0 >= Size);
	return Size0 - Size;
}

//.............................................................................

uint64_t
CLegacyPacketizerRoot::CreateHdr (size_t Size)
{
	TPacketHdr Hdr;
	Hdr.m_Signature = EPacketHdr_Signature;
	Hdr.m_DataSize = (uint16_t) Size;
	Hdr.m_Checksum = 0;	
	Hdr.m_Checksum = crc16 (&Hdr, sizeof (Hdr));
	
	return *(uint64_t*) &Hdr;
}

size_t
CLegacyPacketizerRoot::WriteImpl (
	const void* p0,
	size_t Size0
	)
{
	const char* p = (char*) p0;
	size_t Size = Size0;
	size_t BufferSize = m_Buffer.GetCount ();

	static uint32_t Signature = EPacketHdr_Signature;
	const char* pSignature = (char*) &Signature;

	while (BufferSize < sizeof (uint32_t)) // append signature byte-by-byte
	{
		if (!Size)
			return -1; // all is buffered

		char c = *p;

		if (c == pSignature [BufferSize])
		{
			m_Buffer.Append (c);
			BufferSize++;
		}
		else if (BufferSize)
		{
			m_Buffer.Clear ();
			BufferSize = 0;

			if (c == pSignature [0])
			{
				m_Buffer.Copy (c);
				BufferSize = 1;
			}
		}

		p++;
		Size--;
	}

	if (BufferSize < sizeof (uint64_t)) // packet size and crc
	{
		size_t ChunkSize = sizeof (uint64_t) - BufferSize;
		if (Size < ChunkSize)
		{
			m_Buffer.Append (p, Size);
			return -1; // all is buffered
		}

		m_Buffer.Append (p, ChunkSize);
		p += ChunkSize;
		Size -= ChunkSize;

		uint16_t Checksum = crc16 (m_Buffer, sizeof (uint64_t));
		if (Checksum)
			return Size0 - Size; // ignore broken packet hdr

		BufferSize = sizeof (uint64_t);
	}

	uint32_t DataSize = ((const TPacketHdr*) m_Buffer.ca ())->m_DataSize;
	uint32_t PacketSize = sizeof (uint64_t) + DataSize;

	if (BufferSize < PacketSize)
	{
		size_t ChunkSize = PacketSize - BufferSize;
		if (Size < ChunkSize)
		{
			m_Buffer.Append (p, Size);
			return -1; // all is buffered
		}

		m_Buffer.Append (p, ChunkSize);
		p += ChunkSize;
		Size -= ChunkSize;
	}

	ASSERT (Size0 >= Size);
	return Size0 - Size;
}

//.............................................................................

} // namespace rtl
} // namespace axl
