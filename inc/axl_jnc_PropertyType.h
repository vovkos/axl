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
	EPropertyTypeFlag_ReadOnly = 0x010000,
	EPropertyTypeFlag_Bindable = 0x020000,
	EPropertyTypeFlag_AutoGet  = 0x040000,
};

//.............................................................................

class CPropertyType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionType* m_pGetterType;
	CFunctionTypeOverload m_SetterType;
	rtl::CString m_TypeModifierString;
	CPropertyType* m_pAbstractPropertyMemberType;
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

	CFunctionType*
	GetGetterType ()
	{
		return m_pGetterType;
	}

	const CFunctionTypeOverload&
	GetSetterType ()
	{
		return m_SetterType;
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
