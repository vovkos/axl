// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_FACTORY_H

#include "axl_ref_Ptr.h"
#include "axl_mem_Factory.h"

namespace axl {
namespace ref {

//.............................................................................

template <typename T>
class CObjT: public T
{
public:
	AXL_OBJ_BEGIN_CLASS (CObjT)
		AXL_OBJ_PARENT (T)
		AXL_OBJ_INTERFACE (IRefCount)
	AXL_OBJ_END_CLASS ()
};

//.............................................................................

template <
	typename T,
	typename TAlloc = mem::CStdAlloc
	>
class CFactoryT
{
public:
	typedef TAlloc CAlloc;
	typedef CObjT <T> CObj;

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
			CPtrT <T> Object = mem::CStdFactoryT <CObj, TAlloc>::New (pFilePath, Line, Extra);
			Object->SetFree (&TAlloc::Free);
			return Object;
		}
#else
		CPtrT <T> 
		operator () (size_t Extra = 0)
		{
			CPtrT <T> Object = mem::CStdFactoryT <CObject, TAlloc>::New (Extra);
			Object->SetFree (&TAlloc::Free);
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
class CStaticFactoryT
{
public:
	typedef CObjT <T> CObj;

	class CNew
	{
	public:
		CPtrT <T> 
		operator () (void* p)
		{
			new (p) CObj;
			return (CObj*) p;
		}
	};

public:
	static
	CPtrT <T> 
	New (void* p)
	{
		return CNew () (p);
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

#define AXL_REF_NEW_STATIC(Class, p) \
	axl::ref::CStaticFactoryT <Class>::New (p)

//.............................................................................

} // namespace ref
} // namespace axl
