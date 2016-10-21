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
#include "axl_enc_HexEncoding.h"

namespace axl {
namespace enc {

//..............................................................................

size_t
HexEncoding::encode (
	sl::String* string,
	const void* p,
	size_t size,
	uint_t flags
	)
{
	if (!size)
	{
		string->clear ();
		return 0;
	}

	const uchar_t* src = (const uchar_t*) p;
	const uchar_t* srcEnd = src + size;

	size_t length;
	if ((flags & Flag_NoSpace))
	{
		length = size * 2;
		char* dst = string->createBuffer (length);
		if (!dst)
			return -1;

		if (flags & Flag_UpperCase)
			encode_nsu (dst, src, srcEnd);
		else
			encode_nsl (dst, src, srcEnd);
	}
	else
	{
		length = size * 3 - 1;
		char* dst = string->createBuffer (length);
		if (!dst)
			return -1;

		if (flags & Flag_UpperCase)
			encode_u (dst, src, srcEnd);
		else
			encode_l (dst, src, srcEnd);
	}

	return length;
}

void
HexEncoding::encode_l (
	char* dst,
	const uchar_t* src,
	const uchar_t* srcEnd
	)
{
	uchar_t x = *src;
	*dst++ = getHexChar_l (x >> 4);
	*dst++ = getHexChar_l (x);

	for (src++; src < srcEnd; src++)
	{
		uchar_t x = *src;

		*dst++ = ' ';
		*dst++ = getHexChar_l (x >> 4);
		*dst++ = getHexChar_l (x);
	}
}

void
HexEncoding::encode_u (
	char* dst,
	const uchar_t* src,
	const uchar_t* srcEnd
	)
{
	uchar_t x = *src;
	*dst++ = getHexChar_u (x >> 4);
	*dst++ = getHexChar_u (x);

	for (src++; src < srcEnd; src++)
	{
		uchar_t x = *src;

		*dst++ = ' ';
		*dst++ = getHexChar_u (x >> 4);
		*dst++ = getHexChar_u (x);
	}
}

void
HexEncoding::encode_nsl (
	char* dst,
	const uchar_t* src,
	const uchar_t* srcEnd
	)
{
	for (; src < srcEnd; src++)
	{
		uchar_t x = *src;

		*dst++ = getHexChar_l (x >> 4);
		*dst++ = getHexChar_l (x);
	}
}

void
HexEncoding::encode_nsu (
	char* dst,
	const uchar_t* src,
	const uchar_t* srcEnd
	)
{
	for (; src < srcEnd; src++)
	{
		uchar_t x = *src;

		*dst++ = getHexChar_u (x >> 4);
		*dst++ = getHexChar_u (x);
	}
}

size_t
HexEncoding::decode (
	sl::Array <char>* buffer,
	const sl::StringRef& source
	)
{
	enum State
	{
		State_Normal = 0,
		State_Hex
	};

	State state = State_Normal;

	buffer->clear ();
	buffer->reserve (source.getLength () / 2);

	char hexCodeString [4] = { 0 };
	char* hexCodeEnd;
	size_t hexCodeLen;

	uchar_t x;

	const char* p = source.cp ();
	const char* end = source.getEnd ();

	for (; p < end; p++)
	{
		bool_t isSpace = isspace (*p);

		switch (state)
		{
		case State_Normal:
			if (isSpace)
				break;

			hexCodeString [0] = *p;
			hexCodeLen = 1;
			state = State_Hex;
			break;

		case State_Hex:
			if (!isSpace)
			{
				hexCodeString [hexCodeLen++] = *p;
				if (hexCodeLen < 2)
					break;
			}

			hexCodeString [hexCodeLen] = 0;
			x = (uchar_t) strtoul (hexCodeString, &hexCodeEnd, 16);

			if (hexCodeEnd == &hexCodeString [hexCodeLen])
				buffer->append (x);
			else
				p = end; // not a hex string anymore, break the loop

			state = State_Normal;
			break;
		}
	}

	if (state == State_Hex)
	{
		hexCodeString [hexCodeLen] = 0;
		x = (uchar_t) strtoul (hexCodeString, &hexCodeEnd, 16);

		if (hexCodeEnd == &hexCodeString [hexCodeLen])
			buffer->append (x);
	}

	return buffer->getCount ();
}

//..............................................................................

} // namespace enc
} // namespace axl
