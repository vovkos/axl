// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_STRING_H

#include "axl_sl_StringDetails.h"
#include "axl_sl_Cmp.h"
#include "axl_ref_Buf.h"

namespace axl {
namespace sl {

template <typename T> class StringBase;

//.............................................................................

template <typename T>
class StringRefBase
{
	friend class StringBase <T>;

public:
	typedef StringDetailsBase <T> Details;
	typedef typename Details::Details2 Details2;
	typedef typename Details::Details3 Details3;

	typedef typename Details::C C;
	typedef typename Details::C2 C2;
	typedef typename Details::C3 C3;

	typedef typename Details::Encoding Encoding;
	typedef typename Details2::Encoding Encoding2;
	typedef typename Details3::Encoding Encoding3;

	typedef StringRefBase <C> StringRef;
	typedef StringRefBase <C2> StringRef2;
	typedef StringRefBase <C3> StringRef3;

	typedef StringBase <C> String;
	typedef StringBase <C2> String2;
	typedef StringBase <C3> String3;

protected:
	mutable C* m_p;
	mutable ref::BufHdr* m_hdr;

#if (AXL_PTR_BITS == 64)
	size_t m_length : 63;
#else
	size_t m_length : 31; // more than enough
#endif

	mutable size_t m_isNullTerminated : 1;

public:
	StringRefBase ()
	{
		initialize ();
	}

	StringRefBase (const StringRef& src)
	{
		initialize ();
		attach (src);
	}

	StringRefBase (const StringRef2& src)
	{
		initialize ();
		attach (String (src));
	}

	StringRefBase (const StringRef3& src)
	{
		initialize ();
		attach (String (src));
	}

	StringRefBase (
		const C* p,
		size_t length = -1
		)
	{
		initialize ();
		attach (p, NULL, length);
	}

	StringRefBase (
		const C2* p,
		size_t length = -1
		)
	{
		initialize ();
		attach (String (p, length));
	}

	StringRefBase (
		const C3* p,
		size_t length = -1
		)
	{
		initialize ();
		attach (String (p, length));
	}

	StringRefBase (utf32_t x)
	{
		initialize ();
		attach (String (x));
	}

	StringRefBase (
		utf32_t x,
		size_t count
		)
	{
		initialize ();
		attach (String (x, count));
	}

	~StringRefBase ()
	{
		release ();
	}

	operator const C* () const
	{
		return cc ();
	}

	StringRefBase&
	operator = (const StringRef& src)
	{
		attach (src);
		return *this;
	}

	bool
	operator == (const StringRef& string) const
	{
		return cmp (string) == 0;
	}

	bool
	operator == (const C* p) const
	{
		return cmp (p) == 0;
	}

	bool
	operator != (const StringRef& string) const
	{
		return cmp (string) != 0;
	}

	bool
	operator != (const C* p) const
	{
		return cmp (p) != 0;
	}

	StringRefBase
	operator + (const StringRef& string) const
	{
		String result = *this;
		result.append (string);
		return result;
	}

	StringRefBase
	operator + (const StringRef2& string) const
	{
		String result = *this;
		result.append (string);
		return result;
	}

	StringRefBase
	operator + (const StringRef3& string) const
	{
		String result = *this;
		result.append (string);
		return result;
	}

	StringRefBase
	operator + (const C* p) const
	{
		String result = *this;
		result.append (p);
		return result;
	}

	StringRefBase
	operator + (const C2* p) const
	{
		String result = *this;
		result.append (p);
		return result;
	}

	StringRefBase
	operator + (const C3* p) const
	{
		String result = *this;
		result.append (p);
		return result;
	}

	const C*
	cc () const
	{
		if (m_isNullTerminated)
			return m_p;

		if (!m_length)
			return Details::getEmptyString ();

		String string (m_p, m_length);
		ASSERT (string.m_length == m_length && string.m_isNullTerminated);

		if (m_hdr)
			m_hdr->release ();

		m_p = string.m_p;
		m_hdr = string.m_hdr;
		m_isNullTerminated = true;

		string.m_hdr = NULL;

		return m_p;
	}

	StringRef2
	s2 () const
	{
		return String2 (m_p, m_length);
	}

	StringRef3
	s3 () const
	{
		return String3 (m_p, m_length);
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

	bool
	isNullTerminated () const
	{
		return m_isNullTerminated;
	}

	void
	clear ()
	{
		release ();
	}

	void
	release ()
	{
		if (m_hdr)
			m_hdr->release ();

		initialize ();
	}

	int
	cmp (
		const C* p,
		size_t length = -1
		) const
	{
		if (length == -1)
			length = Details::calcLength (p);

		int result = Details::cmp (m_p, p, AXL_MIN (length, m_length));
		return result ? result : sl::Cmp <size_t> () (m_length, length);
	}

	int
	cmp (const StringRef& string) const
	{
		return cmp (string, string.m_length);
	}

	size_t 
	find (C c) const
	{
		return Details::find (m_p, m_length, c);
	}

	size_t 
	find (const StringRef& subString) const
	{
		return Details::find (m_p, m_length, subString.m_p, subString.m_length);
	}

	size_t 
	findOneOf (const StringRef& charSet) const
	{
		return Details::findOneOf (m_p, m_length, charSet.m_p, charSet.m_length);
	}

	size_t 
	findNotOneOf (const StringRef& charSet) const
	{
		return Details::findNotOneOf (m_p, m_length, charSet.m_p, charSet.m_length);
	}

	size_t 
	reverseFind (C c) const
	{
		return Details::reverseFind (m_p, m_length, c);
	}

	size_t 
	reverseFind (const StringRef& subString) const
	{
		return Details::reverseFind (m_p, m_length, subString.m_p, subString.m_length);
	}

	size_t 
	reverseFindOneOf (const StringRef& charSet) const
	{
		return Details::reverseFindOneOf (m_p, m_length, charSet.m_p, charSet.m_length);
	}

	size_t 
	reverseFindNotOneOf (const StringRef& charSet) const
	{
		return Details::reverseFindNotOneOf (m_p, m_length, charSet.m_p, charSet.m_length);
	}

	StringRef
	getLeftTrimmedString () const
	{
		String string = *this;
		string.trimLeft ();
		return string;
	}

	StringRef
	getRightTimmedString () const
	{
		String string = *this;
		string.trimRight ();
		return string;
	}

	StringRef
	getTrimmedString () const
	{
		String string = *this;
		string.trim ();
		return string;
	}

	StringRef
	getLowerCaseString ()
	{
		String string = *this;
		string.makeLowerCase ();
		return string;
	}

	StringRef
	getUpperCaseString ()
	{
		String string = *this;
		string.makeUpperCase ();
		return string;
	}

	StringRef
	getCaseFoldedString ()
	{
		String string = *this;
		string.makeCaseFolded ();
		return string;
	}

protected:
	void
	initialize ()
	{
		m_p = NULL;
		m_hdr = NULL;
		m_length = 0;
		m_isNullTerminated = false;
	}

	void
	attachBufHdr (ref::BufHdr* hdr) const
	{
		if (hdr == m_hdr)
			return; // try to avoid unnecessary interlocked ops

		if (hdr)
			hdr->addRef ();

		if (m_hdr)
			m_hdr->release ();
		
		m_hdr = hdr;
	}

	void
	attach (const StringRef& src)
	{
		if (&src == this)
			return;

		attachBufHdr (src.m_hdr);

		m_p = src.m_p;
		m_length = src.m_length;
		m_isNullTerminated = src.m_isNullTerminated;
	}

	void
	attach (
		const C* p,
		ref::BufHdr* hdr,
		size_t length
		)
	{
		attachBufHdr (hdr);

		m_p = const_cast <C*> (p);

		if (length == -1)
		{
			m_length = Details::calcLength (p);
			m_isNullTerminated = true;
		}
		else
		{
			m_length = length;
			m_isNullTerminated = false;
		}
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef StringRefBase <char>    StringRef;
typedef StringRefBase <wchar_t> StringRef_w;
typedef StringRefBase <utf8_t>  StringRef_utf8;
typedef StringRefBase <utf16_t> StringRef_utf16;
typedef StringRefBase <utf32_t> StringRef_utf32;

//.............................................................................

template <typename T>
class StringBase: public StringRefBase <T>
{
public:
	typedef StringRefBase <T> StringRef;

	typedef typename StringRef::Details Details;
	typedef typename StringRef::Details2 Details2;
	typedef typename StringRef::Details3 Details3;

	typedef typename StringRef::C C;
	typedef typename StringRef::C2 C2;
	typedef typename StringRef::C3 C3;

	typedef typename StringRef::Encoding Encoding;
	typedef typename StringRef::Encoding2 Encoding2;
	typedef typename StringRef::Encoding3 Encoding3;

	typedef typename StringRef::StringRef2 StringRef2;
	typedef typename StringRef::StringRef3 StringRef3;
	
public:
	StringBase ()
	{
	}

	StringBase (const StringBase& src)
	{
		copy (src);
	}

	StringBase (const StringRef& src)
	{
		copy (src);
	}

	StringBase (const StringRef2& src)
	{
		copy (src);
	}

	StringBase (const StringRef3& src)
	{
		copy (src);
	}

	StringBase (
		const C* p,
		size_t length = -1
		)
	{
		copy (p, length);
	}

	StringBase (
		const C2* p,
		size_t length = -1
		)
	{
		copy (p, length);
	}

	StringBase (
		const C3* p,
		size_t length = -1
		)
	{
		copy (p, length);
	}

	StringBase (utf32_t x)
	{
		copy (x);
	}

	StringBase (
		utf32_t x,
		size_t count
		)
	{
		copy (x, count);
	}

	StringBase (
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		setBuffer (kind, p, size);
	}

	StringBase&
	operator = (const StringBase& src)
	{
		copy (src);
		return *this;
	}

	StringBase&
	operator = (const StringRef& src)
	{
		copy (src);
		return *this;
	}

	StringBase&
	operator = (const StringRef2& src)
	{
		copy (src);
		return *this;
	}

	StringBase&
	operator = (const StringRef3& src)
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
		copy (x);
		return *this;
	}

	StringBase&
	operator += (const StringRef& string)
	{
		append (string);
		return *this;
	}

	StringBase&
	operator += (const StringRef2& string)
	{
		append (string);
		return *this;
	}

	StringBase&
	operator += (const StringRef3& string)
	{
		append (string);
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
		append (x);
		return *this;
	}

	void
	clear ()
	{
		if (!this->m_hdr)
		{
			ASSERT (!this->m_length);
			return;
		}

		if (this->m_hdr->getRefCount () != 1)
		{
			this->release ();
			return;
		}

		this->m_p = (C*) (this->m_hdr + 1);
		this->m_p [0] = 0;
		this->m_length = 0;
		this->m_isNullTerminated = true;
	}

	size_t
	forceCopy (const StringRef& src)
	{
		return copy (src, src.m_length);
	}

	size_t
	copy (const StringRef& src)
	{
		if (&src == this)
			return this->m_length;

		if (!src.m_hdr || src.m_hdr->getFlags () & ref::BufHdrFlag_Exclusive)
			return copy (src, src.m_length);

		this->attach (src);
		return this->m_length;
	}

	size_t
	copy (const StringRef2& src)
	{
		return copy (src, src.getLength ());
	}

	size_t
	copy (const StringRef3& src)
	{
		return copy (src, src.getLength ());
	}

	size_t
	copy (
		const C* p,
		size_t length = -1
		)
	{
		if (p == this->m_p && length == -1)
			return this->m_length;

		if (length == -1)
			length = Details::calcLength (p);

		if (!length)
		{
			clear ();
			return 0;
		}

		if (this->m_hdr && this->m_hdr->isInsideBuffer (p))
		{
			C* end = (C*) this->m_hdr->getBufferEnd ();
			ASSERT (p + length <= end);

			this->m_p = (C*) p;
			this->m_length = length;
			this->m_isNullTerminated = p + length < end && !p [length];
			return length;
		}

		if (!createBuffer (length, false))
			return -1;

		Details::copy (this->m_p, p, length);
		return length;
	}

	size_t
	copy (
		const C2* p,
		size_t length = -1
		)
	{
		if (length == -1)
			length = Details2::calcLength (p);

		if (!length)
		{
			clear ();
			return 0;
		}

		size_t newLength = enc::UtfConvert <Encoding, Encoding2>::calcRequiredLength (p, length);
		if (!createBuffer (newLength, false))
			return -1;

		enc::UtfConvert <Encoding, Encoding2>::convert (this->m_p, newLength, p, length);
		return newLength;
	}

	size_t
	copy (
		const C3* p,
		size_t length = -1
		)
	{
		if (length == -1)
			length = Details3::calcLength (p);

		if (!length)
		{
			clear ();
			return 0;
		}

		size_t newLength = enc::UtfConvert <Encoding, Encoding3>::calcRequiredLength (p, length);
		if (!createBuffer (newLength, false))
			return -1;

		enc::UtfConvert <Encoding, Encoding3>::convert (this->m_p, newLength, p, length);
		return newLength;
	}

	size_t
	copy (utf32_t x)
	{
		if (!x)
		{
			clear ();
			return 0;
		}

		return copy (x, 1);		
	}

	size_t
	copy (
		utf32_t x,
		size_t count
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
		
		size_t newLength = count * codePointLength;
		if (!createBuffer (newLength, false))
			return -1;

		C pattern [sizeof (utf32_t) / sizeof (C)];
		Encoding::encodeCodePoint (pattern, x);
		fillWithPattern (this->m_p, pattern, codePointLength, count);
		return newLength;
	}

	size_t
	append (const StringRef& src)
	{
		return insert (-1, src);
	}

	size_t
	append (const StringRef2& src)
	{
		return insert (-1, src);
	}

	size_t
	append (const StringRef3& src)
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
	append (utf32_t x)
	{
		return x ? append (x, 1) : this->m_length;
	}

	size_t
	append (
		utf32_t x,
		size_t count
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
		const StringRef& src
		)
	{
		return !this->m_length ? copy (src) : insert (index, src, src.m_length);
	}

	size_t
	insert (
		size_t index,
		const StringRef2& src
		)
	{
		return insert (index, src, src.getLength ());
	}

	size_t
	insert (
		size_t index,
		const StringRef3& src		
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
		size_t oldLength = this->m_length;

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
		size_t oldLength = this->m_length;

		if (length == -1)
			length = Details2::calcLength (p);

		if (length == 0)
			return oldLength;

		size_t insertLength = enc::UtfConvert <Encoding, Encoding2>::calcRequiredLength (p, length);
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
		size_t oldLength = this->m_length;

		if (length == -1)
			length = Details3::calcLength (p);

		if (length == 0)
			return oldLength;

		size_t insertLength = enc::UtfConvert <Encoding, Encoding3>::calcRequiredLength (p, length);
		C* dst = insertSpace (index, insertLength);
		if (!dst)
			return -1;

		enc::UtfConvert <Encoding, Encoding3>::convert (dst, insertLength, p, length);
		return oldLength + insertLength;
	}

	size_t
	insert (
		size_t index,
		utf32_t x
		)
	{
		return x ? insert (index, x, 1) : this->m_length;
	}

	size_t
	insert (
		size_t index,
		utf32_t x,
		size_t count
		)
	{
		size_t oldLength = this->m_length;

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
#if (_AXL_OS_WIN)
		return insert (index, Details::getCrLf (), 2);
#else
		return insert (index, '\n');
#endif
	}

	size_t 
	trimLeft ()
	{
		static StringRef whitespace (Details::getWhitespace (), 4);
		size_t i = this->findNotOneOf (whitespace);
		if (i != -1)
		{
			this->m_p += i;
			this->m_length -= i;
		}

		return this->m_length;
	}

	size_t 
	trimRight ()
	{
		static StringRef whitespace (Details::getWhitespace (), 4);
		size_t i = this->reverseFindNotOneOf (whitespace);
		if (i != -1)
			setReducedLength (i + 1);

		return this->m_length;
	}

	size_t 
	trim ()
	{
		trimLeft ();
		return trimRight ();
	}

	size_t 
	replace (
		C from,
		C to
		)
	{
		bool result = ensureExclusive ();
		if (!result)
			return -1;

		size_t count = 0;
		C* p = this->m_p;
		C* end = p + this->m_length;

		for (; p < end; p++)
			if (*p == from)
				*p = to;

		return count;
	}

	template <typename CaseOp>
	size_t
	convertCase ()
	{
		sl::StringRef src = *this; // save old contents -- can't convert in-place because length can increase

		size_t length = enc::UtfConvert <Encoding, Encoding, CaseOp>::calcRequiredLength (this->m_p, this->m_length);
		C* p = createBuffer (length);
		if (!p)
			return -1;

		enc::UtfConvert <Encoding, Encoding, CaseOp>::convert (p, length, src, src.getLength ());
		return length;
	}

	size_t 
	makeLowerCase ()
	{
		return convertCase <enc::UtfToLowerCase> ();
	}

	size_t 
	makeUpperCase ()
	{
		return convertCase <enc::UtfToUpperCase> ();
	}

	size_t 
	makeCaseFolded ()
	{
		return convertCase <enc::UtfToCaseFolded> ();
	}

	size_t
	format_va (
		const C* formatString,
		axl_va_list va
		)
	{
		size_t length = Details::calcFormatLength_va (formatString, va);
		if (!createBuffer (length, false))
			return -1;

		Details::format_va (this->m_p, length + 1, formatString, va);
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
		size_t oldLength = this->m_length;
		size_t newLength = oldLength + appendLength;
		if (!createBuffer (newLength, true))
			return -1;

		Details::format_va (this->m_p + oldLength, appendLength + 1, formatString, va);
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
		if (this->m_length <= delta)
			clear ();
		else
			setReducedLength (this->m_length - delta);

		return this->m_length;
	}

	void
	setReducedLength (size_t length)
	{
		if (length >= this->m_length)
		{
			ASSERT (length == this->m_length); // misuse otherwise
			return; 
		}

		if (!length)
		{
			clear ();
			return;
		}

		ASSERT (this->m_p && this->m_hdr);
		
		if (!this->m_p [length])
		{
			this->m_isNullTerminated = true;
		}
		else if (this->m_hdr->getRefCount () == 1)
		{
			this->m_p [length] = 0;
			this->m_isNullTerminated = true;
		}
		else
		{
			this->m_isNullTerminated = false;
		}

		this->m_length = length;
	}

	bool
	ensureExclusive ()
	{
		return this->m_length ? createBuffer (this->m_length, true) != NULL : true;
	}

	C*
	getBuffer (size_t* length = NULL)
	{
		C* p = createBuffer (this->m_length);
		if (!p)
			return NULL;

		if (length)
		{
			C* end = (C*) ((char*) (this->m_hdr + 1) + this->m_hdr->m_bufferSize);		
			ASSERT (this->m_p >= (C*) (this->m_hdr + 1) && this->m_p < end);

			*length = end - this->m_p;
		}

		return p;
	}

	C*
	createBuffer (
		size_t length,
		bool saveContents = false
		)
	{			
		size_t size = (length + 1) * sizeof (C);

		if (this->m_hdr &&
			this->m_hdr->m_bufferSize >= size &&
			this->m_hdr->getRefCount () == 1)
		{
			this->m_length = length;
			this->m_p [length] = 0;
			this->m_isNullTerminated = true;
			return this->m_p;
		}

		size_t bufferSize = sl::getMinPower2Ge (size);

		ref::Ptr <ref::BufHdr> hdr = AXL_REF_NEW_EXTRA (ref::BufHdr, bufferSize);
		if (!hdr)
			return NULL;

		hdr->m_bufferSize = bufferSize;

		C* p = (C*) (hdr + 1);

		if (saveContents && this->m_p)
		{
			size_t copyLength = AXL_MIN (length, this->m_length);
			Details::copy (p, this->m_p, copyLength);
		}

		if (this->m_hdr)
			this->m_hdr->release ();

		this->m_p = p;
		this->m_hdr = hdr.detach ();
		this->m_length = length;
		this->m_isNullTerminated = true; // AXL_REF_NEW zeroes memory
		return p;
	}

	size_t
	setBuffer (
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		ASSERT (size >= sizeof (ref::BufHdr) + sizeof (C));

		uint_t flags = kind != ref::BufKind_Static ? ref::BufHdrFlag_Exclusive : 0;
		size_t bufferSize = size - sizeof (ref::BufHdr);
		
		ref::Ptr <ref::BufHdr> hdr = AXL_REF_NEW_INPLACE (ref::BufHdr, p, flags);
		hdr->m_bufferSize = bufferSize;

		if (this->m_hdr)
			this->m_hdr->release ();

		this->m_p = (C*) (hdr + 1);
		this->m_p [0] = 0;
		this->m_hdr = hdr.detach ();
		this->m_length = 0;
		this->m_isNullTerminated = true;

		return bufferSize / sizeof (C) - 1;
	}

	C*
	reserve (
		size_t length,
		bool saveContents = false
		)
	{
		if (saveContents && length < this->m_length)
			length = this->m_length;

		size_t oldLength = this->m_length;

		if (!createBuffer (length, saveContents))
			return NULL;

		this->m_p [oldLength] = 0;
		this->m_length = oldLength;
		this->m_isNullTerminated = true;
		return this->m_p;
	}

	size_t
	updateLength ()
	{
		if (!this->m_hdr)
			return 0;

		ASSERT (this->m_hdr->isInsideBuffer (this->m_p));

		size_t maxLength = (C*) this->m_hdr->getBufferEnd () - this->m_p;
		this->m_length = Details::calcLength (this->m_p, maxLength);
		this->m_isNullTerminated = this->m_length < maxLength;
		return this->m_length;
	}

protected:
	C*
	insertSpace (
		size_t index,
		size_t length
		)
	{
		size_t oldLength = this->m_length;

		if (!createBuffer (oldLength + length, true))
			return NULL;

		if (index > oldLength)
			index = oldLength;

		C* dst = this->m_p + index;

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
