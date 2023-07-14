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

#define _AXL_ENC_ESCAPEENCODING_H

#include "axl_enc_HexEncoding.h"
#include "axl_enc_CharCodec.h"
#include "axl_err_Error.h"

namespace axl {
namespace enc {

//..............................................................................

enum EscapeEncodingFlag {
	EscapeEncodingFlag_UpperCase   = 0x01,
	EscapeEncodingFlag_NoTabEscape = 0x02,
	EscapeEncodingFlag_NoCrEscape  = 0x04,
	EscapeEncodingFlag_NoLfEscape  = 0x08,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class EscapeEncodingRoot {
public:
	static
	utf32_t
	findEscapeChar(
		utf32_t c,
		uint_t flags = 0
	) {
		switch (c) {
		case '\0':
			return '0';

		case '\a':
			return 'a';

		case '\b':
			return 'b';

		case '\t':
			return (flags & EscapeEncodingFlag_NoTabEscape) ? 0 : 't';

		case '\n':
			return (flags & EscapeEncodingFlag_NoLfEscape) ? 0 : 'n';

		case '\f':
			return 'f';

		case '\v':
			return 'v';

		case '\r':
			return (flags & EscapeEncodingFlag_NoCrEscape) ? 0 : 'r';

		case '\x1b':
			return 'e';

		default:
			return c;
		};
	}

	static
	utf32_t
	findEscapeReplaceChar(utf32_t c) {
		switch (c) {
		case '0':
			return '\0';

		case 'a':
			return '\a';

		case 'b':
			return '\b';

		case 't':
			return '\t';

		case 'n':
			return '\n';

		case 'f':
			return '\f';

		case 'v':
			return '\v';

		case 'r':
			return '\r';

		case 'e':
			return '\x1b';

		default:
			return c;
		};
	}

	static
	bool
	isHexChar(utf32_t c) {
		return
			c >= '0' && c <= '9' ||
			c >= 'a' && c <= 'f' ||
			c >= 'A' && c <= 'F';
	}

protected:
	template <
		typename GetHexChar,
		typename C
	>
	static
	size_t
	buildHexCodeEscapeSequence(
		C* escapeSequence,
		utf32_t c
	) {
		if ((uint32_t)c <= 0xff) {
			escapeSequence[1] = 'x';
			escapeSequence[2] = GetHexChar()(c >> 4);
			escapeSequence[3] = GetHexChar()(c);
			return 4;
		}

		if ((uint32_t)c <= 0xffff) {
			escapeSequence[1] = 'u';
			escapeSequence[2] = GetHexChar()(c >> 12);
			escapeSequence[3] = GetHexChar()(c >> 8);
			escapeSequence[4] = GetHexChar()(c >> 4);
			escapeSequence[5] = GetHexChar()(c);
			return 6;
		}

		escapeSequence[1] = 'U';
		escapeSequence[2] = GetHexChar()(c >> 28);
		escapeSequence[3] = GetHexChar()(c >> 24);
		escapeSequence[4] = GetHexChar()(c >> 20);
		escapeSequence[5] = GetHexChar()(c >> 16);
		escapeSequence[6] = GetHexChar()(c >> 12);
		escapeSequence[7] = GetHexChar()(c >> 8);
		escapeSequence[8] = GetHexChar()(c >> 4);
		escapeSequence[9] = GetHexChar()(c);
		return 10;
	}

};

//..............................................................................

template <typename T>
class EscapeEncodingUtf: public EscapeEncodingRoot {
public:
	typedef T C;
	typedef typename sl::StringDetailsBase<T>::Encoding UtfEncoding;

public:
	static
	size_t
	encode(
		sl::StringBase<C>* string,
		const sl::StringRefBase<C>& source,
		uint_t flags = 0
	) {
		string->clear();
		string->reserve(source.getLength());

		C escapeSequence[16] = { '\\' };

		const C* src = source.cp();
		const C* srcEnd = source.getEnd();

		while (src < srcEnd) {
			utf32_t buffer[64];

			ConvertResult<Utf32::C, UtfEncoding::C> result = Convert<Utf32, UtfEncoding>::convert(
				buffer,
				buffer + countof(buffer),
				src,
				srcEnd
			);

			if (result.m_src == src)
				break;

			src = result.m_src;

			const utf32_t* p = buffer;
			const utf32_t* end = result.m_dst;
			const utf32_t* base = p;

			for (; p < end; p++) {
				utf32_t c = *p;

				if (c == '\\') {
					escapeSequence[1] = '\\';
					string->append(base, p - base);
					string->append(escapeSequence, 2);
					base = p + 1;
				} else {
					if (isPrintable(c))
						continue;

					utf32_t escape = findEscapeChar(c, flags);
					if (!escape)
						continue;

					string->append(base, p - base);

					size_t escapeSequenceLength;
					if (escape != c) {
						escapeSequence[1] = (C)escape;
						escapeSequenceLength = 2;
					} else {
						escapeSequenceLength = (flags & EscapeEncodingFlag_UpperCase) ?
							buildHexCodeEscapeSequence<GetHexChar_u>(escapeSequence, c) :
							buildHexCodeEscapeSequence<GetHexChar_l>(escapeSequence, c);
					}

					string->append(escapeSequence, escapeSequenceLength);
					base = p + 1;
				}
			}

			string->append(sl::StringRef_utf32(base, p - base));
		}

		return string->getLength();
	}

	static
	sl::StringBase<C>
	encode(
		const sl::StringRefBase<C>& source,
		uint_t flags = 0
	) {
		sl::StringBase<C> string;
		encode(&string, source, flags);
		return string;
	}

	static
	size_t
	decodeEscape(
		utf32_t* result,
		const sl::StringRefBase<C>& source
	) {
		size_t length = source.getLength();
		if (length < 2) {
			err::setError("invalid escape sequence");
			return -1;
		}

		ASSERT(source[0] == '\\');
		C c = source[1];

		size_t hexCodeLen;

		switch (c) {
		case 'x':
			hexCodeLen = 2;
			break;

		case 'u':
			hexCodeLen = 4;
			break;

		case 'U':
			hexCodeLen = 8;
			break;

		default:
			*result = findEscapeReplaceChar(c);
			return 2;
		}

		const C* p = source.cp() + 2;
		const C* end = source.getEnd();
		char hexCodeString[16];

		size_t i = 0;
		for (; i < hexCodeLen && p < end && isHexChar(*p); i++, p++)
			hexCodeString[i] = *p;

		if (!i) {
			err::setError("invalid escape sequence");
			return -1;
		}

		hexCodeString[i] = 0;
		*result = strtoul(hexCodeString, NULL, 16);
		return 2 + i;
	}

	static
	size_t
	decode(
		sl::StringBase<C>* string,
		const sl::StringRefBase<C>& source
	) {
		enum State {
			State_Normal = 0,
			State_Escape,
			State_Hex,
		};

		State state = State_Normal;

		string->clear();
		string->reserve(source.getLength());

		char hexCodeString[16];
		size_t hexCodeLen;
		size_t hexCodeMaxLen;
		ulong_t hexCode;

		const C* p = source.cp();
		const C* end = source.getEnd();
		const C* base = p;

		for (; p < end; p++) {
			C c = *p;

			switch (state) {
			case State_Normal:
				if (c == '\\') {
					string->append(base, p - base);
					state = State_Escape;
				}

				break;

			case State_Escape:
				switch (c) {
				case 'x':
					state = State_Hex;
					hexCodeLen = 0;
					hexCodeMaxLen = 2;
					break;

				case 'u':
					state = State_Hex;
					hexCodeLen = 0;
					hexCodeMaxLen = 4;
					break;

				case 'U':
					state = State_Hex;
					hexCodeLen = 0;
					hexCodeMaxLen = 8;
					break;

				default:
					C replace = (C)findEscapeReplaceChar(c);
					if (replace != c) {
						string->append(replace, 1);
						base = p + 1;
					} else {
						base = p;
					}

					state = State_Normal;
				}

				break;

			case State_Hex:
				if (isHexChar(c)) {
					hexCodeString[hexCodeLen++] = (char)c;
					if (hexCodeLen < hexCodeMaxLen)
						break;

					base = p + 1;
				} else {
					base = p;
				}

				hexCodeString[hexCodeLen] = 0;
				hexCode = strtoul(hexCodeString, NULL, 16);

				if (hexCodeMaxLen == 2) { // \x
					string->append((const C*)&hexCode, 1);
				} else { // \u or \U
					C buffer[4];
					C* end = UtfEncoding::Encoder::encode(buffer, hexCode);
					string->append(buffer, end - buffer);
				}

				state = State_Normal;
				break;
			}
		}

		if (state == State_Normal) // otherwise, unterminated escape
			string->append(base, p - base);

		return string->getLength();
	}

	static
	sl::StringBase<C>
	decode(const sl::StringRefBase<C>& source) {
		sl::StringBase<C> string;
		decode(&string, source);
		return string;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef EscapeEncodingUtf<char>    EscapeEncoding;
typedef EscapeEncodingUtf<wchar_t> EscapeEncoding_w;
typedef EscapeEncodingUtf<utf8_t>  EscapeEncoding_utf8;
typedef EscapeEncodingUtf<utf16_t> EscapeEncoding_utf16;
typedef EscapeEncodingUtf<utf32_t> EscapeEncoding_utf32;

//..............................................................................

class EscapeEncodingDynamic: EscapeEncodingRoot {
public:
	static
	size_t
	encode(
		CharCodec* codec,
		sl::Array<char>* buffer,
		const void* p,
		size_t size,
		uint_t flags = 0
	);

	static
	size_t
	encode(
		CharCodecKind codecKind,
		sl::Array<char>* buffer,
		const void* p,
		size_t size,
		uint_t flags = 0
	) {
		return encode(getCharCodec(codecKind), buffer, p, size, flags);
	}

	static
	sl::Array<char>
	encode(
		CharCodec* codec,
		const void* p,
		size_t size,
		uint_t flags = 0
	) {
		sl::Array<char> buffer;
		encode(codec, &buffer, p, size, flags);
		return buffer;
	}

	static
	sl::Array<char>
	encode(
		CharCodecKind codecKind,
		const void* p,
		size_t size,
		uint_t flags = 0
	) {
		return encode(getCharCodec(codecKind), p, size, flags);
	}

	static
	size_t
	decode(
		CharCodec* codec,
		sl::Array<char>* buffer,
		const void* p,
		size_t size
	);

	static
	size_t
	decode(
		CharCodecKind codecKind,
		sl::Array<char>* buffer,
		const void* p,
		size_t size
	) {
		return decode(getCharCodec(codecKind), buffer, p, size);
	}

	static
	sl::Array<char>
	decode(
		CharCodec* codec,
		const void* p,
		size_t size
	) {
		sl::Array<char> buffer;
		decode(codec, &buffer, p, size);
		return buffer;
	}

	static
	sl::Array<char>
	decode(
		CharCodecKind codecKind,
		const void* p,
		size_t size
	) {
		return decode(getCharCodec(codecKind), p, size);
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
