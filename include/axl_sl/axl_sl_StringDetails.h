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

#define _AXL_SL_STRING_H

#include "axl_enc_Utf.h"
#include "axl_sl_MemMem.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class StringDetailsBase
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class StringDetailsImpl
{
public:
	static
	const T*
	getEmptyString()
	{
		static T emptyString[] = { 0 };
		return emptyString;
	}

	static
	const T*
	getCrLf()
	{
		static T crLf[] = { '\r', '\n', 0 };
		return crLf;
	}

	static
	const T*
	getWhitespace()
	{
		static T whitespace[] = { ' ', '\t', '\r', '\n', 0 };
		return whitespace;
	}

	static
	size_t
	calcLength(const T* p)
	{
		if (!p)
			return 0;

		const T* p0 = p;
		while (*p)
			p++;

		return p - p0;
	}

	static
	size_t
	calcLength(
		const T* p,
		size_t maxLength
		)
	{
		if (!p)
			return 0;

		const T* p0 = p;
		const T* end = p + maxLength;
		while (p < end && *p)
			p++;

		return p - p0;
	}

	static
	int
	cmp(
		const T* p1,
		const T* p2,
		size_t length
		)
	{
		const T* end = p1 + length;
		for (; p1 < end; p1++, p2++)
			if (*p1 < *p2)
				return -1;
			else if (*p1 > *p2)
				return 1;

		return 0;
	}

	static
	size_t
	find(
		const T* p,
		size_t length,
		T c
		)
	{
		const T* p0 = p;
		const T* end = p + length;
		for (; p < end; p++)
			if (*p == c)
				return p - p0;

		return -1;
	}

	static
	size_t
	find(
		const T* p,
		size_t length,
		const T* subString,
		size_t subStringLength
		)
	{
		const T* f = (const T*) memMem(p, length * sizeof(T), subString, subStringLength * sizeof(T));
		return f ? f - p : -1;
	}

	static
	size_t
	findOneOf(
		const T* p,
		size_t length,
		const T* charSet,
		size_t charCount
		)
	{
		if (!charCount)
			return -1;

		const T* p0 = p;
		const T* end = p + length;
		for (; p < end; p++)
		{
			size_t f = find(charSet, charCount, *p);
			if (f != -1)
				return p - p0;
		}

		return -1;
	}

	static
	size_t
	findNotOneOf(
		const T* p,
		size_t length,
		const T* charSet,
		size_t charCount
		)
	{
		if (!charCount)
			return -1;

		const T* p0 = p;
		const T* end = p + length;
		for (; p < end; p++)
		{
			size_t f = find(charSet, charCount, *p);
			if (f == -1)
				return p - p0;
		}

		return -1;
	}

	static
	size_t
	reverseFind(
		const T* p,
		size_t length,
		T c
		)
	{
		if (!length)
			return -1;

		const T* p0 = p;
		for (p += length - 1; p >= p0; p--)
			if (*p == c)
				return p - p0;

		return -1;
	}

	static
	size_t
	reverseFind(
		const T* p,
		size_t length,
		const T* subString,
		size_t subStringLength
		)
	{
		const T* f = reverseMemMem(p, length * sizeof(T), subString, subStringLength * sizeof(T));
		return f ? f - p : -1;
	}

	static
	size_t
	reverseFindOneOf(
		const T* p,
		size_t length,
		const T* charSet,
		size_t charCount
		)
	{
		if (!length || !charCount)
			return -1;

		const T* p0 = p;
		for (p += length - 1; p >= p0; p--)
		{
			size_t f = find(charSet, charCount, *p);
			if (f != -1)
				return p - p0;
		}

		return -1;
	}

	static
	size_t
	reverseFindNotOneOf(
		const T* p,
		size_t length,
		const T* charSet,
		size_t charCount
		)
	{
		if (!length || !charCount)
			return -1;

		const T* p0 = p;
		for (p += length - 1; p >= p0; p--)
		{
			size_t f = find(charSet, charCount, *p);
			if (f == -1)
				return p - p0;
		}

		return -1;
	}
	static
	void
	fill(
		T* p,
		T c,
		size_t count
		)
	{
		const T* end = p + count;
		for (; p < end; p++)
			*p = c;
	}

	static
	void
	copy(
		T* dst,
		const T* src,
		size_t length
		)
	{
		memcpy(dst, src, length * sizeof(T));
	}

	static
	void
	move(
		T* dst,
		const T* src,
		size_t length
		)
	{
		memmove(dst, src, length * sizeof(T));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class StringDetailsBase<utf8_t>: public StringDetailsImpl<utf8_t>
{
public:
	typedef utf8_t  C;
	typedef utf16_t C2;
	typedef utf32_t C3;

	typedef enc::Utf8 Encoding;
	typedef StringDetailsBase<C2> Details2;
	typedef StringDetailsBase<C3> Details3;

	static
	size_t
	calcLength(const C* p)
	{
		return p ? strlen(p) : 0;
	}

	static
	size_t
	calcLength(
		const C* p,
		size_t maxLength
		)
	{
		return p ? strnlen(p, maxLength) : 0;
	}

	static
	C
	toLower(C c)
	{
		return (C)tolower((uint8_t)c);
	}

	static
	C
	toUpper(C c)
	{
		return (C)toupper((uint8_t)c);
	}

	static
	int
	cmp(
		const C* p1,
		const C* p2,
		size_t length
		)
	{
		return memcmp(p1, p2, length);
	}

	static
	size_t
	find(
		const C* p,
		size_t length,
		C c
		)
	{
		const C* f = (const C*) memchr(p, c, length);
		return f ? f - p : -1;
	}

	static
	size_t
	find(
		const C* p,
		size_t length,
		const C* subString,
		size_t subStringLength
		)
	{
		return StringDetailsImpl<utf8_t>::find(p, length, subString, subStringLength);
	}

	static
	void
	fill(
		C* p,
		C c,
		size_t count
		)
	{
		memset(p, c, count);
	}

#if (_AXL_CPP_MSC)
	static
	size_t
	calcFormatLength_va(
		const C* formatString,
		axl_va_list va
		)
	{
		return _vscprintf(formatString, va);
	}

	static
	void
	format_va(
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		_vsnprintf(buffer, bufferLength, formatString, va);
		if (bufferLength)
			buffer[bufferLength - 1] = 0; // not really necessary, just to make it identical to C++11 vsnprintf
	}
#else
	static
	size_t
	calcFormatLength_va(
		const C* formatString,
		axl_va_list va
		)
	{
		return vsnprintf(NULL, 0, formatString, va);
	}

	static
	size_t
	format_va(
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		return vsnprintf(buffer, bufferLength, formatString, va);
	}
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class StringDetailsBase<utf16_t>: public StringDetailsImpl<utf16_t>
{
public:
	typedef utf16_t C;
	typedef utf8_t  C2;
	typedef utf32_t C3;

	typedef enc::Utf16 Encoding;
	typedef StringDetailsBase<C2> Details2;
	typedef StringDetailsBase<C3> Details3;

	static
	C
	toLower(C c)
	{
		return (C)towlower((uint16_t)c);
	}

	static
	C
	toUpper(C c)
	{
		return (C)towupper((uint16_t)c);
	}

#if (_AXL_CPP_MSC)
	static
	size_t
	calcFormatLength_va(
		const C* formatString,
		axl_va_list va
		)
	{
		return _vscwprintf(formatString, va);
	}

	static
	void
	format_va(
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		_vsnwprintf(buffer, bufferLength, formatString, va);
		if (bufferLength)
			buffer[bufferLength - 1] = 0; // not really necessary, just to make it identical to C++11 vsnprintf
	}
#else
	static
	size_t
	calcFormatLength_va(
		const C* formatString,
		axl_va_list va
		)
	{
		ASSERT(false);
		return 0;
	}

	static
	void
	format_va(
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		ASSERT(false);
	}
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class StringDetailsBase<utf32_t>: public StringDetailsImpl<utf32_t>
{
public:
	typedef utf32_t C;
	typedef utf8_t  C2;
	typedef utf16_t C3;

	typedef enc::Utf32 Encoding;
	typedef StringDetailsBase<C2> Details2;
	typedef StringDetailsBase<C3> Details3;

	static
	C
	toLower(C c)
	{
		return (C)towlower((uint32_t)c);
	}

	static
	C
	toUpper(C c)
	{
		return (C)towupper((uint32_t)c);
	}

	// alas, _vscwprintf can't be emulated with vswprintf

	static
	size_t
	calcFormatLength_va(
		const C* formatString,
		axl_va_list va
		)
	{
		ASSERT(false);
		return 0;
	}

	static
	void
	format_va(
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		ASSERT(false);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef StringDetailsBase<char>    StringDetails;
typedef StringDetailsBase<wchar_t> StringDetails_w;
typedef StringDetailsBase<utf8_t>  StringDetails_utf8;
typedef StringDetailsBase<utf16_t> StringDetails_utf16;
typedef StringDetailsBase<utf32_t> StringDetails_utf32;

//..............................................................................

} // namespace sl
} // namespace axl
