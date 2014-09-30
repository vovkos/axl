// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_AUTOPTRARRAY_H

#include "axl_rtl_Array.h"
#include "axl_rtl_Func.h"
#include "axl_mem_Factory.h"

namespace axl {
namespace rtl {

//.............................................................................

template <
	typename T,
	typename TDelete
	>
class CAutoPtrArrayDetailsT: public CSimpleArrayDetailsT <T*>
{
public:
	static 
	void 
	destruct (
		T** p, 
		size_t count
		)
	{ 
		T** end = p + count;
		for (; p < end; p++)
			if (*p)
				TDelete () (*p);
	}

	static 
	void 
	copy (
		T** dst, 
		T* const* src, 
		size_t count
		)
	{ 
		if (src > dst || src + count <= dst)
		{
			T** end = dst + count;
			for (; dst < end; dst++, src++)
			{
				if (*dst)
					TDelete () (*dst);

				*dst = *src;
			}
		}
		else
		{
			T** end = dst;

			dst += count;
			src += count;

			while (dst > end)
			{
				dst--;
				src--;

				if (*dst)
					TDelete () (*dst);

				*dst = *src;
			}
		}
	}

	static 
	void 
	clear (
		T** p, 
		size_t count
		)
	{
		T** begin = p;
		T** end = p + count;

		for (; p < end; p++)
			if (*p)
				TDelete () (*p);
		
		memset (begin, 0, count * sizeof (T)); 
	}	
};

//.............................................................................

template <
	typename T,
	typename TDelete = mem::CStdFactoryT <T>::COperatorDelete
	>
class CAutoPtrArrayT: public rtl::CArrayT <T*, CAutoPtrArrayDetailsT <T, TDelete> >
{
public:
	typedef rtl::CArrayT <T*, CAutoPtrArrayDetailsT <T, TDelete> > CBaseType;

public:
	CAutoPtrArrayT ()
	{ 
	}

	CAutoPtrArrayT (T* e):
		CBaseType (e)
	{
	}

	CAutoPtrArrayT (
		T* const* p, 
		size_t count
		):
		CBaseType (p, count)
	{ 
	}

	CAutoPtrArrayT (
		ref::EBuf bufKind,
		void* p, 
		size_t size
		):
		CBaseType (bufKind, p, size)
	{
	}

	operator const T** () const
	{ 
		return m_p; 
	}

	operator T** () 
	{ 
		return m_p; 
	}

	// .ca () is mostly for passing through vararg
	// thanks a lot gcc
	
	const T** 
	ca () const
	{ 
		return m_p; 
	}

	T** 
	a () 
	{ 
		return m_p; 
	}

	void
	takeOver (CAutoPtrArrayT* src)
	{
		release ();
		m_p = src;
		src->m_p = NULL;
	}

	bool 
	copy (
		T* const* p, 
		size_t count
		)
	{
		return CBaseType::copy (p, count);
	}

	bool 
	copy (T* e)
	{
		return CBaseType::copy (e);
	}

	T** 
	append (
		T* const* p, 
		size_t count
		)
	{ 
		return CBaseType::append (p, count);
	}

	T**
	append (T* e)
	{ 
		return CBaseType::append (e);
	}

	T**
	insert (
		size_t index, 
		T* const* p, 
		size_t count
		)
	{
		return CBaseType::insert (index, p, count);
	}

	T**
	insert (
		size_t index, 
		T* e
		)
	{
		return CBaseType::insert (index, e);
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

	T* 
	getBackAndPop ()
	{
		T* e = getBack ();
		pop ();
		return e;
	}

private:
	CAutoPtrArrayT (const CAutoPtrArrayT& src)
	{   
	}

	void
	operator = (const CAutoPtrArrayT& src)
	{ 
	}

	T**
	appendMultiply (
		T* e,
		size_t count
		)
	{ 
	}

	T**
	insertMultiply (
		size_t index, 
		T* e, 
		size_t count
		)
	{
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
