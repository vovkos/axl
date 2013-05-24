// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	friend class CTypeMgr;
	friend class CDerivableType;
	friend class CClassType;
	friend class CFunctionMgr;
	friend class CParser;

protected:
	CPropertyType* m_pType;
	CType* m_pAuPropValueType; // before the type is calculated
	uint_t m_TypeModifiers;    // before the type is calculated

	// construction / destruction / accessors

	CFunction* m_pConstructor;
	CFunction* m_pStaticConstructor;
	CFunction* m_pDestructor;
	CFunction* m_pGetter;
	CFunction* m_pSetter;

	// parent type

	CNamedType* m_pParentType;
	CStructField* m_pParentTypeField;
	size_t m_ParentClassVTableIndex;

	// fields (augmented fields are stored in a base type)

	size_t m_PackFactor;
	CStructType* m_pDataStructType;
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

	CVariable* 
	GetStaticDataVariable ()
	{
		return m_pStaticDataVariable;
	}
	
	CNamedType* 
	GetParentType ()
	{
		return m_pParentType;
	}

	CStructField* 
	GetParentTypeField ()
	{
		return m_pParentTypeField;
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
	ConvertToMemberProperty (CNamedType* pParentType);

	CStructField*
	CreateField (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0,
		rtl::CBoxListT <CToken>* pInitializer = NULL
		);

	CStructField*
	CreateField (
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0
		)
	{
		return CreateField (rtl::CString (), pType, BitCount, PtrTypeFlags);
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
	CreateDataStructType ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
