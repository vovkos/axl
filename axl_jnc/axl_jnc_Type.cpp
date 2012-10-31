#include "stdafx.h"
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
GetUInt32TypeKind (
	uint32_t Integer,
	bool ForceUnsigned
	)
{
	if (ForceUnsigned)
		return
			Integer <= UINT8_MAX ? EType_Int8_u :
			Integer <= UINT16_MAX ? EType_Int16_u : EType_Int32_u;
	else
		return 
			Integer <= INT8_MAX ? EType_Int8 :
			Integer <= UINT8_MAX ? EType_Int8_u :
			Integer <= INT16_MAX ? EType_Int16 : 
			Integer <= UINT16_MAX ? EType_Int16_u : 
			Integer <= INT32_MAX ? EType_Int32 : EType_Int32_u;
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
GetUInt64TypeKind (
	uint64_t Integer,
	bool ForceUnsigned
	)
{
	if (ForceUnsigned)
		return
			Integer <= UINT8_MAX ? EType_Int8_u :
			Integer <= UINT16_MAX ? EType_Int16_u : 
			Integer <= UINT32_MAX ? EType_Int32_u : EType_Int64_u;
	else
		return 
			Integer <= INT8_MAX ? EType_Int8 :
			Integer <= UINT8_MAX ? EType_Int8_u :
			Integer <= INT16_MAX ? EType_Int16 : 
			Integer <= UINT16_MAX ? EType_Int16_u : 
			Integer <= INT32_MAX ? EType_Int32 : 
			Integer <= UINT32_MAX ? EType_Int32_u : 
			Integer <= INT64_MAX ? EType_Int64 : EType_Int64_u;
}

//.............................................................................

const tchar_t*
GetTypeModifierString (ETypeModifier Modifier)
{
	switch (Modifier)
	{
	case ETypeModifier_Const:
		return _T("const");

	case ETypeModifier_Volatile:
		return _T("volatile");

	case ETypeModifier_Signed:
		return _T("signed");

	case ETypeModifier_Unsigned:
		return _T("unsigned");

	case ETypeModifier_LittleEndian:
		return _T("littleendian");

	case ETypeModifier_BigEndian:
		return _T("bigendian");

	case ETypeModifier_Safe:
		return _T("safe");

	case ETypeModifier_Unsafe:
		return _T("unsafe");

	case ETypeModifier_Reference:
		return _T("reference");

	case ETypeModifier_Pointer:
		return _T("pointer");

	case ETypeModifier_Property:
		return _T("property");

	case ETypeModifier_RemoveConst:
		return _T("remove-const");

	case ETypeModifier_RemoveReference:
		return _T("remove-reference");

	case ETypeModifier_RemovePointer:
		return _T("remove-pointer");

	case ETypeModifier_GetProperty:
		return _T("get-property");

	case ETypeModifier_ArrayToPointer:
		return _T("array-to-pointer");

	case ETypeModifier_EnumToInt:
		return _T("enum-to-pointer");

	default:
		return _T("undefined-type-modifier");
	};
}

//.............................................................................

CType::CType ()
{
	m_ItemKind = EModuleItem_Type;
	m_TypeKind = EType_Void;
	m_Flags = 0;
	m_Size = 0;
	m_pLlvmType = NULL;
}

llvm::Type* 
CType::GetLlvmType ()
{
	if (m_Flags & ETypeFlag_IsLlvmReady)
		return m_pLlvmType;

	llvm::Type* pLlvmType = NULL;

	switch (m_TypeKind)
	{
	case EType_Void:
		pLlvmType = llvm::Type::getVoidTy (llvm::getGlobalContext ());
		break;
	
	case EType_Bool:
	case EType_Int8:
	case EType_Int8_u:
		pLlvmType = llvm::Type::getInt8Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int16:
	case EType_Int16_u:
	case EType_Int16_be:
	case EType_Int16_beu:
		pLlvmType = llvm::Type::getInt16Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int32:
	case EType_Int32_u:
	case EType_Int32_be:
	case EType_Int32_beu:
		pLlvmType = llvm::Type::getInt32Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Int64:
	case EType_Int64_u:
	case EType_Int64_be:
	case EType_Int64_beu:
		pLlvmType = llvm::Type::getInt64Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Float:
		pLlvmType = llvm::Type::getFloatTy (llvm::getGlobalContext ());
		break;
	
	case EType_Double:
		pLlvmType = llvm::Type::getDoubleTy (llvm::getGlobalContext ());
		break;
	
	case EType_Variant:
		break;
	
	case EType_Const:
		pLlvmType = ((CDerivedType*) this)->GetBaseType ()->GetLlvmType ();
		break;
	
	case EType_Pointer:
	case EType_Reference:
		pLlvmType = m_pModule->m_TypeMgr.GetLlvmFatPointerType ();
		break;

	case EType_Pointer_c:
	case EType_Reference_c:
		pLlvmType = llvm::PointerType::get (((CDerivedType*) this)->GetBaseType ()->GetLlvmType (), 0);
		break;

	case EType_Array:
		return ((CArrayType*) this)->GetLlvmType ();
	
	case EType_BitField:
		pLlvmType = ((CDerivedType*) this)->GetBaseType ()->GetLlvmType ();
		break;
	
	case EType_Function:
		return ((CFunctionType*) this)->GetLlvmType ();

	case EType_Event:
		break;
	
	case EType_Enum:
	case EType_Enum_c:
		pLlvmType = llvm::Type::getInt32Ty (llvm::getGlobalContext ());
		break;
	
	case EType_Struct:
	case EType_Union:
		return ((CStructType*) this)->GetLlvmType ();
	
	case EType_Class:
	case EType_Interface:
		return ((CClassType*) this)->GetLlvmType ();
	
	case EType_Property:
		break;
	}

	m_pLlvmType = pLlvmType; // could be NULL
	m_Flags |= ETypeFlag_IsLlvmReady;
	return pLlvmType;
}

size_t
CType::GetAlignFactor ()
{
	return m_Size;
}

rtl::CString 
CType::GetTypeString ()
{
	if (!m_TypeString.IsEmpty ())
		return m_TypeString;

	static const tchar_t*
	BasicTypeNameTable [EType__BasicTypeCount] = 
	{
		_T("void"),
		_T("variant"),
		_T("bool"),
		_T("int8"),
		_T("unsigned int8"),
		_T("int16"),
		_T("unsigned int16"),
		_T("int32"),
		_T("unsigned int32"),
		_T("int64"),
		_T("unsigned int64"),
		_T("bigendian int16"),
		_T("unsigned bigendian int16"),
		_T("bigendian int32"),
		_T("unsigned bigendian int32"),
		_T("bigendian int64"),
		_T("unsigned bigendian int64"),
		_T("float"),
		_T("double"),
	};

	if (m_TypeKind < EType__BasicTypeCount)
	{
		m_TypeString = BasicTypeNameTable [m_TypeKind];
	}
	else switch (m_TypeKind)
	{	
	case EType_Const:
		m_TypeString.Format (_T("const %s"), ((CDerivedType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Volatile:
		m_TypeString.Format (_T("volatile %s"), ((CDerivedType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Pointer:
		m_TypeString.Format (_T("%s* safe"), ((CDerivedType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Pointer_c:
		m_TypeString.Format (_T("%s* unsafe"), ((CDerivedType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Reference:
		m_TypeString.Format (_T("%s& safe"), ((CDerivedType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Reference_c:
		m_TypeString.Format (_T("%s& unsafe"), ((CDerivedType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Array:
		m_TypeString = ((CArrayType*) this)->CreateTypeString ();
		break;

	case EType_BitField:
		m_TypeString = ((CBitFieldType*) this)->CreateTypeString ();
		break;

	case EType_Function:
	case EType_Event:
		m_TypeString = ((CFunctionType*) this)->CreateTypeString ();
		break;

	case EType_Enum:
		m_TypeString.Format (_T("enum %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Enum_c:
		m_TypeString.Format (_T("enumc %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Struct:
		m_TypeString.Format (_T("struct %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Union:
		m_TypeString.Format (_T("union %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Class:
		m_TypeString.Format (_T("class %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Interface:
		m_TypeString.Format (_T("interface %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Import:
		m_TypeString = ((CImportType*) this)->GetName ().GetFullName ();
		break;

	case EType_Property:
		m_TypeString = ((CPropertyType*) this)->CreateTypeString ();
		break;

	default:
		ASSERT (false);
	}

	return m_TypeString;
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
		((CArrayType*) this)->GetBaseType ()->GetTypeKind () == EType_Char;
}

bool 
CType::IsCharPointerType (CType* pType)
{
	return 
		m_TypeKind == EType_Pointer &&
		((CDerivedType*) this)->GetBaseType ()->GetTypeKind () == EType_Char;
}

bool 
CType::IsPropertyType ()
{
	return
		m_TypeKind == EType_Property ||
		m_TypeKind == EType_Const &&
		((CDerivedType*) this)->GetBaseType ()->GetTypeKind () == EType_Property;
}

bool 
CType::IsReferenceType ()
{
	return
		m_TypeKind == EType_Reference ||
		m_TypeKind == EType_Const &&
		((CDerivedType*) this)->GetBaseType ()->GetTypeKind () == EType_Reference;
}

CDerivedType* 
CType::GetDerivedType (EType TypeKind)
{
	return m_pModule->m_TypeMgr.GetDerivedType (this, TypeKind);
}

CArrayType* 
CType::GetArrayType (size_t ElementCount)
{
	return m_pModule->m_TypeMgr.GetArrayType (this, ElementCount);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

static
inline
bool
VerifyModifier (
	bool TypeKindCheck,
	const tchar_t* pTypeKindString,
	int Modifiers,
	ETypeModifier Modifier,
	ETypeModifier AntiModifier
	)
{
	if (!TypeKindCheck)
	{
		err::SetFormatStringError (
			_T("type modifier '%s' can only be applied to %s type"),
			GetTypeModifierString (Modifier),
			pTypeKindString
			);
		return false;
	}

	if (Modifiers & AntiModifier)
	{
		err::SetFormatStringError (
			_T("type modifiers '%s' and '%s' cannot be used together"),
			GetTypeModifierString (Modifier),
			GetTypeModifierString (AntiModifier)
			);
		return false;
	}

	return true;
}

static
inline
bool
VerifyIntegerModifier (
	CType* pType,
	int Modifiers,
	ETypeModifier Modifier,
	ETypeModifier AntiModifier
	)
{
	return VerifyModifier (pType->IsIntegerType (), _T("integer"), Modifiers, Modifier, AntiModifier);
}

static
inline
bool
VerifyPointerModifier (
	CType* pType,
	int Modifiers,
	ETypeModifier Modifier,
	ETypeModifier AntiModifier
	)
{
	return VerifyModifier (pType->IsPointerType (), _T("pointer"), Modifiers, Modifier, AntiModifier);
}

static
inline
EType
GetFatTypeKind (EType TypeKind)
{
	return 
		TypeKind == EType_Pointer_c ? EType_Pointer :
		TypeKind == EType_Reference_c ? EType_Reference : TypeKind;
}

static
inline
EType
GetThinTypeKind (EType TypeKind)
{
	return 
		TypeKind == EType_Pointer ? EType_Pointer_c :
		TypeKind == EType_Reference ? EType_Reference_c : TypeKind;
}

static
EType
GetSignedTypeKind (EType TypeKind)
{
	static EType ResultTypeTable [] = 
	{
		EType_Void,           // EType_Void
		EType_Variant,        // EType_Variant
		EType_Bool,           // EType_Bool
		EType_Int8,           // EType_Int8,           
		EType_Int8,           // EType_Int8_u,         
		EType_Int16,          // EType_Int16,          
		EType_Int16,          // EType_Int16_u,        
		EType_Int32,          // EType_Int32,          
		EType_Int32,          // EType_Int32_u,        
		EType_Int64,          // EType_Int64,          
		EType_Int64,          // EType_Int64_u,        
		EType_Int16_be,       // EType_Int16_be,       
		EType_Int16_be,       // EType_Int16_beu,      
		EType_Int32_be,       // EType_Int32_be,       
		EType_Int32_be,       // EType_Int32_beu,      
		EType_Int64_be,       // EType_Int64_be,       
		EType_Int64_be,       // EType_Int64_beu,      
	};

	ASSERT (TypeKind <= EType_Int64_beu);
	return ResultTypeTable [TypeKind];
}

static
EType
GetUnsignedTypeKind (EType TypeKind)
{
	static EType ResultTypeTable [] = 
	{
		EType_Void,           // EType_Void
		EType_Variant,        // EType_Variant
		EType_Bool,           // EType_Bool
		EType_Int8_u,         // EType_Int8,           
		EType_Int8_u,         // EType_Int8_u,         
		EType_Int16_u,        // EType_Int16,          
		EType_Int16_u,        // EType_Int16_u,        
		EType_Int32_u,        // EType_Int32,          
		EType_Int32_u,        // EType_Int32_u,        
		EType_Int64_u,        // EType_Int64,          
		EType_Int64_u,        // EType_Int64_u,        
		EType_Int16_beu,      // EType_Int16_be,       
		EType_Int16_beu,      // EType_Int16_beu,      
		EType_Int32_beu,      // EType_Int32_be,       
		EType_Int32_beu,      // EType_Int32_beu,      
		EType_Int64_beu,      // EType_Int64_be,       
		EType_Int64_beu,      // EType_Int64_beu,      
	};

	ASSERT (TypeKind <= EType_Int64_beu);
	return ResultTypeTable [TypeKind];
}

static
EType
GetLittleEndianTypeKind (EType TypeKind)
{
	static EType ResultTypeTable [] = 
	{
		EType_Void,           // EType_Void
		EType_Variant,        // EType_Variant
		EType_Bool,           // EType_Bool
		EType_Int8,           // EType_Int8,           
		EType_Int8_u,         // EType_Int8_u,         
		EType_Int16,          // EType_Int16,          
		EType_Int16_u,        // EType_Int16_u,        
		EType_Int32,          // EType_Int32,          
		EType_Int32_u,        // EType_Int32_u,        
		EType_Int64,          // EType_Int64,          
		EType_Int64_u,        // EType_Int64_u,        
		EType_Int16,          // EType_Int16_be,       
		EType_Int16_u,        // EType_Int16_beu,      
		EType_Int32,          // EType_Int32_be,       
		EType_Int32_u,        // EType_Int32_beu,      
		EType_Int64,          // EType_Int64_be,       
		EType_Int64_u,        // EType_Int64_beu,      
	};

	ASSERT (TypeKind <= EType_Int64_beu);
	return ResultTypeTable [TypeKind];
}

static
EType
GetBigEndianTypeKind (EType TypeKind)
{
	static EType ResultTypeTable [] = 
	{
		EType_Void,           // EType_Void
		EType_Variant,        // EType_Variant
		EType_Bool,           // EType_Bool
		EType_Int8,           // EType_Int8,           
		EType_Int8_u,         // EType_Int8_u,         
		EType_Int16_be,       // EType_Int16,          
		EType_Int16_beu,      // EType_Int16_u,        
		EType_Int32_be,       // EType_Int32,          
		EType_Int32_beu,      // EType_Int32_u,        
		EType_Int64_be,       // EType_Int64,          
		EType_Int64_beu,      // EType_Int64_u,        
		EType_Int16_be,       // EType_Int16_be,       
		EType_Int16_beu,      // EType_Int16_beu,      
		EType_Int32_be,       // EType_Int32_be,       
		EType_Int32_beu,      // EType_Int32_beu,      
		EType_Int64_be,       // EType_Int64_be,       
		EType_Int64_beu,      // EType_Int64_beu,      
	};

	ASSERT (TypeKind <= EType_Int64_beu);
	return ResultTypeTable [TypeKind];
}

CType* 
CType::GetModifiedType (int Modifiers)
{
	CType* pType = this;

	if (Modifiers & ETypeModifier_Signed)
	{
		if (!VerifyIntegerModifier (pType, Modifiers, ETypeModifier_Signed, ETypeModifier_Unsigned))
			return NULL;

		EType ModTypeKind = GetSignedTypeKind (pType->m_TypeKind);
		pType = m_pModule->m_TypeMgr.GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_Unsigned)
	{
		if (!VerifyIntegerModifier (pType, Modifiers, ETypeModifier_Unsigned, ETypeModifier_Signed))
			return NULL;

		EType ModTypeKind = GetUnsignedTypeKind (pType->m_TypeKind);
		pType = m_pModule->m_TypeMgr.GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_LittleEndian)
	{
		if (!VerifyIntegerModifier (pType, Modifiers, ETypeModifier_LittleEndian, ETypeModifier_BigEndian))
			return NULL;

		EType ModTypeKind = GetLittleEndianTypeKind (pType->m_TypeKind);
		pType = m_pModule->m_TypeMgr.GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_BigEndian)
	{
		if (!VerifyIntegerModifier (pType, Modifiers, ETypeModifier_BigEndian, ETypeModifier_LittleEndian))
			return NULL;

		EType ModTypeKind = GetBigEndianTypeKind (pType->m_TypeKind);
		pType = m_pModule->m_TypeMgr.GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_Pointer)
	{
		pType = m_pModule->m_TypeMgr.GetDerivedType (pType, EType_Pointer);
	}

	if (Modifiers & ETypeModifier_Reference)
	{
		pType = m_pModule->m_TypeMgr.GetDerivedType (pType, EType_Reference);
		if (!pType)
			return NULL;
	}

	if (Modifiers & ETypeModifier_Safe)
	{
		if (!VerifyPointerModifier (pType, Modifiers, ETypeModifier_Safe, ETypeModifier_Unsafe))
			return NULL;

		EType ModTypeKind = GetFatTypeKind (pType->m_TypeKind);
		CDerivedType* pPointerType = (CDerivedType*) pType;
		pType = m_pModule->m_TypeMgr.GetDerivedType (pPointerType->GetBaseType (), ModTypeKind);
	}

	if (Modifiers & ETypeModifier_Unsafe)
	{
		if (!VerifyPointerModifier (pType, Modifiers, ETypeModifier_Unsafe, ETypeModifier_Safe))
			return NULL;

		EType ModTypeKind = GetThinTypeKind (pType->m_TypeKind);
		CDerivedType* pPointerType = (CDerivedType*) pType;
		pType = m_pModule->m_TypeMgr.GetDerivedType (pPointerType->GetBaseType (), ModTypeKind);
	}

	if (Modifiers & ETypeModifier_RemoveConst)
	{
		if (pType->m_TypeKind == EType_Const)
			pType = ((CDerivedType*) pType)->GetBaseType ();
	}

	if (Modifiers & ETypeModifier_RemoveVolatile)
	{
		if (pType->m_TypeKind == EType_Volatile)
			pType = ((CDerivedType*) pType)->GetBaseType ();
	}

	if (Modifiers & ETypeModifier_RemoveReference)
	{
		if (pType->m_TypeKind == EType_Reference || pType->m_TypeKind == EType_Reference_c)
			pType = ((CDerivedType*) pType)->GetBaseType ();
	}

	if (Modifiers & ETypeModifier_RemovePointer)
	{
		if (pType->m_TypeKind == EType_Pointer || pType->m_TypeKind == EType_Pointer_c)
			pType = ((CDerivedType*) pType)->GetBaseType ();
	}

	if (Modifiers & ETypeModifier_Volatile)
	{
		pType = m_pModule->m_TypeMgr.GetDerivedType (pType, EType_Volatile);
	}

	if (Modifiers & ETypeModifier_Property)
	{
		CFunctionType* pGetterType = m_pModule->m_TypeMgr.GetFunctionType (pType, NULL, 0, 0);
		CFunctionType* pSetterType = !(Modifiers & ETypeModifier_Const) ? 
			m_pModule->m_TypeMgr.GetFunctionType (m_pModule->m_TypeMgr.GetBasicType (EType_Void), &pType, 1, 0) : 
			NULL;
		pType = m_pModule->m_TypeMgr.GetPropertyType (pGetterType, pSetterType);
	}
	else if (Modifiers & ETypeModifier_Const)
	{
		pType = m_pModule->m_TypeMgr.GetDerivedType (pType, EType_Const);
	}

	return pType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
