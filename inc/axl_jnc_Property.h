// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_PropertyType.h"
#include "axl_jnc_PropertyVerifier.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_AutoEv.h"

namespace axl {
namespace jnc {

//.............................................................................

class CProperty: 
	public CModuleItem,
	public CNamespace
{
protected:
	friend class CTypeMgr;
	friend class CClassType;
	friend class CFunctionMgr;
	friend class CParser;

	CPropertyType* m_pType;
	CType* m_pAuPropValueType; // before the type is calculated
	int m_TypeModifiers;       // before the type is calculated

	// construction / destruction / accessors

	CFunction* m_pConstructor;
	CFunction* m_pStaticConstructor;
	CFunction* m_pDestructor;
	CFunction* m_pGetter;
	CFunction* m_pSetter;

	// parent class

	CClassType* m_pParentClassType;
	CStructField* m_pParentClassField;
	size_t m_ParentClassVTableIndex;

	// fields (augmented fields are stored in a base type)

	size_t m_PackFactor;
	CStructType* m_pDataStructType;
	CStructType* m_pStaticDataStructType;
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
	GetDataStructType ()
	{
		return m_pDataStructType;
	}

	CStructType* 
	GetStaticDataStructType ()
	{
		return m_pStaticDataStructType;
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

	CStructField* 
	GetParentClassField ()
	{
		return m_pParentClassField;
	}

	bool
	IsMember ()
	{
		return m_StorageKind >= EStorage_Member && m_StorageKind <= EStorage_Override;
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

	CPropertyType*
	CalcType ();

	bool
	Create (CPropertyType* pType);

	void
	ConvertToMemberProperty (CClassType* pClassType);

	CStructField*
	CreateField (
		EStorage StorageKind,
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0,
		rtl::CBoxListT <CToken>* pInitializer = NULL
		);

	CStructField*
	CreateField (
		EStorage StorageKind,
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0
		)
	{
		return CreateField (StorageKind, rtl::CString (), pType, BitCount, PtrTypeFlags);
	}

	bool
	AddMethod (CFunction* pFunction);

	bool
	AddProperty (CProperty* pProperty);

	bool
	AddAutoEv (CAutoEv* pAutoEv);

	bool
	CalcLayout ();

	CValue
	GetVTablePtrValue ()
	{
		return m_VTablePtrValue;
	}

	bool 
	CompileAutoGetter ();

	bool 
	CompileAutoSetter ();

protected:
	void
	CreateVTablePtr ();

	CValue
	GetAutoAccessorPropertyValue ();

	CStructType*
	GetDataStructType (EStorage StorageKind);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
