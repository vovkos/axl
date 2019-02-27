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

#define _AXL_SL_ITERATOR_H

#include "axl_sl_Operator.h"

namespace axl {
namespace sl {

//..............................................................................

struct ListLink
{
	template <
		typename T,
		typename Entry,
		typename Link,
		typename GetLink
		>
	friend class IteratorBase;

	template <
		typename T,
		typename GetLink,
		typename Iterator,
		typename ConstIterator,
		typename Delete
		>
	friend class ListBase;

protected:
	ListLink* m_next;
	ListLink* m_prev;

public:
	ListLink*
	getNext()
	{
		return m_next;
	}

	ListLink*
	getPrev()
	{
		return m_prev;
	}
};

//..............................................................................

template <
	typename T,
	typename Entry0,
	typename Link0,
	typename GetLink0
	>
class IteratorBase
{
public:
	typedef Entry0 Entry;
	typedef Link0 Link;
	typedef GetLink0 GetLink;

protected:
	Entry* m_p;

public:
	IteratorBase()
	{
		m_p = NULL;
	}

	// operator bool () might seem more logical, but then '+' and '-' will not work

	operator const void* () const
	{
		return m_p;
	}

	T&
	operator ++ ()
	{
		return next();
	}

	T&
	operator -- ()
	{
		return prev();
	}

	T
	operator ++ (int) // post increment
	{
		T old = *(T*)this;
		next();
		return old;
	}

	T
	operator -- (int) // post decrement
	{
		T old = *(T*)this;
		prev();
		return old;
	}

	T&
	operator += (size_t count)
	{
		return inc(count);
	}

	T&
	operator -= (size_t count)
	{
		return inc(-count);
	}

	T
	operator + (size_t count) const
	{
		return getInc(count);
	}

	T
	operator - (size_t count) const
	{
		return getInc(-count);
	}

	T&
	next()
	{
		if (m_p)
			m_p = getEntryFromLink(getLink()->m_next);

		return *(T*)this;
	}

	T&
	prev()
	{
		if (m_p)
			m_p = getEntryFromLink(getLink()->m_prev);

		return *(T*)this;
	}

	T&
	inc(intptr_t count)
	{
		Link* link = getLink();

		if (count > 0)
			for (intptr_t i = 0; i < count && link; i++)
				link = link->m_next;
		else
			for (intptr_t i = 0; i > count && link; i--)
				link = link->m_prev;

		m_p = getEntryFromLink(link);
		return *(T*)this;
	}

	T
	getNext() const
	{
		return IteratorBase(*this).next();
	}

	T
	getPrev() const
	{
		return IteratorBase(*this).prev();
	}

	T
	getInc(intptr_t count) const
	{
		return IteratorBase(*this).inc(count);
	}

	Entry*
	getEntry() const
	{
		return m_p;
	}

	Link*
	getLink() const
	{
		return m_p ? GetLink() (m_p) : NULL;
	}

	static
	Entry*
	getEntryFromLink(Link* p)
	{
		return p ? (Entry*)((char*)p  + 1 - (size_t)GetLink() ((Entry*) 1)) : NULL;
	}

	static
	Link*
	getLinkFromEntry(Entry* p)
	{
		return p ? GetLink() (p) : NULL;
	}

	static
	T
	fromLink(Link* p)
	{
		T it;
		it.m_p = getEntryFromLink(p);
		return it;
	}
};

//..............................................................................

template <
	typename T,
	typename Entry,
	typename Link,
	typename GetLink = ImplicitPtrCast<T, ListLink>
	>
class IteratorImpl: public IteratorBase<
	T,
	Entry,
	Link,
	GetLink
	>
{
public:
	Entry*
	operator * () const
	{
		return this->m_p;
	}

	Entry*
	operator -> () const
	{
		return this->m_p;
	}

	T&
	operator = (Entry* p)
	{
		this->m_p = p;
		return *this;
	}

	bool
	operator == (Entry* p) const
	{
		return this->m_p == p;
	}

	bool
	operator != (Entry* p) const
	{
		return !operator == (p);
	}

	Entry* p() const
	{
		return this->m_p;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink = ImplicitPtrCast<T, ListLink>
	>
class Iterator: public IteratorImpl<
	Iterator<T, GetLink>,
	T,
	ListLink,
	GetLink
	>
{
public:
	Iterator()
	{
	}

	template <
		typename T2,
		typename GetLink2
		>
	Iterator(const Iterator<T2, GetLink2>& src)
	{
		this->operator = (*src);
	}

	Iterator(T* p)
	{
		this->m_p = p;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename GetLink = ImplicitPtrCast<T, ListLink>
	>
class ConstIterator: public IteratorImpl<
	ConstIterator<T, GetLink>,
	const T,
	const ListLink,
	GetLink
	>
{
public:
	ConstIterator()
	{
	}

	template <
		typename T2,
		typename GetLink2
		>
	ConstIterator(const Iterator<T2, GetLink2>& src)
	{
		this->operator = (*src);
	}

	template <
		typename T2,
		typename GetLink2
		>
	ConstIterator(const ConstIterator<T2, GetLink2>& src)
	{
		this->operator = (*src);
	}

	ConstIterator(const T* p)
	{
		this->m_p = p;
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
