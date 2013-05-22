#include "pch.h"
#include "axl_jnc_Type.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

EType
GetInt32TypeKind (int32_t Integer)
{
	return
		Integer >= INT8_MIN && Integer <= INT8_MAX ? EType_Int8 :
		(uint32_t) Integer <= UINT8_MAX ? EType_Int8_u :
		Integer >= INT16_MIN && Integer <= INT16_MAX ? EType_Int16 : 
		(uint32_t) Integer <= UINT16_MAX ? EType_Int16_u : 
		Integer >= INT32_MIN && Integer <= INT32_MAX ? EType_Int32 : EType_Int32_u;
}

EType
GetInt32TypeKind_u (uint32_t Integer)
{
	return
		Integer <= UINT8_MAX ? EType_Int8_u :
		Integer <= UINT16_MAX ? EType_Int16_u : EType_Int32_u;
}

EType
GetInt64TypeKind (int64_t Integer)
{
	return
		Integer >= INT8_MIN && Integer <= INT8_MAX ? EType_Int8 :
		(uint64_t) Integer <= UINT8_MAX ? EType_Int8_u :
		Integer >= INT16_MIN && Integer <= INT16_MAX ? EType_Int16 : 
		(uint64_t) Integer <= UINT16_MAX ? EType_Int16_u : 
		Integer >= INT32_MIN && Integer <= INT32_MAX ? EType_Int32 : 
		(uint64_t) Integer <= UINT32_MAX ? EType_Int32_u : 
		Integer >= INT64_MIN && Integer <= INT64_MAX ? EType_Int64 : EType_Int64_u;
}

EType
GetInt64TypeKind_u (uint64_t Integer)
{
	return
		Integer <= UINT8_MAX ? EType_Int8_u :
		Integer <= UINT16_MAX ? EType_Int16_u : 
		Integer <= UINT32_MAX ? EType_Int32_u : EType_Int64_u;
}

//.............................................................................

rtl::CString 
GetLlvmTypeString (llvm::Type* pLlvmType)
{
	std::string s;
	llvm::raw_string_ostream Stream (s);
	pLlvmType->print (Stream);
	return Stream.str ().c_str ();
}

//.............................................................................

const char*
GetTypeModifierString (ETypeModifier Modifier)
{
	static const char* StringTable [] = 
	{
		"signed",        // ETypeModifier_Signed     = 0x00000001,
		"unsigned",      // ETypeModifier_Unsigned   = 0x00000002,
		"bigendian",     // ETypeModifier_BigEndian  = 0x00000004,
		"nonull",        // ETypeModifier_Nullable   = 0x00000008,
		"const",         // ETypeModifier_Const      = 0x00000010,
		"readonly",      // ETypeModifier_ReadOnly   = 0x00000020,
		"mutable",       // ETypeModifier_Mutable    = 0x00000040,
		"volatile",      // ETypeModifier_Volatile   = 0x00000080,
		"weak",          // ETypeModifier_Weak       = 0x00000100,
		"thin",          // ETypeModifier_Thin       = 0x00000200,
		"unsafe",        // ETypeModifier_Unsafe     = 0x00000400,
		"cdecl",         // ETypeModifier_Cdecl      = 0x00000800,
		"stdcall",       // ETypeModifier_Stdcall    = 0x00001000,
		"class",         // ETypeModifier_Class      = 0x00002000,
		"function",      // ETypeModifier_Function   = 0x00004000,
		"property",      // ETypeModifier_Property   = 0x00008000,
		"autoev",        // ETypeModifier_AutoEv     = 0x00010000,
		"bindable",      // ETypeModifier_Bindable   = 0x00020000,
		"autoget",       // ETypeModifier_AutoGet    = 0x00040000,
		"indexed",       // ETypeModifier_Indexed    = 0x00080000,
		"nonull",        // ETypeModifier_Nullable_p = 0x00100000,
		"const",         // ETypeModifier_Const_p    = 0x00200000,
		"weak",          // ETypeModifier_Weak_p     = 0x00400000,
		"unsafe",        // ETypeModifier_Unsafe_p   = 0x00800000,
		"event",         // ETypeModifier_Event      = 0x01000000,
	};

	size_t i = rtl::GetLoBitIdx32 (Modifier);
	return i < countof (StringTable) ? 
		StringTable [i] : 
		"undefined-type-modifier";
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char* 
GetPtrTypeFlagString (EPtrTypeFlag Flag)
{
	static const char* StringTable [] = 
	{
		"nullable", // EPtrTypeFlag_Nullable  = 0x0100,
		"const",    // EPtrTypeFlag_Const     = 0x0200,
		"readonly", // EPtrTypeFlag_ReadOnly  = 0x0400,
		"mutable",  // EPtrTypeFlag_Mutable   = 0x0800,
		"volatile", // EPtrTypeFlag_Volatile  = 0x1000,
		"event",    // EPtrTypeFlag_Event     = 0x2000,
		"this",     // EPtrTypeFlag_This      = 0x4000,
	};

	size_t i = rtl::GetLoBitIdx32 (Flag >> 8);

	return i < countof (StringTable) ? 
		StringTable [i] : 
		"undefined-ptr-type-flag";
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t
GetPtrTypeFlagsFromModifiers (uint_t Modifiers)
{
	uint_t Flags = 0;

	if (Modifiers & ETypeModifier_Nullable)
		Flags |= EPtrTypeFlag_Nullable;

	if (Modifiers & ETypeModifier_Const)
		Flags |= EPtrTypeFlag_Const;

	if (Modifiers & ETypeModifier_ReadOnly)
		Flags |= EPtrTypeFlag_ReadOnly;

	if (Modifiers & ETypeModifier_Volatile)
		Flags |= EPtrTypeFlag_Volatile;

	if (Modifiers & ETypeModifier_Event)
		Flags |= EPtrTypeFlag_Event;

	return Flags;
}

//.............................................................................

CType::CType ()
{
	m_ItemKind = EModuleItem_Type;
	m_TypeKind = EType_Void;
	m_Flags = 0;
	m_Size = 0;
	m_pLlvmType = NULL;
	m_pDataPtrTypeTuple = NULL;
	m_pPropertyTypeTuple = NULL;
	m_pFunctionArgTuple = NULL;
}

rtl::CString 
CType::GetTypeString ()
{
	if (!m_TypeString.IsEmpty ())
		return m_TypeString;

	PrepareTypeString ();
	
	ASSERT (!m_TypeString.IsEmpty ());
	return m_TypeString;
}

llvm::Type* 
CType::GetLlvmType ()
{
	if (m_pLlvmType)
		return m_pLlvmType;

	PrepareLlvmType ();
	
	ASSERT (m_pLlvmType);
	return m_pLlvmType;
}

CValue 
CType::GetUndefValue ()
{
	llvm::Value* pLlvmValue = llvm::UndefValue::get (GetLlvmType ());
	return CValue (pLlvmValue, this);
}

CValue 
CType::GetZeroValue ()
{
	llvm::Value* pLlvmValue = llvm::Constant::getNullValue (GetLlvmType ());
	return CValue (pLlvmValue, this);
}

bool
CType::PreCalcLayout ()
{
	ASSERT (!(m_Flags & ETypeFlag_LayoutReady));

	if (m_Flags & ETypeFlag_LayoutCalc)
	{
		err::SetFormatStringError ("can't calculate layout of '%s' due to type recursion", GetTypeString ().cc ());
		return false;
	}

	m_Flags |= ETypeFlag_LayoutCalc;
	return true;
}

void
CType::PostCalcLayout ()
{
	m_Flags &= ~ETypeFlag_LayoutCalc;
	m_Flags |= ETypeFlag_LayoutReady;
}

bool 
CType::IsAutoSizeArrayType ()
{
	return 
		m_TypeKind == EType_Array &&
		((CArrayType*) this)->GetElementCount () == 0;
}

bool 
CType::IsCharArrayType ()
{
	return 
		m_TypeKind == EType_Array &&
		((CArrayType*) this)->GetElementType ()->GetTypeKind () == EType_Char;
}

bool
CType::IsBindablePropertyType ()
{
	return 
		m_TypeKind == EType_PropertyRef &&
		(((CPropertyPtrType*) this)->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Bindable) != 0;
}

CArrayType* 
CType::GetArrayType (size_t ElementCount)
{
	return m_pModule->m_TypeMgr.GetArrayType (this, ElementCount);
}

CDataPtrType* 
CType::GetDataPtrType (
	EType TypeKind,
	EDataPtrType PtrTypeKind,
	uint_t Flags
	)
{
	return m_pModule->m_TypeMgr.GetDataPtrType (this, TypeKind, PtrTypeKind, Flags);
}

CFunctionArg* 
CType::GetSimpleFunctionArg (int PtrTypeFlags)
{
	return m_pModule->m_TypeMgr.GetSimpleFunctionArg (this, PtrTypeFlags);
}

void
CType::PrepareTypeString ()
{
	static const char* StringTable [EType__PrimitiveTypeCount] = 
	{
		"void",
		"variant",
		"bool",
		"int8",
		"unsigned int8",
		"int16",
		"unsigned int16",
		"int32",
		"unsigned int32",
		"int64",
		"unsigned int64",
		"bigendian int16",
		"unsigned bigendian int16",
		"bigendian int32",
		"unsigned bigendian int32",
		"bigendian int64",
		"unsigned bigendian int64",
		"float",
		"double",
	};

	ASSERT (m_TypeKind < EType__PrimitiveTypeCount);
	m_TypeString = StringTable [m_TypeKind];
}

void
CType::PrepareLlvmType ()
{
	ASSERT (m_TypeKind < EType__PrimitiveTypeCount);

	switch (m_TypeKind)
	{
	case EType_Void:
		m_pLlvmType = llvm::Type::getVoidTy (llvm::getGlobalContext ());
		break;
	
	case EType_Variant:
		ASSERT (false); // variants are not supported yet
		break;

	case EType_Bool:
		m_pLlvmType = llvm::Type::getInt1Ty (llvm::getGlobalContext ());
		break;

	case EType_Int8:
	case EType_Int8_u:
		m_pLlvmType = llvm::Type::getInt8Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int16:
	case EType_Int16_u:
	case EType_Int16_be:
	case EType_Int16_beu:
		m_pLlvmType = llvm::Type::getInt16Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int32:
	case EType_Int32_u:
	case EType_Int32_be:
	case EType_Int32_beu:
		m_pLlvmType = llvm::Type::getInt32Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int64:
	case EType_Int64_u:
	case EType_Int64_be:
	case EType_Int64_beu:
		m_pLlvmType = llvm::Type::getInt64Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Float:
		m_pLlvmType = llvm::Type::getFloatTy (llvm::getGlobalContext ());
		break;
	
	case EType_Double:
		m_pLlvmType = llvm::Type::getDoubleTy (llvm::getGlobalContext ());
		break;

	default:
		ASSERT (false);
	}
}

//.............................................................................

CGetType::CGetType (
	CModule* pModule, 
	EType TypeKind
	)
{
	m_pType = pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
}

CGetType::CGetType (
	CModule* pModule, 
	EStdType TypeKind
	)
{
	m_pType = pModule->m_TypeMgr.GetStdType (TypeKind);
}

CGetType::CGetType (EType TypeKind)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	m_pType = pModule->m_TypeMgr.GetPrimitiveType (TypeKind);
}

CGetType::CGetType (EStdType TypeKind)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	m_pType = pModule->m_TypeMgr.GetStdType (TypeKind);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
