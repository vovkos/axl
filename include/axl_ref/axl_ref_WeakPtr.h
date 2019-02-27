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

#define _AXL_REF_WEAKPTR_H

#include "axl_ref_RefCount.h"

namespace axl {
namespace ref {

//..............................................................................

// weak ptr

template <typename T>
class WeakPtr
{
	template <typename T2>
	friend class Ptr;

	template <typename T2>
	friend class WeakPtr;

protected:
	T* m_p;
	RefCount* m_refCount;

public:
	WeakPtr()
	{
		m_p = NULL;
		m_refCount = NULL;
	}

	WeakPtr(const NullPtr&)
	{
		m_p = NULL;
		m_refCount = NULL;
	}

	WeakPtr(const WeakPtr& src)
	{
		m_p = NULL, m_refCount = NULL;
		copy(src.m_p, src.m_refCount);
	}

	template <typename T2>
	WeakPtr(const WeakPtr<T2>& src)
	{
		m_p = NULL, m_refCount = NULL;
		copy(src.m_p, src.m_refCount);
	}

	template <typename T2>
	WeakPtr(const Ptr<T2>& src)
	{
		m_p = NULL, m_refCount = NULL;
		copy(src.m_p, src.m_refCount);
	}

	template <typename T2>
	WeakPtr(T2* p)
	{
		m_p = NULL, m_refCount = NULL;
		copy(p, p);
	}

	WeakPtr(
		T* p,
		RefCount* refCount
		)
	{
		m_p = NULL, m_refCount = NULL;
		copy(p, refCount);
	}

	~WeakPtr()
	{
		clear();
	}

	WeakPtr&
	operator = (const NullPtr&)
	{
		clear();
		return *this;
	}

	WeakPtr&
	operator = (const WeakPtr& src)
	{
		copy(src.m_p, src.m_refCount);
		return *this;
	}

	void
	copy(
		T* p,
		RefCount* refCount
		)
	{
		m_p = p;

		if (m_refCount == refCount)
			return;

		if (refCount)
			refCount->addWeakRef();

		if (m_refCount)
			m_refCount->weakRelease();

		m_refCount = refCount;
	}

	void
	attach(
		T* p,
		RefCount* refCount
		)
	{
		if (m_refCount)
			m_refCount->weakRelease();

		m_p = p;
		m_refCount = refCount;
	}

	void
	detach(
		T** pp_o = NULL,
		RefCount** refCount_o = NULL
		)
	{
		if (pp_o)
			*pp_o = m_p;

		if (refCount_o)
			*refCount_o = m_refCount;

		m_p = NULL;
		m_refCount = NULL;
	}

	void
	clear()
	{
		if (m_refCount)
			m_refCount->weakRelease();

		m_p = NULL;
		m_refCount = NULL;
	}

	RefCount*
	getRefCount() const
	{
		return m_refCount;
	}
};

//..............................................................................

} // namespace ref
} // namespace axl
