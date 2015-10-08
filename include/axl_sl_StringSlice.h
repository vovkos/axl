// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_STRINGSLICE_H

#include "axl_sl_String.h"

namespace axl {
namespace sl {

//.............................................................................

template <typename T>
class StringSliceBase
{
public:
	typedef StringDetailsBase <T> Details;
	typedef typename Details::C C;
	typedef typename Details::Encoding Encoding;
	typedef StringBase <C> String;

protected:
	const C* m_p;
	size_t m_length;

public:
	StringSliceBase ()
	{
		copy (NULL, 0);
	}

	StringSliceBase (
		const C* p,
		size_t length = -1
		)
	{
		copy (p, length);
	}

	StringSliceBase (const sl::String& string)
	{
		copy (string, string.getLength ());
	}

	operator const C* () const
	{
		return cc ();
	}

	bool
	operator == (const C* p) const
	{
		return cmp (p) == 0;
	}

	bool
	operator == (const String& string) const
	{
		return cmp (string) == 0;
	}

	bool
	operator == (const StringSliceBase& slice) const
	{
		return cmp (slice) == 0;
	}

	bool
	operator != (const C* p) const
	{
		return cmp (p) != 0;
	}

	bool
	operator != (const String& string) const
	{
		return cmp (string) != 0;
	}

	bool
	operator != (const StringSliceBase& slice) const
	{
		return cmp (slice) != 0;
	}

	StringSliceBase&
	operator = (const C* p)
	{
		copy (p);
		return *this;
	}

	StringSliceBase&
	operator = (const String& string)
	{
		copy (string);
		return *this;
	}

	const C*
	cc () const
	{
		return m_p ? m_p : Details::getEmptyString ();
	}

	size_t
	getLength () const
	{
		return m_length;
	}

	bool
	isEmpty () const
	{
		return m_length == 0;
	}

	void
	clear ()
	{
		copy (NULL, 0);
	}

	int
	cmp (
		const C* p,
		size_t length = -1
		) const
	{
		size_t thisLength = getLength ();

		if (!p)
			return thisLength ? 1 : 0;

		if (length == -1)
			length = Details::calcLength (p);

		int result = Details::cmp (m_p, p, AXL_MIN (length, thisLength));
		return result ? result : sl::Cmp <size_t> () (thisLength, length);
	}

	int
	cmp (const String& string) const
	{
		return cmp (string, string.getLength ());
	}

	int
	cmp (const StringSliceBase& slice) const
	{
		return cmp (slice.m_p, slice.m_length);
	}

	void
	copy (const sl::String& string)
	{
		copy (string, string.getLength ());
	}

	void
	copy (
		const C* p,
		size_t length = -1
		)
	{
		m_p = p;
		m_length = p ? length != -1 ? length : strlen (p) : 0;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef StringSliceBase <char>    StringSlice;
typedef StringSliceBase <wchar_t> StringSlice_w;
typedef StringSliceBase <utf8_t>  StringSlice_utf8;
typedef StringSliceBase <utf16_t> StringSlice_utf16;
typedef StringSliceBase <utf32_t> StringSlice_utf32;

//.............................................................................

} // namespace sl
} // namespace axl

