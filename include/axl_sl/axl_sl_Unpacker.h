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

#define _AXL_SL_UNPACKER_H

#include "axl_sl_Pack.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class UnpackStringBase
{
public:
	size_t
	operator () (
		const void* p,
		size_t size,
		const T** value
		)
	{
		size_t length = StringDetailsBase<T>::calcLength((T*)p);
		size_t stringSize = (length + 1) * sizeof(T);

		if (size < stringSize)
			return -1;

		*value = (T*)p;
		return (length + 1) * sizeof(T);
	}

	size_t
	operator () (
		const void* p,
		size_t size,
		T** value
		)
	{
		return operator () (p, size, (const T**) value);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class UnpackPtr
{
public:
	size_t
	operator () (
		const void* p,
		size_t size,
		T** value
		)
	{
		if (size < sizeof(T))
			return -1;

		*value = (T*)p;
		return sizeof(T);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// self-sized object

template <
	typename T,
	typename SizeOf = sl::SizeOf<T>
	>
class UnpackSelfSizedPtr
{
public:
	size_t
	operator () (
		const void* p,
		size_t size,
		T** value
		)
	{
		if (size < sizeof(T))
			return -1;

		size_t objectSize = SizeOf() ((T*)p);
		ASSERT(objectSize >= sizeof(T)); // otherwise wrong SizeOf impl

		if (size < objectSize)
			return -1;

		*value = (T*)p;
		return objectSize;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef UnpackStringBase<char>    UnpackString;
typedef UnpackStringBase<wchar_t> UnpackString_w;
typedef UnpackStringBase<utf8_t>  UnpackString_utf8;
typedef UnpackStringBase<utf16_t> UnpackString_utf16;
typedef UnpackStringBase<utf32_t> UnpackString_utf32;

//..............................................................................

// most common case

template <typename T>
class Unpack
{
public:
	size_t
	operator () (
		const void* p,
		size_t size,
		T* value
		)
	{
		if (size < sizeof(T))
			return -1;

		*value = *(T*)p;
		return sizeof(T);
	}
};

//..............................................................................

// specialization for strings

template <>
class Unpack<char*>: public UnpackStringBase<char>
{
};

template <>
class Unpack<const char*>: public UnpackStringBase<char>
{
};

template <>
class Unpack<wchar_t*>: public UnpackStringBase<wchar_t>
{
};

template <>
class Unpack<const wchar_t*>: public UnpackStringBase<wchar_t>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for pointers

template <typename T>
class Unpack<T*>: public UnpackPtr<T>
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for errors

template <>
class Unpack<err::ErrorHdr*>: public UnpackSelfSizedPtr<
	err::ErrorHdr,
	err::SizeOfError
	>
{
};

template <>
class Unpack<const err::ErrorHdr*>: public UnpackSelfSizedPtr<
	const err::ErrorHdr,
	err::SizeOfError
	>
{
};
//..............................................................................

class Unpacker
{
protected:
	const void* m_begin;
	const void* m_current;
	size_t m_size;

public:
	Unpacker()
	{
		setup(NULL, 0);
	}

	Unpacker(
		const void* p,
		size_t size
		)
	{
		setup(p, size);
	}

	void
	setup(
		const void* p,
		size_t size
		)
	{
		m_begin = p;
		m_current = p;
		m_size = size;
	}

	void
	clear()
	{
		setup(NULL, 0);
	}

	void
	rewind()
	{
		m_current = m_begin;
	}

	size_t
	getLeftoverSize()
	{
		return m_size - ((uchar_t*)m_current - (uchar_t*)m_begin);
	}

	bool
	unpack(
		void* p,
		size_t size
		);

	template <
		typename T,
		typename Unpack
		>
	bool
	unpack(T* value)
	{
		size_t leftoverSize = getLeftoverSize();
		size_t size = Unpack() (m_current, leftoverSize, value);
		if (size > leftoverSize)
			return false;

		m_current = (uchar_t*)m_current + size;
		return true;
	}

	template <typename T>
	bool
	unpack(T* value)
	{
		return unpack<T, Unpack<T> > (value);
	}

	size_t
	scan_va(
		const char* formatString,
		axl_va_list va
		);

	size_t
	scan(
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL(va, formatString);
		return scan_va(formatString, va);
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
