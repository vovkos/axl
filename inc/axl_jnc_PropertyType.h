// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_VTableType.h"

namespace axl {
namespace jnc {

class CClassType;
class CPropertyPointerType;
class CVariable;
class CClassFieldMember;

//.............................................................................

enum EProperty
{
	EProperty_Undefined,
	EProperty_Global,
	EProperty_Member,
	EProperty_Pointer,
};

//.............................................................................

enum EPropertyTypeFlag
{
	EPropertyTypeFlag_IsBindable = 0x010000,
};

//.............................................................................

class CPropertyType: public CVTableType
{
protected:
	friend class CTypeMgr;
	friend class CClassType;
	friend class CParser;

	EProperty m_PropertyKind;

	CFunction* m_pGetter;
	CFunctionOverload m_Setter;
	CPropertyPointerType* m_pPropertyPointerType;

	CClassType* m_pParentClassType;
	size_t m_ParentVTableIndex;

	union
	{
		CVariable* m_pEventVariable;            // global bindable properties
		CClassFieldMember* m_pEventFieldMember; // member bindable properties
	};

	rtl::CString m_AccessorSignature;
	rtl::CString m_ShortAccessorSignature;

public:
	CPropertyType ();

	EProperty 
	GetPropertyKind ()
	{
		return m_PropertyKind;
	}

	bool
	IsReadOnly ()
	{
		return m_Setter.IsEmpty ();
	}
	
	CFunction*
	GetGetter ()
	{
		return m_pGetter;
	}

	CFunctionOverload*
	GetSetter ()
	{
		return &m_Setter;
	}
	
	CClassType* 
	GetParentClassType ()
	{
		return m_pParentClassType;
	}

	size_t
	GetParentVTableIndex ()
	{
		return m_ParentVTableIndex;
	}

	rtl::CStringA
	GetAccessorSignature ();

	rtl::CStringA
	GetShortAccessorSignature ();

	int 
	CmpAccessorTypes (CPropertyType* pPropertyType)
	{
		return GetAccessorSignature ().Cmp (pPropertyType->GetAccessorSignature ());
	}

	int 
	CmpShortAccessorTypes (CPropertyType* pPropertyType)
	{
		return GetShortAccessorSignature ().Cmp (pPropertyType->GetShortAccessorSignature ());
	}

	CPropertyPointerType* 
	GetPropertyPointerType ();

	CVariable* 
	GetEventVariable ()
	{
		return m_pEventVariable;
	}

	CClassFieldMember* 
	GetEventFieldMember ()
	{
		return m_pEventFieldMember;
	}

	rtl::CString
	CreateTypeString ();

	void
	TagAccessors ();

	bool
	CalcLayout ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
