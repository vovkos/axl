// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_DerivableType.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"
#include "axl_jnc_AutoEv.h"
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
	EClassTypeFlag_Abstract  = 0x020000,
	EClassTypeFlag_AutoEv    = 0x040000,
};

//............................................................................

class CClassType: public CDerivableType
{
protected:
	friend class CTypeMgr;
	friend class CParser;
	friend class CProperty;
	
	CStructType* m_pIfaceStructType;
	CStructType* m_pClassStructType;

	// construction / destruction

	CFunction* m_pPreConstructor;
	CFunction* m_pConstructor;
	CFunction* m_pStaticConstructor;
	CFunction* m_pDestructor;
	CFunction* m_pInitializer;

	// fields

	size_t m_PackFactor;
	CStructType* m_pStaticFieldStructType;
	CVariable* m_pStaticDataVariable;

	// vtable

	rtl::CArrayT <CFunction*> m_VirtualMethodArray;
	rtl::CArrayT <CFunction*> m_OverrideMethodArray;
	rtl::CArrayT <CProperty*> m_VirtualPropertyArray;

	CStructType* m_pVTableStructType;
	rtl::CArrayT <CFunction*> m_VTable;
	CValue m_VTablePtrValue;

	// overloaded operators

	rtl::CArrayT <CFunction*> m_UnaryOperatorTable;
	rtl::CArrayT <CFunction*> m_BinaryOperatorTable;
	rtl::CStringHashTableMapAT <CFunction*> m_CastOperatorMap;

	// autoev (handlers must be disconnected in destructor)

	rtl::CArrayT <CAutoEv*> m_AutoEvArray;
	
	
	CClassPtrTypeTuple* m_pClassPtrTypeTuple;

public:
	CClassType ();

	CStructType* 
	GetIfaceStructType ()
	{
		ASSERT (m_pIfaceStructType);
		return m_pIfaceStructType;
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
	GetDefaultConstructor ();

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

	size_t
	GetPackFactor ()
	{
		return m_PackFactor;
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

	CStructField*
	CreateFieldMember (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0
		)
	{
		return CreateFieldMember (EStorage_Undefined, Name, pType, BitCount, PtrTypeFlags);
	}

	CStructField*
	CreateFieldMember (
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0
		)
	{
		return CreateFieldMember (EStorage_Undefined, rtl::CString (), pType, BitCount, PtrTypeFlags);
	}

	CFunctionType* 
	GetMethodMemberType (
		CFunctionType* pShortType,
		int ThisArgTypeFlags = 0
		);

	CPropertyType* 
	GetPropertyMemberType (CPropertyType* pShortType);

	CAutoEvType* 
	GetAutoEvMemberType (CAutoEvType* pShortType);

	bool
	AddMethodMember (CFunction* pFunction);
	
	CFunction*
	CreateMethodMember (
		EStorage StorageKind,
		const rtl::CString& Name,
		CFunctionType* pShortType
		);

	bool
	AddPropertyMember (CProperty* pProperty);

	CProperty*
	CreatePropertyMember (
		EStorage StorageKind,
		const rtl::CString& Name,
		CPropertyType* pShortType
		);

	bool
	AddAutoEvMember (CAutoEv* pAutoEv);

	bool
	HasVTable ()
	{
		return !m_VTable.IsEmpty ();
	}

	CStructType* 
	GetVTableStructType ()
	{
		ASSERT (m_pVTableStructType);
		return m_pVTableStructType;
	}

	CValue
	GetVTablePtrValue ()
	{
		return m_VTablePtrValue;
	}

	CFunction*
	GetUnaryOperator (EUnOp OpKind)
	{
		ASSERT ((size_t) OpKind < EUnOp__Count);
		return m_UnaryOperatorTable ? m_UnaryOperatorTable [OpKind] : NULL;
	}

	CFunction*
	GetBinaryOperator (EBinOp OpKind)
	{
		ASSERT ((size_t) OpKind < EBinOp__Count);
		return m_BinaryOperatorTable ? m_BinaryOperatorTable [OpKind] : NULL;
	}

	virtual
	bool
	CalcLayout ();

	bool 
	StopAutoEvs (const CValue& ThisValue);

	bool
	CallBaseDestructors (const CValue& ThisValue);

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format (_T("class %s"), m_Tag);
	}

	virtual 
	void
	PrepareLlvmType ()
	{
		ASSERT (false);
	}

	void
	AddVirtualFunction (CFunction* pFunction);

	bool
	OverrideVirtualFunction (CFunction* pFunction);

	void
	CreateVTablePtr ();

	bool
	CreateAutoEvConstructor ();

	bool
	CreateDefaultConstructor ();

	bool
	CreateDefaultDestructor ();
	
	bool
	InitializeInterface (
		CClassType* pClassType,
		const CValue& ObjectPtrValue,
		const CValue& IfacePtrValue,
		const CValue& VTablePtrValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
