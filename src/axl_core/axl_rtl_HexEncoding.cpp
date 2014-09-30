#include "pch.h"
#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace rtl {

//.............................................................................

size_t
HexEncoding::encode (
	String* string,
	const void* p,
	size_t size,
	uint_t flags
	)
{
	if (!size)
		return 0;

	const uchar_t* src = (const uchar_t*) p;
	const uchar_t* srcEnd = src + size;

	size_t length;
	if ((flags & HexEncodeKind_NoSpace))
	{
		length = size * 2;
		char* dst = string->getBuffer (length, false);
		if (!dst)
			return -1;

		if (flags & HexEncodeKind_UpperCase)
			encode_nsu (dst, src, srcEnd);
		else
			encode_nsl (dst, src, srcEnd);
	}
	else
	{
		length = size * 3 - 1;
		char* dst = string->getBuffer (length, false);
		if (!dst)
			return -1;

		if (flags & HexEncodeKind_UpperCase)
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
	Array <uchar_t>* buffer,
	const char* p,
	size_t length
	)
{
	enum StateKind
	{
		StateKind_Normal = 0,
		StateKind_Hex
	};

	StateKind state = StateKind_Normal;

	if (length == -1)
		length = StringDetails::calcLength (p);

	buffer->reserve (length / 2);

	char hexCodeString [4] = { 0 };
	char* hexCodeEnd;
	size_t hexCodeLen;

	uchar_t x;

	const char* end = p + length;
	for (; p < end; p++)
	{
		bool_t isSpace = isspace(*p);

		switch (state)
		{
		case StateKind_Normal:
			if (isSpace)
				break;

			hexCodeLen = 0;
			hexCodeString [hexCodeLen++] = *p;
			state = StateKind_Hex;
			break;

		case StateKind_Hex:
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

			state = StateKind_Normal;
			break;
		}
	}

	if (state == StateKind_Hex)
	{
		hexCodeString [hexCodeLen] = 0;
		x = (uchar_t) strtoul (hexCodeString, &hexCodeEnd, 16);

		if (hexCodeEnd == &hexCodeString [hexCodeLen])
			buffer->append (x);
	}

	return buffer->getCount ();
}

//.............................................................................

} // namespace err
} // namespace axl
