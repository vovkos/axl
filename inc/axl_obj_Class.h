// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_OBJ_CLASS_H

#include "axl_rtl_SimpleSingleton.h"

namespace axl {
namespace obj {

//.............................................................................

// compile-time class information

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// any class including built-in types (int, float etc)

template <typename T>
class CClassT
{
public:
	static
	size_t 
	GetSize ()
	{ 
		return sizeof (T);
	}

	static
	const char*
	GetName ()
	{ 
		return typeid (T).name ();
	}

	static
	void
	Construct (void* p)
	{
		new (p) T;
	}

	static
	void
	Destruct (void* p)
	{
		((T*) p)->~T ();
	}

	static
	void
	Copy (
		void* p,
		const void* pSrc
		)
	{
		*(T*) p = *(const T*) pSrc;
	}

	static
	size_t
	GetInterfaceOffset (const GUID& Guid)
	{
		return -1;
	}

	static
	bool
	HasInterface (const GUID& Guid)
	{
		return false;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for void

template <>
class CClassT <void>
{
public:
	static
	size_t 
	GetSize ()
	{ 
		return 0;
	}

	static
	const char*
	GetName ()
	{ 
		return typeid (void).name ();
	}

	static
	void
	Construct (void* p)
	{
	}

	static
	void
	Destruct (void* p)
	{
	}

	static
	void
	Copy (
		void* p,
		const void* pSrc
		)
	{
	}

	static
	size_t
	GetInterfaceOffset (const GUID& Guid)
	{
		return -1;
	}

	static
	bool
	HasInterface (const GUID& Guid)
	{
		return false;
	}
};

//.............................................................................

// class that provides information about its interfaces

template <typename T>
class CObjClassT: public CClassT <T>
{
public:
	static
	size_t
	GetInterfaceOffset (const GUID& Guid)
	{
		return T::GetInterfaceOffset_s (Guid);
	}

	static
	bool
	HasInterface (const GUID& Guid)
	{
		return GetInterfaceOffset (Guid) != -1;
	}
};

//.............................................................................

// run-time class information

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the only interface not derived from root (can't get class of a class of a class...)
// no GUID is needed 

struct IClass 
{
protected:
	size_t m_Size;
	const char* m_pName;

public:
	size_t 
	GetSize ()
	{ 
		return m_Size;
	}

	const char*
	GetName ()
	{ 
		return m_pName;
	}

	virtual
	void
	Construct (void* p) = 0;

	virtual
	void
	Destruct (void* p) = 0;

	virtual
	void
	Copy (
		void* p,
		const void* pSrc
		) = 0;

	virtual
	size_t
	GetInterfaceOffset (const GUID& Guid) = 0;

	bool
	HasInterface (const GUID& Guid)
	{
		return GetInterfaceOffset (Guid) != -1;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class IClassImplT: public IClass
{
public:
	typedef CClassT <T> CClass;

	IClassImplT ()
	{
		m_Size = CClass::GetSize ();
		m_pName = CClass::GetName ();
	}

	virtual
	void
	Construct (void* p)
	{
		CClass::Construct (p);
	}

	virtual
	void
	Destruct (void* p)
	{
		CClass::Destruct (p);
	}

	virtual
	void
	Copy (
		void* p,
		const void* pSrc
		)
	{
		CClass::Copy (p, pSrc);
	}

	virtual
	size_t
	GetInterfaceOffset (const GUID& Guid)
	{
		return CClass::GetInterfaceOffset (Guid);
	}

	static
	IClassImplT*
	GetSingleton ()
	{
		return rtl::GetSimpleSingleton <IClassImplT> ();
	}
};

//.............................................................................

// class that provides information about its interfaces

template <typename T>
class IObjClassImplT: public IClassImplT <T>
{
public:
	virtual
	size_t
	GetInterfaceOffset (const GUID& Guid)
	{
		return T::GetInterfaceOffset_s (Guid);
	}

	static
	IObjClassImplT*
	GetSingleton ()
	{
		return rtl::GetSimpleSingleton <IObjClassImplT> ();
	}
};

//.............................................................................

// convenient macros for organizing interface maps

#define AXL_OBJ_BEGIN_INTERFACE_MAP(Class) \
	typedef Class __CInterfaceTableClass; \
	static \
	size_t \
	GetInterfaceOffset_s (const GUID& Guid) \
	{

#define AXL_OBJ_INTERFACE(I) \
		if (InlineIsEqualGUID (Guid, __uuidof (I))) \
			return offsetof_class (__CInterfaceTableClass, I);

#define AXL_OBJ_PARENT(B) \
		{ \
		size_t Offset = B::GetInterfaceOffset_s(Guid); \
		if (Offset != -1) \
			return offsetof_class (__CInterfaceTableClass, B) + Offset; \
		}

#define AXL_OBJ_END_INTERFACE_MAP() \
		return  -1; \
	}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_OBJ_SIMPLE_INTERFACE_MAP(Class, I) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (Class) \
		AXL_OBJ_INTERFACE (I) \
	AXL_OBJ_END_INTERFACE_MAP ()

#define AXL_OBJ_EMPTY_INTERFACE_MAP() \
	static \
	size_t \
	GetInterfaceOffset_s (const GUID& Guid) \
	{ \
		return -1; \
	}

//.............................................................................

// this macro will fire a compilation error when used on ref::IRefCount-derived class
// thus preventing from making ref::IRefCount-derived class creatable
// ref::IRefCount-derived classes should be created with ref::CFactoryT / CRtFactoryT / CChildT

#define AXL_OBJ_REF_COUNT_MISCREATION_GUARD(Class) \
	static \
	void \
	__RefCountMiscreationGuard () \
	{ \
		class CRefCountMiscreationGuard: \
			public Class, \
			public axl::ref::IRefCount \
		{ \
		}; \
	}

//.............................................................................

// macros for implementing creatable classes

#define AXL_OBJ_GET_SIMPLE_CLASS(Class) \
	axl::obj::ISimpleClassImplT <Class>::GetSingleton ()

#define AXL_OBJ_GET_CLASS(Class) \
	axl::obj::IObjClassImplT <Class>::GetSingleton ()

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_OBJ_IMPLEMENT_GET_OBJECT(Class) \
	virtual \
	void* \
	GetObject (axl::obj::IClass** ppClass) \
	{ \
		if (ppClass) \
			*ppClass = AXL_OBJ_GET_CLASS (Class); \
		return this; \
	}	

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_OBJ_BEGIN_CLASS(Class) \
	AXL_OBJ_IMPLEMENT_GET_OBJECT (Class) \
	AXL_OBJ_REF_COUNT_MISCREATION_GUARD (Class) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (Class)

#define AXL_OBJ_END_CLASS() \
	AXL_OBJ_END_INTERFACE_MAP ()

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_OBJ_SIMPLE_CLASS(Class, I) \
	AXL_OBJ_IMPLEMENT_GET_OBJECT (Class) \
	AXL_OBJ_REF_COUNT_MISCREATION_GUARD (Class) \
	AXL_OBJ_SIMPLE_INTERFACE_MAP (Class, I)

#define AXL_OBJ_EMPTY_CLASS(Class) \
	AXL_OBJ_IMPLEMENT_GET_OBJECT (Class) \
	AXL_OBJ_REF_COUNT_MISCREATION_GUARD (Class) \
	AXL_OBJ_EMPTY_INTERFACE_MAP ()

//.............................................................................

} // namespace obj
} // namespace axl
