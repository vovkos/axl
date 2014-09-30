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
	typename TAlloc = CStdAlloc
	>
class CStdFactoryT
{
public:
	typedef TAlloc CAlloc;

	class COperatorNew
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

			void* p = TAlloc::alloc (size, typeid (T).name (), filePath, line);
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

			void* p = TAlloc::alloc (size);
			if (!p)
				return NULL;

			memset (p, 0, size);
			new (p) T;
			return (T*) p;
		}
#endif
	};

	class COperatorDelete
	{
	public:
		void 
		operator () (T* p)
		{
			p->~T ();
			TAlloc::free (p);
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
		return COperatorNew () (filePath, line, extra);
	}
#else
	static
	T* 
	operatorNew (size_t extra = 0)
	{
		return COperatorNew () (extra);
	}
#endif

	static
	void 
	operatorDelete (T* p)
	{
		return COperatorDelete () (p);
	}
};

//.............................................................................

template <typename T>
class CCppFactoryT
{
public:
	class COperatorNew
	{
	public:
		T* 
		operator () ()
		{
			return new T;
		}
	};

	class COperatorDelete
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
		return COperatorNew () (extra);
	}

	static
	void 
	operatorDelete (T* p)
	{
		return COperatorDelete () (p);
	}
};

//.............................................................................

// decoupling OperatorNew & OperatorDelete is not good, but templated function is necessary for AXL_MEM_DELETE (p)

template <typename T>
void
stdOperatorDelete (T* p)
{
	typename CStdFactoryT <T>::COperatorDelete () (p);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
void
cppOperatorDelete (T* p)
{
	typename CCppFactoryT <T>::COperatorDelete () (p);
}

//.............................................................................

#ifdef _DEBUG

#define AXL_MEM_NEW(class) \
	axl::mem::CStdFactoryT <class>::operatorNew (__FILE__, __LINE__)

#define AXL_MEM_NEW_EXTRA(class, extra) \
	axl::mem::CStdFactoryT <class>::operatorNew (__FILE__, __LINE__, extra)

#else

#define AXL_MEM_NEW(class) \
	axl::mem::CStdFactoryT <class>::operatorNew ()

#define AXL_MEM_NEW_EXTRA(class, extra) \
	axl::mem::CStdFactoryT <class>::operatorNew (extra)

#endif

#define AXL_MEM_DELETE(p) \
	axl::mem::stdOperatorDelete (p)

//.............................................................................

} // namespace mem
} // namespace axl
