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

#pragma once

#define _AXL_ENC_UTF_H

#include "axl_sl_ByteOrder.h"
#include "axl_sl_Operator.h"

namespace axl {
namespace enc {

//..............................................................................

enum UtfKind
{
	UtfKind_Utf8 = 0,
	UtfKind_Utf16,
	UtfKind_Utf16_be,
	UtfKind_Utf32,
	UtfKind_Utf32_be,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getUtfKindString(UtfKind utfKind);

//..............................................................................

bool
utfIsPrintable(utf32_t c);

bool
utfIsPrintableNonMark(utf32_t c);

bool
utfIsSpace(utf32_t c);

bool
utfIsPunctuation(utf32_t c);

bool
utfIsLetter(utf32_t c);

bool
utfIsDigit(utf32_t c);

bool
utfIsNumber(utf32_t c);

bool
utfIsLetterOrDigit(utf32_t c);

bool
utfIsLetterOrNumber(utf32_t c);

bool
utfIsLowerCase(utf32_t c);

bool
utfIsUpperCase(utf32_t c);

//..............................................................................

// case ops

utf32_t
utfToLowerCase(utf32_t c);

utf32_t
utfToUpperCase(utf32_t c);

utf32_t
utfToCaseFolded(utf32_t c);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UtfToLowerCase
{
public:
	utf32_t
	operator () (utf32_t x) const
	{
		return utfToLowerCase(x);
	}
};

class UtfToUpperCase
{
public:
	utf32_t
	operator () (utf32_t x) const
	{
		return utfToUpperCase(x);
	}
};

class UtfToCaseFolded
{
public:
	utf32_t
	operator () (utf32_t x) const
	{
		return utfToCaseFolded(x);
	}
};

//..............................................................................

template <typename T>
size_t
calcUtfCodePointCount(
	const typename T::C* p,
	size_t length
	)
{
	const typename T::C* end = p + length;

	size_t count = 0;
	while (p < end)
	{
		size_t srcCodePointLength = T::getDecodeCodePointLength(*p);
		if (p + srcCodePointLength > end)
			break;

		p += srcCodePointLength;
		count++;
	}

	return count;
}

//..............................................................................

class Utf8
{
public:
	typedef utf8_t C;

	static
	UtfKind
	getUtfKind()
	{
		return UtfKind_Utf8;
	}

	static
	const uint8_t*
	getBom()
	{
		static uint8_t bom[] = { 0xef, 0xbb, 0xbf };
		return bom;
	}

	static
	size_t
	getBomLength()
	{
		return 3;
	}

	static
	size_t
	getDecodeCodePointLength(utf8_t c)
	{
		return
			(c & 0x80) == 0    ? 1 :     // 0xxxxxxx
			(c & 0xe0) == 0xc0 ? 2 :     // 110xxxxx 10xxxxxx
			(c & 0xf0) == 0xe0 ? 3 :     // 1110xxxx 10xxxxxx 10xxxxxx
			(c & 0xf8) == 0xf0 ? 4 : 1;  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	}

	static
	size_t
	getEncodeCodePointLength(utf32_t x)
	{
		return
			x == 0xffff ? 1 :            // non-character U+FFFF
			x < 0x80 ? 1 :               // 0xxxxxxx
			x < 0x800 ? 2 :              // 110xxxxx 10xxxxxx
			x < 0x10000 ? 3 :            // 1110xxxx 10xxxxxx 10xxxxxx
			x < 0x200000 ? 4 : 1;        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	}

	static
	utf32_t
	decodeCodePoint(const utf8_t* p)
	{
		return
			((*p & 0x80) == 0)    ?      // 0xxxxxxx
				*p :
			((*p & 0xe0) == 0xc0) ?      // 110xxxxx 10xxxxxx
				((p[0] & 0x1f) << 6)  |
				 (p[1] & 0x3f) :
			((*p & 0xf0) == 0xe0) ?      // 1110xxxx 10xxxxxx 10xxxxxx
				((p[0] & 0x0f) << 12) |
				((p[1] & 0x3f) << 6)  |
				 (p[2] & 0x3f) :
			((*p & 0xf8) == 0xf0) ?      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
				((p[0] & 0x07) << 18) |
				((p[1] & 0x3f) << 12) |
				((p[2] & 0x3f) << 6)  |
				 (p[3] & 0x3f) : 0xffff; // use non-character U+FFFF (needs no surrogate in UTF-16)
	}

	static
	void
	encodeCodePoint(
		utf8_t* p,
		utf32_t x
		)
	{
		if (x == 0xffff)                 // non-character U+FFFF
		{
			p[0] = -1;
		}
		else if (x < 0x80)               // 0xxxxxxx
		{
			p[0] = (uint8_t)x;
		}
		else if (x < 0x800)              // 110xxxxx 10xxxxxx
		{
			p[0] = (uint8_t)(x >> 6)          | 0xc0;
			p[1] = (uint8_t)(x & 0x3f)        | 0x80;
		}
		else if (x < 0x10000)            // 1110xxxx 10xxxxxx 10xxxxxx
		{
			p[0] = (uint8_t)(x >> 12)         | 0xe0;
			p[1] = (uint8_t)((x >> 6) & 0x3f) | 0x80;
			p[2] = (uint8_t)(x & 0x3f)        | 0x80;
		}
		else if (x < 0x200000)           // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			p[0] = (uint8_t)(x >> 18)         | 0xf0;
			p[1] = (uint8_t)((x >> 12) & 0x3f)| 0x80;
			p[2] = (uint8_t)((x >> 6) & 0x3f) | 0x80;
			p[3] = (uint8_t)(x & 0x3f)        | 0x80;
		}
		else
		{
			p[0] = -1;
		}
	}

	static
	size_t
	calcCodePointCount(
		const C* p,
		size_t length
		)
	{
		return calcUtfCodePointCount<Utf8> (p, length);
	}
};

//..............................................................................

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
	getUtfKind()
	{
		return UtfKind_Utf16;
	}

	static
	const uint8_t*
	getBom()
	{
		static uint8_t bom[] = { 0xff, 0xfe };
		return bom;
	}

	static
	size_t
	getBomLength()
	{
		return 2;
	}

	static
	bool
	isLeadSurrogate(uint16_t c)
	{
		return c >= 0xd800 && c <= 0xdbff;
	}

	static
	bool
	isTrailSurrogate(uint16_t c)
	{
		return c >= 0xdc00 && c <= 0xdfff;
	}

	static
	bool
	needSurrogate(uint32_t x)
	{
		return x >= 0x10000;
	}

	static
	utf16_t
	getLeadSurrogate(uint32_t x)
	{
		return 0xd800 + (((x - 0x10000) >> 10) & 0x3ff);
	}

	static
	utf16_t
	getTrailSurrogate(uint32_t x)
	{
		return 0xdc00 + (x & 0x3ff);
	}

	static
	utf32_t
	getSurrogateCodePoint(
		uint16_t lead,
		uint16_t trail
		)
	{
		ASSERT(isLeadSurrogate(lead) && isTrailSurrogate(trail));
		return 0x10000 - (0xd800 << 10) - 0xdc00 + (lead << 10) + trail;
	}

	static
	size_t
	getDecodeCodePointLength(utf16_t c)
	{
		return isLeadSurrogate(c) ? 2 : 1;
	}

	static
	size_t
	getEncodeCodePointLength(utf32_t x)
	{
		return needSurrogate(x) ? 2 : 1;
	}

	static
	utf32_t
	decodeCodePoint(const utf16_t* p)
	{
		return isLeadSurrogate(*p) ? getSurrogateCodePoint(p[0], p[1]) : (uint16_t)*p;
	}

	static
	void
	encodeCodePoint(
		utf16_t* p,
		utf32_t x
		)
	{
		if (!needSurrogate(x))
		{
			*p = (utf16_t)x;
		}
		else
		{
			p[0] = getLeadSurrogate(x);
			p[1] = getTrailSurrogate(x);
		}
	}

	static
	size_t
	calcCodePointCount(
		const C* p,
		size_t length
		)
	{
		return calcUtfCodePointCount<Utf16> (p, length);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16_be: public Utf16
{
public:
	static
	UtfKind
	getUtfKind()
	{
		return UtfKind_Utf16_be;
	}

	static
	const uint8_t*
	getBom()
	{
		static uint8_t bom[] = { 0xfe, 0xff };
		return bom;
	}

	static
	size_t
	getDecodeCodePointLength(utf16_t c)
	{
		return isTrailSurrogate(c) ? 2 : 1;
	}

	static
	utf32_t
	decodeCodePoint(const utf16_t* p)
	{
		return isTrailSurrogate(*p) ? getSurrogateCodePoint(p[1], p[0]) : (uint16_t)*p;
	}

	static
	void
	encodeCodePoint(
		utf16_t* p,
		utf32_t x
		)
	{
		if (!needSurrogate(x))
		{
			*p = (utf16_t)x;
		}
		else
		{
			p[0] = getTrailSurrogate(x);
			p[1] = getLeadSurrogate(x);
		}
	}

	static
	size_t
	calcCodePointCount(
		const C* p,
		size_t length
		)
	{
		return calcUtfCodePointCount<Utf16_be> (p, length);
	}
};

//..............................................................................

class Utf32
{
public:
	typedef utf32_t C;

	static
	UtfKind
	getUtfKind()
	{
		return UtfKind_Utf32;
	}

	static
	const uint8_t*
	getBom()
	{
		static uint8_t bom[] = { 0xff, 0xfe, 0x00, 0x00 };
		return bom;
	}

	static
	size_t
	getBomLength()
	{
		return 4;
	}

	static
	size_t
	getDecodeCodePointLength(utf32_t c)
	{
		return 1;
	}

	static
	size_t
	getEncodeCodePointLength(utf32_t x)
	{
		return 1;
	}

	static
	utf32_t
	decodeCodePoint(const utf32_t* p)
	{
		return *p;
	}

	static
	void
	encodeCodePoint(
		utf32_t* p,
		utf32_t x
		)
	{
		*p = x;
	}

	static
	size_t
	calcCodePointCount(
		const C* p,
		size_t length
		)
	{
		return length;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32_be: public Utf32
{
public:
	static
	UtfKind
	getUtfKind()
	{
		return UtfKind_Utf32_be;
	}

	static
	const uint8_t*
	getBom()
	{
		static uint8_t bom[] = { 0x00, 0x00, 0xfe, 0xff };
		return bom;
	}

	static
	utf32_t
	decodeCodePoint(const utf32_t* p)
	{
		return sl::swapByteOrder32(*p);
	}

	static
	void
	encodeCodePoint(
		utf32_t* p,
		utf32_t x
		)
	{
		*p = sl::swapByteOrder32(x);
	}
};

//..............................................................................

template <
	typename DstEncoding0,
	typename SrcEncoding0,
	typename CaseOp = sl::NoOp<utf32_t>
	>
class UtfConvert
{
public:
	typedef DstEncoding0 DstEncoding;
	typedef SrcEncoding0 SrcEncoding;
	typedef typename DstEncoding::C DstUnit;
	typedef typename SrcEncoding::C SrcUnit;

public:
	static
	size_t
	calcRequiredLength(
		const SrcUnit* p,
		size_t length
		)
	{
		const SrcUnit* end = p + length;

		size_t resultLength = 0;
		while (p < end)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength(*p);
			if (p + srcCodePointLength > end)
				break;

			utf32_t x = CaseOp() (SrcEncoding::decodeCodePoint(p));
			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength(x);

			resultLength += dstCodePointLength;
			p += srcCodePointLength;
		}

		return resultLength;
	}

	static
	size_t
	convert(
		DstUnit* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		size_t* takenSrcLength_o = NULL
		)
	{
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const SrcUnit* src0 = src;
		const SrcUnit* srcEnd = src + srcLength;

		while (src < srcEnd)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength(*src);
			if (src + srcCodePointLength > srcEnd)
				break;

			utf32_t x = CaseOp() (SrcEncoding::decodeCodePoint(src));
			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength(x);
			if (dst + dstCodePointLength > dstEnd)
				break;

			DstEncoding::encodeCodePoint(dst, x);

			dst += dstCodePointLength;
			src += srcCodePointLength;
		}

		if (takenSrcLength_o)
			*takenSrcLength_o = src - src0;

		return dst - dst0;
	}

	static
	size_t
	convert(
		uchar_t* cpl, // src code point length table
		DstUnit* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		size_t* takenSrcLength_o = NULL
		)
	{
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const SrcUnit* src0 = src;
		const SrcUnit* srcEnd = src + srcLength;

		while (src < srcEnd)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength(*src);
			if (src + srcCodePointLength > srcEnd)
				break;

			utf32_t x = CaseOp() (SrcEncoding::decodeCodePoint(src));
			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength(x);
			if (dst + dstCodePointLength > dstEnd)
				break;

			DstEncoding::encodeCodePoint(dst, x);
			*cpl++ = (uchar_t)srcCodePointLength;

			dst += dstCodePointLength;
			src += srcCodePointLength;
		}

		if (takenSrcLength_o)
			*takenSrcLength_o = src - src0;

		return dst - dst0;
	}
};

//..............................................................................

template <
	typename SrcEncoding0,
	typename CaseOp = sl::NoOp<utf32_t>
	>
class UtfToAsciiConvert
{
public:
	typedef SrcEncoding0 SrcEncoding;
	typedef typename SrcEncoding::C SrcUnit;

public:
	static
	size_t
	calcRequiredLength(
		const SrcUnit* p,
		size_t length
		)
	{
		const SrcUnit* end = p + length;

		size_t resultLength = 0;
		while (p < end)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength(*p);
			if (p + srcCodePointLength > end)
				break;

			resultLength++;
			p += srcCodePointLength;
		}

		return resultLength;
	}

	static
	size_t
	convert(
		char* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		size_t* takenSrcLength_o = NULL
		)
	{
		char* dst0 = dst;
		char* dstEnd = dst + dstLength;
		const SrcUnit* src0 = src;
		const SrcUnit* srcEnd = src + srcLength;

		while (src < srcEnd && dst < dstEnd)
		{
			size_t srcCodePointLength = SrcEncoding::getDecodeCodePointLength(*src);
			if (src + srcCodePointLength > srcEnd)
				break;

			utf32_t x = CaseOp() (SrcEncoding::decodeCodePoint(src));
			*dst = (char)x;

			dst++;
			src += srcCodePointLength;
		}

		if (takenSrcLength_o)
			*takenSrcLength_o = src - src0;

		return dst - dst0;
	}
};

//..............................................................................

template <
	typename DstEncoding0,
	typename CaseOp = sl::NoOp<utf32_t>
	>
class AsciiToUtfConvert
{
public:
	typedef DstEncoding0 DstEncoding;
	typedef typename DstEncoding::C DstUnit;

public:
	static
	size_t
	calcRequiredLength(
		const char* p,
		size_t length
		)
	{
		const char* end = p + length;

		size_t resultLength = 0;
		while (p < end)
		{
			utf32_t x = CaseOp() ((uchar_t)*p); // don't propagate sign bit

			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength(x);

			resultLength += dstCodePointLength;
			p++;
		}

		return resultLength;
	}

	static
	size_t
	convert(
		DstUnit* dst,
		size_t dstLength,
		const char* src,
		size_t srcLength,
		size_t* takenSrcLength_o = NULL
		)
	{
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const char* src0 = src;
		const char* srcEnd = src + srcLength;

		while (src < srcEnd)
		{
			utf32_t x = CaseOp() ((uchar_t)*src); // don't propagate sign bit

			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength(x);
			if (dst + dstCodePointLength > dstEnd)
				break;

			DstEncoding::encodeCodePoint(dst, x);

			dst += dstCodePointLength;
			src++;
		}

		if (takenSrcLength_o)
			*takenSrcLength_o = src - src0;

		return dst - dst0;
	}

	static
	size_t
	convert(
		uchar_t* cpl,
		DstUnit* dst,
		size_t dstLength,
		const char* src,
		size_t srcLength,
		size_t* takenSrcLength_o = NULL
		)
	{
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const char* src0 = src;
		const char* srcEnd = src + srcLength;

		while (src < srcEnd)
		{
			utf32_t x = CaseOp() ((uchar_t)*src); // don't propagate sign bit

			size_t dstCodePointLength = DstEncoding::getEncodeCodePointLength(x);
			if (dst + dstCodePointLength > dstEnd)
				break;

			DstEncoding::encodeCodePoint(dst, x);
			*cpl++ = 1;

			dst += dstCodePointLength;
			src++;
		}

		if (takenSrcLength_o)
			*takenSrcLength_o = src - src0;

		return dst - dst0;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
