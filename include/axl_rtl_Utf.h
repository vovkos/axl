// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_UTF_H

#include "axl_rtl_ByteOrder.h"

namespace axl {
namespace rtl {

//.............................................................................

enum EUtf
{
	EUtf_Utf8 = 0,
	EUtf_Utf16,
	EUtf_Utf16_be,
	EUtf_Utf32,
	EUtf_Utf32_be,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetUtfKindString (EUtf UtfKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
size_t
CalcUtfCodePointCount (
	const typename T::C* p,
	size_t Length
	)
{
	const typename T::C* pEnd = p + Length;

	size_t Count = 0;
	while (p < pEnd)
	{
		size_t SrcCodePointLength = T::GetDecodeCodePointLength (*p);
		if (p + SrcCodePointLength > pEnd)
			break;

		p += SrcCodePointLength;
		Count++;
	}

	return Count;
}

//.............................................................................

class CUtf8
{
public:
	typedef utf8_t C;

	static
	EUtf
	GetUtfKind ()
	{
		return EUtf_Utf8;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0xef, 0xbb, 0xbf };
		return Bom;
	}

	static
	size_t
	GetBomLength ()
	{
		return 3;
	}

	static
	size_t
	GetDecodeCodePointLength (utf8_t c)
	{
		return
			(c & 0x80) == 0    ? 1 :     // 0xxxxxxx
			(c & 0xe0) == 0xc0 ? 2 :     // 110xxxxx 10xxxxxx
			(c & 0xf0) == 0xe0 ? 3 :     // 1110xxxx 10xxxxxx 10xxxxxx
			(c & 0xf8) == 0xf0 ? 4 : 1;  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	}

	static
	size_t
	GetEncodeCodePointLength (utf32_t x)
	{
		return
			x < 0x80 ? 1 :               // 0xxxxxxx
			x < 0x800 ? 2 :              // 110xxxxx 10xxxxxx
			x < 0x10000 ? 3 :            // 1110xxxx 10xxxxxx 10xxxxxx
			x < 0x200000 ? 4 : 1;        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	}

	static
	utf32_t
	DecodeCodePoint (const utf8_t* p)
	{
		return
			((*p & 0x80) == 0)    ?      // 0xxxxxxx
				*p :
			((*p & 0xe0) == 0xc0) ?      // 110xxxxx 10xxxxxx
				((p [0] & 0x1f) << 6)  |
				 (p [1] & 0x3f) :
			((*p & 0xf0) == 0xe0) ?      // 1110xxxx 10xxxxxx 10xxxxxx
				((p [0] & 0x0f) << 12) |
				((p [1] & 0x3f) << 6)  |
				 (p [2] & 0x3f) :
			((*p & 0xf8) == 0xf0) ?      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
				((p [0] & 0x07) << 18) |
				((p [1] & 0x3f) << 12) |
				((p [2] & 0x3f) << 6)  |
				 (p [3] & 0x3f) : 0xffff; // use non-character U+FFFF (needs no surrogate in UTF-16)
	}

	static
	void
	EncodeCodePoint (
		utf8_t* p,
		utf32_t x
		)
	{
		if (x < 0x80)                    // 0xxxxxxx
		{
			p [0] = (uint8_t) x;
		}
		else if (x < 0x800)              // 110xxxxx 10xxxxxx
		{
			p [0] = (uint8_t) (x >> 6)          | 0xc0;
			p [1] = (uint8_t) (x & 0x3f)        | 0x80;
		}
		else if (x < 0x10000)            // 1110xxxx 10xxxxxx 10xxxxxx
		{
			p [0] = (uint8_t) (x >> 12)         | 0xe0;
			p [1] = (uint8_t) ((x >> 6) & 0x3f) | 0x80;
			p [2] = (uint8_t) (x & 0x3f)        | 0x80;
		}
		else if (x < 0x200000)           // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			p [0] = (uint8_t) (x >> 18)         | 0xf0;
			p [1] = (uint8_t) ((x >> 12) & 0x3f)| 0x80;
			p [2] = (uint8_t) ((x >> 6) & 0x3f) | 0x80;
			p [3] = (uint8_t) (x & 0x3f)        | 0x80;
		}
		else
		{
			p [0] = -1;
		}
	}

	static
	size_t
	CalcCodePointCount (
		const C* p,
		size_t Length
		)
	{
		return CalcUtfCodePointCount <CUtf8> (p, Length);
	}
};

//.............................................................................

class CUtf16
{
public:
	typedef utf16_t C;

	enum ESurrogate
	{
		ESurrogate_MinLead  = 0xd800,
		ESurrogate_MaxLead  = 0xdbff,
		ESurrogate_MinTrail = 0xdc00,
		ESurrogate_MaxTrail = 0xdfff,
	};

	static
	EUtf
	GetUtfKind ()
	{
		return EUtf_Utf16;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0xff, 0xfe };
		return Bom;
	}

	static
	size_t
	GetBomLength ()
	{
		return 2;
	}

	static
	bool
	IsLeadSurrogate (uint16_t c)
	{
		return c >= 0xd800 && c <= 0xdbff;
	}

	static
	bool
	IsTrailSurrogate (uint16_t c)
	{
		return c >= 0xdc00 && c <= 0xdfff;
	}

	static
	bool
	NeedSurrogate (uint32_t x)
	{
		return x >= 0x10000;
	}

	static
	utf16_t
	GetLeadSurrogate (uint32_t x)
	{
		return 0xd800 + (((x - 0x10000) >> 10) & 0x3ff);
	}

	static
	utf16_t
	GetTrailSurrogate (uint32_t x)
	{
		return 0xdc00 + (x & 0x3ff);
	}

	static
	utf32_t
	GetSurrogateCodePoint (
		uint16_t Lead,
		uint16_t Trail
		)
	{
		ASSERT (IsLeadSurrogate (Lead) && IsTrailSurrogate (Trail));
		return 0x10000 - (0xd800 << 10) - 0xdc00 + (Lead << 10) + Trail;
	}

	static
	size_t
	GetDecodeCodePointLength (utf16_t c)
	{
		return IsLeadSurrogate (c) ? 2 : 1;
	}

	static
	size_t
	GetEncodeCodePointLength (utf32_t x)
	{
		return NeedSurrogate (x) ? 2 : 1;
	}

	static
	utf32_t
	DecodeCodePoint (const utf16_t* p)
	{
		return IsLeadSurrogate (*p) ? GetSurrogateCodePoint (p [0], p [1]) : (uint16_t) *p;
	}

	static
	void
	EncodeCodePoint (
		utf16_t* p,
		utf32_t x
		)
	{
		if (!NeedSurrogate (x))
		{
			*p = (utf16_t) x;
		}
		else
		{
			p [0] = GetLeadSurrogate (x);
			p [1] = GetTrailSurrogate (x);
		}
	}

	static
	size_t
	CalcCodePointCount (
		const C* p,
		size_t Length
		)
	{
		return CalcUtfCodePointCount <CUtf16> (p, Length);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUtf16_be: public CUtf16
{
public:
	static
	EUtf
	GetUtfKind ()
	{
		return EUtf_Utf16_be;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0xfe, 0xff };
		return Bom;
	}

	static
	size_t
	GetDecodeCodePointLength (utf16_t c)
	{
		return IsTrailSurrogate (c) ? 2 : 1;
	}

	static
	utf32_t
	DecodeCodePoint (const utf16_t* p)
	{
		return IsTrailSurrogate (*p) ? GetSurrogateCodePoint (p [1], p [0]) : (uint16_t) *p;
	}

	static
	void
	EncodeCodePoint (
		utf16_t* p,
		utf32_t x
		)
	{
		if (!NeedSurrogate (x))
		{
			*p = (utf16_t) x;
		}
		else
		{
			p [0] = GetTrailSurrogate (x);
			p [1] = GetLeadSurrogate (x);
		}
	}

	static
	size_t
	CalcCodePointCount (
		const C* p,
		size_t Length
		)
	{
		return CalcUtfCodePointCount <CUtf16_be> (p, Length);
	}
};

//.............................................................................

class CUtf32
{
public:
	typedef utf32_t C;

	static
	EUtf
	GetUtfKind ()
	{
		return EUtf_Utf32;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0xff, 0xfe, 0x00, 0x00 };
		return Bom;
	}

	static
	size_t
	GetBomLength ()
	{
		return 4;
	}

	static
	size_t
	GetDecodeCodePointLength (utf32_t c)
	{
		return 1;
	}

	static
	size_t
	GetEncodeCodePointLength (utf32_t x)
	{
		return 1;
	}

	static
	utf32_t
	DecodeCodePoint (const utf32_t* p)
	{
		return *p;
	}

	static
	void
	EncodeCodePoint (
		utf32_t* p,
		utf32_t x
		)
	{
		*p = x;
	}

	static
	size_t
	CalcCodePointCount (
		const C* p,
		size_t Length
		)
	{
		return Length;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUtf32_be: public CUtf32
{
public:
	static
	EUtf
	GetUtfKind ()
	{
		return EUtf_Utf32_be;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0x00, 0x00, 0xfe, 0xff };
		return Bom;
	}

	static
	utf32_t
	DecodeCodePoint (const utf32_t* p)
	{
		return rtl::SwapByteOrder32 (*p);
	}

	static
	void
	EncodeCodePoint (
		utf32_t* p,
		utf32_t x
		)
	{
		*p = rtl::SwapByteOrder32 (x);
	}
};

//.............................................................................

template <
	typename TDstEncoding,
	typename TSrcEncoding
	>
class CUtfConvertT
{
public:
	typedef TDstEncoding CDstEncoding;
	typedef TSrcEncoding CSrcEncoding;
	typedef typename CDstEncoding::C CDstUnit;
	typedef typename CSrcEncoding::C CSrcUnit;

public:
	static
	size_t
	CalcRequiredLength (
		const CSrcUnit* p,
		size_t Length
		)
	{
		const CSrcUnit* pEnd = p + Length;

		size_t ResultLength = 0;
		while (p < pEnd)
		{
			size_t SrcCodePointLength = CSrcEncoding::GetDecodeCodePointLength (*p);
			if (p + SrcCodePointLength > pEnd)
				break;

			utf32_t x = CSrcEncoding::DecodeCodePoint (p);
			size_t DstCodePointLength = CDstEncoding::GetEncodeCodePointLength (x);

			ResultLength += DstCodePointLength;
			p += SrcCodePointLength;
		}

		return ResultLength;
	}

	static
	void
	Convert (
		CDstUnit* pDst,
		size_t DstLength,
		const CSrcUnit* pSrc,
		size_t SrcLength,
		size_t* pTakenDstLength = NULL,
		size_t* pTakenSrcLength = NULL,
		size_t* pExpectedSrcLength = NULL
		)
	{
		CDstUnit* pDst0 = pDst;
		CDstUnit* pDstEnd = pDst + DstLength;
		const CSrcUnit* pSrc0 = pSrc;
		const CSrcUnit* pSrcEnd = pSrc + SrcLength;

		size_t ExpectedSrcLength = 0;

		while (pSrc < pSrcEnd)
		{
			size_t SrcCodePointLength = CSrcEncoding::GetDecodeCodePointLength (*pSrc);
			if (pSrc + SrcCodePointLength > pSrcEnd)
			{
				ExpectedSrcLength = SrcCodePointLength;
				break;
			}

			utf32_t x = CSrcEncoding::DecodeCodePoint (pSrc);
			size_t DstCodePointLength = CDstEncoding::GetEncodeCodePointLength (x);
			if (pDst + DstCodePointLength > pDstEnd)
				break;

			CDstEncoding::EncodeCodePoint (pDst, x);

			pDst += DstCodePointLength;
			pSrc += SrcCodePointLength;
		}

		if (pTakenDstLength)
			*pTakenDstLength = pDst - pDst0;

		if (pTakenSrcLength)
			*pTakenSrcLength = pSrc - pSrc0;

		if (pExpectedSrcLength)
			*pExpectedSrcLength = ExpectedSrcLength;
	}
};

//.............................................................................

template <typename TSrcEncoding>
class CUtfToAsciiConvertT
{
public:
	typedef TSrcEncoding CSrcEncoding;
	typedef typename CSrcEncoding::C CSrcUnit;

public:
	static
	size_t
	CalcRequiredLength (
		const CSrcUnit* p,
		size_t Length
		)
	{
		const CSrcUnit* pEnd = p + Length;

		size_t ResultLength = 0;
		while (p < pEnd)
		{
			size_t SrcCodePointLength = CSrcEncoding::GetDecodeCodePointLength (*p);
			if (p + SrcCodePointLength > pEnd)
				break;

			ResultLength++;
			p += SrcCodePointLength;
		}

		return ResultLength;
	}

	static
	void
	Convert (
		char* pDst,
		size_t DstLength,
		const CSrcUnit* pSrc,
		size_t SrcLength,
		size_t* pTakenDstLength = NULL,
		size_t* pTakenSrcLength = NULL,
		size_t* pExpectedSrcLength = NULL
		)
	{
		char* pDst0 = pDst;
		char* pDstEnd = pDst + DstLength;
		const CSrcUnit* pSrc0 = pSrc;
		const CSrcUnit* pSrcEnd = pSrc + SrcLength;

		size_t ExpectedSrcLength = 0;

		while (pSrc < pSrcEnd && pDst < pDstEnd)
		{
			size_t SrcCodePointLength = CSrcEncoding::GetDecodeCodePointLength (*pSrc);
			if (pSrc + SrcCodePointLength > pSrcEnd)
			{
				ExpectedSrcLength = SrcCodePointLength;
				break;
			}

			utf32_t x = CSrcEncoding::DecodeCodePoint (pSrc);
			*pDst = (char) x;

			pDst++;
			pSrc += SrcCodePointLength;
		}

		if (pTakenDstLength)
			*pTakenDstLength = pDst - pDst0;

		if (pTakenSrcLength)
			*pTakenSrcLength = pSrc - pSrc0;

		if (pExpectedSrcLength)
			*pExpectedSrcLength = ExpectedSrcLength;

	}
};

//.............................................................................

template <typename TDstEncoding>
class CAsciiToUtfConvertT
{
public:
	typedef TDstEncoding CDstEncoding;
	typedef typename CDstEncoding::C CDstUnit;

public:
	static
	size_t
	CalcRequiredLength (
		const char* p,
		size_t Length
		)
	{
		const char* pEnd = p + Length;

		size_t ResultLength = 0;
		while (p < pEnd)
		{
			utf32_t x = (uchar_t) *p; // don't propagate sign bit

			size_t DstCodePointLength = CDstEncoding::GetEncodeCodePointLength (x);

			ResultLength += DstCodePointLength;
			p++;
		}

		return ResultLength;
	}

	static
	void
	Convert (
		CDstUnit* pDst,
		size_t DstLength,
		const char* pSrc,
		size_t SrcLength,
		size_t* pTakenDstLength = NULL,
		size_t* pTakenSrcLength = NULL,
		size_t* pExpectedSrcLength = NULL
		)
	{
		CDstUnit* pDst0 = pDst;
		CDstUnit* pDstEnd = pDst + DstLength;
		const char* pSrc0 = pSrc;
		const char* pSrcEnd = pSrc + SrcLength;

		while (pSrc < pSrcEnd)
		{
			utf32_t x = (uchar_t) *pSrc; // don't propagate sign bit

			size_t DstCodePointLength = CDstEncoding::GetEncodeCodePointLength (x);
			if (pDst + DstCodePointLength > pDstEnd)
				break;

			CDstEncoding::EncodeCodePoint (pDst, x);

			pDst += DstCodePointLength;
			pSrc++;
		}

		if (pTakenDstLength)
			*pTakenDstLength = pDst - pDst0;

		if (pTakenSrcLength)
			*pTakenSrcLength = pSrc - pSrc0;

		if (pExpectedSrcLength)
			*pExpectedSrcLength = 0;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
