// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ModuleItem.h"

namespace axl {
namespace jnc {

class CTypeMgr;
class CArrayType;
class CStructType;
class CClassType;
class CPropertyType;
class CDataPtrType;
class CFunctionArg;
class CValue;
class CRuntime;

struct TDataPtrTypeTuple;
struct TSimplePropertyTypeTuple;
struct TFunctionArgTuple;

//.............................................................................

enum EType
{
	// primitive types (completely identified by EType)

	EType_Void,                // v
	EType_Variant,             // z
	EType_Bool,                // b
	
	// little-endian integers
	
	EType_Int8,                // is1
	EType_Int8_u,              // iu1
	EType_Int16,               // is2
	EType_Int16_u,             // iu2
	EType_Int32,               // is4
	EType_Int32_u,             // iu4
	EType_Int64,               // is8
	EType_Int64_u,             // iu8

	// big-endian integers

	EType_Int16_be,            // ibs2
	EType_Int16_beu,           // ibu2
	EType_Int32_be,            // ibs4
	EType_Int32_beu,           // ibu4
	EType_Int64_be,            // ibs8
	EType_Int64_beu,           // ibu8

	// floating point 

	EType_Float,               // f4
	EType_Double,              // f8

	// derived types

	EType_Array,               // A
	EType_BitField,            // B

	// named types
	 
	EType_Enum,                // E
	EType_Struct,              // SS/SP (struct/pointer struct)
	EType_Union,               // U
	EType_Class,               // CC/CO/CB/CA/CF/CD (class/object/box/autoev-iface/f-closure/d-closure)
	
	// function types

	EType_Function,            // F
	EType_Property,            // X
	
	// pointers & references

	EType_DataPtr,             // PD
	EType_DataRef,             // RD
	EType_ClassPtr,            // PC
	EType_ClassRef,            // RC
	EType_FunctionPtr,         // PF
	EType_FunctionRef,         // RF
	EType_PropertyPtr,         // PX
	EType_PropertyRef,         // RX

	// import types (resolved after linkage)

	EType_NamedImport,         // ZN
	EType_ImportPtr,           // ZP

	EType__Count,
	EType__EndianDelta = EType_Int16_be - EType_Int16,
	EType__PrimitiveTypeCount = EType_Double + 1,

	// aliases

#if (_AXL_PTR_BITNESS == 64)
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

//.............................................................................

enum EStdType
{
	EStdType_BytePtr,
	EStdType_ObjectHdr,
	EStdType_ObjectHdrPtr,
	EStdType_ObjectClass,
	EStdType_ObjectPtr,
	EStdType_SimpleFunction,
	EStdType_SimpleMulticast,
	EStdType_SimpleEventPtr,
	EStdType_Binder,
	EStdType_AutoEvBindSite,
	EStdType_IScheduler,
	EStdType_ISchedulerPtr,
	EStdType_FmtLiteral,
	EStdType__Count,
};

//.............................................................................

enum ETypeModifier
{
	ETypeModifier_Unsigned    = 0x00000001,
	ETypeModifier_BigEndian   = 0x00000002,
	ETypeModifier_Const       = 0x00000004,
	ETypeModifier_PubConst    = 0x00000008,
	ETypeModifier_Volatile    = 0x00000010,
	ETypeModifier_Weak        = 0x00000020,
	ETypeModifier_Thin        = 0x00000040,
	ETypeModifier_Unsafe      = 0x00000080,
	ETypeModifier_Cdecl       = 0x00000100,
	ETypeModifier_Stdcall     = 0x00000200,
	ETypeModifier_Function    = 0x00000400,
	ETypeModifier_Property    = 0x00000800,
	ETypeModifier_Bindable    = 0x00001000,
	ETypeModifier_AutoGet     = 0x00002000,
	ETypeModifier_Indexed     = 0x00004000,
	ETypeModifier_Multicast   = 0x00008000,
	ETypeModifier_Event       = 0x00010000,
	ETypeModifier_PubEvent    = 0x00020000,
	ETypeModifier_AutoEv      = 0x00040000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ETypeModifierMask
{
	ETypeModifierMask_Integer = 
		ETypeModifier_Unsigned |
		ETypeModifier_BigEndian,

	ETypeModifierMask_Function = 
		ETypeModifier_Function | 
		ETypeModifier_Cdecl | 
		ETypeModifier_Stdcall,

	ETypeModifierMask_Property = 
		ETypeModifier_Property | 
		ETypeModifier_Cdecl | 
		ETypeModifier_Stdcall |
		ETypeModifier_Const | 
		ETypeModifier_Bindable | 
		ETypeModifier_Indexed,

	ETypeModifierMask_DataPtr = 
		ETypeModifier_Const | 
		ETypeModifier_PubConst | 
		ETypeModifier_Volatile |
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,

	ETypeModifierMask_ClassPtr = 
		ETypeModifier_Const | 
		ETypeModifier_PubConst | 
		ETypeModifier_Volatile |
		ETypeModifier_Event | 
		ETypeModifier_PubEvent | 
		ETypeModifier_Weak |
		ETypeModifier_Unsafe,
	
	ETypeModifierMask_FunctionPtr = 
		ETypeModifier_Weak | 
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,

	ETypeModifierMask_PropertyPtr = 
		ETypeModifier_Weak | 
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,

	ETypeModifierMask_ImportPtr = 
		ETypeModifierMask_DataPtr |
		ETypeModifierMask_ClassPtr |
		ETypeModifierMask_FunctionPtr |
		ETypeModifierMask_PropertyPtr,

	ETypeModifierMask_DeclPtr = 
		ETypeModifier_Const | 
		ETypeModifier_PubConst | 
		ETypeModifier_Volatile |
		ETypeModifier_Event | 
		ETypeModifier_PubEvent,

	ETypeModifierMask_CallConv = 
		ETypeModifier_Cdecl |
		ETypeModifier_Stdcall,

	ETypeModifierMask_PtrKind = 
		ETypeModifier_Weak |
		ETypeModifier_Thin,

	ETypeModifierMask_TypeKind = 
		ETypeModifier_Function |
		ETypeModifier_Property |
		ETypeModifier_Multicast |
		ETypeModifier_AutoEv,

	ETypeModifierMask_Const = 
		ETypeModifier_Const |
		ETypeModifier_PubConst |
		ETypeModifier_Event |
		ETypeModifier_PubEvent,

	ETypeModifierMask_Event = 
		ETypeModifier_Event |
		ETypeModifier_PubEvent |
		ETypeModifier_Const |
		ETypeModifier_PubConst |
		ETypeModifierMask_TypeKind,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ETypeModifier
GetFirstTypeModifier (uint_t Modifiers)
{
	return (ETypeModifier) (1 << rtl::GetLoBitIdx (Modifiers));
}

const char*
GetTypeModifierString (ETypeModifier Modifier);

rtl::CString
GetTypeModifierString (uint_t Modifiers);

inline
const char*
GetFirstTypeModifierString (uint_t Modifiers)
{
	return GetTypeModifierString (GetFirstTypeModifier (Modifiers));
}

//.............................................................................

enum ETypeFlag
{
	ETypeFlag_Named        = 0x0100,
	ETypeFlag_Child        = 0x0200, // constructor has an implicit 'parent' arg
	ETypeFlag_Pod          = 0x0400, // plain-old-data
	ETypeFlag_GcRoot       = 0x0800, // is or contains gc-traceable pointers
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EPtrTypeFlag
{
	EPtrTypeFlag_Unsafe    = 0x010000, // all ptr
	EPtrTypeFlag_Checked   = 0x020000, // all ptr
	EPtrTypeFlag_Const     = 0x040000, // class & data ptr
	EPtrTypeFlag_PubConst  = 0x080000, // class & data ptr
	EPtrTypeFlag_Volatile  = 0x100000, // class & data ptr
	EPtrTypeFlag_Event     = 0x200000, // class only
	EPtrTypeFlag_PubEvent  = 0x400000, // class only

	EPtrTypeFlag__AllMask  = 0x0f0000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EPtrTypeFlag
GetFirstPtrTypeFlag (uint_t Flags)
{
	return (EPtrTypeFlag) (1 << rtl::GetLoBitIdx (Flags));
}

const char* 
GetPtrTypeFlagString (EPtrTypeFlag Flag);

rtl::CString
GetPtrTypeFlagString (uint_t Flags);

rtl::CString
GetPtrTypeFlagSignature (uint_t Flags);

inline
const char* 
GetFirstPtrTypeFlagString (uint_t Flags)
{
	return GetPtrTypeFlagString (GetFirstPtrTypeFlag (Flags));
}

uint_t
GetPtrTypeFlagsFromModifiers (uint_t Modifiers);

//.............................................................................

// data ptr

enum EDataPtrType
{
	EDataPtrType_Normal = 0,
	EDataPtrType_Thin,
	EDataPtrType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetDataPtrTypeKindString (EDataPtrType PtrTypeKind);

//.............................................................................

// useful for simple checks

enum ETypeKindFlag
{
	ETypeKindFlag_Integer      = 0x00000001,
	ETypeKindFlag_Signed       = 0x00000002,
	ETypeKindFlag_Unsigned     = 0x00000004,
	ETypeKindFlag_LittleEndian = 0x00000008,
	ETypeKindFlag_BigEndian    = 0x00000010,
	ETypeKindFlag_Fp           = 0x00000020,
	ETypeKindFlag_Numeric      = 0x00000040,
	ETypeKindFlag_Named        = 0x00000080,
	ETypeKindFlag_Derivable    = 0x00000100,
	ETypeKindFlag_DataPtr      = 0x00000200,
	ETypeKindFlag_ClassPtr     = 0x00000400,
	ETypeKindFlag_FunctionPtr  = 0x00000800,
	ETypeKindFlag_PropertyPtr  = 0x00001000,
	ETypeKindFlag_Ptr          = 0x00002000,
	ETypeKindFlag_Ref          = 0x00004000,
	ETypeKindFlag_Import       = 0x00008000,
	ETypeKindFlag_Code         = 0x00010000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t 
GetTypeKindFlags (EType TypeKind);

//.............................................................................

// integer type utils

EType
GetInt32TypeKind (int32_t Integer);

EType
GetInt32TypeKind_u (uint32_t Integer);

EType
GetInt64TypeKind (int64_t Integer);

EType
GetInt64TypeKind_u (uint64_t Integer);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EType
GetBigEndianIntegerTypeKind (EType TypeKind)
{
	return (GetTypeKindFlags (TypeKind) & ETypeKindFlag_LittleEndian) ? 
		(EType) (TypeKind + EType__EndianDelta) : 
		TypeKind;
}

inline
EType
GetLittleEndianIntegerTypeKind (EType TypeKind)
{
	return (GetTypeKindFlags (TypeKind) & ETypeKindFlag_BigEndian) ? 
		(EType) (TypeKind - EType__EndianDelta) : 
		TypeKind;
}

inline
EType
GetUnsignedIntegerTypeKind (EType TypeKind)
{
	return (GetTypeKindFlags (TypeKind) & ETypeKindFlag_Signed) ? 
		(EType) (TypeKind + 1) : 
		TypeKind;
}

inline
EType
GetSignedIntegerTypeKind (EType TypeKind)
{
	return (GetTypeKindFlags (TypeKind) & ETypeKindFlag_Unsigned) ? 		
		(EType) (TypeKind - 1) : 
		TypeKind;
}

inline
bool
IsEquivalentIntegerTypeKind (
	EType TypeKind1,
	EType TypeKind2
	)
{
	return GetSignedIntegerTypeKind (TypeKind1) == GetSignedIntegerTypeKind (TypeKind2);
}

//.............................................................................

rtl::CString 
GetLlvmTypeString (llvm::Type* pLlvmType);

//.............................................................................

class CType: public CModuleItem
{
	friend class CTypeMgr;

protected:
	EType m_TypeKind;
	size_t m_Size;
	size_t m_AlignFactor;
	rtl::CString m_Signature;
	rtl::CString m_TypeString;
	llvm::Type* m_pLlvmType;

	CClassType* m_pBoxClassType;
	TSimplePropertyTypeTuple* m_pSimplePropertyTypeTuple;
	TFunctionArgTuple* m_pFunctionArgTuple;
	TDataPtrTypeTuple* m_pDataPtrTypeTuple;

public:
	CType ();

	EType
	GetTypeKind ()
	{
		return m_TypeKind;
	}

	uint_t 
	GetTypeKindFlags ()
	{
		return jnc::GetTypeKindFlags (m_TypeKind);
	}

	size_t
	GetSize ()
	{
		return m_Size;
	}

	size_t
	GetAlignFactor ()
	{
		return m_AlignFactor;
	}

	rtl::CString
	GetSignature ()
	{
		return m_Signature;
	}

	rtl::CString 
	GetTypeString ();

	rtl::CString 
	GetLlvmTypeString ()
	{
		return jnc::GetLlvmTypeString (GetLlvmType ());
	}

	llvm::Type* 
	GetLlvmType ();

	CValue 
	GetUndefValue ();

	CValue 
	GetZeroValue ();

	int
	Cmp (CType* pType)
	{
		return pType != this ? m_Signature.Cmp (pType->m_Signature) : 0;
	}

	CArrayType* 
	GetArrayType (size_t ElementCount);

	CDataPtrType* 
	GetDataPtrType (
		CNamespace* pAnchorNamespace,
		EType TypeKind,
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		uint_t Flags = 0
		);

	CDataPtrType* 
	GetDataPtrType (
		EType TypeKind,
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetDataPtrType (NULL, TypeKind, PtrTypeKind, Flags);
	}

	CDataPtrType* 
	GetDataPtrType (
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetDataPtrType (EType_DataPtr, PtrTypeKind, Flags);
	}

	CDataPtrType* 
	GetDataPtrType_c ()
	{
		return GetDataPtrType (EType_DataPtr, EDataPtrType_Thin, EPtrTypeFlag_Unsafe);
	}

	CFunctionArg* 
	GetSimpleFunctionArg (uint_t PtrTypeFlags = 0);

	CClassType* 
	GetBoxClassType ();

	virtual 
	void
	EnumGcRoots (
		CRuntime* pRuntime,
		void* p
		)
	{
		ASSERT (false);
	}

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ();

	virtual 
	bool
	CalcLayout ()
	{
		ASSERT (m_Size && m_AlignFactor);
		return true;
	}
};

//.............................................................................

class CTypedef: public CUserModuleItem
{
	friend class CTypeMgr;

protected:
	CType* m_pType;

public:
	CTypedef ()
	{
		m_ItemKind = EModuleItem_Typedef;
		m_pType  = NULL;
	}

	CType*
	GetType ()
	{
		return m_pType;
	}
};

//.............................................................................

CType* 
GetSimpleType (
	CModule* pModule,
	EType TypeKind
	);

CType* 
GetSimpleType (
	CModule* pModule,
	EStdType StdTypeKind
	);

//.............................................................................

} // namespace jnc {
} // namespace axl {

