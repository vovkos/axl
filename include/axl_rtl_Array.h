// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_ARRAY_H

#include "axl_err_Error.h"

namespace axl {
namespace rtl {

//.............................................................................

// general case: full cycle of construction, copy, destruction

template <typename T>
class ArrayDetails
{
public:
	static 
	void 
	construct (
		T* p, 
		size_t count
		)
	{ 
		memset (p, 0, count * sizeof (T)); // zero memory before construction

		T* end = p + count;
		for (; p < end; p++)
			new (p) T;
	}

	static 
	void 
	constructCopy (
		T* dst, 
		const T* src, 
		size_t count
		)
	{ 
		memset (dst, 0, count * sizeof (T)); // zero memory before construction

		T* end = dst + count;
		for (; dst < end; dst++, src++)
			new (dst) T (*src);
	}

	static 
	void 
	destruct (
		T* p, 
		size_t count
		)
	{ 
		T* end = p + count;
		for (; p < end; p++)
			p->~T ();
	}

	static 
	void 
	copy (
		T* dst, 
		const T* src, 
		size_t count
		)
	{ 
		if (src > dst || src + count <= dst)
		{
			T* end = dst + count;
			for (; dst < end; dst++, src++)
				*dst = *src;
		}
		else
		{
			T* end = dst;

			dst += count;
			src += count;

			while (dst > end)
			{
				dst--;
				src--;
				*dst = *src;
			}
		}
	}

	static 
	void 
	clear (
		T* p, 
		size_t count
		)
	{
		T* begin = p;
		T* end = p + count;

		for (; p < end; p++)
			p->~T ();
		
		p = begin; 
		memset (p, 0, count * sizeof (T)); 
		
		for (; p < end; p++)
			new (p) T;
	}	
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// fast memory block operations for types that do not need ctor/dtor

template <typename T>
class SimpleArrayDetails
{
public:
	static 
	void 
	construct (
		T* p, 
		size_t count
		)
	{ 
		memset (p, 0, count * sizeof (T)); 
	}

	static 
	void 
	constructCopy (
		T* dst, 
		const T* src, 
		size_t count
		)
	{ 
		memcpy (dst, src, count * sizeof (T)); 
	}

	static 
	void 
	destruct (
		T* p, 
		size_t count
		)
	{
	}

	static 
	void 
	copy (
		T* dst, 
		const T* src, 
		size_t count
		)
	{ 
		memmove (dst, src, count * sizeof (T)); 
	}

	static 
	void 
	clear (
		T* p, 
		size_t count
		)
	{ 
		memset (p, 0, count * sizeof (T)); 
	}	
};

//.............................................................................

// specialization for simple types

template <>
class ArrayDetails <char>: public SimpleArrayDetails <char>
{
};

template <>
class ArrayDetails <wchar_t>: public SimpleArrayDetails <wchar_t>
{
};

template <>
class ArrayDetails <float>: public SimpleArrayDetails <float>
{
};

template <>
class ArrayDetails <double>: public SimpleArrayDetails <double>
{
};

template <>
class ArrayDetails <int8_t>: public SimpleArrayDetails <int8_t>
{
};

template <>
class ArrayDetails <uint8_t>: public SimpleArrayDetails <uint8_t>
{
};

template <>
class ArrayDetails <int16_t>: public SimpleArrayDetails <int16_t>
{
};

template <>
class ArrayDetails <uint16_t>: public SimpleArrayDetails <uint16_t>
{
};

template <>
class ArrayDetails <int32_t>: public SimpleArrayDetails <int32_t>
{
};

template <>
class ArrayDetails <uint32_t>: public SimpleArrayDetails <uint32_t>
{
};

template <>
class ArrayDetails <int64_t>: public SimpleArrayDetails <int64_t>
{
};

template <>
class ArrayDetails <uint64_t>: public SimpleArrayDetails <uint64_t>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for pointers

template <typename T>
class ArrayDetails <T*>: public SimpleArrayDetails <T*>
{
};

//.............................................................................

template <
	typename T,
	typename Details = ArrayDetails <T>
	>
class Array
{
public:
	typedef Details Details;
	
	class Hdr: public ref::RefCount
	{
	public:
		size_t m_maxCount;
		size_t m_count;

		~Hdr ()
		{
			Details::destruct ((T*) (this + 1), m_count);
		}
	};

protected:
	T* m_p;

public:
	Array ()
	{ 
		m_p = NULL;
	}

	Array (T e)
	{ 
		m_p = NULL;
		copy (&e, 1);
	}

	Array (const Array& src)
	{   
		m_p = NULL;
		copy (src); 
	}

	Array (
		const T* p, 
		size_t count
		)
	{ 
		m_p = NULL;
		copy (p, count); 
	}

	Array (
		ref::BufKind bufKind,
		void* p, 
		size_t size
		)
	{
		m_p = NULL;
		setBuffer (bufKind, p, size);
	}

	~Array ()
	{
		release ();
	}

	operator const T* () const
	{ 
		return m_p; 
	}

	operator T* () 
	{ 
		return m_p; 
	}

	Array& 
	operator = (const Array& src)
	{ 
		copy (src);
		return *this;
	}

	// .ca () is mostly for passing through vararg
	// thanks a lot gcc
	
	const T* 
	ca () const
	{ 
		return m_p; 
	}

	T* 
	a () 
	{ 
		return m_p; 
	}
	
	size_t 
	getCount () const
	{ 
		return m_p ? getHdr ()->m_count : 0; 
	}

	size_t 
	getReservedCount () const
	{ 
		return m_p ? getHdr ()->m_maxCount : 0; 
	}

	bool 
	isEmpty () const
	{ 
		return getCount () == 0; 
	}

	const T&
	getFront () const
	{
		ASSERT (!isEmpty ());
		return m_p [0];
	}

	T&
	getFront () 
	{
		ASSERT (!isEmpty ());
		return m_p [0];
	}

	const T&
	getBack () const
	{
		ASSERT (!isEmpty ());
		return m_p [getCount () - 1];
	}

	T&
	getBack () 
	{
		ASSERT (!isEmpty ());
		return m_p [getCount () - 1];
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
		setCount (0); 
	}

	bool
	copy (const Array& src)
	{ 
		if (m_p == src.m_p)
			return true;

		if (!src.m_p)
		{
			release ();
			return true; 
		}

		if (src.getHdr ()->getFree () == (mem::FFree*) -1)
			return copy (src, src.getCount ()); 

		if (src.m_p)
			src.getHdr ()->addRef ();

		if (m_p)
			getHdr ()->release ();

		m_p = src.m_p;
		return true;
	}

	bool 
	copy (
		const T* p, 
		size_t count
		)
	{
		if (count == 0)
		{
			clear ();
			return true;
		}

		bool result = setCount (count);
		if (!result)
			return false;

		Details::copy (m_p, p, count);
		return true;
	}

	bool 
	copy (T e)
	{
		return copy (&e, 1);
	}

	T* 
	append (
		const T* p, 
		size_t count
		)
	{ 
		return insert (-1, p, count); 
	}

	T*
	append (T e)
	{ 
		return insert (-1, e); 
	}

	T*
	appendMultiply (
		T e,
		size_t count
		)
	{ 
		return insertMultiply (-1, e, count); 
	}

	T*
	append (const Array& src)
	{ 
		return insert (-1, src, src.getCount ()); 
	}

	T*
	insertSpace (
		size_t index, 
		size_t count
		)
	{
		size_t oldCount = getCount ();
		bool result = setCount (oldCount + count);
		if (!result)
			return NULL;

		if (index > oldCount)
			index = oldCount;

		T* dst = m_p + index;

		if (count && index < oldCount)
			Details::copy (dst + count, dst, oldCount - index);

		return dst;
	}

	T*
	insert (
		size_t index, 
		const T* p, 
		size_t count
		)
	{
		T* dst = insertSpace (index, count);

		if (p)
			Details::copy (dst, p, count);
		else
			Details::clear (dst, count);

		return dst;
	}

	T*
	insert (
		size_t index, 
		T e
		)
	{
		T* dst = insertSpace (index, 1);
		*dst = e;
		return dst;
	}

	T*
	insertMultiply (
		size_t index, 
		T e, 
		size_t count
		)
	{
		T* dst = insertSpace (index, count);
		T* end = dst + count;

		for (; dst < end; dst++)
			*dst = e;

		return dst;
	}

	T* 
	insert (
		size_t index, 
		const Array& src
		)
	{ 
		return insert (index, src, src.getCount ()); 
	}

	bool 
	remove (
		size_t index, 
		size_t count = 1
		)
	{
		if (count == 0)
			return true;

		size_t oldCount = getCount ();
		if (index >= oldCount)
			return true;

		if (index + count >= oldCount)
			return setCount (index);

		bool result = ensureExclusive ();
		if (!result)
			return false;

		size_t newCount = oldCount - count;

		T* dst = m_p + index;
		Details::copy (dst, dst + count, newCount - index);

		return setCount (newCount);
	}

	size_t
	pop (size_t count = 1)
	{
		size_t oldCount = getCount ();
		if (count >= oldCount)
			count = oldCount;

		setCount (oldCount - count);
		return count;
	}

	T 
	getBackAndPop ()
	{
		T e = getBack ();
		pop ();
		return e;
	}

	bool 
	move (
		size_t indexDst, 
		size_t indexSrc, 
		size_t count = 1
		)
	{
		if (count == 0 || indexDst == indexSrc)
			return true;

		size_t oldCount = getCount ();

		if (indexDst + count > oldCount || indexSrc + count > oldCount)
		{
			err::setError (err::StatusKind_InvalidParameter);
			return false;
		}

		T* temp = (T*) AXL_MEM_ALLOC (count * sizeof (T));
		if (!temp)
			return false;

		T* dst = m_p + indexDst;
		T* src = m_p + indexSrc;

		Details::constructCopy (temp, src, count);

		if (indexSrc < indexDst)
			Details::copy (src, src + count, indexDst - indexSrc);
		else
			Details::copy (dst + count, dst, indexSrc - indexDst);

		Details::copy (dst, temp, count);

		Details::destruct (temp, count);
		AXL_MEM_FREE (temp);

		return true;
	}

	size_t
	find (T e)
	{
		size_t count = getCount ();
		for (size_t i = 0; i < count; i++)
			if (m_p [i] == e)
				return i;

		return -1;
	}

	size_t
	findReverse (T e)
	{
		size_t count = getCount ();
		for (intptr_t i = count - 1; i >= 0; i--)
			if (m_p [i] == e)
				return i;

		return -1;
	}

	T* 
	getBuffer ()
	{ 
		return ensureExclusive () ? m_p : NULL; 
	}

	T* 
	getBuffer (size_t count)
	{ 
		return setCount (count) ? m_p : NULL; 
	}

	bool 
	reserve (size_t count)
	{
		if (count <= getReservedCount ())
			return ensureExclusive ();

		size_t maxCount = rtl::getMinPower2Ge (count);
		size_t size = maxCount * sizeof (T);

		ref::Ptr<Hdr> newHdr = AXL_REF_NEW_EXTRA (Hdr, size);
		if (!newHdr)
			return false;

		size_t oldCount = getCount ();

		newHdr->m_count = oldCount;
		newHdr->m_maxCount = maxCount;

		T* p = (T*) (newHdr + 1);

		if (oldCount)
		{
			Details::constructCopy (p, m_p, oldCount);
			getHdr ()->release ();
		}

		m_p = p;

		newHdr.detach ();
		return true;
	}

	size_t 
	ensureCount (size_t count)
	{
		if (getCount () < count)
			setCount (count);

		return getCount ();
	}

	bool 
	setCount (size_t count)
	{
		Hdr* oldHdr = getHdr ();

		if (oldHdr && oldHdr->getRefCount () == 1)
		{
			if (oldHdr->m_count == count)
				return true;

			if (oldHdr->m_maxCount >= count)
			{
				size_t oldCount = oldHdr->m_count;
				if (count > oldCount)
					Details::construct (m_p + oldCount, count - oldCount);
				else
					Details::destruct (m_p + count, oldCount - count);

				oldHdr->m_count = count;
				return true;
			}
		}

		if (count == 0)
		{
			release ();
			return true;
		}

		if (!oldHdr)
		{
			bool result = reserve (count);
			if (!result)
				return false;

			Details::construct (m_p, count);
			getHdr ()->m_count = count;
			return true;
		}

		size_t maxCount = rtl::getMinPower2Gt (count); // make a room
		size_t size = maxCount * sizeof (T);

		ref::Ptr<Hdr> newHdr = AXL_REF_NEW_EXTRA (Hdr, size);
		if (!newHdr)
			return false;

		newHdr->m_count = count;
		newHdr->m_maxCount = maxCount;

		T* p = (T*) (newHdr + 1);

		if (count <= oldHdr->m_count)
		{
			Details::constructCopy (p, m_p, count);
		}
		else
		{
			Details::constructCopy (p, m_p, oldHdr->m_count);
			Details::construct (p + oldHdr->m_count, count - oldHdr->m_count);
		}
		
		oldHdr->release ();

		m_p = p;

		newHdr.detach ();
		return true;
	}

	bool 
	ensureExclusive ()
	{ 
		return m_p ? setCount (getCount ()) : true; 
	}

	void
	setBuffer (
		ref::BufKind bufKind,
		void* p,
		size_t size
		)
	{
		ASSERT (size >= sizeof (Hdr) + sizeof (T));

		Hdr* oldHdr = getHdr ();
		
		mem::FFree* pfFree = bufKind == ref::BufKind_Static ? NULL : (mem::FFree*) -1;
		ref::Ptr <Hdr> newHdr = AXL_REF_NEW_INPLACE (Hdr, p, pfFree);
		newHdr->m_count = 0;
		newHdr->m_maxCount = (size - sizeof (Hdr)) / sizeof (T);

		if (oldHdr)
			oldHdr->release ();

		m_p = (T*) (newHdr + 1);

		newHdr.detach ();
	}

protected:
	Hdr*
	getHdr () const
	{
		return m_p ? (Hdr*) m_p - 1 : NULL;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
