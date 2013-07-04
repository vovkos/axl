#include "pch.h"
#include "axl_jnc_TypeMgr.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_DeclTypeCalc.h"

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
	m_DataPtrTypeList.Clear ();
	m_ClassPtrTypeList.Clear ();
	m_FunctionPtrTypeList.Clear ();
	m_PropertyPtrTypeList.Clear ();
	m_NamedImportTypeList.Clear ();
	m_ImportPtrTypeList.Clear ();
	m_AutoEvClassTypeList.Clear ();
	m_FunctionClosureClassTypeList.Clear ();
	m_PropertyClosureClassTypeList.Clear ();
	m_DataClosureClassTypeList.Clear ();
	m_MulticastClassTypeList.Clear ();
	m_McSnapshotClassTypeList.Clear ();

	m_SimplePropertyTypeTupleList.Clear ();
	m_FunctionArgTupleList.Clear ();
	m_DataPtrTypeTupleList.Clear ();
	m_ClassPtrTypeTupleList.Clear ();
	m_FunctionPtrTypeTupleList.Clear ();
	m_PropertyPtrTypeTupleList.Clear ();

	m_FunctionArgList.Clear ();
	m_TypedefList.Clear ();

	m_TypeMap.Clear ();

	m_StaticDestructArray.Clear ();

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
		pType = GetPrimitiveType (EType_Int8_u)->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe);
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
		pType = ((CClassType*) GetStdType (EStdType_SimpleMulticast))->GetClassPtrType (EClassPtrType_Normal);
		break;

	case EStdType_AutoEvBindSite:
		pType = CreateAutoEvBindSiteType ();
		break;

	case EStdType_Binder:
		pType = GetFunctionType (GetStdType (EStdType_SimpleEventPtr), NULL, 0);
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
	char Buffer [256];
	rtl::CArrayT <CNamedImportType*> SuperImportTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CNamedImportType> ImportType = m_NamedImportTypeList.GetHead ();
	for (; ImportType; ImportType++)
	{
		CNamedImportType* pImportType = *ImportType;
		CModuleItem* pItem = pImportType->m_pAnchorNamespace->FindItemTraverse (pImportType->m_Name);
		if (!pItem)
		{
			err::SetFormatStringError ("unresolved import '%s'", pImportType->GetTypeString ().cc ());
			return false;
		}

		EModuleItem ItemKind = pItem->GetItemKind ();
		switch (ItemKind)
		{
		case EModuleItem_Type:
			pImportType->m_pActualType = (CType*) pItem;
			break;

		case EModuleItem_Typedef:
			pImportType->m_pActualType = ((CTypedef*) pItem)->GetType ();
			if (pImportType->m_pActualType->GetTypeKind () == EType_NamedImport)
				SuperImportTypeArray.Append (pImportType);
			break;

		default:
			err::SetFormatStringError ("'%s' is not a type", pImportType->GetTypeString ().cc ());
			return false;
		}
	}

	// eliminate super-imports and detect import loops

	size_t Count = SuperImportTypeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CNamedImportType* pSuperImportType = SuperImportTypeArray [i];
		pSuperImportType->m_Flags |= EImportTypeFlag_ImportLoop;
	
		CType* pType = pSuperImportType->m_pActualType;
		while (pType->m_TypeKind == EType_NamedImport)
		{
			CImportType* pImportType = (CImportType*) pType;
			if (pImportType->m_Flags & EImportTypeFlag_ImportLoop)
			{
				err::SetFormatStringError ("'%s': import loop detected", pImportType->GetTypeString ().cc ());
				return false;
			}

			pImportType->m_Flags |= EImportTypeFlag_ImportLoop;
			pType = pImportType->m_pActualType;
		}

		CType* pExternType = pType;
		while (pType->m_TypeKind == EType_NamedImport)
		{
			CImportType* pImportType = (CImportType*) pType;
			pImportType->m_pActualType = pExternType;
			pImportType->m_Flags &= ~EImportTypeFlag_ImportLoop;
			pType = pImportType->m_pActualType;
		}
	}

	rtl::CIteratorT <CImportPtrType> ImportPtrType = m_ImportPtrTypeList.GetHead ();
	for (; ImportPtrType; ImportPtrType++)
	{
		CImportPtrType* pImportType = *ImportPtrType;

		CDeclTypeCalc TypeCalc;

		CType* pType = TypeCalc.CalcPtrType (
			pImportType->m_pTargetType->m_pActualType, 
			pImportType->m_TypeModifiers
			);

		if (!pType)
			return false;

		if (pImportType->GetFlags () & EPtrTypeFlag_Checked)
			pType = GetCheckedPtrType (pType);

		pImportType->m_pActualType = pType;
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
	rtl::CString Signature = CBitFieldType::CreateSignature (pBaseType, BitOffset, BitCount);

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CBitFieldType*) It->m_Value;

	CBitFieldType* pType = AXL_MEM_NEW (CBitFieldType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pBaseType = pBaseType;
	pType->m_BitOffset = BitOffset;
	pType->m_BitCount = BitCount;

	m_BitFieldTypeList.InsertTail (pType);
	It->m_Value = pType;

	if (pBaseType->GetTypeKindFlags () & ETypeKindFlag_Import)
	{
		pType->m_pBaseType_i = (CImportType*) pBaseType;
		m_pModule->MarkForLayout (pType);
	}
	else
	{
		bool Result = pType->EnsureLayout ();
		if (!Result)
			return NULL;
	}

	return pType;
}

CArrayType* 
CTypeMgr::GetArrayType (
	CType* pElementType,
	size_t ElementCount
	)
{
	rtl::CString Signature = CArrayType::CreateSignature (pElementType, ElementCount);

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

	m_ArrayTypeList.InsertTail (pType);
	
	if (pElementType->GetTypeKindFlags () & ETypeKindFlag_Import)
	{
		pType->m_pElementType_i = (CImportType*) pElementType;
		m_pModule->MarkForLayout (pType);
	}
	else
	{
		bool Result = pType->EnsureLayout ();
		if (!Result)
			return NULL;
	}

	It->m_Value = pType;
	return pType;
}

CTypedef*
CTypeMgr::CreateTypedef (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CType* pType
	)
{
	CTypedef* pTypedef = AXL_MEM_NEW (CTypedef);
	pTypedef->m_Name = Name;
	pTypedef->m_QualifiedName = QualifiedName;
	pTypedef->m_Tag = QualifiedName;
	pTypedef->m_pType = pType;
	m_TypedefList.InsertTail (pTypedef);

	return pTypedef;
}

CEnumType* 
CTypeMgr::CreateEnumType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CType* pBaseType,
	uint_t Flags
	)
{
	const char* pSignaturePrefix = (Flags & EEnumTypeFlag_Exposed) ? "EC" : "EE";

	CEnumType* pType = AXL_MEM_NEW (CEnumType);

	if (Name.IsEmpty ())
	{
		m_UnnamedEnumTypeCounter++;
		pType->m_Signature.Format ("%s%d:%s", pSignaturePrefix, m_UnnamedEnumTypeCounter, m_pModule->GetFilePath ().cc ());
		pType->m_Tag.Format ("_unnamed_enum_%d", m_UnnamedEnumTypeCounter);
	}
	else
	{
		pType->m_Signature.Format ("%s%s", pSignaturePrefix, QualifiedName.cc ());
		pType->m_Name = Name;
		pType->m_QualifiedName = QualifiedName;
		pType->m_Tag = QualifiedName;
		pType->m_Flags |= ETypeFlag_Named;
	}

	if (!pBaseType)
		pBaseType = GetPrimitiveType (EType_Int);

	pType->m_pModule = m_pModule;
	pType->m_pBaseType = pBaseType;
	pType->m_Flags |= Flags;
	m_EnumTypeList.InsertTail (pType);

	if (pBaseType->GetTypeKindFlags () & ETypeKindFlag_Import)
		pType->m_pBaseType_i = (CImportType*) pBaseType;

	m_pModule->MarkForLayout (pType);
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
		pType->m_Signature.Format ("S%d:%s", m_UnnamedStructTypeCounter, m_pModule->GetFilePath ().cc ());
		pType->m_Tag.Format ("_unnamed_struct_%d", m_UnnamedStructTypeCounter);
	}
	else
	{
		pType->m_Signature.Format ("S%s", QualifiedName.cc ());
		pType->m_Name = Name;
		pType->m_QualifiedName = QualifiedName;
		pType->m_Tag = QualifiedName;
		pType->m_Flags |= ETypeFlag_Named;
	}

	pType->m_pModule = m_pModule;
	pType->m_PackFactor = PackFactor;
	m_StructTypeList.InsertTail (pType);
	m_pModule->MarkForLayout (pType);
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
		pType->m_Signature.Format ("U%d:%s", m_UnnamedUnionTypeCounter, m_pModule->GetFilePath ().cc ());
		pType->m_Tag.Format ("_unnamed_union_%d", m_UnnamedUnionTypeCounter);
	}
	else
	{
		pType->m_Signature.Format ("U%s", QualifiedName.cc ());
		pType->m_Name = Name;
		pType->m_QualifiedName = QualifiedName;
		pType->m_Tag = QualifiedName;
		pType->m_Flags |= ETypeFlag_Named;
	}

	m_pModule->MarkForLayout (pType); // before child struct

	CStructType* pUnionStructType = CreateUnnamedStructType ();
	pUnionStructType->m_pParentNamespace = pType;
	pUnionStructType->m_StructTypeKind = EStructType_UnionStruct;
	pUnionStructType->m_Tag.Format ("%s.struct", pType->m_Tag.cc ());

	pType->m_pModule = m_pModule;
	pType->m_pStructType = pUnionStructType;
	m_UnionTypeList.InsertTail (pType);
	return pType;
}

CClassType* 
CTypeMgr::CreateClassType (
	EClassType ClassTypeKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	size_t PackFactor
	)
{
	CClassType* pType;
	
	switch (ClassTypeKind)
	{
	case EClassType_AutoEv:
		pType = AXL_MEM_NEW (CAutoEvClassType);
		m_AutoEvClassTypeList.InsertTail ((CAutoEvClassType*) pType);
		break;

	case EClassType_FunctionClosure:
		pType = AXL_MEM_NEW (CFunctionClosureClassType);
		m_FunctionClosureClassTypeList.InsertTail ((CFunctionClosureClassType*) pType);
		break;

	case EClassType_PropertyClosure:
		pType = AXL_MEM_NEW (CPropertyClosureClassType);
		m_PropertyClosureClassTypeList.InsertTail ((CPropertyClosureClassType*) pType);
		break;

	case EClassType_DataClosure:
		pType = AXL_MEM_NEW (CDataClosureClassType);
		m_DataClosureClassTypeList.InsertTail ((CDataClosureClassType*) pType);
		break;

	case EClassType_Multicast:
		pType = AXL_MEM_NEW (CMulticastClassType);
		m_MulticastClassTypeList.InsertTail ((CMulticastClassType*) pType);
		break;

	case EClassType_McSnapshot:
		pType = AXL_MEM_NEW (CMcSnapshotClassType);
		m_McSnapshotClassTypeList.InsertTail ((CMcSnapshotClassType*) pType);
		break;

	default:
		pType = AXL_MEM_NEW (CClassType);
		m_ClassTypeList.InsertTail (pType);
	}	

	if (Name.IsEmpty ())
	{
		m_UnnamedClassTypeCounter++;
		pType->m_Signature.Format ("CC%d%s", m_UnnamedClassTypeCounter, m_pModule->GetFilePath ().cc ());
		pType->m_Tag.Format ("_unnamed_class_%d", m_UnnamedClassTypeCounter);
	}
	else
	{
		pType->m_Signature.Format ("CC%s", QualifiedName.cc ());
		pType->m_Name = Name;
		pType->m_QualifiedName = QualifiedName;
		pType->m_Tag = QualifiedName;
		pType->m_Flags |= ETypeFlag_Named;
	}

	m_pModule->MarkForLayout (pType); // before child structs

	CStructType* pVTableStructType = CreateUnnamedStructType ();
	pVTableStructType->m_Tag.Format ("%s.vtbl", pType->m_Tag.cc ());

	CStructType* pIfaceHdrStructType = CreateUnnamedStructType (PackFactor);
	pIfaceHdrStructType->m_Tag.Format ("%s.ifacehdr", pType->m_Tag.cc ());
	pIfaceHdrStructType->CreateField (pVTableStructType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe));
	pIfaceHdrStructType->CreateField (GetStdType (EStdType_ObjectHdr)->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe));

	CStructType* pIfaceStructType = CreateUnnamedStructType (PackFactor);
	pIfaceStructType->m_StructTypeKind = EStructType_IfaceStruct;
	pIfaceStructType->m_Tag.Format ("%s.iface", pType->m_Tag.cc ());
	pIfaceStructType->m_pParentNamespace = pType;
	pIfaceStructType->m_StorageKind = EStorage_Member;
	pIfaceStructType->m_PackFactor = PackFactor;
	pIfaceStructType->AddBaseType (pIfaceHdrStructType);

	CStructType* pClassStructType = CreateUnnamedStructType (PackFactor);
	pClassStructType->m_StructTypeKind = EStructType_ClassStruct;
	pClassStructType->m_Tag.Format ("%s.class", pType->m_Tag.cc ());
	pClassStructType->m_pParentNamespace = pType;
	pClassStructType->CreateField (GetStdType (EStdType_ObjectHdr));
	pClassStructType->CreateField (pIfaceStructType);

	pType->m_pModule = m_pModule;
	pType->m_ClassTypeKind = ClassTypeKind;
	pType->m_pVTableStructType = pVTableStructType;
	pType->m_pIfaceStructType = pIfaceStructType;
	pType->m_pClassStructType = pClassStructType;
	return pType;
}

CClassType*
CTypeMgr::GetBoxClassType (CType* pBaseType)
{
	if (pBaseType->m_pBoxClassType)
		return pBaseType->m_pBoxClassType;

	EType BaseTypeKind = pBaseType->GetTypeKind ();
	switch (BaseTypeKind)
	{
	case EType_Void:
		err::SetFormatStringError ("cannot create a box class for 'void'");
		return NULL;

	case EType_Class:
		return (CClassType*) pBaseType;
	}

	CClassType* pType = CreateUnnamedClassType (EClassType_Box);
	pType->m_Tag.Format ("object <%s>", pBaseType->GetTypeString ().cc ());
	pType->m_Signature.Format ("CB%s", pBaseType->GetSignature ().cc ());
	pType->CreateField (pBaseType);
	pType->EnsureLayout ();

	pBaseType->m_pBoxClassType = pType;
	return pType;
}

CFunctionArg*
CTypeMgr::CreateFunctionArg (
	const rtl::CString& Name,
	CType* pType,
	uint_t PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	CFunctionArg* pFunctionArg = AXL_MEM_NEW (CFunctionArg);
	pFunctionArg->m_pModule = m_pModule;
	pFunctionArg->m_Name = Name;
	pFunctionArg->m_QualifiedName = Name;
	pFunctionArg->m_Tag = Name;
	pFunctionArg->m_pType = pType;
	pFunctionArg->m_PtrTypeFlags = PtrTypeFlags;

	if (pInitializer)
		pFunctionArg->m_Initializer.TakeOver (pInitializer);

	m_FunctionArgList.InsertTail (pFunctionArg);
	
	if (pType->GetTypeKindFlags () & ETypeKindFlag_Import)
	{
		pFunctionArg->m_pType_i = (CImportType*) pType;
		m_pModule->MarkForLayout (pFunctionArg);
	}
	else
	{
		bool Result = pFunctionArg->EnsureLayout ();
		if (!Result)
			return NULL;
	}

	return pFunctionArg;
}

CFunctionArg*
CTypeMgr::GetSimpleFunctionArg (
	EStorage StorageKind,
	CType* pType,
	uint_t PtrTypeFlags
	)
{
	TFunctionArgTuple* pTuple = GetFunctionArgTuple (pType);

	// this x const x volatile
	
	size_t i1 = StorageKind == EStorage_This;
	size_t i2 = (PtrTypeFlags & EPtrTypeFlag_Const) != 0;
	size_t i3 = (PtrTypeFlags & EPtrTypeFlag_Volatile) != 0;
		
	if (pTuple->m_ArgArray [i1] [i2] [i3])
		return pTuple->m_ArgArray [i1] [i2] [i3];

	CFunctionArg* pArg = CreateFunctionArg (rtl::CString (), pType, PtrTypeFlags);
	if (!pArg)
		return NULL;

	pArg->m_StorageKind = StorageKind;

	pTuple->m_ArgArray [i1] [i2] [i3] = pArg;
	return pArg;
}

CFunctionType* 
CTypeMgr::GetFunctionType (
	ECallConv CallConv,
	CType* pReturnType,
	const rtl::CArrayT <CFunctionArg*>& ArgArray,
	uint_t Flags
	)
{
	if (!pReturnType)
		pReturnType = GetPrimitiveType (EType_Void);

	rtl::CString Signature = CFunctionType::CreateSignature (
		CallConv, 
		pReturnType, 
		ArgArray, 
		ArgArray.GetCount (), 
		Flags
		);

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CFunctionType*) It->m_Value;

	CFunctionType* pType = AXL_MEM_NEW (CFunctionType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pReturnType = pReturnType;
	pType->m_CallConv = CallConv;
	pType->m_Flags = Flags;
	pType->m_ArgArray = ArgArray;

	m_FunctionTypeList.InsertTail (pType);

	if (pReturnType->GetTypeKindFlags () & ETypeKindFlag_Import)
	{
		pType->m_pReturnType_i = (CImportType*) pReturnType;
		m_pModule->MarkForLayout (pType);
	}
	else
	{
		bool Result = pType->EnsureLayout ();
		if (!Result)
			return NULL;
	}

	It->m_Value = pType;	
	return pType;
}

CFunctionType* 
CTypeMgr::GetFunctionType (
	ECallConv CallConv,
	CType* pReturnType,
	CType* const* pArgTypeArray,
	size_t ArgCount, 
	uint_t Flags
	)
{
	if (!pReturnType)
		pReturnType = GetPrimitiveType (EType_Void);

	rtl::CArrayT <CFunctionArg*> ArgArray;
	ArgArray.SetCount (ArgCount);
	for (size_t i = 0; i < ArgCount; i++)
	{
		CFunctionArg* pArg = GetSimpleFunctionArg (pArgTypeArray [i]);
		if (!pArg)
			return NULL;

		ArgArray [i] = pArg;
	}

	rtl::CString Signature = CFunctionType::CreateSignature (
		CallConv, 
		pReturnType, 
		pArgTypeArray, 
		ArgCount, 
		Flags
		);

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CFunctionType*) It->m_Value;

	CFunctionType* pType = AXL_MEM_NEW (CFunctionType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pReturnType = pReturnType;
	pType->m_CallConv = CallConv;
	pType->m_Flags = Flags;
	pType->m_ArgArray = ArgArray;

	m_FunctionTypeList.InsertTail (pType);
	
	if (pReturnType->GetTypeKindFlags () & ETypeKindFlag_Import)
	{
		pType->m_pReturnType_i = (CImportType*) pReturnType;
		m_pModule->MarkForLayout (pType);
	}
	else
	{
		bool Result = pType->EnsureLayout ();
		if (!Result)
			return NULL;
	}

	It->m_Value = pType;	
	return pType;
}

CFunctionType* 
CTypeMgr::CreateUserFunctionType (
	ECallConv CallConv,
	CType* pReturnType,
	const rtl::CArrayT <CFunctionArg*>& ArgArray,
	uint_t Flags
	)
{
	if (!pReturnType)
		pReturnType = GetPrimitiveType (EType_Void);

	rtl::CString Signature = CFunctionType::CreateSignature (
		CallConv, 
		pReturnType, 
		ArgArray, 
		ArgArray.GetCount (), 
		Flags
		);

	CFunctionType* pType = AXL_MEM_NEW (CFunctionType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pReturnType = pReturnType;
	pType->m_CallConv = CallConv;
	pType->m_Flags = Flags | EFunctionTypeFlag_User;
	pType->m_ArgArray = ArgArray;

	m_FunctionTypeList.InsertTail (pType);

	if (pReturnType->GetTypeKindFlags () & ETypeKindFlag_Import)
	{
		pType->m_pReturnType_i = (CImportType*) pReturnType;
		m_pModule->MarkForLayout (pType);
	}
	else
	{
		bool Result = pType->EnsureLayout ();
		if (!Result)
			return NULL;
	}

	return pType;
}

CFunctionType* 
CTypeMgr::GetMemberMethodType (
	CNamedType* pParentType,
	CFunctionType* pFunctionType,
	uint_t ThisArgPtrTypeFlags
	)
{
	if (!IsClassType (pParentType, EClassType_StdObject)) // std object members are miscellaneous closures
		ThisArgPtrTypeFlags |= EPtrTypeFlag_Checked;

	CType* pThisArgType = pParentType->GetThisArgType (ThisArgPtrTypeFlags);
	CFunctionArg* pThisArg = GetSimpleFunctionArg (EStorage_This, pThisArgType);

	rtl::CArrayT <CFunctionArg*> ArgArray = pFunctionType->m_ArgArray;
	ArgArray.Insert (0, pThisArg);
	
	CFunctionType* pMemberMethodType;

	if (pFunctionType->m_Flags & EFunctionTypeFlag_User)
	{
		pMemberMethodType = CreateUserFunctionType (
			pFunctionType->m_CallConv,
			pFunctionType->m_pReturnType,
			ArgArray,
			pFunctionType->m_Flags
			);

		pMemberMethodType->m_pShortType = pFunctionType;
	}
	else
	{
		pMemberMethodType = GetFunctionType (
			pFunctionType->m_CallConv,
			pFunctionType->m_pReturnType,
			ArgArray,
			pFunctionType->m_Flags
			);

		pMemberMethodType->m_pShortType = pFunctionType;
	}

	return pMemberMethodType;
}

CFunctionType* 
CTypeMgr::GetStdObjectMemberMethodType (CFunctionType* pFunctionType)
{
	if (pFunctionType->m_pStdObjectMemberMethodType)
		return pFunctionType->m_pStdObjectMemberMethodType;
	
	CClassType* pClassType = (CClassType*) GetStdType (EStdType_ObjectClass);
	pFunctionType->m_pStdObjectMemberMethodType = pClassType->GetMemberMethodType (pFunctionType);
	return pFunctionType->m_pStdObjectMemberMethodType;
}

CPropertyType* 
CTypeMgr::GetPropertyType (
	CFunctionType* pGetterType,
	const CFunctionTypeOverload& SetterType,
	uint_t Flags
	)
{
	rtl::CString Signature = CPropertyType::CreateSignature (pGetterType, SetterType, Flags);
	
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

	if (Flags & EPropertyTypeFlag_Bindable)
	{
		CFunctionType* pBinderType = (CFunctionType*) GetStdType (EStdType_Binder);
		if (pGetterType->IsMemberMethodType ())
			pBinderType = pBinderType->GetMemberMethodType (pGetterType->GetThisTargetType (), EPtrTypeFlag_Const);

		pType->m_pBinderType = pBinderType;
	}

	m_PropertyTypeList.InsertTail (pType);

	It->m_Value = pType;	
	return pType;
}

CPropertyType* 
CTypeMgr::GetSimplePropertyType (
	ECallConv CallConv,
	CType* pReturnType,
	uint_t Flags
	)
{
	TSimplePropertyTypeTuple* pTuple = GetSimplePropertyTypeTuple (pReturnType);

	size_t i1 = CallConv == ECallConv_Stdcall;
	size_t i2 = (Flags & EPropertyTypeFlag_Const) != 0;
	size_t i3 = (Flags & EPropertyTypeFlag_Bindable) != 0;

	if (pTuple->m_PropertyTypeArray [i1] [i2] [i3])
		return pTuple->m_PropertyTypeArray [i1] [i2] [i3];

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

	pTuple->m_PropertyTypeArray [i1] [i2] [i3] = pPropertyType;
	return pPropertyType;
}

CPropertyType* 
CTypeMgr::GetIndexedPropertyType (
	ECallConv CallConv,
	CType* pReturnType,
	CType* const* pIndexArgTypeArray,
	size_t IndexArgCount,
	uint_t Flags
	)
{
	CFunctionType* pGetterType = GetFunctionType (CallConv, pReturnType, pIndexArgTypeArray, IndexArgCount, 0);

	if (Flags & EPropertyTypeFlag_Const)
		return GetPropertyType (pGetterType, NULL, Flags);

	char Buffer [256];
	rtl::CArrayT <CType*> ArgTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgTypeArray.Copy (pIndexArgTypeArray, IndexArgCount);
	ArgTypeArray.Append (pReturnType);

	CFunctionType* pSetterType = GetFunctionType (CallConv, NULL, ArgTypeArray, IndexArgCount + 1, 0);
	return GetPropertyType (pGetterType, pSetterType, Flags);
}

CPropertyType* 
CTypeMgr::GetIndexedPropertyType (
	ECallConv CallConv,
	CType* pReturnType,
	const rtl::CArrayT <CFunctionArg*>& ArgArray,
	uint_t Flags
	)
{
	CFunctionType* pGetterType = GetFunctionType (CallConv, pReturnType, ArgArray, 0);

	if (Flags & EPropertyTypeFlag_Const)
		return GetPropertyType (pGetterType, NULL, Flags);

	rtl::CArrayT <CFunctionArg*> SetterArgArray = ArgArray;
	SetterArgArray.Append (pReturnType->GetSimpleFunctionArg ());

	CFunctionType* pSetterType = GetFunctionType (CallConv, NULL, SetterArgArray, 0);
	return GetPropertyType (pGetterType, pSetterType, Flags);
}

CPropertyType* 
CTypeMgr::CreateIndexedPropertyType (
	ECallConv CallConv,
	CType* pReturnType,
	const rtl::CArrayT <CFunctionArg*>& ArgArray,
	uint_t Flags
	)
{
	CFunctionType* pGetterType = CreateUserFunctionType (CallConv, pReturnType, ArgArray, 0);

	if (Flags & EPropertyTypeFlag_Const)
		return GetPropertyType (pGetterType, NULL, Flags);

	rtl::CArrayT <CFunctionArg*> SetterArgArray = ArgArray;
	SetterArgArray.Append (pReturnType->GetSimpleFunctionArg ());

	CFunctionType* pSetterType = CreateUserFunctionType (CallConv, NULL, SetterArgArray, 0);
	return GetPropertyType (pGetterType, pSetterType, Flags);
}

CPropertyType* 
CTypeMgr::GetMemberPropertyType (
	CNamedType* pParentType,
	CPropertyType* pPropertyType
	)
{
	CFunctionType* pGetterType = GetMemberMethodType (pParentType, pPropertyType->m_pGetterType);

	size_t SetterTypeOverloadCount = pPropertyType->m_SetterType.GetOverloadCount ();

	char Buffer [256];
	rtl::CArrayT <CFunctionType*> SetterTypeOverloadArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	SetterTypeOverloadArray.SetCount (SetterTypeOverloadCount);

	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pOverloadType = pPropertyType->m_SetterType.GetOverload (i);
		SetterTypeOverloadArray [i] = GetMemberMethodType (pParentType, pOverloadType);
	}

	CPropertyType* pMemberPropertyType = GetPropertyType (
		pGetterType, 
		CFunctionTypeOverload (SetterTypeOverloadArray, SetterTypeOverloadCount), 
		pPropertyType->m_Flags
		);

	pMemberPropertyType->m_pShortType = pPropertyType;
	return pMemberPropertyType;
}

CPropertyType* 
CTypeMgr::GetStdObjectMemberPropertyType (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pStdObjectMemberPropertyType)
		return pPropertyType->m_pStdObjectMemberPropertyType;

	CClassType* pClassType = (CClassType*) GetStdType (EStdType_ObjectClass);
	pPropertyType->m_pStdObjectMemberPropertyType = pClassType->GetMemberPropertyType (pPropertyType);
	return pPropertyType->m_pStdObjectMemberPropertyType;
}

CPropertyType* 
CTypeMgr::GetShortPropertyType (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pShortType)
		return pPropertyType->m_pShortType;
	
	if (!pPropertyType->IsMemberPropertyType ())
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

CClassType* 
CTypeMgr::GetMulticastType (CFunctionPtrType* pFunctionPtrType)
{
	bool Result;

	if (pFunctionPtrType->m_pMulticastType)
		return pFunctionPtrType->m_pMulticastType;

	CType* pReturnType = pFunctionPtrType->GetTargetType ()->GetReturnType ();
	if (pReturnType->GetTypeKind () != EType_Void)
	{
		err::SetFormatStringError ("multicast cannot only return void, not '%s'", pReturnType->GetTypeString ());
		return NULL;
	}

	CMulticastClassType* pType = (CMulticastClassType*) CreateUnnamedClassType (EClassType_Multicast);
	pType->m_pTargetType = pFunctionPtrType;

	// fields

	pType->m_FieldArray [EMulticastField_Lock] = pType->CreateField (GetPrimitiveType (EType_Int_p), 0, EPtrTypeFlag_Volatile);
	pType->m_FieldArray [EMulticastField_MaxCount] = pType->CreateField (GetPrimitiveType (EType_SizeT));
	pType->m_FieldArray [EMulticastField_Count] = pType->CreateField (GetPrimitiveType (EType_SizeT));
	pType->m_FieldArray [EMulticastField_PtrArray] = pType->CreateField (pFunctionPtrType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe));
	pType->m_FieldArray [EMulticastField_HandleTable] = pType->CreateField (GetPrimitiveType (EType_Int_p));

	CType* pArgType;
	CFunctionType* pMethodType;	

	// methods

	pMethodType = GetFunctionType (NULL, NULL, 0);
	pType->m_MethodArray [EMulticastMethod_Clear] = pType->CreateMethod (EStorage_Member, "Clear", pMethodType);
	
	pReturnType = GetPrimitiveType (EType_Int_p);
	pArgType = pFunctionPtrType;
	pMethodType = GetFunctionType (pReturnType, &pArgType, 1);

	pType->m_MethodArray [EMulticastMethod_Set] = pType->CreateMethod (EStorage_Member, "Set", pMethodType);
	pType->m_MethodArray [EMulticastMethod_Add] = pType->CreateMethod (EStorage_Member, "Add", pMethodType);

	pReturnType = pFunctionPtrType;
	pArgType = GetPrimitiveType (EType_Int_p);
	pMethodType = GetFunctionType (pReturnType, &pArgType, 1);
	pType->m_MethodArray [EMulticastMethod_Remove] = pType->CreateMethod (EStorage_Member, "Remove", pMethodType);

	pReturnType = pFunctionPtrType->GetTargetType ()->GetFunctionPtrType ();
	pMethodType = GetFunctionType (pReturnType, NULL, 0);
	pType->m_MethodArray [EMulticastMethod_GetSnapshot] = pType->CreateMethod (EStorage_Member, "GetSnapshot", pMethodType);

	pMethodType = pFunctionPtrType->GetTargetType ();
	pType->m_MethodArray [EMulticastMethod_Call] = pType->CreateMethod (EStorage_Member, "Call", pMethodType);

	// snapshot closure

	CMcSnapshotClassType* pSnapshotType = (CMcSnapshotClassType*) CreateUnnamedClassType (EClassType_McSnapshot);
	pSnapshotType->m_pTargetType = pFunctionPtrType;

	// fields

	pSnapshotType->m_FieldArray [EMcSnapshotField_Count] = pSnapshotType->CreateField (GetPrimitiveType (EType_SizeT));
	pSnapshotType->m_FieldArray [EMcSnapshotField_PtrArray] = pSnapshotType->CreateField (pFunctionPtrType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe));

	// call method

	pMethodType = pFunctionPtrType->GetTargetType ();
	pSnapshotType->m_MethodArray [EMcSnapshotMethod_Call] = pSnapshotType->CreateMethod (EStorage_Member, "Call", pMethodType);

	pType->m_pSnapshotType = pSnapshotType;
	
	if (!m_pModule->m_NamespaceMgr.GetCurrentScope ())
	{
		m_pModule->MarkForLayout (pType);
		m_pModule->MarkForLayout (pType->m_pSnapshotType);
	}
	else
	{
		Result = 
			pType->CalcLayout () &&
			pType->m_pSnapshotType->CalcLayout ();

		if (!Result)
			return NULL;
	}

	m_pModule->MarkForCompile (pType);
	m_pModule->MarkForCompile (pType->m_pSnapshotType);

	pFunctionPtrType->m_pMulticastType = pType;
	return pType;
}

CClassType* 
CTypeMgr::GetAutoEvInterfaceType (CFunctionType* pStartMethodType)
{
	CType* pReturnType = pStartMethodType->GetReturnType ();
	if (pReturnType->GetTypeKind () != EType_Void)
	{
		err::SetFormatStringError ("autoev must return 'void', not '%s'", pReturnType->GetTypeString ());
		return NULL;
	}

	if (pStartMethodType->m_pAutoEvInterfaceType)
		return pStartMethodType->m_pAutoEvInterfaceType;

	CClassType* pType = CreateUnnamedClassType (EClassType_AutoEvIface);
	pType->m_Signature.Format ("CA%s", pStartMethodType->GetTypeString ().cc ());
	pType->CreateMethod (EStorage_Abstract, "Start", pStartMethodType);
	pType->CreateMethod (EStorage_Abstract, "Stop", (CFunctionType*) GetStdType (EStdType_SimpleFunction));
	return pType;
}

CAutoEvClassType* 
CTypeMgr::CreateAutoEvType (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CClassType* pIfaceType,
	CClassType* pParentType
	)
{
	CAutoEvClassType* pType = (CAutoEvClassType*) CreateClassType (EClassType_AutoEv, Name, QualifiedName);
	
	pType->AddBaseType (pIfaceType);

	// fields

	pType->m_FieldArray [EAutoEvField_Lock]  = pType->CreateField (m_pModule->GetSimpleType (EType_Int_p));
	pType->m_FieldArray [EAutoEvField_State] = pType->CreateField (m_pModule->GetSimpleType (EType_Int_p));

	rtl::CArrayT <CFunction*> VirtualMethodArray = pIfaceType->GetVirtualMethodArray ();
	ASSERT (VirtualMethodArray.GetCount () == 2);

	CFunctionType* pStartMethodType = VirtualMethodArray [0]->GetType ()->GetShortType ();
	rtl::CArrayT <CFunctionArg*> ArgArray = pStartMethodType->GetArgArray ();

	size_t ArgCount = ArgArray.GetCount ();
	for (size_t i = 0; i < ArgCount; i++)
	{
		CFunctionArg* pArg = ArgArray [i];
		CStructField* pField = pType->CreateField (pArg->GetName (), pArg->GetType ());
		if (!pField)
			return NULL;

		if (i == 0)
			pType->m_FirstArgField = pField;
	}

	// constructor & destructor

	if (pParentType)
	{
		CClassPtrType* pParentPtrType = pParentType->GetClassPtrType ();
		
		pType->m_Flags |= ETypeFlag_Child;
		pType->m_FieldArray [EAutoEvField_Parent] = pType->CreateField (pParentPtrType);

		CFunctionType* pConstructorType = GetFunctionType (NULL, (CType**) &pParentPtrType, 1);
		CFunction* pConstructor = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Constructor, pConstructorType);
		pType->AddMethod (pConstructor);
	} 
	else 
	{
		pType->CreateDefaultMemberMethod (EFunction_Constructor);
	}

	pType->CreateDefaultMemberMethod (EFunction_Destructor);

	// methods

	pType->m_MethodArray [EAutoEvMethod_Start] = pType->CreateMethod (EStorage_Override, "Start", pStartMethodType);
	pType->m_MethodArray [EAutoEvMethod_Stop]  = pType->CreateMethod (EStorage_Override, "Stop", (CFunctionType*) GetStdType (EStdType_SimpleFunction));
	return pType;
}

CFunctionClosureClassType* 
CTypeMgr::GetFunctionClosureClassType (
	CFunctionType* pTargetType,
	CFunctionType* pThunkType,
	CType* const* ppArgTypeArray,
	const size_t* pClosureMap,
	size_t ArgCount,
	uint64_t WeakMask
	)
{
	rtl::CString Signature = CClosureClassType::CreateSignature (
		pTargetType,
		pThunkType,
		ppArgTypeArray, 
		pClosureMap, 
		ArgCount, 
		WeakMask
		);

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CFunctionClosureClassType*) It->m_Value;

	CFunctionClosureClassType* pType = (CFunctionClosureClassType*) CreateUnnamedClassType (EClassType_FunctionClosure);
	pType->m_Signature = Signature;
	pType->m_ClosureMap.Copy (pClosureMap, ArgCount);
	pType->m_WeakMask = WeakMask;

	pType->CreateField (pTargetType->GetFunctionPtrType (EFunctionPtrType_Thin));

	for (size_t i = 0; i < ArgCount; i++)
		pType->CreateField (ppArgTypeArray [i]);

	CFunction* pThunkFunction = m_pModule->m_FunctionMgr.CreateInternalFunction ("thunk_function", pThunkType);
	pType->AddMethod (pThunkFunction);
	pType->m_pThunkFunction = pThunkFunction;

	pType->EnsureLayout ();
	m_pModule->MarkForCompile (pType);

	It->m_Value = pType;

	return pType;
}

CPropertyClosureClassType* 
CTypeMgr::GetPropertyClosureClassType (
	CPropertyType* pTargetType,
	CPropertyType* pThunkType,
	CType* const* ppArgTypeArray,
	const size_t* pClosureMap,
	size_t ArgCount,
	uint64_t WeakMask
	)
{
	rtl::CString Signature = CClosureClassType::CreateSignature (
		pTargetType,
		pThunkType,
		ppArgTypeArray, 
		pClosureMap, 
		ArgCount, 
		WeakMask
		);

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CPropertyClosureClassType*) It->m_Value;

	CPropertyClosureClassType* pType = (CPropertyClosureClassType*) CreateUnnamedClassType (EClassType_PropertyClosure);
	pType->m_Signature = Signature;
	pType->m_ClosureMap.Copy (pClosureMap, ArgCount);
	pType->m_WeakMask = WeakMask;

	pType->CreateField (pTargetType->GetPropertyPtrType (EPropertyPtrType_Thin));

	for (size_t i = 0; i < ArgCount; i++)
		pType->CreateField (ppArgTypeArray [i]);

	CProperty* pThunkProperty = m_pModule->m_FunctionMgr.CreateInternalProperty ("thunk_property", pThunkType);
	pType->AddProperty (pThunkProperty);
	pType->m_pThunkProperty = pThunkProperty;

	pType->EnsureLayout ();
	m_pModule->MarkForCompile (pType);

	It->m_Value = pType;

	return pType;
}

CDataClosureClassType* 
CTypeMgr::GetDataClosureClassType (
	CType* pTargetType,
	CPropertyType* pThunkType
	)
{
	rtl::CString Signature = CDataClosureClassType::CreateSignature (pTargetType, pThunkType);

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CDataClosureClassType*) It->m_Value;

	CDataClosureClassType* pType = (CDataClosureClassType*) CreateUnnamedClassType (EClassType_DataClosure);
	pType->m_Signature = Signature;

	pType->CreateField (pTargetType->GetDataPtrType ());

	CProperty* pThunkProperty = m_pModule->m_FunctionMgr.CreateInternalProperty ("thunk_property", pThunkType);
	pType->AddProperty (pThunkProperty);
	pType->m_pThunkProperty = pThunkProperty;

	pType->EnsureLayout ();
	m_pModule->MarkForCompile (pType);

	It->m_Value = pType;

	return pType;
}

CDataPtrType* 
CTypeMgr::GetDataPtrType (
	CType* pDataType,
	EType TypeKind,
	EDataPtrType PtrTypeKind,
	uint_t Flags
	)
{
	ASSERT (TypeKind == EType_DataPtr || TypeKind == EType_DataRef);
	ASSERT ((size_t) PtrTypeKind < EDataPtrType__Count);
	ASSERT (pDataType->GetTypeKind () != EType_NamedImport); // for imports, GetImportPtrType () should be called

	ASSERT (TypeKind != EType_DataRef || pDataType->m_TypeKind != EType_DataRef); // dbl reference
	
	if (Flags & EPtrTypeFlag_Unsafe)
		Flags |= ETypeFlag_Pod;
	else if (TypeKind == EType_DataPtr && PtrTypeKind == EDataPtrType_Normal)
		Flags |= ETypeFlag_GcRoot;

	TDataPtrTypeTuple* pTuple = GetDataPtrTypeTuple (pDataType);
	
	// ref x ptrkind x volatile x const x checked

	size_t i1 = TypeKind == EType_DataRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Const) ? 0 : 1;
	size_t i4 = (Flags & EPtrTypeFlag_Volatile) ? 0 : 1;
	size_t i5 = (Flags & EPtrTypeFlag_Unsafe) ? 0 : (Flags & EPtrTypeFlag_Checked) ? 1 : 2;
		
	if (pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5];

	size_t Size = PtrTypeKind == EDataPtrType_Thin ? sizeof (void*) : sizeof (TDataPtr);
	
	CDataPtrType* pType = AXL_MEM_NEW (CDataPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CDataPtrType::CreateSignature (pDataType, TypeKind, PtrTypeKind, Flags);
	pType->m_TypeKind = TypeKind;
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_Size = Size;
	pType->m_AlignFactor = sizeof (void*);
	pType->m_pTargetType = pDataType;
	pType->m_Flags = Flags;

	m_DataPtrTypeList.InsertTail (pType);
	pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5] = pType;	
	return pType;
}

CStructType*
CTypeMgr::GetDataPtrStructType (CType* pDataType)
{
	TDataPtrTypeTuple* pTuple = GetDataPtrTypeTuple (pDataType);
	if (pTuple->m_pPtrStructType)
		return pTuple->m_pPtrStructType;

	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag.Format ("%s.ptr", pDataType->GetTypeString ().cc ());
	pType->CreateField (pDataType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe));
	pType->CreateField (GetStdType (EStdType_BytePtr));
	pType->CreateField (GetStdType (EStdType_BytePtr));
	pType->CreateField (GetPrimitiveType (EType_SizeT));
	pType->EnsureLayout ();

	pTuple->m_pPtrStructType = pType;
	return pType;
}

CClassPtrType* 
CTypeMgr::GetClassPtrType (
	CClassType* pClassType,
	EType TypeKind,
	EClassPtrType PtrTypeKind,
	uint_t Flags
	)
{
	ASSERT ((size_t) PtrTypeKind < EClassPtrType__Count);

	if (Flags & EPtrTypeFlag_Unsafe)
		Flags |= ETypeFlag_Pod;
	else if (TypeKind == EType_ClassPtr)
		Flags |= ETypeFlag_GcRoot;

	TClassPtrTypeTuple* pTuple = GetClassPtrTypeTuple (pClassType);

	// ref x ptrkind x const x checked

	size_t i1 = TypeKind == EType_ClassRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Const) ? 0 : 1;
	size_t i4 = (Flags & EPtrTypeFlag_Volatile) ? 0 : 1;
	size_t i5 = (Flags & EPtrTypeFlag_Unsafe) ? 0 : (Flags & EPtrTypeFlag_Checked) ? 1 : 2;
		
	if (pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5];

	CClassPtrType* pType = AXL_MEM_NEW (CClassPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CClassPtrType::CreateSignature (pClassType, TypeKind, PtrTypeKind, Flags);
	pType->m_TypeKind = TypeKind;
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_pTargetType = pClassType;
	pType->m_Flags = Flags;

	m_ClassPtrTypeList.InsertTail (pType);
	pTuple->m_PtrTypeArray [i1] [i2] [i3] [i4] [i5] = pType;	
	return pType;
}

CFunctionPtrType*
CTypeMgr::GetFunctionPtrType (
	CFunctionType* pFunctionType,
	EType TypeKind,
	EFunctionPtrType PtrTypeKind,
	uint_t Flags
	)
{
	ASSERT (TypeKind == EType_FunctionPtr || TypeKind == EType_FunctionRef);
	ASSERT ((size_t) PtrTypeKind < EFunctionPtrType__Count);

	if (Flags & EPtrTypeFlag_Unsafe)
		Flags |= ETypeFlag_Pod;
	else if (TypeKind == EType_FunctionPtr && PtrTypeKind != EFunctionPtrType_Thin)
		Flags |= ETypeFlag_GcRoot;

	TFunctionPtrTypeTuple* pTuple = GetFunctionPtrTypeTuple (pFunctionType);

	// ref x kind x checked

	size_t i1 = TypeKind == EType_FunctionRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Unsafe) ? 0 : (Flags & EPtrTypeFlag_Checked) ? 1 : 2;

	if (pTuple->m_PtrTypeArray [i1] [i2] [i3])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3];

	size_t Size = PtrTypeKind == EFunctionPtrType_Thin ? sizeof (void*) : sizeof (TFunctionPtr);

	CFunctionPtrType* pType = AXL_MEM_NEW (CFunctionPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CFunctionPtrType::CreateSignature (pFunctionType, TypeKind, PtrTypeKind, Flags);
	pType->m_TypeKind = TypeKind;
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_Size = Size;
	pType->m_AlignFactor = sizeof (void*);
	pType->m_pTargetType = pFunctionType;
	pType->m_Flags = Flags;

	m_FunctionPtrTypeList.InsertTail (pType);
	pTuple->m_PtrTypeArray [i1] [i2] [i3] = pType;
	return pType;
}

CStructType* 
CTypeMgr::GetFunctionPtrStructType (CFunctionType* pFunctionType)
{
	TFunctionPtrTypeTuple* pTuple = GetFunctionPtrTypeTuple (pFunctionType);
	if (pTuple->m_pPtrStructType)
		return pTuple->m_pPtrStructType;

	rtl::CString Signature;
	Signature.Format ("SP%s", pFunctionType->GetSignature ().cc ());

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CStructType*) It->m_Value;

	CFunctionType* pStdObjectMemberMethodType = pFunctionType->GetStdObjectMemberMethodType ();

	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag.Format ("%s.ptr", pFunctionType->GetTypeString ().cc ());
	pType->m_Signature = Signature;
	pType->CreateField (pStdObjectMemberMethodType->GetFunctionPtrType (EFunctionPtrType_Thin, EPtrTypeFlag_Unsafe));
	pType->CreateField (GetStdType (EStdType_ObjectPtr));
	pType->EnsureLayout ();

	pTuple->m_pPtrStructType = pType;
	It->m_Value = pType;
	return pType;
}

CPropertyPtrType* 
CTypeMgr::GetPropertyPtrType (
	CPropertyType* pPropertyType,
	EType TypeKind,
	EPropertyPtrType PtrTypeKind,
	uint_t Flags
	)
{
	ASSERT (TypeKind == EType_PropertyPtr || TypeKind == EType_PropertyRef);
	ASSERT ((size_t) PtrTypeKind < EPropertyPtrType__Count);

	if (Flags & EPtrTypeFlag_Unsafe)
		Flags |= ETypeFlag_Pod;
	else if (TypeKind == EType_PropertyPtr && PtrTypeKind != EPropertyPtrType_Thin)
		Flags |= ETypeFlag_GcRoot;

	TPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);

	// ref x kind x checked

	size_t i1 = TypeKind == EType_PropertyRef;
	size_t i2 = PtrTypeKind;
	size_t i3 = (Flags & EPtrTypeFlag_Unsafe) ? 0 : (Flags & EPtrTypeFlag_Checked) ? 1 : 2;

	if (pTuple->m_PtrTypeArray [i1] [i2] [i3])
		return pTuple->m_PtrTypeArray [i1] [i2] [i3];

	size_t Size = PtrTypeKind == EFunctionPtrType_Thin ? sizeof (void*) : sizeof (TPropertyPtr);

	CPropertyPtrType* pType = AXL_MEM_NEW (CPropertyPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = CPropertyPtrType::CreateSignature (pPropertyType, TypeKind, PtrTypeKind, Flags);
	pType->m_TypeKind = TypeKind;
	pType->m_PtrTypeKind = PtrTypeKind;
	pType->m_Size = Size;
	pType->m_AlignFactor = sizeof (void*);
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
	
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag.Format ("%s.vtbl", pPropertyType->GetTypeString ().cc ());

	if (pPropertyType->GetFlags () & EPropertyTypeFlag_Bindable)
		pType->CreateField (pPropertyType->m_pBinderType->GetFunctionPtrType (EFunctionPtrType_Thin));

	pType->CreateField (pPropertyType->m_pGetterType->GetFunctionPtrType (EFunctionPtrType_Thin));

	size_t SetterTypeOverloadCount = pPropertyType->m_SetterType.GetOverloadCount ();
	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pSetterType = pPropertyType->m_SetterType.GetOverload (i);
		pType->CreateField (pSetterType->GetFunctionPtrType (EFunctionPtrType_Thin));
	}

	pType->EnsureLayout ();

	pPropertyType->m_pVTableStructType = pType;
	return pType;
}

CStructType* 
CTypeMgr::GetPropertyPtrStructType (CPropertyType* pPropertyType)
{
	TPropertyPtrTypeTuple* pTuple = GetPropertyPtrTypeTuple (pPropertyType);
	if (pTuple->m_pPtrStructType)
		return pTuple->m_pPtrStructType;

	rtl::CString Signature;
	Signature.Format ("SP%s", pPropertyType->GetSignature ().cc ());

	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CStructType*) It->m_Value;

	CPropertyType* pStdObjectMemberPropertyType = pPropertyType->GetStdObjectMemberPropertyType ();

	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag.Format ("%s.ptr", pPropertyType->GetTypeString ().cc ());
	pType->m_Signature = Signature;
	pType->CreateField (pStdObjectMemberPropertyType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe));
	pType->CreateField (GetStdType (EStdType_ObjectPtr));
	pType->EnsureLayout ();

	pTuple->m_pPtrStructType = pType;
	It->m_Value = pType;
	return pType;
}

CNamedImportType*
CTypeMgr::GetNamedImportType (
	const CQualifiedName& Name,
	CNamespace* pAnchorNamespace
	)
{
	rtl::CString Signature = CNamedImportType::CreateSignature (Name, pAnchorNamespace);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CNamedImportType*) It->m_Value;

	CNamedImportType* pType = AXL_MEM_NEW (CNamedImportType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_Name = Name;
	pType->m_pAnchorNamespace = pAnchorNamespace;
	pType->m_pModule = m_pModule;	

	m_NamedImportTypeList.InsertTail (pType);
	It->m_Value = pType;

	return pType;
}

CImportPtrType*
CTypeMgr::GetImportPtrType (	
	CNamedImportType* pNamedImportType,
	uint_t TypeModifiers,
	uint_t Flags
	)
{
	rtl::CString Signature = CImportPtrType::CreateSignature (
		pNamedImportType, 
		TypeModifiers,
		Flags
		);
	
	rtl::CStringHashTableMapIteratorAT <CType*> It = m_TypeMap.Goto (Signature);
	if (It->m_Value)
		return (CImportPtrType*) It->m_Value;

	CImportPtrType* pType = AXL_MEM_NEW (CImportPtrType);
	pType->m_pModule = m_pModule;
	pType->m_Signature = Signature;
	pType->m_pTargetType = pNamedImportType;
	pType->m_TypeModifiers = TypeModifiers;
	pType->m_Flags = Flags;

	m_ImportPtrTypeList.InsertTail (pType);
	It->m_Value = pType;

	return pType;
}

CType*
CTypeMgr::GetCheckedPtrType (CType* pType)
{
	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_DataPtr:
		return ((CDataPtrType*) pType)->GetCheckedPtrType ();

	case EType_ClassPtr:
		return ((CClassPtrType*) pType)->GetCheckedPtrType ();

	case EType_FunctionPtr:
		return ((CFunctionPtrType*) pType)->GetCheckedPtrType ();

	case EType_PropertyPtr:
		return ((CPropertyPtrType*) pType)->GetCheckedPtrType ();

	case EType_ImportPtr:
		return ((CImportPtrType*) pType)->GetCheckedPtrType ();

	default:
		ASSERT (false);
		return pType;
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TSimplePropertyTypeTuple*
CTypeMgr::GetSimplePropertyTypeTuple (CType* pType)
{
	if (pType->m_pSimplePropertyTypeTuple)
		return pType->m_pSimplePropertyTypeTuple;

	TSimplePropertyTypeTuple* pTuple = AXL_MEM_NEW (TSimplePropertyTypeTuple);
	pType->m_pSimplePropertyTypeTuple = pTuple;
	m_SimplePropertyTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

TFunctionArgTuple*
CTypeMgr::GetFunctionArgTuple (CType* pType)
{
	if (pType->m_pFunctionArgTuple)
		return pType->m_pFunctionArgTuple;

	TFunctionArgTuple* pTuple = AXL_MEM_NEW (TFunctionArgTuple);
	pType->m_pFunctionArgTuple = pTuple;
	m_FunctionArgTupleList.InsertTail (pTuple);
	return pTuple;
}

TDataPtrTypeTuple*
CTypeMgr::GetDataPtrTypeTuple (CType* pType)
{
	if (pType->m_pDataPtrTypeTuple)
		return pType->m_pDataPtrTypeTuple;

	TDataPtrTypeTuple* pTuple = AXL_MEM_NEW (TDataPtrTypeTuple);
	pType->m_pDataPtrTypeTuple = pTuple;
	m_DataPtrTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

TClassPtrTypeTuple*
CTypeMgr::GetClassPtrTypeTuple (CClassType* pClassType)
{
	if (pClassType->m_pClassPtrTypeTuple)
		return pClassType->m_pClassPtrTypeTuple;

	TClassPtrTypeTuple* pTuple = AXL_MEM_NEW (TClassPtrTypeTuple);
	pClassType->m_pClassPtrTypeTuple = pTuple;
	m_ClassPtrTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

TFunctionPtrTypeTuple*
CTypeMgr::GetFunctionPtrTypeTuple (CFunctionType* pFunctionType)
{
	if (pFunctionType->m_pFunctionPtrTypeTuple)
		return pFunctionType->m_pFunctionPtrTypeTuple;

	TFunctionPtrTypeTuple* pTuple = AXL_MEM_NEW (TFunctionPtrTypeTuple);
	pFunctionType->m_pFunctionPtrTypeTuple = pTuple;
	m_FunctionPtrTypeTupleList.InsertTail (pTuple);
	return pTuple;
}

TPropertyPtrTypeTuple*
CTypeMgr::GetPropertyPtrTypeTuple (CPropertyType* pPropertyType)
{
	if (pPropertyType->m_pPropertyPtrTypeTuple)
		return pPropertyType->m_pPropertyPtrTypeTuple;

	TPropertyPtrTypeTuple* pTuple = AXL_MEM_NEW (TPropertyPtrTypeTuple);
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
	pType->m_Flags = ETypeFlag_Pod | EModuleItemFlag_LayoutReady;
	pType->m_Size = Size;
	pType->m_AlignFactor = Size;
	pType->m_Signature = pSignature;
	pType->m_pSimplePropertyTypeTuple = NULL;
	pType->m_pFunctionArgTuple = NULL;
	pType->m_pDataPtrTypeTuple = NULL;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CStructType*
CTypeMgr::CreateObjectHdrType ()
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag = "objhdr";
	pType->CreateField (GetStdType (EStdType_BytePtr));  // CClassType* m_pType;
	pType->CreateField (GetPrimitiveType (EType_SizeT)); // size_t m_ScopeLevel;
	pType->CreateField (GetPrimitiveType (EType_Int_p)); // intptr_t m_Flags;
	pType->EnsureLayout ();
	return pType;
}

CClassType*
CTypeMgr::CreateObjectType ()
{
	CClassType* pType = CreateUnnamedClassType (EClassType_StdObject);
	pType->m_Tag = "object";
	pType->m_Signature = "CO"; // special signature to ensure type equality between modules
	pType->EnsureLayout ();
	return pType;
}

CStructType*
CTypeMgr::CreateAutoEvBindSiteType ()
{
	CStructType* pType = CreateUnnamedStructType ();
	pType->m_Tag = "aevbindsite";
	pType->CreateField (GetStdType (EStdType_SimpleEventPtr));
	pType->CreateField (GetPrimitiveType (EType_Int_p));
	pType->EnsureLayout ();
	return pType;
}

CClassType* 
CTypeMgr::CreateISchedulerType ()
{
	CClassType* pType = CreateClassType ("IScheduler", "IScheduler");
	CType* pReturnType = GetPrimitiveType (EType_Void);
	CType* pArgType = ((CFunctionType*) GetStdType (EStdType_SimpleFunction))->GetFunctionPtrType ();
	CFunctionType* pScheduleType = GetFunctionType (pReturnType, &pArgType, 1);
	pType->CreateMethod (EStorage_Abstract, "Schedule", pScheduleType);
	pType->EnsureLayout ();
	return pType;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
