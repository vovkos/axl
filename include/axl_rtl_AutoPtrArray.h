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
	typename Delete
	>
class AutoPtrArrayDetails: public SimpleArrayDetails <T*>
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
				Delete () (*p);
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
					Delete () (*dst);

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
					Delete () (*dst);

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
				Delete () (*p);
		
		memset (begin, 0, count * sizeof (T)); 
	}	
};

//.............................................................................

template <
	typename T,
	typename Delete = mem::StdFactory <T>::Delete
	>
class AutoPtrArray: public rtl::Array <T*, AutoPtrArrayDetails <T, Delete> >
{
public:
	typedef rtl::Array <T*, AutoPtrArrayDetails <T, Delete> > Baseype;

public:
	AutoPtrArray ()
	{ 
	}

	AutoPtrArray (T* e):
		Baseype (e)
	{
	}

	AutoPtrArray (
		T* const* p, 
		size_t count
		):
		Baseype (p, count)
	{ 
	}

	AutoPtrArray (
		ref::BufKind bufKind,
		void* p, 
		size_t size
		):
		Baseype (bufKind, p, size)
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
	takeOver (AutoPtrArray* src)
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
		return Baseype::copy (p, count);
	}

	bool 
	copy (T* e)
	{
		return Baseype::copy (e);
	}

	T** 
	append (
		T* const* p, 
		size_t count
		)
	{ 
		return Baseype::append (p, count);
	}

	T**
	append (T* e)
	{ 
		return Baseype::append (e);
	}

	T**
	insert (
		size_t index, 
		T* const* p, 
		size_t count
		)
	{
		return Baseype::insert (index, p, count);
	}

	T**
	insert (
		size_t index, 
		T* e
		)
	{
		return Baseype::insert (index, e);
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
	AutoPtrArray (const AutoPtrArray& src)
	{   
	}

	void
	operator = (const AutoPtrArray& src)
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
