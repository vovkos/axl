// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_UTF_H

#include "axl_rtl_ByteOrder.h"

namespace axl {
namespace rtl {

//.............................................................................

enum UtfKind
{
	UtfKind_Utf8 = 0,
	UtfKind_Utf16,
	UtfKind_Utf16_be,
	UtfKind_Utf32,
	UtfKind_Utf32_be,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getUtfKindString (UtfKind utfKind);

//.............................................................................

struct UtfCodePointAttr
{
    uint16_t m_category           : 8; /* 5 used */
    uint16_t m_direction          : 8; /* 5 used */
    uint16_t m_combiningClass     : 8;
    uint16_t m_joining            : 2;
    int16_t m_digitValue          : 6; /* 5 used */
    int16_t m_mirrorDiff          : 16;
    int16_t m_lowerCaseDiff       : 16;
    int16_t m_upperCaseDiff       : 16;
    int16_t m_titleCaseDiff       : 16;
    int16_t m_caseFoldDiff        : 16;
    uint16_t m_lowerCaseSpecial   : 1;
    uint16_t m_upperCaseSpecial   : 1;
    uint16_t m_titleCaseSpecial   : 1;
    uint16_t m_caseFoldSpecial    : 1;
    uint16_t m_unicodeVersion     : 4;
    uint16_t m_graphemeBreakClass : 8; /* 4 used */
    uint16_t m_wordBreakClass     : 8; /* 4 used */
    uint16_t m_sentenceBreakClass : 8; /* 4 used */
    uint16_t m_lineBreakClass     : 8; /* 6 used */
    uint16_t m_script             : 8; /* 5 used */
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const UtfCodePointAttr*
getUtfCodePointAttr (utf32_t c);

inline
utf32_t
utfToLower (utf32_t c)
{
	const UtfCodePointAttr* attr = getUtfCodePointAttr (c);
	return attr->m_lowerCaseDiff && !attr->m_lowerCaseSpecial ? c + attr->m_lowerCaseDiff : c;
}

inline
utf32_t
utfToUpper (utf32_t c)
{
	const UtfCodePointAttr* attr = getUtfCodePointAttr (c);
	return attr->m_upperCaseDiff && !attr->m_upperCaseSpecial ? c + attr->m_upperCaseDiff : c;
}

inline
utf32_t
utfToCaseFold (utf32_t c)
{
	const UtfCodePointAttr* attr = getUtfCodePointAttr (c);
	return attr->m_caseFoldDiff && !attr->m_caseFoldSpecial ? c + attr->m_caseFoldDiff : c;
}

//.............................................................................

template <typename T>
size_t
calcUtfCodePointCount (
	const typename T::C* p,
	size_t length
	)
{
	const typename T::C* end = p + length;

	size_t count = 0;
	while (p < end)
	{
		size_t srcCodePointLength = T::getDecodeCodePointLength (*p);
		if (p + srcCodePointLength > end)
			break;

		p += srcCodePointLength;
		count++;
	}

	return count;
}

//.............................................................................

class Utf8
{
public:
	typedef utf8_t C;

	static
	UtfKind
	getUtfKind ()
	{
		return UtfKind_Utf8;
	}

	static
	const uint8_t*
	getBom ()
	{
		static uint8_t bom [] = { 0xef, 0xbb, 0xbf };
		return bom;
	}

	static
	size_t
	getBomLength ()
	{
		return 3;
	}

	static
	size_t
	getDecodeCodePointLength (utf8_t c)
	{
		return
			(c & 0x80) == 0    ? 1 :     // 0xxxxxxx
			(c & 0xe0) == 0xc0 ? 2 :     // 110xxxxx 10xxxxxx
			(c & 0xf0) == 0xe0 ? 3 :     // 1110xxxx 10xxxxxx 10xxxxxx
			(c & 0xf8) == 0xf0 ? 4 : 1;  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	}

	static
	size_t
	getEncodeCodePointLength (utf32_t x)
	{
		return
			x < 0x80 ? 1 :               // 0xxxxxxx
			x < 0x800 ? 2 :              // 110xxxxx 10xxxxxx
			x < 0x10000 ? 3 :            // 1110xxxx 10xxxxxx 10xxxxxx
			x < 0x200000 ? 4 : 1;        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	}

	static
	utf32_t
	decodeCodePoint (const utf8_t* p)
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
	encodeCodePoint (
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
	calcCodePointCount (
		const C* p,
		size_t length
		)
	{
		return calcUtfCodePointCount <Utf8> (p, length);
	}
};

//.............................................................................

class Utf16
{
public:
	typedef utf16_t C;

	enum SurrogateKind
	{
		SurrogateKind_MinLead  = 0xd800,
		SurrogateKind_MaxLead  = 0xdbff,
		SurrogateKind_MinTrail = 0xdc00,
		SurrogateKind_MaxTrail = 0xdfff,
	};

	static
	UtfKind
	getUtfKind ()
	{
		return UtfKind_Utf16;
	}

	static
	const uint8_t*
	getBom ()
	{
		static uint8_t bom [] = { 0xff, 0xfe };
		return bom;
	}

	static
	size_t
	getBomLength ()
	{
		return 2;
	}

	static
	bool
	isLeadSurrogate (uint16_t c)
	{
		return c >= 0xd800 && c <= 0xdbff;
	}

	static
	bool
	isTrailSurrogate (uint16_t c)
	{
		return c >= 0xdc00 && c <= 0xdfff;
	}

	static
	bool
	needSurrogate (uint32_t x)
	{
		return x >= 0x10000;
	}

	static
	utf16_t
	getLeadSurrogate (uint32_t x)
	{
		return 0xd800 + (((x - 0x10000) >> 10) & 0x3ff);
	}

	static
	utf16_t
	getTrailSurrogate (uint32_t x)
	{
		return 0xdc00 + (x & 0x3ff);
	}

	static
	utf32_t
	getSurrogateCodePoint (
		uint16_t lead,
		uint16_t trail
		)
	{
		ASSERT (isLeadSurrogate (lead) && isTrailSurrogate (trail));
		return 0x10000 - (0xd800 << 10) - 0xdc00 + (lead << 10) + trail;
	}

	static
	size_t
	getDecodeCodePointLength (utf16_t c)
	{
		return isLeadSurrogate (c) ? 2 : 1;
	}

	static
	size_t
	getEncodeCodePointLength (utf32_t x)
	{
		return needSurrogate (x) ? 2 : 1;
	}

	static
	utf32_t
	decodeCodePoint (const utf16_t* p)
	{
		return isLeadSurrogate (*p) ? getSurrogateCodePoint (p [0], p [1]) : (uint16_t) *p;
	}

	static
	void
	encodeCodePoint (
		utf16_t* p,
		utf32_t x
		)
	{
		if (!needSurrogate (x))
		{
			*p = (utf16_t) x;
		}
		else
		{
			p [0] = getLeadSurrogate (x);
			p [1] = getTrailSurrogate (x);
		}
	}

	static
	size_t
	calcCodePointCount (
		const C* p,
		size_t length
		)
	{
		return calcUtfCodePointCount <Utf16> (p, length);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16_be: public Utf16
{
public:
	static
	UtfKind
	getUtfKind ()
	{
		return UtfKind_Utf16_be;
	}

	static
	const uint8_t*
	getBom ()
	{
		static uint8_t bom [] = { 0xfe, 0xff };
		return bom;
	}

	static
	size_t
	getDecodeCodePointLength (utf16_t c)
	{
		return isTrailSurrogate (c) ? 2 : 1;
	}

	static
	utf32_t
	decodeCodePoint (const utf16_t* p)
	{
		return isTrailSurrogate (*p) ? getSurrogateCodePoint (p [1], p [0]) : (uint16_t) *p;
	}

	static
	void
	encodeCodePoint (
		utf16_t* p,
		utf32_t x
		)
	{
		if (!needSurrogate (x))
		{
			*p = (utf16_t) x;
		}
		else
		{
			p [0] = getTrailSurrogate (x);
			p [1] = getLeadSurrogate (x);
		}
	}

	static
	size_t
	calcCodePointCount (
		const C* p,
		size_t length
		)
	{
		return calcUtfCodePointCount <Utf16_be> (p, length);
	}
};

//.............................................................................

class Utf32
{
public:
	typedef utf32_t C;

	static
	UtfKind
	getUtfKind ()
	{
		return UtfKind_Utf32;
	}

	static
	const uint8_t*
	getBom ()
	{
		static uint8_t bom [] = { 0xff, 0xfe, 0x00, 0x00 };
		return bom;
	}

	static
	size_t
	getBomLength ()
	{
		return 4;
	}

	static
	size_t
	getDecodeCodePointLength (utf32_t c)
	{
		return 1;
	}

	static
	size_t
	getEncodeCodePointLength (utf32_t x)
	{
		return 1;
	}

	static
	utf32_t
	decodeCodePoint (const utf32_t* p)
	{
		return *p;
	}

	static
	void
	encodeCodePoint (
		utf32_t* p,
		utf32_t x
		)
	{
		*p = x;
	}

	static
	size_t
	calcCodePointCount (
		const C* p,
		size_t length
		)
	{
		return length;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32_be: public Utf32
{
public:
	static
	UtfKind
	getUtfKind ()
	{
		return UtfKind_Utf32_be;
	}

	static
	const uint8_t*
	getBom ()
	{
		static uint8_t bom [] = { 0x00, 0x00, 0xfe, 0xff };
		return bom;
	}

	static
	utf32_t
	decodeCodePoint (const utf32_t* p)
	{
		return rtl::swapByteOrder32 (*p);
	}

	static
	void
	encodeCodePoint (
		utf32_t* p,
		utf32_t x
		)
	{
		*p = rtl::swapByteOrder32 (x);
	}
};

//.............................................................................

template <
	typename DstEncoding_0,
	typename SrcEncoding_0
	>
class UtfConvert
{
public:
	typedef DstEncoding_0 DstEncoding;
	typedef SrcEncoding_0 SrcEncoding;
	typedef typename DstEncoding::C DstUnit;
	typedef typename SrcEncoding::C SrcUnit;

public:
	static
	size_t
	calcRequiredLength (
		const SrcUnit* p,
		size_t length
		)
	{
		const SrcUnit* end = p + length;

		size_t resultLength = 0;
		while (p < end)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength (*p);
			if (p + srcCodePointLength > end)
				break;

			utf32_t x = SrcEncoding::decodeCodePoint (p);
			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength (x);

			resultLength += dstCodePointLength;
			p += srcCodePointLength;
		}

		return resultLength;
	}

	static
	void
	convert (
		DstUnit* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		size_t* takenDstLength_o = NULL,
		size_t* takenSrcLength_o = NULL,
		size_t* expectedSrcLength_o = NULL
		)
	{
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const SrcUnit* src0 = src;
		const SrcUnit* srcEnd = src + srcLength;

		size_t expectedSrcLength = 0;

		while (src < srcEnd)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength (*src);
			if (src + srcCodePointLength > srcEnd)
			{
				expectedSrcLength = srcCodePointLength;
				break;
			}

			utf32_t x = SrcEncoding::decodeCodePoint (src);
			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength (x);
			if (dst + dstCodePointLength > dstEnd)
				break;

			DstEncoding::encodeCodePoint (dst, x);

			dst += dstCodePointLength;
			src += srcCodePointLength;
		}

		if (takenDstLength_o)
			*takenDstLength_o = dst - dst0;

		if (takenSrcLength_o)
			*takenSrcLength_o = src - src0;

		if (expectedSrcLength_o)
			*expectedSrcLength_o = expectedSrcLength;
	}
};

//.............................................................................

template <typename SrcEncoding_0>
class UtfToAsciiConvert
{
public:
	typedef SrcEncoding_0 SrcEncoding;
	typedef typename SrcEncoding::C SrcUnit;

public:
	static
	size_t
	calcRequiredLength (
		const SrcUnit* p,
		size_t length
		)
	{
		const SrcUnit* end = p + length;

		size_t resultLength = 0;
		while (p < end)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength (*p);
			if (p + srcCodePointLength > end)
				break;

			resultLength++;
			p += srcCodePointLength;
		}

		return resultLength;
	}

	static
	void
	convert (
		char* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		size_t* takenDstLength_o = NULL,
		size_t* takenSrcLength_o = NULL,
		size_t* expectedSrcLength_o = NULL
		)
	{
		char* dst0 = dst;
		char* dstEnd = dst + dstLength;
		const SrcUnit* src0 = src;
		const SrcUnit* srcEnd = src + srcLength;

		size_t expectedSrcLength = 0;

		while (src < srcEnd && dst < dstEnd)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength (*src);
			if (src + srcCodePointLength > srcEnd)
			{
				expectedSrcLength = srcCodePointLength;
				break;
			}

			utf32_t x = SrcEncoding::decodeCodePoint (src);
			*dst = (char) x;

			dst++;
			src += srcCodePointLength;
		}

		if (takenDstLength_o)
			*takenDstLength_o = dst - dst0;

		if (takenSrcLength_o)
			*takenSrcLength_o = src - src0;

		if (expectedSrcLength_o)
			*expectedSrcLength_o = expectedSrcLength;

	}
};

//.............................................................................

template <typename DstEncoding_0>
class AsciiToUtfConvert
{
public:
	typedef DstEncoding_0 DstEncoding;
	typedef typename DstEncoding::C DstUnit;

public:
	static
	size_t
	calcRequiredLength (
		const char* p,
		size_t length
		)
	{
		const char* end = p + length;

		size_t resultLength = 0;
		while (p < end)
		{
			utf32_t x = (uchar_t) *p; // don't propagate sign bit

			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength (x);

			resultLength += dstCodePointLength;
			p++;
		}

		return resultLength;
	}

	static
	void
	convert (
		DstUnit* dst,
		size_t dstLength,
		const char* src,
		size_t srcLength,
		size_t* takenDstLength_o = NULL,
		size_t* takenSrcLength_o = NULL,
		size_t* expectedSrcLength_o = NULL
		)
	{
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const char* src0 = src;
		const char* srcEnd = src + srcLength;

		while (src < srcEnd)
		{
			utf32_t x = (uchar_t) *src; // don't propagate sign bit

			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength (x);
			if (dst + dstCodePointLength > dstEnd)
				break;

			DstEncoding::encodeCodePoint (dst, x);

			dst += dstCodePointLength;
			src++;
		}

		if (takenDstLength_o)
			*takenDstLength_o = dst - dst0;

		if (takenSrcLength_o)
			*takenSrcLength_o = src - src0;

		if (expectedSrcLength_o)
			*expectedSrcLength_o = 0;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
