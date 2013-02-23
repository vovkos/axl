// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_PropertyType.h"
#include "axl_jnc_PropertyVerifier.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

class CProperty: 
	public CDeclModuleItem,
	public CNamespace
{
protected:
	friend class CTypeMgr;
	friend class CClassType;
	friend class CFunctionMgr;
	friend class CParser;

	CPropertyType* m_pType;
	int m_TypeFlags; // before the type was calculated

	// construction / destruction / accessors

	CFunction* m_pConstructor;
	CFunction* m_pStaticConstructor;
	CFunction* m_pDestructor;
	CFunction* m_pGetter;
	CFunction* m_pSetter;

	// parent class

	CClassType* m_pParentClassType;
	CStructField* m_pParentClassFieldMember;
	size_t m_ParentClassVTableIndex;
	CStructField* m_pDataFieldMember;

	// fields

	size_t m_PackFactor;
	CStructType* m_pFieldStructType;
	CStructField* m_pEventMember;
	CStructType* m_pStaticFieldStructType;
	CVariable* m_pStaticDataVariable;

	// vtable

	rtl::CArrayT <CFunction*> m_VTable;
	CValue m_VTablePtrValue;

	CPropertyVerifier m_Verifier;

public:
	CProperty ();

	CPropertyType* 
	GetType ()
	{
		return m_pType;
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
	GetGetter ()
	{
		return m_pGetter;
	}

	CFunction* 
	GetSetter ()
	{
		return m_pSetter;
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

	CStructField* 
	GetEventMember ()
	{
		return m_pEventMember;
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
	
	CClassType* 
	GetParentClassType ()
	{
		return m_pParentClassType;
	}

	bool
	IsVirtual ()
	{
		return m_StorageKind >= EStorage_Abstract && m_StorageKind <= EStorage_Override;
	}

	size_t 
	GetParentClassVTableIndex ()
	{
		return m_ParentClassVTableIndex;
	}

	CStructField* 
	GetDataFieldMember ()
	{
		return m_pDataFieldMember;
	}

	CType*
	CalcType ();

	bool
	Create (CPropertyType* pType);

	void
	ConvertToPropertyMember (CClassType* pClassType);

	CStructField*
	CreateFieldMember (
		EStorage StorageKind,
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0
		);

	CStructField*
	CreateFieldMember (
		EStorage StorageKind,
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0
		)
	{
		return CreateFieldMember (StorageKind, rtl::CString (), pType, BitCount, PtrTypeFlags);
	}

	bool
	AddMethodMember (CFunction* pFunction);

	bool
	AddPropertyMember (CProperty* pProperty);

	bool
	CalcLayout ();

	rtl::CArrayT <CFunction*>
	GetVTable ()
	{
		return m_VTable;
	}

	bool
	GetVTablePtrValue (CValue* pValue);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
