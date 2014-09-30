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
class CArrayDetailsT
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
class CSimpleArrayDetailsT
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
class CArrayDetailsT <char>: public CSimpleArrayDetailsT <char>
{
};

template <>
class CArrayDetailsT <wchar_t>: public CSimpleArrayDetailsT <wchar_t>
{
};

template <>
class CArrayDetailsT <float>: public CSimpleArrayDetailsT <float>
{
};

template <>
class CArrayDetailsT <double>: public CSimpleArrayDetailsT <double>
{
};

template <>
class CArrayDetailsT <int8_t>: public CSimpleArrayDetailsT <int8_t>
{
};

template <>
class CArrayDetailsT <uint8_t>: public CSimpleArrayDetailsT <uint8_t>
{
};

template <>
class CArrayDetailsT <int16_t>: public CSimpleArrayDetailsT <int16_t>
{
};

template <>
class CArrayDetailsT <uint16_t>: public CSimpleArrayDetailsT <uint16_t>
{
};

template <>
class CArrayDetailsT <int32_t>: public CSimpleArrayDetailsT <int32_t>
{
};

template <>
class CArrayDetailsT <uint32_t>: public CSimpleArrayDetailsT <uint32_t>
{
};

template <>
class CArrayDetailsT <int64_t>: public CSimpleArrayDetailsT <int64_t>
{
};

template <>
class CArrayDetailsT <uint64_t>: public CSimpleArrayDetailsT <uint64_t>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for pointers

template <typename T>
class CArrayDetailsT <T*>: public CSimpleArrayDetailsT <T*>
{
};

//.............................................................................

template <
	typename T,
	typename TDetails = CArrayDetailsT <T>
	>
class CArrayT
{
public:
	typedef TDetails CDetails;
	
	class CHdr: public ref::CRefCount
	{
	public:
		size_t m_maxCount;
		size_t m_count;

		~CHdr ()
		{
			CDetails::destruct ((T*) (this + 1), m_count);
		}
	};

protected:
	T* m_p;

public:
	CArrayT ()
	{ 
		m_p = NULL;
	}

	CArrayT (T e)
	{ 
		m_p = NULL;
		copy (&e, 1);
	}

	CArrayT (const CArrayT& src)
	{   
		m_p = NULL;
		copy (src); 
	}

	CArrayT (
		const T* p, 
		size_t count
		)
	{ 
		m_p = NULL;
		copy (p, count); 
	}

	CArrayT (
		ref::EBuf bufKind,
		void* p, 
		size_t size
		)
	{
		m_p = NULL;
		setBuffer (bufKind, p, size);
	}

	~CArrayT ()
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

	CArrayT& 
	operator = (const CArrayT& src)
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
	copy (const CArrayT& src)
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

		CDetails::copy (m_p, p, count);
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
	append (const CArrayT& src)
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
			CDetails::copy (dst + count, dst, oldCount - index);

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
			CDetails::copy (dst, p, count);
		else
			CDetails::clear (dst, count);

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
		const CArrayT& src
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
		CDetails::copy (dst, dst + count, newCount - index);

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
			err::setError (err::EStatus_InvalidParameter);
			return false;
		}

		T* temp = (T*) AXL_MEM_ALLOC (count * sizeof (T));
		if (!temp)
			return false;

		T* dst = m_p + indexDst;
		T* src = m_p + indexSrc;

		CDetails::constructCopy (temp, src, count);

		if (indexSrc < indexDst)
			CDetails::copy (src, src + count, indexDst - indexSrc);
		else
			CDetails::copy (dst + count, dst, indexSrc - indexDst);

		CDetails::copy (dst, temp, count);

		CDetails::destruct (temp, count);
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

		ref::CPtrT<CHdr> newHdr = AXL_REF_NEW_EXTRA (CHdr, size);
		if (!newHdr)
			return false;

		size_t oldCount = getCount ();

		newHdr->m_count = oldCount;
		newHdr->m_maxCount = maxCount;

		T* p = (T*) (newHdr + 1);

		if (oldCount)
		{
			CDetails::constructCopy (p, m_p, oldCount);
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
		CHdr* oldHdr = getHdr ();

		if (oldHdr && oldHdr->getRefCount () == 1)
		{
			if (oldHdr->m_count == count)
				return true;

			if (oldHdr->m_maxCount >= count)
			{
				size_t oldCount = oldHdr->m_count;
				if (count > oldCount)
					CDetails::construct (m_p + oldCount, count - oldCount);
				else
					CDetails::destruct (m_p + count, oldCount - count);

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

			CDetails::construct (m_p, count);
			getHdr ()->m_count = count;
			return true;
		}

		size_t maxCount = rtl::getMinPower2Gt (count); // make a room
		size_t size = maxCount * sizeof (T);

		ref::CPtrT<CHdr> newHdr = AXL_REF_NEW_EXTRA (CHdr, size);
		if (!newHdr)
			return false;

		newHdr->m_count = count;
		newHdr->m_maxCount = maxCount;

		T* p = (T*) (newHdr + 1);

		if (count <= oldHdr->m_count)
		{
			CDetails::constructCopy (p, m_p, count);
		}
		else
		{
			CDetails::constructCopy (p, m_p, oldHdr->m_count);
			CDetails::construct (p + oldHdr->m_count, count - oldHdr->m_count);
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
		ref::EBuf bufKind,
		void* p,
		size_t size
		)
	{
		ASSERT (size >= sizeof (CHdr) + sizeof (T));

		CHdr* oldHdr = getHdr ();
		
		mem::FFree* pfFree = bufKind == ref::EBuf_Static ? NULL : (mem::FFree*) -1;
		ref::CPtrT <CHdr> newHdr = AXL_REF_NEW_INPLACE (CHdr, p, pfFree);
		newHdr->m_count = 0;
		newHdr->m_maxCount = (size - sizeof (CHdr)) / sizeof (T);

		if (oldHdr)
			oldHdr->release ();

		m_p = (T*) (newHdr + 1);

		newHdr.detach ();
	}

protected:
	CHdr*
	getHdr () const
	{
		return m_p ? (CHdr*) m_p - 1 : NULL;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
