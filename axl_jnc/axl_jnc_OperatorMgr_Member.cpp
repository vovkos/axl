#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::GetFieldMember (
	const CValue& ThisValue,
	CStructField* pMember,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	bool Result;

	CNamespace* pParentNamespace = pMember->GetParentType ()->GetParentNamespace ();
	ASSERT (pParentNamespace);

	if (pMember->GetStorageKind () != EStorage_Static && ThisValue.IsEmpty ())
	{
		err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag);
		return false;
	}

	if (pParentNamespace->GetNamespaceKind () == ENamespace_Property)
	{
		CProperty* pProperty = (CProperty*) pParentNamespace;

		if (pMember->GetStorageKind () == EStorage_Static)
			return GetStructFieldMember (
				pProperty->GetStaticDataVariable (), 
				pMember,
				pCoord,
				pResultValue
				);

		CClassType* pParentClassType = pProperty->GetParentClassType ();
		ASSERT (pParentClassType);

		CBaseTypeCoord Coord;

		CValue FieldValue;
		Result = GetClassFieldMember (
			ThisValue, 
			pProperty->GetDataFieldMember (),
			&Coord,
			&FieldValue
			);

		if (!Result)
			return false;

		return GetStructFieldMember (
			FieldValue, 
			pMember,
			pCoord,
			pResultValue
			);
	}
	else
	{
		ASSERT (pParentNamespace->GetNamespaceKind () == ENamespace_Type && ((CNamedType*) pParentNamespace)->GetTypeKind () == EType_Class);
		CClassType* pClassType = (CClassType*) pParentNamespace;

		if (pMember->GetStorageKind () == EStorage_Static)
			return GetStructFieldMember (
				pClassType->GetStaticDataVariable (), 
				pMember,
				pCoord,
				pResultValue
				);

		return GetClassFieldMember (
			ThisValue, 
			pMember,
			pCoord,
			pResultValue
			);	
	}
}

bool
COperatorMgr::MemberOperator (
	const CValue& RawOpValue,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepDataRef);
	if (!Result)
		return false;

	CType* pType = OpValue.GetType ();
	if (pType->GetTypeKind () == EType_DataRef)
		pType = ((CDataPtrType*) pType)->GetTargetType ();

	if (pType->GetTypeKind () == EType_DataPtr)
	{
		pType = ((CDataPtrType*) pType)->GetTargetType ();

		Result = UnaryOperator (EUnOp_Indir, &OpValue);
		if (!Result)
			return false;
	}

	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Struct:
		return GetStructMember (OpValue, (CStructType*) pType, pName, pResultValue);

	case EType_Union:
		return GetUnionMember (OpValue, (CUnionType*) pType, pName, pResultValue);

	case EType_Multicast:
		return GetMulticastMember (OpValue, (CMulticastType*) pType, pName, pResultValue);

	case EType_ClassPtr:
		return 
			PrepareOperand (&OpValue) &&
			GetClassMember (OpValue, ((CClassPtrType*) pType)->GetTargetType (), pName, pResultValue);

	default:
		err::SetFormatStringError (_T("member operator cannot be applied to '%s'"), pType->GetTypeString ());
		return false;
	}
}

bool
COperatorMgr::WeakMemberOperator (
	const CValue& RawOpValue,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("weak member operator is not implemented yet"));
	return false;
}

/*

bool
COperatorMgr::PointerToMemberOperator (
	const CValue& RawOpValue,
	const tchar_t* pName,
	CValue* pResultValue
	)
{	
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	TUnaryOperatorTypeInfo TypeInfo;
	IUnaryOperator* pOperator = GetUnaryOperator (EUnOp_Ptr, OpValue.GetType (), &TypeInfo);
	if (pOperator)
	{
		CValue CastOpValue;
		Result = 
			CastOperator (OpValue, TypeInfo.m_pOpType, &CastOpValue) &&
			pOperator->Operator (CastOpValue, &OpValue) && 
			PrepareOperand (&OpValue);

		if (!Result)
			return false;
	}

	return MemberOperator (OpValue, pName, pResultValue);
}

*/

bool
COperatorMgr::GetStructMember (
	const CValue& OpValue,
	CStructType* pStructType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CBaseTypeCoord Coord;
	CModuleItem* pMember = pStructType->FindItemTraverse (pName, &Coord, ETraverse_NoParentNamespace);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pStructType->GetTypeString ());
		return false;
	}

	EModuleItem ItemKind = pMember->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_StructField:
		return GetStructFieldMember (OpValue, (CStructField*) pMember, &Coord, pResultValue);

	default:
		err::SetFormatStringError (_T("non-struct members are not supported yet"));
		return false;
	}
}

bool
COperatorMgr::GetStructFieldMember (
	const CValue& OpValue,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	pCoord->m_LlvmIndexArray.Append (pField->GetLlvmIndex ());
	pCoord->m_Offset += pField->GetOffset ();
	
	EValue OpValueKind = OpValue.GetValueKind ();
	if (OpValueKind == EValue_Const)
	{
		pResultValue->CreateConst (
			(char*) OpValue.GetConstData () + pCoord->m_Offset,
			pField->GetType () 
			);

		return true;
	}

	if (OpValue.GetType ()->GetTypeKind () != EType_DataRef)
	{
		m_pModule->m_LlvmBuilder.CreateExtractValue (
			OpValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (),
			pField->GetType (),
			pResultValue
			);

		return true;
	}

	CDataPtrType* pOpType = (CDataPtrType*) OpValue.GetType ();

	pCoord->m_LlvmIndexArray.Insert (0, 0);

	if (pOpType->GetPtrTypeKind () == EDataPtrType_Unsafe)
	{
		CType* pResultType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Unsafe, pField->GetPtrTypeFlags ());

		m_pModule->m_LlvmBuilder.CreateGep (
			OpValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (), 
			pResultType, 
			pResultValue
			);

		return true;
	}
	
	ASSERT (pOpType->GetTypeKind () == EType_DataRef);

	if (OpValue.GetValueKind () == EValue_Variable)
	{
		CType* pResultType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin, pField->GetPtrTypeFlags ());

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateGep (
			OpValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (), 
			NULL, 
			&PtrValue
			);
		
		pResultValue->SetLlvmValue(
			PtrValue.GetLlvmValue (), 
			pResultType,
			OpValue.GetVariable (), 
			OpValue.GetFlags () & EValueFlag_NoDataPtrRangeCheck // propagate 
			);
	}
	else
	{
		CType* pResultType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Normal, pField->GetPtrTypeFlags ());

		CValue PtrValue;
		CValue ValidatorValue;

		m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 1, NULL, &ValidatorValue);
		m_pModule->m_LlvmBuilder.CreateGep (
			PtrValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (), 
			NULL, 
			&PtrValue
			);

		CValue DataPtrValue = pResultType->GetUndefValue ();
		m_pModule->m_LlvmBuilder.CreateInsertValue (DataPtrValue, PtrValue, 0, NULL, &DataPtrValue);
		m_pModule->m_LlvmBuilder.CreateInsertValue (DataPtrValue, ValidatorValue, 1, pResultType, pResultValue);
	}

	return true;
}

bool
COperatorMgr::GetUnionMember (
	const CValue& OpValue,
	CUnionType* pUnionType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CModuleItem* pMember = pUnionType->FindItem (pName);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pUnionType->GetTypeString ());
		return false;
	}

	EModuleItem ItemKind = pMember->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_StructField:
		return GetUnionFieldMember (OpValue, (CStructField*) pMember, pResultValue);

	default:
		err::SetFormatStringError (_T("non-struct members are not supported yet"));
		return false;
	}
}

bool
COperatorMgr::GetUnionFieldMember (
	const CValue& OpValue,
	CStructField* pField,
	CValue* pResultValue
	)
{
	EValue OpValueKind = OpValue.GetValueKind ();
	if (OpValueKind == EValue_Const)
	{
		pResultValue->CreateConst (OpValue.GetConstData (), pField->GetType ());
		return true;
	}

	if (OpValue.GetType ()->GetTypeKind () != EType_DataRef)
	{
		err::SetFormatStringError (_T("union member operator on registers is not implemented yet"));
		return false;
	}

	CDataPtrType* pOpType = (CDataPtrType*) OpValue.GetType ();

	if (pOpType->GetPtrTypeKind () == EDataPtrType_Unsafe)
	{
		CType* pResultType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Unsafe, pField->GetPtrTypeFlags ());

		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pResultType, pResultValue);
		return true;
	}
	
	if (OpValue.GetValueKind () == EValue_Variable)
	{
		CType* pResultType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin, pField->GetPtrTypeFlags ());

		CType* pCastType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Unsafe);
		CValue CastValue;
		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pCastType, &CastValue);

		pResultValue->SetLlvmValue (
			CastValue.GetLlvmValue (), 
			pResultType,
			OpValue.GetVariable (), 
			OpValue.GetFlags () & EValueFlag_NoDataPtrRangeCheck // propagate trusted flag
			);
	}
	else
	{
		CType* pResultType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Normal, pField->GetPtrTypeFlags ());
		pResultValue->OverrideType (OpValue, pResultType);
	}

	return true;
}

bool
COperatorMgr::GetMulticastMember (
	const CValue& OpValue,
	CMulticastType* pMulticastType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
	EDataPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	CFunction* pFunction;

	if (_tcscmp (pName, _T("Set")) == 0)
		pFunction = pMulticastType->GetSetMethod (PtrTypeKind);
	else if (_tcscmp (pName, _T("Add")) == 0)
		pFunction = pMulticastType->GetAddMethod (PtrTypeKind);
	else if (_tcscmp (pName, _T("Remove")) == 0)
		pFunction = pMulticastType->GetRemoveMethod (PtrTypeKind);
	else if (_tcscmp (pName, _T("Snapshot")) == 0)
		pFunction = pMulticastType->GetSnapshotMethod (PtrTypeKind);
	else if (_tcscmp (pName, _T("Call")) == 0)
		pFunction = pMulticastType->GetCallMethod (PtrTypeKind);
	else 
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pMulticastType->GetTypeString ());
		return false;
	}

//	pResult
	


	return true;
}

bool
COperatorMgr::GetClassMember (
	const CValue& OpValue,
	CClassType* pClassType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CBaseTypeCoord Coord;
	CModuleItem* pMember = pClassType->FindItemTraverse (pName, &Coord, ETraverse_NoParentNamespace);	
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pClassType->GetTypeString ());
		return false;
	}

	CModuleItemDecl* pDecl = pMember->GetItemDecl ();
	if (pDecl->GetAccessKind () != EAccess_Public &&
		m_pModule->m_NamespaceMgr.GetAccessKind (Coord.m_pType) == EAccess_Public)
	{
		err::SetFormatStringError (_T("'%s.%s' is protected"), Coord.m_pType->GetQualifiedName (), pName);
		return false;
	}

	CheckClassPtrNull (OpValue);

	EModuleItem MemberKind = pMember->GetItemKind ();
	switch (MemberKind)
	{
	case EModuleItem_StructField:
		return GetClassFieldMember (
			OpValue, 
			(CStructField*) pMember, 
			&Coord,
			pResultValue
			);
		
	case EModuleItem_Function:
		pResultValue->SetFunction ((CFunction*) pMember);
		break;

	case EModuleItem_Property:
		pResultValue->SetProperty ((CProperty*) pMember);
		break;

	default:
		err::SetFormatStringError (_T("invalid class member kind"));
		return false;
	}

	if (pDecl->GetStorageKind () == EStorage_Static)
		return true;
	
	CClosure* pClosure = pResultValue->CreateClosure ();
	pClosure->GetArgList ()->InsertHead (OpValue);
	return true;
}

bool
COperatorMgr::GetClassFieldMember (
	const CValue& OpValue,
	CStructField* pMember,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_ClassPtr);
	CClassType* pClassType = (CClassType*) pMember->GetParentType ()->GetParentNamespace ();

	pCoord->m_LlvmIndexArray.Insert (0, 0);
	pCoord->m_LlvmIndexArray.Append (pMember->GetLlvmIndex ());

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (
		OpValue, 
		pCoord->m_LlvmIndexArray, 
		pCoord->m_LlvmIndexArray.GetCount (), 
		NULL,
		&PtrValue
		);

	int PtrTypeFlags = pMember->GetPtrTypeFlags ();
	
	if ((OpValue.GetType ()->GetFlags () & EPtrTypeFlag_Const) && pMember->GetStorageKind () != EStorage_Mutable ||
		(PtrTypeFlags & EPtrTypeFlag_ReadOnly) && 
		m_pModule->m_NamespaceMgr.GetAccessKind (pCoord->m_pType) == EAccess_Public)
	{
		PtrTypeFlags |= EPtrTypeFlag_Const;
	}

	CDataPtrType* pResultType = pMember->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin, PtrTypeFlags);

	pResultValue->SetLlvmValue (
		PtrValue.GetLlvmValue (),
		pResultType,
		pMember,
		EValueFlag_NoDataPtrRangeCheck
		);

	CClosure* pClosure = pResultValue->CreateClosure ();
	pClosure->GetArgList ()->InsertHead (OpValue);
	return true;
}

bool
COperatorMgr::GetClassFieldMemberValue (
	const CValue& ObjValue,
	CStructField* pMember,
	CValue* pValue
	)
{
	ASSERT (ObjValue.GetType ()->GetTypeKind () == EType_ClassPtr);

	CValue FieldValue;
	CBaseTypeCoord ClosureCoord;

	return 
		GetClassFieldMember (ObjValue, pMember, &ClosureCoord, &FieldValue) && 
		LoadDataRef (FieldValue, pValue);
}

bool
COperatorMgr::SetClassFieldMemberValue (
	const CValue& ObjValue,
	CStructField* pMember,
	const CValue& Value
	)
{
	ASSERT (ObjValue.GetType ()->GetTypeKind () == EType_ClassPtr);

	CValue FieldValue;
	CBaseTypeCoord ClosureCoord;

	return 
		GetClassFieldMember (ObjValue, pMember, &ClosureCoord, &FieldValue) && 
		BinaryOperator (EBinOp_Assign, FieldValue, Value);
}

bool
COperatorMgr::GetClassVTable (
	const CValue& OpValue,
	CClassType* pClassType,
	CValue* pResultValue
	)
{
	size_t IndexArray [] = 
	{
		0, // class.iface*
		0, // class.iface.hdr*
		0, // class.vtbl**
	};

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (
		OpValue, 
		IndexArray, 
		countof (IndexArray),
		NULL, 
		&PtrValue
		);

	// class.vtbl*

	CDataPtrType* pResultType = pClassType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe);
	m_pModule->m_LlvmBuilder.CreateLoad (PtrValue, pResultType, pResultValue);
	return true;
}

bool
COperatorMgr::GetVirtualMethodMember (
	CFunction* pFunction,
	CClosure* pClosure,
	CValue* pResultValue
	)
{
	ASSERT (pFunction->IsVirtual ());

	if (!pClosure || !pClosure->IsMemberClosure ())
	{
		err::SetFormatStringError (_T("virtual method requires an object pointer"));
		return false;
	}
	
	CValue Value = *pClosure->GetArgList ()->GetHead ();
	CClassType* pClassType = ((CClassPtrType*) Value.GetType ())->GetTargetType ();
	CClassType* pVTableType = pFunction->GetVirtualOriginClassType ();
	size_t VTableIndex = pFunction->GetClassVTableIndex ();
	
	CBaseTypeCoord Coord;
	pClassType->FindBaseType (pVTableType, &Coord);
	VTableIndex += Coord.m_VTableIndex;
	
	// class.vtbl*

	CValue PtrValue;
	GetClassVTable (Value, pClassType, &PtrValue);

	// pfn*

	m_pModule->m_LlvmBuilder.CreateGep2 (
		PtrValue, 
		VTableIndex,
		NULL, 
		&PtrValue
		);

	// pfn

	m_pModule->m_LlvmBuilder.CreateLoad (
		PtrValue, 
		NULL,
		&PtrValue
		);

	pResultValue->SetLlvmValue (
		PtrValue.GetLlvmValue (), 
		pFunction->GetType ()->GetFunctionPtrType (EFunctionPtrType_Thin)
		);

	pResultValue->SetClosure (pClosure);
	return true;
}

bool
COperatorMgr::GetVirtualPropertyMember (
	CProperty* pProperty,
	CClosure* pClosure,
	CValue* pResultValue
	)
{
	ASSERT (pProperty->IsVirtual ());
	
	if (!pClosure || !pClosure->IsMemberClosure ())
	{
		err::SetFormatStringError (_T("virtual method requires an object pointer"));
		return false;
	}

	CValue Value = *pClosure->GetArgList ()->GetHead ();
	CClassType* pClassType = ((CClassPtrType*) Value.GetType ())->GetTargetType ();
	size_t VTableIndex = pProperty->GetParentClassVTableIndex ();

	CBaseTypeCoord Coord;
	pClassType->FindBaseType (pProperty->GetParentClassType (), &Coord);
	VTableIndex += Coord.m_VTableIndex;

	// class.vtbl*

	CValue PtrValue;
	GetClassVTable (Value, pClassType, &PtrValue);

	// property.vtbl*

	m_pModule->m_LlvmBuilder.CreateGep2 (
		PtrValue, 
		VTableIndex,
		NULL,
		&PtrValue
		);

	m_pModule->m_LlvmBuilder.CreateBitCast (
		PtrValue, 
		pProperty->GetType ()->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe),
		&PtrValue
		);

	pResultValue->OverrideType (PtrValue, pProperty->GetType ()->GetPropertyPtrType (EPropertyPtrType_Thin));
	pResultValue->SetClosure (pClosure);
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
