// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_PACKETIZER_H

#include "axl_sl_Array.h"

namespace axl {
namespace sl {

//.............................................................................

class PacketizerRoot
{
protected:
	enum
	{
		PacketHdrSignature = 'kap\x02',
	};

protected:
	sl::Array <char> m_buffer;

public:
	void
	reset ()
	{
		m_buffer.clear ();
	}

	static
	uint64_t
	createHdr (size_t size)
	{
		return ((uint64_t) size << 32) | PacketHdrSignature;
	}

protected:
	size_t
	writeImpl (
		const void* p,
		size_t size
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class Packetizer: public PacketizerRoot
{
public:
	void
	write (
		const void* p,
		size_t size
		)
	{
		while (size)
		{
			size_t taken = writeImpl (p, size);
			if (taken == -1)
				break;

			ASSERT (m_buffer.getCount () >= sizeof (uint64_t));
			size_t dataSize = m_buffer.getCount () - sizeof (uint64_t);
			if (dataSize)
				static_cast <T*> (this)->onPacket (m_buffer.a () + sizeof (uint64_t), dataSize);

			reset ();

			p = (char*) p + taken;
			size -= taken;
		}
	}
};

//.............................................................................

// old AXL used different format of header

class LegacyPacketizerRoot
{
protected:
	enum
	{
		PacketHdrSignature = '.GSM',
	};

	struct PacketHdr
	{
		uint32_t m_signature;
		uint16_t m_dataSize;
		uint16_t m_checksum; // CRC16
	};

protected:
	sl::Array <char> m_buffer;

public:
	void
	reset ()
	{
		m_buffer.clear ();
	}

	static
	uint64_t
	createHdr (size_t size);

protected:
	size_t
	writeImpl (
		const void* p,
		size_t size
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class LegacyPacketizer: public LegacyPacketizerRoot
{
public:
	void
	write (
		const void* p,
		size_t size
		)
	{
		while (size)
		{
			size_t taken = writeImpl (p, size);
			if (taken == -1)
				break;

			ASSERT (m_buffer.getCount () >= sizeof (uint64_t));
			size_t dataSize = m_buffer.getCount () - sizeof (uint64_t);
			if (dataSize)
				static_cast <T*> (this)->onPacket (m_buffer.a () + sizeof (uint64_t), dataSize);

			reset ();

			p = (char*) p + taken;
			size -= taken;
		}
	}
};

//.............................................................................

} // namespace sl
} // namespace axl
