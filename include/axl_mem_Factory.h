// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_FACTORY_H

#include "axl_mem_Alloc.h"

namespace axl {
namespace mem {

//.............................................................................

template <
	typename T,
	typename Alloc_0 = StdAlloc
	>
class StdFactory
{
public:
	typedef Alloc_0 Alloc;

	class New
	{
	public:
#ifdef _DEBUG
		T*
		operator () (
			const char* filePath,
			int line,
			size_t extra = 0
			)
		{
			size_t size = sizeof (T) + extra;

			void* p = Alloc::alloc (size, typeid (T).name (), filePath, line);
			if (!p)
				return NULL;

			memset (p, 0, size); // zero memory before construction
			new (p) T;
			return (T*) p;
		}
#else
		T*
		operator () (size_t extra = 0)
		{
			size_t size = sizeof (T) + extra;

			void* p = Alloc::alloc (size);
			if (!p)
				return NULL;

			memset (p, 0, size);
			new (p) T;
			return (T*) p;
		}
#endif
	};

	class Delete
	{
	public:
		void
		operator () (T* p)
		{
			p->~T ();
			Alloc::free (p);
		}
	};

public:
#ifdef _DEBUG
	static
	T*
	operatorNew (
		const char* filePath,
		int line,
		size_t extra = 0
		)
	{
		return New () (filePath, line, extra);
	}
#else
	static
	T*
	operatorNew (size_t extra = 0)
	{
		return New () (extra);
	}
#endif

	static
	void
	operatorDelete (T* p)
	{
		return Delete () (p);
	}
};

//.............................................................................

template <typename T>
class CppFactory
{
public:
	class New
	{
	public:
		T*
		operator () ()
		{
			return new T;
		}
	};

	class Delete
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
	operatorNew (size_t extra = 0)
	{
		return New () (extra);
	}

	static
	void
	operatorDelete (T* p)
	{
		return Delete () (p);
	}
};

//.............................................................................

// decoupling new & delete is not good, but templated function is necessary for AXL_MEM_DELETE (p)

template <typename T>
void
stdOperatorDelete (T* p)
{
	typename StdFactory <T>::Delete () (p);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
void
cppOperatorDelete (T* p)
{
	typename CppFactory <T>::Delete () (p);
}

//.............................................................................

#ifdef _DEBUG

#define AXL_MEM_NEW(Class) \
	axl::mem::StdFactory <Class>::operatorNew (__FILE__, __LINE__)

#define AXL_MEM_NEW_EXTRA(Class, extra) \
	axl::mem::StdFactory <Class>::operatorNew (__FILE__, __LINE__, extra)

#else

#define AXL_MEM_NEW(Class) \
	axl::mem::StdFactory <Class>::operatorNew ()

#define AXL_MEM_NEW_EXTRA(Class, extra) \
	axl::mem::StdFactory <Class>::operatorNew (extra)

#endif

#define AXL_MEM_DELETE(p) \
	axl::mem::stdOperatorDelete (p)

//.............................................................................

} // namespace mem
} // namespace axl
