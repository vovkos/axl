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
class CPropertyType;
class CDataPtrType;
class CDataPtrTypeTuple;
class CPropertyTypeTuple;
class CFunctionArgTuple;
class CFunctionArg;
class CValue;

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
	EType_Struct,              // S
	EType_Union,               // U
	EType_Class,               // C/O/X (class/object/box)

	// function types

	EType_Function,            // FC/FS (cdecl/stdcall)
	EType_Property,            // YY/YB (normal/bindable)
	
	// pointers & references

	EType_DataPtr,             // PD
	EType_DataRef,             // RD
	EType_ClassPtr,            // PC
	EType_FunctionPtr,         // PF
	EType_FunctionRef,         // RF
	EType_PropertyPtr,         // PY
	EType_PropertyRef,         // RY
	EType_AutoEvPtr,           // PV
	EType_AutoEvRef,           // RV
	
	// multicast

	EType_Multicast,           // M
	EType_McSnapshot,          // Ms

	// autoev

	EType_AutoEv,              // V

	// import type (resolved after linkage or instantiation of generic)

	EType_Import,              // ZN/ZD/ZP (name/data/pointer)

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
	EStdType_ObjectClass,
	EStdType_ObjectPtr,
	EStdType_SimpleFunction,
	EStdType_SimpleMulticast,
	EStdType_SimpleEventPtr,
	EStdType_StrenthenClosureFunction,
	EStdType_AutoEvBindSite,
	EStdType_IScheduler,
	EStdType_ISchedulerPtr,
	EStdType__Count,
};

//.............................................................................

enum ETypeFlag
{
	ETypeFlag_LayoutReady  = 0x0001,
	ETypeFlag_LayoutCalc   = 0x0002,
	ETypeFlag_Named        = 0x0004,
	ETypeFlag_Moveable     = 0x0008, // moveable by byte-to-byte copy
	ETypeFlag_Pod          = 0x0010, // plain-old-data
	ETypeFlag_ImportLoop   = 0x0020, // used for detection of import loops
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EPtrTypeFlag
{
	EPtrTypeFlag_Nullable  = 0x0100, // all 
	EPtrTypeFlag_Const     = 0x0200, // class & data ptr
	EPtrTypeFlag_ReadOnly  = 0x0400, // data ptr only
	EPtrTypeFlag_Mutable   = 0x0800, // data ptr only
	EPtrTypeFlag_Volatile  = 0x1000, // data ptr only
	EPtrTypeFlag_Event     = 0x2000, // multicast ptr only
	EPtrTypeFlag_This      = 0x4000, // 'this' arguments have this flag
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

inline
const char* 
GetFirstPtrTypeFlagString (uint_t Flags)
{
	return GetPtrTypeFlagString (GetFirstPtrTypeFlag (Flags));
}

uint_t
GetPtrTypeFlagsFromModifiers (uint_t Modifiers);

//.............................................................................

enum ETypeModifier
{
	ETypeModifier_Signed     = 0x00000001,
	ETypeModifier_Unsigned   = 0x00000002,
	ETypeModifier_BigEndian  = 0x00000004,
	ETypeModifier_Nullable   = 0x00000008,
	ETypeModifier_Const      = 0x00000010,
	ETypeModifier_ReadOnly   = 0x00000020,
	ETypeModifier_Mutable    = 0x00000040,
	ETypeModifier_Volatile   = 0x00000080,
	ETypeModifier_Weak       = 0x00000100,
	ETypeModifier_Thin       = 0x00000200,
	ETypeModifier_Unsafe     = 0x00000400,
	ETypeModifier_Cdecl      = 0x00000800,
	ETypeModifier_Stdcall    = 0x00001000,
	ETypeModifier_Class      = 0x00002000,
	ETypeModifier_Function   = 0x00004000,
	ETypeModifier_Property   = 0x00008000,
	ETypeModifier_AutoEv     = 0x00010000,
	ETypeModifier_Bindable   = 0x00020000,
	ETypeModifier_AutoGet    = 0x00040000,
	ETypeModifier_Indexed    = 0x00080000,

	// since 'class' implies pointer w/o requiring '*' symbol
	// we need to somehow distinguish where to apply certain modifier.
	// we 'promote' conflicting modifiers upon discovering 'class' or 'import' type
	// thus applying it to the leftmost site on the right of the modifier, e.g.

	// nonull weak CClass nonull weak function* x ();
	// nonull const unsafe CClass nonull const unsafe* y;

	ETypeModifier_Nullable_p = 0x00100000,
	ETypeModifier_Const_p    = 0x00200000,
	ETypeModifier_Weak_p     = 0x00400000,
	ETypeModifier_Unsafe_p   = 0x00800000,

	// this modifier is sort of virtual: it doesnt come from the parser
	// its created during type calculation upon discovering EDeclPrefix_Event

	ETypeModifier_Event      = 0x01000000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ETypeModifierMask
{
	ETypeModifierMask_Integer = 
		ETypeModifier_Signed | 
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
		ETypeModifier_AutoGet | 
		ETypeModifier_Indexed,

	ETypeModifierMask_DataPtr = 
		ETypeModifier_Nullable |
		ETypeModifier_Const | 
		ETypeModifier_ReadOnly | 
		ETypeModifier_Mutable | 
		ETypeModifier_Volatile |
		ETypeModifier_Unsafe |
		ETypeModifier_Event,

	ETypeModifierMask_ClassPtr = 
		ETypeModifier_Nullable |
		ETypeModifier_Const | 
		ETypeModifier_Weak |
		ETypeModifier_Unsafe,

	ETypeModifierMask_ClassPtr_p = 
		ETypeModifier_Nullable_p |
		ETypeModifier_Const_p | 
		ETypeModifier_Weak_p |
		ETypeModifier_Unsafe_p,
		
	ETypeModifierMask_FunctionPtr = 
		ETypeModifier_Cdecl | 
		ETypeModifier_Stdcall |
		ETypeModifier_Nullable |
		ETypeModifier_Weak | 
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,

	ETypeModifierMask_PropertyPtr = 
		ETypeModifier_Cdecl | 
		ETypeModifier_Stdcall |
		ETypeModifier_Bindable | 
		ETypeModifier_AutoGet | 
		ETypeModifier_Indexed |
		ETypeModifier_Nullable |
		ETypeModifier_Weak | 
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,

	ETypeModifierMask_AutoEvPtr = 
		ETypeModifier_Nullable |
		ETypeModifier_Weak | 
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,

	ETypeModifierMask_ImportPtr = 
		ETypeModifierMask_DataPtr |
		ETypeModifierMask_ClassPtr |
		ETypeModifierMask_ClassPtr_p |
		ETypeModifierMask_FunctionPtr |
		ETypeModifierMask_PropertyPtr |
		ETypeModifierMask_AutoEvPtr,

	ETypeModifierMask_Multicast = 
		ETypeModifier_Cdecl |
		ETypeModifier_Stdcall |
		ETypeModifier_Nullable |
		ETypeModifier_Weak |
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,
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

enum EDataPtrType
{
	EDataPtrType_Normal = 0,
	EDataPtrType_Thin,
	EDataPtrType_Unsafe,
	EDataPtrType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetDataPtrTypeKindString (EDataPtrType PtrTypeKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EDataPtrType
GetDataPtrTypeKindFromModifiers (uint_t Modifiers)
{
	return (Modifiers & ETypeModifier_Unsafe) ? EDataPtrType_Unsafe : EDataPtrType_Normal;
}

//.............................................................................

EType
GetInt32TypeKind (int32_t Integer);

EType
GetInt32TypeKind_u (uint32_t Integer);

EType
GetInt64TypeKind (int64_t Integer);

EType
GetInt64TypeKind_u (uint64_t Integer);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// checks

inline
bool
IsIntegerTypeKind (EType TypeKind)
{
	return TypeKind >= EType_Int8 && TypeKind <= EType_Int64_beu;
}

inline
bool
IsLittleEndianIntegerTypeKind (EType TypeKind)
{
	return TypeKind >= EType_Int8 && TypeKind <= EType_Int64_u;
}

inline
bool
IsBigEndianIntegerTypeKind (EType TypeKind)
{
	return TypeKind >= EType_Int16_be && TypeKind <= EType_Int64_beu;
}

inline
bool
IsSignedIntegerTypeKind (EType TypeKind)
{
	return IsIntegerTypeKind (TypeKind) && (TypeKind & 1) != 0;
}

inline
bool
IsUnsignedIntegerTypeKind (EType TypeKind)
{
	return IsIntegerTypeKind (TypeKind) && (TypeKind & 1) == 0;
}

inline
bool
IsEquivalentIntegerTypeKind (
	EType TypeKind1,
	EType TypeKind2
	)
{
	return IsSignedIntegerTypeKind (TypeKind1) == IsSignedIntegerTypeKind (TypeKind2);
}

inline 
bool 
IsFpTypeKind (EType TypeKind)
{
	return TypeKind == EType_Float || TypeKind == EType_Double;
}

inline
bool
IsNumericTypeKind (EType TypeKind)
{
	return TypeKind >= EType_Bool && TypeKind <= EType_Double;
}

inline
bool
IsDerivableTypeKind (EType TypeKind)
{
	return TypeKind >= EType_Struct && TypeKind <= EType_Class;
}

inline 
bool 
IsDataPtrTypeKind (EType TypeKind)
{
	return TypeKind == EType_DataPtr || TypeKind == EType_DataRef;
}

inline 
bool 
IsFunctionPtrTypeKind (EType TypeKind)
{
	return TypeKind == EType_FunctionPtr || TypeKind == EType_FunctionRef;
}

inline 
bool 
IsPropertyPtrTypeKind (EType TypeKind)
{
	return TypeKind == EType_PropertyPtr || TypeKind == EType_PropertyRef;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// conversions

inline
EType
GetBigEndianIntegerTypeKind (EType TypeKind)
{
	return TypeKind >= EType_Int16 && TypeKind <= EType_Int64_u ? (EType) (TypeKind + EType__EndianDelta) : TypeKind;
}

inline
EType
GetLittleEndianIntegerTypeKind (EType TypeKind)
{
	return IsBigEndianIntegerTypeKind (TypeKind) ? (EType) (TypeKind - EType__EndianDelta) : TypeKind;
}

inline
EType
GetUnsignedIntegerTypeKind (EType TypeKind)
{
	return IsSignedIntegerTypeKind (TypeKind) ? (EType) (TypeKind + 1) : TypeKind;
}

inline
EType
GetSignedIntegerTypeKind (EType TypeKind)
{
	return IsUnsignedIntegerTypeKind (TypeKind) ? (EType) (TypeKind - 1) : TypeKind;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

rtl::CString 
GetLlvmTypeString (llvm::Type* pLlvmType);

//.............................................................................

class CType: public CModuleItem
{
	friend class CTypeMgr;

protected:
	EType m_TypeKind;
	size_t m_Size;
	int m_Flags;
	rtl::CString m_Signature;
	rtl::CString m_TypeString;	
	llvm::Type* m_pLlvmType;

	CDataPtrTypeTuple* m_pDataPtrTypeTuple;
	CPropertyTypeTuple* m_pPropertyTypeTuple;
	CFunctionArgTuple* m_pFunctionArgTuple;

public:
	CType ();

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

	virtual
	size_t
	GetAlignFactor ()
	{
		return m_Size;
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

	bool
	IsIntegerType ()
	{
		return IsIntegerTypeKind (m_TypeKind);
	}

	bool
	IsLittleEndianIntegerType ()	
	{
		return IsLittleEndianIntegerTypeKind (m_TypeKind);
	}

	bool
	IsBigEndianIntegerType ()	
	{
		return IsBigEndianIntegerTypeKind (m_TypeKind);
	}

	bool
	IsSignedIntegerType ()	
	{
		return IsSignedIntegerTypeKind (m_TypeKind);
	}

	bool
	IsUnsignedIntegerType ()	
	{
		return IsUnsignedIntegerTypeKind (m_TypeKind);
	}

	bool
	IsEquivalentIntegerType (CType* pType)	
	{
		return IsEquivalentIntegerTypeKind (m_TypeKind, pType->m_TypeKind);
	}

	bool 
	IsFpType ()	
	{
		return IsFpTypeKind (m_TypeKind);
	}

	bool
	IsNumericType ()	
	{
		return IsNumericTypeKind (m_TypeKind);
	}

	bool
	IsDerivableType ()	
	{
		return IsDerivableTypeKind (m_TypeKind);
	}

	bool 
	IsDataPtrType ()	
	{
		return IsDataPtrTypeKind (m_TypeKind);
	}

	bool 
	IsFunctionPtrType ()	
	{
		return IsFunctionPtrTypeKind (m_TypeKind);
	}

	bool 
	IsPropertyPtrType ()	
	{
		return IsPropertyPtrTypeKind (m_TypeKind);
	}

	bool 
	IsAutoSizeArrayType ();

	bool 
	IsCharArrayType ();

	bool
	IsBindablePropertyType ();

	CArrayType* 
	GetArrayType (size_t ElementCount);

	CDataPtrType* 
	GetDataPtrType (
		EType TypeKind,
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		uint_t Flags = 0
		);

	CDataPtrType* 
	GetDataPtrType (
		EDataPtrType PtrTypeKind = EDataPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetDataPtrType (EType_DataPtr, PtrTypeKind, Flags);
	}

	CFunctionArg* 
	GetSimpleFunctionArg (int PtrTypeFlags = 0);

	virtual 
	bool
	CalcLayout ()
	{
		return true;
	}

protected:
	bool 
	PreCalcLayout ();

	void
	PostCalcLayout ();

	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ();
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

class CGetType 
{
protected:
	CType* m_pType;

public:
	CGetType (
		CModule* pModule, 
		EType TypeKind
		);

	CGetType (
		CModule* pModule, 
		EStdType TypeKind
		);

	CGetType (EType TypeKind);

	CGetType (EStdType TypeKind);

	operator CType* ()
	{
		return m_pType;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {