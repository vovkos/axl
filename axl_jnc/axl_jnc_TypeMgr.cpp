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
	m_UnnamedTypeCounter = 0;
}

void
CTypeMgr::Clear ()
{
	m_ArrayTypeList.Clear ();
	m_BitFieldTypeList.Clear ();
	m_EnumTypeList.Clear ();
	m_StructTypeList.Clear ();
	m_UnionTypeList.Clear ();
	m_ClassTypeList.Clear ();
	m_FunctionTypeList.Clear ();
	m_PropertyTypeList.Clear ();
	m_EventTypeList.Clear ();
	m_DataPtrTypeList.Clear ();
	m_ClassPtrTypeList.Clear ();
	m_FunctionPtrTypeList.Clear ();
	m_PropertyPtrTypeList.Clear ();
	m_ImportTypeList.Clear ();
	m_DataPtrTypeTupleList.Clear ();
	m_ClassPtrTypeTupleList.Clear ();
	m_FunctionPtrTypeTupleList.Clear ();
	m_PropertyPtrTypeTupleList.Clear ();

	m_TypeMap.Clear ();

	SetupAllPrimitiveTypes ();

	memset (m_StdTypeArray, 0, sizeof (m_StdTypeArray));
	m_UnnamedTypeCounter = 0;
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
		pType = GetPrimitiveType (EType_Int8_u)->GetDataPtrType (EDataPtrType_Unsafe);
		break;

	case EStdType_DataPtrValidator:
		pType = CreateDataPtrValidatorType ();
		break;

	case EStdType_ObjectHdr:
		pType = CreateObjectHdrType ();
		break;

	case EStdType_ObjectClass:
		pType = CreateObjectType ();
		break;

	case EStdType_ObjectPtr:
		pType = ((CClassType*) GetStdType (EStdType_ObjectClass))->GetClassPtrType ();
		break;

	case EStdType_SimpleFunction:
		pType = GetFunctionType (GetPrimitiveType (EType_Void), NULL, 0, 0);
		break;

	case EStdType_SimpleEvent:
		pType = GetEventType ((CFunctionType*) GetStdType (EStdType_SimpleFunction));
		break;

	case EStdType_StrenthenClosureFunction:
		pType = GetFunctionType (GetStdType (EStdType_ObjectPtr), GetStdType (EStdType_ObjectPtr), 0);
		break;

	default:
		ASSERT (false);
		return NULL;
	}

	m_StdTypeArray [StdType] = pType;
	return pType;
}

bool
CTypeMgr::ResolveImportTypes ()
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
CTypeMgr::CalcTypeLayouts ()
{
	bool Result;

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
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;
	pType->m_BitOffset = BitOffset;
	pType->m_BitCount = BitCount;
	pType->m_Size = pBaseType->GetSize ();

	m_BitFieldTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CArrayType* 
CTypeMgr::GetArrayType (
	CType* pElementType,
	size_t ElementCount
	)
{
	rtl::CStringA Signature = CArrayType::CreateSignature (pElementType, ElementCount);

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CArrayType*) It->m_Value;

	CArrayType* pType = AXL_MEM_NEW (CArrayType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pElementType = pElementType;
	pType->m_ElementCount = ElementCount;
	pType->m_Size = pElementType->GetSize () * ElementCount;
	pType->m_Flags = pElementType->GetFlags () & ETypeFlag_Pod;

	m_ArrayTypeList.InsertTail (pType);
	It->m_Value = pType;
	
	return pType;
}

CEnumType* 
CTypeMgr::CreateEnumType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	int Flags
	)
{
	const char* pSignaturePrefix = (Flags & EEnumTypeFlag_Exposed) ? "EC" : "EE";

	if (Name.IsEmpty ())
	{
		CEnumType* pType = AXL_MEM_NEW (CEnumType);
		pType->m_pModule = m_pModule;
		pType->m_Signature.Format ("%s%d:%s", pSignaturePrefix, m_UnnamedTypeCounter++, m_pModule->GetFilePath ());
		pType->m_Tag.Format (_T("_unnamed_enum_%d"), m_UnnamedTypeCounter);
		pType->m_Flags |= Flags;
		m_EnumTypeList.InsertTail (pType);
		return pType;
	}


	CEnumType* pType = AXL_MEM_NEW (CEnumType);
	pType->m_pModule = m_pModule;
	pType->m_Signature.Format ("%s%s", pSignaturePrefix, QualifiedName);
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_Tag = QualifiedName;
	pType->m_Flags |= Flags | ETypeFlag_Named;

	m_EnumTypeList.InsertTail (pType);
	return pType;
}

CStructType* 
CTypeMgr::CreateStructType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	size_t PackFactor
	)
{
	if (Name.IsEmpty ())
	{
		CStructType* pType = AXL_MEM_NEW (CStructType);
		pType->m_pModule = m_pModule;
		pType->m_Signature.Format ("S%d:%s", m_UnnamedTypeCounter++, m_pModule->GetFilePath ());
		pType->m_Tag.Format (_T("_unnamed_struct_%d"), m_UnnamedTypeCounter);
		pType->m_PackFactor = PackFactor;
		m_StructTypeList.InsertTail (pType);
		return pType;
	}

	CStructType* pType = AXL_MEM_NEW (CStructType);	
	pType->m_pModule = m_pModule;
	pType->m_Signature.Format ("S%s", QualifiedName);
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_Tag = QualifiedName;
	pType->m_Flags |= ETypeFlag_Named;
	pType->m_PackFactor = PackFactor;

	m_StructTypeList.InsertTail (pType);
	return pType;
}

CUnionType* 
CTypeMgr::CreateUnionType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	if (Name.IsEmpty ())
	{
		CUnionType* pType = AXL_MEM_NEW (CUnionType);
		pType->m_pModule = m_pModule;
		pType->m_Signature.Format ("U%d:%s", m_UnnamedTypeCounter++, m_pModule->GetFilePath ());
		pType->m_Tag.Format (_T("_unnamed_union_%d"), m_UnnamedTypeCounter);
		m_UnionTypeList.InsertTail (pType);
		return pType;
	}

	CUnionType* pType = AXL_MEM_NEW (CUnionType);	
	pType->m_pModule = m_pModule;
	pType->m_Signature.Format ("U%s", QualifiedName);
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_Tag = QualifiedName;
	pType->m_Flags |= ETypeFlag_Named;
	
	m_UnionTypeList.InsertTail (pType);
	return pType;
}

CClassType* 
CTypeMgr::CreateClassType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	size_t PackFactor,
	int Flags
	)
{
	char SignatureChar = (EClassTypeFlag_Interface) ? 'I' : 'C';

	if (Name.IsEmpty ())
	{
		CClassType* pType = AXL_MEM_NEW (CClassType);
		pType->m_pModule = m_pModule;
		pType->m_Signature.Format ("%c%d%s", SignatureChar, m_UnnamedTypeCounter++, m_pModule->GetFilePath ());
		pType->m_Tag.Format (_T("_unnamed_class_%d"), m_UnnamedTypeCounter);
		pType->m_Flags |= Flags;
		pType->m_PackFactor = PackFactor;
		m_ClassTypeList.InsertTail (pType);
		return pType;
	}

	CClassType* pType = AXL_MEM_NEW (CClassType);
	pType->m_pModule = m_pModule;
	pType->m_Signature.Format ("%c%s", SignatureChar, QualifiedName);
	pType->m_Name = Name;
	pType->m_QualifiedName = QualifiedName;
	pType->m_Tag = QualifiedName;
	pType->m_PackFactor = PackFactor;
	pType->m_Flags |= Flags | ETypeFlag_Named;
	
	m_ClassTypeList.InsertTail (pType);
	return pType;
}

CFunctionType* 
CTypeMgr::GetFunctionType (	
	ECallConv CallConv,
	CType* pReturnType,
	const rtl::CArrayT <CType*>& ArgTypeArray,
	int Flags
	)
{
	if (!pReturnType)
		pReturnType = GetPrimitiveType (EType_Void);

	rtl::CStringA Signature = CFunctionType::CreateSignature (
		CallConv, 
		pReturnType, 
		ArgTypeArray, 
		ArgTypeArray.GetCount (), 
		Flags
		);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CFunctionType*) It->m_Value;

	CFunctionType* pType = AXL_MEM_NEW (CFunctionType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pReturnType = pReturnType;
	pType->m_ArgTypeArray = ArgTypeArray;
	pType->m_CallConv = CallConv;
	pType->m_Flags = Flags;

	m_FunctionTypeList.InsertTail (pType);
	It->m_Value = pType;	
	return pType;
}

CFunctionType* 
CTypeMgr::GetMethodMemberType (
	CClassType* pClassType,
	CFunctionType* pFunctionType
	)
{
	rtl::CArrayT <CType*> ArgTypeArray = pFunctionType->GetArgTypeArray ();
	ArgTypeArray.Insert (0, pClassType);	
	
	return GetFunctionType (
		pFunctionType->m_CallConv,
		pFunctionType->m_pReturnType,
		ArgTypeArray,
		pFunctionType->m_Flags
		);
}

CFunctionType* 
CTypeMgr::GetAbstractMethodMemberType (CFunctionType* pFunctionType)
{
	if (pFunctionType->m_pAbstractMethodMemberType)
		return pFunctionType->m_pAbstractMethodMemberType;
	
	CClassType* pClassType = (CClassType*) GetStdType (EStdType_ObjectClass);
	pFunctionType->m_pAbstractMethodMemberType = pClassType->GetMethodMemberType (pFunctionType);
	return pFunctionType->m_pAbstractMethodMemberType;
}

CPropertyType* 
CTypeMgr::GetPropertyType (
	CFunctionType* pGetterType,
	const CFunctionTypeOverload& SetterType,
	int Flags
	)
{
	rtl::CStringA Signature = CPropertyType::CreateSignature (pGetterType, SetterType, Flags);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CPropertyType*) It->m_Value;

	CPropertyType* pType = AXL_MEM_NEW (CPropertyType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pGetterType = pGetterType;
	pType->m_SetterType = SetterType;

	if (SetterType.IsEmpty ())
		Flags |= EPropertyTypeFlag_ReadOnly;

	if (Flags & EPropertyTypeFlag_Bindable)
		pType->m_pBindablePropertyType = pType;

	pType->m_Flags = Flags;

	m_PropertyTypeList.InsertTail (pType);
	It->m_Value = pType;	
	return pType;
}

CPropertyType* 
CTypeMgr::GetSimplePropertyType (
	ECallConv CallConv,
	CType* pReturnType,
	int Flags
	)
{
	if (Flags & EPropertyTypeFlag_Bindable)
	{
		Flags &= ~EPropertyTypeFlag_Bindable;
		CPropertyType* pPropertType = GetSimplePropertyType (CallConv, pReturnType, Flags);
		return GetBindablePropertyType (pPropertType);
	}

	CDataPtrTypeTuple* pTuple = GetDataPtrTypeTuple (pReturnType);

	size_t i1 = CallConv == ECallConv_Stdcall;
	size_t i2 = (Flags & EPropertyTypeFlag_ReadOnly) != 0;

	if (pTuple->m_SimplePropertyTypeArray [i1] [i2])
		return pTuple->m_SimplePropertyTypeArray [i1] [i2];

	CPropertyType* pPropertyType;

	CFunctionType* pGetterType = GetFunctionType (CallConv, pReturnType, NULL, 0, 0);
	if (Flags & EPropertyTypeFlag_ReadOnly)
	{
		pPropertyType = GetPropertyType (pGetterType, NULL, Flags);
	}
	else
	{
		CFunctionType* pSetterType = GetFunctionType (CallConv, NULL, &pReturnType, 1, 0);
		pPropertyType = GetPropertyType (pGetterType, pSetterType, Flags);
	}

	pTuple->m_SimplePropertyTypeArray [i1] [i2] = pPropertyType;
	return pPropertyType;
}

CPropertyType* 
CTypeMgr::GetIndexedPropertyType (
	ECallConv CallConv,
	CType* pReturnType,
	const rtl::CArrayT <CType*>& IndexArgTypeArray,
	int Flags
	)
{
	CFunctionType* pGetterType = GetFunctionType (CallConv, pReturnType, IndexArgTypeArray, 0);

	if (Flags & EPropertyTypeFlag_ReadOnly)
	{
		return GetPropertyType (pGetterType, NULL, Flags);
	}
	else
	{
		rtl::CArrayT <CType*> ArgTypeArray = IndexArgTypeArray;
		ArgTypeArray.Append (pReturnType);

		CFunctionType* pSetterType = GetFunctionType (CallConv, NULL, ArgTypeArray, 0);
		return GetPropertyType (pGetterType, pSetterType, Flags);
	}
}

CPropertyType* 
CTypeMgr::GetPropertyMemberType (
	CClassType* pClassType,
	CPropertyType* pPropertyType
	)
{
	CFunctionType* pGetterType = GetMethodMemberType (pClassType, pPropertyType->m_pGetterType);

	size_t SetterTypeOverloadCount = pPropertyType->m_SetterType.GetOverloadCount ();

	char Buffer [256];
	rtl::CArrayT <CFunctionType*> SetterTypeOverloadArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	SetterTypeOverloadArray.SetCount (SetterTypeOverloadCount);

	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pOverloadType = pPropertyType->m_SetterType.GetOverload (i);
		SetterTypeOverloadArray [i] = GetMethodMemberType (pClassType, pOverloadType);
	}

	return GetPropertyType (
		pGetterType, 
		CFunctionTypeOverload (SetterTypeOverloadArray, SetterTypeOverloadCount), 
		pPropertyType->m_Flags
		);
}

CPropertyType* 
CTypeMgr::GetAbstractPropertyMemberType (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pAbstractPropertyMemberType)
		return pPropertyType->m_pAbstractPropertyMemberType;

	CClassType* pClassType = (CClassType*) GetStdType (EStdType_ObjectClass);
	pPropertyType->m_pAbstractPropertyMemberType = pClassType->GetPropertyMemberType (pPropertyType);
	return pPropertyType->m_pAbstractPropertyMemberType;
}

CPropertyType* 
CTypeMgr::GetBindablePropertyType (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pBindablePropertyType)
		return pPropertyType->m_pBindablePropertyType;

	ASSERT (!(pPropertyType->m_Flags & EPropertyTypeFlag_Bindable));

	pPropertyType->m_pBindablePropertyType = GetPropertyType (
		pPropertyType->m_pGetterType, 
		pPropertyType->m_SetterType, 
		pPropertyType->m_Flags | EPropertyTypeFlag_Bindable
		);
	
	return pPropertyType->m_pBindablePropertyType;
}

CEventType* 
CTypeMgr::GetEventType (CFunctionPtrType* pFunctionPtrType)
{
	if (pFunctionPtrType->m_pEventType)
		return pFunctionPtrType->m_pEventType;

	rtl::CStringA Signature = 'V';
	Signature += pFunctionPtrType->GetSignature ();

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
	{
		ASSERT (false);
		return (CEventType*) It->m_Value;
	}

	CEventType* pType = AXL_MEM_NEW (CEventType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pFunctionPtrType = pFunctionPtrType;

	m_EventTypeList.InsertTail (pType);
	It->m_Value = pType;
	pFunctionPtrType->m_pEventType = pType;
	return pType;
}

CDataPtrType* 
CTypeMgr::GetDataPtrType (
	CType* pDataType,
	EType TypeKind,
	EDataPtrType PtrTypeKind,
	int Flags
	)
{
	ASSERT (TypeKind == EType_DataPtr || TypeKind == EType_DataRef);
	ASSERT (PtrTypeKind >= 0 && PtrTypeKind < EDataPtrType__Count);

	ASSERT (TypeKind != EType_DataRef || pDataType->m_TypeKind != EType_DataRef); // dbl reference
	
	CDataPtrTypeTuple* pTuple = GetDataPtrTypeTuple (pDataType);

	// ref x ptrkind x volatile x const x nonull

	size_t i1 = TypeKind == EType_DataRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Volatile) != 0;
	size_t i4 = (Flags & EPtrTypeFlag_Const) != 0;
	size_t i5 = (Flags & EPtrTypeFlag_NoNull) != 0;
		
	if (pTuple->m_DataPtrArray [i1] [i2] [i3] [i4] [i5])
		return pTuple->m_DataPtrArray [i1] [i2] [i3] [i4] [i5];

	size_t Size = 0;

	switch (PtrTypeKind)
	{
	case EDataPtrType_Normal:
		Size = sizeof (TDataPtr);
		break;

	case EDataPtrType_Thin:
		Size = sizeof (void*);
		break;

	case EDataPtrType_Unsafe:
		Size = sizeof (void*);
		Flags |= ETypeFlag_Pod;
		break;

	default:
		ASSERT (false);
	}

	CDataPtrType* pType = AXL_MEM_NEW (CDataPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CDataPtrType::CreateSignature (pDataType, TypeKind, PtrTypeKind, Flags);
	pType->m_TypeKind = TypeKind;
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_Size = Size;
	pType->m_pDataType = pDataType;
	pType->m_Flags = Flags;

	m_DataPtrTypeList.InsertTail (pType);
	pTuple->m_DataPtrArray [i1] [i2] [i3] [i4] [i5] = pType;	
	return pType;
}

CStructType*
CTypeMgr::GetDataPtrStructType (CType* pDataType)
{
	CDataPtrTypeTuple* pTuple = GetDataPtrTypeTuple (pDataType);
	if (pTuple->m_pDataPtrStructType)
		return pTuple->m_pDataPtrStructType;

	CStructType* pType = CreateDataPtrStructType (pDataType);
	pTuple->m_pDataPtrStructType = pType;
	return pType;
}

CClassPtrType* 
CTypeMgr::GetClassPtrType (
	CClassType* pClassType,
	EClassPtrType PtrTypeKind,
	int Flags
	)
{
	ASSERT (PtrTypeKind >= 0 && PtrTypeKind < EClassPtrType__Count);

	CClassPtrTypeTuple* pTuple = GetClassPtrTypeTuple (pClassType);

	// ptrkind x const x nonull

	size_t i1 = PtrTypeKind;
	size_t i2 = (Flags & EPtrTypeFlag_Const) != 0;
	size_t i3 = (Flags & EPtrTypeFlag_NoNull) != 0;
		
	if (pTuple->m_ClassPtrArray [i1] [i2] [i3])
		return pTuple->m_ClassPtrArray [i1] [i2] [i3];

	if (PtrTypeKind == EClassPtrType_Unsafe)
		Flags |= ETypeFlag_Pod;

	CClassPtrType* pType = AXL_MEM_NEW (CClassPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CClassPtrType::CreateSignature (pClassType, PtrTypeKind, Flags);
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_pClassType = pClassType;
	pType->m_Flags = Flags;

	m_ClassPtrTypeList.InsertTail (pType);
	pTuple->m_ClassPtrArray [i1] [i2] [i3] = pType;	
	return pType;
}

CFunctionPtrType*
CTypeMgr::GetFunctionPtrType (
	CFunctionType* pFunctionType,
	EType TypeKind,
	EFunctionPtrType PtrTypeKind,
	int Flags
	)
{
	ASSERT (TypeKind == EType_FunctionPtr || TypeKind == EType_FunctionRef);
	ASSERT (PtrTypeKind >= 0 && PtrTypeKind < EFunctionPtrType__Count);

	CFunctionPtrTypeTuple* pTuple = GetFunctionPtrTypeTuple (pFunctionType);

	// ref x kind x nonull

	size_t i1 = TypeKind == EType_FunctionRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_NoNull) != 0;

	if (pTuple->m_FunctionPtrArray [i1] [i2] [i3])
		return pTuple->m_FunctionPtrArray [i1] [i2] [i3];

	size_t Size = 0;

	switch (PtrTypeKind)
	{
	case EFunctionPtrType_Normal:
		Size = sizeof (TFunctionPtr);
		break;

	case EFunctionPtrType_Thin:
		Size = sizeof (void*);
		break;

	case EFunctionPtrType_Weak:
		Size = sizeof (TFunctionWeakPtr);
		break;

	case EFunctionPtrType_Unsafe:
		Size = sizeof (void*);
		Flags |= ETypeFlag_Pod;
		break;

	default:
		ASSERT (false);
	}

	CFunctionPtrType* pType = AXL_MEM_NEW (CFunctionPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CFunctionPtrType::CreateSignature (pFunctionType, TypeKind, PtrTypeKind, Flags);
	pType->m_TypeKind = TypeKind;
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_Size = Size;
	pType->m_pFunctionType = pFunctionType;
	pType->m_Flags = Flags;

	m_FunctionPtrTypeList.InsertTail (pType);
	pTuple->m_FunctionPtrArray [i1] [i2] [i3] = pType;
	return pType;
}

CStructType* 
CTypeMgr::GetFunctionPtrStructType (CFunctionType* pFunctionType)
{
	CFunctionPtrTypeTuple* pTuple = GetFunctionPtrTypeTuple (pFunctionType);
	if (pTuple->m_pFunctionPtrStructType)
		return pTuple->m_pFunctionPtrStructType;

	pTuple->m_pFunctionPtrStructType = CreateFunctionPtrStructType (pFunctionType);
	return pTuple->m_pFunctionPtrStructType;
}

CStructType* 
CTypeMgr::GetFunctionWeakPtrStructType (CFunctionType* pFunctionType)
{
	CFunctionPtrTypeTuple* pTuple = GetFunctionPtrTypeTuple (pFunctionType);
	if (pTuple->m_pFunctionWeakPtrStructType)
		return pTuple->m_pFunctionWeakPtrStructType;

	pTuple->m_pFunctionWeakPtrStructType = CreateFunctionWeakPtrStructType (pFunctionType);
	return pTuple->m_pFunctionWeakPtrStructType;
}

CPropertyPtrType* 
CTypeMgr::GetPropertyPtrType (
	CPropertyType* pPropertyType,
	EType TypeKind,
	EPropertyPtrType PtrTypeKind,
	int Flags
	)
{
	ASSERT (TypeKind == EType_PropertyPtr || TypeKind == EType_PropertyRef);
	ASSERT (PtrTypeKind >= 0 && PtrTypeKind < EPropertyPtrType__Count);

	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);

	// ref x kind x nonull

	size_t i1 = TypeKind == EType_FunctionRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_NoNull) != 0;

	if (pTuple->m_PropertyPtrArray [i1] [i2] [i3])
		return pTuple->m_PropertyPtrArray [i1] [i2] [i3];

	size_t Size = 0;

	if (!(pPropertyType->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		switch (PtrTypeKind)
		{
		case EPropertyPtrType_Normal:
			Size = sizeof (TPropertyPtr);
			break;

		case EPropertyPtrType_Thin:
			Size = sizeof (void**);
			break;
		
		case EPropertyPtrType_Weak:
			Size = sizeof (TPropertyWeakPtr);
			break;

		case EPropertyPtrType_Unsafe:
			Size = sizeof (void**);
			Flags |= ETypeFlag_Pod;
			break;

		default:
			ASSERT (false);
		}
	}
	else
	{
		switch (PtrTypeKind)
		{
		case EPropertyPtrType_Normal:
			Size = sizeof (TAuPropertyPtr);
			break;

		case EPropertyPtrType_Thin:
			Size = sizeof (TAuPropertyThinPtr);
			break;

		case EPropertyPtrType_Weak:
			Size = sizeof (TAuPropertyWeakPtr);
			break;

		case EPropertyPtrType_Unsafe:
			Size = sizeof (TAuPropertyUnsafePtr);
			Flags |= ETypeFlag_Pod;
			break;

		default:
			ASSERT (false);
		}
	}

	CPropertyPtrType* pType = AXL_MEM_NEW (CPropertyPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CPropertyPtrType::CreateSignature (pPropertyType, TypeKind, PtrTypeKind, Flags);
	pType->m_TypeKind = TypeKind;
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_Size = Size;
	pType->m_pPropertyType = pPropertyType;
	pType->m_Flags = Flags;

	m_PropertyPtrTypeList.InsertTail (pType);
	pTuple->m_PropertyPtrArray [i1] [i2] [i3] = pType;
	return pType;
}

CStructType* 
CTypeMgr::GetPropertyVTableStructType (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pVTableStructType)
		return pPropertyType->m_pVTableStructType;

	pPropertyType->m_pVTableStructType = CreatePropertyVTableStructType (pPropertyType);
	return pPropertyType->m_pVTableStructType;
}

CStructType* 
CTypeMgr::GetPropertyPtrStructType (CPropertyType* pPropertyType)
{
	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);
	if (pTuple->m_pPropertyPtrStructType)
		return pTuple->m_pPropertyPtrStructType;

	pTuple->m_pPropertyPtrStructType = CreatePropertyPtrStructType (pPropertyType);
	return pTuple->m_pPropertyPtrStructType;
}

CStructType* 
CTypeMgr::GetPropertyWeakPtrStructType (CPropertyType* pPropertyType)
{
	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);
	if (pTuple->m_pPropertyWeakPtrStructType)
		return pTuple->m_pPropertyWeakPtrStructType;

	pTuple->m_pPropertyWeakPtrStructType = CreatePropertyWeakPtrStructType (pPropertyType);
	return pTuple->m_pPropertyWeakPtrStructType;
}

CStructType* 
CTypeMgr::GetBindablePropertyThinPtrStructType (CPropertyType* pPropertyType)
{
	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);
	if (pTuple->m_pBindablePropertyThinPtrStructType)
		return pTuple->m_pBindablePropertyThinPtrStructType;

	pTuple->m_pBindablePropertyThinPtrStructType = CreateBindablePropertyThinPtrStructType (pPropertyType);
	return pTuple->m_pBindablePropertyThinPtrStructType;
}

CStructType* 
CTypeMgr::GetBindablePropertyUnsafePtrStructType (CPropertyType* pPropertyType)
{
	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);
	if (pTuple->m_pBindablePropertyUnsafePtrStructType)
		return pTuple->m_pBindablePropertyUnsafePtrStructType;

	pTuple->m_pBindablePropertyUnsafePtrStructType = CreateBindablePropertyUnsafePtrStructType (pPropertyType);
	return pTuple->m_pBindablePropertyUnsafePtrStructType;
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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CDataPtrTypeTuple*
CTypeMgr::GetDataPtrTypeTuple (CType* pType)
{
	if (pType->m_pDataPtrTypeTuple)
		return pType->m_pDataPtrTypeTuple;

	CDataPtrTypeTuple* pTuple = AXL_MEM_NEW (CDataPtrTypeTuple);
	pType->m_pDataPtrTypeTuple = pTuple;
	m_DataPtrTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

CClassPtrTypeTuple*
CTypeMgr::GetClassPtrTypeTuple (CClassType* pClassType)
{
	if (pClassType->m_pClassPtrTypeTuple)
		return pClassType->m_pClassPtrTypeTuple;

	CClassPtrTypeTuple* pTuple = AXL_MEM_NEW (CClassPtrTypeTuple);
	pClassType->m_pClassPtrTypeTuple = pTuple;
	m_ClassPtrTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

CFunctionPtrTypeTuple*
CTypeMgr::GetFunctionPtrTypeTuple (CFunctionType* pFunctionType)
{
	if (pFunctionType->m_pFunctionPtrTypeTuple)
		return pFunctionType->m_pFunctionPtrTypeTuple;

	CFunctionPtrTypeTuple* pTuple = AXL_MEM_NEW (CFunctionPtrTypeTuple);
	pFunctionType->m_pFunctionPtrTypeTuple = pTuple;
	m_FunctionPtrTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

CPropertyPtrTypeTuple*
CTypeMgr::GetPropertyPtrTypeTuple (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pPropertyPtrTypeTuple)
		return pPropertyType->m_pPropertyPtrTypeTuple;

	CPropertyPtrTypeTuple* pTuple = AXL_MEM_NEW (CPropertyPtrTypeTuple);
	pPropertyType->m_pPropertyPtrTypeTuple = pTuple;
	m_PropertyPtrTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CTypeMgr::SetupAllPrimitiveTypes ()
{
	SetupPrimitiveType (EType_Void,      0, "v");
	SetupPrimitiveType (EType_Variant,   sizeof (TVariant), "z");
	SetupPrimitiveType (EType_Bool,      1, "b");
	SetupPrimitiveType (EType_Int8,      1, "is1");
	SetupPrimitiveType (EType_Int8_u,    1, "iu1");
	SetupPrimitiveType (EType_Int16,     2, "is2");
	SetupPrimitiveType (EType_Int16_u,   2, "iu2");
	SetupPrimitiveType (EType_Int32,     4, "is4");
	SetupPrimitiveType (EType_Int32_u,   4, "iu4");
	SetupPrimitiveType (EType_Int64,     8, "is8");
	SetupPrimitiveType (EType_Int64_u,   8, "iu8");
	SetupPrimitiveType (EType_Int16_be,  2, "ibs2");
	SetupPrimitiveType (EType_Int16_beu, 2, "ibu2");
	SetupPrimitiveType (EType_Int32_be,  4, "ibs4");
	SetupPrimitiveType (EType_Int32_beu, 4, "ibu4");
	SetupPrimitiveType (EType_Int64_be,  8, "ibs8");
	SetupPrimitiveType (EType_Int64_beu, 8, "ibu8");
	SetupPrimitiveType (EType_Float,     4, "f4");
	SetupPrimitiveType (EType_Double,    8, "f8");
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
	pType->m_Flags = ETypeFlag_Pod;
	pType->m_Size = Size;
	pType->m_Signature = pSignature;
	pType->m_pDataPtrTypeTuple = NULL;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CStructType* 
CTypeMgr::CreateDataPtrValidatorType ()
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag = _T("sptrv");
	pType->CreateMember (GetStdType (EStdType_BytePtr));
	pType->CreateMember (GetStdType (EStdType_BytePtr));
	pType->CreateMember (GetPrimitiveType (EType_SizeT));
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateObjectHdrType ()
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag = _T("objhdr");
	pType->CreateMember (GetStdType (EStdType_BytePtr));  // CClassType* m_pType;
	pType->CreateMember (GetPrimitiveType (EType_SizeT)); // size_t m_ScopeLevel;
	pType->CalcLayout ();
	return pType;
}

CClassType*
CTypeMgr::CreateObjectType ()
{
	CClassType* pType = CreateUnnamedClassType ();
	pType->m_Tag = _T("object");
	pType->m_Flags |= EClassTypeFlag_StdObject;
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateDataPtrStructType (CType* pBaseType)
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("%s.sptr"), pBaseType->GetTypeString ());
	pType->CreateMember (pBaseType->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateMember (GetStdType (EStdType_DataPtrValidator));
	pType->CalcLayout ();
	return pType;
}

CStructType* 
CTypeMgr::CreateFunctionPtrStructType (CFunctionType* pFunctionType)
{
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("fn.ptr"));
	pType->CreateMember (pFunctionType->GetFunctionPtrType (EFunctionPtrType_Unsafe));
	pType->CreateMember (GetStdType (EStdType_ObjectPtr));
	pType->CalcLayout ();
	return pType;
}

CStructType* 
CTypeMgr::CreateFunctionWeakPtrStructType (CFunctionType* pFunctionType)
{
	CFunctionType* pStrenthenClosureType = (CFunctionType*) GetStdType (EStdType_StrenthenClosureFunction);
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("fn.wptr"));
	pType->CreateMember (pFunctionType->GetFunctionPtrType (EFunctionPtrType_Unsafe));
	pType->CreateMember (GetStdType (EStdType_ObjectPtr));
	pType->CreateMember (pStrenthenClosureType->GetFunctionPtrType (EFunctionPtrType_Unsafe));
	pType->CalcLayout ();
	return NULL;
}

CStructType*
CTypeMgr::CreatePropertyVTableStructType (CPropertyType* pPropertyType)
{
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.vtbl"));
	pType->CreateMember (pPropertyType->m_pGetterType->GetFunctionPtrType (EFunctionPtrType_Unsafe));

	size_t SetterTypeOverloadCount = pPropertyType->m_SetterType.GetOverloadCount ();
	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pSetterType = pPropertyType->m_SetterType.GetOverload (i);
		pType->CreateMember (pSetterType->GetFunctionPtrType (EFunctionPtrType_Unsafe));
	}

	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreatePropertyPtrStructType (CPropertyType* pPropertyType)
{
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.ptr"));
	pType->CreateMember (pPropertyType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateMember (GetStdType (EStdType_ObjectPtr));

	if (pPropertyType->m_Flags & EPropertyTypeFlag_Bindable)
		pType->CreateMember (GetStdType (EStdType_SimpleEvent)->GetDataPtrType ());

	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreatePropertyWeakPtrStructType (CPropertyType* pPropertyType)
{
	CFunctionType* pStrenthenClosureType = (CFunctionType*) GetStdType (EStdType_StrenthenClosureFunction);
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.wptr"));
	pType->CreateMember (pPropertyType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateMember (GetStdType (EStdType_ObjectPtr));

	if (pPropertyType->m_Flags & EPropertyTypeFlag_Bindable)
		pType->CreateMember (GetStdType (EStdType_SimpleEvent)->GetDataPtrType ());

	pType->CreateMember (pStrenthenClosureType->GetFunctionPtrType (EFunctionPtrType_Unsafe));
	pType->CalcLayout ();
	return NULL;
}

CStructType*
CTypeMgr::CreateBindablePropertyThinPtrStructType (CPropertyType* pPropertyType)
{
	ASSERT (pPropertyType->m_Flags & EPropertyTypeFlag_Bindable);
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.tptr"));
	pType->CreateMember (pPropertyType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateMember (GetStdType (EStdType_SimpleEvent)->GetDataPtrType ());
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateBindablePropertyUnsafePtrStructType (CPropertyType* pPropertyType)
{
	ASSERT (pPropertyType->m_Flags & EPropertyTypeFlag_Bindable);
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.uptr"));
	pType->CreateMember (pPropertyType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateMember (GetStdType (EStdType_SimpleEvent)->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CalcLayout ();
	return pType;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
