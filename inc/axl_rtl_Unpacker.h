// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
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
		size_t Size,
		const T** ppValue
		)
	{
		size_t Length = CStringDetailsT <T>::GetLength ((T*) p);
		size_t StringSize = (Length + 1) * sizeof (T);

		if (Size < StringSize)
			return -1;

		*ppValue = (T*) p;
		return (Length + 1) * sizeof (T);
	}

	size_t
	operator () (
		const void* p,
		size_t Size,
		T** ppValue
		)
	{
		return operator () (p, Size, (const T**) ppValue);
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
		size_t Size,
		T** ppValue
		)
	{
		if (Size < sizeof (T))
			return -1;

		*ppValue = (T*) p;
		return sizeof (T);
	}
};


//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CUnpackStringT <char>    CUnpackStringA;
typedef CUnpackStringT <wchar_t> CUnpackStringW;
typedef CUnpackStringT <tchar_t> CUnpackString;

//.............................................................................

// most common case

template <typename T>
class CUnpackT
{
public:
	size_t
	operator () (
		const void* p,
		size_t Size,
		T* pValue
		)
	{
		if (Size < sizeof (T))
			return -1;

		*pValue = *(T*) p;
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
	const void* m_pBegin;
	const void* m_pCurrent;
	size_t m_Size;

public:
	CUnpacker ()
	{
		Setup (NULL, 0);
	}

	CUnpacker (
		const void* p,
		size_t Size
		)
	{
		Setup (p, Size);
	}

	void 
	Setup (
		const void* p,
		size_t Size
		)
	{
		m_pBegin = p;
		m_pCurrent = p;
		m_Size = Size;
	}

	void 
	Clear ()
	{
		Setup (NULL, 0);
	}

	void 
	Rewind ()
	{
		m_pCurrent = m_pBegin;
	}

	size_t
	GetLeftoverSize ()
	{
		return m_Size - ((uchar_t*) m_pCurrent - (uchar_t*) m_pBegin);
	}

	bool
	Unpack (
		void* p,
		size_t Size
		);

	template <
		typename T,
		typename TUnpack
		>
	bool
	Unpack (T* pValue)
	{
		size_t LeftoverSize = GetLeftoverSize ();
		size_t Size = TUnpack () (m_pCurrent, LeftoverSize, pValue);
		if (Size > LeftoverSize)
			return false;

		m_pCurrent = (uchar_t*) m_pCurrent + Size;
		return true;
	}

	template <typename T>
	bool
	Unpack (T* pValue)
	{
		return Unpack <T, CUnpackT <T> > (pValue);
	}

	size_t
	ScanV (
		const char* pFormat,
		va_list va
		);

	size_t
	Scan (
		const char* pFormat,
		...
		)
	{
		return ScanV (pFormat, va_start_e (pFormat));
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
