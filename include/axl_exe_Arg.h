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
class ArgRoot
{
public:
	typedef T Arg;
	typedef g::Type <T> Type;

	enum
	{
		hasShadow = false
	};

	class Shadow
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
class Arg: public ArgRoot <T>
{
public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		Shadow* shadow,
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
class Arg <void>: public ArgRoot <void>
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
class ArgString: public ArgRoot <T*>
{
public:
	enum
	{
		hasShadow = true
	};

	class Shadow
	{
	public:
		rtl::String <T> m_string;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		Shadow* shadow,
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

typedef ArgString <char>    ArgStringA;
typedef ArgString <wchar_t> ArgStringW;
typedef ArgString <char> ArgString;


//.............................................................................

// specialization for const char*, const wchar_t*, Error

template <>
class Arg <const char*>: public ArgString <char>
{
};

template <>
class Arg <char*>: public ArgString <char>
{
};

template <>
class Arg <const wchar_t*>: public ArgString <wchar_t>
{
};

template <>
class Arg <wchar_t*>: public ArgString <wchar_t>
{
};

//.............................................................................

// pack object pointer and back up object in the shadow buffer

template <typename T>
class ArgObjPtr: public ArgRoot <T*>
{
public:
	enum
	{
		hasShadow = true
	};

	class Shadow
	{
	public:
		T m_object;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		Shadow* shadow,
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
	typename GetSize = rtl::GetSize <T>
	>
class ArgVsoPtr: public ArgRoot <T*>
{
public:
	enum
	{
		hasShadow = true
	};

	class Shadow
	{
	public:
		ref::Buf <T, GetSize> m_object;
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		Shadow* shadow,
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
class Arg <const err::Error*>: public ArgVsoPtr <err::Error, err::GetErrorSize>
{
};

template <>
class Arg <err::Error*>: public ArgVsoPtr <err::Error, err::GetErrorSize>
{
};

//.............................................................................

// compile-time sequencing

template <
	typename Arg1, 
	typename Arg2
	>
class ArgSeqEx
{
public:
	class Arg
	{
	public:
		typename Arg1::Arg m_arg1;
		typename Arg2::Arg m_arg2;
	};

	typedef g::Type <Arg> Type;

	class Shadow
	{
	public:
		typename Arg1::Shadow m_shadow1;
		typename Arg2::Shadow m_shadow2;
	};

	enum
	{
		hasShadow = Arg1::hasShadow || Arg2::hasShadow
	};

public:
	axl_va_list
	operator () (
		void* p,
		size_t* size,
		Shadow* shadow,
		axl_va_list va
		)
	{
		size_t size1 = 0;
		size_t size2 = 0;

		if (!p)
		{
			va = Arg1 () (NULL, &size1, NULL, va);
			va = Arg2 () (NULL, &size2, NULL, va);
		}
		else
		{
			va = Arg1 () (p, &size1, &shadow->m_shadow1, va);
			va = Arg2 () ((uchar_t*) p + size1, &size2, &shadow->m_shadow2, va);
		}

		*size = size1 + size2;

		return va;
	}
};

//.............................................................................

//  helper typedefs for packing up to 6 arguments

template <
	typename Arg1, 
	typename Arg2
	>
class ArgSeqEx_2: public ArgSeqEx <Arg1, Arg2>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Arg1, 
	typename Arg2, 
	typename Arg3
	>
class ArgSeqEx_3: public ArgSeqEx <ArgSeqEx <Arg1, Arg2>, Arg3>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Arg1, 
	typename Arg2, 
	typename Arg3, 
	typename Arg4
	>
class ArgSeqEx_4: public ArgSeqEx <ArgSeqEx_3 <Arg1, Arg2, Arg3>, Arg4>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Arg1, 
	typename Arg2, 
	typename Arg3, 
	typename Arg4, 
	typename Arg5
	>
class ArgSeqEx_5: public ArgSeqEx <ArgSeqEx_4 <Arg1, Arg2, Arg3, Arg4>, Arg5>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Arg1, 
	typename Arg2, 
	typename Arg3, 
	typename Arg4, 
	typename Arg5, 
	typename Arg6
	>
class ArgSeqEx_6: public ArgSeqEx <ArgSeqEx_5 <Arg1, Arg2, Arg3, Arg4, Arg5>, Arg6>
{
};

//.............................................................................

//  helper typedefs for packing up to 6 simple arguments

template <
	typename T1, 
	typename T2
	>
class ArgSeq_2: public ArgSeqEx <
	Arg <T1>, 
	Arg <T2>
	>
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T1, 
	typename T2, 
	typename T3
	>
class ArgSeq_3: public ArgSeqEx_3 <
	Arg <T1>, 
	Arg <T2>,
	Arg <T3>
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
class ArgSeq_4: public ArgSeqEx_4 <
	Arg <T1>, 
	Arg <T2>,
	Arg <T3>,
	Arg <T4>
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
class ArgSeq_5: public ArgSeqEx_5 <
	Arg <T1>, 
	Arg <T2>,
	Arg <T3>,
	Arg <T4>,
	Arg <T5>
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
class ArgSeq_6: public ArgSeqEx_6 <
	Arg <T1>, 
	Arg <T2>,
	Arg <T3>,
	Arg <T4>,
	Arg <T5>,
	Arg <T6>
	>
{
};

//.............................................................................

class ArgBlock: 
	public ref::IRefCount,
	public mem::Block
{
public:
	ref::Ptr <obj::IType> m_agent;

	~ArgBlock ()
	{
		if (m_agent && m_p)
			m_agent->destruct (m_p);
	}
};

//.............................................................................

template <typename T>
ref::Ptr <ArgBlock> 
createArgBlockV (axl_va_list va)
{
	size_t size = 0;
	T () (NULL, &size, NULL, va);

	ref::Ptr <ArgBlock> package = AXL_REF_NEW_EXTRA (ArgBlock, size);
	package->m_p = package + 1;
	package->m_size = size;

	obj::IType* type = AXL_OBJ_TYPEOF (T::Type);

	if (T::hasShadow)
	{
		typedef ref::Box <typename T::Shadow> Shadow;
		ref::Ptr <Shadow> shadow = AXL_REF_NEW (Shadow);		
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
ref::Ptr <ArgBlock> 
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
