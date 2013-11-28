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
struct TInterface;

//.............................................................................

enum EClassType
{
	EClassType_Normal = 0,
	EClassType_StdObject, // EStdType_Object
	EClassType_Box,
	EClassType_Multicast,
	EClassType_McSnapshot,
	EClassType_Reactor,
	EClassType_ReactorIface,
	EClassType_FunctionClosure,
	EClassType_PropertyClosure,
	EClassType_DataClosure,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EClassTypeFlag
{
	EClassTypeFlag_Abstract = 0x010000,
	EClassTypeFlag_Opaque   = 0x020000,
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
	rtl::CArrayT <CStructField*> m_ClassMemberFieldArray;

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

	bool
	IsCreatable ()
	{
		return 
			m_ClassTypeKind != EClassType_StdObject && 
			!(m_Flags & (EClassTypeFlag_Abstract | EClassTypeFlag_Opaque));
	}

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
		CNamespace* pAnchorNamespace,
		EType TypeKind,
		EClassPtrType PtrTypeKind = EClassPtrType_Normal,
		uint_t Flags = 0
		);

	CClassPtrType*
	GetClassPtrType (
		EType TypeKind,
		EClassPtrType PtrTypeKind = EClassPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetClassPtrType (NULL, TypeKind, PtrTypeKind, Flags);
	}

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

	rtl::CArrayT <CBaseTypeSlot*> 
	GetBaseTypePrimeArray ()
	{
		return m_BaseTypePrimeArray;
	}

	rtl::CArrayT <CStructField*>
	GetClassMemberFieldArray ()
	{
		return m_ClassMemberFieldArray;
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

	virtual
	void
	EnumGcRoots (
		CRuntime* pRuntime,
		void* p
		);

	bool
	CallBaseTypeDestructors (const CValue& ThisValue);

	bool
	CallMemberFieldDestructors (const CValue& ThisValue);

	bool
	CallMemberPropertyDestructors (const CValue& ThisValue);

protected:
	void
	EnumGcRootsImpl (
		CRuntime* pRuntime,
		TInterface* pInterface
		);

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
	void
	PrepareLlvmDiType ()
	{
		m_LlvmDiType = GetClassStructType ()->GetLlvmDiType ();
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

enum EObjectFlag
{
	EObjectFlag_Alive     = 0x0001,
	EObjectFlag_Static    = 0x0010,
	EObjectFlag_Stack     = 0x0020,
	EObjectFlag_HeapU     = 0x0040,
	EObjectFlag_Extern    = 0x0080,
	EObjectFlag_GcMark    = 0x0100,
	EObjectFlag_GcMark_wc = 0x0200, // weak closure (function or property ptr)
};

// master header of class instance

struct TObject
{
	CClassType* m_pType; // for GC tracing & QueryInterface
	size_t m_ScopeLevel;
	uintptr_t m_Flags;
	rtl::TListLink m_GcHeapLink; // objects allocated on gc heap get into a list

	// followed by TInterface of object
};

class CObjectGcHeapLink
{
public:
	rtl::TListLink*
	operator () (TObject* pObject)
	{
		return &pObject->m_GcHeapLink;
	}
};

// header of class interface

struct TInterface
{
	void* m_pVTable;
	TObject* m_pObject; // back pointer to master header

	// followed by parents, then by interface data fields
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// interface with master header

template <typename T>
class CObjectT:
	public TObject,
	public T
{
public:
	CObjectT ()
	{
		m_ScopeLevel = 0;
		m_Flags = EObjectFlag_Alive | EObjectFlag_Extern;
		this->m_pType = NULL;   // should be primed later
		this->m_pObject = this; // thanks a log gcc
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef
void
FObject_Prime (TObject* pObject);

typedef
void
FObject_Construct (TInterface* pInterface);

typedef
void	
FObject_Destruct (TInterface* pInterface);

//.............................................................................

} // namespace jnc {
} // namespace axl {
