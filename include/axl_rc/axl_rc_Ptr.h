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

#define _axl_rc_PTR_H

#include "axl_rc_RefCount.h"

namespace axl {
namespace rc {

//..............................................................................

template <typename T>
class Ptr {
	template <typename T2>
	friend class Ptr;

	template <typename T2>
	friend class WeakPtr;

protected:
	T* m_p;
	RefCount* m_refCount;

public:
	Ptr() {
		initialize();
	}

	Ptr(const NullPtr&) {
		initialize();
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Ptr(Ptr&& src) {
		initialize();
		move(std::move(src));
	}
#endif

	Ptr(const Ptr& src) {
		initialize();
		copy(src);
	}

	template <typename A>
	Ptr(const Ptr<A>& src) {
		initialize();
		copy(src.m_p, src.m_refCount);
	}

	template <typename A>
	Ptr(const WeakPtr<A>& src) {
		initialize();
		if (src.m_refCount && src.m_refCount->addRefByWeakPtr())
			attach(src.m_p, src.m_refCount);
	}

	template <typename A>
	Ptr(A* p) {
		initialize();
		copy(p, p);
	}

	Ptr(
		T* p,
		RefCount* refCount
	) {
		m_p = NULL, m_refCount = NULL;
		copy(p, refCount);
	}

	~Ptr() {
		clear();
	}

	operator T* () const {
		return m_p;
	}

	T*
	operator -> () const {
		ASSERT(m_p);
		return m_p;
	}

	Ptr&
	operator = (const NullPtr&) {
		clear();
		return *this;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Ptr&
	operator = (Ptr&& src) {
		copy(src);
		return *this;
	}
#endif

	Ptr&
	operator = (const Ptr& src) {
		copy(src);
		return *this;
	}

	T*
	p() const {
		return m_p;
	}

	bool
	isExclusive() const {
		ASSERT(m_p);
		return m_p->getRefCount() == 1;
	}

	RefCount*
	getRefCount() const {
		return m_refCount;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	void
	move(Ptr&& src) {
		m_p = src.m_p;
		m_refCount = src.m_refCount;
		src.initialize();
	}
#endif

	void
	copy(const Ptr& src) {
		copy(src.m_p, src.m_refCount);
	}

	void
	copy(
		T* p,
		RefCount* refCount
	) {
		m_p = p;

		if (m_refCount == refCount)
			return;

		if (refCount)
			refCount->addRef();

		if (m_refCount)
			m_refCount->release();

		m_refCount = refCount;
	}

	void
	attach(
		T* p,
		RefCount* refCount
	) {
		if (m_refCount)
			m_refCount->release();

		m_p = p;
		m_refCount = refCount;
	}

	T*
	detach(RefCount** refCount = NULL) {
		T* p = m_p;

		if (refCount)
			*refCount = m_refCount;

		initialize();
		return p;
	}

	void
	clear() {
		if (m_refCount)
			m_refCount->release();

		initialize();
	}

protected:
	void
	initialize() {
		m_p = NULL;
		m_refCount = NULL;
	}
};

//..............................................................................

} // namespace rc
} // namespace axl
