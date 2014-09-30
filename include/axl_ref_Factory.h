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
	typename TAlloc = mem::CStdAlloc
	>
class CFactoryT
{
public:
	typedef TAlloc CAlloc;

	class COperatorNew
	{
	public:
#ifdef _DEBUG
		CPtrT <T> 
		operator () (
			const char* filePath,
			int line,
			size_t extra = 0
			)
		{
			CPtrT <T> object = mem::CStdFactoryT <T, TAlloc>::operatorNew (filePath, line, extra);
			object->setTarget (object, &rtl::CTypeT <T>::destruct, &TAlloc::free);
			return object;
		}
#else
		CPtrT <T> 
		operator () (size_t extra = 0)
		{
			CPtrT <T> object = mem::CStdFactoryT <T, TAlloc>::operatorNew (extra);
			object->setTarget (object, &rtl::CTypeT <T>::destruct, &TAlloc::free);
			return object;
		}
#endif
	};

public:

#ifdef _DEBUG
	static
	CPtrT <T> 
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
	CPtrT <T> 
	operatorNew (size_t extra = 0)
	{
		return COperatorNew () (extra);
	}
#endif
};

//.............................................................................

template <typename T>
class CInPlaceFactoryT
{
public:
	class COperatorNew
	{
	public:
		CPtrT <T> 
		operator () (
			void* p,
			mem::FFree* pfFree
			)
		{
			new (p) T;
			((T*) p)->setTarget (p, &rtl::CTypeT <T>::destruct, pfFree); 
			return (T*) p;
		}
	};

public:
	static
	CPtrT <T> 
	operatorNew (
		void* p,
		mem::FFree* pfFree
		)
	{
		return COperatorNew () (p, pfFree);
	}
};

//.............................................................................

#ifdef _DEBUG

#define AXL_REF_NEW(class) \
	axl::ref::CFactoryT <class>::operatorNew (__FILE__, __LINE__)

#define AXL_REF_NEW_EXTRA(class, extra) \
	axl::ref::CFactoryT <class>::operatorNew (__FILE__, __LINE__, extra)

#else

#define AXL_REF_NEW(class) \
	axl::ref::CFactoryT <class>::operatorNew ()

#define AXL_REF_NEW_EXTRA(class, extra) \
	axl::ref::CFactoryT <class>::operatorNew (extra)

#endif

#define AXL_REF_NEW_INPLACE(class, p, pfFree) \
	axl::ref::CInPlaceFactoryT <class>::operatorNew (p, pfFree)

//.............................................................................

} // namespace ref
} // namespace axl
