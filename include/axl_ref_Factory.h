// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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

	class CNew
	{
	public:
#ifdef _DEBUG
		CPtrT <T> 
		operator () (
			const char* pFilePath,
			int Line,
			size_t Extra = 0
			)
		{
			CPtrT <T> Object = mem::CStdFactoryT <T, TAlloc>::New (pFilePath, Line, Extra);
			Object->SetTarget (Object, &rtl::CTypeT <T>::Destruct, &TAlloc::Free);
			return Object;
		}
#else
		CPtrT <T> 
		operator () (size_t Extra = 0)
		{
			CPtrT <T> Object = mem::CStdFactoryT <T, TAlloc>::New (Extra);
			Object->SetTarget (Object, &rtl::CTypeT <T>::Destruct, &TAlloc::Free);
			return Object;
		}
#endif
	};

public:

#ifdef _DEBUG
	static
	CPtrT <T> 
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
	CPtrT <T> 
	New (size_t Extra = 0)
	{
		return CNew () (Extra);
	}
#endif
};

//.............................................................................

template <typename T>
class CInPlaceFactoryT
{
public:
	class CNew
	{
	public:
		CPtrT <T> 
		operator () (
			void* p,
			mem::FFree pfFree
			)
		{
			new (p) T;
			((T*) p)->SetTarget (p, &rtl::CTypeT <T>::Destruct, pfFree); 
			return (T*) p;
		}
	};

public:
	static
	CPtrT <T> 
	New (
		void* p,
		mem::FFree pfFree
		)
	{
		return CNew () (p, pfFree);
	}
};

//.............................................................................

#ifdef _DEBUG

#define AXL_REF_NEW(Class) \
	axl::ref::CFactoryT <Class>::New (__FILE__, __LINE__)

#define AXL_REF_NEW_EXTRA(Class, Extra) \
	axl::ref::CFactoryT <Class>::New (__FILE__, __LINE__, Extra)

#else

#define AXL_REF_NEW(Class) \
	axl::ref::CFactoryT <Class>::New ()

#define AXL_REF_NEW_EXTRA(Class, Extra) \
	axl::ref::CFactoryT <Class>::New (Extra)

#endif

#define AXL_REF_NEW_INPLACE(Class, p, pfFree) \
	axl::ref::CInPlaceFactoryT <Class>::New (p, pfFree)

//.............................................................................

} // namespace ref
} // namespace axl
