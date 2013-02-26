// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionTypeOverload.h"
#include "axl_jnc_StructType.h"

namespace axl {
namespace jnc {

class CPropertyPtrType;
class CPropertyPtrTypeTuple;

enum EPropertyPtrType;

//.............................................................................

enum EPropertyTypeFlag
{
	EPropertyTypeFlag_Const     = 0x0100,
	EPropertyTypeFlag_Bindable  = 0x0200,
	EPropertyTypeFlag_AutoGet   = 0x0400,
	EPropertyTypeFlag_AutoSet   = 0x0800,
	
	EPropertyTypeFlag_Augmented = EPropertyTypeFlag_Bindable | EPropertyTypeFlag_AutoGet,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EPropertyTypeFlag
GetFirstPropertyTypeFlag (int Flags)
{
	return (EPropertyTypeFlag) (1 << rtl::GetLoBitIdx (Flags));
}

const tchar_t* 
GetPropertyTypeFlagString (EPropertyTypeFlag Flag);

inline
const tchar_t* 
GetPropertyTypeFlagString (int Flags)
{
	return GetPropertyTypeFlagString (GetFirstPropertyTypeFlag (Flags));
}

int
GetPropertyTypeFlagsFromModifiers (int Modifiers);

//.............................................................................

enum EAuPropertyField
{
	EAuPropertyField_OnChangeEvent = 0,
	EAuPropertyField_PropValue,
	EAuPropertyField__Count
};

const tchar_t* 
GetAuPropertyFieldString (EAuPropertyField Field);

//.............................................................................

class CPropertyType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionType* m_pGetterType;
	CFunctionTypeOverload m_SetterType;
	rtl::CString m_TypeModifierString;
	CPropertyType* m_pAbstractPropertyMemberType;
	CPropertyType* m_pShortType;
	CPropertyType* m_pBindablePropertyType;
	CStructType* m_pVTableStructType;
	CStructType* m_pAuFieldStructType;
	CStructField* m_AuFieldArray [EAuPropertyField__Count];
	CPropertyPtrTypeTuple* m_pPropertyPtrTypeTuple;

public:
	CPropertyType ();

	bool
	IsReadOnly ()
	{
		return m_SetterType.IsEmpty ();
	}

	bool
	IsIndexed ()
	{
		return !m_pGetterType->GetArgTypeArray ().IsEmpty ();
	}

	bool
	IsPropertyMemberType ()
	{
		return m_pGetterType->IsMethodMemberType ();
	}

	CClassPtrType* 
	GetThisArgType ()
	{
		return m_pGetterType->GetThisArgType ();
	}

	CFunctionType*
	GetGetterType ()
	{
		return m_pGetterType;
	}

	CFunctionTypeOverload*
	GetSetterType ()
	{
		return &m_SetterType;
	}

	CType*
	GetReturnType ()
	{
		ASSERT (m_pGetterType);
		return m_pGetterType->GetReturnType ();
	}

	CPropertyType*
	GetAbstractPropertyMemberType ();

	CPropertyType*
	GetShortType  ();

	CPropertyType*
	GetBindablePropertyType ();

	CPropertyPtrType* 
	GetPropertyPtrType (
		EType TypeKind,
		EPropertyPtrType PtrTypeKind = (EPropertyPtrType) 0,
		int Flags = 0
		);

	CPropertyPtrType* 
	GetPropertyPtrType (
		EPropertyPtrType PtrTypeKind = (EPropertyPtrType) 0,
		int Flags = 0
		)
	{
		return GetPropertyPtrType (EType_PropertyPtr, PtrTypeKind, Flags);
	}

	CStructType* 
	GetAuFieldStructType ()
	{
		return m_pAuFieldStructType;
	}

	CStructField* 
	GetAuField (EAuPropertyField Field)
	{
		ASSERT ((size_t) Field < EAuPropertyField__Count);
		return m_AuFieldArray [Field];
	}

	CStructType*
	GetVTableStructType ();

	rtl::CString
	GetTypeModifierString ();

	static
	rtl::CStringA
	CreateSignature (
		CFunctionType* pGetterType,
		const CFunctionTypeOverload& SetterType,
		int Flags
		);

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ()
	{
		ASSERT (false);
	}
};

//.............................................................................

class CPropertyTypeTuple: public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CPropertyType* m_SimplePropertyTypeArray [ECallConv__Count] [2] [3]; // callconv x const x autoget/autoset
	CPropertyType* m_DataRefPropertyTypeArray [ECallConv__Count] [2]; // callconv x const

public:
	CPropertyTypeTuple ()
	{
		memset (this, 0, sizeof (CPropertyTypeTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
