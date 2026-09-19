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

#define _AXL_RC_REFCOUNT_H

#include "axl_sys_Atomic.h"

namespace axl {
namespace rc {

// RefCount decouples destruction and freeing -- this is done to allow overriding allocation at run-time.

// RefCount features:
// -weak references and strong references
// -using different allocators for creating ref-counted object
// -creating ref-counted objects on member fields buffers (be sure to make container non-copiable)
// -creating ref-counted objects on static/stack buffers

//..............................................................................

typedef
void
FreeFunc(void* p);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class RefCount {
	AXL_DISABLE_COPY(RefCount)

protected:
	FreeFunc* m_freeFunc;
	volatile int32_t m_refCount;
	volatile int32_t m_weakRefCount;

public:
	RefCount();

	virtual
	~RefCount() {
		ASSERT(m_refCount == 0 && m_weakRefCount > 0);
	}

	void
	prime(FreeFunc* freeFunc) {
		ASSERT(m_refCount == 0); // should only be called once in the very beginning
		m_freeFunc = freeFunc;
	}

	size_t
	getRefCount() const {
		return m_refCount;
	}

	size_t
	getWeakRefCount() const {
		return m_weakRefCount;
	}

	size_t
	incRef() {
		return sys::atomicInc(&m_refCount);
	}

	size_t
	decRef();

	size_t
	incWeakRef() {
		return sys::atomicInc(&m_weakRefCount);
	}

	size_t
	decWeakRef();

	size_t
	incRefByWeakPtr();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
RefCount::RefCount() {
	m_freeFunc = NULL;
	m_refCount = 0;
	m_weakRefCount = 1;
}


inline
size_t
RefCount::decRef() {
	intptr_t refCount = sys::atomicDec(&m_refCount);

	if (!refCount) {
		this->~RefCount();
		decWeakRef(); // decWeakRef () should be here, not in ~RefCount ()
	}

	return refCount;
}

inline
size_t
RefCount::decWeakRef() {
	intptr_t refCount = sys::atomicDec(&m_weakRefCount);
	if (!refCount && m_freeFunc)
		m_freeFunc(this);

	return refCount;
}

inline
size_t
RefCount::incRefByWeakPtr() {
	for (;;) {
		int32_t old = m_refCount;
		if (old == 0)
			return 0;

		if (sys::atomicCmpXchg(&m_refCount, old, old + 1) == old)
			return old + 1;
	}
}

//..............................................................................

template <typename T>
class Box:
	public RefCount,
	public T {
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class DecRef {
public:
	void
	operator () (T* p) const {
		p->decRef();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class DecWeakRef {
public:
	void
	operator () (T* p) const {
		p->decWeakRef();
	}
};

//..............................................................................

// forwards

template <typename T>
class Ptr;

template <typename T>
class WeakPtr;

AXL_SELECT_ANY struct NullPtr {} g_nullPtr;

//..............................................................................

} // namespace rc
} // namespace axl
