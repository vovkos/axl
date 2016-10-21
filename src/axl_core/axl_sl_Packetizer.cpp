//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_sl_Packetizer.h"
#include "axl_sl_Hash.h"

namespace axl {
namespace sl {

//..............................................................................

size_t
PacketizerRoot::writeImpl (
	const void* p0,
	size_t size0
	)
{
	const char* p = (char*) p0;
	size_t size = size0;
	size_t bufferSize = m_buffer.getCount ();

	static uint32_t signatureBuffer = PacketHdrSignature;
	const char* signature = (char*) &signatureBuffer;

	while (bufferSize < sizeof (uint32_t)) // append signature byte-by-byte
	{
		if (!size)
			return -1; // all is buffered

		char c = *p;

		if (c == signature [bufferSize])
		{
			m_buffer.append (c);
			bufferSize++;
		}
		else if (bufferSize)
		{
			m_buffer.clear ();
			bufferSize = 0;

			if (c == signature [0])
			{
				m_buffer.copy (c);
				bufferSize = 1;
			}
		}

		p++;
		size--;
	}

	if (bufferSize < sizeof (uint64_t)) // packet size
	{
		size_t chunkSize = sizeof (uint64_t) - bufferSize;
		if (size < chunkSize)
		{
			m_buffer.append (p, size);
			return -1; // all is buffered
		}

		m_buffer.append (p, chunkSize);
		p += chunkSize;
		size -= chunkSize;

		bufferSize = sizeof (uint64_t);
	}

	uint32_t dataSize = ((const uint32_t*) m_buffer.cp ()) [1];
	uint32_t packetSize = sizeof (uint64_t) + dataSize;

	if (bufferSize < packetSize)
	{
		size_t chunkSize = packetSize - bufferSize;
		if (size < chunkSize)
		{
			m_buffer.append (p, size);
			return -1; // all is buffered
		}

		m_buffer.append (p, chunkSize);
		p += chunkSize;
		size -= chunkSize;
	}

	ASSERT (size0 >= size);
	return size0 - size;
}

//..............................................................................

uint64_t
LegacyPacketizerRoot::createHdr (size_t size)
{
	PacketHdr hdr;
	hdr.m_signature = PacketHdrSignature;
	hdr.m_dataSize = (uint16_t) size;
	hdr.m_checksum = 0;
	hdr.m_checksum = checksum16 (&hdr, sizeof (hdr));

	return *(uint64_t*) &hdr;
}

size_t
LegacyPacketizerRoot::writeImpl (
	const void* p0,
	size_t size0
	)
{
	const char* p = (char*) p0;
	size_t size = size0;
	size_t bufferSize = m_buffer.getCount ();

	static uint32_t signatureBuffer = PacketHdrSignature;
	const char* signature = (char*) &signatureBuffer;

	while (bufferSize < sizeof (uint32_t)) // append signature byte-by-byte
	{
		if (!size)
			return -1; // all is buffered

		char c = *p;

		if (c == signature [bufferSize])
		{
			m_buffer.append (c);
			bufferSize++;
		}
		else if (bufferSize)
		{
			m_buffer.clear ();
			bufferSize = 0;

			if (c == signature [0])
			{
				m_buffer.copy (c);
				bufferSize = 1;
			}
		}

		p++;
		size--;
	}

	if (bufferSize < sizeof (uint64_t)) // packet size and crc
	{
		size_t chunkSize = sizeof (uint64_t) - bufferSize;
		if (size < chunkSize)
		{
			m_buffer.append (p, size);
			return -1; // all is buffered
		}

		m_buffer.append (p, chunkSize);
		p += chunkSize;
		size -= chunkSize;

		uint16_t checksum = checksum16 (m_buffer, sizeof (uint64_t));
		if (checksum)
			return size0 - size; // ignore broken packet hdr

		bufferSize = sizeof (uint64_t);
	}

	uint32_t dataSize = ((const PacketHdr*) m_buffer.cp ())->m_dataSize;
	uint32_t packetSize = sizeof (uint64_t) + dataSize;

	if (bufferSize < packetSize)
	{
		size_t chunkSize = packetSize - bufferSize;
		if (size < chunkSize)
		{
			m_buffer.append (p, size);
			return -1; // all is buffered
		}

		m_buffer.append (p, chunkSize);
		p += chunkSize;
		size -= chunkSize;
	}

	ASSERT (size0 >= size);
	return size0 - size;
}

//..............................................................................

} // namespace sl
} // namespace axl
