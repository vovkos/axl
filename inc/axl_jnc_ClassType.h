// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_StructTypeRoot.h"
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
	size_t m_Offset;
	size_t m_LlvmIndex;
	size_t m_MethodTableIndex;

public:
	CClassBaseType ()
	{
		m_pType = NULL;
		m_Offset = 0;
		m_LlvmIndex = -1;
		m_MethodTableIndex = -1;
	}

	CClassType*
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

	CClassType*
	GetParentType ()
	{
		return (CClassType*) (CNamedType*) m_pParentNamespace; // double cast cause CStructType is not defined yet
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassFieldMember: public CClassMember
{
protected:
	friend class CClassType;

	CType* m_pType;
	CType* m_pBitFieldBaseType;
	size_t m_BitCount;
	size_t m_Offset;
	size_t m_LlvmIndex;

public:
	CClassFieldMember ()
	{
		m_MemberKind = EClassMember_Field;
		m_pType = NULL;
		m_pBitFieldBaseType = NULL;
		m_BitCount = 0;
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

class CClassType: public CStructTypeRoot
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	rtl::CStringHashTableMapAT <CClassBaseType*> m_BaseTypeMap;
	rtl::CStdListT <CClassBaseType> m_BaseTypeList;

	rtl::CStdListT <CClassFieldMember> m_FieldMemberList;
	rtl::CStdListT <CClassMethodMember> m_MethodMemberList;
	rtl::CStdListT <CClassPropertyMember> m_PropertyMemberList;
	
	rtl::CArrayT <void*> m_MethodTable;

	void* m_pStaticFieldBlock;

	llvm::StructType* m_pLlvmInterfaceType;
	llvm::StructType* m_pLlvmClassType;

public:
	CClassType ()
	{
		m_TypeKind = EType_Class;
		m_pStaticFieldBlock = NULL;
		m_pLlvmInterfaceType = NULL;
	}

	size_t 
	GetClassSize ()
	{
		ASSERT (m_TypeKind == EType_Class);
		return sizeof (TObjectHdr) + m_FieldAlignedSize;
	}

	llvm::Type* 
	GetLlvmType ();

	llvm::StructType*
	GetLlvmInterfaceType ();

	llvm::StructType*
	GetLlvmClassType ();

	llvm::PointerType*
	GetLlvmInterfacePtrType ()
	{
		return llvm::PointerType::get (GetLlvmInterfaceType (), 0);
	}

	llvm::PointerType*
	GetLlvmClassPtrType ()
	{
		return llvm::PointerType::get (GetLlvmClassType (), 0);
	}

	size_t
	GetMethodCount ()
	{
		return m_MethodTable.GetCount ();
	}

	void**
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

	bool
	FindBaseType (
		CClassType* pType,
		size_t* pOffset = NULL,
		rtl::CArrayT <size_t>* pLlvmIndexArray = NULL,
		size_t* pMethodTableIndex = NULL
		);

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

	CClassMember*
	FindMember (
		const tchar_t* pName,
		size_t* pBaseTypeOffset,
		rtl::CArrayT <size_t>* pLlvmBaseTypeIndexArray,
		size_t* pBaseTypeMethodTableIndex
		);

	CClassFieldMember*
	CreateFieldMember (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0
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
	
	bool
	CalcLayout ();

	bool
	InitializeObject (
		TObjectHdr* pObject,
		int Flags
		);

protected:
	bool
	InitializeInterface (
		TInterfaceHdr* pInterface,
		TObjectHdr* pObject,
		void** pMethodTable
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
