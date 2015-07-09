// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_FACTORY_H

#include "axl_ref_Ptr.h"
#include "axl_mem_Factory.h"
#include "axl_rtl_Type.h"

namespace axl {
namespace ref {

//.............................................................................

template <
	typename T,
	typename Alloc_0 = mem::StdAlloc
	>
class Factory
{
public:
	typedef Alloc_0 Alloc;

	class New
	{
	public:
#ifdef _AXL_DEBUG
		Ptr <T>
		operator () (
			const char* filePath,
			int line,
			size_t extra = 0
			)
		{
			Ptr <T> object = mem::StdFactory <T, Alloc>::operatorNew (filePath, line, extra);
			object->setTarget (object, &rtl::Type <T>::destruct, &Alloc::free);
			return object;
		}
#else
		Ptr <T>
		operator () (size_t extra = 0)
		{
			Ptr <T> object = mem::StdFactory <T, Alloc>::operatorNew (extra);
			object->setTarget (object, &rtl::Type <T>::destruct, &Alloc::free);
			return object;
		}
#endif
	};

public:

#ifdef _AXL_DEBUG
	static
	Ptr <T>
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
	Ptr <T>
	operatorNew (size_t extra = 0)
	{
		return New () (extra);
	}
#endif
};

//.............................................................................

template <typename T>
class InPlaceFactory
{
public:
	class New
	{
	public:
		Ptr <T>
		operator () (
			void* p,
			FreeFunc* freeFunc
			)
		{
			new (p) T;
			((T*) p)->setTarget (p, &rtl::Type <T>::destruct, freeFunc);
			return (T*) p;
		}
	};

public:
	static
	Ptr <T>
	operatorNew (
		void* p,
		FreeFunc* freeFunc
		)
	{
		return New () (p, freeFunc);
	}
};

//.............................................................................

#ifdef _AXL_DEBUG

#define AXL_REF_NEW(Class) \
	axl::ref::Factory <Class>::operatorNew (__FILE__, __LINE__)

#define AXL_REF_NEW_EXTRA(Class, extra) \
	axl::ref::Factory <Class>::operatorNew (__FILE__, __LINE__, extra)

#else

#define AXL_REF_NEW(Class) \
	axl::ref::Factory <Class>::operatorNew ()

#define AXL_REF_NEW_EXTRA(Class, extra) \
	axl::ref::Factory <Class>::operatorNew (extra)

#endif

#define AXL_REF_NEW_INPLACE(Class, p, freeFunc) \
	axl::ref::InPlaceFactory <Class>::operatorNew (p, freeFunc)

//.............................................................................

} // namespace ref
} // namespace axl
