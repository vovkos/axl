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
#include "axl_enc_EscapeEncoding.h"

namespace axl {
namespace enc {

//..............................................................................

static
inline
size_t
appendEncoded_utf32(
	CharCodec* codec,
	sl::Array<char>* finalBuffer,
	sl::Array<char>* encodeBuffer,
	const utf32_t* p,
	size_t length
	)
{
	codec->encode_utf32(encodeBuffer, p, length);
	return finalBuffer->append(*encodeBuffer);
}

static
inline
size_t
appendEncoded_utf8(
	CharCodec* codec,
	sl::Array<char>* finalBuffer,
	sl::Array<char>* encodeBuffer,
	const utf8_t* p,
	size_t length
	)
{
	codec->encode_utf8(encodeBuffer, p, length);
	return finalBuffer->append(*encodeBuffer);
}

size_t
EscapeEncodingDynamic::encode(
	CharCodec* codec,
	sl::Array<char>* buffer,
	const void* p0,
	size_t size
	)
{
	buffer->clear();
	buffer->reserve(size);

	sl::Array<char> encodeBuffer;
	char escapeSequence[16] = { '\\' };

	const char* src = (char*)p0;
	const char* srcEnd = src + size;

	while (src < srcEnd)
	{
		utf32_t decodeBuffer[64];

		size_t takenSrcSize;
		size_t takenBufferLength = codec->decode_utf32(
			decodeBuffer,
			countof(decodeBuffer),
			src,
			srcEnd - src,
			&takenSrcSize
			);

		if (!takenSrcSize)
			break;

		src += takenSrcSize;

		const utf32_t* p = decodeBuffer;
		const utf32_t* end = p + takenBufferLength;
		const utf32_t* base = p;

		for (; p < end; p++)
		{
			utf32_t c = *p;

			if (c == '\\')
			{
				escapeSequence[1] = '\\';

				appendEncoded_utf32(codec, buffer, &encodeBuffer, base, p - base);
				appendEncoded_utf8(codec, buffer, &encodeBuffer, escapeSequence, 2);

				base = p + 1;
			}
			else
			{
				if (isprint(c))
					continue;

				appendEncoded_utf32(codec, buffer, &encodeBuffer, base, p - base);

				utf32_t escape = findEscapeChar(c);
				if (escape != c)
				{
					escapeSequence[1] = (char)escape;
					appendEncoded_utf8(codec, buffer, &encodeBuffer, escapeSequence, 2);
				}
				else if ((uint32_t)c <= 0xff)
				{
					escapeSequence[1] = 'x';
					escapeSequence[2] = HexEncoding::getHexChar_l(c >> 4);
					escapeSequence[3] = HexEncoding::getHexChar_l(c);
					appendEncoded_utf8(codec, buffer, &encodeBuffer, escapeSequence, 4);
				}
				else if ((uint32_t)c <= 0xffff)
				{
					escapeSequence[1] = 'u';
					escapeSequence[2] = HexEncoding::getHexChar_l(c >> 16);
					escapeSequence[3] = HexEncoding::getHexChar_l(c >> 8);
					escapeSequence[4] = HexEncoding::getHexChar_l(c >> 4);
					escapeSequence[5] = HexEncoding::getHexChar_l(c);
					appendEncoded_utf8(codec, buffer, &encodeBuffer, escapeSequence, 6);
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
					appendEncoded_utf8(codec, buffer, &encodeBuffer, escapeSequence, 10);
				}

				base = p + 1;
			}
		}

		appendEncoded_utf32(codec, buffer, &encodeBuffer, base, p - base);
	}

	return buffer->getCount();
}

size_t
EscapeEncodingDynamic::decode(
	CharCodec* codec,
	sl::Array<char>* buffer,
	const void* p0,
	size_t size
	)
{
	enum State
	{
		State_Normal = 0,
		State_Escape,
		State_Hex,
	};

	State state = State_Normal;

	buffer->clear();
	buffer->reserve(size);

	sl::Array<char> encodeBuffer;
	char hexCodeString[16];
	size_t hexCodeLen;
	size_t hexCodeMaxLen;
	ulong_t hexCode;

	const char* src = (char*)p0;
	const char* srcEnd = src + size;

	while (src < srcEnd)
	{
		utf32_t decodeBuffer[64];

		size_t takenSrcSize;
		size_t takenBufferLength = codec->decode_utf32(
			decodeBuffer,
			countof(decodeBuffer),
			src,
			srcEnd - src,
			&takenSrcSize
			);

		if (!takenSrcSize)
			break;

		src += takenSrcSize;

		const utf32_t* p = decodeBuffer;
		const utf32_t* end = p + takenBufferLength;
		const utf32_t* base = p;

		for (; p < end; p++)
		{
			utf32_t c = *p;

			switch (state)
			{
			case State_Normal:
				if (c == '\\')
				{
					appendEncoded_utf32(codec, buffer, &encodeBuffer, base, p - base);
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
					utf32_t replace = findEscapeReplaceChar(c);
					if (replace != c)
					{
						appendEncoded_utf32(codec, buffer, &encodeBuffer, &replace, 1);
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
					buffer->append((const char*)&hexCode, 1);
				else // \u or \U
					appendEncoded_utf32(codec, buffer, &encodeBuffer, (utf32_t*)&hexCode, 1);

				state = State_Normal;
				break;
			}
		}

		if (state == State_Normal) // otherwise, unterminated escape
			appendEncoded_utf32(codec, buffer, &encodeBuffer, base, p - base);
	}

	return buffer->getCount();
}

//..............................................................................

} // namespace enc
} // namespace axl
