// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_FACTORY_H

#include "axl_mem_Alloc.h"

namespace axl {
namespace mem {

//.............................................................................

template <
	typename T,
	typename TAlloc = CStdAlloc
	>
class CStdFactoryT
{
public:
	typedef TAlloc CAlloc;

	class CNew
	{
	public:
#ifdef _DEBUG
		T* 
		operator () (
			const char* pFilePath,
			int Line,
			size_t Extra = 0
			)
		{
			size_t Size = sizeof (T) + Extra;

			void* p = TAlloc::Alloc (Size, typeid (T).name (), pFilePath, Line);
			if (!p)
				return NULL;

			memset (p, 0, Size); // important -- some legacy functions rely on zeroed values
			new (p) T;
			return (T*) p;
		}
#else
		T* 
		operator () (size_t Extra = 0)
		{
			size_t Size = sizeof (T) + Extra;

			void* p = TAlloc::Alloc (Size);
			if (!p)
				return NULL;

			memset (p, 0, Size);
			new (p) T;
			return (T*) p;
		}
#endif
	};

	class CDelete
	{
	public:
		void 
		operator () (T* p)
		{
			p->~T ();
			TAlloc::Free (p);
		}
	};

public:
#ifdef _DEBUG
	static
	T* 
	New (
		const char* pFilePath,
		int Line,
		size_t Extra = 0
		)
	{
		return CNew () (pFilePath, Line, Extra);
	}
#else
	static
	T* 
	New (size_t Extra = 0)
	{
		return CNew () (Extra);
	}
#endif

	static
	void 
	Delete (T* p)
	{
		return CDelete () (Extra);
	}
};

//.............................................................................

template <typename T>
class CCppFactoryT
{
public:
	class CNew
	{
	public:
		T* 
		operator () ()
		{
			return new T;
		}
	};

	class CDelete
	{
	public:
		void 
		operator () (T* p)
		{
			delete p;
		}
	};

public:
	static
	T* 
	New (size_t Extra = 0)
	{
		return CNew () (Extra);
	}

	static
	void 
	Delete (T* p)
	{
		return CDelete () (Extra);
	}
};

//.............................................................................

// decoupling New & Delete is not good, but templated function is necessary for AXL_MEM_DELETE (p)

template <typename T>
void
StdDelete (T* p)
{
	CStdFactoryT <T>::CDelete () (p);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
void
CppDelete (T* p)
{
	CCppFactoryT <T>::CDelete () (p);
}

//.............................................................................

#ifdef _DEBUG

#define AXL_MEM_NEW(Class) \
	axl::mem::CStdFactoryT <Class>::New (__FILE__, __LINE__)

#define AXL_MEM_NEW_EXTRA(Class, Extra) \
	axl::mem::CStdFactoryT <Class>::New (__FILE__, __LINE__, Extra)

#else

#define AXL_MEM_NEW(Class) \
	axl::mem::CStdFactoryT <Class>::New ()

#define AXL_MEM_NEW_EXTRA(Class, Extra) \
	axl::mem::CStdFactoryT <Class>::New (Extra)

#endif

#define AXL_MEM_DELETE(p) \
	axl::mem::StdDelete (p)

//.............................................................................

} // namespace mem
} // namespace axl
