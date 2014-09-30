// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_STRING_H

#include "axl_ref_Buf.h"
#include "axl_rtl_BitIdx.h"
#include "axl_rtl_Utf.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename T>
class CStringDetailsT
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CStringDetailsT <utf8_t>
{
public:
	typedef utf8_t  C;
	typedef utf16_t C2;
	typedef utf32_t C3;

	typedef CUtf8 CEncoding;
	typedef CStringDetailsT <C2> CDetails2;
	typedef CStringDetailsT <C3> CDetails3;

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
		return strlen (p);
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
class CStringDetailsImplT
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
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CStringDetailsT <utf16_t>: public CStringDetailsImplT <utf16_t>
{
public:
	typedef utf16_t C;
	typedef utf8_t  C2;
	typedef utf32_t C3;

	typedef CUtf16 CEncoding;
	typedef CStringDetailsT <C2> CDetails2;
	typedef CStringDetailsT <C3> CDetails3;

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
class CStringDetailsT <utf32_t>: public CStringDetailsImplT <utf32_t>
{
public:
	typedef utf32_t C;
	typedef utf8_t  C2;
	typedef utf16_t C3;

	typedef CUtf32 CEncoding;
	typedef CStringDetailsT <C2> CDetails2;
	typedef CStringDetailsT <C3> CDetails3;

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

typedef CStringDetailsT <char>    CStringDetails;
typedef CStringDetailsT <wchar_t> CStringDetails_w;
typedef CStringDetailsT <utf8_t>  CStringDetails_utf8;
typedef CStringDetailsT <utf16_t> CStringDetails_utf16;
typedef CStringDetailsT <utf32_t> CStringDetails_utf32;

//.............................................................................

enum EString
{
	EString_Empty = 0
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CStringT
{
public:
	class CHdr: public ref::CRefCount
	{
	public:
		size_t m_maxLength;
		size_t m_length;
	};

	typedef CStringDetailsT <T> CDetails;
	typedef typename CDetails::CDetails2 CDetails2;
	typedef typename CDetails::CDetails3 CDetails3;

	typedef typename CDetails::C C;
	typedef typename CDetails::C2 C2;
	typedef typename CDetails::C3 C3;

	typedef typename CDetails::CEncoding CEncoding;
	typedef typename CDetails2::CEncoding CEncoding2;
	typedef typename CDetails3::CEncoding CEncoding3;

	typedef CStringT <C2> CString2;
	typedef CStringT <C3> CString3;

protected:
	C* m_p;

public:
	CStringT ()
	{
		m_p = NULL;
	}

	CStringT (EString stringKind)
	{
		ASSERT (stringKind == EString_Empty);
		m_p = NULL;
	}

	CStringT (const CStringT& src)
	{
		m_p = NULL;
		copy (src);
	}

	CStringT (const CString2& src)
	{
		m_p = NULL;
		copy (src);
	}

	CStringT (const CString3& src)
	{
		m_p = NULL;
		copy (src);
	}

	CStringT (
		const C* p,
		size_t length = -1
		)
	{
		m_p = NULL;
		copy (p, length);
	}

	CStringT (
		const C2* p,
		size_t length = -1
		)
	{
		m_p = NULL;
		copy (p, length);
	}

	CStringT (
		const C3* p,
		size_t length = -1
		)
	{
		m_p = NULL;
		copy (p, length);
	}

	CStringT (
		utf32_t x,
		size_t count = 1
		)
	{
		m_p = NULL;
		copy (x, count);
	}

	CStringT (
		ref::EBuf kind,
		void* p,
		size_t size
		)
	{
		m_p = NULL;
		setBuffer (kind, p, size);
	}

	~CStringT ()
	{
		release ();
	}

	operator const C* () const
	{
		return cc ();
	}

	bool
	operator == (const CStringT& string) const
	{
		return cmp (string) == 0;
	}

	bool
	operator == (const C* p) const
	{
		return cmp (p) == 0;
	}

	bool
	operator != (const CStringT& string) const
	{
		return cmp (string) != 0;
	}

	bool
	operator != (const C* p) const
	{
		return cmp (p) != 0;
	}

	CStringT&
	operator = (EString stringKind)
	{
		ASSERT (stringKind == EString_Empty);
		clear ();
		return *this;
	}

	CStringT&
	operator = (const CStringT& src)
	{
		copy (src);
		return *this;
	}

	CStringT&
	operator = (const CString2& src)
	{
		copy (src);
		return *this;
	}

	CStringT&
	operator = (const CString3& src)
	{
		copy (src);
		return *this;
	}

	CStringT&
	operator = (const C* p)
	{
		copy (p, -1);
		return *this;
	}

	CStringT&
	operator = (const C2* p)
	{
		copy (p, -1);
		return *this;
	}

	CStringT&
	operator = (const C3* p)
	{
		copy (p, -1);
		return *this;
	}

	CStringT&
	operator = (utf32_t x)
	{
		copy (x, 1);
		return *this;
	}

	CStringT&
	operator += (const C* p)
	{
		append (p, -1);
		return *this;
	}

	CStringT&
	operator += (const C2* p)
	{
		append (p, -1);
		return *this;
	}

	CStringT&
	operator += (const C3* p)
	{
		append (p, -1);
		return *this;
	}

	CStringT&
	operator += (utf32_t x)
	{
		append (x, 1);
		return *this;
	}

	CStringT
	operator + (const CStringT& string) const
	{
		CStringT result = *this;
		result.append (string);
		return result;
	}

	CStringT
	operator + (const C* p) const
	{
		CStringT result = *this;
		result.append (p);
		return result;
	}

	CStringT
	operator + (const C2* p) const
	{
		CStringT result = *this;
		result.append (p);
		return result;
	}

	CStringT
	operator + (const C3* p) const
	{
		CStringT result = *this;
		result.append (p);
		return result;
	}

	CStringT
	operator + (utf32_t x) const
	{
		CStringT result = *this;
		result.append (x);
		return result;
	}

	CStringT
	operator + (const CString2& string) const
	{
		CStringT result = *this;
		result.append (string);
		return result;
	}

	CStringT
	operator + (const CString3& string) const
	{
		CStringT result = *this;
		result.append (string);
		return result;
	}

	const C*
	cc () const
	{
		return m_p ? m_p : CDetails::getEmptyString ();
	}

	CString2
	cc2 () const
	{
		return CString2 (m_p, getLength ());
	}

	CString3
	cc3 () const
	{
		return CString3 (m_p, getLength ());
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
			length = CDetails::calcLength (p);

		int result = CDetails::cmp (m_p, p, AXL_MIN (length, thisLength));
		return result ? result : rtl::CCmpT <size_t> () (thisLength, length);
	}

	int
	cmp (const CStringT& string) const
	{
		return cmp (string, string.getLength ());
	}

	size_t
	forceCopy (const CStringT& src)
	{
		return copy (src, src.getLength ());
	}

	size_t
	copy (const CStringT& src)
	{
		if (m_p == src.m_p)
			return getLength ();

		if (src.m_p)
		{
			if (src.getHdr ()->getFree () == (mem::FFree*) -1)
				return copy (src, src.getLength ());

			src.getHdr ()->addRef ();
		}

		if (m_p)
			getHdr ()->release ();

		m_p = src.m_p;
		return getLength ();
	}

	size_t
	copy (const CString2& src)
	{
		return copy (src, src.getLength ());
	}

	size_t
	copy (const CString3& src)
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
			length = CDetails::calcLength (p);

		if (!setLength (length, false))
			return -1;

		if (p != m_p)
			CDetails::copy (m_p, p, length);

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
			length = CDetails2::calcLength (p);

		size_t newLength = CUtfConvertT <CEncoding, CEncoding2>::calcRequiredLength (p, length);
		if (newLength == -1)
			return -1;

		if (!setLength (newLength, false))
			return -1;

		CUtfConvertT <CEncoding, CEncoding2>::convert (m_p, newLength, p, length);
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
			length = CDetails3::calcLength (p);

		size_t newLength = CUtfConvertT <CEncoding, CEncoding3>::calcRequiredLength (p, length);
		if (newLength == -1)
			return -1;

		if (!setLength (newLength, false))
			return -1;

		CUtfConvertT <CEncoding, CEncoding3>::convert (m_p, newLength, p, length);
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

		size_t codePointLength = CEncoding::getEncodeCodePointLength (x);
		if (codePointLength == -1)
			return -1;

		ASSERT (codePointLength <= 4);

		if (!setLength (count * codePointLength, false))
			return -1;

		C pattern [sizeof (utf32_t) / sizeof (C)];
		CEncoding::encodeCodePoint (pattern, x);
		fillWithPattern (m_p, pattern, codePointLength, count);
		return count;
	}


	size_t
	append (const CStringT& src)
	{
		return isEmpty () ? copy (src) : append (src, src.getLength ());
	}

	size_t
	append (const CString2& src)
	{
		return append (src, src.getLength ());
	}

	size_t
	append (const CString3& src)
	{
		return append (src, src.getLength ());
	}

	size_t
	append (
		const C* p,
		size_t length = -1
		)
	{
		size_t oldLength = getLength ();

		if (!p)
			return oldLength;

		if (length == -1)
			length = CDetails::calcLength (p);

		if (length == 0)
			return oldLength;

		size_t newLength = oldLength + length;
		if (!setLength (newLength, true))
			return -1;

		CDetails::copy (m_p + oldLength, p, length);
		return newLength;
	}

	size_t
	append (
		const C2* p,
		size_t length = -1
		)
	{
		size_t oldLength = getLength ();

		if (!p)
			return oldLength;

		if (length == -1)
			length = CDetails2::calcLength (p);

		if (length == 0)
			return oldLength;

		size_t appendLength = CUtfConvertT <CEncoding, CEncoding2>::calcRequiredLength (p, length);
		if (appendLength == -1)
			return -1;

		size_t newLength = oldLength + appendLength;
		if (!setLength (newLength, true))
			return -1;

		CUtfConvertT <CEncoding, CEncoding2>::convert (m_p + oldLength, appendLength, p, length);
		return newLength;
	}

	size_t
	append (
		const C3* p,
		size_t length = -1
		)
	{
		size_t oldLength = getLength ();

		if (!p)
			return oldLength;

		if (length == -1)
			length = CDetails3::calcLength (p);

		if (length == 0)
			return oldLength;

		size_t appendLength = CUtfConvertT <CEncoding, CEncoding3>::calcRequiredLength (p, length);
		if (appendLength == -1)
			return -1;

		size_t newLength = oldLength + appendLength;
		if (!setLength (newLength, true))
			return -1;

		CUtfConvertT <CEncoding, CEncoding3>::convert (m_p + oldLength, appendLength, p, length);
		return newLength;
	}

	size_t
	append (
		utf32_t x,
		size_t count = 1
		)
	{
		size_t oldLength = getLength ();

		if (count == 0)
			return oldLength;

		size_t codePointLength = CEncoding::getEncodeCodePointLength (x);
		if (codePointLength == -1)
			return -1;

		ASSERT (codePointLength <= 4);

		size_t newLength = oldLength + count * codePointLength;
		if (!setLength (newLength, true))
			return -1;

		C pattern [sizeof (utf32_t) / sizeof (C)];
		CEncoding::encodeCodePoint (pattern, x);
		fillWithPattern (m_p + oldLength, pattern, codePointLength, count);
		return newLength;
	}

	size_t
	appendNewLine ()
	{
#if (_AXL_ENV == AXL_ENV_WIN)
		return append (CDetails::getCrLf (), 2);
#else
		return append ('\n');
#endif
	}

	size_t
	format_va (
		const C* formatString,
		axl_va_list va
		)
	{
		size_t length = CDetails::calcFormatLength_va (formatString, va);
		if (!setLength (length, false))
			return -1;

		CDetails::format_va (m_p, length + 1, formatString, va);
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
		size_t appendLength = CDetails::calcFormatLength_va (formatString, va);
		size_t oldLength = getLength ();
		size_t newLength = oldLength + appendLength;
		if (!setLength (newLength, true))
			return -1;

		CDetails::format_va (m_p + oldLength, appendLength + 1, formatString, va);
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
	CStringT
	format_sva (
		const C* formatString,
		axl_va_list va
		)
	{
		CStringT result;
		result.format_va (formatString, va);
		return result;
	}

	static
	CStringT
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

		CHdr* hdr = getHdr ();
		hdr->m_length = CDetails::calcLength (m_p);
		return hdr->m_length;
	}

	bool
	ensureExclusive ()
	{
		return m_p ? setLength (getLength (), true) : true;
	}

	void
	setBuffer (
		ref::EBuf kind,
		void* p,
		size_t size
		)
	{
		ASSERT (size >= sizeof (CHdr) + sizeof (C));

		CHdr* oldHdr = getHdr ();

		mem::FFree* pfFree = kind == ref::EBuf_Static ? NULL : (mem::FFree*) -1;
		ref::CPtrT <CHdr> newHdr = AXL_REF_NEW_INPLACE (CHdr, p, pfFree);
		newHdr->m_length = 0;
		newHdr->m_maxLength = (size - sizeof (CHdr)) / sizeof (C) - 1;

		if (oldHdr)
			oldHdr->release ();

		m_p = (T*) (newHdr + 1);
		m_p [0] = 0;

		newHdr.detach ();
	}

protected:
	CHdr*
	getHdr () const
	{
		return m_p ? (CHdr*) m_p - 1 : NULL;
	}

	bool
	setLength (
		size_t length,
		bool saveContents
		)
	{
		CHdr* oldHdr = getHdr ();

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

		size_t maxLength = rtl::getMinPower2Gt (length) - 1;
		ASSERT (maxLength >= length);

		size_t size = (maxLength + 1) * sizeof (C);
		ref::CPtrT <CHdr> newHdr = AXL_REF_NEW_EXTRA (CHdr, size);
		if (!newHdr)
			return false;

		newHdr->m_length = length;
		newHdr->m_maxLength = maxLength;

		C* p = (C*) (newHdr + 1);
		p [0] = p [length] = 0;

		if (saveContents && m_p)
		{
			size_t copyLength = AXL_MIN (length, oldHdr->m_length);
			CDetails::copy (p, m_p, copyLength + 1);
		}

		if (oldHdr)
			oldHdr->release ();

		m_p = p;

		newHdr.detach ();
		return true;
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
			CDetails::fill (p, *pattern, count);
			return;
		}

		C* end = p + count * patternLength;

		for (; p < end; p += patternLength)
			CDetails::copy (p, pattern, patternLength);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CStringT <char>    CString;
typedef CStringT <wchar_t> CString_w;
typedef CStringT <utf8_t>  CString_utf8;
typedef CStringT <utf16_t> CString_utf16;
typedef CStringT <utf32_t> CString_utf32;

//.............................................................................

inline
CString
formatString_va (
	const char* formatString,
	axl_va_list va
	)
{
	return CString::format_sva (formatString, va);
}

inline
CString
formatString (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return CString::format_sva (formatString, va);
}

//.............................................................................

} // namespace rtl
} // namespace axl

