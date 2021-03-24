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

namespace axl {
namespace enc {

//..............................................................................

class EscapeEncodingRoot
{
public:
	static
	utf32_t
	findEscapeChar(utf32_t c)
	{
		switch (c)
		{
		case '\0':
			return '0';

		case '\a':
			return 'a';

		case '\b':
			return 'b';

		case '\t':
			return 't';

		case '\n':
			return 'n';

		case '\f':
			return 'f';

		case '\v':
			return 'v';

		case '\r':
			return 'r';

		case '\x1b':
			return 'e';

		default:
			return c;
		};
	}

	static
	utf32_t
	findEscapeReplaceChar(utf32_t c)
	{
		switch (c)
		{
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
	isHexChar(utf32_t c)
	{
		return
			c >= '0' && c <= '9' ||
			c >= 'a' && c <= 'f' ||
			c >= 'A' && c <= 'F';
	}
};

//..............................................................................

template <typename T>
class EscapeEncodingUtf: public EscapeEncodingRoot
{
public:
	typedef T C;
	typedef typename sl::StringDetailsBase<T>::Encoding UtfEncoding;

public:
	static
	size_t
	encode(
		sl::StringBase<C>* string,
		const sl::StringRefBase<C>& source
		)
	{
		string->clear();
		string->reserve(source.getLength());

		C escapeSequence[16] = { '\\' };

		const C* src = source.cp();
		const C* srcEnd = source.getEnd();

		while (src < srcEnd)
		{
			utf32_t buffer[64];

			size_t takenSrcLength;
			size_t takenBufferLength = UtfConvert<Utf32, UtfEncoding>::convert(
				buffer,
				countof(buffer),
				src,
				srcEnd - src,
				&takenSrcLength
				);

			if (!takenSrcLength)
				break;

			src += takenSrcLength;

			const utf32_t* p = buffer;
			const utf32_t* end = p + takenBufferLength;
			const utf32_t* base = p;

			for (; p < end; p++)
			{
				utf32_t c = *p;

				if (c == '\\')
				{
					escapeSequence[1] = '\\';

					string->append(base, p - base);
					string->append(escapeSequence, 2);

					base = p + 1;
				}
				else
				{
					if (utfIsPrintable(c))
						continue;

					string->append(base, p - base);

					utf32_t escape = findEscapeChar(c);
					if (escape != c)
					{
						escapeSequence[1] = (C)escape;
						string->append(escapeSequence, 2);
					}
					else if ((uint32_t)c <= 0xff)
					{
						escapeSequence[1] = 'x';
						escapeSequence[2] = HexEncoding::getHexChar_l(c >> 4);
						escapeSequence[3] = HexEncoding::getHexChar_l(c);
						string->append(escapeSequence, 4);
					}
					else if ((uint32_t)c <= 0xffff)
					{
						escapeSequence[1] = 'u';
						escapeSequence[2] = HexEncoding::getHexChar_l(c >> 12);
						escapeSequence[3] = HexEncoding::getHexChar_l(c >> 8);
						escapeSequence[4] = HexEncoding::getHexChar_l(c >> 4);
						escapeSequence[5] = HexEncoding::getHexChar_l(c);
						string->append(escapeSequence, 6);
					}
					else
					{
						escapeSequence[1] = 'U';
						escapeSequence[2] = HexEncoding::getHexChar_l(c >> 28);
						escapeSequence[3] = HexEncoding::getHexChar_l(c >> 24);
						escapeSequence[4] = HexEncoding::getHexChar_l(c >> 20);
						escapeSequence[5] = HexEncoding::getHexChar_l(c >> 16);
						escapeSequence[6] = HexEncoding::getHexChar_l(c >> 12);
						escapeSequence[7] = HexEncoding::getHexChar_l(c >> 8);
						escapeSequence[8] = HexEncoding::getHexChar_l(c >> 4);
						escapeSequence[9] = HexEncoding::getHexChar_l(c);
						string->append(escapeSequence, 10);
					}

					base = p + 1;
				}
			}

			string->append(sl::StringRef_utf32(base, p - base));
		}

		return string->getLength();
	}

	static
	sl::StringBase<C>
	encode(const sl::StringRefBase<C>& source)
	{
		sl::StringBase<C> string;
		encode(&string, source);
		return string;
	}

	static
	size_t
	decode(
		sl::StringBase<C>* string,
		const sl::StringRefBase<C>& source
		)
	{
		enum State
		{
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

		for (; p < end; p++)
		{
			C c = *p;

			switch (state)
			{
			case State_Normal:
				if (c == '\\')
				{
					string->append(base, p - base);
					state = State_Escape;
				}

				break;

			case State_Escape:
				switch (c)
				{
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
					if (replace != c)
					{
						string->append(replace, 1);
						base = p + 1;
					}
					else
					{
						base = p;
					}

					state = State_Normal;
				}

				break;

			case State_Hex:
				if (isHexChar(c))
				{
					hexCodeString[hexCodeLen++] = (char)c;
					if (hexCodeLen < hexCodeMaxLen)
						break;

					base = p + 1;
				}
				else
				{
					base = p;
				}

				hexCodeString[hexCodeLen] = 0;
				hexCode = strtoul(hexCodeString, NULL, 16);

				if (hexCodeMaxLen == 2) // \x
				{
					string->append((const C*)&hexCode, 1);
				}
				else // \u or \U
				{
					C buffer[4];
					size_t length = UtfEncoding::getEncodeCodePointLength(hexCode);
					UtfEncoding::encodeCodePoint(buffer, hexCode);
					string->append(buffer, length);
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
	decode(const sl::StringRefBase<C>& source)
	{
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

class EscapeEncodingDynamic: EscapeEncodingRoot
{
public:
	static
	size_t
	encode(
		CharCodec* codec,
		sl::Array<char>* buffer,
		const void* p,
		size_t size
		);

	static
	size_t
	encode(
		CharCodecKind codecKind,
		sl::Array<char>* buffer,
		const void* p,
		size_t size
		)
	{
		return encode(getCharCodec(codecKind), buffer, p, size);
	}

	static
	sl::Array<char>
	encode(
		CharCodec* codec,
		const void* p,
		size_t size
		)
	{
		sl::Array<char> buffer;
		encode(codec, &buffer, p, size);
		return buffer;
	}

	static
	sl::Array<char>
	encode(
		CharCodecKind codecKind,
		const void* p,
		size_t size
		)
	{
		return encode(getCharCodec(codecKind), p, size);
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
		)
	{
		return decode(getCharCodec(codecKind), buffer, p, size);
	}

	static
	sl::Array<char>
	decode(
		CharCodec* codec,
		const void* p,
		size_t size
		)
	{
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
		)
	{
		return decode(getCharCodec(codecKind), p, size);
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
