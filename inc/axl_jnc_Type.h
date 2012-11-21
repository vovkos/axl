// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

class CTypeMgr;
class CDerivedType;
class CArrayType;

typedef CDerivedType CPointerType;
typedef CDerivedType CQualifierType;

//.............................................................................

enum EType
{
	// basic types:

	EType_Void,           // a
	EType_Variant,        // b
	EType_Bool,           // c
	
	// little-endian integers
	
	EType_Int8,           // d
	EType_Int8_u,         // e
	EType_Int16,          // f
	EType_Int16_u,        // g
	EType_Int32,          // h
	EType_Int32_u,        // i
	EType_Int64,          // j
	EType_Int64_u,        // k

	// big-endian integers

	EType_Int16_be,       // l
	EType_Int16_beu,      // m
	EType_Int32_be,       // n
	EType_Int32_beu,      // o
	EType_Int64_be,       // p
	EType_Int64_beu,      // q

	// floating point 

	EType_Float,          // r
	EType_Double,         // s

	// derived types:

	// qualifiers: const, volatile, nonull

	EType_Qualifier,      // A 

	// pointers & references

	EType_Pointer,        // B
	EType_Pointer_u,      // C
	EType_Reference,      // E
	EType_Reference_u,    // F

	// integer derivatives

	EType_BitField,       // H
	EType_Enum,           // I
	EType_Enum_c,         // J

	// aggregates

	EType_Array,          // K
	EType_Struct,         // L
	EType_Union,          // M
	EType_Interface,      // N
	EType_Class,          // O

	// function types

	EType_Function,       // P
	EType_Event,          // Q
	EType_Property,       // R

	// import type (resolved after linkage or instantiation of generic)

	EType_Import,         // Z

	EType__Count,
	EType__BasicTypeCount = EType_Double + 1,

	// aliases

#ifdef _WIN64
	EType_Int_p    = EType_Int64,
	EType_Int_pu   = EType_Int64_u,
	EType_Int_pbe  = EType_Int64_be,
	EType_Int_pbeu = EType_Int64_beu,
#else
	EType_Int_p    = EType_Int32,
	EType_Int_pu   = EType_Int32_u,
	EType_Int_pbe  = EType_Int32_be,
	EType_Int_pbeu = EType_Int32_beu,
#endif

	EType_SizeT    = EType_Int_pu,
	EType_Int      = EType_Int32,
	EType_Char     = EType_Int8,
	EType_UChar    = EType_Int8_u,
	EType_Byte     = EType_Int8_u,
	EType_WChar    = EType_Int16,
	EType_Short    = EType_Int16,
	EType_UShort   = EType_Int16_u,
	EType_Word     = EType_Int16_u,
	EType_Long     = EType_Int32,
	EType_ULong    = EType_Int32_u,
	EType_DWord    = EType_Int32_u,
	EType_QWord    = EType_Int64_u,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ETypeQualifier
{
	ETypeQualifier_Const     = 0x01,
	ETypeQualifier_Volatile  = 0x02,
	ETypeQualifier_NoNull    = 0x04,
	ETypeQualifier_Weak      = 0x08,
	ETypeQualifier_Bindable  = 0x10,

	ETypeQualifier__Mask     = 0x1f,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ETypeFlag
{
	ETypeFlag_IsPod          = 0x200, // plain-old-data
	ETypeFlag_IsIncomplete   = 0x400,
	ETypeFlag_IsLlvmReady    = 0x800,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ETypeModifier
{
	ETypeModifier_Const        = 0x00001,
	ETypeModifier_Volatile     = 0x00002,
	ETypeModifier_Signed       = 0x00004,
	ETypeModifier_Unsigned     = 0x00008,
	ETypeModifier_BigEndian    = 0x00010,
	ETypeModifier_LittleEndian = 0x00020,
	ETypeModifier_Safe         = 0x00040,
	ETypeModifier_Unsafe       = 0x00080,	
	ETypeModifier_NoNull       = 0x00100,	
	ETypeModifier_Strong       = 0x00200,
	ETypeModifier_Weak         = 0x00400,	
	ETypeModifier_Property     = 0x01000,
	ETypeModifier_Bindable     = 0x02000,
};

const tchar_t*
GetTypeModifierString (ETypeModifier Modifier);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

EType
GetInt32TypeKind (int32_t Integer);

EType
GetUInt32TypeKind (
	uint32_t Integer,
	bool ForceUnsigned
	);

EType
GetInt64TypeKind (int64_t Integer);

EType
GetUInt64TypeKind (
	uint64_t Integer,
	bool ForceUnsigned
	);

//.............................................................................

class CType: public CModuleItem
{
protected:
	friend class CTypeMgr;

	EType m_TypeKind;
	size_t m_Size;
	int m_Flags;
		
	rtl::CStringA m_Signature;
	rtl::CString m_TypeString;

	llvm::Type* m_pLlvmType;

public:
	CType ();
	
	llvm::Type* 
	GetLlvmType ();

	llvm::UndefValue*
	GetLlvmUndefValue ()
	{
		return llvm::UndefValue::get (GetLlvmType ());
	}

	EType
	GetTypeKind ()
	{
		return m_TypeKind;
	}

	size_t
	GetSize ()
	{
		return m_Size;
	}

	int
	GetFlags ()
	{
		return m_Flags;
	}

	size_t
	GetAlignFactor ();

	rtl::CStringA
	GetSignature ()
	{
		return m_Signature;
	}

	rtl::CString 
	GetTypeString ();

	int
	Cmp (CType* pType)
	{
		return pType != this ? m_Signature.Cmp (pType->m_Signature) : 0;
	}

	bool 
	IsIntegerType ()
	{
		return m_TypeKind >= EType_Int8 && m_TypeKind <= EType_Int64_beu;
	}

	bool 
	IsFpType ()
	{
		return m_TypeKind == EType_Float || m_TypeKind == EType_Double;
	}

	bool 
	IsSignedType ()
	{
		return IsIntegerType () && (m_TypeKind & 1) != 0; // odd types are signed
	}

	bool 
	IsUnsignedType ()
	{
		return !IsSignedType ();
	}

	bool
	IsNumericType ()
	{
		return m_TypeKind >= EType_Bool && m_TypeKind <= EType_Double;
	}

	bool
	IsPointerType ()
	{
		return m_TypeKind == EType_Pointer || m_TypeKind == EType_Pointer_u;
	}
	
	bool
	IsDoubleReferenceType ();

	bool
	IsReferenceType ()
	{
		return m_TypeKind == EType_Reference || m_TypeKind == EType_Reference_u;
	}

	bool 
	IsAutoSizeArrayType ();

	bool 
	IsCharArrayType ();

	bool 
	IsCharPointerType (CType* pType);

	CPointerType* 
	GetPointerType (EType TypeKind);

	CArrayType* 
	GetArrayType (size_t ElementCount);

	CType* 
	GetModifiedType (int Modifiers);

	CType*
	GetUnqualifiedType ();
};

//.............................................................................

class CDerivedType: 
	public CType,
	public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CType* m_pBaseType;	

public:
	CDerivedType ()
	{
		m_pBaseType = NULL;
	}

	CType* 
	GetBaseType ()
	{
		return m_pBaseType;
	}
};

//.............................................................................

class CNamedType: 
	public CType,
	public CNamespace,
	public rtl::TListLink
{
public:
	CNamedType ()
	{
		m_NamespaceKind = ENamespace_NamedType;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
