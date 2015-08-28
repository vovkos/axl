// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_AUTOPTRARRAY_H

#include "axl_rtl_Array.h"
#include "axl_rtl_Func.h"
#include "axl_mem_New.h"

namespace axl {
namespace rtl {

//.............................................................................

template <
	typename T,
	typename Delete_0 = typename mem::StdDelete <T>
	>
class AutoPtrArray: public Array <T*>
{
	AXL_DISABLE_COPY (AutoPtrArray)

public:
	typedef Array <T*> BaseType;
	typedef Delete_0 Delete;

public:
	AutoPtrArray ():
		BaseType ()
	{
	}

	AutoPtrArray (T* e):
		BaseType (e)
	{
	}

	AutoPtrArray (
		T* const* p,
		size_t count
		):
		BaseType (p, count)
	{
	}

	AutoPtrArray (
		ref::BufKind bufKind,
		void* p,
		size_t size
		):
		BaseType (bufKind, p, size)
	{
	}

	~AutoPtrArray ()
	{
		deleteElements ();
	}

	void
	clear ()
	{
		deleteElements ();
		BaseType::clear ();
	}

	void
	takeOver (AutoPtrArray* src)
	{
		deleteElements ();
		this->release ();
		this->m_p = src->m_p;
		src->m_p = NULL;
	}

protected:
	void
	deleteElements ()
	{
		size_t count = this->getCount ();

		for (size_t i = 0; i < count; i++)
		{
			T* e = this->m_p [i];
			if (e)
				Delete () (e);
		}
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
