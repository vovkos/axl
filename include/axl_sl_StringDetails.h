// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_STRING_H

#include "axl_enc_Utf.h"

namespace axl {
namespace sl {

//.............................................................................

template <typename T>
class StringDetailsBase
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class StringDetailsBase <utf8_t>
{
public:
	typedef utf8_t  C;
	typedef utf16_t C2;
	typedef utf32_t C3;

	typedef enc::Utf8 Encoding;
	typedef StringDetailsBase <C2> Details2;
	typedef StringDetailsBase <C3> Details3;

	static
	const C*
	getEmptyString ()
	{
		return "";
	}

	static
	const C*
	getCrLf ()
	{
		return "\r\n";
	}

	static
	size_t
	calcLength (const C* p)
	{
		return p ? strlen (p) : 0;
	}

	static
	size_t
	calcLength (
		const C* p,
		size_t maxLength
		)
	{
		return p ? strnlen (p, maxLength) : 0;
	}

	static
	int
	cmp (
		const C* p1,
		const C* p2,
		size_t length
		)
	{
		return memcmp (p1, p2, length);
	}

	static
	void
	fill (
		C* p,
		C c,
		size_t count
		)
	{
		memset (p, c, count);
	}

	static
	void
	copy (
		C* dst,
		const C* src,
		size_t count
		)
	{
		memcpy (dst, src, count);
	}

	static
	void
	move (
		C* dst,
		const C* src,
		size_t count
		)
	{
		memmove (dst, src, count);
	}

#if (_AXL_CPP == AXL_CPP_MSC)
	static
	size_t
	calcFormatLength_va (
		const C* formatString,
		axl_va_list va
		)
	{
		return _vscprintf (formatString, va.m_va);
	}

	static
	void
	format_va (
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		_vsnprintf (buffer, bufferLength, formatString, va.m_va);
		if (bufferLength)
			buffer [bufferLength - 1] = 0; // not really necessary, just to make it identical to C++11 vsnprintf
	}
#else
	static
	size_t
	calcFormatLength_va (
		const C* formatString,
		axl_va_list va
		)
	{
		return vsnprintf (NULL, 0, formatString, va.m_va);
	}

	static
	size_t
	format_va (
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		return vsnprintf (buffer, bufferLength, formatString, va.m_va);
	}
#endif
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class StringDetailsImpl
{
public:
	static
	const T*
	getEmptyString ()
	{
		static T emptyString [1] = { 0 };
		return emptyString;
	}

	static
	const T*
	getCrLf ()
	{
		static T crLf [2] = { '\r', '\n' };
		return crLf;
	}

	static
	size_t
	calcLength (const T* p)
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
	calcLength (
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
	cmp (
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
	void
	fill (
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
	copy (
		T* dst,
		const T* src,
		size_t count
		)
	{
		memcpy (dst, src, count * sizeof (T));
	}

	static
	void
	move (
		T* dst,
		const T* src,
		size_t count
		)
	{
		memmove (dst, src, count * sizeof (T));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class StringDetailsBase <utf16_t>: public StringDetailsImpl <utf16_t>
{
public:
	typedef utf16_t C;
	typedef utf8_t  C2;
	typedef utf32_t C3;

	typedef enc::Utf16 Encoding;
	typedef StringDetailsBase <C2> Details2;
	typedef StringDetailsBase <C3> Details3;

#if (_AXL_CPP == AXL_CPP_MSC)
	static
	size_t
	calcFormatLength_va (
		const C* formatString,
		axl_va_list va
		)
	{
		return _vscwprintf (formatString, va.m_va);
	}

	static
	void
	format_va (
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		_vsnwprintf (buffer, bufferLength, formatString, va.m_va);
		if (bufferLength)
			buffer [bufferLength - 1] = 0; // not really necessary, just to make it identical to C++11 vsnprintf
	}
#else
	static
	size_t
	calcFormatLength_va (
		const C* formatString,
		axl_va_list va
		)
	{
		ASSERT (false);
		return 0;
	}

	static
	void
	format_va (
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		ASSERT (false);
	}
#endif
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class StringDetailsBase <utf32_t>: public StringDetailsImpl <utf32_t>
{
public:
	typedef utf32_t C;
	typedef utf8_t  C2;
	typedef utf16_t C3;

	typedef enc::Utf32 Encoding;
	typedef StringDetailsBase <C2> Details2;
	typedef StringDetailsBase <C3> Details3;

#if (_AXL_CPP == AXL_CPP_MSC)
	static
	size_t
	calcFormatLength_va (
		const C* formatString,
		axl_va_list va
		)
	{
		ASSERT (false);
		return 0;
	}

	static
	void
	format_va (
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		ASSERT (false);
	}
#else
	static
	size_t
	calcFormatLength_va (
		const C* formatString,
		axl_va_list va
		)
	{
		return vswprintf (NULL, 0, formatString, va.m_va);
	}

	static
	void
	format_va (
		C* buffer,
		size_t bufferLength,
		const C* formatString,
		axl_va_list va
		)
	{
		vswprintf (buffer, bufferLength, formatString, va.m_va);
	}
#endif
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef StringDetailsBase <char>    StringDetails;
typedef StringDetailsBase <wchar_t> StringDetails_w;
typedef StringDetailsBase <utf8_t>  StringDetails_utf8;
typedef StringDetailsBase <utf16_t> StringDetails_utf16;
typedef StringDetailsBase <utf32_t> StringDetails_utf32;

//.............................................................................

} // namespace sl
} // namespace axl
