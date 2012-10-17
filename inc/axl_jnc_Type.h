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
	EType_Bool,           // a
	EType_Int8,           // b
	EType_Int8_u,         // c
	EType_Int16,          // d
	EType_Int16_u,        // e
	EType_Int16_be,       // f
	EType_Int16_ube,      // g
	EType_Int32,          // h
	EType_Int32_u,        // i
	EType_Int32_be,       // j
	EType_Int32_ube,      // k
	EType_Int64,          // l
	EType_Int64_u,        // m
	EType_Int64_be,       // n
	EType_Int64_ube,      // o
	EType_Float,          // p
	EType_Double,         // q
	EType_Variant,        // z

	// derived types

	EType_Const,          // C
	EType_Pointer,        // P
	EType_Reference,      // R
	EType_Array,          // A
	EType_BitField,       // B
	EType_Function,       // F
	EType_Event,          // V
	EType_Enum,           // E
	EType_EnumC,          // N
	EType_Struct,         // S
	EType_Union,          // U
	EType_Class,          // X
	EType_Interface,      // I
	EType_Property,       // Y

	// import type (resolved after linkage or instantiation of generic)

	EType_Import,         // Z

	EType__Count,
	EType__BasicTypeCount = EType_Variant + 1,

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
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ETypeModifier
{
	ETypeModifier_Const             = 0x0001,
	ETypeModifier_Property          = 0x0002,
	ETypeModifier_Signed            = 0x0004,
	ETypeModifier_Unsigned          = 0x0008,
	ETypeModifier_BigEndian         = 0x0010,
	ETypeModifier_LittleEndian      = 0x0020,
	ETypeModifier_Reference         = 0x0100,
	ETypeModifier_Pointer           = 0x0200,	
	ETypeModifier_RemoveConst       = 0x0400,
	ETypeModifier_RemoveReference   = 0x0800,
	ETypeModifier_RemovePointer     = 0x1000,
	ETypeModifier_GetProperty       = 0x2000,
	ETypeModifier_ArrayToPointer    = 0x4000,
	ETypeModifier_EnumToInt         = 0x8000,
};

const tchar_t*
GetTypeModifierString (ETypeModifier Modifier);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

/*

enum ETypeMod 
{
	ETypeMod_Reference       = 0x00000001,
	ETypeMod_Dereference     = 0x00000002,
	ETypeMod_FullDereference = 0x00000004,

	ETypeMod_Addr            = 0x00000010,
	ETypeMod_Indir           = 0x00000020,
	ETypeMod_FullIndir       = 0x00000040,

	ETypeMod_GetProperty     = 0x00000100,
	ETypeMod_ArrayToPointer  = 0x00000200,

	ETypeMod_Const           = 0x00001000,
	ETypeMod_Unconst         = 0x00002000,

	ETypeMod_Move            = ETypeMod_FullDereference,
	ETypeMod_MoveRef         = ETypeMod_FullDereference,
	ETypeMod_Operator        = ETypeMod_FullDereference | ETypeMod_ArrayToPointer,
};

*/

//.............................................................................

class CType: public CModuleItem
{
protected:
	friend class CTypeMgr;

	CTypeMgr* m_pTypeMgr;

	EType m_TypeKind;
	ulong_t m_Flags;
	size_t m_Size; // not really necessary for most types, but it's nice to have it in plain text

	rtl::CStringA m_Signature;
	rtl::CString m_TypeString;

public:
	CType ();

	CTypeMgr* 
	GetTypeMgr ()
	{
		return m_pTypeMgr;
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

	size_t
	GetAlignFactor ();

	rtl::CStringA
	GetSignature ()
	{
		return m_Signature;
	}

	rtl::CString 
	GetTypeString ();

	bool 
	IsIntegerType ()
	{
		return m_TypeKind >= EType_Int8 && m_TypeKind <= EType_Int64_ube;
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
	GetConstType ();

	CDerivedType* 
	GetPointerType ();

	CDerivedType* 
	GetReferenceType ();

	CArrayType* 
	GetArrayType (size_t ElementCount);

	CType* 
	GetTypeMod (int Modifiers);
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

struct TFatPointer
{
	void* m_p;
	void* m_pParent;
	CType* m_pType;
	CType* m_pParentType;
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
