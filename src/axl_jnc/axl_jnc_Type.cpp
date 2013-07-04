#include "pch.h"
#include "axl_jnc_Type.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

uint_t 
GetTypeKindFlags (EType TypeKind)
{
	static uint_t FlagTable [EType__Count] = 
	{
		0,                           // EType_Void
		0,                           // EType_Variant

		ETypeKindFlag_Numeric,       // EType_Bool		
				
		ETypeKindFlag_Signed |       // EType_Int8
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Unsigned |     // EType_Int8_u
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Signed |       // EType_Int16
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Unsigned |     // EType_Int16_u                
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Signed |       // EType_Int32
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Unsigned |     // EType_Int32_u                
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Signed |       // EType_Int64
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Unsigned |     // EType_Int64_u               
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Signed |       // EType_Int16_be
		ETypeKindFlag_BigEndian |
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Unsigned |     // EType_Int16_beu
		ETypeKindFlag_BigEndian |
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Signed |       // EType_Int32_be
		ETypeKindFlag_BigEndian |
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Unsigned |     // EType_Int32_beu              
		ETypeKindFlag_BigEndian |
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Signed |       // EType_Int64_be
		ETypeKindFlag_BigEndian |
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Unsigned |     // EType_Int64_beu                
		ETypeKindFlag_BigEndian |
		ETypeKindFlag_Integer |  
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Fp |           // EType_Float
		ETypeKindFlag_Numeric,

		ETypeKindFlag_Fp |           // EType_Double
		ETypeKindFlag_Numeric,

		0,                           // EType_Array
		0,                           // EType_BitField

		ETypeKindFlag_Named,         // EType_Enum

		ETypeKindFlag_Derivable |    // EType_Struct
		ETypeKindFlag_Named,

		ETypeKindFlag_Derivable |    // EType_Union
		ETypeKindFlag_Named,

		ETypeKindFlag_Derivable |    // EType_Class
		ETypeKindFlag_Named,

		ETypeKindFlag_Code,          // EType_Function
		ETypeKindFlag_Code,          // EType_Property

		ETypeKindFlag_DataPtr |      // EType_DataPtr
		ETypeKindFlag_Ptr,

		ETypeKindFlag_DataPtr |      // EType_DataRef
		ETypeKindFlag_Ptr |
		ETypeKindFlag_Ref,

		ETypeKindFlag_ClassPtr |     // EType_ClassPtr
		ETypeKindFlag_Ptr,

		ETypeKindFlag_ClassPtr |     // EType_ClassRef
		ETypeKindFlag_Ptr |
		ETypeKindFlag_Ref,

		ETypeKindFlag_FunctionPtr |  // EType_FunctionPtr
		ETypeKindFlag_Ptr,

		ETypeKindFlag_FunctionPtr |  // EType_FunctionRef
		ETypeKindFlag_Ptr |
		ETypeKindFlag_Ref,

		ETypeKindFlag_PropertyPtr |  // EType_PropertyPtr
		ETypeKindFlag_Ptr,

		ETypeKindFlag_PropertyPtr |  // EType_PropertyRef
		ETypeKindFlag_Ptr |
		ETypeKindFlag_Ref,

		ETypeKindFlag_Import,        // EType_NamedImport

		ETypeKindFlag_Import |       // EType_ImportPtr
		ETypeKindFlag_Ptr,
	};

	return TypeKind < EType__Count ? FlagTable [TypeKind] : 0;
}

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
		"unsigned",     // ETypeModifier_Unsigned    = 0x00000001,
		"bigendian",    // ETypeModifier_BigEndian   = 0x00000002,
		"const",        // ETypeModifier_Const       = 0x00000004,
		"readonly",     // ETypeModifier_ReadOnly    = 0x00000008,
		"volatile",     // ETypeModifier_Volatile    = 0x00000010,
		"weak",         // ETypeModifier_Weak        = 0x00000020,
		"thin",         // ETypeModifier_Thin        = 0x00000040,
		"unsafe",       // ETypeModifier_Unsafe      = 0x00000080,
		"cdecl",        // ETypeModifier_Cdecl       = 0x00000100,
		"stdcall",      // ETypeModifier_Stdcall     = 0x00000200,
		"function",     // ETypeModifier_Function    = 0x00000400,
		"property",     // ETypeModifier_Property    = 0x00000800,
		"bindable",     // ETypeModifier_Bindable    = 0x00001000,
		"autoget",      // ETypeModifier_AutoGet     = 0x00002000,
		"indexed",      // ETypeModifier_Indexed     = 0x00004000,
		"multicast",    // ETypeModifier_Multicast   = 0x00008000,
		"event",        // ETypeModifier_Event       = 0x00010000,
		"autoev",       // ETypeModifier_AutoEv      = 0x00020000,
	};

	size_t i = rtl::GetLoBitIdx32 (Modifier);
	return i < countof (StringTable) ? 
		StringTable [i] : 
		"undefined-type-modifier";
}

rtl::CString
GetTypeModifierString (uint_t Modifiers)
{
	if (!Modifiers)
		return rtl::CString ();

	ETypeModifier Modifier = GetFirstTypeModifier (Modifiers);
	rtl::CString String = GetTypeModifierString (Modifier);
	Modifiers &= ~Modifier;

	while (Modifiers)
	{
		Modifier = GetFirstTypeModifier (Modifiers);

		String += ' ';
		String += GetTypeModifierString (Modifier);

		Modifiers &= ~Modifier;
	}

	return String;
}

//.............................................................................

const char* 
GetPtrTypeFlagString (EPtrTypeFlag Flag)
{
	static const char* StringTable [] = 
	{
		"unsafe",   // EPtrTypeFlag_Unsafe   = 0x1000,
		"checked",  // EPtrTypeFlag_Checked  = 0x2000,
		"const",    // EPtrTypeFlag_Const    = 0x4000,
		"volatile", // EPtrTypeFlag_Volatile = 0x8000,
	};

	size_t i = rtl::GetLoBitIdx32 (Flag >> 12);

	return i < countof (StringTable) ? 
		StringTable [i] : 
		"undefined-ptr-type-flag";
}

rtl::CString
GetPtrTypeFlagString (uint_t Flags)
{
	rtl::CString String;

	if (Flags & EPtrTypeFlag_Unsafe)
		String = "unsafe";
	else if (Flags & EPtrTypeFlag_Checked)
		String = "checked";

	if (Flags & EPtrTypeFlag_Const)
	{
		if (!String.IsEmpty ())
			String += ' ';

		String += "const";
	}

	if (Flags & EPtrTypeFlag_Volatile)
	{
		if (!String.IsEmpty ())
			String += ' ';

		String += "volatile";
	}

	return String;
}

rtl::CString
GetPtrTypeFlagSignature (uint_t Flags)
{
	rtl::CString Signature;

	if (Flags & EPtrTypeFlag_Unsafe)
		Signature = 'u';
	else if (Flags & EPtrTypeFlag_Checked)
		Signature = 's';

	if (Flags & EPtrTypeFlag_Const)
		Signature += 'c';

	if (Flags & EPtrTypeFlag_Volatile)
		Signature += 'v';

	return Signature;
}

uint_t
GetPtrTypeFlagsFromModifiers (uint_t Modifiers)
{
	uint_t Flags = 0;

	if (Modifiers & ETypeModifier_Unsafe)
		Flags |= EPtrTypeFlag_Unsafe;

	if (Modifiers & ETypeModifier_Const)
		Flags |= EPtrTypeFlag_Const;

	if (Modifiers & ETypeModifier_Volatile)
		Flags |= EPtrTypeFlag_Volatile;

	return Flags;
}

//.............................................................................

CType::CType ()
{
	m_ItemKind = EModuleItem_Type;
	m_TypeKind = EType_Void;
	m_Size = 0;
	m_AlignFactor = 0;
	m_pLlvmType = NULL;
	m_pSimplePropertyTypeTuple = NULL;
	m_pFunctionArgTuple = NULL;
	m_pDataPtrTypeTuple = NULL;
	m_pBoxClassType = NULL;
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
CType::GetSimpleFunctionArg (uint_t PtrTypeFlags)
{
	return m_pModule->m_TypeMgr.GetSimpleFunctionArg (this, PtrTypeFlags);
}

CClassType* 
CType::GetBoxClassType ()
{
	return m_pModule->m_TypeMgr.GetBoxClassType (this);
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

} // namespace jnc {
} // namespace axl {
