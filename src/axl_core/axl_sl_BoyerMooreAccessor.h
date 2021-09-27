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

#include "axl_sl_BoyerMooreFind.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class BoyerMooreAccessor {
protected:
	const T* m_p;

public:
	BoyerMooreAccessor(const T* p) {
		m_p = p;
	}

	T
	getChar(size_t i) const {
		return m_p[i];
	}

	void
	saveTail(
		size_t i,
		size_t size
	) const {}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class BoyerMooreReverseAccessor: public BoyerMooreAccessor<T> {
public:
	BoyerMooreReverseAccessor(const T* p)
		: BoyerMooreAccessor<T>  (p) {}

	T
	getChar(size_t i) const {
		return this->m_p[-i];
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class BoyerMooreIncrementalAccessor: public BoyerMooreAccessor<T> {
protected:
	BoyerMooreIncrementalContext<T>* m_incrementalContext;

public:
	BoyerMooreIncrementalAccessor(
		const T* p,
		BoyerMooreIncrementalContext<T>* incrementalContext
	): BoyerMooreAccessor<T>  (p) {
		m_incrementalContext = incrementalContext;
	}

	T
	getChar(size_t i) const {
		size_t tailLength = m_incrementalContext->m_tail.getCount();
		return i < tailLength ?
			this->m_incrementalContext->m_tail[i] :
			this->m_p[i - tailLength];
	}

	void
	saveTail(
		size_t i,
		size_t size
	) const {
		if (!size) {
			m_incrementalContext->m_tail.clear();
			return;
		}

		size_t tailLength = m_incrementalContext->m_tail.getCount();
		if (i < tailLength) {
			ASSERT(size > tailLength - i);

			size_t copySize = size - (tailLength - i);
			m_incrementalContext->m_tail.remove(0, i);
			m_incrementalContext->m_tail.append(this->m_p, copySize);
		} else {
			m_incrementalContext->m_tail.copy(this->m_p + i - tailLength, size);
		}
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class BoyerMooreIncrementalReverseAccessor: public BoyerMooreIncrementalAccessor<T> {
public:
	BoyerMooreIncrementalReverseAccessor(
		const T* p,
		BoyerMooreIncrementalContext<T>* incrementalContext
	): BoyerMooreIncrementalAccessor<T> (p, incrementalContext) {}

	T
	getChar(size_t i) const {
		size_t tailLength = this->m_incrementalContext->m_tail.getCount();
		return i < tailLength ?
			this->m_incrementalContext->m_tail[i] :
			this->m_p[-(i - tailLength)]; // tail is already reversed
	}

	void
	saveTail(
		size_t i,
		size_t size
	) const {
		if (!size) {
			this->m_incrementalContext->m_tail.clear();
			return;
		}

		size_t tailLength = this->m_incrementalContext->m_tail.getCount();
		if (i < tailLength) {
			ASSERT(size > tailLength - i);

			size_t copySize = size - (tailLength - i);
			this->m_incrementalContext->m_tail.remove(0, i);
			this->m_incrementalContext->m_tail.appendReverse(this->m_p - copySize + 1, copySize);
		} else {
			this->m_incrementalContext->m_tail.copyReverse(this->m_p - size - (i - tailLength) + 1, size);
		}
	}
};

//..............................................................................

template <typename Base>
class TextBoyerMooreAccessorImpl: public Base {
public:
	TextBoyerMooreAccessorImpl(const utf32_t* p):
		Base(p) {}

	bool
	isDelimChar(size_t i) const {
		return i == -1 ? true : !enc::utfIsLetterOrNumber(Base::getChar(i));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Base>
class TextBoyerMooreIncrementalAccessorImpl: public Base {
public:
	TextBoyerMooreIncrementalAccessorImpl(
		const utf32_t* p,
		BoyerMooreIncrementalContext<utf32_t>* incrementalContext
	): Base  (p, incrementalContext) {}

	bool
	isDelimChar(size_t i) const {
		utf32_t c = i != -1 ? Base::getChar(i) : this->m_incrementalContext->m_prefix;
		return !enc::utfIsLetterOrNumber(c);
	}

	void
	saveTail(
		size_t i,
		size_t size
	) const {
		ASSERT(i);
		this->m_incrementalContext->m_prefix = Base::getChar(i - 1);
		Base::saveTail(i, size);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Base>
class TextBoyerMooreCaseFoldedAccessorImpl: public TextBoyerMooreAccessorImpl<Base> {
public:
	TextBoyerMooreCaseFoldedAccessorImpl(const utf32_t* p):
		TextBoyerMooreAccessorImpl<Base> (p) {}

	utf32_t
	getChar(size_t i) const {
		return enc::utfToCaseFolded(Base::getChar(i));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Base>
class TextBoyerMooreCaseFoldedIncrementalAccessorImpl: public TextBoyerMooreIncrementalAccessorImpl<Base> {
public:
	TextBoyerMooreCaseFoldedIncrementalAccessorImpl(
		const utf32_t* p,
		BoyerMooreIncrementalContext<utf32_t>* incrementalContext
	): TextBoyerMooreIncrementalAccessorImpl<Base> (p, incrementalContext) {}

	utf32_t
	getChar(size_t i) const {
		return enc::utfToCaseFolded(Base::getChar(i));
	}
};

//..............................................................................

typedef BoyerMooreAccessor<uchar_t> BinaryBoyerMooreAccessor;
typedef BoyerMooreReverseAccessor<uchar_t> BinaryBoyerMooreReverseAccessor;
typedef BoyerMooreIncrementalAccessor<uchar_t> BinaryBoyerMooreIncrementalAccessor;
typedef BoyerMooreIncrementalReverseAccessor<uchar_t> BinaryBoyerMooreIncrementalReverseAccessor;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef
TextBoyerMooreAccessorImpl<BoyerMooreAccessor<utf32_t> >
TextBoyerMooreAccessor;

typedef
TextBoyerMooreAccessorImpl<BoyerMooreReverseAccessor<utf32_t> >
TextBoyerMooreReverseAccessor;

typedef
TextBoyerMooreIncrementalAccessorImpl<BoyerMooreIncrementalAccessor<utf32_t> >
TextBoyerMooreIncrementalAccessor;

typedef
TextBoyerMooreIncrementalAccessorImpl<BoyerMooreIncrementalReverseAccessor<utf32_t> >
TextBoyerMooreIncrementalReverseAccessor;

typedef
TextBoyerMooreCaseFoldedAccessorImpl<BoyerMooreAccessor<utf32_t> >
TextBoyerMooreCaseFoldedAccessor;

typedef
TextBoyerMooreCaseFoldedAccessorImpl<BoyerMooreReverseAccessor<utf32_t> >
TextBoyerMooreCaseFoldedReverseAccessor;

typedef
TextBoyerMooreCaseFoldedIncrementalAccessorImpl<BoyerMooreIncrementalAccessor<utf32_t> >
TextBoyerMooreCaseFoldedIncrementalAccessor;

typedef
TextBoyerMooreCaseFoldedIncrementalAccessorImpl<BoyerMooreIncrementalReverseAccessor<utf32_t> >
TextBoyerMooreCaseFoldedIncrementalReverseAccessor;

//..............................................................................

} // namespace gc
} // namespace axl
