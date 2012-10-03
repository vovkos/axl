#include "stdafx.h"
#include "axl_jnc_Type.h"
#include "axl_jnc_TypeMgr.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetTypeModifierString (ETypeModifier Modifier)
{
	switch (Modifier)
	{
	case ETypeModifier_Const:
		return _T("const");

	case ETypeModifier_Property:
		return _T("property");

	case ETypeModifier_Signed:
		return _T("signed");

	case ETypeModifier_Unsigned:
		return _T("unsigned");

	case ETypeModifier_BigEndian:
		return _T("bigendian");

	case ETypeModifier_LittleEndian:
		return _T("littleendian");

	case ETypeModifier_Reference:
		return _T("reference");

	case ETypeModifier_Pointer:
		return _T("pointer");

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
	m_pTypeMgr = NULL;
	m_ItemKind = EModuleItem_Type;
	m_TypeKind = EType_Void;
	m_Size = 0;
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
		_T("bool"),
		_T("int8"),
		_T("unsigned int8"),
		_T("int16"),
		_T("unsigned int16"),
		_T("bigendian int16"),
		_T("unsigned bigendian int16"),
		_T("int32"),
		_T("unsigned int32"),
		_T("bigendian int32"),
		_T("unsigned bigendian int32"),
		_T("int64"),
		_T("unsigned int64"),
		_T("bigendian int64"),
		_T("unsigned bigendian int64"),
		_T("float"),
		_T("double"),
		_T("variant"),
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

	case EType_Pointer:
		m_TypeString.Format (_T("%s*"), ((CDerivedType*) this)->GetBaseType ()->GetTypeString ());
		break;

	case EType_Reference:
		m_TypeString.Format (_T("%s&"), ((CDerivedType*) this)->GetBaseType ()->GetTypeString ());
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

	case EType_EnumC:
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
CType::GetConstType ()
{
	return m_pTypeMgr->GetConstType (this);
}

CDerivedType* 
CType::GetPointerType ()
{
	return m_pTypeMgr->GetPointerType (this);
}

CDerivedType* 
CType::GetReferenceType ()
{
	return m_pTypeMgr->GetReferenceType (this);
}

CArrayType* 
CType::GetArrayType (size_t ElementCount)
{
	return m_pTypeMgr->GetArrayType (this, ElementCount);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

static
bool
CheckIntegerModifier (
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
EType
GetSignedTypeKind (EType TypeKind)
{
	static EType ResultTypeTable [] = 
	{
		EType_Void,           // EType_Void
		EType_Bool,           // EType_Bool
		EType_Int8,           // EType_Int8,           
		EType_Int8,           // EType_Int8_u,         
		EType_Int16,          // EType_Int16,          
		EType_Int16,          // EType_Int16_u,        
		EType_Int16_be,       // EType_Int16_be,       
		EType_Int16_be,       // EType_Int16_ube,      
		EType_Int32,          // EType_Int32,          
		EType_Int32,          // EType_Int32_u,        
		EType_Int32_be,       // EType_Int32_be,       
		EType_Int32_be,       // EType_Int32_ube,      
		EType_Int64,          // EType_Int64,          
		EType_Int64,          // EType_Int64_u,        
		EType_Int64_be,       // EType_Int64_be,       
		EType_Int64_be,       // EType_Int64_ube,      
	};

	ASSERT (TypeKind <= EType_Int64_ube);
	return ResultTypeTable [TypeKind];
}

static
EType
GetUnsignedTypeKind (EType TypeKind)
{
	static EType ResultTypeTable [] = 
	{
		EType_Void,           // EType_Void
		EType_Bool,           // EType_Bool
		EType_Int8_u,         // EType_Int8,           
		EType_Int8_u,         // EType_Int8_u,         
		EType_Int16_u,        // EType_Int16,          
		EType_Int16_u,        // EType_Int16_u,        
		EType_Int16_ube,      // EType_Int16_be,       
		EType_Int16_ube,      // EType_Int16_ube,      
		EType_Int32_u,        // EType_Int32,          
		EType_Int32_u,        // EType_Int32_u,        
		EType_Int32_ube,      // EType_Int32_be,       
		EType_Int32_ube,      // EType_Int32_ube,      
		EType_Int64_u,        // EType_Int64,          
		EType_Int64_u,        // EType_Int64_u,        
		EType_Int64_ube,      // EType_Int64_be,       
		EType_Int64_ube,      // EType_Int64_ube,      
	};

	ASSERT (TypeKind <= EType_Int64_ube);
	return ResultTypeTable [TypeKind];
}

static
EType
GetBigEndianTypeKind (EType TypeKind)
{
	static EType ResultTypeTable [] = 
	{
		EType_Void,           // EType_Void
		EType_Bool,           // EType_Bool
		EType_Int8,           // EType_Int8,           
		EType_Int8_u,         // EType_Int8_u,         
		EType_Int16_be,       // EType_Int16,          
		EType_Int16_ube,      // EType_Int16_u,        
		EType_Int16_be,       // EType_Int16_be,       
		EType_Int16_ube,      // EType_Int16_ube,      
		EType_Int32_be,       // EType_Int32,          
		EType_Int32_ube,      // EType_Int32_u,        
		EType_Int32_be,       // EType_Int32_be,       
		EType_Int32_ube,      // EType_Int32_ube,      
		EType_Int64_be,       // EType_Int64,          
		EType_Int64_ube,      // EType_Int64_u,        
		EType_Int64_be,       // EType_Int64_be,       
		EType_Int64_ube,      // EType_Int64_ube,      
	};

	ASSERT (TypeKind <= EType_Int64_ube);
	return ResultTypeTable [TypeKind];
}

static
EType
GetLittleEndianTypeKind (EType TypeKind)
{
	static EType ResultTypeTable [] = 
	{
		EType_Void,           // EType_Void
		EType_Bool,           // EType_Bool
		EType_Int8,           // EType_Int8,           
		EType_Int8_u,         // EType_Int8_u,         
		EType_Int16,          // EType_Int16,          
		EType_Int16_u,        // EType_Int16_u,        
		EType_Int16,          // EType_Int16_be,       
		EType_Int16_u,        // EType_Int16_ube,      
		EType_Int32,          // EType_Int32,          
		EType_Int32_u,        // EType_Int32_u,        
		EType_Int32,          // EType_Int32_be,       
		EType_Int32_u,        // EType_Int32_ube,      
		EType_Int64,          // EType_Int64,          
		EType_Int64_u,        // EType_Int64_u,        
		EType_Int64,          // EType_Int64_be,       
		EType_Int64_u,        // EType_Int64_ube,      
	};

	ASSERT (TypeKind <= EType_Int64_ube);
	return ResultTypeTable [TypeKind];
}

CType* 
CType::GetTypeMod (int Modifiers)
{
	CType* pType = this;

	if (Modifiers & ETypeModifier_Signed)
	{
		if (!CheckIntegerModifier (pType, Modifiers, ETypeModifier_Signed, ETypeModifier_Unsigned))
			return false;

		EType ModTypeKind = GetSignedTypeKind (pType->m_TypeKind);
		pType = m_pTypeMgr->GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_Unsigned)
	{
		if (!CheckIntegerModifier (pType, Modifiers, ETypeModifier_Unsigned, ETypeModifier_Signed))
			return false;

		EType ModTypeKind = GetUnsignedTypeKind (pType->m_TypeKind);
		pType = m_pTypeMgr->GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_BigEndian)
	{
		if (!CheckIntegerModifier (pType, Modifiers, ETypeModifier_BigEndian, ETypeModifier_LittleEndian))
			return false;

		EType ModTypeKind = GetBigEndianTypeKind (pType->m_TypeKind);
		pType = m_pTypeMgr->GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_LittleEndian)
	{
		if (!CheckIntegerModifier (pType, Modifiers, ETypeModifier_LittleEndian, ETypeModifier_BigEndian))
			return false;

		EType ModTypeKind = GetLittleEndianTypeKind (pType->m_TypeKind);
		pType = m_pTypeMgr->GetBasicType (ModTypeKind);
	}

	if (Modifiers & ETypeModifier_Pointer)
	{
		pType = m_pTypeMgr->GetPointerType (pType);
	}

	if (Modifiers & ETypeModifier_Reference)
	{
		pType = m_pTypeMgr->GetReferenceType (pType);
		if (!pType)
			return NULL;
	}

	if (Modifiers & ETypeModifier_Property)
	{
		CFunctionType* pGetterType = m_pTypeMgr->GetFunctionType (pType, NULL, 0, 0);
		CFunctionType* pSetterType = !(Modifiers & ETypeModifier_Const) ? 
			m_pTypeMgr->GetFunctionType (m_pTypeMgr->GetBasicType (EType_Void), &pType, 1, 0) : 
			NULL;
		pType = m_pTypeMgr->GetPropertyType (pGetterType, pSetterType);
	}
	else if (Modifiers & ETypeModifier_Const)
	{
		pType = m_pTypeMgr->GetConstType (pType);
	}

	return pType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
