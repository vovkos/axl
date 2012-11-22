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
#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CClassBaseType: public rtl::TListLink
{
protected:
	friend class CClassType;

	CClassType* m_pType;
	size_t m_FieldOffset;
	size_t m_MethodTableIndex;

public:
	CClassBaseType ()
	{
		m_pType = NULL;
		m_FieldOffset = 0;
		m_MethodTableIndex = 0;
	}

	CClassType*
	GetType ()
	{
		return m_pType;
	}

	size_t 
	GetFieldOffset ()
	{
		return m_FieldOffset;
	}

	size_t 
	GetMethodTableIndex ()
	{
		return m_MethodTableIndex;
	}
};

//............................................................................

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
	EClassMemberStorage_Undefined,
	EClassMemberStorage_Static,
	EClassMemberStorage_Dynamic,
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
		m_Storage = EClassMemberStorage_Undefined;
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

	CType* m_pType;
	size_t m_Offset;
	size_t m_LlvmIndex;

public:
	CClassFieldMember ()
	{
		m_MemberKind = EClassMember_Field;
		m_pType = NULL;
		m_Offset = 0;
		m_LlvmIndex = -1;
	}

	CType*
	GetType ()
	{
		return m_pType;
	}

	size_t
	GetOffset ()
	{
		return m_Offset;
	}

	size_t
	GetLlvmIndex ()
	{
		return m_LlvmIndex;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassMethodMember;

class CClassMethod: public rtl::TListLink
{
protected:
	friend class CClassType;

	CClassMethodMember* m_pMethodMember;
	CFunction* m_pFunction;
	size_t m_MethodTableIndex;

public:
	CClassMethod ()
	{
		m_pFunction = NULL;
		m_MethodTableIndex = -1;
	}

	CClassMethodMember*
	GetMethodMember ()
	{
		return m_pMethodMember;
	}

	CFunction*
	GetFunction ()
	{
		return m_pFunction;
	}

	size_t
	GetMethodTableIndex ()
	{
		return m_MethodTableIndex;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassMethodMember: public CClassMember
{
protected:
	friend class CClassType;

	rtl::CStdListT <CClassMethod> m_OverloadList;

public:
	CClassMethodMember ()
	{
		m_MemberKind = EClassMember_Method;
	}

	rtl::CIteratorT <CClassMethod>
	GetFirstOverload ()
	{
		return m_OverloadList.GetHead ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassPropertyMember: public CClassMember
{
protected:
	friend class CClassType;

	CProperty* m_pProperty;
	size_t m_MethodTableIndex;

public:
	CClassPropertyMember ()
	{
		m_MemberKind = EClassMember_Property;
		m_pProperty = NULL;
		m_MethodTableIndex = -1;
	}

	CProperty*
	GetProperty ()
	{
		return m_pProperty;
	}

	size_t
	GetMethodTableIndex ()
	{
		return m_MethodTableIndex;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassType: public CStructTypeT <CClassType>
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	rtl::CStringHashTableMapAT <CClassBaseType*> m_BaseTypeMap;
	rtl::CStdListT <CClassBaseType> m_BaseTypeList;

	rtl::CStdListT <CClassFieldMember> m_FieldMemberList;
	rtl::CStdListT <CClassMethodMember> m_MethodMemberList;
	rtl::CStdListT <CClassPropertyMember> m_PropertyMemberList;

	rtl::CArrayT <CFunction*> m_MethodTable;

	void* m_pStaticFieldBlock;

public:
	CClassType ()
	{
		m_TypeKind = EType_Class;
	}
	
	llvm::StructType* 
	GetLlvmType ();

	size_t
	GetMethodCount ()
	{
		return m_MethodTable.GetCount ();
	}

	CFunction**
	GetMethodTable ()
	{
		return m_MethodTable;
	}

	size_t 
	GetBaseTypeCount ()
	{
		return m_BaseTypeList.GetCount ();
	}

	rtl::CIteratorT <CClassBaseType>
	GetFirstBaseType ()
	{
		return m_BaseTypeList.GetHead ();
	}

	CClassBaseType*
	FindBaseType (CClassType* pType)
	{
		rtl::CStringHashTableMapIteratorAT <CClassBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
		return It ? It->m_Value : NULL;
	}

	CClassBaseType*
	AddBaseType (CClassType* pType);

	rtl::CIteratorT <CClassFieldMember>
	GetFirstFieldMember ()
	{
		return m_FieldMemberList.GetHead ();
	}

	rtl::CIteratorT <CClassMethodMember>
	GetFirstMethodMember ()
	{
		return m_MethodMemberList.GetHead ();
	}

	rtl::CIteratorT <CClassPropertyMember>
	GetFirstPropertyMember ()
	{
		return m_PropertyMemberList.GetHead ();
	}

	CClassFieldMember*
	CreateFieldMember (
		const rtl::CString& Name,
		CType* pType
		);

	CClassMethod*
	CreateMethod (
		const rtl::CString& Name,
		CFunction* pFunction
		);

	CClassPropertyMember*
	CreatePropertyMember (
		const rtl::CString& Name,
		CProperty* pProperty
		);
	
protected:
	CClassBaseType*
	CreateBaseTypeImpl (
		CClassType* pType,
		size_t FieldOffset,
		size_t MethodTableIndex
		);

	CClassFieldMember*
	CreateFieldMemberImpl (
		const rtl::CString& Name,
		CType* pType,
		size_t Offset
		);

	CClassMethod*
	CreateMethodImpl (
		const rtl::CString& Name,
		CFunction* pFunction,
		size_t MethodTableIndex
		);

	CClassMethod*
	CreateMethodImpl (
		CClassMethodMember* pMember,
		CFunction* pFunction,
		size_t MethodTableIndex
		);

	CClassPropertyMember*
	CreatePropertyMemberImpl (
		const rtl::CString& Name,
		CProperty* pProperty,
		size_t MethodTableIndex
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
