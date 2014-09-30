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
	typename Alloc = mem::StdAlloc
	>
class Factory
{
public:
	typedef Alloc Alloc;

	class OperatorNew
	{
	public:
#ifdef _DEBUG
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

#ifdef _DEBUG
	static
	Ptr <T> 
	operatorNew (
		const char* filePath,
		int line,
		size_t extra = 0
		)
	{
		return OperatorNew () (filePath, line, extra);
	}
#else
	static
	Ptr <T> 
	operatorNew (size_t extra = 0)
	{
		return OperatorNew () (extra);
	}
#endif
};

//.............................................................................

template <typename T>
class InPlaceFactory
{
public:
	class OperatorNew
	{
	public:
		Ptr <T> 
		operator () (
			void* p,
			mem::FFree* pfFree
			)
		{
			new (p) T;
			((T*) p)->setTarget (p, &rtl::Type <T>::destruct, pfFree); 
			return (T*) p;
		}
	};

public:
	static
	Ptr <T> 
	operatorNew (
		void* p,
		mem::FFree* pfFree
		)
	{
		return OperatorNew () (p, pfFree);
	}
};

//.............................................................................

#ifdef _DEBUG

#define AXL_REF_NEW(class) \
	axl::ref::Factory <class>::operatorNew (__FILE__, __LINE__)

#define AXL_REF_NEW_EXTRA(class, extra) \
	axl::ref::Factory <class>::operatorNew (__FILE__, __LINE__, extra)

#else

#define AXL_REF_NEW(class) \
	axl::ref::Factory <class>::operatorNew ()

#define AXL_REF_NEW_EXTRA(class, extra) \
	axl::ref::Factory <class>::operatorNew (extra)

#endif

#define AXL_REF_NEW_INPLACE(class, p, pfFree) \
	axl::ref::InPlaceFactory <class>::operatorNew (p, pfFree)

//.............................................................................

} // namespace ref
} // namespace axl
