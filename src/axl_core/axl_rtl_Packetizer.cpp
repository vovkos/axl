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
	
	static char Signature [4] = { 'p', 'a', 'k', ':' };

	while (BufferSize < sizeof (uint32_t)) // append signature byte-by-byte
	{
		if (!Size)
			return -1; // all is buffered

		if (*p == Signature [BufferSize])
		{
			m_Buffer.Append (*p);
			BufferSize++;
		}
		else
		{
			m_Buffer.Clear ();
			BufferSize = 0;
		}

		p++;
		Size--;
	}

	if (BufferSize < sizeof (uint64_t)) // packet size
	{
		size_t ChunkSize = sizeof (uint64_t) - BufferSize;
		if (ChunkSize > Size) 
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
		if (ChunkSize > Size)
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

 