// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionTypeOverload.h"

namespace axl {
namespace jnc {

class CPropertyPtrType;
class CPropertyPtrTypeTuple;

enum EPropertyPtrType;

//.............................................................................

enum EPropertyTypeFlag
{
	EPropertyTypeFlag_ReadOnly  = 0x0100,
	EPropertyTypeFlag_AutoGet   = 0x0200,
	EPropertyTypeFlag_Bindable  = 0x0400,
	
	EPropertyTypeFlag_Augmented = EPropertyTypeFlag_AutoGet | EPropertyTypeFlag_Bindable,
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

} // namespace jnc {
} // namespace axl {
