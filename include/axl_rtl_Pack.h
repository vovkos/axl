// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_PACK_H

#include "axl_rtl_String.h"
#include "axl_err_Error.h"
#include "axl_mem_Block.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename T>
class CPackT
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		axl_va_list va
		)
	{
		*pSize = sizeof (T);

		if (p)
			*(T*) p = AXL_VA_ARG (va, T);

		return va;
	}
};

//.............................................................................

// thanks a lot gcc

template <typename T>
class CPackIntTruncT
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		axl_va_list va
		)
	{
		*pSize = sizeof (T);

		if (p)
			*(T*) p = (T) AXL_VA_ARG (va, int);

		return va;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CPackT <char>: public CPackIntTruncT <char>
{
};

template <>
class CPackT <wchar_t>: public CPackIntTruncT <wchar_t>
{
};

template <>
class CPackT <int8_t>: public CPackIntTruncT <int8_t>
{
};

template <>
class CPackT <uint8_t>: public CPackIntTruncT <uint8_t>
{
};

template <>
class CPackT <int16_t>: public CPackIntTruncT <int16_t>
{
};

template <>
class CPackT <uint16_t>: public CPackIntTruncT <uint16_t>
{
};

//.............................................................................

// pack string directly into buffer

template <typename T>
class CPackStringT
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		axl_va_list va
		)
	{
		T* pString = AXL_VA_ARG (va, T*);

		size_t Length = pString ? CStringDetailsT <T>::CalcLength (pString) : 0;			
		size_t Size = (Length + 1) * sizeof (T);

		*pSize = Size;

		if (p)
		{
			if (pString)
				memcpy (p, pString, Size);
			else
				*(T**) p = NULL;
		}

		return va;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CPackStringT <char>    CPackString;
typedef CPackStringT <wchar_t> CPackString_w;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class CPackT <const char*>: public CPackStringT <char>
{
};

template <>
class CPackT <char*>: public CPackStringT <char>
{
};

template <>
class CPackT <const wchar_t*>: public CPackStringT <wchar_t>
{
};

template <>
class CPackT <wchar_t*>: public CPackStringT <wchar_t>
{
};

//.............................................................................

// general pointer

template <typename T>
class CPackT <T*>
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		axl_va_list va
		)
	{
		T* pObj = AXL_VA_ARG (va, T*);

		*pSize = sizeof (T);

		if (p)
		{
			if (pObj)
				memcpy (p, pObj, sizeof (T));
			else
				memset (p, 0, sizeof (T));
		}

		return va;
	}
};

//.............................................................................

// variable sized object pointer

template <
	typename T,
	typename TGetSize = rtl::CGetSizeT <T>
	>
class CPackVsoPtrT
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		axl_va_list va
		)
	{
		T* pObj = AXL_VA_ARG (va, T*);

		size_t Size = pObj ? TGetSize () (*pObj) : sizeof (T);

		*pSize = Size;

		if (p)
		{
			if (pObj)
				memcpy (p, pObj, Size);
			else
				memset (p, 0, Size);
		}

		return va;
	}
};

//.............................................................................

// pack error last

class CPackLastError
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		axl_va_list va
		)
	{
		err::CError Error = err::GetError ();

		*pSize = Error->m_Size;

		if (p)
			memcpy (p, Error, Error->m_Size);

		return va;
	}
};

//.............................................................................

// specialization for errors

template <>
class CPackT <err::TError*>: public CPackVsoPtrT <err::TError, err::CGetErrorSize>
{
};

template <>
class CPackT <const err::TError*>: public CPackVsoPtrT <err::TError, err::CGetErrorSize>
{
};

//.............................................................................

// pack block referenced by (void* p, size_t Size) pair

class CPackPtrSize
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		axl_va_list va
		)
	{
		void* pPtr = AXL_VA_ARG (va, void*);
		size_t Size = AXL_VA_ARG (va, size_t);
	
		*pSize = Size;

		if (p)
		{
			if (pPtr)
				memcpy (p, pPtr, Size);
			else
				memset (p, 0, Size);
		}

		return va;
	}
};

//.............................................................................

// compile-time sequencing

template <
	typename TPack1, 
	typename TPack2
	>
class CPackSeqExT
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		axl_va_list va
		)
	{
		size_t Size1 = 0;
		size_t Size2 = 0;

		if (!p)
		{
			va = TPack1 () (NULL, &Size1, va);
			va = TPack2 () (NULL, &Size2, va);
		}
		else
		{
			va = TPack1 () (p, &Size1, va);
			va = TPack2 () ((uchar_t*) p + Size1, &Size2, va);
		}

		*pSize = Size1 + Size2;
		return va;
	}
};

//.............................................................................

//  helper typedefs for packing up to 6 elements

template <
	typename TPack1, 
	typename TPack2
	>
class CPackSeqExT_2: public CPackSeqExT <TPack1, TPack2>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TPack1, 
	typename TPack2, 
	typename TPack3
	>
class CPackSeqExT_3: public CPackSeqExT <CPackSeqExT <TPack1, TPack2>, TPack3>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TPack1, 
	typename TPack2, 
	typename TPack3, 
	typename TPack4
	>
class CPackSeqExT_4: public CPackSeqExT <CPackSeqExT_3 <TPack1, TPack2, TPack3>, TPack4>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TPack1, 
	typename TPack2, 
	typename TPack3, 
	typename TPack4, 
	typename TPack5
	>
class CPackSeqExT_5: public CPackSeqExT <CPackSeqExT_4 <TPack1, TPack2, TPack3, TPack4>, TPack5>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TPack1, 
	typename TPack2, 
	typename TPack3, 
	typename TPack4, 
	typename TPack5, 
	typename TPack6
	>
class CPackSeqExT_6: public CPackSeqExT <CPackSeqExT_5 <TPack1, TPack2, TPack3, TPack4, TPack5>, TPack6>
{
};

//.............................................................................

//  helper typedefs for packing up to 6 simple elements

template <
	typename T1, 
	typename T2
	>
class CPackSeqT_2: public CPackSeqExT <
	CPackT <T1>, 
	CPackT <T2>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1, 
	typename T2, 
	typename T3
	>
class CPackSeqT_3: public CPackSeqExT_3 <
	CPackT <T1>, 
	CPackT <T2>,
	CPackT <T3>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1, 
	typename T2, 
	typename T3, 
	typename T4
	>
class CPackSeqT_4: public CPackSeqExT_4 <
	CPackT <T1>, 
	CPackT <T2>,
	CPackT <T3>,
	CPackT <T4>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1, 
	typename T2, 
	typename T3, 
	typename T4, 
	typename T5
	>
class CPackSeqT_5: public CPackSeqExT_5 <
	CPackT <T1>, 
	CPackT <T2>,
	CPackT <T3>,
	CPackT <T4>,
	CPackT <T5>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1, 
	typename T2, 
	typename T3, 
	typename T4, 
	typename T5, 
	typename T6
	>
class CPackSeqT_6: public CPackSeqExT_6 <
	CPackT <T1>, 
	CPackT <T2>,
	CPackT <T3>,
	CPackT <T4>,
	CPackT <T5>,
	CPackT <T6>
	>
{
};

//.............................................................................

template <typename T>
ref::CPtrT <mem::TBlock> 
CreatePackage_va (axl_va_list va)
{
	size_t Size = 0;
	T () (NULL, &Size, va);

	typedef ref::CBoxT <mem::TBlock> CPackage;

	ref::CPtrT <CPackage> Package = AXL_REF_NEW_EXTRA (CPackage, Size);
	Package->m_p = Package + 1;
	Package->m_Size = Size;
	
	T () (Package + 1, &Size, va);
	
	return Package;			
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
ref::CPtrT <mem::TBlock> 
CreatePackage (
	int Unused,
	...
	)
{
	AXL_VA_DECL (va, Unused);
	return CreatePackage_va <T> (va);
}

//.............................................................................

} // namespace rtl
} // namespace axl
