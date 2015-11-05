// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_RUNTIMETYPE_H

#include "axl_sl_SimpleSingleton.h"
#include "axl_sl_Guid.h"
#include "axl_sl_Type.h"

namespace axl {
namespace sl {

//.............................................................................

// run-time type information

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// the only interface not derived from root (can't get type of a type of a type...)

struct IType
{
protected:
	size_t m_size;
	const char* m_name;
	const sl::Guid* m_guid;
	
public:
	size_t 
	getSize ()
	{ 
		return m_size;
	}

	const char*
	getName ()
	{ 
		return m_name;
	}

	const sl::Guid*
	getGuid ()
	{ 
		return m_guid;
	}

	virtual
	void
	construct (void* p) = 0;

	virtual
	void
	destruct (void* p) = 0;

	virtual
	size_t
	getInterfaceOffset (const sl::Guid& guid) = 0;

	bool
	hasInterface (const sl::Guid& guid)
	{
		return getInterfaceOffset (guid) != -1;
	}
};

//.............................................................................

// root interface

struct IRoot
{
	// {430F1A01-517D-4AFA-AB46-B28E30479E59}	
	AXL_OBJ_INTERFACE (
		IRoot,
		0x430f1a01, 0x517d, 0x4afa, 0xab, 0x46, 0xb2, 0x8e, 0x30, 0x47, 0x9e, 0x59
		)

	virtual
	void*
	getObject (IType** type) = 0;

	void* 
	getObject ()
	{
		return getObject (NULL);
	}	

	IType* 
	getType ()
	{
		IType* type;
		getObject (&type);
		return type;
	}
	
	void*
	getInterface (const sl::Guid& guid)
	{
		IType* type;
		void* p = getObject (&type);
		size_t offset = type->getInterfaceOffset (guid);
		return offset != -1 ? (uchar_t*) p + offset : NULL;
	}
};

//.............................................................................

// pointer to interface, queries interface when necessary

template <typename I>
class Ptr
{
protected:
	I* m_p;

public:
	Ptr ()
	{
		m_p = NULL;
	}

	Ptr (I* p)
	{
		m_p = p;
	}

	Ptr (IRoot* p)
	{
		m_p = (I*) p->getInterface (AXL_OBJ_GUIDOF (I));
	}

	operator I* ()
	{
		return m_p;
	}

	I* 
	operator -> ()
	{
		return m_p;
	}
};

//.............................................................................

template <typename T>
class ITypeSimpleImplT: public IType
{
public:
	typedef Type <T> Type;

	ITypeSimpleImplT ()
	{
		m_size = Type::getSize ();
		m_name = Type::getName ();
	}

	virtual
	void
	construct (void* p)
	{
		Type::construct (p);
	}

	virtual
	void
	destruct (void* p)
	{
		Type::destruct (p);
	}

	virtual
	size_t
	getInterfaceOffset (const sl::Guid& guid)
	{
		return -1;
	}

	static
	ITypeSimpleImplT*
	getSingleton ()
	{
		return mt::getSimpleSingleton <ITypeSimpleImplT> ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class ITypeClassImplT: public ITypeSimpleImplT <T>
{
public:
	ITypeClassImplT ()
	{
		this->m_guid = T::__GetGuid ();
	}

	virtual
	size_t
	getInterfaceOffset (const sl::Guid& guid)
	{
		return T::__GetInterfaceOffset (guid);
	}

	static
	ITypeClassImplT*
	getSingleton ()
	{
		return mt::getSimpleSingleton <ITypeClassImplT> ();
	}
};

//.............................................................................

// convenient macros for accessing IType* singletons

#define AXL_OBJ_TYPEOF(class) \
	axl::obj::ITypeSimpleImplT <class>::getSingleton ()

#define AXL_OBJ_CLASSOF(class) \
	axl::obj::ITypeClassImplT <class>::getSingleton ()

//.............................................................................

// tagging with GUIDs

#define AXL_OBJ_GUID_TAG(l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	static \
	const axl::sl::Guid* \
	__GetGuid () \
	{ \
		static axl::sl::Guid guidTag = AXL_SL_GUID_INITIALIZER(l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8); \
		return &guidTag; \
	}

// use existing GUID

#define AXL_OBJ_GUID_TAG_E(guid) \
	static \
	const axl::sl::Guid* \
	__GetGuid () \
	{ \
		return &guid; \
	}

#define AXL_OBJ_GUIDOF(class) \
	(*(class::__GetGuid ()))

//.............................................................................

// convenient macros for organizing interface maps

#define AXL_OBJ_BEGIN_INTERFACE_MAP(class) \
	typedef class __CThisClass; \
	static \
	size_t \
	__GetInterfaceOffset (const sl::Guid& guid) \
	{

#define AXL_OBJ_INTERFACE_ENTRY(I) \
		if (guid == AXL_OBJ_GUIDOF (I)) \
			return sl::OffsetOfClass <__CThisClass, I> () (); \

#define AXL_OBJ_INTERFACE_CHAIN(B) \
		{ \
		size_t offset = B::__GetInterfaceOffset (guid); \
		if (offset != -1) \
			return sl::OffsetOfClass <__CThisClass, B> () () + offset; \
		}

#define AXL_OBJ_END_INTERFACE_MAP() \
		return  -1; \
	}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_OBJ_SIMPLE_INTERFACE_MAP(class, I) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (class) \
		AXL_OBJ_INTERFACE_ENTRY (I) \
	AXL_OBJ_END_INTERFACE_MAP ()

//.............................................................................

// tag with GUID and create a singe-entry interface map, so when deriving from 
// a single interface it is not required to explicitly specify interface map

#define AXL_OBJ_INTERFACE(iface, l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_OBJ_GUID_TAG (l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_OBJ_SIMPLE_INTERFACE_MAP (iface, iface)

//.............................................................................

#define AXL_OBJ_IMPLEMENT_GET_OBJECT(class) \
	virtual \
	void* \
	getObject (axl::obj::IType** type) \
	{ \
		if (type) \
			*type = AXL_OBJ_CLASSOF (class); \
		return this; \
	}	

// will be redefined in axl_ref_RefCount.h

#define AXL_OBJ_REF_COUNT_MISCREATION_GUARD(class) 

//.............................................................................

// convenient macros for declaring creatable classes

#define AXL_OBJ_BEGIN_CLASS(class, l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_OBJ_GUID_TAG (l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_OBJ_IMPLEMENT_GET_OBJECT (class) \
	AXL_OBJ_REF_COUNT_MISCREATION_GUARD (class) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (class)

#define AXL_OBJ_BEGIN_CLASS_E(class, guid) \
	AXL_OBJ_GUID_TAG_E (guid) \
	AXL_OBJ_IMPLEMENT_GET_OBJECT (class) \
	AXL_OBJ_REF_COUNT_MISCREATION_GUARD (class) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (class)

#define AXL_OBJ_BEGIN_CLASS_0(class) \
	AXL_OBJ_GUID_TAG_E (axl::sl::g_nullGuid) \
	AXL_OBJ_IMPLEMENT_GET_OBJECT (class) \
	AXL_OBJ_REF_COUNT_MISCREATION_GUARD (class) \
	AXL_OBJ_BEGIN_INTERFACE_MAP (class)

#define AXL_OBJ_END_CLASS() \
	AXL_OBJ_END_INTERFACE_MAP ()

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_OBJ_CLASS(class, iface, l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_OBJ_BEGIN_CLASS (class, l, s1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
	AXL_OBJ_INTERFACE_ENTRY (iface) \
	AXL_OBJ_END_CLASS()

#define AXL_OBJ_CLASS_E(class, iface, guid) \
	AXL_OBJ_BEGIN_CLASS_E (class, guid) \
	AXL_OBJ_INTERFACE_ENTRY (iface) \
	AXL_OBJ_END_CLASS()

#define AXL_OBJ_CLASS_0(class, iface) \
	AXL_OBJ_BEGIN_CLASS_0 (class) \
	AXL_OBJ_INTERFACE_ENTRY (iface) \
	AXL_OBJ_END_CLASS()

//.............................................................................

} // namespace sl
} // namespace axl
