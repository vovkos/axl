// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_DerivableType.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Property.h"
#include "axl_jnc_UnOp.h"
#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

class CClassPtrType;
struct TClassPtrTypeTuple;

//.............................................................................

enum EClassType
{
	EClassType_Normal = 0,
	EClassType_StdObject, // EStdType_Object
	EClassType_Box,
	EClassType_Multicast,
	EClassType_McSnapshot,
	EClassType_AutoEv,
	EClassType_AutoEvIface,
	EClassType_FunctionClosure,
	EClassType_PropertyClosure,
	EClassType_DataClosure,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EClassTypeFlag
{
	EClassTypeFlag_Abstract = 0x010000,
};

//.............................................................................

enum EClassPtrType
{
	EClassPtrType_Normal = 0,
	EClassPtrType_Weak,
	EClassPtrType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetClassPtrTypeKindString (EClassPtrType PtrTypeKind);

//............................................................................

class CClassType: public CDerivableType
{
	friend class CTypeMgr;
	friend class CParser;
	friend class CProperty;
	
protected:
	EClassType m_ClassTypeKind;

	CStructType* m_pIfaceStructType;
	CStructType* m_pClassStructType;

	CFunction* m_pPrimer;
	CFunction* m_pDestructor;

	// prime arrays

	rtl::CArrayT <CBaseTypeSlot*> m_BaseTypePrimeArray;
	rtl::CArrayT <CStructField*> m_MemberPrimeArray;

	// destruct arrays

	rtl::CArrayT <CClassType*> m_BaseTypeDestructArray;
	rtl::CArrayT <CStructField*> m_MemberFieldDestructArray;
	rtl::CArrayT <CProperty*> m_MemberPropertyDestructArray;

	// vtable

	rtl::CArrayT <CFunction*> m_VirtualMethodArray;
	rtl::CArrayT <CFunction*> m_OverrideMethodArray;
	rtl::CArrayT <CProperty*> m_VirtualPropertyArray;

	CStructType* m_pVTableStructType;
	rtl::CArrayT <CFunction*> m_VTable;
	CValue m_VTablePtrValue;
	
	TClassPtrTypeTuple* m_pClassPtrTypeTuple;

public:
	CClassType ();

	EClassType 
	GetClassTypeKind ()
	{
		return m_ClassTypeKind;
	}

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
		EType TypeKind,
		EClassPtrType PtrTypeKind = EClassPtrType_Normal,
		uint_t Flags = 0
		);

	CClassPtrType* 
	GetClassPtrType (
		EClassPtrType PtrTypeKind = EClassPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetClassPtrType (EType_ClassPtr, PtrTypeKind, Flags);
	}

	virtual
	CType*
	GetThisArgType (uint_t PtrTypeFlags)
	{		
		return (CType*) GetClassPtrType (EClassPtrType_Normal, PtrTypeFlags);
	}

	CFunction* 
	GetPrimer ()
	{
		return m_pPrimer;
	}

	CFunction* 
	GetDestructor ()
	{
		return m_pDestructor;
	}

	rtl::CConstListT <CStructField>
	GetFieldList ()
	{
		return m_pIfaceStructType->GetFieldList ();
	}

	virtual
	CStructField*
	GetFieldByIndex (size_t Index);

	virtual
	bool
	AddMethod (CFunction* pFunction);

	virtual
	bool
	AddProperty (CProperty* pProperty);

	bool
	HasVTable ()
	{
		return !m_VTable.IsEmpty ();
	}

	rtl::CArrayT <CFunction*> 
	GetVirtualMethodArray ()
	{
		return m_VirtualMethodArray;
	}
	
	rtl::CArrayT <CProperty*> 
	GetVirtualPropertyArray ()
	{
		return m_VirtualPropertyArray;
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

	virtual 
	bool
	Compile ();

	bool
	CallBaseTypeDestructors (const CValue& ThisValue);

	bool
	CallMemberFieldDestructors (const CValue& ThisValue);

	bool
	CallMemberPropertyDestructors (const CValue& ThisValue);

protected:
	virtual
	CStructField*
	CreateFieldImpl (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0,
		uint_t PtrTypeFlags = 0,
		rtl::CBoxListT <CToken>* pConstructor = NULL,
		rtl::CBoxListT <CToken>* pInitializer = NULL
		);

	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format ("class %s", m_Tag.cc ()); // thanks a lot gcc
	}

	virtual 
	void
	PrepareLlvmType ()
	{
		m_pLlvmType = GetClassStructType ()->GetLlvmType ();
	}

	virtual
	bool
	CalcLayout ();

	void
	AddVirtualFunction (CFunction* pFunction);

	bool
	OverrideVirtualFunction (CFunction* pFunction);

	void
	CreateVTablePtr ();

	void
	CreatePrimer ();

	bool
	PrimeInterface (
		CClassType* pClassType,
		const CValue& ObjectPtrValue,
		const CValue& IfacePtrValue,
		const CValue& VTablePtrValue
		);

	bool
	CompileDefaultPreConstructor ();

	bool
	CompileDefaultDestructor ();

	bool
	CompilePrimer ();
};

//.............................................................................

inline
bool
IsClassType (
	CType* pType, 
	EClassType ClassTypeKind
	)
{
	return 
		pType->GetTypeKind () == EType_Class &&
		((CClassType*) pType)->GetClassTypeKind () == ClassTypeKind;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
