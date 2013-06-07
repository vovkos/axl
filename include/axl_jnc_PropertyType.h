// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionTypeOverload.h"
#include "axl_jnc_StructType.h"

namespace axl {
namespace jnc {

class CPropertyPtrType;
class CPropertyPtrTypeTuple;
class CClassType;

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
GetFirstPropertyTypeFlag (uint_t Flags)
{
	return (EPropertyTypeFlag) (1 << rtl::GetLoBitIdx (Flags));
}

const char* 
GetPropertyTypeFlagString (EPropertyTypeFlag Flag);

rtl::CString
GetPropertyTypeFlagString (uint_t Flags);

inline
const char* 
GetFirstPropertyTypeFlagString (uint_t Flags)
{
	return GetPropertyTypeFlagString (GetFirstPropertyTypeFlag (Flags));
}

uint_t
GetPropertyTypeFlagsFromModifiers (uint_t Modifiers);

//.............................................................................

enum EPropertyPtrType
{
	EPropertyPtrType_Normal = 0,
	EPropertyPtrType_Weak,
	EPropertyPtrType_Thin,
	EPropertyPtrType_Unsafe,
	EPropertyPtrType__Count,
};

const char*
GetPropertyPtrTypeKindString (EPropertyPtrType PtrTypeKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EPropertyPtrType 
GetPropertyPtrTypeKindFromModifiers (uint_t Modifiers)
{
	return 
		(Modifiers & ETypeModifier_Unsafe) ? EPropertyPtrType_Unsafe :
		(Modifiers & ETypeModifier_Weak) ? EPropertyPtrType_Weak : 
		(Modifiers & ETypeModifier_Thin) ? EPropertyPtrType_Thin : EPropertyPtrType_Normal;
}

//.............................................................................

class CPropertyType: public CType
{
	friend class CTypeMgr;

protected:
	CFunctionType* m_pGetterType;
	CFunctionTypeOverload m_SetterType;
	rtl::CString m_TypeModifierString;
	CPropertyType* m_pStdObjectMemberPropertyType;
	CPropertyType* m_pShortType;
	CPropertyType* m_pBindablePropertyType;
	CStructType* m_pVTableStructType;
	CStructType* m_pAuFieldStructType;
	CStructField* m_AuFieldArray [EStdField__Count];
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
		return !m_pGetterType->GetArgArray ().IsEmpty ();
	}

	bool
	IsMemberPropertyType ()
	{
		return m_pGetterType->IsMemberMethodType ();
	}

	CType* 
	GetThisArgType ()
	{
		return m_pGetterType->GetThisArgType ();
	}

	CNamedType*
	GetThisTargetType ()
	{
		return m_pGetterType->GetThisTargetType ();
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
	GetMemberPropertyType (CClassType* pType);

	CPropertyType*
	GetStdObjectMemberPropertyType ();

	CPropertyType*
	GetShortType  ();

	CPropertyType*
	GetBindablePropertyType ();

	CPropertyPtrType* 
	GetPropertyPtrType (
		EType TypeKind,
		EPropertyPtrType PtrTypeKind = EPropertyPtrType_Normal,
		uint_t Flags = 0
		);

	CPropertyPtrType* 
	GetPropertyPtrType (
		EPropertyPtrType PtrTypeKind = EPropertyPtrType_Normal,
		uint_t Flags = 0
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
	GetStdField (EStdField Field);

	CStructType*
	GetVTableStructType ();

	rtl::CString
	GetTypeModifierString ();

	static
	rtl::CString
	CreateSignature (
		CFunctionType* pGetterType,
		const CFunctionTypeOverload& SetterType,
		uint_t Flags
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
	friend class CTypeMgr;

protected:
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