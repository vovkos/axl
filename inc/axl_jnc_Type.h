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

//.............................................................................

enum EType
{
	// simple types

	EType_Void,           // v
	EType_Variant,        // z
	EType_Bool,           // a
	
	// little-endian integers
	
	EType_Int8,           // b
	EType_Int8_u,         // c
	EType_Int16,          // d
	EType_Int16_u,        // e
	EType_Int32,          // f
	EType_Int32_u,        // g
	EType_Int64,          // h
	EType_Int64_u,        // i

	// big-endian integers

	EType_Int16_be,       // j
	EType_Int16_beu,      // k
	EType_Int32_be,       // l
	EType_Int32_beu,      // m
	EType_Int64_be,       // n
	EType_Int64_beu,      // o

	// floating point 

	EType_Float,          // p
	EType_Double,         // q

	// derived types

	EType_Const,          // C
	EType_Volatile,       // V
	EType_Pointer,        // P
	EType_Pointer_c,      // O
	EType_Reference,      // R
	EType_Reference_c,    // H
	EType_Array,          // A
	EType_BitField,       // B
	EType_Function,       // F
	EType_Event,          // V
	EType_Enum,           // E
	EType_Enum_c,          // N
	EType_Struct,         // S
	EType_Union,          // U
	EType_Class,          // X
	EType_Interface,      // I
	EType_Property,       // Y

	// import type (resolved after linkage or instantiation of generic)

	EType_Import,         // Z

	EType__Count,
	EType__BasicTypeCount = EType_Double + 1,

	// aliases

	EType_Int    = EType_Int32,
	EType_Char   = EType_Int8,
	EType_UChar  = EType_Int8_u,
	EType_Byte   = EType_Int8_u,
	EType_WChar  = EType_Int16,
	EType_Short  = EType_Int16,
	EType_UShort = EType_Int16_u,
	EType_Word   = EType_Int16_u,
	EType_Long   = EType_Int32,
	EType_ULong  = EType_Int32_u,
	EType_DWord  = EType_Int32_u,
	EType_QWord  = EType_Int64_u,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ETypeFlag
{
	ETypeFlag_IsIncomplete = 0x01,
	ETypeFlag_IsLlvmReady  = 0x02,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ETypeModifier
{
	ETypeModifier_Const             = 0x00001,
	ETypeModifier_Volatile          = 0x00002,
	ETypeModifier_Signed            = 0x00004,
	ETypeModifier_Unsigned          = 0x00008,
	ETypeModifier_BigEndian         = 0x00010,
	ETypeModifier_LittleEndian      = 0x00020,
	ETypeModifier_Safe              = 0x00040,
	ETypeModifier_Unsafe            = 0x00080,	
	ETypeModifier_Reference         = 0x00100,
	ETypeModifier_Pointer           = 0x00200,	
	ETypeModifier_Property          = 0x00400,
	ETypeModifier_RemoveConst       = 0x00800,
	ETypeModifier_RemoveVolatile    = 0x01000,
	ETypeModifier_RemoveReference   = 0x02000,
	ETypeModifier_RemovePointer     = 0x04000,
	ETypeModifier_GetProperty       = 0x08000,
	ETypeModifier_ArrayToPointer    = 0x10000,
	ETypeModifier_EnumToInt         = 0x20000,
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
	ulong_t m_Flags;
	size_t m_Size; // not really necessary for most types, but it's nice to have it in plain text

	rtl::CStringA m_Signature;
	rtl::CString m_TypeString;

	llvm::Type* m_pLlvmType;

public:
	CType ();

	llvm::Type* 
	GetLlvmType ();

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
		return m_Signature.Cmp (pType->m_Signature);
	}

	bool 
	IsIntegerType ()
	{
		return m_TypeKind >= EType_Int8 && m_TypeKind <= EType_Int64_beu;
	}
	
	bool 
	IsSignedType ()
	{
		return IsIntegerType () && !(m_TypeKind & 1); // even types are signed
	}

	bool 
	IsNumericType ()
	{
		return m_TypeKind >= EType_Bool && m_TypeKind <= EType_Double;
	}

	bool 
	IsPointerType ()
	{
		return m_TypeKind >= EType_Pointer && m_TypeKind <= EType_Reference_c;
	}

	bool 
	IsPropertyType (); // property or const property

	bool 
	IsReferenceType (); // reference or const reference

	bool 
	IsAutoSizeArrayType ();

	bool 
	IsCharArrayType ();

	bool 
	IsCharPointerType (CType* pType);

	CDerivedType* 
	GetDerivedType (EType TypeKind);

	CArrayType* 
	GetArrayType (size_t ElementCount);

	CType* 
	GetModifiedType (int Modifiers);
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
