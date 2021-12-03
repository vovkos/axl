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

#include "axl_enc_Utf8Decoder.h"
#include "axl_enc_Utf16Decoder.h"
#include "axl_enc_Utf32Decoder.h"

namespace axl {
namespace enc {

//..............................................................................

enum UtfKind {
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

// three strategies for handling invalid sequences or codepoints

// ignore errors and only emit valid codepoints

class UtfOnConvertErrorIgnore {
};

// emit at least one replacement char for an invalid sequence

class UtfOnConvertErrorReplaceOneOrMore {
};

// emit one replacement char for each code unit in an invalid sequence

class UtfOnConvertErrorReplaceEach {
};

//..............................................................................

class Utf8 {
public:
	typedef utf8_t C;
	typedef Utf8Decoder Decoder;
	typedef Utf8ReverseDecoder ReverseDecoder;

	/////////////////////////////////
	// deprecated
	/////////////////////////////////

	static
	size_t
	getDecodeCodePointLength(utf8_t c) {
		return
			(c & 0x80) == 0    ? 1 : // 0xxxxxxx
			(c & 0xe0) == 0xc0 ? 2 : // 110xxxxx 10xxxxxx
			(c & 0xf0) == 0xe0 ? 3 : // 1110xxxx 10xxxxxx 10xxxxxx
			(c & 0xf8) == 0xf0 ? 4 : // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			1;
	}

	static
	utf32_t
	decodeCodePoint(const utf8_t* p) {
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
				 (p[3] & 0x3f) :
			StdChar_Replacement;
	}

	/////////////////////////////////

	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf8;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xef, 0xbb, 0xbf };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 3;
	}

	static
	size_t
	getEncodeCodePointLength(
		utf32_t cp,
		utf32_t replacement = StdChar_Replacement
	) {
		return
			cp < 0x80 ? 1 :      // 0xxxxxxx
			cp < 0x10000 ?
				cp < 0x800 ? 2 : // 110xxxxx 10xxxxxx
				3 :             // 1110xxxx 10xxxxxx 10xxxxxx
			cp < 0x200000 ? 4 :  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			getEncodeCodePointLength(replacement);
	}

	static
	void
	encodeCodePoint(
		utf8_t* p,
		utf32_t cp,
		utf32_t replacement = StdChar_Replacement
	) {
		if (cp < 0x80) {
			p[0] = (uint8_t)cp;                             // 0xxxxxxx
		} else if (cp < 0x10000) {
			if (cp < 0x800) {
				p[0] = (uint8_t)(cp >> 6)          | 0xc0;  // 110xxxxx 10xxxxxx
				p[1] = (uint8_t)(cp & 0x3f)        | 0x80;
			} else {
				p[0] = (uint8_t)(cp >> 12)         | 0xe0;  // 1110xxxx 10xxxxxx 10xxxxxx
				p[1] = (uint8_t)((cp >> 6) & 0x3f) | 0x80;
				p[2] = (uint8_t)(cp & 0x3f)        | 0x80;
			}
		} else if (cp < 0x200000) {
			p[0] = (uint8_t)(cp >> 18)             | 0xf0; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			p[1] = (uint8_t)((cp >> 12) & 0x3f)    | 0x80;
			p[2] = (uint8_t)((cp >> 6) & 0x3f)     | 0x80;
			p[3] = (uint8_t)(cp & 0x3f)            | 0x80;
		} else {
			encodeCodePoint(p, replacement);
		}
	}
};

//..............................................................................

class Utf16Base {
public:
	typedef utf16_t C;

	static
	size_t
	getBomLength() {
		return 2;
	}

	static
	bool
	isSurrogate(uint32_t c) {
        return (c - 0xd800 < 0x800);  // d800 .. dfff;
	}

	static
	bool
	isSurrogateNeeded(uint32_t cp) {
		return cp >= 0x10000;
	}

	static
	utf16_t
	getHiSurrogate(uint32_t cp) {
		return 0xd800 + (((cp - 0x10000) >> 10) & 0x3ff);
	}

	static
	utf16_t
	getLoSurrogate(uint32_t cp) {
		return 0xdc00 + (cp & 0x3ff);
	}

	static
	size_t
	getEncodeCodePointLength(utf32_t cp) {
		return isSurrogateNeeded(cp) ? 2 : 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16: public Utf16Base {
public:
	typedef Utf16Decoder Decoder;
	typedef Utf16ReverseDecoder ReverseDecoder;

	/////////////////////////////////
	// deprecated
	/////////////////////////////////

	static
	size_t
	getDecodeCodePointLength(utf16_t c) {
		return isHiSurrogate(c) ? 2 : 1;
	}

	static
	utf32_t
	decodeCodePoint(const utf16_t* p) {
		return isHiSurrogate(*p) ? getSurrogateCodePoint(p[0], p[1]) : (uint16_t)*p;
 	}

	static
	utf32_t
	getSurrogateCodePoint(
		uint16_t lead,
		uint16_t trail
	) {
		ASSERT(isLeadSurrogate(lead) && isTrailSurrogate(trail));
		return 0x10000 - (0xd800 << 10) - 0xdc00 + (lead << 10) + trail;
	}

	/////////////////////////////////

	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf16;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xff, 0xfe };
		return bom;
	}

	static
	bool
	isHiSurrogate(uint16_t c) {
		return (c & 0xfc00) == 0xd800; // d800 .. dbff;
	}

	static
	bool
	isLoSurrogate(uint16_t c) {
		return (c & 0xfc00) == 0xdc00; // dc00 .. dfff;
	}

	static
	void
	encodeCodePoint(
		utf16_t* p,
		utf32_t cp
	) {
		if (!isSurrogateNeeded(cp)) {
			*p = (utf16_t)cp;
		} else {
			p[0] = getHiSurrogate(cp);
			p[1] = getLoSurrogate(cp);
		}
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16_be: public Utf16Base {
public:
	typedef Utf16Decoder_be Decoder;
	typedef Utf16ReverseDecoder_be ReverseDecoder;

	/////////////////////////////////
	// deprecated
	/////////////////////////////////

	static
	size_t
	getDecodeCodePointLength(utf16_t c) {
		return isHiSurrogate(sl::swapByteOrder16(c)) ? 2 : 1;
	}

	static
	utf32_t
	decodeCodePoint(const utf16_t* p) {
		uint16_t c = sl::swapByteOrder16(p[0]);
		return isHiSurrogate(c) ? Utf16::getSurrogateCodePoint(c, sl::swapByteOrder16(p[1])) : c;
 	}

	/////////////////////////////////

	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf16_be;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xfe, 0xff };
		return bom;
	}

	static
	bool
	isHiSurrogate(uint16_t c) {
		return Utf16::isHiSurrogate(sl::swapByteOrder16(c));
	}

	static
	bool
	isLoSurrogate(uint16_t c) {
		return Utf16::isLoSurrogate(sl::swapByteOrder16(c));
	}

	static
	void
	encodeCodePoint(
		utf16_t* p,
		utf32_t cp
	) {
		if (!isSurrogateNeeded(cp)) {
			*p = sl::swapByteOrder16((utf16_t)cp);
		} else {
			p[0] = sl::swapByteOrder16(getHiSurrogate(cp));
			p[1] = sl::swapByteOrder16(getLoSurrogate(cp));
		}
	}
};

//..............................................................................

class Utf32Base {
public:
	typedef utf32_t C;

	static
	size_t
	getBomLength() {
		return 4;
	}

	static
	size_t
	getDecodeCodePointLength(utf32_t c) {
		return 1;
	}

	static
	size_t
	getEncodeCodePointLength(utf32_t x) {
		return 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32: public Utf32Base {
public:
	typedef Utf8Decoder Decoder;
	typedef Utf8ReverseDecoder ReverseDecoder;

	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf32;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xff, 0xfe, 0x00, 0x00 };
		return bom;
	}

	static
	utf32_t
	decodeCodePoint(const utf32_t* p) {
		return *p;
	}

	static
	void
	encodeCodePoint(
		utf32_t* p,
		utf32_t x
	) {
		*p = x;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32_be: public Utf32Base {
public:
	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf32_be;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0x00, 0x00, 0xfe, 0xff };
		return bom;
	}

	static
	utf32_t
	decodeCodePoint(const utf32_t* p) {
		return sl::swapByteOrder32(*p);
	}

	static
	void
	encodeCodePoint(
		utf32_t* p,
		utf32_t x
	) {
		*p = sl::swapByteOrder32(x);
	}
};

//..............................................................................

template <
	typename DstEncoding0,
	typename SrcEncoding0,
	typename CaseOp = sl::NoOp<utf32_t>
>
class UtfConvert {
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
	) {
		const SrcUnit* end = p + length;

		size_t resultLength = 0;
		while (p < end) {
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
	) {
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const SrcUnit* src0 = src;
		const SrcUnit* srcEnd = src + srcLength;

		while (src < srcEnd) {
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
	) {
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const SrcUnit* src0 = src;
		const SrcUnit* srcEnd = src + srcLength;

		while (src < srcEnd) {
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
class UtfToAsciiConvert {
public:
	typedef SrcEncoding0 SrcEncoding;
	typedef typename SrcEncoding::C SrcUnit;

public:
	static
	size_t
	calcRequiredLength(
		const SrcUnit* p,
		size_t length
	) {
		const SrcUnit* end = p + length;

		size_t resultLength = 0;
		while (p < end) {
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
	) {
		char* dst0 = dst;
		char* dstEnd = dst + dstLength;
		const SrcUnit* src0 = src;
		const SrcUnit* srcEnd = src + srcLength;

		while (src < srcEnd && dst < dstEnd) {
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
class AsciiToUtfConvert {
public:
	typedef DstEncoding0 DstEncoding;
	typedef typename DstEncoding::C DstUnit;

public:
	static
	size_t
	calcRequiredLength(
		const char* p,
		size_t length
	) {
		const char* end = p + length;

		size_t resultLength = 0;
		while (p < end) {
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
	) {
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const char* src0 = src;
		const char* srcEnd = src + srcLength;

		while (src < srcEnd) {
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
	) {
		DstUnit* dst0 = dst;
		DstUnit* dstEnd = dst + dstLength;
		const char* src0 = src;
		const char* srcEnd = src + srcLength;

		while (src < srcEnd) {
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
