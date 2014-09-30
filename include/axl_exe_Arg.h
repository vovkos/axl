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
	typedef g::CTypeT <T> CType;

	enum
	{
		hasShadow = false
	};

	class CShadow
	{
	};

	axl_va_list
	operator () (
		void* p,
		size_t* size,
		void* shadow,
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
		size_t* size,
		CShadow* shadow,
		axl_va_list va
		)
	{
		T* src = &AXL_VA_ARG (va, T);
		*size = (sizeof (T) + 3) & ~3; // round to sizeof (int)

		if (p)
			new (p) T (*src);

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
		size_t* size,
		void* shadow,
		axl_va_list va
		)
	{
		*size = 0;
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
		hasShadow = true
	};

	class CShadow
	{
	public:
		rtl::CStringT <T> m_string;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		CShadow* shadow,
		axl_va_list va
		)
	{
		T* string = AXL_VA_ARG (va, T*);
		*size = sizeof (T*);

		if (p)
		{
			shadow->m_string = string;
			*(const T**) p = (const T*) shadow->m_string;
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
		hasShadow = true
	};

	class CShadow
	{
	public:
		T m_object;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		CShadow* shadow,
		axl_va_list va
		)
	{
		T* object = AXL_VA_ARG (va, T*);
		*size = sizeof (T*);

		if (p)
		{
			if (object)
			{
				shadow->m_object = *object;
				*(T**) p = &shadow->m_object;
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
		hasShadow = true
	};

	class CShadow
	{
	public:
		ref::CBufT <T, TGetSize> m_object;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		CShadow* shadow,
		axl_va_list va
		)
	{
		T* object = AXL_VA_ARG (va, T*);
		*size = sizeof (T*);

		if (p)
		{
			if (object)
			{
				shadow->m_object = *object;
				*(T**) p = shadow->m_object;
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
		typename TArg1::CArg m_arg1;
		typename TArg2::CArg m_arg2;
	};

	typedef g::CTypeT <CArg> CType;

	class CShadow
	{
	public:
		typename TArg1::CShadow m_shadow1;
		typename TArg2::CShadow m_shadow2;
	};

	enum
	{
		hasShadow = TArg1::hasShadow || TArg2::hasShadow
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		CShadow* shadow,
		axl_va_list va
		)
	{
		size_t size1 = 0;
		size_t size2 = 0;

		if (!p)
		{
			va = TArg1 () (NULL, &size1, NULL, va);
			va = TArg2 () (NULL, &size2, NULL, va);
		}
		else
		{
			va = TArg1 () (p, &size1, &shadow->m_shadow1, va);
			va = TArg2 () ((uchar_t*) p + size1, &size2, &shadow->m_shadow2, va);
		}

		*size = size1 + size2;

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
	ref::CPtrT <obj::IType> m_agent;

	~CArgBlock ()
	{
		if (m_agent && m_p)
			m_agent->destruct (m_p);
	}
};

//.............................................................................

template <typename T>
ref::CPtrT <CArgBlock> 
createArgBlockV (axl_va_list va)
{
	size_t size = 0;
	T () (NULL, &size, NULL, va);

	ref::CPtrT <CArgBlock> package = AXL_REF_NEW_EXTRA (CArgBlock, size);
	package->m_p = package + 1;
	package->m_size = size;

	obj::IType* type = AXL_OBJ_TYPEOF (T::CType);

	if (T::hasShadow)
	{
		typedef ref::CBoxT <typename T::CShadow> CShadow;
		ref::CPtrT <CShadow> shadow = AXL_REF_NEW (CShadow);		
		T () (package + 1, &size, shadow, va);
		package->m_agent.copy (type, shadow.getRefCount ());
	}
	else
	{
		T () (package + 1, &size, NULL, va);
		package->m_agent.copy (type, NULL);
	}
	
	return package;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
ref::CPtrT <CArgBlock> 
createArgBlock (
	int unused,
	...
	)
{
	AXL_VA_DECL (va, unused);
	return createArgBlockV <T> (va);
}

//.............................................................................

} // namespace exe
} // namespace axl
