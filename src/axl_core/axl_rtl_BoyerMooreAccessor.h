#pragma once

#include "axl_rtl_BoyerMooreFind.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename T>
class BoyerMooreAccessor
{
protected:
	const T* m_p;

public:
	BoyerMooreAccessor (const T* p)
	{
		m_p = p;
	}

	T
	getChar (size_t i) const
	{
		return m_p [i];
	}

	void
	saveTail (
		size_t i,
		size_t size
		) const
	{
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class BoyerMooreReverseAccessor: public BoyerMooreAccessor <T>
{
public:
	BoyerMooreReverseAccessor (const T* p)
		: BoyerMooreAccessor <T>  (p)
	{
	}

	T
	getChar (size_t i) const
	{
		return m_p [-i];
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class BoyerMooreIncrementalAccessor: public BoyerMooreAccessor <T>
{
protected:
	BoyerMooreIncrementalContext <T>* m_incrementalContext;

public:
	BoyerMooreIncrementalAccessor (
		const T* p,
		BoyerMooreIncrementalContext <T>* incrementalContext
		): BoyerMooreAccessor <T>  (p)
	{
		m_incrementalContext = incrementalContext;
	}

	T
	getChar (size_t i) const
	{
		size_t tailLength = m_incrementalContext->m_tail.getCount ();
		return i < tailLength ? m_incrementalContext->m_tail [i] : m_p [i - tailLength];
	}

	void
	saveTail (
		size_t i,
		size_t size
		) const
	{
		m_incrementalContext->m_offset += i;

		if (!size)
		{
			m_incrementalContext->m_tail.clear ();
			return;
		}

		size_t tailLength = m_incrementalContext->m_tail.getCount ();
		if (i < tailLength)
		{
			ASSERT (size > tailLength - i);

			size_t copySize = size - (tailLength - i);
			m_incrementalContext->m_tail.remove (0, i);
			m_incrementalContext->m_tail.append (m_p, copySize);
		}
		else
		{
			m_incrementalContext->m_tail.copy (m_p + i - tailLength, size);
		}
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class BoyerMooreIncrementalReverseAccessor: public BoyerMooreIncrementalAccessor <T>
{
public:
	BoyerMooreIncrementalReverseAccessor (
		const T* p,
		BoyerMooreIncrementalContext <T>* incrementalContext
		): BoyerMooreIncrementalAccessor <T> (p, incrementalContext)
	{
	}

	T
	getChar (size_t i) const
	{
		size_t tailLength = m_incrementalContext->m_tail.getCount ();
		return i < tailLength ? m_incrementalContext->m_tail [i] : m_p [-(i - tailLength)]; // tail is already reversed
	}

	void
	saveTail (
		size_t i,
		size_t size
		) const
	{
		m_incrementalContext->m_offset += i;

		if (!size)
		{
			m_incrementalContext->m_tail.clear ();
			return;
		}

		size_t tailLength = m_incrementalContext->m_tail.getCount ();
		if (i < tailLength)
		{
			ASSERT (size > tailLength - i);
		
			size_t copySize = size - (tailLength - i);
			m_incrementalContext->m_tail.remove (0, i);
			m_incrementalContext->m_tail.appendReverse (m_p - copySize + 1, copySize);
		}
		else
		{
			m_incrementalContext->m_tail.copyReverse (m_p - size + i - tailLength + 1, size);
		}
	}
};

//.............................................................................

template <typename Base>
class TextBoyerMooreCaseFoldAccessorImpl: public Base
{
public:
	TextBoyerMooreCaseFoldAccessorImpl (const utf32_t* p):
		Base (p)			
	{
	}

	utf32_t
	getChar (size_t i) const
	{
		return utfToCaseFold (Base::getChar (i));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Base>
class TextBoyerMooreCaseFoldIncrementalAccessorImpl: public Base
{
public:
	TextBoyerMooreCaseFoldIncrementalAccessorImpl (
		const utf32_t* p,
		BoyerMooreIncrementalContext <utf32_t>* incrementalContext
		): Base  (p, incrementalContext)
	{
	}

	utf32_t
	getChar (size_t i) const
	{
		return utfToCaseFold (Base::getChar (i));
	}
};

//.............................................................................

typedef BoyerMooreAccessor <char> BinaryBoyerMooreAccessor;
typedef BoyerMooreReverseAccessor <char> BinaryBoyerMooreReverseAccessor;
typedef BoyerMooreIncrementalAccessor <char> BinaryBoyerMooreIncrementalAccessor;
typedef BoyerMooreIncrementalReverseAccessor <char> BinaryBoyerMooreIncrementalReverseAccessor;

typedef BoyerMooreAccessor <utf32_t> TextBoyerMooreAccessor;
typedef BoyerMooreReverseAccessor <utf32_t> TextBoyerMooreReverseAccessor;
typedef BoyerMooreIncrementalAccessor <utf32_t> TextBoyerMooreIncrementalAccessor;
typedef BoyerMooreIncrementalReverseAccessor <utf32_t> TextBoyerMooreIncrementalReverseAccessor;

typedef 
TextBoyerMooreCaseFoldAccessorImpl <BoyerMooreAccessor <utf32_t> >
TextBoyerMooreCaseFoldAccessor;

typedef 
TextBoyerMooreCaseFoldAccessorImpl <BoyerMooreReverseAccessor <utf32_t> >
TextBoyerMooreCaseFoldReverseAccessor;

typedef 
TextBoyerMooreCaseFoldIncrementalAccessorImpl <BoyerMooreIncrementalAccessor <utf32_t> >
TextBoyerMooreCaseFoldIncrementalAccessor;

typedef 
TextBoyerMooreCaseFoldIncrementalAccessorImpl <BoyerMooreIncrementalReverseAccessor <utf32_t> >
TextBoyerMooreCaseFoldIncrementalReverseAccessor;

//.............................................................................

} // namespace gc
} // namespace axl
