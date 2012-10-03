#include "stdafx.h"
#include "axl_jnc_TypeMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CTypeMgr::Clear ()
{
	m_DerivedTypeList.Clear ();
	m_ArrayTypeList.Clear ();
	m_BitFieldTypeList.Clear ();
	m_FunctionTypeList.Clear ();
	m_EnumTypeList.Clear ();
	m_StructTypeList.Clear ();
	m_ClassTypeList.Clear ();
	m_PropertyTypeList.Clear ();
	m_TypeMap.Clear ();
}

void
CTypeMgr::SetupAllBasicTypes ()
{
	SetupBasicType (EType_Void,      0, "v");
	SetupBasicType (EType_Bool,      1, "a");
	SetupBasicType (EType_Int8,      1, "b");
	SetupBasicType (EType_Int8_u,    1, "c");
	SetupBasicType (EType_Int16,     2, "d");
	SetupBasicType (EType_Int16_u,   2, "e");
	SetupBasicType (EType_Int16_be,  2, "f");
	SetupBasicType (EType_Int16_ube, 2, "g");
	SetupBasicType (EType_Int32,     4, "h");
	SetupBasicType (EType_Int32_u,   4, "i");
	SetupBasicType (EType_Int32_be,  4, "j");
	SetupBasicType (EType_Int32_ube, 4, "k");
	SetupBasicType (EType_Int64,     8, "l");
	SetupBasicType (EType_Int64_u,   8, "m");
	SetupBasicType (EType_Int64_be,  8, "n");
	SetupBasicType (EType_Int64_ube, 8, "o");
	SetupBasicType (EType_Float,     4, "p");
	SetupBasicType (EType_Double,    8, "q");
	SetupBasicType (EType_Variant,   sizeof (TFatPointer), "z");
}

void
CTypeMgr::SetupBasicType (
	EType Type,
	size_t Size,
	const char* pSignature
	)
{
	ASSERT (Type >= EType_Void && Type <= EType_Variant);
		
	CType* pType = &m_BasicTypeArray [Type];
	pType->m_pTypeMgr = this;
	pType->m_TypeKind = Type;
	pType->m_Size = Size;
	pType->m_Signature = pSignature;

	m_TypeMap.Goto (pSignature)->m_Value = pType;
}

CType* 
CTypeMgr::GetBasicType (EType Type)
{
	ASSERT (Type >= EType_Void && Type <= EType_Variant);
	return &m_BasicTypeArray [Type];
}

CDerivedType* 
CTypeMgr::GetConstType (CType* pBaseType)
{
	if (pBaseType->m_TypeKind == EType_Const)
		return (CDerivedType*) pBaseType;

	rtl::CStringA Signature = "C";
	Signature += pBaseType->GetSignature ();

	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CDerivedType*) It->m_Value;

	CDerivedType* pType = AXL_MEM_NEW (CDerivedType);
	pType->m_pTypeMgr = this;
	pType->m_TypeKind = EType_Const;
	pType->m_Size = pBaseType->GetSize ();
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;

	m_DerivedTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CDerivedType* 
CTypeMgr::GetPointerType (CType* pBaseType)
{
	rtl::CStringA Signature = "P";
	Signature += pBaseType->GetSignature ();

	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CDerivedType*) It->m_Value;

	CDerivedType* pType = AXL_MEM_NEW (CDerivedType);
	pType->m_pTypeMgr = this;
	pType->m_TypeKind = EType_Pointer;
	pType->m_Size = sizeof (TFatPointer);
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;

	m_DerivedTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CDerivedType* 
CTypeMgr::GetReferenceType (CType* pBaseType)
{
	if (pBaseType->IsReferenceType ())
	{
		err::SetFormatStringError (_T("reference to reference is illegal"));
		return NULL;
	}
	
	if (pBaseType->IsPropertyType ())
	{
		err::SetFormatStringError (_T("reference to property is illegal"));
		return NULL;
	}

	rtl::CStringA Signature = "R";
	Signature += pBaseType->GetSignature ();

	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CDerivedType*) It->m_Value;

	CDerivedType* pType = AXL_MEM_NEW (CDerivedType);
	pType->m_pTypeMgr = this;
	pType->m_TypeKind = EType_Reference;
	pType->m_Size = sizeof (TFatPointer);
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;

	m_DerivedTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CArrayType* 
CTypeMgr::GetArrayType (
	CType* pBaseType,
	size_t ElementCount
	)
{
	rtl::CStringA Signature = CArrayType::CreateSignature (pBaseType, ElementCount);

	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CArrayType*) It->m_Value;

	CArrayType* pType = AXL_MEM_NEW (CArrayType);
	pType->m_pTypeMgr = this;
	pType->m_TypeKind = EType_Array;
	pType->m_Size = pBaseType->GetSize () * ElementCount;
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;
	pType->m_ElementCount = ElementCount;

	m_ArrayTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CBitFieldType* 
CTypeMgr::GetBitFieldType (
	CType* pBaseType,
	size_t BitOffset,
	size_t BitCount
	)
{
	rtl::CStringA Signature = CBitFieldType::CreateSignature (pBaseType, BitOffset, BitCount);

	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CBitFieldType*) It->m_Value;

	CBitFieldType* pType = AXL_MEM_NEW (CBitFieldType);
	pType->m_pTypeMgr = this;
	pType->m_TypeKind = EType_BitField;
	pType->m_Size = pBaseType->GetSize ();
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;
	pType->m_BitOffset = BitOffset;
	pType->m_BitCount = BitCount;

	m_BitFieldTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CFunctionType* 
CTypeMgr::GetFunctionType (
	CType* pReturnType,
	CType** ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA Signature = CFunctionType::CreateSignature (pReturnType, ppArgType, ArgCount, Flags);
	
	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CFunctionType*) It->m_Value;

	CFunctionType* pType = AXL_MEM_NEW (CFunctionType);
	pType->m_pTypeMgr = this;
	pType->m_TypeKind = EType_Function;
	pType->m_Size = sizeof (TFatPointer);
	pType->m_Signature = Signature;
	pType->m_pReturnType = pReturnType;
	pType->m_ArgTypeArray.Copy (ppArgType, ArgCount);
	pType->m_Flags = Flags;

	m_FunctionTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CPropertyType* 
CTypeMgr::GetPropertyType (
	CFunctionType* pGetterType,
	const CFunctionTypeOverload& SetterType
	)
{
	rtl::CStringA Signature = CPropertyType::CreateSignature (pGetterType, SetterType);
	
	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CPropertyType*) It->m_Value;

	CPropertyType* pType = AXL_MEM_NEW (CPropertyType);
	pType->m_pTypeMgr = this;
	pType->m_TypeKind = EType_Property;
	pType->m_Size = sizeof (TFatPointer);
	pType->m_Signature = Signature;
	pType->m_pGetterType = pGetterType;
	pType->m_SetterType = SetterType;

	m_PropertyTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CEnumType* 
CTypeMgr::GetEnumType (
	EType TypeKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	ASSERT (TypeKind == EType_Enum || TypeKind == EType_EnumC);

	if (Name.IsEmpty ())
	{
		CEnumType* pType = AXL_MEM_NEW (CEnumType);
		pType->m_TypeKind = TypeKind;
		pType->m_pTypeMgr = this;
		m_EnumTypeList.InsertTail (pType);
		return pType;
	}

	rtl::CStringA Signature;
	Signature.Format (_T("%c%s"), TypeKind == EType_Enum ? 'E' : 'N', QualifiedName);
	
	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CEnumType*) It->m_Value;

	CEnumType* pType = AXL_MEM_NEW (CEnumType);
	pType->m_TypeKind = TypeKind;
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_pTypeMgr = this;
	m_EnumTypeList.InsertTail (pType);
	return pType;
}

CStructType* 
CTypeMgr::GetStructType (
	EType TypeKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	ASSERT (TypeKind == EType_Struct || TypeKind == EType_Union);

	if (Name.IsEmpty ())
	{
		CStructType* pType = AXL_MEM_NEW (CStructType);
		pType->m_TypeKind = TypeKind;
		pType->m_pTypeMgr = this;
		m_StructTypeList.InsertTail (pType);
		return pType;
	}

	rtl::CStringA Signature;
	Signature.Format (_T("%c%s"), TypeKind == EType_Struct ? 'S' : 'U', QualifiedName);
	
	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CStructType*) It->m_Value;

	CStructType* pType = AXL_MEM_NEW (CStructType);
	pType->m_TypeKind = TypeKind;
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_pTypeMgr = this;
	m_StructTypeList.InsertTail (pType);
	return pType;
}

CClassType* 
CTypeMgr::GetClassType (
	EType TypeKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	ASSERT (TypeKind == EType_Class || TypeKind == EType_Interface);

	if (Name.IsEmpty ())
	{
		CClassType* pType = AXL_MEM_NEW (CClassType);
		pType->m_TypeKind = TypeKind;
		pType->m_pTypeMgr = this;
		m_ClassTypeList.InsertTail (pType);
		return pType;
	}

	rtl::CStringA Signature;
	Signature.Format (_T("%c%s"), TypeKind == EType_Class ? 'C' : 'I', QualifiedName);
	
	rtl::CHashTableMapIteratorT <const char*, CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CClassType*) It->m_Value;

	CClassType* pType = AXL_MEM_NEW (CClassType);
	pType->m_TypeKind = TypeKind;
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_pTypeMgr = this;
	m_ClassTypeList.InsertTail (pType);
	return pType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
