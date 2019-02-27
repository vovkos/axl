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

#include "axl_sl_StringDetails.h"
#include "axl_sl_Hash.h"
#include "axl_ref_Buf.h"

namespace axl {
namespace sl {

template <typename T, typename Details> class StringBase;

//..............................................................................

enum CaseConvertMethod
{
	CaseConvertMethod_PerCodePoint = 0, // true UTF conversion (default)
	CaseConvertMethod_PerCodeUnit,      // per-unit tolower/toupper (less precise, but faster)
	CaseConvertMethod_Default = CaseConvertMethod_PerCodePoint
};

//..............................................................................

template <
	typename T,
	typename Details0 = StringDetailsBase<T>
	>
class StringRefBase
{
public:
	typedef Details0 Details;
	typedef typename Details::Details2 Details2;
	typedef typename Details::Details3 Details3;

	typedef typename Details::C C;
	typedef typename Details::C2 C2;
	typedef typename Details::C3 C3;

	typedef typename Details::Encoding Encoding;
	typedef typename Details2::Encoding Encoding2;
	typedef typename Details3::Encoding Encoding3;

	typedef StringRefBase<C, Details> StringRef;
	typedef StringRefBase<C2, Details2> StringRef2;
	typedef StringRefBase<C3, Details3> StringRef3;

	typedef StringBase<C, Details> String;
	typedef StringBase<C2, Details2> String2;
	typedef StringBase<C3, Details3> String3;

protected:
	mutable C* m_p;
	mutable ref::BufHdr* m_hdr;
	size_t m_length;
	mutable bool m_isNullTerminated;

public:
	StringRefBase()
	{
		initialize();
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	StringRefBase(StringRef&& src)
	{
		initialize();
		move(std::move(src));
	}
#endif

	StringRefBase(const StringRef& src)
	{
		initialize();
		attach(src);
	}

	StringRefBase(const C* p)
	{
		initialize();
		attach(NULL, p, Details::calcLength(p), p != NULL);
	}

	StringRefBase(
		const C* p,
		size_t length,
		bool isNullTerminated = false
		)
	{
		initialize();
		attach(NULL, p, length, isNullTerminated);
	}

	StringRefBase(
		const C* p,
		const void* end,
		bool isNullTerminated = false
		)
	{
		initialize();
		attach(NULL, p, (C*)end - p, isNullTerminated);
	}

	StringRefBase(
		ref::BufHdr* hdr,
		const C* p
		)
	{
		initialize();
		attach(hdr, p, Details::calcLength(p), p != NULL);
	}

	StringRefBase(
		ref::BufHdr* hdr,
		const C* p,
		size_t length,
		bool isNullTerminated = false
		)
	{
		initialize();
		attach(hdr, p, length, isNullTerminated);
	}

	StringRefBase(
		ref::BufHdr* hdr,
		const C* p,
		const void* end,
		bool isNullTerminated = false
		)
	{
		initialize();
		attach(hdr, p, (C*)end - p, isNullTerminated);
	}

	~StringRefBase()
	{
		release();
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	StringRefBase&
	operator = (StringRef&& src)
	{
		move(std::move(src));
		return *this;
	}
#endif

	StringRefBase&
	operator = (const StringRef& src)
	{
		attach(src);
		return *this;
	}

	bool
	operator == (const StringRef& string) const
	{
		return cmp(string) == 0;
	}

	bool
	operator != (const StringRef& string) const
	{
		return cmp(string) != 0;
	}

	bool
	operator < (const StringRef& string) const
	{
		return cmp(string) < 0;
	}

	bool
	operator <= (const StringRef& string) const
	{
		return cmp(string) <= 0;
	}

	bool
	operator > (const StringRef& string) const
	{
		return cmp(string) > 0;
	}

	bool
	operator >= (const StringRef& string) const
	{
		return cmp(string) >= 0;
	}

	bool
	operator == (const C* p) const
	{
		return cmp(p) == 0;
	}

	bool
	operator != (const C* p) const
	{
		return cmp(p) != 0;
	}

	bool
	operator < (const C* p) const
	{
		return cmp(p) < 0;
	}

	bool
	operator <= (const C* p) const
	{
		return cmp(p) <= 0;
	}

	bool
	operator > (const C* p) const
	{
		return cmp(p) > 0;
	}

	bool
	operator >= (const C* p) const
	{
		return cmp(p) >= 0;
	}

	String
	operator + (const StringRef& string) const
	{
		String result = *this;
		result.append(string);
		return result;
	}

	String
	operator + (const StringRef2& string) const
	{
		String result = *this;
		result.append(string);
		return result;
	}

	String
	operator + (const StringRef3& string) const
	{
		String result = *this;
		result.append(string);
		return result;
	}

	String
	operator + (const C* p) const
	{
		String result = *this;
		result.append(p);
		return result;
	}

	String
	operator + (const C2* p) const
	{
		String result = *this;
		result.append(p);
		return result;
	}

	String
	operator + (const C3* p) const
	{
		String result = *this;
		result.append(p);
		return result;
	}

	const C&
	operator [] (intptr_t i) const
	{
		ASSERT((size_t)i < m_length);
		return m_p[(size_t)i];
	}

	const C*
	cp() const
	{
		return m_p;
	}

	const C*
	sz() const
	{
		return m_length ? ensureNullTerminated() : Details::getEmptyString();
	}

	const C*
	szn() const
	{
		return m_length ? ensureNullTerminated() : NULL;
	}

	String2
	s2() const
	{
		return String2(m_p, m_length);
	}

	String3
	s3() const
	{
		return String3(m_p, m_length);
	}

	size_t
	getLength() const
	{
		return m_length;
	}

	ref::BufHdr*
	getHdr() const
	{
		return m_hdr;
	}

	const C*
	getEnd() const
	{
		return m_p + m_length;
	}


	bool
	isEmpty() const
	{
		return m_length == 0;
	}

	bool
	isNullTerminated() const
	{
		return m_isNullTerminated;
	}

	void
	clear()
	{
		release();
	}

	void
	release()
	{
		if (m_hdr)
			m_hdr->release();

		initialize();
	}

	size_t
	offset(size_t length)
	{
		if (length > m_length)
			length = m_length;

		m_p += length;
		m_length -= length;
		return m_length;
	}

	int
	cmp(const StringRef& string) const
	{
		int result = Details::cmp(m_p, string.m_p, AXL_MIN(m_length, string.m_length));
		return
			result ? result :
			m_length < string.m_length ? -1 :
			m_length > string.m_length ? 1 : 0;
	}

	int
	cmpIgnoreCase(
		const StringRef& string,
		CaseConvertMethod method = CaseConvertMethod_Default
		) const
	{
		return method == CaseConvertMethod_PerCodeUnit ?
			cmpIgnoreCase_pcu(string) :
			cmpIgnoreCase_pcp(string);
	}

	size_t
	hash() const
	{
		return djb2(m_p, m_length * sizeof(C));
	}

	size_t
	hashIgnoreCase(CaseConvertMethod method = CaseConvertMethod_Default) const
	{
		return method == CaseConvertMethod_PerCodeUnit ?
			hashIgnoreCase_pcu() :
			hashIgnoreCase_pcp();
	}

	bool
	isEqual(const StringRef& string) const
	{
		return
			m_length == string.m_length &&
			Details::cmp(m_p, string.m_p, m_length) == 0;
	}

	bool
	isEqualIgnoreCase(
		const StringRef& string,
		CaseConvertMethod method = CaseConvertMethod_Default
		) const
	{
		return cmpIgnoreCase(string, method) == 0;
	}

	bool
	isPrefix(const StringRef& string) const
	{
		return
			m_length >= string.m_length &&
			Details::cmp(m_p, string.m_p, string.m_length) == 0;
	}

	bool
	isPrefixIgnoreCase(
		const StringRef& string,
		CaseConvertMethod method = CaseConvertMethod_Default
		) const
	{
		return
			m_length >= string.m_length &&
			getLeftSubString(string.m_length).isEqualIgnoreCase(string, method);
	}

	bool
	isSuffix(const StringRef& string) const
	{
		return
			m_length >= string.m_length &&
			Details::cmp(m_p + m_length - string.m_length, string.m_p, string.m_length) == 0;
	}

	bool
	isSuffixIgnoreCase(
		const StringRef& string,
		CaseConvertMethod method = CaseConvertMethod_Default
		) const
	{
		return
			m_length >= string.m_length &&
			getRightSubString(string.m_length).isEqualIgnoreCase(string, method);
	}

	size_t
	find(C c) const
	{
		return Details::find(m_p, m_length, c);
	}

	size_t
	find(const StringRef& subString) const
	{
		return Details::find(m_p, m_length, subString.m_p, subString.m_length);
	}

	size_t
	findOneOf(const StringRef& charSet) const
	{
		return Details::findOneOf(m_p, m_length, charSet.m_p, charSet.m_length);
	}

	size_t
	findNotOneOf(const StringRef& charSet) const
	{
		return Details::findNotOneOf(m_p, m_length, charSet.m_p, charSet.m_length);
	}

	size_t
	reverseFind(C c) const
	{
		return Details::reverseFind(m_p, m_length, c);
	}

	size_t
	reverseFind(const StringRef& subString) const
	{
		return Details::reverseFind(m_p, m_length, subString.m_p, subString.m_length);
	}

	size_t
	reverseFindOneOf(const StringRef& charSet) const
	{
		return Details::reverseFindOneOf(m_p, m_length, charSet.m_p, charSet.m_length);
	}

	size_t
	reverseFindNotOneOf(const StringRef& charSet) const
	{
		return Details::reverseFindNotOneOf(m_p, m_length, charSet.m_p, charSet.m_length);
	}

	StringRef
	getSubString(
		size_t first,
		size_t length = -1
		) const
	{
		if (first > m_length)
			return StringRef();

		size_t end = length == -1 ? m_length : first + length;
		if (end > m_length)
			end = m_length;

		return StringRef(m_hdr, m_p + first, end - first);
	}

	StringRef
	getLeftSubString(size_t length) const
	{
		if (length >= m_length)
			return *this;

		return StringRef(m_hdr, m_p, length);
	}

	StringRef
	getRightSubString(size_t length) const
	{
		if (length >= m_length)
			return *this;

		return StringRef(m_hdr, m_p + m_length - length, length);
	}

	StringRef
	getLeftTrimmedString() const
	{
		static StringRef whitespace(Details::getWhitespace(), 4);
		size_t i = findNotOneOf(whitespace);
		if (i == -1)
			return StringRef();
		else if (i == 0)
			return *this;

		StringRef string;
		string.attach(m_hdr, m_p + i, m_length - i, m_isNullTerminated);
		return string;
	}

	StringRef
	getRightTimmedString() const
	{
		static StringRef whitespace(Details::getWhitespace(), 4);
		size_t i = reverseFindNotOneOf(whitespace);
		if (i == -1)
			return StringRef();
		else if (i == m_length - 1)
			return *this;

		StringRef string;
		string.attach(m_hdr, m_p, i + 1, false);
		return string;
	}

	StringRef
	getTrimmedString() const
	{
		static StringRef whitespace(Details::getWhitespace(), 4);
		size_t i = findNotOneOf(whitespace);
		if (i == -1)
			return StringRef();

		size_t j = reverseFindNotOneOf(whitespace);
		ASSERT(j != -1);

		if (j != m_length - 1)
		{
			StringRef string;
			string.attach(m_hdr, m_p + i, j + 1 - i, false);
			return string;
		}
		else if (i != 0)
		{
			StringRef string;
			string.attach(m_hdr, m_p + i, m_length - i, m_isNullTerminated);
			return string;
		}
		else
		{
			return *this;
		}
	}

	String
	getLowerCaseString(CaseConvertMethod method = CaseConvertMethod_Default)
	{
		String string = *this;
		string.makeLowerCase(method);
		return string;
	}

	String
	getUpperCaseString(CaseConvertMethod method = CaseConvertMethod_Default)
	{
		String string = *this;
		string.makeUpperCase(method);
		return string;
	}

	String
	getCaseFoldedString(CaseConvertMethod method = CaseConvertMethod_Default)
	{
		String string = *this;
		string.makeCaseFolded(method);
		return string;
	}

protected:
	const C*
	ensureNullTerminated() const
	{
		if (m_isNullTerminated)
			return m_p;

		if (m_hdr && m_hdr->isInsideBuffer(m_p + m_length) && !m_p[m_length])
		{
			m_isNullTerminated = true;
			return m_p;
		}

		String string(*this);
		attachBufHdr(string.getHdr());
		m_p = (C*)string.sz();
		m_isNullTerminated = true;
		return m_p;
	}

	void
	initialize()
	{
		m_p = NULL;
		m_hdr = NULL;
		m_length = 0;
		m_isNullTerminated = false;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	void
	move(StringRefBase&& src)
	{
		if (m_hdr)
			m_hdr->release();

		m_p = src.m_p;
		m_hdr = src.m_hdr;
		m_length = src.m_length;
		m_isNullTerminated = src.m_isNullTerminated;
		src.initialize();
	}
#endif

	void
	attachBufHdr(ref::BufHdr* hdr) const
	{
		if (hdr == m_hdr)
			return; // try to avoid unnecessary interlocked ops

		if (hdr)
			hdr->addRef();

		if (m_hdr)
			m_hdr->release();

		m_hdr = hdr;
	}

	void
	attach(const StringRef& src)
	{
		if (&src != this)
			attach(src.m_hdr, src.m_p, src.m_length, src.m_isNullTerminated);
	}

	void
	attach(
		ref::BufHdr* hdr,
		const C* p,
		size_t length,
		bool isNullTerminated
		)
	{
		ASSERT(length != -1 && (!isNullTerminated || !p[length]));

		attachBufHdr(hdr);

		m_p = (C*)p;
		m_length = length;
		m_isNullTerminated = isNullTerminated;
	}

	int
	cmpIgnoreCase_pcu(const StringRef& string) const
	{
		size_t length = AXL_MIN(m_length, string.m_length);

		const C* p1 = m_p;
		const C* p2 = string.m_p;
		const C* end = p1 + length;
		for (; p1 < end; p1++, p2++)
		{
			C c1 = Details::toLower(*p1);
			C c2 = Details::toLower(*p2);

			if (c1 < c2)
				return -1;
			else if (c1 > c2)
				return 1;
		}

		return
			m_length < string.m_length ? -1 :
			m_length > string.m_length ? 1 : 0;
	}

	int
	cmpIgnoreCase_pcp(const StringRef& string) const
	{
		const C* p1 = m_p;
		const C* end1 = p1 + m_length;
		const C* p2 = string.m_p;
		const C* end2 = p2 + string.m_length;

		while (p1 < end1 && p2 < end2)
		{
			size_t codePointLength1 = Encoding::getDecodeCodePointLength(*p1);
			size_t codePointLength2 = Encoding::getDecodeCodePointLength(*p2);

			if (p1 + codePointLength1 > end1)
				return p2 + codePointLength2 > end2 ? 0 : -1; // if both are badly terminated, return 0
			else if (p2 + codePointLength2 > end2)
				return 1;

			utf32_t c1 = Encoding::decodeCodePoint(p1);
			utf32_t c2 = Encoding::decodeCodePoint(p2);

			c1 = enc::UtfToCaseFolded() (c1);
			c2 = enc::UtfToCaseFolded() (c2);

			if (c1 < c2)
				return -1;
			else if (c1 > c2)
				return 1;

			p1 += codePointLength1;
			p2 += codePointLength2;
		}

		return
			p2 < end2 ? -1 :
			p1 < end1 ? 1 : 0;
	}

	size_t
	hashIgnoreCase_pcu() const
	{
		size_t h = djb2();

		const C* end = m_p + m_length;
		for (const C* p = m_p; p < end; p++)
		{
			C c = Details::toLower(*p);
			h = djb2(h, &c, sizeof(c));
		}

		return h;
	}

	size_t
	hashIgnoreCase_pcp() const
	{
		size_t h = djb2();

		const C* p = m_p;
		const C* end = m_p + m_length;

		while (p < end)
		{
			size_t codePointLength = Encoding::getDecodeCodePointLength(*p);
			if (p + codePointLength > end)
				break;

			utf32_t c = Encoding::decodeCodePoint(p);
			c = enc::UtfToCaseFolded() (c);
			h = djb2(h, &c, sizeof(c));
			p += codePointLength;
		}

		return h;
	}
};

//..............................................................................

template <
	typename T,
	typename Details0 = StringDetailsBase<T>
	>
class StringBase: public StringRefBase<T, Details0>
{
public:
	typedef StringRefBase<T, Details0> StringRef;

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
	StringBase()
	{
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	StringBase(StringBase&& src)
	{
		copy(std::move(src));
	}

	StringBase(StringRef&& src)
	{
		copy(std::move(src));
	}
#endif

	StringBase(const StringBase& src)
	{
		copy(src);
	}

	StringBase(const StringRef& src)
	{
		copy(src);
	}

	StringBase(const StringRef2& src)
	{
		copy(src);
	}

	StringBase(const StringRef3& src)
	{
		copy(src);
	}

	StringBase(
		const C* p,
		size_t length = -1
		)
	{
		copy(p, length);
	}

	StringBase(
		const C* p,
		const void* end
		)
	{
		copy(p, (C*)end - p);
	}

	StringBase(
		const C2* p,
		size_t length = -1
		)
	{
		copy(p, length);
	}

	StringBase(
		const C2* p,
		const void* end
		)
	{
		copy(p, (C2*)end - p);
	}

	StringBase(
		const C3* p,
		size_t length = -1
		)
	{
		copy(p, length);
	}

	StringBase(
		const C3* p,
		const void* end
		)
	{
		copy(p, (C3*)end - p);
	}

	StringBase(utf32_t x)
	{
		copy(x);
	}

	StringBase(
		utf32_t x,
		size_t count
		)
	{
		copy(x, count);
	}

	StringBase(
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		setBuffer(kind, p, size);
	}

	operator const C* () const
	{
		return sz();
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	StringBase&
	operator = (StringBase&& src)
	{
		copy(std::move(src));
		return *this;
	}

	StringBase&
	operator = (StringRef&& src)
	{
		copy(std::move(src));
		return *this;
	}
#endif

	StringBase&
	operator = (const StringBase& src)
	{
		copy(src);
		return *this;
	}

	StringBase&
	operator = (const StringRef& src)
	{
		copy(src);
		return *this;
	}

	StringBase&
	operator = (const StringRef2& src)
	{
		copy(src);
		return *this;
	}

	StringBase&
	operator = (const StringRef3& src)
	{
		copy(src);
		return *this;
	}

	StringBase&
	operator = (const C* p)
	{
		copy(p, -1);
		return *this;
	}

	StringBase&
	operator = (const C2* p)
	{
		copy(p, -1);
		return *this;
	}

	StringBase&
	operator = (const C3* p)
	{
		copy(p, -1);
		return *this;
	}

	StringBase&
	operator = (utf32_t x)
	{
		copy(x);
		return *this;
	}

	StringBase&
	operator += (const StringRef& string)
	{
		append(string);
		return *this;
	}

	StringBase&
	operator += (const StringRef2& string)
	{
		append(string);
		return *this;
	}

	StringBase&
	operator += (const StringRef3& string)
	{
		append(string);
		return *this;
	}

	StringBase&
	operator += (const C* p)
	{
		append(p, -1);
		return *this;
	}

	StringBase&
	operator += (const C2* p)
	{
		append(p, -1);
		return *this;
	}

	StringBase&
	operator += (const C3* p)
	{
		append(p, -1);
		return *this;
	}

	StringBase&
	operator += (utf32_t x)
	{
		append(x);
		return *this;
	}

	const C&
	operator [] (intptr_t i) const
	{
		ASSERT((size_t)i < this->m_length);
		return this->m_p[(size_t)i];
	}

	C&
	operator [] (intptr_t i)
	{
		ASSERT((size_t)i < this->m_length);
		return this->m_p[(size_t)i];
	}

	C*
	p()
	{
		return this->m_p;
	}

	const C*
	sz() const
	{
		ASSERT(!this->m_length || this->m_isNullTerminated);
		return this->m_length ? this->m_p : Details::getEmptyString();
	}

	const C*
	szn() const
	{
		ASSERT(!this->m_length || this->m_isNullTerminated);
		return this->m_length ? this->m_p : NULL;
	}

	void
	clear()
	{
		if (!this->m_hdr)
		{
			ASSERT(!this->m_length);
			return;
		}

		if (this->m_hdr->getRefCount() != 1)
		{
			this->release();
			return;
		}

		this->m_p = (C*)(this->m_hdr + 1);
		this->m_p[0] = 0;
		this->m_length = 0;
	}

	size_t
	forceCopy(const StringRef& src)
	{
		return copy(src.cp(), src.getLength());
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	size_t
	copy(StringRef&& src)
	{
		if (src.isEmpty())
		{
			clear();
			src.release();
			return 0;
		}

		ref::BufHdr* hdr = src.getHdr();
		if (!hdr || (hdr->getFlags() & ref::BufHdrFlag_Exclusive) || !src.isNullTerminated())
		{
			copy(src.cp(), src.getLength());
			src.release();
			return this->m_length;
		}

		this->move(std::move(src));
		return this->m_length;
	}
#endif

	size_t
	copy(const StringRef& src)
	{
		if (&src == this)
			return this->m_length;

		if (src.isEmpty())
		{
			clear();
			return 0;
		}

		ref::BufHdr* hdr = src.getHdr();
		if (!hdr || (hdr->getFlags() & ref::BufHdrFlag_Exclusive) || !src.isNullTerminated())
			return copy(src.cp(), src.getLength());

		this->attach(src);
		return this->m_length;
	}

	size_t
	copy(const StringRef2& src)
	{
		return copy(src.cp(), src.getLength());
	}

	size_t
	copy(const StringRef3& src)
	{
		return copy(src.cp(), src.getLength());
	}

	size_t
	copy(
		const C* p,
		size_t length = -1
		)
	{
		if (p == this->m_p && (length == -1 || length == this->m_length))
			return this->m_length;

		if (length == -1)
			length = Details::calcLength(p);

		if (!length)
		{
			clear();
			return 0;
		}

		if (this->m_hdr && this->m_hdr->isInsideBuffer(p))
		{
			C* end = (C*)this->m_hdr->getEnd();
			ASSERT(p + length <= end);

			this->m_p = (C*)p;
			this->m_length = length;
			this->m_isNullTerminated = p + length < end && !p[length];
			return length;
		}

		if (!createBuffer(length, false))
			return -1;

		Details::copy(this->m_p, p, length);
		return length;
	}

	size_t
	copy(
		const C2* p,
		size_t length = -1
		)
	{
		if (length == -1)
			length = Details2::calcLength(p);

		if (!length)
		{
			clear();
			return 0;
		}

		size_t newLength = enc::UtfConvert<Encoding, Encoding2>::calcRequiredLength(p, length);
		if (!createBuffer(newLength, false))
			return -1;

		enc::UtfConvert<Encoding, Encoding2>::convert(this->m_p, newLength, p, length);
		return newLength;
	}

	size_t
	copy(
		const C3* p,
		size_t length = -1
		)
	{
		if (length == -1)
			length = Details3::calcLength(p);

		if (!length)
		{
			clear();
			return 0;
		}

		size_t newLength = enc::UtfConvert<Encoding, Encoding3>::calcRequiredLength(p, length);
		if (!createBuffer(newLength, false))
			return -1;

		enc::UtfConvert<Encoding, Encoding3>::convert(this->m_p, newLength, p, length);
		return newLength;
	}

	size_t
	copy(utf32_t x)
	{
		if (!x)
		{
			clear();
			return 0;
		}

		return copy(x, 1);
	}

	size_t
	copy(
		utf32_t x,
		size_t count
		)
	{
		if (!count)
		{
			clear();
			return 0;
		}

		size_t codePointLength = Encoding::getEncodeCodePointLength(x);
		if (codePointLength == -1)
			return -1;

		ASSERT(codePointLength <= 4);

		size_t newLength = count * codePointLength;
		if (!createBuffer(newLength, false))
			return -1;

		C pattern[sizeof(utf32_t) / sizeof(C)];
		Encoding::encodeCodePoint(pattern, x);
		fillWithPattern(this->m_p, pattern, codePointLength, count);
		return newLength;
	}

	size_t
	append(const StringRef& src)
	{
		return insert(-1, src);
	}

	size_t
	append(const StringRef2& src)
	{
		return insert(-1, src);
	}

	size_t
	append(const StringRef3& src)
	{
		return insert(-1, src);
	}

	size_t
	append(
		const C* p,
		size_t length = -1
		)
	{
		return insert(-1, p, length);
	}

	size_t
	append(
		const C2* p,
		size_t length = -1
		)
	{
		return insert(-1, p, length);
	}

	size_t
	append(
		const C3* p,
		size_t length = -1
		)
	{
		return insert(-1, p, length);
	}

	size_t
	append(utf32_t x)
	{
		return x ? append(x, 1) : this->m_length;
	}

	size_t
	append(
		utf32_t x,
		size_t count
		)
	{
		return insert(-1, x, count);
	}

	size_t
	appendNewLine()
	{
		return insertNewLine(-1);
	}

	size_t
	insert(
		size_t index,
		const StringRef& src
		)
	{
		return !this->m_length ? copy(src) : insert(index, src.cp(), src.getLength());
	}

	size_t
	insert(
		size_t index,
		const StringRef2& src
		)
	{
		return insert(index, src.cp(), src.getLength());
	}

	size_t
	insert(
		size_t index,
		const StringRef3& src
		)
	{
		return insert(index, src.cp(), src.getLength());
	}

	size_t
	insert(
		size_t index,
		const C* p,
		size_t length = -1
		)
	{
		size_t oldLength = this->m_length;

		if (length == -1)
			length = Details::calcLength(p);

		if (length == 0)
			return oldLength;

		ref::Ptr<void> shadow;
		if (this->m_hdr && this->m_hdr->isInsideBuffer(p))
			shadow = this->m_hdr; // ensure we keep p intact

		C* dst = insertSpace(index, length);
		if (!dst)
			return -1;

		Details::copy(dst, p, length);
		return oldLength + length;
	}

	size_t
	insert(
		size_t index,
		const C2* p,
		size_t length = -1
		)
	{
		size_t oldLength = this->m_length;

		if (length == -1)
			length = Details2::calcLength(p);

		if (length == 0)
			return oldLength;

		size_t insertLength = enc::UtfConvert<Encoding, Encoding2>::calcRequiredLength(p, length);
		C* dst = insertSpace(index, insertLength);
		if (!dst)
			return -1;

		enc::UtfConvert<Encoding, Encoding2>::convert(dst, insertLength, p, length);
		return oldLength + insertLength;
	}

	size_t
	insert(
		size_t index,
		const C3* p,
		size_t length = -1
		)
	{
		size_t oldLength = this->m_length;

		if (length == -1)
			length = Details3::calcLength(p);

		if (length == 0)
			return oldLength;

		size_t insertLength = enc::UtfConvert<Encoding, Encoding3>::calcRequiredLength(p, length);
		C* dst = insertSpace(index, insertLength);
		if (!dst)
			return -1;

		enc::UtfConvert<Encoding, Encoding3>::convert(dst, insertLength, p, length);
		return oldLength + insertLength;
	}

	size_t
	insert(
		size_t index,
		utf32_t x
		)
	{
		return x ? insert(index, x, 1) : this->m_length;
	}

	size_t
	insert(
		size_t index,
		utf32_t x,
		size_t count
		)
	{
		size_t oldLength = this->m_length;

		if (count == 0)
			return oldLength;

		size_t codePointLength = Encoding::getEncodeCodePointLength(x);
		if (codePointLength == -1)
			return -1;

		ASSERT(codePointLength <= 4);

		size_t insertLength = count * codePointLength;

		C* dst = insertSpace(index, count * codePointLength);
		if (!dst)
			return -1;

		C pattern[sizeof(utf32_t) / sizeof(C)];
		Encoding::encodeCodePoint(pattern, x);
		fillWithPattern(dst, pattern, codePointLength, count);
		return oldLength + insertLength;
	}

	size_t
	insertNewLine(size_t index)
	{
#if (_AXL_OS_WIN)
		return insert(index, Details::getCrLf(), 2);
#else
		return insert(index, '\n');
#endif
	}

	size_t
	remove(
		size_t index,
		size_t count = 1
		)
	{
		bool result;

		if (count == 0)
			return this->m_length;

		size_t oldLength = this->m_length;
		if (index >= oldLength)
			return this->m_length;

		if (index + count >= oldLength)
		{
			result = setReducedLength(index);
			return result ? index : -1;
		}

		result = ensureExclusive();
		if (!result)
			return -1;

		size_t newLength = oldLength - count;

		T* dst = this->m_p + index;
		Details::copy(dst, dst + count, newLength - index);

		result = setReducedLength(newLength);
		return result ? newLength : -1;
	}

	size_t
	trimLeft()
	{
		static StringRef whitespace(Details::getWhitespace(), 4);
		size_t i = this->findNotOneOf(whitespace);
		if (i == -1)
		{
			clear();
			return 0;
		}

		this->m_p += i;
		this->m_length -= i;
		return this->m_length;
	}

	size_t
	trimRight()
	{
		static StringRef whitespace(Details::getWhitespace(), 4);
		size_t i = this->reverseFindNotOneOf(whitespace);
		if (i == -1)
		{
			clear();
			return 0;
		}

		setReducedLength(i + 1);
		return this->m_length;
	}

	size_t
	trim()
	{
		trimLeft();
		return trimRight();
	}

	size_t
	replace(
		const StringRef& from,
		const StringRef& to
		)
	{
		return replace(from, from.getLength(), to, to.getLength());
	}

	size_t
	replace(
		const C* from,
		const C* to
		)
	{
		return replace(from, Details::calcLength(from), to, Details::calcLength(to));
	}

	size_t
	replace(
		const C* from,
		size_t fromLength,
		const C* to,
		size_t toLength
		)
	{
		bool result = ensureExclusive();
		if (!result)
			return -1;

		size_t count = 0;
		size_t offset = 0;

		for (;;)
		{
			size_t i = Details::find(this->m_p + offset, this->m_length - offset, from, fromLength);
			if (i == -1)
				break;

			i += offset;

			result =
				fromLength < toLength ?	insertSpace(i, toLength - fromLength) != NULL :
				fromLength > toLength ?	remove(i, fromLength - toLength) != -1 :
				true;

			if (!result)
				return -1;

			Details::copy(this->m_p + i, to, toLength);
			offset = i + toLength;
		}

		return count;
	}

	size_t
	replace(
		C from,
		C to
		)
	{
		bool result = ensureExclusive();
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

	size_t
	makeLowerCase(CaseConvertMethod method = CaseConvertMethod_Default)
	{
		return method == CaseConvertMethod_PerCodeUnit ?
			convertCase_pcu(Details::toLower) :
			convertCase_pcp<enc::UtfToLowerCase> ();
	}

	size_t
	makeUpperCase(CaseConvertMethod method = CaseConvertMethod_Default)
	{
		return method == CaseConvertMethod_PerCodeUnit ?
			convertCase_pcu(Details::toUpper) :
			convertCase_pcp<enc::UtfToUpperCase> ();
	}

	size_t
	makeCaseFolded(CaseConvertMethod method = CaseConvertMethod_Default)
	{
		return method == CaseConvertMethod_PerCodeUnit ?
			convertCase_pcu(Details::toLower) :
			convertCase_pcp<enc::UtfToCaseFolded> ();
	}

	size_t
	format_va(
		const C* formatString,
		axl_va_list va
		)
	{
		size_t length = Details::calcFormatLength_va(formatString, va);
		if (!createBuffer(length, false))
			return -1;

		Details::format_va(this->m_p, length + 1, formatString, va);
		return length;
	}

	size_t
	format(
		const C* formatString,
		...
		)
	{
		AXL_VA_DECL(va, formatString);
		return format_va(formatString, va);
	}

	size_t
	appendFormat_va(
		const C* formatString,
		axl_va_list va
		)
	{
		size_t appendLength = Details::calcFormatLength_va(formatString, va);
		size_t oldLength = this->m_length;
		size_t newLength = oldLength + appendLength;
		if (!createBuffer(newLength, true))
			return -1;

		Details::format_va(this->m_p + oldLength, appendLength + 1, formatString, va);
		return newLength;
	}

	size_t
	appendFormat(
		const C* formatString,
		...
		)
	{
		AXL_VA_DECL(va, formatString);
		return appendFormat_va(formatString, va);
	}

	size_t
	chop(size_t delta)
	{
		if (this->m_length <= delta)
		{
			clear();
		}
		else
		{
			bool result = setReducedLength(this->m_length - delta);
			if (!result)
				return -1;
		}

		return this->m_length;
	}

	bool
	setReducedLength(size_t length)
	{
		if (length == this->m_length)
			return true;

		if (!length)
		{
			clear();
			return true;
		}

		ASSERT(this->m_p && this->m_hdr);
		ASSERT(length < this->m_hdr->getLeftoverBufferSize(this->m_p) / sizeof(C)); // misuse otherwise

		bool isNullTerminated = !this->m_p[length];
		if (!isNullTerminated && this->m_hdr->getRefCount() == 1)
		{
			this->m_p[length] = 0;
			isNullTerminated = true;
		}

		this->m_length = length;
		return isNullTerminated || createBuffer(length, true) != NULL;
	}

	bool
	isExclusive()
	{
		return !this->m_length || this->m_hdr && this->m_hdr->getRefCount() == 1;
	}

	bool
	ensureExclusive()
	{
		return this->m_length ? createBuffer(this->m_length, true) != NULL : true;
	}

	C*
	getBuffer(size_t* length = NULL)
	{
		C* p = createBuffer(this->m_length, true);
		if (!p)
			return NULL;

		if (length)
		{
			size_t bufferLength = this->m_hdr->getLeftoverBufferSize(this->m_p) / sizeof(C);
			ASSERT(bufferLength);

			*length = bufferLength - 1;
		}

		return p;
	}

	C*
	createBuffer(
		size_t length,
		bool saveContents = false
		)
	{
		size_t size = (length + 1) * sizeof(C);

		if (this->m_hdr &&
			this->m_hdr->m_bufferSize >= size &&
			this->m_hdr->getRefCount() == 1)
		{
			if (!this->m_length || !saveContents)
				this->m_p = (C*)(this->m_hdr + 1);

			if (this->m_hdr->getLeftoverBufferSize(this->m_p) >= size)
			{
				this->m_length = length;
				this->m_p[length] = 0;
				this->m_isNullTerminated = true;
				return this->m_p;
			}
		}

		size_t bufferSize = getAllocSize(size);

		ref::Ptr<ref::BufHdr> hdr = AXL_REF_NEW_EXTRA(ref::BufHdr, bufferSize);
		if (!hdr)
			return NULL;

		hdr->m_bufferSize = bufferSize;

		C* p = (C*)(hdr + 1);

		if (saveContents && this->m_p)
		{
			size_t copyLength = AXL_MIN(length, this->m_length);
			Details::copy(p, this->m_p, copyLength);
		}

		if (this->m_hdr)
			this->m_hdr->release();

		p[length] = 0; // ensure zero termination

		this->m_p = p;
		this->m_hdr = hdr.detach();
		this->m_length = length;
		this->m_isNullTerminated = true;
		return p;
	}

	size_t
	setBuffer(
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		ASSERT(size >= sizeof(ref::BufHdr) + sizeof(C));

		uint_t flags = kind != ref::BufKind_Static ? ref::BufHdrFlag_Exclusive : 0;
		size_t bufferSize = size - sizeof(ref::BufHdr);

		ref::Ptr<ref::BufHdr> hdr = AXL_REF_NEW_INPLACE(ref::BufHdr, p, NULL, flags);
		hdr->m_bufferSize = bufferSize;

		if (this->m_hdr)
			this->m_hdr->release();

		this->m_p = (C*)(hdr + 1);
		this->m_p[0] = 0;
		this->m_hdr = hdr.detach();
		this->m_length = 0;
		this->m_isNullTerminated = true;
		return bufferSize / sizeof(C) - 1;
	}

	C*
	reserve(
		size_t length,
		bool saveContents = false
		)
	{
		if (saveContents && length < this->m_length)
			length = this->m_length;

		size_t oldLength = this->m_length;

		if (!createBuffer(length, saveContents))
			return NULL;

		this->m_p[oldLength] = 0;
		this->m_length = oldLength;
		this->m_isNullTerminated = true;
		return this->m_p;
	}

	size_t
	updateLength()
	{
		if (!this->m_hdr)
			return 0;

		size_t fullLength = this->m_hdr->getLeftoverBufferSize(this->m_p) / sizeof(C);
		ASSERT(fullLength);

		this->m_length = Details::calcLength(this->m_p, fullLength - 1);
		return this->m_length;
	}

protected:
	C*
	insertSpace(
		size_t index,
		size_t length
		)
	{
		size_t oldLength = this->m_length;

		if (!createBuffer(oldLength + length, true))
			return NULL;

		if (index > oldLength)
			index = oldLength;

		C* dst = this->m_p + index;

		if (length && index < oldLength)
			Details::move(dst + length, dst, oldLength - index);

		return dst;
	}

	static
	void
	fillWithPattern(
		C* p,
		const C* pattern,
		size_t patternLength,
		size_t count
		)
	{
		if (patternLength == 1)
		{
			Details::fill(p, *pattern, count);
			return;
		}

		C* end = p + count * patternLength;

		for (; p < end; p += patternLength)
			Details::copy(p, pattern, patternLength);
	}

	template <typename CaseOp>
	size_t
	convertCase_pcu(CaseOp op)
	{
		bool result = ensureExclusive();
		if (!result)
			return -1;

		C* end = this->m_p + this->m_length;
		for (C* p = this->m_p; p < end; p++)
			*p = op(*p);

		return this->m_length;
	}

	template <typename CaseOp>
	size_t
	convertCase_pcp()
	{
		StringRef src = *this; // save old contents -- can't convert in-place because length can increase

		size_t length = enc::UtfConvert<Encoding, Encoding, CaseOp>::calcRequiredLength(this->m_p, this->m_length);
		C* p = createBuffer(length);
		if (!p)
			return -1;

		enc::UtfConvert<Encoding, Encoding, CaseOp>::convert(p, length, src.cp(), src.getLength());
		return length;
	}
};

//..............................................................................

typedef StringRefBase<char>    StringRef;
typedef StringRefBase<wchar_t> StringRef_w;
typedef StringRefBase<utf8_t>  StringRef_utf8;
typedef StringRefBase<utf16_t> StringRef_utf16;
typedef StringRefBase<utf32_t> StringRef_utf32;

typedef StringBase<char>    String;
typedef StringBase<wchar_t> String_w;
typedef StringBase<utf8_t>  String_utf8;
typedef StringBase<utf16_t> String_utf16;
typedef StringBase<utf32_t> String_utf32;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specializations for ArgType

template <>
class ArgType<String_utf8>
{
public:
	typedef const StringRef_utf8& Type;
};

template <>
class ArgType<String_utf16>
{
public:
	typedef const StringRef_utf16& Type;
};

template <>
class ArgType<String_utf32>
{
public:
	typedef const StringRef_utf32& Type;
};


//..............................................................................

inline
String
formatString_va(
	const char* formatString,
	axl_va_list va
	)
{
	String string;
	string.format_va(formatString, va);
	return string;
}

inline
String
formatString(
	const char* formatString,
	...
	)
{
	AXL_VA_DECL(va, formatString);
	return formatString_va(formatString, va);
}

inline
String_w
formatString_w_va(
	const wchar_t* formatString,
	axl_va_list va
	)
{
	String_w string;
	string.format_va(formatString, va);
	return string;
}

inline
String_w
formatString_w(
	const wchar_t* formatString,
	...
	)
{
	AXL_VA_DECL(va, formatString);
	return formatString_w_va(formatString, va);
}

//..............................................................................

} // namespace sl
} // namespace axl
