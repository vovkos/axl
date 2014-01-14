// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_GUID_H

#include "axl_g_Def.h"

namespace axl {
namespace rtl {

template <typename T> class CStringT;
typedef CStringT <char> CString;

//.............................................................................

enum EGuidStringFlag
{
	EGuidStringFlag_UpperCase   = 0x01,
	EGuidStringFlag_CurlyBraces = 0x02,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TGuid
{
	union
	{
		struct
		{
			uint32_t m_Data1;
			uint16_t m_Data2;
			uint16_t m_Data3;
			uint8_t m_Data4 [8];
		};

		struct
		{
			uint32_t m_Long1;
			uint32_t m_Long2;
			uint32_t m_Long3;
			uint32_t m_Long4;
		};

#if (_AXL_ENV == AXL_ENV_WIN)
		GUID m_Guid;
#endif
	};

	bool
	operator == (const TGuid& Guid) const
	{
		return IsEqual (Guid);
	}

	bool
	operator != (const TGuid& Guid) const
	{
		return !IsEqual (Guid);
	}

	int
	Cmp (const TGuid& Guid) const
	{
		return memcmp (this, &Guid, sizeof (TGuid));
	}

	bool
	IsEqual (const TGuid& Guid) const
	{
		return
			m_Long1 == Guid.m_Long1 &&
			m_Long2 == Guid.m_Long2 &&
			m_Long3 == Guid.m_Long3 &&
			m_Long4 == Guid.m_Long4;
	}

	rtl::CString
	GetGuidString (uint_t Flags = 0) const;

	void
	Setup (
		uint32_t l,
		uint16_t s1,
		uint16_t s2,
		uint8_t b1,
		uint8_t b2,
		uint8_t b3,
		uint8_t b4,
		uint8_t b5,
		uint8_t b6,
		uint8_t b7,
		uint8_t b8
		)
	{
		m_Data1 = l;
		m_Data2 = s1;
		m_Data3 = s2;
		m_Data4 [0] = b1;
		m_Data4 [1] = b2;
		m_Data4 [2] = b3;
		m_Data4 [3] = b4;
		m_Data4 [4] = b5;
		m_Data4 [5] = b6;
		m_Data4 [6] = b7;
		m_Data4 [7] = b8;
	}

	bool
	Parse (const char* pString);

	bool
	Generate ();
};

//.............................................................................

inline
TGuid
BuildGuid (
	uint32_t l,
	uint16_t s1,
	uint16_t s2,
	uint8_t b1,
	uint8_t b2,
	uint8_t b3,
	uint8_t b4,
	uint8_t b5,
	uint8_t b6,
	uint8_t b7,
	uint8_t b8
	)
{
	TGuid Guid;
	Guid.Setup (l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8);
	return Guid;
}

inline
TGuid
ParseGuid (const char* pString)
{
	TGuid Guid;
	Guid.Parse (pString);
	return Guid;
}

inline
TGuid
GenerateGuid ()
{
	TGuid Guid;
	Guid.Generate ();
	return Guid;
}

//.............................................................................

// global GUIDs

#define AXL_RTL_GUID_INITIALIZER(l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	{ { { l, s1, s2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } } } }

#define AXL_RTL_DEFINE_GUID(n, l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	extern AXL_SELECT_ANY const axl::rtl::TGuid n = \
		AXL_RTL_GUID_INITIALIZER (l, s1, s2, b1, b2,  b3,  b4,  b5,  b6,  b7,  b8)

AXL_RTL_DEFINE_GUID (GUID_Null, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

//.............................................................................

} // namespace rtl
} // namespace axl
