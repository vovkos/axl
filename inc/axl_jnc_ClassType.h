// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Decl.h"

namespace axl {
namespace jnc {

class CFunctionMgr;

//.............................................................................

enum EClassTypeFlag
{
	EClassTypeFlag_IsAutoEv = 0x010000,
};

//.............................................................................

class CClassBaseType: public rtl::TListLink
{
protected:
	friend class CClassType;

	CClassType* m_pType;
	CStructBaseType* m_pFieldBaseType;
	size_t m_VTableIndex;

public:
	CClassBaseType ()
	{
		m_pType = NULL;
		m_pFieldBaseType = NULL;
		m_VTableIndex = -1;
	}

	CClassType*
	GetType ()
	{
		return m_pType;
	}

	CStructBaseType* 
	GetFieldBaseType ()
	{
		return m_pFieldBaseType;
	}

	size_t
	GetVTableIndex ()
	{
		return m_VTableIndex;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassBaseTypeCoord
{
public:
	CStructBaseTypeCoord m_FieldCoord;
	size_t m_VTableIndex;

	CClassBaseTypeCoord ()
	{
		m_VTableIndex = 0;
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

const tchar_t*
GetClassMemberKindString (EClassMember MemberKind);

const tchar_t*
GetClassMemberStorageString (EClassMemberStorage Storage);

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
	EAccess m_Access;

public:
	CClassMember ()
	{
		m_ItemKind = EModuleItem_ClassMember;
		m_MemberKind = EClassMember_Undefined;
		m_Storage = EClassMemberStorage_Undefined;
		m_Access = EAccess_Public;
	}

	EClassMember GetMemberKind ()
	{
		return m_MemberKind;
	}

	EClassMemberStorage GetStorage ()
	{
		return m_Storage;
	}

	EAccess GetAccess ()
	{
		return m_Access;
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

	CStructMember* m_pStructMember;

public:
	CClassFieldMember ()
	{
		m_MemberKind = EClassMember_Field;
		m_pType = NULL;
		m_pBitFieldBaseType = NULL;
		m_BitCount = 0;

		m_pStructMember = NULL;
	}

	CType*
	GetType ()
	{
		return m_pType;
	}

	size_t
	GetOffset ()
	{
		ASSERT (m_pStructMember);
		return m_pStructMember->GetOffset ();
	}

	size_t
	GetLlvmIndex ()
	{
		ASSERT (m_pStructMember);
		return m_pStructMember->GetLlvmIndex ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassMethodMember: 
	public CClassMember,
	public CFunctionOverload
{
protected:
	friend class CClassType;

public:
	CClassMethodMember ()
	{
		m_MemberKind = EClassMember_Method;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CClassPropertyMember: public CClassMember
{
protected:
	friend class CClassType;

	CPropertyType* m_pType;

public:
	CClassPropertyMember ()
	{
		m_MemberKind = EClassMember_Property;
		m_pType = NULL;
	}

	CPropertyType*
	GetType ()
	{
		return m_pType;
	}
};

//.............................................................................

class CClassType: public CVTableType
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	size_t m_PackFactor;

	rtl::CStringHashTableMapAT <CClassBaseType*> m_BaseTypeMap;
	rtl::CStdListT <CClassBaseType> m_BaseTypeList;
	rtl::CStdListT <CClassFieldMember> m_FieldMemberList;
	rtl::CStdListT <CClassMethodMember> m_MethodMemberList;
	rtl::CStdListT <CClassPropertyMember> m_PropertyMemberList;

	CFunctionType* m_pSimpleMethodType;

	CFunction* m_pPreConstructor;
	CFunctionOverload m_Constructor;
	CFunction* m_pDestructor;
	CFunction* m_pInitializer;

	rtl::CArrayT <CFunction*> m_MethodFunctionArray;

	CStructType* m_pInterfaceHdrStructType;
	CStructType* m_pInterfaceStructType;
	CStructType* m_pClassStructType;

	rtl::CBoxListT <CToken> m_AutoEvBody;

public:
	CClassType ();

	size_t
	GetPackFactor ()
	{
		return m_PackFactor;
	}

	CStructType* 
	GetInterfaceHdrStructType ()
	{
		ASSERT (m_pInterfaceHdrStructType);
		return m_pInterfaceHdrStructType;
	}

	CStructType* 
	GetInterfaceStructType ()
	{
		ASSERT (m_pInterfaceStructType);
		return m_pInterfaceStructType;
	}

	CStructType* 
	GetClassStructType ()
	{
		ASSERT (m_pClassStructType);
		return m_pClassStructType;
	}

	CFunctionType* 
	GetSimpleMethodType ();

	CFunctionType* 
	GetMethodType (CFunctionType* pShortType);

	CFunction* 
	GetPreConstructor ()
	{
		return m_pPreConstructor;
	}

	CFunctionOverload*
	GetConstructor ()
	{
		return &m_Constructor;
	}

	CFunction* 
	GetDestructor ()
	{
		return m_pDestructor;
	}

	CFunction* 
	GetInitializer ();

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
		CClassBaseTypeCoord* pCoord = NULL
		)
	{
		return FindBaseTypeImpl (pType, pCoord, 0);
	}

	CClassBaseType*
	AddBaseType (CClassType* pType);

	CModuleItem*
	FindItemWithBaseTypeList (const tchar_t* pName);

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
		CClassBaseTypeCoord* pBaseTypeCoord = NULL
		)
	{
		return FindMemberImpl (true, pName, pBaseTypeCoord, 0);
	}

	CFunction*
	CreatePreConstructor ();

	CFunction*
	CreateConstructor (CFunctionType* pType);

	CFunction*
	CreateDestructor ();

	CClassFieldMember*
	CreateFieldMember (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0
		);

	CClassFieldMember*
	CreateFieldMember (
		CType* pType,
		size_t BitCount = 0
		)
	{
		return CreateFieldMember (rtl::CString (), pType, BitCount);
	}

	CClassMethodMember*
	CreateMethodMember (
		const rtl::CString& Name,
		CFunction* pFunction
		);

	void
	AddMethodFunction (CFunction* pFunction);

	CClassPropertyMember*
	CreatePropertyMember (
		const rtl::CString& Name,
		CPropertyType* pProperty
		);

	const rtl::CBoxListT <CToken>*
	GetAutoEvBody ()
	{
		return &m_AutoEvBody;
	}

	void
	SetAutoEvBody (rtl::CBoxListT <CToken>* pTokenList);

	bool
	CalcLayout ();

protected:
	bool
	FindBaseTypeImpl (
		CClassType* pType,
		CClassBaseTypeCoord* pCoord,
		size_t Level
		);

	CClassMember*
	FindMemberImpl (
		bool IncludeThis,
		const tchar_t* pName,
		CClassBaseTypeCoord* pBaseTypeCoord,
		size_t Level
		);

	bool
	LayoutFunction (CFunction* pFunction);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
