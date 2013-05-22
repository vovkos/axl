// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_ARG_H

#include "axl_rtl_String.h"
#include "axl_mem_Block.h"
#include "axl_err_Error.h"

namespace axl {
namespace exe {

//.............................................................................

template <typename T>
class CArgRootT
{
public:
	typedef T CArg;
	typedef obj::CTypeT <T> CType;

	enum
	{
		HasShadow = false
	};

	class CShadow
	{
	};

	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		void* pShadow,
		axl_va_list va
		);
};

//.............................................................................

// packing of arguments:
// on x86 it is simply padding to sizeof (int) boundary
// on amd64 big objects (sizeof (T) > sizeof (void*)) should be put in shadow buffer

#if (_AXL_CPU == AXL_CPU_X86)

template <typename T>
class CArgT: public CArgRootT <T>
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		CShadow* pShadow,
		axl_va_list va
		)
	{
		T* pSrc = &AXL_VA_ARG (va, T);
		*pSize = (sizeof (T) + 3) & ~3; // round to sizeof (int)

		if (p)
			new (p) T (*pSrc);

		return va;
	}
};

#elif (_AXL_CPU == AXL_CPU_AMD64)

#pragma AXL_TODO ("implement AMD64 argument packing")

#endif

//.............................................................................

// specialization for void

template <>
class CArgT <void>: public CArgRootT <void>
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		void* pShadow,
		axl_va_list va
		)
	{
		*pSize = 0;
		return va;
	}
};

//.............................................................................

// pack string pointer and back up string in the shadow buffer

template <typename T>
class CArgStringT: public CArgRootT <T*>
{
public:
	enum
	{
		HasShadow = true
	};

	class CShadow
	{
	public:
		rtl::CStringT <T> m_String;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		CShadow* pShadow,
		axl_va_list va
		)
	{
		T* pString = AXL_VA_ARG (va, T*);
		*pSize = sizeof (T*);

		if (p)
		{
			pShadow->m_String = pString;
			*(const T**) p = (const T*) pShadow->m_String;
		}

		return va;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CArgStringT <char>    CArgStringA;
typedef CArgStringT <wchar_t> CArgStringW;
typedef CArgStringT <char> CArgString;


//.............................................................................

// specialization for const char*, const wchar_t*, Error

template <>
class CArgT <const char*>: public CArgStringT <char>
{
};

template <>
class CArgT <char*>: public CArgStringT <char>
{
};

template <>
class CArgT <const wchar_t*>: public CArgStringT <wchar_t>
{
};

template <>
class CArgT <wchar_t*>: public CArgStringT <wchar_t>
{
};

//.............................................................................

// pack object pointer and back up object in the shadow buffer

template <typename T>
class CArgObjPtrT: public CArgRootT <T*>
{
public:
	enum
	{
		HasShadow = true
	};

	class CShadow
	{
	public:
		T m_Object;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		CShadow* pShadow,
		axl_va_list va
		)
	{
		T* pObject = AXL_VA_ARG (va, T*);
		*pSize = sizeof (T*);

		if (p)
		{
			if (pObject)
			{
				pShadow->m_Object = *pObject;
				*(T**) p = &pShadow->m_Object;
			}
			else
			{
				*(T**) p = NULL;
			}
		}

		return va;
	}
};

//.............................................................................

// pack variable sized object pointer and back up object in the shadow buffer

template <
	typename T,
	typename TGetSize = rtl::CGetSizeT <T>
	>
class CArgVsoPtrT: public CArgRootT <T*>
{
public:
	enum
	{
		HasShadow = true
	};

	class CShadow
	{
	public:
		ref::CBufT <T, TGetSize> m_Object;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		CShadow* pShadow,
		axl_va_list va
		)
	{
		T* pObject = AXL_VA_ARG (va, T*);
		*pSize = sizeof (T*);

		if (p)
		{
			if (pObject)
			{
				pShadow->m_Object = *pObject;
				*(T**) p = pShadow->m_Object;
			}
			else
			{
				*(T**) p = NULL;
			}
		}

		return va;
	}
};

//.............................................................................

// specialization for TError

template <>
class CArgT <const err::TError*>: public CArgVsoPtrT <err::TError, err::CGetErrorSize>
{
};

template <>
class CArgT <err::TError*>: public CArgVsoPtrT <err::TError, err::CGetErrorSize>
{
};

//.............................................................................

// compile-time sequencing

template <
	typename TArg1, 
	typename TArg2
	>
class CArgSeqExT
{
public:
	class CArg
	{
	public:
		typename TArg1::CArg m_Arg1;
		typename TArg2::CArg m_Arg2;
	};

	typedef obj::CTypeT <CArg> CType;

	class CShadow
	{
	public:
		typename TArg1::CShadow m_Shadow1;
		typename TArg2::CShadow m_Shadow2;
	};

	enum
	{
		HasShadow = TArg1::HasShadow || TArg2::HasShadow
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* pSize,
		CShadow* pShadow,
		axl_va_list va
		)
	{
		size_t Size1 = 0;
		size_t Size2 = 0;

		if (!p)
		{
			va = TArg1 () (NULL, &Size1, NULL, va);
			va = TArg2 () (NULL, &Size2, NULL, va);
		}
		else
		{
			va = TArg1 () (p, &Size1, &pShadow->m_Shadow1, va);
			va = TArg2 () ((uchar_t*) p + Size1, &Size2, &pShadow->m_Shadow2, va);
		}

		*pSize = Size1 + Size2;

		return va;
	}
};

//.............................................................................

//  helper typedefs for packing up to 6 arguments

template <
	typename TArg1, 
	typename TArg2
	>
class CArgSeqExT_2: public CArgSeqExT <TArg1, TArg2>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TArg1, 
	typename TArg2, 
	typename TArg3
	>
class CArgSeqExT_3: public CArgSeqExT <CArgSeqExT <TArg1, TArg2>, TArg3>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TArg1, 
	typename TArg2, 
	typename TArg3, 
	typename TArg4
	>
class CArgSeqExT_4: public CArgSeqExT <CArgSeqExT_3 <TArg1, TArg2, TArg3>, TArg4>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TArg1, 
	typename TArg2, 
	typename TArg3, 
	typename TArg4, 
	typename TArg5
	>
class CArgSeqExT_5: public CArgSeqExT <CArgSeqExT_4 <TArg1, TArg2, TArg3, TArg4>, TArg5>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TArg1, 
	typename TArg2, 
	typename TArg3, 
	typename TArg4, 
	typename TArg5, 
	typename TArg6
	>
class CArgSeqExT_6: public CArgSeqExT <CArgSeqExT_5 <TArg1, TArg2, TArg3, TArg4, TArg5>, TArg6>
{
};

//.............................................................................

//  helper typedefs for packing up to 6 simple arguments

template <
	typename T1, 
	typename T2
	>
class CArgSeqT_2: public CArgSeqExT <
	CArgT <T1>, 
	CArgT <T2>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1, 
	typename T2, 
	typename T3
	>
class CArgSeqT_3: public CArgSeqExT_3 <
	CArgT <T1>, 
	CArgT <T2>,
	CArgT <T3>
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
class CArgSeqT_4: public CArgSeqExT_4 <
	CArgT <T1>, 
	CArgT <T2>,
	CArgT <T3>,
	CArgT <T4>
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
class CArgSeqT_5: public CArgSeqExT_5 <
	CArgT <T1>, 
	CArgT <T2>,
	CArgT <T3>,
	CArgT <T4>,
	CArgT <T5>
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
class CArgSeqT_6: public CArgSeqExT_6 <
	CArgT <T1>, 
	CArgT <T2>,
	CArgT <T3>,
	CArgT <T4>,
	CArgT <T5>,
	CArgT <T6>
	>
{
};

//.............................................................................

class CArgBlock: 
	public ref::IRefCount,
	public mem::TBlock
{
public:
	ref::CPtrT <obj::IType> m_Agent;

	~CArgBlock ()
	{
		if (m_Agent && m_p)
			m_Agent->Destruct (m_p);
	}
};

//.............................................................................

template <typename T>
ref::CPtrT <CArgBlock> 
CreateArgBlockV (axl_va_list va)
{
	size_t Size = 0;
	T () (NULL, &Size, NULL, va);

	ref::CPtrT <CArgBlock> Package = AXL_REF_NEW_EXTRA (CArgBlock, Size);
	Package->m_p = Package + 1;
	Package->m_Size = Size;

	obj::IType* pType = AXL_OBJ_TYPEOF (T::CType);

	if (T::HasShadow)
	{
		typedef ref::CBoxT <typename T::CShadow> CShadow;
		ref::CPtrT <CShadow> Shadow = AXL_REF_NEW (CShadow);		
		T () (Package + 1, &Size, Shadow, va);
		Package->m_Agent.Copy (pType, Shadow.GetRefCount ());
	}
	else
	{
		T () (Package + 1, &Size, NULL, va);
		Package->m_Agent.Copy (pType, NULL);
	}
	
	return Package;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
ref::CPtrT <CArgBlock> 
CreateArgBlock (
	int Unused,
	...
	)
{
	AXL_VA_DECL (va, Unused);
	return CreateArgBlockV <T> (va);
}

//.............................................................................

} // namespace exe
} // namespace axl
