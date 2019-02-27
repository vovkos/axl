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

#define _AXL_MEM_POOL_H

#include "axl_sl_List.h"

namespace axl {
namespace mem {

//..............................................................................

template <typename T>
class Pool
{
protected:
	sl::List<T> m_freeList;

public:
	T*
	get()
	{
		return !m_freeList.isEmpty() ? m_freeList.removeHead() : AXL_MEM_NEW(T);
	}

	void
	put(T* p)
	{
		putFront(p); // by default, use stack rather than queue semantics
	}

	void
	putFront(T* p)
	{
		m_freeList.insertHead(p);
	}

	void
	putBack(T* p)
	{
		m_freeList.insertTail(p);
	}

	void
	put(sl::List<T>* list)
	{
		m_freeList.insertListTail(list);
	}

	bool
	reserve(size_t count)
	{
		size_t freeCount = m_freeList.getCount();
		if (count <= freeCount)
			return true;

		count -= freeCount;
		for (size_t i = 0; i < count; i++)
		{
			T* p = AXL_MEM_NEW(T);
			if (!p)
				return false;

			m_freeList.insertTail(p);
		}

		return true;
	}
};

//..............................................................................

} // namespace mem
} // namespace axl
