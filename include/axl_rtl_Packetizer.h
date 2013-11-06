// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_PACKETIZER_H

#include "axl_rtl_Array.h"

namespace axl {
namespace rtl {

//.............................................................................

class CPacketizerRoot
{
protected:
	enum 
	{
		EPacketHdr_Signature = ':kap',
	};

protected:
	rtl::CArrayT <char> m_Buffer;

public:
	void
	Reset ()
	{
		m_Buffer.Clear ();
	}

	static
	uint64_t
	CreateHdr (size_t Size)
	{
		return ((uint64_t) Size << 32) | EPacketHdr_Signature;
	}

protected:
	size_t
	WriteImpl (
		const void* p,
		size_t Size
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CPacketizerT: public CPacketizerRoot
{
public:
	void
	Write (
		const void* p,
		size_t Size
		)
	{
		while (Size)
		{
			size_t Taken = WriteImpl (p, Size);
			if (Taken == -1)
				break;
			
			ASSERT (m_Buffer.GetCount () >= sizeof (uint64_t));
			size_t DataSize = m_Buffer.GetCount () - sizeof (uint64_t);
			if (DataSize)
				static_cast <T*> (this)->OnPacket (m_Buffer.a () + sizeof (uint64_t), DataSize);

			Reset ();

			p = (char*) p + Taken;
			Size -= Taken;
		}
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
