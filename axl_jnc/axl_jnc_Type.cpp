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
		
	case ETypeModifier_Dynamic:
		return _T("dynamic");

	case ETypeModifier_NoNull:
		return _T("nonull");

	case ETypeModifier_Property:
		return _T("property");

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
		pLlvmType = llvm::Type::getInt1Ty (llvm::getGlobalContext ());
		break;

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
	
	case EType_Qualifier:
		pLlvmType = ((CPointerType*) this)->GetBaseType ()->GetLlvmType ();
		break;
	
	case EType_Pointer:
	case EType_Reference:
	case EType_Pointer_d:
	case EType_Reference_d:
		pLlvmType = m_pModule->m_TypeMgr.GetLlvmFatPointerType ();
		break;

	case EType_Pointer_u:
	case EType_Reference_u:
		pLlvmType = llvm::PointerType::get (((CPointerType*) this)->GetBaseType ()->GetLlvmType (), 0);
		break;

	case EType_Array:
		return ((CArrayType*) this)->GetLlvmType ();
	
	case EType_BitField:
		pLlvmType = ((CPointerType*) this)->GetBaseType ()->GetLlvmType ();
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

	case EType_Import:
		return ((CImportType*) this)->GetExternType ()->GetLlvmType ();
	}

	m_pLlvmType = pLlvmType; // could be NULL
	m_Flags |= ETypeFlag_IsLlvmReady;
	return pLlvmType;
}

size_t
CType::GetAlignFactor ()
{
	switch (m_TypeKind)
	{
	case EType_Struct:
	case EType_Union:
		return ((CStructType*) this)->GetAlignFactor ();

	case EType_Import:
		return ((CImportType*) this)->GetExternType ()->GetAlignFactor ();

	default:
		return m_Size;
	};
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
	case EType_Qualifier:
		m_TypeString = ((CQualifierType*) this)->GetBaseType ()->GetTypeString ();

		if (m_Flags & ETypeQualifier_Const)
			m_TypeString.Append (_T(" const"));

		if (m_Flags & ETypeQualifier_Volatile)
			m_TypeString.Append (_T(" volatile"));

		if (m_Flags & ETypeQualifier_NoNull)
			m_TypeString.Append (_T(" nonull"));

		break;

	case EType_Pointer:
		m_TypeString.Format (_T("%s* safe"), ((CPointerType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Pointer_u:
		m_TypeString.Format (_T("%s* unsafe"), ((CPointerType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Pointer_d:
		m_TypeString.Format (_T("%s* dynamic"), ((CPointerType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Reference:
		m_TypeString.Format (_T("%s& safe"), ((CPointerType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Reference_u:
		m_TypeString.Format (_T("%s& unsafe"), ((CPointerType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Reference_d:
		m_TypeString.Format (_T("%s& dynamic"), ((CPointerType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_BitField:
		m_TypeString = ((CBitFieldType*) this)->CreateTypeString ();
		break;

	case EType_Enum:
		m_TypeString.Format (_T("enum %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Enum_c:
		m_TypeString.Format (_T("enumc %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Array:
		m_TypeString = ((CArrayType*) this)->CreateTypeString ();
		break;

	case EType_Struct:
		m_TypeString.Format (_T("struct %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Union:
		m_TypeString.Format (_T("union %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Interface:
		m_TypeString.Format (_T("interface %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Class:
		m_TypeString.Format (_T("class %s"), ((CNamedType*) this)->GetQualifiedName ());
		break;

	case EType_Function:
	case EType_Event:
		m_TypeString = ((CFunctionType*) this)->CreateTypeString ();
		break;

	case EType_Property:
		m_TypeString = ((CPropertyType*) this)->CreateTypeString ();
		break;

	case EType_Import:
		m_TypeString = ((CImportType*) this)->GetName ().GetFullName ();
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
		((CPointerType*) this)->GetBaseType ()->GetTypeKind () == EType_Char;
}

CPointerType* 
CType::GetPointerType (EType TypeKind)
{
	return m_pModule->m_TypeMgr.GetPointerType (this, TypeKind);
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
VerifyAntiModifier (
	int Modifiers,
	ETypeModifier Modifier,
	ETypeModifier AntiModifier
	)
{
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
	if (!pType->IsIntegerType ())
	{
		err::SetFormatStringError (
			_T("type modifier '%s' can only be applied to integer type"),
			GetTypeModifierString (Modifier)
			);
		return false;
	}

	return VerifyAntiModifier (Modifiers, Modifier, AntiModifier);
}

static
inline
bool
VerifyPointerModifier (
	CType* pType,
	int Modifiers,
	ETypeModifier Modifier,
	ETypeModifier AntiModifier1,
	ETypeModifier AntiModifier2
	)
{
	if (!pType->IsPointerType () && !pType->IsReferenceType ())
	{
		err::SetFormatStringError (
			_T("type modifier '%s' can only be applied to pointer or reference type"),
			GetTypeModifierString (Modifier)
			);
		return false;
	}

	return 
		VerifyAntiModifier (Modifiers, Modifier, AntiModifier1) &&
		VerifyAntiModifier (Modifiers, Modifier, AntiModifier2);
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

static
inline
EType
GetUnsafePointerTypeKind (EType TypeKind)
{
	return 
		TypeKind == EType_Pointer ? EType_Pointer_u :
		TypeKind == EType_Reference ? EType_Reference_u : TypeKind;
}

static
inline
EType
GetDynamicPointerTypeKind (EType TypeKind)
{
	return 
		TypeKind == EType_Pointer ? EType_Pointer_d :
		TypeKind == EType_Reference ? EType_Reference_d : TypeKind;
}

CType* 
CType::GetModifiedType (int Modifiers)
{
	CType* pType = this;

	if (Modifiers & ETypeModifier_Signed)
	{
		if (!VerifyIntegerModifier (pType, Modifiers, ETypeModifier_Signed, ETypeModifier_Unsigned))
			return NULL;

		// do nothing
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

		// do nothing
	}

	if (Modifiers & ETypeModifier_BigEndian)
	{
		if (!VerifyIntegerModifier (pType, Modifiers, ETypeModifier_BigEndian, ETypeModifier_LittleEndian))
			return NULL;

		EType ModTypeKind = GetBigEndianTypeKind (pType->m_TypeKind);
		pType = m_pModule->m_TypeMgr.GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_Safe)
	{
		if (!VerifyPointerModifier (pType, Modifiers, ETypeModifier_Safe, ETypeModifier_Unsafe, ETypeModifier_Dynamic))
			return NULL;

		// do nothing
	}

	if (Modifiers & ETypeModifier_Unsafe)
	{
		if (!VerifyPointerModifier (pType, Modifiers, ETypeModifier_Unsafe, ETypeModifier_Safe, ETypeModifier_Dynamic))
			return NULL;

		EType ModTypeKind = GetUnsafePointerTypeKind (pType->m_TypeKind);
		CPointerType* pPointerType = (CPointerType*) pType;
		pType = m_pModule->m_TypeMgr.GetPointerType (pPointerType->GetBaseType (), ModTypeKind);
	}

	if (Modifiers & ETypeModifier_Dynamic)
	{
		if (!VerifyPointerModifier (pType, Modifiers, ETypeModifier_Dynamic, ETypeModifier_Safe, ETypeModifier_Unsafe))
			return NULL;

		EType ModTypeKind = GetDynamicPointerTypeKind (pType->m_TypeKind);
		CPointerType* pPointerType = (CPointerType*) pType;
		pType = m_pModule->m_TypeMgr.GetPointerType (pPointerType->GetBaseType (), ModTypeKind);
	}

	if (Modifiers & ETypeModifier_NoNull)
	{
		if (!VerifyPointerModifier (pType, Modifiers, ETypeModifier_NoNull, (ETypeModifier) 0, (ETypeModifier) 0))
			return NULL;

		pType = m_pModule->m_TypeMgr.GetQualifiedType (pType, ETypeQualifier_NoNull);
	}

	if (Modifiers & ETypeModifier_Volatile)
	{
		pType = m_pModule->m_TypeMgr.GetQualifiedType (pType, ETypeQualifier_Volatile);
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
		pType = m_pModule->m_TypeMgr.GetQualifiedType (pType, ETypeQualifier_Const);
	}

	return pType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
