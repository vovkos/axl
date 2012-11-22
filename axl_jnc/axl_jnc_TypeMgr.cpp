#include "stdafx.h"
#include "axl_jnc_TypeMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CTypeMgr::CTypeMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_pBytePtrType = NULL;
	m_pSafePtrStructType = NULL;
	m_pFunctionPtrStructType = NULL;
	m_UnnamedTypeIndex = 0;

	SetupAllBasicTypes ();
}

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

	m_pBytePtrType = NULL;
	m_pSafePtrStructType = NULL;
	m_pFunctionPtrStructType = NULL;
	m_UnnamedTypeIndex = 0;
}

void
CTypeMgr::SetupAllBasicTypes ()
{
	SetupBasicType (EType_Void,      0, "a");
	SetupBasicType (EType_Variant,   sizeof (TVariant), "b");
	SetupBasicType (EType_Bool,      1, "c");
	SetupBasicType (EType_Int8,      1, "d");
	SetupBasicType (EType_Int8_u,    1, "e");
	SetupBasicType (EType_Int16,     2, "f");
	SetupBasicType (EType_Int16_u,   2, "g");
	SetupBasicType (EType_Int32,     4, "h");
	SetupBasicType (EType_Int32_u,   4, "i");
	SetupBasicType (EType_Int64,     8, "j");
	SetupBasicType (EType_Int64_u,   8, "k");
	SetupBasicType (EType_Int16_be,  2, "l");
	SetupBasicType (EType_Int16_beu, 2, "m");
	SetupBasicType (EType_Int32_be,  4, "n");
	SetupBasicType (EType_Int32_beu, 4, "o");
	SetupBasicType (EType_Int64_be,  8, "p");
	SetupBasicType (EType_Int64_beu, 8, "q");
	SetupBasicType (EType_Float,     4, "r");
	SetupBasicType (EType_Double,    8, "s");
}

void
CTypeMgr::SetupBasicType (
	EType TypeKind,
	size_t Size,
	const char* pSignature
	)
{
	ASSERT (TypeKind < EType__BasicTypeCount);
		
	CType* pType = &m_BasicTypeArray [TypeKind];
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = TypeKind;
	pType->m_Flags = ETypeFlag_IsPod;
	pType->m_Size = Size;
	pType->m_Signature = pSignature;

	m_TypeMap.Goto (pSignature)->m_Value = pType;
}

bool
CTypeMgr::ResolveImports ()
{
	rtl::CIteratorT <CImportType> ImportType = m_ImportTypeList.GetHead ();
	for (; ImportType; ImportType++)
	{
		CImportType* pImportType = *ImportType;
		CModuleItem* pItem = pImportType->m_pAnchorNamespace->FindItemTraverse (pImportType->m_Name);
		if (!pItem)
		{
			err::SetFormatStringError (_T("unresolved import '%s'"), pImportType->m_Name.GetFullName ());
			return false;
		}

		pItem = UnAliasItem (pItem);
		
		EModuleItem ItemKind = pItem->GetItemKind ();
		if (ItemKind != EModuleItem_Type)
		{
			err::SetFormatStringError (_T("'%s' is not a type"), pImportType->m_Name.GetFullName ());
			return false;
		}

		pImportType->m_pExternType = (CType*) pItem;
	}

	return true;
}

bool
VerifyReferenceType (CType* pBaseType)
{
	if (pBaseType->IsReferenceType ())
	{
		err::SetFormatStringError (_T("reference to reference is illegal"));
		return false;
	}
	
	if (pBaseType->GetTypeKind () == EType_Property)
	{
		err::SetFormatStringError (_T("reference to property is illegal"));
		return false;
	}

	return true;
}

CType* 
CTypeMgr::GetQualifiedType (
	CType* pBaseType,
	int Flags
	)
{
	Flags &= ETypeQualifier__Mask;
	if (!Flags)
		return pBaseType;

	if (pBaseType->GetTypeKind () == EType_Qualifier)
	{
		CQualifierType* pQualifierType = (CQualifierType*) pBaseType;

		int BaseFlags = pQualifierType->GetFlags ();
		BaseFlags &= ETypeQualifier__Mask;

		if ((BaseFlags & Flags) == Flags)
			return pBaseType;

		Flags |= BaseFlags;
		pBaseType = pQualifierType->GetBaseType ();
	}

	rtl::CStringA Signature;
	Signature.Format ("A%d%s", Flags, pBaseType->GetSignature ());

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CQualifierType*) It->m_Value;

	CQualifierType* pType = AXL_MEM_NEW (CQualifierType);
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = EType_Qualifier;
	pType->m_Flags = Flags;
	pType->m_Size = pBaseType->GetSize ();
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;

	m_DerivedTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CPointerType* 
CTypeMgr::GetPointerType (
	EType TypeKind,
	CType* pBaseType
	)
{
	rtl::CStringA Signature;
	size_t Size;
	int Flags = 0;

	switch (TypeKind)
	{
	case EType_Pointer:
		Signature = 'B';
		Size = sizeof (TSafePtr);
		break;

	case EType_Pointer_u:
		Signature = 'C';
		Size = sizeof (void*);
		Flags = ETypeFlag_IsPod;
		break;

	case EType_Reference:
		Signature = 'E';
		Size = sizeof (TSafePtr);
		break;

	case EType_Reference_u:
		Signature = 'F';
		Size = sizeof (void*);
		Flags = ETypeFlag_IsPod;
		break;

	default:
		ASSERT (false);
		return NULL;
	}

	Signature += pBaseType->GetSignature ();

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CPointerType*) It->m_Value;

	CPointerType* pType = AXL_MEM_NEW (CPointerType);
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = TypeKind;
	pType->m_Size = Size;
	pType->m_Flags = Flags;
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;

	m_DerivedTypeList.InsertTail (pType);
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
	EType TypeKind = pBaseType->GetTypeKind ();
	if (TypeKind < EType_Int8 || TypeKind > EType_Int64_u)
	{
		err::SetFormatStringError (_T("bit field can only be used with little-endian integer types"));
		return NULL;
	}

	rtl::CStringA Signature = CBitFieldType::CreateSignature (pBaseType, BitOffset, BitCount);

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CBitFieldType*) It->m_Value;

	CBitFieldType* pType = AXL_MEM_NEW (CBitFieldType);
	pType->m_pModule = m_pModule;
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

CEnumType* 
CTypeMgr::GetEnumType (
	EType TypeKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	ASSERT (TypeKind == EType_Enum || TypeKind == EType_Enum_c);

	if (Name.IsEmpty ())
	{
		CEnumType* pType = AXL_MEM_NEW (CEnumType);
		pType->m_pModule = m_pModule;
		pType->m_TypeKind = TypeKind;
		pType->m_pModule = m_pModule;
		m_EnumTypeList.InsertTail (pType);
		return pType;
	}

	rtl::CStringA Signature;
	Signature.Format ("%c%s", TypeKind == EType_Enum ? 'I' : 'J', QualifiedName);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CEnumType*) It->m_Value;

	CEnumType* pType = AXL_MEM_NEW (CEnumType);
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = TypeKind;
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_pModule = m_pModule;

	m_EnumTypeList.InsertTail (pType);
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

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CArrayType*) It->m_Value;

	CArrayType* pType = AXL_MEM_NEW (CArrayType);
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = EType_Array;
	pType->m_Flags = pBaseType->GetFlags () & ETypeFlag_IsPod;
	pType->m_Size = pBaseType->GetSize () * ElementCount;
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;
	pType->m_ElementCount = ElementCount;

	m_ArrayTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CStructType* 
CTypeMgr::GetStructType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	size_t PackFactor
	)
{
	if (Name.IsEmpty ())
	{
		CStructType* pType = AXL_MEM_NEW (CStructType);
		pType->m_pModule = m_pModule;
		pType->m_Signature.Format ("L%d", m_UnnamedTypeIndex++);
		pType->m_PackFactor = PackFactor;
		m_StructTypeList.InsertTail (pType);
		return pType;
	}

	rtl::CStringA Signature;
	Signature.Format ("L%s", QualifiedName);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CStructType*) It->m_Value;

	CStructType* pType = AXL_MEM_NEW (CStructType);	
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_PackFactor = PackFactor;

	m_StructTypeList.InsertTail (pType);
	It->m_Value = pType;

	return pType;
}

CUnionType* 
CTypeMgr::GetUnionType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	if (Name.IsEmpty ())
	{
		CUnionType* pType = AXL_MEM_NEW (CUnionType);
		pType->m_pModule = m_pModule;
		pType->m_Signature.Format ("M%d", m_UnnamedTypeIndex++);
		m_UnionTypeList.InsertTail (pType);
		return pType;
	}

	rtl::CStringA Signature;
	Signature.Format ("M%s", QualifiedName);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CUnionType*) It->m_Value;

	CUnionType* pType = AXL_MEM_NEW (CUnionType);	
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	
	m_UnionTypeList.InsertTail (pType);
	It->m_Value = pType;

	return pType;
}
CStructType*
CTypeMgr::GetSafePtrStructType ()
{
	if (m_pSafePtrStructType)
		return m_pSafePtrStructType;

	m_pSafePtrStructType = GetStructType ("sptr", "jnc.sptr");
	m_pSafePtrStructType->CreateMember ("m_p", GetBytePtrType ());
	m_pSafePtrStructType->CreateMember ("m_beg", GetBytePtrType ());
	m_pSafePtrStructType->CreateMember ("m_end", GetBytePtrType ());
	m_pSafePtrStructType->CreateMember ("m_scope", GetBasicType (EType_SizeT));

	return m_pSafePtrStructType;
}

CStructType*
CTypeMgr::GetFunctionPtrStructType ()
{
	if (m_pFunctionPtrStructType)
		return m_pFunctionPtrStructType;

	m_pFunctionPtrStructType = GetStructType ("fptr", "jnc.fptr");
	m_pFunctionPtrStructType->CreateMember ("m_pfn", GetBytePtrType ());
	m_pFunctionPtrStructType->CreateMember ("m_iface", GetBytePtrType ());

	return m_pFunctionPtrStructType;
}

CPointerType* 
CTypeMgr::GetBytePtrType ()
{
	if (m_pBytePtrType)
		return m_pBytePtrType;

	m_pBytePtrType = GetPointerType (EType_Pointer_u, EType_Int8);
	return m_pBytePtrType;
}

CClassType* 
CTypeMgr::GetClassType (
	EType TypeKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	size_t PackFactor
	)
{
	ASSERT (TypeKind == EType_Class || TypeKind == EType_Interface);

	char SignatureChar = TypeKind == EType_Interface ? 'N' : 'O';

	if (Name.IsEmpty ())
	{
		CClassType* pType = AXL_MEM_NEW (CClassType);
		pType->m_pModule = m_pModule;
		pType->m_TypeKind = TypeKind;
		pType->m_Signature.Format ("%c%d", SignatureChar, m_UnnamedTypeIndex++);
		pType->m_PackFactor = PackFactor;
		m_ClassTypeList.InsertTail (pType);
		return pType;
	}

	rtl::CStringA Signature;
	Signature.Format ("%c%s", SignatureChar, QualifiedName);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CClassType*) It->m_Value;

	CClassType* pType = AXL_MEM_NEW (CClassType);
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = TypeKind;
	pType->m_Signature = Signature;
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_PackFactor = PackFactor;

	m_ClassTypeList.InsertTail (pType);
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
	rtl::CStringA Signature = CFunctionType::CreateSignature (EType_Function, pReturnType, ppArgType, ArgCount, Flags);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CFunctionType*) It->m_Value;

	CFunctionType* pType = AXL_MEM_NEW (CFunctionType);
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = EType_Function;
	pType->m_Size = sizeof (TFunctionPtr);
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
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CPropertyType*) It->m_Value;

	CPropertyType* pType = AXL_MEM_NEW (CPropertyType);
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = EType_Property;
	pType->m_Size = sizeof (TFunctionPtr) + SetterType.GetOverloadCount () * sizeof (void*);
	pType->m_Signature = Signature;
	pType->m_pGetterType = pGetterType;
	pType->m_SetterType = SetterType;

	m_PropertyTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CImportType*
CTypeMgr::GetImportType (
	const CQualifiedName& Name,
	CNamespace* pAnchorNamespace
	)
{
	rtl::CStringA Signature;
	Signature.Format ("Z%s.%s", pAnchorNamespace->GetQualifiedName (), Name.GetFullName ());
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CImportType*) It->m_Value;

	CImportType* pType = AXL_MEM_NEW (CImportType);
	pType->m_pModule = m_pModule;
	pType->m_Name = Name;
	pType->m_pAnchorNamespace = pAnchorNamespace;
	pType->m_pModule = m_pModule;

	m_ImportTypeList.InsertTail (pType);
	It->m_Value = pType;

	return pType;
}

//.............................................................................

/*

bool
CStructType::AddGenericArgument (CImportType* pType)
{
	if (m_GenericArgumentArray.Find (pType) != -1)
	{
		err::SetFormatStringError (_T("multiple generic argument names '%s'"), pType->GetTypeString ());
		return false;
	}

	m_GenericArgumentArray.Append (pType);
	return true;
}

*/

} // namespace axl {
} // namespace jnc {
