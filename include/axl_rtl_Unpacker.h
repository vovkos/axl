// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_UNPACKER_H

#include "axl_rtl_Pack.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename T>
class CUnpackStringT
{
public:
	size_t
	operator () (
		const void* p,
		size_t size,
		const T** value
		)
	{
		size_t length = CStringDetailsT <T>::calcLength ((T*) p);
		size_t stringSize = (length + 1) * sizeof (T);

		if (size < stringSize)
			return -1;

		*value = (T*) p;
		return (length + 1) * sizeof (T);
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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CUnpackPtrT
{
public:
	size_t
	operator () (
		const void* p,
		size_t size,
		T** value
		)
	{
		if (size < sizeof (T))
			return -1;

		*value = (T*) p;
		return sizeof (T);
	}
};


//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CUnpackStringT <char>    CUnpackString;
typedef CUnpackStringT <wchar_t> CUnpackString_w;

//.............................................................................

// most common case

template <typename T>
class CUnpackT
{
public:
	size_t
	operator () (
		const void* p,
		size_t size,
		T* value
		)
	{
		if (size < sizeof (T))
			return -1;

		*value = *(T*) p;
		return sizeof (T);
	}
};

//.............................................................................

// specialization for strings

template <>
class CUnpackT <char*>: public CUnpackStringT <char>
{
};

template <>
class CUnpackT <const char*>: public CUnpackStringT <char>
{
};

template <>
class CUnpackT <wchar_t*>: public CUnpackStringT <wchar_t>
{
};

template <>
class CUnpackT <const wchar_t*>: public CUnpackStringT <wchar_t>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for pointers

template <typename T>
class CUnpackT <T*>: public CUnpackPtrT <T>
{
};

//.............................................................................

class CUnpacker
{
protected:
	const void* m_begin;
	const void* m_current;
	size_t m_size;

public:
	CUnpacker ()
	{
		setup (NULL, 0);
	}

	CUnpacker (
		const void* p,
		size_t size
		)
	{
		setup (p, size);
	}

	void 
	setup (
		const void* p,
		size_t size
		)
	{
		m_begin = p;
		m_current = p;
		m_size = size;
	}

	void 
	clear ()
	{
		setup (NULL, 0);
	}

	void 
	rewind ()
	{
		m_current = m_begin;
	}

	size_t
	getLeftoverSize ()
	{
		return m_size - ((uchar_t*) m_current - (uchar_t*) m_begin);
	}

	bool
	unpack (
		void* p,
		size_t size
		);

	template <
		typename T,
		typename TUnpack
		>
	bool
	unpack (T* value)
	{
		size_t leftoverSize = getLeftoverSize ();
		size_t size = TUnpack () (m_current, leftoverSize, value);
		if (size > leftoverSize)
			return false;

		m_current = (uchar_t*) m_current + size;
		return true;
	}

	template <typename T>
	bool
	unpack (T* value)
	{
		return unpack <T, CUnpackT <T> > (value);
	}

	size_t
	scan_va (
		const char* formatString,
		axl_va_list va
		);

	size_t
	scan (
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return scan_va (formatString, va);
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
