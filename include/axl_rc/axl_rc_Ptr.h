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

#define _AXL_RC_PTR_H

#include "axl_rc_RefCount.h"

namespace axl {
namespace rc {

//..............................................................................

// a detached (disowned) pointer -- carries the ref-count along, but reads as a plain T*

template <typename T>
class DetachedPtr {
public:
	T* m_p;
	RefCount* m_refCount;

public:
	DetachedPtr() {
		m_p = NULL;
		m_refCount = NULL;
	}

	DetachedPtr(const NullPtr&) {
		m_p = NULL;
		m_refCount = NULL;
	}

	template <typename T2>
	DetachedPtr(T2* p) {
		m_p = p;
		m_refCount = p;
	}

	DetachedPtr(
		T* p,
		RefCount* refCount
	) {
		m_p = p;
		m_refCount = refCount;
	}

	operator T* () const {
		return m_p;
	}

	T*
	operator -> () const {
		ASSERT(m_p);
		return m_p;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

	Ptr(const Ptr& src) {
		initialize(src.m_p, src.m_refCount);
		incRef();
	}

	Ptr(Ptr&& src) {
		initialize(src.m_p, src.m_refCount);
		src.initialize();
	}

	template <typename T2>
	Ptr(const Ptr<T2>& src) {
		initialize(src.m_p, src.m_refCount);
		incRef();
	}

	template <typename T2>
	Ptr(Ptr<T2>&& src) {
		initialize(src.m_p, src.m_refCount);
		src.initialize();
	}

	template <typename T2>
	Ptr(const WeakPtr<T2>& src) {
		if (src.m_refCount && src.m_refCount->incRefByWeakPtr())
			initialize(src.m_p, src.m_refCount);
		else
			initialize();
	}

	template <typename T2>
	Ptr(T2* p) {
		initialize(p, p);
		incRef();
	}

	Ptr(
		T* p,
		RefCount* refCount
	) {
		initialize(p, refCount);
		incRef();
	}

	~Ptr() {
		decRef();
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

	Ptr&
	operator = (const Ptr& src) {
		setup(src.m_p, src.m_refCount);
		return *this;
	}

	Ptr&
	operator = (Ptr&& src) {
		move(std::move(src));
		return *this;
	}

	template <typename T2>
	Ptr&
	operator = (const Ptr<T2>& src) {
		setup(src.m_p, src.m_refCount);
		return *this;
	}

	template <typename T2>
	Ptr&
	operator = (Ptr<T2>&& src) {
		move(std::move(src));
		return *this;
	}

	template <typename T2>
	Ptr&
	operator = (const WeakPtr<T2>& src) {
		if (src.m_refCount && src.m_refCount->incRefByWeakPtr())
			attach(src.m_p, src.m_refCount);
		else
			clear();

		return *this;
	}

	template <typename T2>
	Ptr&
	operator = (T2* p) {
		setup(p, p);
		return *this;
	}

	bool
	isExclusive() const {
		ASSERT(m_refCount);
		return m_refCount->getRefCount() == 1;
	}

	T*
	p() const {
		return m_p;
	}

	RefCount*
	getRefCount() const {
		return m_refCount;
	}

	template <typename T2>
	void
	setup(T2* p) {
		setup(p, p);
	}

	template <typename T2>
	void
	setup(
		T2* p,
		RefCount* refCount
	) {
		m_p = p;

		if (refCount != m_refCount) {
			if (refCount)
				refCount->incRef();

			decRef();
			m_refCount = refCount;
		}
	}

	template <typename T2>
	void
	move(Ptr<T2>&& src) {
		T2* p = src.m_p;
		RefCount* refCount = src.m_refCount;
		src.initialize();
		attach(p, refCount);
	}

	template <typename T2>
	void
	attach(T2* p) {
		attach(p, p);
	}

	template <typename T2>
	void
	attach(
		T2* p,
		RefCount* refCount
	) {
		decRef();
		m_p = p;
		m_refCount = refCount;
	}

	template <typename T2>
	void
	attach(const DetachedPtr<T2>& src) {
		attach(src.m_p, src.m_refCount);
	}

	DetachedPtr<T>
	detach() {
		DetachedPtr<T> detached(m_p, m_refCount);
		initialize();
		return detached;
	}

	void
	clear() {
		decRef();
		initialize();
	}

protected:
	void
	initialize() {
		initialize(NULL, NULL);
	}

	void
	initialize(
		T* p,
		RefCount* refCount
	) {
		m_p = p;
		m_refCount = refCount;
	}

	void
	incRef() const {
		if (m_refCount)
			m_refCount->incRef();
	}

	void
	decRef() const {
		if (m_refCount)
			m_refCount->decRef();
	}
};

//..............................................................................

} // namespace rc
} // namespace axl
