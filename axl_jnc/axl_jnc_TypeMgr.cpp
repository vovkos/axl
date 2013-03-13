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

	m_UnnamedEnumTypeCounter = 0;
	m_UnnamedStructTypeCounter = 0;
	m_UnnamedUnionTypeCounter = 0;
	m_UnnamedClassTypeCounter = 0;
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
	m_MulticastTypeList.Clear ();
	m_McSnapshotTypeList.Clear ();
	m_AutoEvTypeList.Clear ();
	m_DataPtrTypeList.Clear ();
	m_ClassPtrTypeList.Clear ();
	m_FunctionPtrTypeList.Clear ();
	m_PropertyPtrTypeList.Clear ();
	m_ImportTypeList.Clear ();

	m_PropertyTypeTupleList.Clear ();
	m_DataPtrTypeTupleList.Clear ();
	m_ClassPtrTypeTupleList.Clear ();
	m_FunctionPtrTypeTupleList.Clear ();
	m_PropertyPtrTypeTupleList.Clear ();

	m_TypeMap.Clear ();

	SetupAllPrimitiveTypes ();

	memset (m_StdTypeArray, 0, sizeof (m_StdTypeArray));

	m_UnnamedEnumTypeCounter = 0;
	m_UnnamedStructTypeCounter = 0;
	m_UnnamedUnionTypeCounter = 0;
	m_UnnamedClassTypeCounter = 0;
}

CType* 
CTypeMgr::GetStdType (EStdType StdType)
{
	ASSERT ((size_t) StdType < EStdType__Count);
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

	case EStdType_SimpleMulticast:
		pType = GetMulticastType ((CFunctionType*) GetStdType (EStdType_SimpleFunction));
		break;

	case EStdType_SimpleEventPtr:
		pType = GetStdType (EStdType_SimpleMulticast)->GetDataPtrType (EDataPtrType_Normal, EPtrTypeFlag_Event);
		break;

	case EStdType_StrenthenClosureFunction:
		pType = GetFunctionType (GetStdType (EStdType_ObjectPtr), GetStdType (EStdType_ObjectPtr), 0);
		break;

	case EStdType_AutoEvBindSite:
		pType = CreateAutoEvBindSiteType ();
		break;

	case EStdType_IScheduler:
		pType = CreateISchedulerType ();
		break;

	case EStdType_ISchedulerPtr:
		pType = ((CClassType*) GetStdType (EStdType_IScheduler))->GetClassPtrType ();
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

	rtl::CIteratorT <CClassType> ClassType = m_ClassTypeList.GetHead ();
	for (; ClassType; ClassType++)
	{
		Result = ClassType->CalcLayout ();
		if (!Result)
			return false;
	}

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

	CType* pRootType = pElementType->m_TypeKind == EType_Array ? 
		((CArrayType*) pElementType)->m_pRootType : 
		pElementType;

	CArrayType* pType = AXL_MEM_NEW (CArrayType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pElementType = pElementType;
	pType->m_pRootType = pRootType;
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

	CEnumType* pType = AXL_MEM_NEW (CEnumType);

	if (Name.IsEmpty ())
	{
		m_UnnamedEnumTypeCounter++;
		pType->m_Signature.Format ("%s%d:%s", pSignaturePrefix, m_UnnamedEnumTypeCounter, m_pModule->GetFilePath ());
		pType->m_Tag.Format (_T("_unnamed_enum_%d"), m_UnnamedEnumTypeCounter);
	}
	else
	{
		pType->m_Signature.Format ("%s%s", pSignaturePrefix, QualifiedName);
		pType->m_Name = Name;
		pType->m_QualifiedName = QualifiedName;
		pType->m_Tag = QualifiedName;
		pType->m_Flags |= ETypeFlag_Named;
	}

	pType->m_pModule = m_pModule;
	pType->m_pBaseType = GetPrimitiveType (EType_Int);
	pType->m_Size = pType->m_pBaseType->GetSize ();
	pType->m_Flags |= Flags;
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
	CStructType* pType = AXL_MEM_NEW (CStructType);	

	if (Name.IsEmpty ())
	{
		m_UnnamedStructTypeCounter++;
		pType->m_Signature.Format ("S%d:%s", m_UnnamedStructTypeCounter, m_pModule->GetFilePath ());
		pType->m_Tag.Format (_T("_unnamed_struct_%d"), m_UnnamedStructTypeCounter);
	}
	else
	{
		pType->m_Signature.Format ("S%s", QualifiedName);
		pType->m_Name = Name;
		pType->m_QualifiedName = QualifiedName;
		pType->m_Tag = QualifiedName;
		pType->m_Flags |= ETypeFlag_Named;
	}

	pType->m_pModule = m_pModule;
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
	CUnionType* pType = AXL_MEM_NEW (CUnionType);

	if (Name.IsEmpty ())
	{
		m_UnnamedUnionTypeCounter++;
		pType->m_Signature.Format ("U%d:%s", m_UnnamedUnionTypeCounter, m_pModule->GetFilePath ());
		pType->m_Tag.Format (_T("_unnamed_union_%d"), m_UnnamedUnionTypeCounter);
	}
	else
	{
		pType->m_Signature.Format ("U%s", QualifiedName);
		pType->m_Name = Name;
		pType->m_QualifiedName = QualifiedName;
		pType->m_Tag = QualifiedName;
		pType->m_Flags |= ETypeFlag_Named;
	}

	pType->m_pModule = m_pModule;
	m_UnionTypeList.InsertTail (pType);
	return pType;
}

CClassType* 
CTypeMgr::CreateClassType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	size_t PackFactor
	)
{
	CClassType* pType = AXL_MEM_NEW (CClassType);

	if (Name.IsEmpty ())
	{
		m_UnnamedClassTypeCounter++;
		pType->m_Signature.Format ("C%d%s", m_UnnamedClassTypeCounter, m_pModule->GetFilePath ());
		pType->m_Tag.Format (_T("_unnamed_class_%d"), m_UnnamedClassTypeCounter);
	}
	else
	{
		pType->m_Signature.Format ("C%s", QualifiedName);
		pType->m_Name = Name;
		pType->m_QualifiedName = QualifiedName;
		pType->m_Tag = QualifiedName;
		pType->m_Flags |= ETypeFlag_Named;
	}

	CStructType* pVTableStructType = CreateUnnamedStructType ();
	pVTableStructType->m_Tag.Format (_T("%s.vtbl"), pType->m_Tag);

	CStructType* pIfaceHdrStructType = CreateUnnamedStructType (PackFactor);
	pIfaceHdrStructType->m_Tag.Format (_T("%s.ifacehdr"), pType->m_Tag);
	pIfaceHdrStructType->CreateFieldMember (pVTableStructType->GetDataPtrType (EDataPtrType_Unsafe));
	pIfaceHdrStructType->CreateFieldMember (GetStdType (EStdType_ObjectHdr)->GetDataPtrType (EDataPtrType_Unsafe));

	CStructType* pIfaceStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (PackFactor);
	pIfaceStructType->m_Tag.Format (_T("%s.iface"), pType->m_Tag);
	pIfaceStructType->m_pParentNamespace = pType;
	pIfaceStructType->m_StorageKind = EStorage_Member;
	pIfaceStructType->AddBaseType (pIfaceHdrStructType);

	CStructType* pClassStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (PackFactor);
	pClassStructType->m_Tag.Format (_T("%s.class"), pType->m_Tag);
	pClassStructType->m_pParentNamespace = pType;
	pClassStructType->CreateFieldMember (GetStdType (EStdType_ObjectHdr));
	pClassStructType->CreateFieldMember (pIfaceStructType);

	pType->m_pModule = m_pModule;
	pType->m_PackFactor = PackFactor;
	pType->m_pVTableStructType = pVTableStructType;
	pType->m_pIfaceStructType = pIfaceStructType;
	pType->m_pClassStructType = pClassStructType;
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
	CFunctionType* pFunctionType,
	int ThisArgTypeFlags
	)
{
	CClassPtrType* pThisArgType = pClassType->GetClassPtrType (EClassPtrType_Normal, ThisArgTypeFlags);

	rtl::CArrayT <CType*> ArgTypeArray = pFunctionType->GetArgTypeArray ();
	ArgTypeArray.Insert (0, pThisArgType);	
	
	CFunctionType* pMethodMemberType = GetFunctionType (
		pFunctionType->m_CallConv,
		pFunctionType->m_pReturnType,
		ArgTypeArray,
		pFunctionType->m_Flags
		);

	pMethodMemberType->m_pShortType = pFunctionType;
	return  pMethodMemberType;
}

CFunctionType* 
CTypeMgr::GetStdObjectMethodMemberType (CFunctionType* pFunctionType)
{
	if (pFunctionType->m_pStdObjectMethodMemberType)
		return pFunctionType->m_pStdObjectMethodMemberType;
	
	CClassType* pClassType = (CClassType*) GetStdType (EStdType_ObjectClass);
	pFunctionType->m_pStdObjectMethodMemberType = pClassType->GetMethodMemberType (pFunctionType);
	return pFunctionType->m_pStdObjectMethodMemberType;
}

CFunctionType* 
CTypeMgr::GetShortFunctionType (CFunctionType* pFunctionType)
{
	if (pFunctionType->m_pShortType)
		return pFunctionType->m_pShortType;
	
	if (!pFunctionType->IsMethodMemberType ())
	{
		pFunctionType->m_pShortType = pFunctionType;
		return pFunctionType;
	}

	rtl::CArrayT <CType*> ArgTypeArray = pFunctionType->m_ArgTypeArray;
	ArgTypeArray.Remove (0);

	pFunctionType->m_pShortType = GetFunctionType (
		pFunctionType->m_CallConv,
		pFunctionType->m_pReturnType,
		ArgTypeArray,
		pFunctionType->m_Flags
		);

	return pFunctionType->m_pShortType;
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

	if (SetterType.IsEmpty ())
		Flags |= EPropertyTypeFlag_Const;

	CPropertyType* pType = AXL_MEM_NEW (CPropertyType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pGetterType = pGetterType;
	pType->m_SetterType = SetterType;
	pType->m_Flags = Flags;

	if (Flags & EPropertyTypeFlag_Augmented)
	{
		pType->m_pAuFieldStructType = CreateUnnamedStructType ();

		if (Flags & EPropertyTypeFlag_Bindable) // event first
		{
			pType->m_AuFieldArray [EAuPropertyField_OnChange] = pType->m_pAuFieldStructType->CreateFieldMember (_T("onchange"), GetStdType (EStdType_SimpleMulticast));
			pType->m_pBindablePropertyType = pType;
		}

		if (Flags & EPropertyTypeFlag_AutoGet)
		{
			pType->m_AuFieldArray [EAuPropertyField_PropValue] = pType->m_pAuFieldStructType->CreateFieldMember (_T("propvalue"), pGetterType->GetReturnType ());
		}
	}

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

	CPropertyTypeTuple* pTuple = GetPropertyTypeTuple (pReturnType);

	size_t i1 = CallConv == ECallConv_Stdcall;
	size_t i2 = (Flags & EPropertyTypeFlag_Const) != 0;
	size_t i3 = (Flags & EPropertyTypeFlag_AutoSet) ? 2 : (Flags & EPropertyTypeFlag_AutoGet) ? 1 : 0;

	if (pTuple->m_SimplePropertyTypeArray [i1] [i2] [i3])
		return pTuple->m_SimplePropertyTypeArray [i1] [i2] [i3];

	CPropertyType* pPropertyType;

	CFunctionType* pGetterType = GetFunctionType (CallConv, pReturnType, NULL, 0, 0);
	if (Flags & EPropertyTypeFlag_Const)
	{
		pPropertyType = GetPropertyType (pGetterType, NULL, Flags);
	}
	else
	{
		CFunctionType* pSetterType = GetFunctionType (CallConv, NULL, &pReturnType, 1, 0);
		pPropertyType = GetPropertyType (pGetterType, pSetterType, Flags);
	}

	pTuple->m_SimplePropertyTypeArray [i1] [i2] [i3] = pPropertyType;
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

	if (Flags & EPropertyTypeFlag_Const)
		return GetPropertyType (pGetterType, NULL, Flags);

	rtl::CArrayT <CType*> ArgTypeArray = IndexArgTypeArray;
	ArgTypeArray.Append (pReturnType);

	CFunctionType* pSetterType = GetFunctionType (CallConv, NULL, ArgTypeArray, 0);
	return GetPropertyType (pGetterType, pSetterType, Flags);
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

	CPropertyType* pPropertyMemberType = GetPropertyType (
		pGetterType, 
		CFunctionTypeOverload (SetterTypeOverloadArray, SetterTypeOverloadCount), 
		pPropertyType->m_Flags
		);

	pPropertyMemberType->m_pShortType = pPropertyType;
	return pPropertyMemberType;
}

CPropertyType* 
CTypeMgr::GetStdObjectPropertyMemberType (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pStdObjectPropertyMemberType)
		return pPropertyType->m_pStdObjectPropertyMemberType;

	CClassType* pClassType = (CClassType*) GetStdType (EStdType_ObjectClass);
	pPropertyType->m_pStdObjectPropertyMemberType = pClassType->GetPropertyMemberType (pPropertyType);
	return pPropertyType->m_pStdObjectPropertyMemberType;
}

CPropertyType* 
CTypeMgr::GetShortPropertyType (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pShortType)
		return pPropertyType->m_pShortType;
	
	if (!pPropertyType->IsPropertyMemberType ())
	{
		pPropertyType->m_pShortType = pPropertyType;
		return pPropertyType;
	}

	CFunctionType* pGetterType = pPropertyType->m_pGetterType->GetShortType ();
	CFunctionTypeOverload SetterType;

	size_t SetterCount = pPropertyType->m_SetterType.GetOverloadCount ();
	for (size_t i = 0; i < SetterCount; i++)
	{
		CFunctionType* pSetterType = pPropertyType->m_SetterType.GetOverload (i)->GetShortType ();
		SetterType.AddOverload (pSetterType);
	}

	pPropertyType->m_pShortType = GetPropertyType (pGetterType, SetterType, pPropertyType->m_Flags);
	return pPropertyType->m_pShortType;
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

CAutoEvType* 
CTypeMgr::GetAutoEvMemberType (
	CClassType* pClassType,
	CAutoEvType* pAutoEvType
	)
{
	CAutoEvType* pAutoEvMemberType = GetAutoEvType (
		GetMethodMemberType (pClassType, pAutoEvType->m_pStarterType), 
		GetMethodMemberType (pClassType, pAutoEvType->m_pStopperType)
		);

	pAutoEvMemberType->m_pShortType = pAutoEvType;
	return pAutoEvMemberType;
}

CAutoEvType* 
CTypeMgr::GetStdObjectAutoEvMemberType (CAutoEvType* pAutoEvType)
{
	if (pAutoEvType->m_pStdObjectAutoEvMemberType)
		return pAutoEvType->m_pStdObjectAutoEvMemberType;

	CClassType* pClassType = (CClassType*) GetStdType (EStdType_ObjectClass);
	pAutoEvType->m_pStdObjectAutoEvMemberType = pClassType->GetAutoEvMemberType (pAutoEvType);
	return pAutoEvType->m_pStdObjectAutoEvMemberType;
}

CAutoEvType* 
CTypeMgr::GetShortAutoEvType (CAutoEvType* pAutoEvType)
{
	if (pAutoEvType->m_pShortType)
		return pAutoEvType->m_pShortType;
	
	if (!pAutoEvType->IsAutoEvMemberType ())
	{
		pAutoEvType->m_pShortType = pAutoEvType;
		return pAutoEvType;
	}

	pAutoEvType->m_pShortType = GetAutoEvType (
		pAutoEvType->m_pStarterType->GetShortType (), 
		pAutoEvType->m_pStarterType->GetShortType ()
		);

	return pAutoEvType->m_pShortType;
}

CMulticastType* 
CTypeMgr::GetMulticastType (CFunctionPtrType* pFunctionPtrType)
{
	if (pFunctionPtrType->m_pMulticastType)
		return pFunctionPtrType->m_pMulticastType;

	CMulticastType* pType = AXL_MEM_NEW (CMulticastType);
	pType->m_pModule = m_pModule;
	pType->m_Signature.Format ("M%s", pFunctionPtrType->GetSignature ());
	pType->m_pTargetType = pFunctionPtrType;

	m_MulticastTypeList.InsertTail (pType);
	pFunctionPtrType->m_pMulticastType = pType;
	return pType;
}

CMcSnapshotType* 
CTypeMgr::GetMcSnapshotType (CFunctionPtrType* pFunctionPtrType)
{
	if (pFunctionPtrType->m_pMcSnapshotType)
		return pFunctionPtrType->m_pMcSnapshotType;

	CMcSnapshotType* pType = AXL_MEM_NEW (CMcSnapshotType);
	pType->m_pModule = m_pModule;
	pType->m_Signature.Format ("Ms%s", pFunctionPtrType->GetSignature ());
	pType->m_pTargetType = pFunctionPtrType;

	m_McSnapshotTypeList.InsertTail (pType);
	pFunctionPtrType->m_pMcSnapshotType = pType;
	return pType;
}

CAutoEvType* 
CTypeMgr::GetAutoEvType (
	CFunctionType* pStarterType,
	CFunctionType* pStopperType
	)
{
	if (!pStopperType)
		pStopperType = (CFunctionType*) GetStdType (EStdType_SimpleFunction);

	rtl::CStringA Signature = CAutoEvType::CreateSignature (pStarterType, pStopperType);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CAutoEvType*) It->m_Value;

	CAutoEvType* pType = AXL_MEM_NEW (CAutoEvType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pStarterType = pStarterType;
	pType->m_pStopperType = pStopperType;

	m_AutoEvTypeList.InsertTail (pType);
	It->m_Value = pType;	
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
	ASSERT ((size_t) PtrTypeKind < EDataPtrType__Count);

	ASSERT (TypeKind != EType_DataRef || pDataType->m_TypeKind != EType_DataRef); // dbl reference
	
	CDataPtrTypeTuple* pTuple = GetDataPtrTypeTuple (pDataType);

	// ref x ptrkind x volatile x const x nullable

	size_t i1 = TypeKind == EType_DataRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Const) != 0;
	size_t i4 = (Flags & EPtrTypeFlag_Volatile) != 0;
	size_t i5 = (Flags & EPtrTypeFlag_Nullable) != 0;
		
	if (pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5];

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
	pType->m_pTargetType = pDataType;
	pType->m_Flags = Flags & ~EPtrTypeFlag_ReadOnly; // not really necessary, just to keep it clean

	m_DataPtrTypeList.InsertTail (pType);
	pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5] = pType;	
	return pType;
}

CStructType*
CTypeMgr::GetDataPtrStructType (CType* pDataType)
{
	CDataPtrTypeTuple* pTuple = GetDataPtrTypeTuple (pDataType);
	if (pTuple->m_pPtrStructType)
		return pTuple->m_pPtrStructType;

	CStructType* pType = CreateDataPtrStructType (pDataType);
	pTuple->m_pPtrStructType = pType;
	return pType;
}

CClassPtrType* 
CTypeMgr::GetClassPtrType (
	CClassType* pClassType,
	EClassPtrType PtrTypeKind,
	int Flags
	)
{
	ASSERT ((size_t) PtrTypeKind < EClassPtrType__Count);

	CClassPtrTypeTuple* pTuple = GetClassPtrTypeTuple (pClassType);

	// ptrkind x const x nullable

	size_t i1 = PtrTypeKind;
	size_t i2 = (Flags & EPtrTypeFlag_Const) != 0;
	size_t i3 = (Flags & EPtrTypeFlag_Nullable) != 0;
		
	if (pTuple->m_PtrTypeArray [i1] [i2] [i3])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3];

	if (PtrTypeKind == EClassPtrType_Unsafe)
		Flags |= ETypeFlag_Pod;

	CClassPtrType* pType = AXL_MEM_NEW (CClassPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CClassPtrType::CreateSignature (pClassType, PtrTypeKind, Flags);
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_pTargetType = pClassType;
	pType->m_Flags = Flags;

	m_ClassPtrTypeList.InsertTail (pType);
	pTuple->m_PtrTypeArray [i1] [i2] [i3] = pType;	
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
	ASSERT ((size_t) PtrTypeKind < EFunctionPtrType__Count);

	CFunctionPtrTypeTuple* pTuple = GetFunctionPtrTypeTuple (pFunctionType);

	// ref x kind x nullable

	size_t i1 = TypeKind == EType_FunctionRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Nullable) != 0;

	if (pTuple->m_PtrTypeArray [i1] [i2] [i3])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3];

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
		Size = sizeof (TFunctionPtr_w);
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
	pType->m_pTargetType = pFunctionType;
	pType->m_Flags = Flags;

	m_FunctionPtrTypeList.InsertTail (pType);
	pTuple->m_PtrTypeArray [i1] [i2] [i3] = pType;
	return pType;
}

CStructType* 
CTypeMgr::GetFunctionPtrStructType (CFunctionType* pFunctionType)
{
	CFunctionPtrTypeTuple* pTuple = GetFunctionPtrTypeTuple (pFunctionType);
	if (pTuple->m_pPtrStructType)
		return pTuple->m_pPtrStructType;

	pTuple->m_pPtrStructType = CreateFunctionPtrStructType (pFunctionType);
	return pTuple->m_pPtrStructType;
}

CStructType* 
CTypeMgr::GetFunctionPtrStructType_w (CFunctionType* pFunctionType)
{
	CFunctionPtrTypeTuple* pTuple = GetFunctionPtrTypeTuple (pFunctionType);
	if (pTuple->m_pPtrStructType_w)
		return pTuple->m_pPtrStructType_w;

	pTuple->m_pPtrStructType_w = CreateFunctionPtrStructType_w (pFunctionType);
	return pTuple->m_pPtrStructType_w;
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
	ASSERT ((size_t) PtrTypeKind < EPropertyPtrType__Count);

	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);

	// ref x kind x nullable

	size_t i1 = TypeKind == EType_PropertyRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Nullable) != 0;

	if (pTuple->m_PtrTypeArray [i1] [i2] [i3])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3];

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
			Size = sizeof (TPropertyPtr_w);
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
			Size = sizeof (TAuPropertyPtr_t);
			break;

		case EPropertyPtrType_Weak:
			Size = sizeof (TAuPropertyPtr_w);
			break;

		case EPropertyPtrType_Unsafe:
			Size = sizeof (TAuPropertyPtr_u);
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
	pType->m_pTargetType = pPropertyType;
	pType->m_Flags = Flags;

	m_PropertyPtrTypeList.InsertTail (pType);
	pTuple->m_PtrTypeArray [i1] [i2] [i3] = pType;
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
	if (pTuple->m_pPtrStructType)
		return pTuple->m_pPtrStructType;

	pTuple->m_pPtrStructType = CreatePropertyPtrStructType (pPropertyType);
	return pTuple->m_pPtrStructType;
}

CStructType* 
CTypeMgr::GetPropertyPtrStructType_w (CPropertyType* pPropertyType)
{
	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);
	if (pTuple->m_pPtrStructType_w)
		return pTuple->m_pPtrStructType_w;

	pTuple->m_pPtrStructType_w = CreatePropertyPtrStructType_w (pPropertyType);
	return pTuple->m_pPtrStructType_w;
}

CStructType* 
CTypeMgr::GetAuPropertyPtrStructType_t (CPropertyType* pPropertyType)
{
	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);
	if (pTuple->m_pAuPtrStructType_t)
		return pTuple->m_pAuPtrStructType_t;

	pTuple->m_pAuPtrStructType_t = CreateAuPropertyPtrStructType_t (pPropertyType);
	return pTuple->m_pAuPtrStructType_t;
}

CStructType* 
CTypeMgr::GetAuPropertyPtrStructType_u (CPropertyType* pPropertyType)
{
	CPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);
	if (pTuple->m_pAuPtrStructType_u)
		return pTuple->m_pAuPtrStructType_u;

	pTuple->m_pAuPtrStructType_u = CreateAuPropertyPtrStructType_u (pPropertyType);
	return pTuple->m_pAuPtrStructType_u;
}

CAutoEvPtrType* 
CTypeMgr::GetAutoEvPtrType (
	CAutoEvType* pAutoEvType,
	EType TypeKind,
	EAutoEvPtrType PtrTypeKind,
	int Flags
	)
{
	ASSERT (TypeKind == EType_AutoEvPtr || TypeKind == EType_AutoEvRef);
	ASSERT ((size_t) PtrTypeKind < EAutoEvPtrType__Count);

	CAutoEvPtrTypeTuple* pTuple = GetAutoEvPtrTypeTuple (pAutoEvType);

	// ref x kind x nullable

	size_t i1 = TypeKind == EType_AutoEvRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Nullable) != 0;

	if (pTuple->m_PtrTypeArray [i1] [i2] [i3])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3];

	size_t Size = 0;

	switch (PtrTypeKind)
	{
	case EAutoEvPtrType_Normal:
		Size = sizeof (TAutoEvPtr);
		break;

	case EAutoEvPtrType_Thin:
		Size = sizeof (void**);
		break;
		
	case EAutoEvPtrType_Weak:
		Size = sizeof (TAutoEvPtr_w);
		break;

	case EAutoEvPtrType_Unsafe:
		Size = sizeof (void**);
		Flags |= ETypeFlag_Pod;
		break;

	default:
		ASSERT (false);
	}

	CAutoEvPtrType* pType = AXL_MEM_NEW (CAutoEvPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CAutoEvPtrType::CreateSignature (pAutoEvType, TypeKind, PtrTypeKind, Flags);
	pType->m_TypeKind = TypeKind;
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_Size = Size;
	pType->m_pTargetType = pAutoEvType;
	pType->m_Flags = Flags;

	m_AutoEvPtrTypeList.InsertTail (pType);
	pTuple->m_PtrTypeArray [i1] [i2] [i3] = pType;
	return pType;
}

CStructType* 
CTypeMgr::GetAutoEvVTableStructType (CAutoEvType* pAutoEvType)
{
	if (pAutoEvType->m_pVTableStructType)
		return pAutoEvType->m_pVTableStructType;

	pAutoEvType->m_pVTableStructType = CreateAutoEvVTableStructType (pAutoEvType);
	return pAutoEvType->m_pVTableStructType;
}

CStructType* 
CTypeMgr::GetAutoEvPtrStructType (CAutoEvType* pAutoEvType)
{
	CAutoEvPtrTypeTuple* pTuple = GetAutoEvPtrTypeTuple (pAutoEvType);
	if (pTuple->m_pPtrStructType)
		return pTuple->m_pPtrStructType;

	pTuple->m_pPtrStructType = CreateAutoEvPtrStructType (pAutoEvType);
	return pTuple->m_pPtrStructType;
}

CStructType* 
CTypeMgr::GetAutoEvPtrStructType_w (CAutoEvType* pAutoEvType)
{
	CAutoEvPtrTypeTuple* pTuple = GetAutoEvPtrTypeTuple (pAutoEvType);
	if (pTuple->m_pPtrStructType_w)
		return pTuple->m_pPtrStructType_w;

	pTuple->m_pPtrStructType_w = CreateAutoEvPtrStructType_w (pAutoEvType);
	return pTuple->m_pPtrStructType_w;
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

CPropertyTypeTuple*
CTypeMgr::GetPropertyTypeTuple (CType* pType)
{
	if (pType->m_pPropertyTypeTuple)
		return pType->m_pPropertyTypeTuple;

	CPropertyTypeTuple* pTuple = AXL_MEM_NEW (CPropertyTypeTuple);
	pType->m_pPropertyTypeTuple = pTuple;
	m_PropertyTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

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

CAutoEvPtrTypeTuple*
CTypeMgr::GetAutoEvPtrTypeTuple (CAutoEvType* pAutoEvType)
{
	if (pAutoEvType->m_pAutoEvPtrTypeTuple)
		return pAutoEvType->m_pAutoEvPtrTypeTuple;

	CAutoEvPtrTypeTuple* pTuple = AXL_MEM_NEW (CAutoEvPtrTypeTuple);
	pAutoEvType->m_pAutoEvPtrTypeTuple = pTuple;
	m_AutoEvPtrTypeTupleList.InsertTail (pTuple);
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
	pType->m_pPropertyTypeTuple = NULL;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CStructType* 
CTypeMgr::CreateDataPtrValidatorType ()
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag = _T("ptrvtor");
	pType->CreateFieldMember (GetStdType (EStdType_BytePtr));
	pType->CreateFieldMember (GetStdType (EStdType_BytePtr));
	pType->CreateFieldMember (GetPrimitiveType (EType_SizeT));
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateObjectHdrType ()
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag = _T("objhdr");
	pType->CreateFieldMember (GetStdType (EStdType_BytePtr));  // CClassType* m_pType;
	pType->CreateFieldMember (GetPrimitiveType (EType_SizeT)); // size_t m_ScopeLevel;
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
CTypeMgr::CreateAutoEvBindSiteType ()
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag = _T("aevbindsite");
	pType->CreateFieldMember (GetStdType (EStdType_SimpleEventPtr));
	pType->CreateFieldMember (GetPrimitiveType (EType_Int_p));
	pType->CalcLayout ();
	return pType;
}

CClassType* 
CTypeMgr::CreateISchedulerType ()
{
	CClassType* pType = CreateClassType (_T("IScheduler"), _T("IScheduler"));
	CType* pReturnType = GetPrimitiveType (EType_Void);
	CType* pArgType = ((CFunctionType*) GetStdType (EStdType_SimpleFunction))->GetFunctionPtrType ();
	CFunctionType* pScheduleType = GetFunctionType (pReturnType, &pArgType, 1);
	pType->CreateMethodMember (EStorage_Abstract, _T("Schedule"), pScheduleType);
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateDataPtrStructType (CType* pBaseType)
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("%s.ptr"), pBaseType->GetTypeString ());
	pType->CreateFieldMember (pBaseType->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateFieldMember (GetStdType (EStdType_DataPtrValidator));
	pType->CalcLayout ();
	return pType;
}

CStructType* 
CTypeMgr::CreateFunctionPtrStructType (CFunctionType* pFunctionType)
{
	CFunctionType* pStdObjectMethodMemberType = pFunctionType->GetStdObjectMethodMemberType ();

	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("fn.ptr"));
	pType->CreateFieldMember (pStdObjectMethodMemberType->GetFunctionPtrType (EFunctionPtrType_Thin));
	pType->CreateFieldMember (GetStdType (EStdType_ObjectPtr));
	pType->CalcLayout ();
	return pType;
}

CStructType* 
CTypeMgr::CreateFunctionPtrStructType_w (CFunctionType* pFunctionType)
{
	CFunctionType* pStdObjectMethodMemberType = pFunctionType->GetStdObjectMethodMemberType ();
	CFunctionType* pStrenthenClosureType = (CFunctionType*) GetStdType (EStdType_StrenthenClosureFunction);

	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("fn.ptr_w"));
	pType->CreateFieldMember (pStdObjectMethodMemberType->GetFunctionPtrType (EFunctionPtrType_Thin));
	pType->CreateFieldMember (GetStdType (EStdType_ObjectPtr));
	pType->CreateFieldMember (pStrenthenClosureType->GetFunctionPtrType (EFunctionPtrType_Thin));
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreatePropertyVTableStructType (CPropertyType* pPropertyType)
{
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.vtbl"));
	pType->CreateFieldMember (pPropertyType->m_pGetterType->GetFunctionPtrType (EFunctionPtrType_Thin));

	size_t SetterTypeOverloadCount = pPropertyType->m_SetterType.GetOverloadCount ();
	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pSetterType = pPropertyType->m_SetterType.GetOverload (i);
		pType->CreateFieldMember (pSetterType->GetFunctionPtrType (EFunctionPtrType_Thin));
	}

	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreatePropertyPtrStructType (CPropertyType* pPropertyType)
{
	CPropertyType* pStdObjectPropertyMemberType = pPropertyType->GetStdObjectPropertyMemberType ();

	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.ptr"));

	if (!(pPropertyType->GetFlags () & EPropertyTypeFlag_Augmented))
	{
		pType->CreateFieldMember (pStdObjectPropertyMemberType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	}
	else
	{
		ASSERT (false);
		// thin pointer goes first
	}

	pType->CreateFieldMember (GetStdType (EStdType_ObjectPtr));

	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreatePropertyPtrStructType_w (CPropertyType* pPropertyType)
{
	ASSERT (false);

	CPropertyType* pStdObjectPropertyMemberType = pPropertyType->GetStdObjectPropertyMemberType ();
	CFunctionType* pStrenthenClosureType = (CFunctionType*) GetStdType (EStdType_StrenthenClosureFunction);
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.ptr_w"));
	pType->CreateFieldMember (pStdObjectPropertyMemberType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateFieldMember (GetStdType (EStdType_ObjectPtr));
	pType->CreateFieldMember (pStrenthenClosureType->GetFunctionPtrType (EFunctionPtrType_Unsafe));
	pType->CalcLayout ();
	return NULL;
}

CStructType*
CTypeMgr::CreateAuPropertyPtrStructType_t (CPropertyType* pPropertyType)
{
	ASSERT (pPropertyType->m_Flags & EPropertyTypeFlag_Augmented);
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.ptr_t"));
	pType->CreateFieldMember (pPropertyType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateFieldMember (pPropertyType->GetAuFieldStructType ()->GetDataPtrType ());
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateAuPropertyPtrStructType_u (CPropertyType* pPropertyType)
{
	ASSERT (pPropertyType->m_Flags & EPropertyTypeFlag_Augmented);
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.ptr_u"));
	pType->CreateFieldMember (pPropertyType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateFieldMember (pPropertyType->GetAuFieldStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateAutoEvVTableStructType (CAutoEvType* pAutoEvType)
{
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("autoev.vtbl"));
	pType->CreateFieldMember (pAutoEvType->m_pStarterType->GetFunctionPtrType (EFunctionPtrType_Thin));
	pType->CreateFieldMember (pAutoEvType->m_pStopperType->GetFunctionPtrType (EFunctionPtrType_Thin));
	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateAutoEvPtrStructType (CAutoEvType* pAutoEvType)
{
	CAutoEvType* pStdObjectAutoEvMemberType = pAutoEvType->GetStdObjectAutoEvMemberType ();

	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("autoev.ptr"));
	pType->CreateFieldMember (pStdObjectAutoEvMemberType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateFieldMember (GetStdType (EStdType_ObjectPtr));

	pType->CalcLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateAutoEvPtrStructType_w (CAutoEvType* pAutoEvType)
{
	ASSERT (false);

	CAutoEvType* pStdObjectAutoEvMemberType = pAutoEvType->GetStdObjectAutoEvMemberType ();
	CFunctionType* pStrenthenClosureType = (CFunctionType*) GetStdType (EStdType_StrenthenClosureFunction);
	CStructType* pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	pType->m_Tag.Format (_T("prop.ptr_w"));
	pType->CreateFieldMember (pStdObjectAutoEvMemberType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe));
	pType->CreateFieldMember (GetStdType (EStdType_ObjectPtr));
	pType->CreateFieldMember (pStrenthenClosureType->GetFunctionPtrType (EFunctionPtrType_Unsafe));
	pType->CalcLayout ();
	return NULL;
}
//.............................................................................

} // namespace jnc {
} // namespace axl {
