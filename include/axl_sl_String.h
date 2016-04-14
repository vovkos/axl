// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_STRING_H

#include "axl_ref_Buf.h"
#include "axl_sl_BitIdx.h"
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
		return strlen (p); // no NULL-check
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
		const T* p0 = p;
		while (*p)
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

enum StringKind
{
	StringKind_Empty = 0
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class StringBase
{
public:
	class Hdr: public ref::RefCount
	{
	public:
		size_t m_maxLength;
		size_t m_length;
	};

	typedef StringDetailsBase <T> Details;
	typedef typename Details::Details2 Details2;
	typedef typename Details::Details3 Details3;

	typedef typename Details::C C;
	typedef typename Details::C2 C2;
	typedef typename Details::C3 C3;

	typedef typename Details::Encoding Encoding;
	typedef typename Details2::Encoding Encoding2;
	typedef typename Details3::Encoding Encoding3;

	typedef StringBase <C2> String2;
	typedef StringBase <C3> String3;

protected:
	C* m_p;

public:
	StringBase ()
	{
		m_p = NULL;
	}

	StringBase (StringKind stringKind)
	{
		ASSERT (stringKind == StringKind_Empty);
		m_p = NULL;
	}

	StringBase (const StringBase& src)
	{
		m_p = NULL;
		copy (src);
	}

	StringBase (const String2& src)
	{
		m_p = NULL;
		copy (src);
	}

	StringBase (const String3& src)
	{
		m_p = NULL;
		copy (src);
	}

	StringBase (
		const C* p,
		size_t length = -1
		)
	{
		m_p = NULL;
		copy (p, length);
	}

	StringBase (
		const C2* p,
		size_t length = -1
		)
	{
		m_p = NULL;
		copy (p, length);
	}

	StringBase (
		const C3* p,
		size_t length = -1
		)
	{
		m_p = NULL;
		copy (p, length);
	}

	StringBase (
		utf32_t x,
		size_t count = 1
		)
	{
		m_p = NULL;
		copy (x, count);
	}

	StringBase (
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		m_p = NULL;
		setBuffer (kind, p, size);
	}

	~StringBase ()
	{
		release ();
	}

	operator const C* () const
	{
		return cc ();
	}

	bool
	operator == (const StringBase& string) const
	{
		return cmp (string) == 0;
	}

	bool
	operator == (const C* p) const
	{
		return cmp (p) == 0;
	}

	bool
	operator != (const StringBase& string) const
	{
		return cmp (string) != 0;
	}

	bool
	operator != (const C* p) const
	{
		return cmp (p) != 0;
	}

	StringBase&
	operator = (StringKind stringKind)
	{
		ASSERT (stringKind == StringKind_Empty);
		clear ();
		return *this;
	}

	StringBase&
	operator = (const StringBase& src)
	{
		copy (src);
		return *this;
	}

	StringBase&
	operator = (const String2& src)
	{
		copy (src);
		return *this;
	}

	StringBase&
	operator = (const String3& src)
	{
		copy (src);
		return *this;
	}

	StringBase&
	operator = (const C* p)
	{
		copy (p, -1);
		return *this;
	}

	StringBase&
	operator = (const C2* p)
	{
		copy (p, -1);
		return *this;
	}

	StringBase&
	operator = (const C3* p)
	{
		copy (p, -1);
		return *this;
	}

	StringBase&
	operator = (utf32_t x)
	{
		copy (x, 1);
		return *this;
	}

	StringBase&
	operator += (const C* p)
	{
		append (p, -1);
		return *this;
	}

	StringBase&
	operator += (const C2* p)
	{
		append (p, -1);
		return *this;
	}

	StringBase&
	operator += (const C3* p)
	{
		append (p, -1);
		return *this;
	}

	StringBase&
	operator += (utf32_t x)
	{
		append (x, 1);
		return *this;
	}

	StringBase
	operator + (const StringBase& string) const
	{
		StringBase result = *this;
		result.append (string);
		return result;
	}

	StringBase
	operator + (const C* p) const
	{
		StringBase result = *this;
		result.append (p);
		return result;
	}

	StringBase
	operator + (const C2* p) const
	{
		StringBase result = *this;
		result.append (p);
		return result;
	}

	StringBase
	operator + (const C3* p) const
	{
		StringBase result = *this;
		result.append (p);
		return result;
	}

	StringBase
	operator + (utf32_t x) const
	{
		StringBase result = *this;
		result.append (x);
		return result;
	}

	StringBase
	operator + (const String2& string) const
	{
		StringBase result = *this;
		result.append (string);
		return result;
	}

	StringBase
	operator + (const String3& string) const
	{
		StringBase result = *this;
		result.append (string);
		return result;
	}

	const C*
	cc () const
	{
		return m_p ? m_p : Details::getEmptyString ();
	}

	String2
	s2 () const
	{
		return String2 (m_p, getLength ());
	}

	String3
	s3 () const
	{
		return String3 (m_p, getLength ());
	}

	size_t
	getLength () const
	{
		return m_p ? getHdr ()->m_length : 0;
	}

	size_t
	getBufferLength () const
	{
		return m_p ? getHdr ()->m_maxLength : 0;
	}

	bool
	isEmpty () const
	{
		return getLength () == 0;
	}

	void
	release ()
	{
		if (!m_p)
			return;

		getHdr ()->release ();
		m_p = NULL;
	}

	void
	clear ()
	{
		setLength (0, false);
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
	cmp (const StringBase& string) const
	{
		return cmp (string, string.getLength ());
	}

	size_t
	forceCopy (const StringBase& src)
	{
		return copy (src, src.getLength ());
	}

	size_t
	copy (const StringBase& src)
	{
		if (m_p == src.m_p)
			return getLength ();

		if (src.m_p)
		{
			if (src.getHdr ()->getFlags () & ref::BufHdrFlag_Exclusive)
				return copy (src, src.getLength ());

			src.getHdr ()->addRef ();
		}

		if (m_p)
			getHdr ()->release ();

		m_p = src.m_p;
		return getLength ();
	}

	size_t
	copy (const String2& src)
	{
		return copy (src, src.getLength ());
	}

	size_t
	copy (const String3& src)
	{
		return copy (src, src.getLength ());
	}

	size_t
	copy (
		const C* p,
		size_t length = -1
		)
	{
		if (p == m_p && length == -1)
			return getLength ();

		if (!p)
		{
			clear ();
			return 0;
		}

		if (length == -1)
			length = Details::calcLength (p);

		if (!setLength (length, false))
			return -1;

		if (p != m_p)
			Details::copy (m_p, p, length);

		return length;
	}

	size_t
	copy (
		const C2* p,
		size_t length = -1
		)
	{
		if (!p)
		{
			clear ();
			return 0;
		}

		if (length == -1)
			length = Details2::calcLength (p);

		size_t newLength = enc::UtfConvert <Encoding, Encoding2>::calcRequiredLength (p, length);
		if (newLength == -1)
			return -1;

		if (!setLength (newLength, false))
			return -1;

		enc::UtfConvert <Encoding, Encoding2>::convert (m_p, newLength, p, length);
		return length;
	}

	size_t
	copy (
		const C3* p,
		size_t length = -1
		)
	{
		if (!p)
		{
			clear ();
			return 0;
		}

		if (length == -1)
			length = Details3::calcLength (p);

		size_t newLength = enc::UtfConvert <Encoding, Encoding3>::calcRequiredLength (p, length);
		if (newLength == -1)
			return -1;

		if (!setLength (newLength, false))
			return -1;

		enc::UtfConvert <Encoding, Encoding3>::convert (m_p, newLength, p, length);
		return length;
	}

	size_t
	copy (
		utf32_t x,
		size_t count = 1
		)
	{
		if (!count)
		{
			clear ();
			return 0;
		}

		size_t codePointLength = Encoding::getEncodeCodePointLength (x);
		if (codePointLength == -1)
			return -1;

		ASSERT (codePointLength <= 4);

		if (!setLength (count * codePointLength, false))
			return -1;

		C pattern [sizeof (utf32_t) / sizeof (C)];
		Encoding::encodeCodePoint (pattern, x);
		fillWithPattern (m_p, pattern, codePointLength, count);
		return count;
	}

	size_t
	append (const StringBase& src)
	{
		return insert (-1, src);
	}

	size_t
	append (const String2& src)
	{
		return insert (-1, src);
	}

	size_t
	append (const String3& src)
	{
		return insert (-1, src);
	}

	size_t
	append (
		const C* p,
		size_t length = -1
		)
	{
		return insert (-1, p, length);
	}

	size_t
	append (
		const C2* p,
		size_t length = -1
		)
	{
		return insert (-1, p, length);
	}

	size_t
	append (
		const C3* p,
		size_t length = -1
		)
	{
		return insert (-1, p, length);
	}

	size_t
	append (
		utf32_t x,
		size_t count = 1
		)
	{
		return insert (-1, x, count);
	}

	size_t
	appendNewLine ()
	{
		return insertNewLine (-1);
	}

	size_t
	insert (
		size_t index,
		const StringBase& src
		)
	{
		return isEmpty () ? copy (src) : insert (index, src, src.getLength ());
	}

	size_t
	insert (
		size_t index,
		const String2& src
		)
	{
		return insert (index, src, src.getLength ());
	}

	size_t
	insert (
		size_t index,
		const String3& src		
		)
	{
		return insert (index, src, src.getLength ());
	}

	size_t
	insert (
		size_t index,
		const C* p,
		size_t length = -1
		)
	{
		size_t oldLength = getLength ();

		if (!p)
			return oldLength;

		if (length == -1)
			length = Details::calcLength (p);

		if (length == 0)
			return oldLength;

		C* dst = insertSpace (index, length);
		if (!dst)
			return -1;

		Details::copy (dst, p, length);
		return oldLength + length;
	}

	size_t
	insert (
		size_t index,
		const C2* p,
		size_t length = -1
		)
	{
		size_t oldLength = getLength ();

		if (!p)
			return oldLength;

		if (length == -1)
			length = Details2::calcLength (p);

		if (length == 0)
			return oldLength;

		size_t insertLength = enc::UtfConvert <Encoding, Encoding2>::calcRequiredLength (p, length);
		if (insertLength == -1)
			return -1;

		C* dst = insertSpace (index, insertLength);
		if (!dst)
			return -1;

		enc::UtfConvert <Encoding, Encoding2>::convert (dst, insertLength, p, length);
		return oldLength + insertLength;
	}

	size_t
	insert (
		size_t index,
		const C3* p,
		size_t length = -1
		)
	{
		size_t oldLength = getLength ();

		if (!p)
			return oldLength;

		if (length == -1)
			length = Details3::calcLength (p);

		if (length == 0)
			return oldLength;

		size_t insertLength = enc::UtfConvert <Encoding, Encoding3>::calcRequiredLength (p, length);
		if (insertLength == -1)
			return -1;

		C* dst = insertSpace (index, insertLength);
		if (!dst)
			return -1;

		enc::UtfConvert <Encoding, Encoding3>::convert (dst, insertLength, p, length);
		return oldLength + insertLength;
	}

	size_t
	insert (
		size_t index,
		utf32_t x,
		size_t count = 1
		)
	{
		size_t oldLength = getLength ();

		if (count == 0)
			return oldLength;

		size_t codePointLength = Encoding::getEncodeCodePointLength (x);
		if (codePointLength == -1)
			return -1;

		ASSERT (codePointLength <= 4);

		size_t insertLength = count * codePointLength;

		C* dst = insertSpace (index, count * codePointLength);
		if (!dst)
			return -1;

		C pattern [sizeof (utf32_t) / sizeof (C)];
		Encoding::encodeCodePoint (pattern, x);
		fillWithPattern (dst, pattern, codePointLength, count);
		return oldLength + insertLength;
	}

	size_t
	insertNewLine (size_t index)
	{
#if (_AXL_ENV == AXL_ENV_WIN)
		return insert (index, Details::getCrLf (), 2);
#else
		return insert (index, '\n');
#endif
	}

	size_t
	format_va (
		const C* formatString,
		axl_va_list va
		)
	{
		size_t length = Details::calcFormatLength_va (formatString, va);
		if (!setLength (length, false))
			return -1;

		Details::format_va (m_p, length + 1, formatString, va);
		return length;
	}

	size_t
	format (
		const C* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return format_va (formatString, va);
	}

	size_t
	appendFormat_va (
		const C* formatString,
		axl_va_list va
		)
	{
		size_t appendLength = Details::calcFormatLength_va (formatString, va);
		size_t oldLength = getLength ();
		size_t newLength = oldLength + appendLength;
		if (!setLength (newLength, true))
			return -1;

		Details::format_va (m_p + oldLength, appendLength + 1, formatString, va);
		return newLength;
	}

	size_t
	appendFormat (
		const C* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return appendFormat_va (formatString, va);
	}

	static
	StringBase
	format_sva (
		const C* formatString,
		axl_va_list va
		)
	{
		StringBase result;
		result.format_va (formatString, va);
		return result;
	}

	static
	StringBase
	format_s (
		const C* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return format_sva (formatString, va);
	}

	size_t
	reduceLength (size_t delta)
	{
		size_t length = getLength ();
		if (length <= delta)
		{
			clear ();
			return 0;
		}

		length -= delta;
		setLength (length, true);
		return length;
	}

	void
	setReducedLength (size_t length)
	{
		size_t oldLength = getLength ();
		if (length < oldLength)
			setLength (length, true);
	}

	C*
	getBuffer ()
	{
		return ensureExclusive () ? m_p : NULL;
	}

	C*
	getBuffer (
		size_t length,
		bool saveContents = false
		)
	{
		return setLength (length, saveContents) ? m_p : NULL;
	}

	C*
	reserve (
		size_t length,
		bool saveContents = false
		)
	{
		size_t oldLength = getLength ();

		if (!setLength (length, saveContents))
			return NULL;

		setLength (oldLength, saveContents);
		return m_p;
	}

	size_t
	updateLength ()
	{
		if (!m_p)
			return 0;

		Hdr* hdr = getHdr ();
		hdr->m_length = Details::calcLength (m_p);
		return hdr->m_length;
	}

	bool
	ensureExclusive ()
	{
		return m_p ? setLength (getLength (), true) : true;
	}

	void
	setBuffer (
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		ASSERT (size >= sizeof (Hdr) + sizeof (C));

		Hdr* oldHdr = getHdr ();

		uint_t flags = kind != ref::BufKind_Static ? ref::BufHdrFlag_Exclusive : 0;
		ref::Ptr <Hdr> newHdr = AXL_REF_NEW_INPLACE (Hdr, p, flags);
		newHdr->m_length = 0;
		newHdr->m_maxLength = (size - sizeof (Hdr)) / sizeof (C) - 1;

		if (oldHdr)
			oldHdr->release ();

		m_p = (T*) (newHdr + 1);
		m_p [0] = 0;

		newHdr.detach ();
	}

protected:
	Hdr*
	getHdr () const
	{
		return m_p ? (Hdr*) m_p - 1 : NULL;
	}

	bool
	setLength (
		size_t length,
		bool saveContents
		)
	{
		Hdr* oldHdr = getHdr ();

		if (oldHdr &&
			oldHdr->m_maxLength >= length &&
			oldHdr->getRefCount () == 1)
		{
			oldHdr->m_length = length;
			m_p [length] = 0;
			return true;
		}

		if (length == 0)
		{
			release ();
			return true;
		}

		size_t maxLength = sl::getMinPower2Gt (length) - 1;
		ASSERT (maxLength >= length);

		size_t size = (maxLength + 1) * sizeof (C);
		ref::Ptr <Hdr> newHdr = AXL_REF_NEW_EXTRA (Hdr, size);
		if (!newHdr)
			return false;

		newHdr->m_length = length;
		newHdr->m_maxLength = maxLength;

		C* p = (C*) (newHdr + 1);
		p [0] = p [length] = 0;

		if (saveContents && m_p)
		{
			size_t copyLength = AXL_MIN (length, oldHdr->m_length);
			Details::copy (p, m_p, copyLength + 1);
		}

		if (oldHdr)
			oldHdr->release ();

		m_p = p;

		newHdr.detach ();
		return true;
	}

	C*
	insertSpace (
		size_t index,
		size_t length
		)
	{
		size_t oldLength = getLength ();
		bool result = setLength (oldLength + length, true);
		if (!result)
			return NULL;

		if (index > oldLength)
			index = oldLength;

		C* dst = m_p + index;

		if (length && index < oldLength)
			Details::move (dst + length, dst, oldLength - index);

		return dst;
	}

	static
	void
	fillWithPattern (
		C* p,
		const C* pattern,
		size_t patternLength,
		size_t count
		)
	{
		if (patternLength == 1)
		{
			Details::fill (p, *pattern, count);
			return;
		}

		C* end = p + count * patternLength;

		for (; p < end; p += patternLength)
			Details::copy (p, pattern, patternLength);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef StringBase <char>    String;
typedef StringBase <wchar_t> String_w;
typedef StringBase <utf8_t>  String_utf8;
typedef StringBase <utf16_t> String_utf16;
typedef StringBase <utf32_t> String_utf32;

//.............................................................................

inline
String
formatString_va (
	const char* formatString,
	axl_va_list va
	)
{
	return String::format_sva (formatString, va);
}

inline
String
formatString (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return String::format_sva (formatString, va);
}

//.............................................................................

} // namespace sl
} // namespace axl

