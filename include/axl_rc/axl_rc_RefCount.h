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
	getRefCount() {
		return m_refCount;
	}

	size_t
	getWeakRefCount() {
		return m_weakRefCount;
	}

	size_t
	addRef() {
		return sys::atomicInc(&m_refCount);
	}

	size_t
	release();

	size_t
	addWeakRef() {
		return sys::atomicInc(&m_weakRefCount);
	}

	size_t
	weakRelease();

	size_t
	addRefByWeakPtr();
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
RefCount::release() {
	intptr_t refCount = sys::atomicDec(&m_refCount);

	if (!refCount) {
		this->~RefCount();
		weakRelease(); // weakRelease () should be here, not in ~RefCount ()
	}

	return refCount;
}

inline
size_t
RefCount::weakRelease() {
	intptr_t refCount = sys::atomicDec(&m_weakRefCount);
	if (!refCount && m_freeFunc)
		m_freeFunc(this);

	return refCount;
}

inline
size_t
RefCount::addRefByWeakPtr() {
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
class Release {
public:
	void
	operator () (T* p) const {
		p->release();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class WeakRelease {
public:
	void
	operator () (T* p) const {
		p->weakRelease();
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
