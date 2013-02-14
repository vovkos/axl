// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_StructType.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"
#include "axl_jnc_UnOp.h"
#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

class CClassPtrType;
class CClassPtrTypeTuple;

enum EClassPtrType;

//.............................................................................

enum EClassTypeFlag
{
	EClassTypeFlag_StdObject = 0x010000, // EStdType_Object 
	EClassTypeFlag_Interface = 0x020000,
	EClassTypeFlag_AutoEv    = 0x040000,
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

class CClassType: public CNamedType
{
protected:
	friend class CTypeMgr;
	friend class CParser;
	friend class CProperty;
	
	CStructType* m_pInterfaceStructType;
	CStructType* m_pClassStructType;

	// construction / destruction

	CFunction* m_pPreConstructor;
	CFunction* m_pConstructor;
	CFunction* m_pStaticConstructor;
	CFunction* m_pDestructor;
	CFunction* m_pInitializer;

	// base types

	rtl::CStringHashTableMapAT <CClassBaseType*> m_BaseTypeMap;
	rtl::CStdListT <CClassBaseType> m_BaseTypeList;

	// fields

	size_t m_PackFactor;
	CStructType* m_pFieldStructType;
	CStructMember* m_pFieldMember;
	CStructType* m_pStaticFieldStructType;
	CVariable* m_pStaticDataVariable;

	// vtable

	rtl::CArrayT <CFunction*> m_VirtualMethodArray;
	rtl::CArrayT <CProperty*> m_VirtualPropertyArray;

	CStructType* m_pVTableStructType;
	rtl::CArrayT <CFunction*> m_VTable;
	CValue m_VTablePtrValue;

	// overloaded operators

	rtl::CArrayT <CFunction*> m_UnaryOperatorTable;
	rtl::CArrayT <CFunction*> m_BinaryOperatorTable;
	rtl::CStringHashTableMapAT <CFunction*> m_CastOperatorMap;

	// autoev

	rtl::CBoxListT <CToken> m_AutoEvBody;

	CClassPtrTypeTuple* m_pClassPtrTypeTuple;

public:
	CClassType ();

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

	CClassPtrType* 
	GetClassPtrType (
		EClassPtrType PtrTypeKind = (EClassPtrType) 0,
		int Flags = 0
		);

	CFunction* 
	GetPreConstructor ()
	{
		return m_pPreConstructor;
	}

	CFunction* 
	GetConstructor ()
	{
		return m_pConstructor;
	}

	CFunction* 
	GetStaticConstructor ()
	{
		return m_pStaticConstructor;
	}

	CFunction* 
	GetDestructor ()
	{
		return m_pDestructor;
	}

	CFunction* 
	GetInitializer ();

	rtl::CConstListT <CClassBaseType>
	GetBaseTypeList ()
	{
		return m_BaseTypeList;
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
	
	CModuleItem*
	FindMember (
		const tchar_t* pName,
		CClassBaseTypeCoord* pBaseTypeCoord = NULL
		)
	{
		return FindMemberImpl (true, true, pName, pBaseTypeCoord, 0);
	}

	size_t
	GetPackFactor ()
	{
		return m_PackFactor;
	}

	CStructType* 
	GetFieldStructType ()
	{
		return m_pFieldStructType;
	}

	CStructMember* 
	GetFieldMember ()
	{
		return m_pFieldMember;
	}

	CStructType* 
	GetStaticFieldStructType ()
	{
		return m_pStaticFieldStructType;
	}

	CVariable* 
	GetStaticDataVariable ()
	{
		return m_pStaticDataVariable;
	}

	CStructMember*
	CreateFieldMember (
		EStorage StorageKind,
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0
		);

	CStructMember*
	CreateFieldMember (
		EStorage StorageKind,
		CType* pType,
		size_t BitCount = 0
		)
	{
		return CreateFieldMember (StorageKind, rtl::CString (), pType, BitCount);
	}

	CStructMember*
	CreateFieldMember (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0
		)
	{
		return CreateFieldMember (EStorage_Undefined, Name, pType, BitCount);
	}

	CStructMember*
	CreateFieldMember (
		CType* pType,
		size_t BitCount = 0
		)
	{
		return CreateFieldMember (EStorage_Undefined, rtl::CString (), pType, BitCount);
	}

	CFunctionType* 
	GetMethodMemberType (
		CFunctionType* pShortType,
		int ThisArgTypeFlags = 0
		);

	CPropertyType* 
	GetPropertyMemberType (CPropertyType* pShortType);

	bool
	AddMethodMember (CFunction* pFunction);

	bool
	AddPropertyMember (CProperty* pProperty);

	rtl::CArrayT <CFunction*>
	GetVTable ()
	{
		return m_VTable;
	}

	CStructType* 
	GetVTableStructType ()
	{
		ASSERT (m_pVTableStructType);
		return m_pVTableStructType;
	}

	bool
	GetVTablePtrValue (CValue* pValue);

	CFunction*
	GetUnaryOperator (EUnOp OpKind)
	{
		ASSERT (OpKind >= 0 && OpKind < EUnOp__Count);
		return m_UnaryOperatorTable ? m_UnaryOperatorTable [OpKind] : NULL;
	}

	CFunction*
	GetBinaryOperator (EBinOp OpKind)
	{
		ASSERT (OpKind >= 0 && OpKind < EBinOp__Count);
		return m_BinaryOperatorTable ? m_BinaryOperatorTable [OpKind] : NULL;
	}

	const rtl::CBoxListT <CToken>*
	GetAutoEvBody ()
	{
		return &m_AutoEvBody;
	}

	void
	SetAutoEvBody (rtl::CBoxListT <CToken>* pTokenList);

	virtual
	bool
	CalcLayout ();

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format ((m_Flags & EClassTypeFlag_Interface) ? _T("interface %s") : _T("class %s"), m_Tag);
	}

	virtual 
	void
	PrepareLlvmType ()
	{
		ASSERT (false);
	}

	bool
	FindBaseTypeImpl (
		CClassType* pType,
		CClassBaseTypeCoord* pCoord,
		size_t Level
		);

	CModuleItem*
	FindMemberImpl (
		bool IncludeThis,
		bool IncludeExtensionNamespace,
		const tchar_t* pName,
		CClassBaseTypeCoord* pBaseTypeCoord,
		size_t Level
		);

	bool
	LayoutNamedVirtualFunction (CFunction* pFunction);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
