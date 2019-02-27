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

#define _AXL_SL_ARRAY_H

#include "axl_sl_ArrayDetails.h"
#include "axl_err_Error.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename T,
	typename Details0 = ArrayDetails<T>
	>
class ArrayRef
{
public:
	typedef Details0 Details;
	typedef typename Details::Hdr Hdr;
	typedef typename Details::ValueArg ValueArg;

protected:
	T* m_p;
	Hdr* m_hdr;
	size_t m_count;

public:
	ArrayRef()
	{
		initialize();
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	ArrayRef(ArrayRef&& src)
	{
		initialize();
		move(std::move(src));
	}
#endif

	ArrayRef(const ArrayRef& src)
	{
		initialize();
		attach(src);
	}

	ArrayRef(
		const T* p,
		size_t count
		)
	{
		initialize();
		attach(NULL, p, count);
	}

	ArrayRef(
		const T* p,
		const void* end
		)
	{
		initialize();
		attach(NULL, p, (T*)end - p);
	}

	ArrayRef(
		Hdr* hdr,
		const T* p,
		size_t count
		)
	{
		initialize();
		attach(hdr, p, count);
	}

	ArrayRef(
		Hdr* hdr,
		const T* p,
		const void* end
		)
	{
		initialize();
		attach(hdr, p, (T*)end - p);
	}


	~ArrayRef()
	{
		release();
	}

	operator const T* () const
	{
		return m_p;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	ArrayRef&
	operator = (ArrayRef&& src)
	{
		move(std::move(src));
		return *this;
	}
#endif

	ArrayRef&
	operator = (const ArrayRef& src)
	{
		attach(src);
		return *this;
	}

	const T&
	operator [] (intptr_t i) const
	{
		ASSERT((size_t)i < m_count);
		return m_p[i];
	}

	const T*
	cp() const
	{
		return m_p;
	}

	size_t
	getCount() const
	{
		return m_count;
	}

	Hdr*
	getHdr() const
	{
		return m_hdr;
	}

	bool
	isEmpty() const
	{
		return m_count == 0;
	}

	const T*
	getBegin() const
	{
		return m_p;
	}

	const T*
	getEnd() const
	{
		return m_p + m_count;
	}

	const T&
	getFront() const
	{
		ASSERT(m_count);
		return m_p[0];
	}

	const T&
	getBack() const
	{
		ASSERT(m_count);
		return m_p[m_count - 1];
	}

	size_t
	find(ValueArg e) const
	{
		for (size_t i = 0; i < m_count; i++)
			if (m_p[i] == e)
				return i;

		return -1;
	}

	size_t
	findReverse(ValueArg e) const
	{
		for (intptr_t i = m_count - 1; i >= 0; i--)
			if (m_p[i] == e)
				return i;

		return -1;
	}

	void
	release()
	{
		if (m_hdr)
			m_hdr->release();

		initialize();
	}

protected:
	void
	initialize()
	{
		m_p = NULL;
		m_hdr = NULL;
		m_count = 0;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	void
	move(ArrayRef&& src)
	{
		if (m_hdr)
			m_hdr->release();

		m_p = src.m_p;
		m_hdr = src.m_hdr;
		m_count = src.m_count;
		src.initialize();
	}
#endif

	void
	attach(const ArrayRef& src)
	{
		if (&src != this)
			attach(src.m_hdr, src.m_p, src.m_count);
	}

	void
	attach(
		Hdr* hdr,
		T* p,
		size_t count
		)
	{
		if (hdr != m_hdr) // try to avoid unnecessary interlocked ops
		{
			if (hdr)
				hdr->addRef();

			if (m_hdr)
				m_hdr->release();

			m_hdr = hdr;
		}

		m_p = p;
		m_count = count;
	}
};

//..............................................................................

template <
	typename T,
	typename Details0 = ArrayDetails<T>
	>
class Array: public ArrayRef<T, Details0>
{
public:
	typedef sl::ArrayRef<T, Details0> ArrayRef;

	typedef typename ArrayRef::Details Details;
	typedef typename ArrayRef::Hdr Hdr;
	typedef typename ArrayRef::ValueArg ValueArg;
	typedef typename Details::Construct Construct;
	typedef typename Details::ZeroConstruct ZeroConstruct;

public:
	Array()
	{
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Array(Array&& src)
	{
		copy(std::move(src));
	}

	Array(ArrayRef&& src)
	{
		copy(std::move(src));
	}
#endif

	Array(const Array& src)
	{
		copy(src);
	}

	Array(const ArrayRef& src)
	{
		copy(src);
	}

	explicit Array(size_t count)
	{
		setCount(count);
	}

	Array(
		const T* p,
		size_t count
		)
	{
		copy(p, count);
	}

	Array(
		const T* p,
		const void* end
		)
	{
		copy(p, (T*)end - p);
	}

	Array(
		ref::BufKind bufKind,
		void* p,
		size_t size
		)
	{
		setBuffer(bufKind, p, size);
	}

	operator const T* () const
	{
		return this->m_p;
	}

	operator T* ()
	{
		return this->m_p;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Array&
	operator = (Array&& src)
	{
		copy(std::move(src));
		return *this;
	}

	Array&
	operator = (ArrayRef&& src)
	{
		copy(std::move(src));
		return *this;
	}
#endif

	Array&
	operator = (const Array& src)
	{
		copy(src);
		return *this;
	}

	Array&
	operator = (const ArrayRef& src)
	{
		copy(src);
		return *this;
	}

	const T&
	operator [] (intptr_t i) const
	{
		return ArrayRef::operator [] (i);
	}

	T&
	operator [] (intptr_t i)
	{
		return (T&) ArrayRef::operator [] (i);
	}

	T*
	p()
	{
		return ensureExclusive() ? this->m_p : NULL;
	}

	const T*
	getBegin() const
	{
		return this->cp();
	}

	T*
	getBegin()
	{
		return p();
	}

	const T*
	getEnd() const
	{
		return ArrayRef::getEnd();
	}

	T*
	getEnd()
	{
		return (T*)ArrayRef::getEnd();
	}

	const T&
	getFront() const
	{
		return ArrayRef::getFront();
	}

	T&
	getFront()
	{
		return (T&) ArrayRef::getFront();
	}

	const T&
	getBack() const
	{
		return ArrayRef::getBack();
	}

	T&
	getBack()
	{
		return (T&) ArrayRef::getBack();
	}

	void
	clear()
	{
		setCount(0);
	}

	void
	zeroConstruct(
		size_t index,
		size_t count
		)
	{
		if (index >= this->m_count)
			return;

		if (count == -1 || index + count > this->m_count)
			count = this->m_count - index;

		T* p = this->m_p + index;
		Details::destruct(p, count);
		ZeroConstruct() (p, count);
	}

	void
	zeroConstruct()
	{
		Details::destruct(this->m_p, this->m_count);
		ZeroConstruct() (this->m_p, this->m_count);
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	size_t
	copy(ArrayRef&& src)
	{
		if (src.isEmpty())
		{
			clear();
			src.release();
			return 0;
		}

		Hdr* hdr = src.getHdr();
		if (!hdr || (hdr->getFlags() & ref::BufHdrFlag_Exclusive))
		{
			copy(src, src.getCount());
			src.release();
			return this->m_count;
		}

		this->move(std::move(src));
		return this->m_count;
	}
#endif

	size_t
	copy(const ArrayRef& src)
	{
		if (&src == this)
			return this->m_count;

		if (src.isEmpty())
		{
			clear();
			return 0;
		}

		Hdr* hdr = src.getHdr();
		if (!hdr || (hdr->getFlags() & ref::BufHdrFlag_Exclusive))
			return copy(src, src.getCount());

		this->attach(src);
		return this->m_count;
	}

	size_t
	copyReverse(const ArrayRef& src)
	{
		return copyReverse(src, src.m_count);
	}

	size_t
	copy(
		const T* p,
		size_t count
		)
	{
		if (count == 0)
		{
			clear();
			return 0;
		}

		ref::Ptr<void> shadow;
		if (this->m_hdr && this->m_hdr->isInsideBuffer(p))
			if (Details::IsSimple)
			{
				// for simple non-ctor-dtor types we can apply in-buffer shift

				T* end = (T*)this->m_hdr->getEnd();
				ASSERT(p + count <= end);

				this->m_p = (T*)p;
				this->m_count = count;
				return count;
			}
			else
			{
				shadow = this->m_hdr; // ensure we keep p intact
			}

		bool result = setCount(count);
		if (!result)
			return -1;

		Details::copy(this->m_p, p, count);
		return count;
	}

	size_t
	copyReverse(
		const T* p,
		size_t count
		)
	{
		if (count == 0)
		{
			clear();
			return 0;
		}

		ref::Ptr<void> shadow;
		if (this->m_hdr && this->m_hdr->isInsideBuffer(p))
			shadow = this->m_hdr; // ensure we keep p intact

		bool result = setCount(count);
		if (!result)
			return -1;

		Details::copyReverse(this->m_p, p, count);
		return count;
	}

	size_t
	copy(ValueArg e)
	{
		return copy(&e, 1);
	}

	size_t
	appendEmptySpace(size_t count)
	{
		return insertEmptySpace(-1, count);
	}

	size_t
	appendZeroConstruct(size_t count)
	{
		return insertZeroConstruct(-1, count);
	}

	size_t
	append(
		const T* p,
		size_t count
		)
	{
		return insert(-1, p, count);
	}

	size_t
	appendReverse(
		const T* p,
		size_t count
		)
	{
		return insertReverse(-1, p, count);
	}

	size_t
	append(ValueArg e)
	{
		return insert(-1, e);
	}

	size_t
	appendMultiply(
		ValueArg e,
		size_t count
		)
	{
		return insertMultiply(-1, e, count);
	}

	size_t
	append(const ArrayRef& src)
	{
		return insert(-1, src, src.getCount());
	}

	size_t
	appendReverse(const ArrayRef& src)
	{
		return insertReverse(-1, src, src.getCount());
	}

	size_t
	insertEmptySpace(
		size_t index,
		size_t count
		)
	{
		T* dst = insertSpace(index, count);
		return dst ? this->m_count : -1;
	}

	size_t
	insertZeroConstruct(
		size_t index,
		size_t count
		)
	{
		T* dst = insertSpace(index, count);
		if (!dst)
			return -1;

		Details::destruct(dst, count);
		ZeroConstruct() (dst, count);
		return this->m_count;
	}

	size_t
	insert(
		size_t index,
		const T* p,
		size_t count
		)
	{
		ref::Ptr<void> shadow;
		if (this->m_hdr && this->m_hdr->isInsideBuffer(p))
			shadow = this->m_hdr; // ensure we keep p intact

		T* dst = insertSpace(index, count);
		if (!dst)
			return -1;

		if (p)
			Details::copy(dst, p, count);

		return this->m_count;
	}

	size_t
	insertReverse(
		size_t index,
		const T* p,
		size_t count
		)
	{
		ref::Ptr<void> shadow;
		if (this->m_hdr && this->m_hdr->isInsideBuffer(p))
			shadow = this->m_hdr; // ensure we keep p intact

		T* dst = insertSpace(index, count);
		if (!dst)
			return -1;

		if (p)
			Details::copyReverse(dst, p, count);

		return this->m_count;
	}

	size_t
	insert(
		size_t index,
		ValueArg e
		)
	{
		T* dst = insertSpace(index, 1);
		if (!dst)
			return -1;

		*dst = e;
		return this->m_count;
	}

	size_t
	insertMultiply(
		size_t index,
		ValueArg e,
		size_t count
		)
	{
		T* dst = insertSpace(index, count);
		if (!dst)
			return -1;

		T* end = dst + count;

		for (; dst < end; dst++)
			*dst = e;

		return this->m_count;
	}

	size_t
	insert(
		size_t index,
		const ArrayRef& src
		)
	{
		return insert(index, src, src.getCount());
	}

	size_t
	insertReverse(
		size_t index,
		const ArrayRef& src
		)
	{
		return insertReverse(index, src, src.getCount());
	}

	size_t
	remove(
		size_t index,
		size_t count = 1
		)
	{
		if (count == 0)
			return this->m_count;

		size_t oldCount = this->m_count;
		if (index >= oldCount)
			return this->m_count;

		if (count == -1)
			count = oldCount - index;
		else if (index + count >= oldCount)
			return setCount(index);

		bool result = ensureExclusive();
		if (!result)
			return -1;

		size_t newCount = oldCount - count;

		T* dst = this->m_p + index;
		Details::copy(dst, dst + count, newCount - index);

		result = setCount(newCount);
		return result ? newCount : -1;
	}

	size_t
	pop(size_t count = 1)
	{
		if (count >= this->m_count)
			count = this->m_count;

		size_t newCount = this->m_count - count;
		bool result = setCount(newCount);
		return result ? newCount : -1;
	}

	T
	getBackAndPop()
	{
		T e = this->getBack();
		pop();
		return e;
	}

	void
	reverse(
		size_t index,
		size_t count
		)
	{
		size_t thisCount = this->m_count;

		if (index >= thisCount)
			return;

		if (index + count > thisCount)
			count = thisCount - index;

		Details::reverse(this->m_p + index, count);
	}

	void
	reverse()
	{
		reverse(0, -1);
	}

	void
	reverseFrom(size_t index)
	{
		reverse(index, -1);
	}

	void
	reverseUntil(size_t index)
	{
		reverse(0, index);
	}

	bool
	isExclusive()
	{
		return !this->m_count || this->m_hdr && this->m_hdr->getRefCount() == 1;
	}

	bool
	ensureExclusive()
	{
		return this->m_count ? setCount(this->m_count) : true;
	}

	bool
	reserve(size_t count)
	{
		size_t size = count * sizeof(T);

		if (this->m_hdr &&
			this->m_hdr->getRefCount() == 1 &&
			this->m_hdr->m_bufferSize >= size)
			return true;

		size_t bufferSize = getAllocSize(size);

		ref::Ptr<Hdr> hdr = AXL_REF_NEW_EXTRA(Hdr, bufferSize);
		if (!hdr)
			return false;

		hdr->m_bufferSize = bufferSize;
		Details::setHdrCount(hdr, this->m_count);

		T* p = (T*)(hdr + 1);

		if (this->m_count)
			Details::constructCopy(p, this->m_p, this->m_count);

		if (this->m_hdr)
			this->m_hdr->release();

		this->m_p = p;
		this->m_hdr = hdr.detach();
		return true;
	}

	bool
	setCount(size_t count)
	{
		return setCountImpl<Construct> (count);
	}

	bool
	setCountZeroConstruct(size_t count)
	{
		return setCountImpl<ZeroConstruct> (count);
	}

	size_t
	ensureCount(size_t count)
	{
		if (this->m_count < count)
			setCount(count);

		return this->m_count;
	}

	size_t
	ensureCountZeroConstruct(size_t count)
	{
		if (this->m_count < count)
			setCountZeroConstruct(count);

		return this->m_count;
	}

	size_t
	setBuffer(
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		ASSERT(size >= sizeof(Hdr) + sizeof(T));

		uint_t flags = kind != ref::BufKind_Static ? ref::BufHdrFlag_Exclusive : 0;
		size_t bufferSize = size - sizeof(Hdr);

		ref::Ptr<Hdr> hdr = AXL_REF_NEW_INPLACE(Hdr, p, NULL, flags);
		hdr->m_bufferSize = bufferSize;
		Details::setHdrCount(hdr, 0);

		if (this->m_hdr)
			this->m_hdr->release();

		this->m_p = (T*)(hdr + 1);
		this->m_hdr = hdr.detach();
		this->m_count = 0;

		return bufferSize / sizeof(T);
	}

protected:
	T*
	insertSpace(
		size_t index,
		size_t count
		)
	{
		size_t oldCount = this->m_count;
		bool result = setCount(this->m_count + count);
		if (!result)
			return NULL;

		if (index > oldCount)
			index = oldCount;

		T* dst = this->m_p + index;

		if (count && index < oldCount)
			Details::copy(dst + count, dst, oldCount - index);

		return dst;
	}

	template <typename Construct>
	bool
	setCountImpl(size_t count)
	{
		size_t size = count * sizeof(T);

		if (this->m_hdr &&
			this->m_hdr->getRefCount() == 1)
		{
			if (this->m_count == count)
				return true;

			if (this->m_hdr->m_bufferSize >= size)
			{
				if (count > this->m_count)
					Construct() (this->m_p + this->m_count, count - this->m_count);
				else
					Details::destruct(this->m_p + count, this->m_count - count);

				Details::setHdrCount(this->m_hdr, count);
				this->m_count = count;
				return true;
			}
		}

		if (count == 0)
		{
			this->release();
			return true;
		}

		if (!this->m_count)
		{
			bool result = reserve(count);
			if (!result)
				return false;

			Construct() (this->m_p, count);
			Details::setHdrCount(this->m_hdr, count);
			this->m_count = count;
			return true;
		}

		ASSERT(this->m_hdr);

		size_t bufferSize = getAllocSize(size);

		ref::Ptr<Hdr> hdr = AXL_REF_NEW_EXTRA(Hdr, bufferSize);
		if (!hdr)
			return false;

		hdr->m_bufferSize = bufferSize;
		Details::setHdrCount(hdr, count);

		T* p = (T*)(hdr + 1);

		if (count <= this->m_count)
		{
			Details::constructCopy(p, this->m_p, count);
		}
		else
		{
			Details::constructCopy(p, this->m_p, this->m_count);
			Construct() (p + this->m_count, count - this->m_count);
		}

		this->m_hdr->release();

		this->m_p = p;
		this->m_hdr = hdr.detach();
		this->m_count = count;
		return true;
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
