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

namespace axl {
namespace enc {

//..............................................................................

class EscapeEncodingRoot
{
public:
	static
	char
	findEscapeChar(char x);

	static
	char
	findEscapeReplaceChar(char x);
};

//..............................................................................

template <typename T>
class EscapeEncodingBase: public EscapeEncodingRoot
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

		C escapeSequence[4] = { '\\' };

		const C* p = source.cp();
		const C* end = source.getEnd();
		const C* base = p;

		for (; p < end; p++)
		{
			if (*p == '\\')
			{
				string->append(base, p - base);

				escapeSequence[1] = '\\';
				string->append(escapeSequence, 2);

				base = p + 1;
			}
			else
			{
				if (isprint(*(uchar_t*)p))
					continue;

				string->append(base, p - base);

				char escape = findEscapeChar(*p);
				if (escape != *p)
				{
					escapeSequence[1] = escape;
					string->append(escapeSequence, 2);
				}
				else
				{
					escapeSequence[1] = 'x';
					escapeSequence[2] = HexEncoding::getHexChar_l(*p >> 4);
					escapeSequence[3] = HexEncoding::getHexChar_l(*p);
					string->append(escapeSequence, 4);
				}

				base = p + 1;
			}
		}

		string->append(base, p - base);

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
		string->reserve(source.getLength() / 2);

		char replace;
		char hexCodeString[16];
		size_t hexCodeLen;
		size_t hexCodeMaxLen;
		ulong_t hexCode;

		const C* p = source.cp();
		const C* end = source.getEnd();
		const C* base = p;

		for (; p < end; p++)
		{
			switch (state)
			{
			case State_Normal:
				if (*p == '\\')
				{
					string->append(base, p - base);
					state = State_Escape;
				}

				break;

			case State_Escape:
				switch (*p)
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
					replace = findEscapeReplaceChar(*p);
					if (replace != *p)
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
				if (isHexChar(*p))
				{
					hexCodeString[hexCodeLen++] = *p;
					if (hexCodeLen < hexCodeMaxLen)
						break;

					base = p + 1;
				}
				else
				{
					base = p;
				}

				if (!hexCodeLen)
				{
					hexCode = '?';
					string->append((const C*)&hexCode, 1);
				}
				else
				{
					hexCodeString[hexCodeLen] = 0;
					hexCode = strtoul(hexCodeString, NULL, 16);

					if (hexCodeMaxLen == 2) // \x
					{
						string->append((const C*)&hexCode, 1);
					}
					else // \u
					{
						C buffer[4];
						size_t length = UtfEncoding::getEncodeCodePointLength(hexCode);
						UtfEncoding::encodeCodePoint(buffer, hexCode);
						string->append(buffer, length);
					}
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

protected:
	static
	bool
	isHexChar(C c)
	{
		return
			c >= '0' && c <= '9' ||
			c >= 'a' && c <= 'f' ||
			c >= 'A' && c <= 'F';
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef EscapeEncodingBase<char>    EscapeEncoding;
typedef EscapeEncodingBase<wchar_t> EscapeEncoding_w;
typedef EscapeEncodingBase<utf8_t>  EscapeEncoding_utf8;
typedef EscapeEncodingBase<utf16_t> EscapeEncoding_utf16;
typedef EscapeEncodingBase<utf32_t> EscapeEncoding_utf32;

//..............................................................................

} // namespace enc
} // namespace axl
