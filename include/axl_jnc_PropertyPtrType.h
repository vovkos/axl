// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_PropertyType.h"
#include "axl_jnc_FunctionPtrType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CPropertyPtrType: public CType
{
	friend class CTypeMgr;

protected:
	EPropertyPtrType m_PtrTypeKind;
	CPropertyType* m_pTargetType;

public:
	CPropertyPtrType ();

	EPropertyPtrType
	GetPtrTypeKind ()
	{
		return m_PtrTypeKind;
	}

	CPropertyType* 
	GetTargetType ()
	{
		return m_pTargetType;
	}

	bool
	HasClosure ()
	{
		return m_PtrTypeKind == EPropertyPtrType_Normal || m_PtrTypeKind == EPropertyPtrType_Weak;
	}

	CPropertyPtrType*
	GetCheckedPtrType ()
	{
		return !(m_Flags & (EPtrTypeFlag_Checked | EPtrTypeFlag_Unsafe)) ?  
			m_pTargetType->GetPropertyPtrType (m_TypeKind, m_PtrTypeKind, m_Flags | EPtrTypeFlag_Checked) : 
			this;			
	}

	CPropertyPtrType*
	GetUnCheckedPtrType ()
	{
		return (m_Flags & EPtrTypeFlag_Checked) ?  
			m_pTargetType->GetPropertyPtrType (m_TypeKind, m_PtrTypeKind, m_Flags & ~EPtrTypeFlag_Checked) : 
			this;			
	}

	CStructType* 
	GetPropertyPtrStructType ();

	static
	rtl::CString
	CreateSignature (
		CPropertyType* pPropertyType,
		EType TypeKind,
		EPropertyPtrType PtrTypeKind,
		uint_t Flags
		);

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ();
};

//.............................................................................

struct TPropertyPtrTypeTuple: rtl::TListLink
{
	CStructType* m_pPtrStructType;
	CPropertyPtrType* m_PtrTypeArray [2] [3] [3]; // ref x kind x unsafe / checked
};

//.............................................................................

inline
bool
IsBindableType (CType* pType)
{
	return 
		pType->GetTypeKind () == EType_PropertyRef &&
		(((CPropertyPtrType*) pType)->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Bindable) != 0;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

