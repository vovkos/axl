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
#include "axl_sys_TlsSlot.h"

namespace axl {
namespace mem {

//..............................................................................

template <typename T>
class Pool {
protected:
	sl::List<T> m_freeList;

public:
	void
	clear() {
		m_freeList.clear();
	}

	T*
	get() {
		return !m_freeList.isEmpty() ? m_freeList.removeHead() : new T;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	T*
	get(T&& src) {
		T* p = get();
		*p = std::move(src);
		return p;
	}
#endif

	T*
	get(const T& src) {
		T* p = get();
		*p = src;
		return p;
	}

	void
	put(T* p) {
		putFront(p); // by default, use stack rather than queue semantics
	}

	void
	put(sl::List<T>* list) {
		putFront(list);
	}

	void
	putFront(T* p) {
		m_freeList.insertHead(p);
	}

	void
	putBack(T* p) {
		m_freeList.insertTail(p);
	}

	void
	putFront(sl::List<T>* list) {
		m_freeList.insertListHead(list);
	}

	void
	putBack(sl::List<T>* list) {
		m_freeList.insertListTail(list);
	}

	bool
	reserve(size_t count) {
		size_t freeCount = m_freeList.getCount();
		if (count <= freeCount)
			return true;

		count -= freeCount;
		for (size_t i = 0; i < count; i++) {
			T* p = new (std::nothrow_t) T;
			if (!p)
				return false;

			m_freeList.insertTail(p);
		}

		return true;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
Pool<T>*
getCurrentThreadPool() {
	Pool<T>* pool = sys::getTlsPtrSlotValue<Pool<T> >();
	if (pool)
		return pool;

	rc::Ptr<Pool<T> > newPool = AXL_RC_NEW(rc::Box<Pool<T> >);
	sys::setTlsPtrSlotValue<Pool<T> >(newPool);
	return newPool;
}

//..............................................................................

} // namespace mem
} // namespace axl
