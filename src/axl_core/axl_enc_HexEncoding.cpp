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

class InsertNoSpace
{
public:
	size_t
	operator () (
		char* p,
		size_t i
		)
	{
		return 0;
	}
};

class InsertNoSpaceMultiline
{
public:
	size_t
	operator () (
		char* p,
		size_t i
		)
	{
		if (i & 0x0f)
			return 0;

		*p = '\n';
		return 1;
	}
};

class InsertSpace
{
public:
	size_t
	operator () (
		char* p,
		size_t i
		)
	{
		*p = ' ';
		return 1;
	}
};

class InsertSpaceMultiline
{
public:
	size_t
	operator () (
		char* p,
		size_t i
		)
	{
		*p = (i & 0x0f) ? ' ' : '\n';
		return 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename GetHexChar,
	typename InsertSpace
	>
void
encodeImpl(
	char* dst,
	const uchar_t* src,
	size_t size
	)
{
	uchar_t x = src[0];
	*dst++ = GetHexChar()(x >> 4);
	*dst++ = GetHexChar()(x);

	for (size_t i = 1; i < size; i++)
	{
		uchar_t x = src[i];
		dst += InsertSpace()(dst, i);
		*dst++ = GetHexChar()(x >> 4);
		*dst++ = GetHexChar()(x);
	}
}

//..............................................................................

size_t
HexEncoding::encode(
	sl::String* string,
	const void* p,
	size_t size,
	uint_t flags
	)
{
	if (!size)
	{
		string->clear();
		return 0;
	}

	const uchar_t* src = (const uchar_t*) p;

	size_t length;
	if ((flags & HexEncodingFlag_NoSpace))
	{
		length = size * 2;
		if (flags & HexEncodingFlag_Multiline)
		{
			size_t lineCount = length / 16;
			if (lineCount & 0x0f)
				lineCount++;

			length += lineCount - 1;
		}

		char* dst = string->createBuffer(length);
		if (!dst)
			return -1;

		if (flags & HexEncodingFlag_Multiline)
			if (flags & HexEncodingFlag_UpperCase)
				encodeImpl<GetHexChar_u, InsertNoSpaceMultiline>(dst, src, size);
			else
				encodeImpl<GetHexChar_l, InsertNoSpaceMultiline>(dst, src, size);
		else
			if (flags & HexEncodingFlag_UpperCase)
				encodeImpl<GetHexChar_u, InsertNoSpace>(dst, src, size);
			else
				encodeImpl<GetHexChar_l, InsertNoSpace>(dst, src, size);
	}
	else
	{
		length = size * 3 - 1;
		char* dst = string->createBuffer(length);
		if (!dst)
			return -1;

		if (flags & HexEncodingFlag_Multiline)
			if (flags & HexEncodingFlag_UpperCase)
				encodeImpl<GetHexChar_u, InsertSpaceMultiline>(dst, src, size);
			else
				encodeImpl<GetHexChar_l, InsertSpaceMultiline>(dst, src, size);
		else
			if (flags & HexEncodingFlag_UpperCase)
				encodeImpl<GetHexChar_u, InsertSpace>(dst, src, size);
			else
				encodeImpl<GetHexChar_l, InsertSpace>(dst, src, size);
	}

	return length;
}

size_t
HexEncoding::decode(
	sl::Array<char>* buffer,
	const sl::StringRef& source
	)
{
	enum State
	{
		State_Normal = 0,
		State_Hex
	};

	State state = State_Normal;

	buffer->clear();
	buffer->reserve(source.getLength() / 2);

	char hexCodeString[4] = { 0 };
	char* hexCodeEnd;
	size_t hexCodeLen;

	uchar_t x;

	const char* p = source.cp();
	const char* end = source.getEnd();

	for (; p < end; p++)
	{
		bool_t isSpace = isspace(*p);

		switch (state)
		{
		case State_Normal:
			if (isSpace)
				break;

			hexCodeString[0] = *p;
			hexCodeLen = 1;
			state = State_Hex;
			break;

		case State_Hex:
			if (!isSpace)
			{
				hexCodeString[hexCodeLen++] = *p;
				if (hexCodeLen < 2)
					break;
			}

			hexCodeString[hexCodeLen] = 0;
			x = (uchar_t)strtoul(hexCodeString, &hexCodeEnd, 16);

			if (hexCodeEnd == &hexCodeString[hexCodeLen])
				buffer->append(x);
			else
				p = end; // not a hex string anymore, break the loop

			state = State_Normal;
			break;
		}
	}

	if (state == State_Hex)
	{
		hexCodeString[hexCodeLen] = 0;
		x = (uchar_t)strtoul(hexCodeString, &hexCodeEnd, 16);

		if (hexCodeEnd == &hexCodeString[hexCodeLen])
			buffer->append(x);
	}

	return buffer->getCount();
}

//..............................................................................

} // namespace enc
} // namespace axl
