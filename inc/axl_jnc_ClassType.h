// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_StructType.h"
#include "axl_jnc_FunctionType.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EClassMember
{
	EClassMember_Undefined,

	EClassMember_Field,
	EClassMember_Method,
	EClassMember_Property,
	EClassMember_Constructor,
	EClassMember_Destructor,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EClassMemberStorage
{
	EClassMemberStorage_Dynamic,
	EClassMemberStorage_Static,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassMember: 
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CClassType;

	EClassMember m_MemberKind;
	EClassMemberStorage m_Storage;

public:
	CClassMember ()
	{
		m_ItemKind = EModuleItem_ClassMember;
		m_MemberKind = EClassMember_Undefined;
		m_Storage = EClassMemberStorage_Dynamic;
	}

	EClassMember GetMemberKind ()
	{
		return m_MemberKind;
	}

	EClassMemberStorage GetStorage ()
	{
		return m_Storage;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassFieldMember: public CClassMember
{
protected:
	friend class CClassType;

	CStructMember* m_pField;

public:
	CClassFieldMember ()
	{
		m_MemberKind = EClassMember_Field;
		m_pField = NULL;
	}

	CStructMember*
	GetField ()
	{
		return m_pField;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassMethodMember: public CClassMember
{
protected:
	friend class CClassType;

	size_t m_MethodTableIndex;
	CFunctionOverload m_Function;

public:
	CClassMethodMember ()
	{
		m_MemberKind = EClassMember_Method;
		m_MethodTableIndex = -1;
	}

	CFunctionOverload*
	GetFunction ()
	{
		return &m_Function;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassPropertyMember: public CClassMember
{
protected:
	friend class CClassType;

	size_t m_MethodTableIndex;
	CProperty* m_pProperty;

public:
	CClassPropertyMember ()
	{
		m_MemberKind = EClassMember_Property;
		m_MethodTableIndex = -1;
		m_pProperty = NULL;
	}

	CProperty*
	GetProperty ()
	{
		return m_pProperty;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassType: public CNamedType
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	rtl::CArrayT <CType*> m_BaseTypeArray; // structs, interfaces or classes
	rtl::CStdListT <CClassFieldMember> m_FieldMemberList;
	rtl::CStdListT <CClassMethodMember> m_MethodMemberList;
	rtl::CStdListT <CClassPropertyMember> m_PropertyMemberList;
	
	CStructType* m_pFieldBlockType;
	CStructType* m_pStaticFieldBlockType;
	void* m_pStaticFieldBlock;

public:
	CClassType ()
	{
		m_TypeKind = EType_Class;
	}

	CStructType* 
	GetFieldBlockType ()
	{
		return m_pFieldBlockType;
	}

	CModuleItem*
	FindMember (
		const tchar_t* pName,
		bool Traverse = false
		);

	CClassFieldMember*
	CreateFieldMember (
		const rtl::CString& Name,
		CType* pType,
		size_t PackFactor = 8
		);

	CClassMethodMember*
	CreateMethodMember (
		const rtl::CString& Name,
		CFunction* pFunction
		);

	CClassPropertyMember*
	CreatePropertyMember (
		const rtl::CString& Name,
		CProperty* pProperty
		);

	rtl::CIteratorT <CClassFieldMember>
	GetFirstField ()
	{
		return m_FieldMemberList.GetHead ();
	}

	rtl::CIteratorT <CClassMethodMember>
	GetFirstMethod ()
	{
		return m_MethodMemberList.GetHead ();
	}

	rtl::CIteratorT <CClassPropertyMember>
	GetFirstProperty ()
	{
		return m_PropertyMemberList.GetHead ();
	}
};

//.............................................................................

struct TObject
{
	intptr_t m_Flags; 

	// followed by field block
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TInterface
{
	void* m_pMethodTable;
	void* m_pFieldBlock;
	TObject* m_pObject;
	CType* m_pObjectType;
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
