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

	SetupAllPrimitiveTypes ();

	memset (m_StdTypeArray, 0, sizeof (m_StdTypeArray));
	m_UnnamedTypeIndex = 0;
}

void
CTypeMgr::Clear ()
{
	m_QualifierTypeList.Clear ();
	m_PointerTypeList.Clear ();
	m_ArrayTypeList.Clear ();
	m_BitFieldTypeList.Clear ();
	m_FunctionTypeList.Clear ();
	m_EnumTypeList.Clear ();
	m_StructTypeList.Clear ();
	m_ClassTypeList.Clear ();
	m_PropertyTypeList.Clear ();
	m_TypeMap.Clear ();

	SetupAllPrimitiveTypes ();

	memset (m_StdTypeArray, 0, sizeof (m_StdTypeArray));
	m_UnnamedTypeIndex = 0;
}

void
CTypeMgr::SetupAllPrimitiveTypes ()
{
	SetupPrimitiveType (EType_Void,      0, "a");
	SetupPrimitiveType (EType_Variant,   sizeof (TVariant), "b");
	SetupPrimitiveType (EType_Bool,      1, "c");
	SetupPrimitiveType (EType_Int8,      1, "d");
	SetupPrimitiveType (EType_Int8_u,    1, "e");
	SetupPrimitiveType (EType_Int16,     2, "f");
	SetupPrimitiveType (EType_Int16_u,   2, "g");
	SetupPrimitiveType (EType_Int32,     4, "h");
	SetupPrimitiveType (EType_Int32_u,   4, "i");
	SetupPrimitiveType (EType_Int64,     8, "j");
	SetupPrimitiveType (EType_Int64_u,   8, "k");
	SetupPrimitiveType (EType_Int16_be,  2, "l");
	SetupPrimitiveType (EType_Int16_beu, 2, "m");
	SetupPrimitiveType (EType_Int32_be,  4, "n");
	SetupPrimitiveType (EType_Int32_beu, 4, "o");
	SetupPrimitiveType (EType_Int64_be,  8, "p");
	SetupPrimitiveType (EType_Int64_beu, 8, "q");
	SetupPrimitiveType (EType_Float,     4, "r");
	SetupPrimitiveType (EType_Double,    8, "s");
}

void
CTypeMgr::SetupPrimitiveType (
	EType TypeKind,
	size_t Size,
	const char* pSignature
	)
{
	ASSERT (TypeKind < EType__PrimitiveTypeCount);
		
	CType* pType = &m_PrimitiveTypeArray [TypeKind];
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = TypeKind;
	pType->m_Flags = ETypeFlag_IsPod;
	pType->m_Size = Size;
	pType->m_Signature = pSignature;

	memset (pType->m_PointerTypeArray, 0, sizeof (pType->m_PointerTypeArray));

	m_TypeMap.Goto (pSignature)->m_Value = pType;
}

CType* 
CTypeMgr::GetStdType (EStdType StdType)
{
	ASSERT (StdType >= 0 && StdType < EStdType__Count);
	if (m_StdTypeArray [StdType])
		return m_StdTypeArray [StdType];

	CType* pType;

	switch (StdType)
	{
	case EStdType_BytePtr:
		pType = GetPointerType (EType_Pointer_u, EType_Int8);
		break;

	case EStdType_SafePtr:
		pType = CreateSafePtrType ();
		break;

	case EStdType_SafePtrValidator:
		pType = CreateSafePtrValidatorType ();
		break;

	case EStdType_InterfacePtr:
		pType = CreateInterfacePtrType ();
		break;

	case EStdType_FunctionPtr:
		pType = CreateFunctionPtrType ();
		break;

	case EStdType_ObjectHdr:
		pType = CreateObjectHdrType ();
		break;

	default:
		ASSERT (false);
		return NULL;
	}

	m_StdTypeArray [StdType] = pType;
	return pType;
}

CStructType* 
CTypeMgr::CreateSafePtrType ()
{
	CStructType* pType = GetStructType ("sptr", "jnc.sptr");
	pType->CreateMember ("m_p", GetStdType (EStdType_BytePtr));
	pType->CreateMember ("m_Validator", GetStdType (EStdType_SafePtrValidator));
	bool Result = pType->CalcLayout ();
	ASSERT (Result);
	return pType;
}

CStructType* 
CTypeMgr::CreateSafePtrValidatorType ()
{
	CStructType* pType = GetStructType ("sptrv", "jnc.sptrv");
	pType->CreateMember ("m_pRegionBegin", GetStdType (EStdType_BytePtr));
	pType->CreateMember ("m_pRegionEnd", GetStdType (EStdType_BytePtr));
	pType->CreateMember ("m_ScopeLevel", GetPrimitiveType (EType_SizeT));
	bool Result = pType->CalcLayout ();
	ASSERT (Result);
	return pType;
}

CStructType*
CTypeMgr::CreateInterfacePtrType ()
{
	CStructType* pType = GetStructType ("iface", "jnc.iface");
	pType->CreateMember ("m_p", GetStdType (EStdType_BytePtr));
	pType->CreateMember ("m_ScopeLevel", GetPrimitiveType (EType_SizeT));
	bool Result = pType->CalcLayout ();
	ASSERT (Result);
	return pType;
}

CStructType*
CTypeMgr::CreateFunctionPtrType ()
{
	CStructType* pType = GetStructType ("fptr", "jnc.fptr");
	pType->CreateMember ("m_pfn", GetStdType (EStdType_BytePtr));
	pType->CreateMember ("m_pInterface", GetStdType (EStdType_BytePtr));
	bool Result = pType->CalcLayout ();
	ASSERT (Result);
	return pType;
}

CStructType*
CTypeMgr::CreateObjectHdrType ()
{
	CStructType* pType = GetStructType ("obj", "jnc.obj");
	pType->CreateMember ("m_pType", GetStdType (EStdType_BytePtr));
	pType->CreateMember ("m_ScopeLevel", GetPrimitiveType (EType_SizeT));
	bool Result = pType->CalcLayout ();
	ASSERT (Result);
	return pType;
}

bool
CTypeMgr::ResolveImports ()
{
	bool Result;

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

	// re-layout all structs, unions, interfaces and classes

	rtl::CIteratorT <CStructType> StructType = m_StructTypeList.GetHead ();
	for (; StructType; StructType++)
	{
		Result = StructType->CalcLayout ();
		if (!Result)
			return false;
	}

	rtl::CIteratorT <CUnionType> UnionType = m_UnionTypeList.GetHead ();
	for (; UnionType; UnionType++)
	{
		Result = UnionType->CalcLayout ();
		if (!Result)
			return false;
	}

	rtl::CIteratorT <CClassType> ClassType = m_ClassTypeList.GetHead ();
	for (; ClassType; ClassType++)
	{
		Result = ClassType->CalcLayout ();
		if (!Result)
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

	m_QualifierTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CPointerType* 
CTypeMgr::GetPointerType (
	EType TypeKind,
	CType* pBaseType
	)
{
	size_t Index = TypeKind - EType_Pointer;
	ASSERT (Index < 4);

	if (pBaseType->m_PointerTypeArray [Index])
		return pBaseType->m_PointerTypeArray [Index];

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
	{
		ASSERT (false);
		return (CPointerType*) It->m_Value;
	}

	CPointerType* pType = AXL_MEM_NEW (CPointerType);
	pType->m_pModule = m_pModule;
	pType->m_TypeKind = TypeKind;
	pType->m_Size = Size;
	pType->m_Flags = Flags;
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;

	m_PointerTypeList.InsertTail (pType);
	It->m_Value = pType;
	pBaseType->m_PointerTypeArray [Index] = pType;
	
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
	pType->m_Size = sizeof (TInterface);
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
