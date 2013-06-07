#include "pch.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::GetField (
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	CNamespace* pParentNamespace = pField->GetParentType ()->GetParentNamespace ();
	ENamespace NamespaceKind = pParentNamespace->GetNamespaceKind ();

	CNamedType* pNamedType;

	switch (NamespaceKind)
	{
	case ENamespace_Property:
		return GetPropertyField ((CProperty*) pParentNamespace, pField, pCoord, pResultValue);

	case ENamespace_AutoEv:
		return GetAutoEvField ((CAutoEv*) pParentNamespace, pField, pCoord, pResultValue);

	case ENamespace_Type:
		pNamedType = (CNamedType*) pParentNamespace;

	default:
		pNamedType = pField->GetParentType ();
	}

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	if (!ThisValue)
	{
		err::SetFormatStringError (
			"function '%s' has no 'this' pointer", 
			m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag.cc () // thanks a lot gcc
			);
		return false;
	}

	if (ThisValue.GetType ()->GetTypeKind () == EType_DataPtr)
		ThisValue = m_pModule->m_FunctionMgr.GetThinThisValue ();

	return GetNamedTypeField (ThisValue, pNamedType, pField, pCoord, pResultValue);	
}

bool
COperatorMgr::GetPropertyField (
	CProperty* pProperty,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	if (!pProperty->GetParentType ())
		return GetStructField (
			pProperty->GetStaticDataVariable (), 
			pField,
			pCoord,
			pResultValue
			);

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	if (!ThisValue)
	{
		err::SetFormatStringError (
			"function '%s' has no 'this' pointer", 
			m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag.cc ()
			);
		return false;
	}

	if (ThisValue.GetType ()->GetTypeKind () == EType_DataPtr)
		ThisValue = m_pModule->m_FunctionMgr.GetThinThisValue ();

	CValue FieldValue;	
	return 
		GetNamedTypeField (ThisValue, pProperty->GetParentType (), pProperty->GetParentTypeField (),  NULL, &FieldValue) &&
		GetStructField (FieldValue, pField, pCoord, pResultValue);
}

bool
COperatorMgr::GetAutoEvField (
	CAutoEv* pAutoEv,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	if (!pAutoEv->GetParentClassType ())
		return GetStructField (
			pAutoEv->GetStaticDataVariable (), 
			pField,
			pCoord,
			pResultValue
			);

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	if (!ThisValue)
	{
		err::SetFormatStringError (
			"function '%s' has no 'this' pointer", 
			m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag.cc ()
			);
		return false;
	}

	CValue FieldValue;
	return 
		GetClassField (ThisValue, pAutoEv->GetParentClassField (), NULL, &FieldValue) &&
		GetStructField (FieldValue, pField, pCoord, pResultValue);
}

CType*
COperatorMgr::GetMemberOperatorResultType (
	const CValue& RawOpValue,
	const char* pName
	)
{
	bool Result;

	CValue OpValue;
	PrepareOperandType (RawOpValue, &OpValue, EOpFlag_KeepDataRef);
	
	CType* pType = OpValue.GetType ();
	if (pType->GetTypeKind () == EType_DataRef)
		pType = ((CDataPtrType*) pType)->GetTargetType ();
	else if (
		pType->GetTypeKind () == EType_AutoEvPtr || 
		pType->GetTypeKind () == EType_AutoEvRef)
		pType = ((CAutoEvPtrType*) pType)->GetTargetType ();

	if (pType->GetTypeKind () == EType_DataPtr)
	{
		pType = ((CDataPtrType*) pType)->GetTargetType ();

		Result = GetUnaryOperatorResultType (EUnOp_Indir, &OpValue);
		if (!Result)
			return NULL;
	}

	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Struct:
	case EType_Union:
		return GetNamedTypeMemberType (OpValue, (CNamedType*) pType, pName);

	case EType_Multicast:
		return GetMulticastMemberType (OpValue, (CMulticastType*) pType, pName);

	case EType_AutoEv:
		return GetMemberAutoEvType (OpValue, (CAutoEvType*) pType, pName);

	case EType_ClassPtr:
		PrepareOperandType (&OpValue);
		return GetNamedTypeMemberType (OpValue, ((CClassPtrType*) pType)->GetTargetType (), pName);

	default:
		err::SetFormatStringError ("member operator cannot be applied to '%s'", pType->GetTypeString ().cc ());
		return NULL;
	}
}

CType*
COperatorMgr::GetNamedTypeMemberType (
	const CValue& OpValue,
	CNamedType* pNamedType,
	const char* pName
	)
{
	CModuleItem* pMember = pNamedType->FindItemTraverse (pName, NULL, ETraverse_NoParentNamespace);	
	if (!pMember)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pNamedType->GetTypeString ().cc ());
		return NULL;
	}

	EModuleItem MemberKind = pMember->GetItemKind ();
	switch (MemberKind)
	{
	case EModuleItem_StructField:
		return GetFieldType (OpValue, (CStructField*) pMember);
		
	case EModuleItem_Function:
		return ((CFunction*) pMember)->GetType ()->GetShortType ()->GetFunctionPtrType (
			EType_FunctionRef, 
			EFunctionPtrType_Thin
			);

	case EModuleItem_Property:
		return ((CProperty*) pMember)->GetType ()->GetShortType ()->GetPropertyPtrType (
			EType_PropertyRef, 
			EPropertyPtrType_Thin
			);

	case EModuleItem_AutoEv:
		return ((CAutoEv*) pMember)->GetType ()->GetShortType ()->GetAutoEvPtrType (
			EType_AutoEvRef, 
			EAutoEvPtrType_Thin
			);

	default:
		err::SetFormatStringError ("invalid member kind '%s'", GetModuleItemKindString (MemberKind));
		return NULL;
	}
}

bool
COperatorMgr::GetNamedTypeMember (
	const CValue& OpValue,
	CNamedType* pNamedType,
	const char* pName,
	CValue* pResultValue
	)
{
	CBaseTypeCoord Coord;
	CModuleItem* pMember = pNamedType->FindItemTraverse (pName, &Coord, ETraverse_NoParentNamespace);	
	if (!pMember)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pNamedType->GetTypeString ().cc ());
		return false;
	}

	CModuleItemDecl* pDecl = pMember->GetItemDecl ();
	if (pDecl->GetAccessKind () != EAccess_Public &&
		m_pModule->m_NamespaceMgr.GetAccessKind (Coord.m_pType) == EAccess_Public)
	{
		err::SetFormatStringError ("'%s.%s' is protected", Coord.m_pType->GetQualifiedName ().cc (), pName);
		return false;
	}

	EModuleItem MemberKind = pMember->GetItemKind ();
	switch (MemberKind)
	{
	case EModuleItem_StructField:
		return GetNamedTypeField (OpValue, pNamedType, (CStructField*) pMember, &Coord, pResultValue);
	
	case EModuleItem_Function:
		pResultValue->SetFunction ((CFunction*) pMember);
		break;

	case EModuleItem_Property:
		pResultValue->SetProperty ((CProperty*) pMember);
		break;

	case EModuleItem_AutoEv:
		pResultValue->SetAutoEv ((CAutoEv*) pMember);
		break;

	default:
		err::SetFormatStringError ("invalid class member kind");
		return false;
	}

	if (pDecl->GetStorageKind () == EStorage_Static)
		return true;
	
	CValue ThisArgValue = OpValue;
	if (pNamedType->GetTypeKind () != EType_Class)
	{
		bool Result = UnaryOperator (EUnOp_Addr, &ThisArgValue);
		if (!Result)
			return false;
	}

	CClosure* pClosure = pResultValue->CreateClosure ();
	pClosure->GetArgList ()->InsertHead (ThisArgValue);
	return true;
}

CType*
COperatorMgr::GetFieldType (
	const CValue& OpValue,
	CStructField* pField
	)	
{
	CType* pOpType = OpValue.GetType ();
	EType OpTypeKind = pOpType->GetTypeKind ();

	bool IsUnsafe;

	switch (OpTypeKind)
	{
	case EType_DataPtr:
	case EType_DataRef:
		IsUnsafe = ((CDataPtrType*) pOpType)->GetPtrTypeKind () == EDataPtrType_Unsafe;
		break;

	case EType_ClassPtr:
		IsUnsafe = ((CClassPtrType*) pOpType)->GetPtrTypeKind () == EClassPtrType_Unsafe;
		break;

	case EType_PropertyPtr:
	case EType_PropertyRef:
		IsUnsafe = ((CPropertyPtrType*) pOpType)->GetPtrTypeKind () == EPropertyPtrType_Unsafe;
		break;

	default:
		return pField->GetType ();
	}

	return pField->GetType ()->GetDataPtrType (
		EType_DataRef,
		IsUnsafe ? EDataPtrType_Unsafe : EDataPtrType_Thin,
		pOpType->GetFlags ()
		);
}

bool
COperatorMgr::GetNamedTypeField (
	const CValue& OpValue,
	CNamedType* pNamedType,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	EType TypeKind = pNamedType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Struct:
		return GetStructField (OpValue, pField, pCoord,	pResultValue);

	case EType_Union:
		return GetUnionField (OpValue, pField, pResultValue);

	case EType_Class:
		return GetClassField (OpValue, pField, pCoord, pResultValue);

	default:
		err::SetFormatStringError ("cannot get a field member of '%s'", pNamedType->GetTypeString ().cc ());
		return false;
	}
}

bool
COperatorMgr::GetStructField (
	const CValue& OpValue,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	CBaseTypeCoord Coord;
	if (!pCoord)
		pCoord = &Coord;

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

	if (OpValue.GetType ()->GetTypeKind () == EType_Struct)
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

	ASSERT (OpValue.GetType ()->IsDataPtrType ());
	CDataPtrType* pOpType = (CDataPtrType*) OpValue.GetType ();

	pCoord->m_LlvmIndexArray.Insert (0, 0);

	CDataPtrType* pPtrType;
	int PtrTypeFlags = pOpType->GetFlags () | pField->GetPtrTypeFlags ();
	
	CValue PtrValue;

	EDataPtrType PtrTypeKind = pOpType->GetPtrTypeKind ();
	switch (PtrTypeKind)
	{
	case EDataPtrType_Unsafe:
		pPtrType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Unsafe, PtrTypeFlags);

		m_pModule->m_LlvmBuilder.CreateGep (
			OpValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (), 
			pPtrType, 
			pResultValue
			);
		break;

	case EDataPtrType_Thin:
		pPtrType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin, PtrTypeFlags);

		m_pModule->m_LlvmBuilder.CreateGep (
			OpValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (), 
			NULL, 
			&PtrValue
			);

		if (OpValue.GetValueKind () == EValue_Variable)
			pResultValue->SetThinDataPtr (
				PtrValue.GetLlvmValue (), 
				pPtrType,
				OpValue
				);
		else
			pResultValue->SetThinDataPtr (
				PtrValue.GetLlvmValue (), 
				pPtrType,
				OpValue.GetThinDataPtrValidator ()
				);
		break;

	case EDataPtrType_Normal:
		m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &PtrValue);

		pPtrType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin, PtrTypeFlags);

		m_pModule->m_LlvmBuilder.CreateGep (
			PtrValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (), 
			NULL, 
			&PtrValue
			);

		pResultValue->SetThinDataPtr (
			PtrValue.GetLlvmValue (), 
			pPtrType,
			OpValue
			);

		break;

	default:
		ASSERT (false);
	}

	return true;
}

bool
COperatorMgr::GetUnionField (
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
		err::SetFormatStringError ("union member operator on registers is not implemented yet");
		return false;
	}

	CDataPtrType* pOpType = (CDataPtrType*) OpValue.GetType ();

	CDataPtrType* pPtrType;
	int PtrTypeFlags = pOpType->GetFlags () | pField->GetPtrTypeFlags ();
	
	CValue PtrValue;

	EDataPtrType PtrTypeKind = pOpType->GetPtrTypeKind ();
	switch (PtrTypeKind)
	{
	case EDataPtrType_Unsafe:
		pPtrType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Unsafe, PtrTypeFlags);

		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pPtrType, pResultValue);
		break;

	case EDataPtrType_Thin:
		pPtrType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin, PtrTypeFlags);

		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pPtrType, &PtrValue);

		if (OpValue.GetValueKind () == EValue_Variable)
			pResultValue->SetThinDataPtr (
				PtrValue.GetLlvmValue (), 
				pPtrType,
				OpValue
				);
		else
			pResultValue->SetThinDataPtr (
				PtrValue.GetLlvmValue (), 
				pPtrType,
				OpValue.GetThinDataPtrValidator ()
				);
		break;

	case EDataPtrType_Normal:
		m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &PtrValue);

		pPtrType = pField->GetType ()->GetDataPtrType (EDataPtrType_Unsafe);

		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pPtrType, &PtrValue);

		pPtrType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin, PtrTypeFlags);

		pResultValue->SetThinDataPtr (
			PtrValue.GetLlvmValue (), 
			pPtrType,
			OpValue
			);

		break;

	default:
		ASSERT (false);
	}

	return true;
}

bool
COperatorMgr::GetClassField (
	const CValue& OpValue,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_ClassPtr);
	CClassType* pClassType = (CClassType*) pField->GetParentType ()->GetParentNamespace ();

	CBaseTypeCoord Coord;
	if (!pCoord)
		pCoord = &Coord;

	pCoord->m_LlvmIndexArray.Insert (0, 0);
	pCoord->m_LlvmIndexArray.Append (pField->GetLlvmIndex ());

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (
		OpValue, 
		pCoord->m_LlvmIndexArray, 
		pCoord->m_LlvmIndexArray.GetCount (), 
		NULL,
		&PtrValue
		);

	int PtrTypeFlags = pField->GetPtrTypeFlags ();
	
	if ((OpValue.GetType ()->GetFlags () & EPtrTypeFlag_Const) && !(PtrTypeFlags & EPtrTypeFlag_Mutable) ||
		(PtrTypeFlags & EPtrTypeFlag_ReadOnly) && m_pModule->m_NamespaceMgr.GetAccessKind (pCoord->m_pType) == EAccess_Public)
	{
		PtrTypeFlags |= EPtrTypeFlag_Const;
	}

	CDataPtrType* pPtrType = pField->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin, PtrTypeFlags);

	pResultValue->SetThinDataPtr (
		PtrValue.GetLlvmValue (), 
		pPtrType,
		OpValue,
		PtrValue,
		pField->GetType ()->GetSize ()
		);

	return true;
}

bool
COperatorMgr::GetMemberOperatorResultType (
	const CValue& RawOpValue,
	const char* pName,
	CValue* pResultValue
	)
{
	CType* pResultType = GetMemberOperatorResultType (RawOpValue, pName);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
	return true;
}

bool
COperatorMgr::MemberOperator (
	const CValue& RawOpValue,
	size_t Index,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepDataRef);
	if (!Result)
		return false;

	CType* pType = OpValue.GetType ();
	if (pType->GetTypeKind () != EType_DataRef)
	{
		err::SetFormatStringError ("indexed member operator cannot be applied to '%s'", pType->GetTypeString ().cc ());
		return false;		
	}

	pType = ((CDataPtrType*) pType)->GetTargetType ();

	if (pType->GetTypeKind () == EType_Import)
		pType = ((CImportType*) pType)->GetActualType ();

	CStructField* pField;

	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Array:
		return BinaryOperator (EBinOp_Idx, OpValue, CValue (Index, EType_SizeT), pResultValue);

	case EType_Struct:
		pField = ((CStructType*) pType)->GetFieldByIndex (Index);		
		return pField && GetStructField (OpValue, pField, NULL, pResultValue);

	case EType_Union:
		pField = ((CUnionType*) pType)->GetFieldByIndex (Index);		
		return pField && GetUnionField (OpValue, pField, pResultValue);

	default:
		err::SetFormatStringError ("indexed member operator cannot be applied to '%s'", pType->GetTypeString ().cc ());
		return false;
	}
}

bool
COperatorMgr::MemberOperator (
	const CValue& RawOpValue,
	const char* pName,
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
	else if (
		pType->GetTypeKind () == EType_AutoEvPtr ||
		pType->GetTypeKind () == EType_AutoEvRef
		)
		pType = ((CAutoEvPtrType*) pType)->GetTargetType ();

	if (pType->GetTypeKind () == EType_Import)
		pType = ((CImportType*) pType)->GetActualType ();

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
	case EType_Union:
		return GetNamedTypeMember (OpValue, (CNamedType*) pType, pName, pResultValue);

	case EType_ClassPtr:
		return 
			PrepareOperand (&OpValue) &&
			GetNamedTypeMember (OpValue, ((CClassPtrType*) pType)->GetTargetType (), pName, pResultValue);

	case EType_Multicast:
		return GetMulticastMember (OpValue, (CMulticastType*) pType, pName, pResultValue);

	case EType_AutoEv:
		return GetAutoEvMember (OpValue, (CAutoEvType*) pType, pName, pResultValue);

	default:
		err::SetFormatStringError ("member operator cannot be applied to '%s'", pType->GetTypeString ().cc ());
		return false;
	}
}

CType*
COperatorMgr::GetFunctionType (
	const CValue& OpValue,
	CFunctionType* pFunctionType
	)	
{
	CFunctionPtrType* pFunctionPtrType = pFunctionType->GetFunctionPtrType (
		EType_FunctionRef, 
		EFunctionPtrType_Thin
		);

	CClosure* pClosure = OpValue.GetClosure ();
	if (!pClosure)
		return pFunctionPtrType;

	return GetClosureOperatorResultType (pFunctionPtrType, pClosure->GetArgList ());
}

CType*
COperatorMgr::GetMulticastMemberType (
	const CValue& OpValue,
	CMulticastType* pMulticastType,
	const char* pName
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
	
	rtl::CStringHashTableMapIteratorT <EMulticastMethod> Method = m_MulticastMethodMap.Find (pName);
	if (!Method)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pMulticastType->GetTypeString ().cc ());
		return NULL;
	}

	CFunction* pFunction = pMulticastType->GetMethod (Method->m_Value, pPtrType->GetPtrTypeKind ());
	CFunctionPtrType* pFunctionPtrType = pFunction->GetType ()->GetFunctionPtrType (EFunctionPtrType_Thin);

	CType* pThisType = GetUnaryOperatorResultType (EUnOp_Addr, OpValue);
	if (!pThisType)
		return NULL;

	rtl::CBoxListT <CValue> ArgList;
	ArgList.InsertHead (pThisType);
	return GetClosureOperatorResultType (pFunctionPtrType, &ArgList);
}

bool
COperatorMgr::GetMulticastMember (
	const CValue& OpValue,
	CMulticastType* pMulticastType,
	const char* pName,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
	
	rtl::CStringHashTableMapIteratorT <EMulticastMethod> Method = m_MulticastMethodMap.Find (pName);
	if (!Method)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pMulticastType->GetTypeString ().cc ());
		return false;
	}

	CFunction* pFunction = pMulticastType->GetMethod (Method->m_Value, pPtrType->GetPtrTypeKind ());
	
	CValue PtrValue;
	return 
		UnaryOperator (EUnOp_Addr, OpValue, &PtrValue) &&
		ClosureOperator (pFunction, PtrValue, pResultValue);
}

CType*
COperatorMgr::GetMemberAutoEvType (
	const CValue& OpValue,
	CAutoEvType* pAutoEvType,
	const char* pName
	)
{
	if (OpValue.GetValueKind () != EValue_AutoEv)
	{
		err::SetFormatStringError ("autoev pointers are not supported yet");
		return NULL;
	}

	CAutoEv* pAutoEv = OpValue.GetAutoEv ();
	
	CFunction* pFunction = 
		strcmp (pName, "Start") == 0 ? pAutoEv->GetStarter () :
		strcmp (pName, "Stop") == 0 ? pAutoEv->GetStopper () : NULL;
	
	if (!pFunction)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pAutoEvType->GetTypeString ().cc ());
		return NULL;
	}

	return GetFunctionType (OpValue, pFunction->GetType ());
}

bool
COperatorMgr::GetAutoEvMember (
	const CValue& OpValue,
	CAutoEvType* pAutoEvType,
	const char* pName,
	CValue* pResultValue
	)
{
	if (OpValue.GetValueKind () != EValue_AutoEv)
	{
		err::SetFormatStringError ("autoev pointers are not supported yet");
		return false;
	}

	CAutoEv* pAutoEv = OpValue.GetAutoEv ();
	
	CFunction* pFunction = 
		strcmp (pName, "Start") == 0 ? pAutoEv->GetStarter () :
		strcmp (pName, "Stop") == 0 ? pAutoEv->GetStopper () : NULL;
	
	if (!pFunction)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pAutoEvType->GetTypeString ().cc ());
		return false;
	}

	pResultValue->SetFunction (pFunction);
	pResultValue->SetClosure (OpValue.GetClosure ());
	return true;
}

bool
COperatorMgr::GetClassFieldValue (
	const CValue& ObjValue,
	CStructField* pMember,
	CValue* pValue
	)
{
	ASSERT (ObjValue.GetType ()->GetTypeKind () == EType_ClassPtr);

	CValue FieldValue;
	return 
		GetClassField (ObjValue, pMember, NULL, &FieldValue) && 
		LoadDataRef (FieldValue, pValue);
}

bool
COperatorMgr::SetClassFieldValue (
	const CValue& ObjValue,
	CStructField* pMember,
	const CValue& Value
	)
{
	ASSERT (ObjValue.GetType ()->GetTypeKind () == EType_ClassPtr);

	CValue FieldValue;
	return 
		GetClassField (ObjValue, pMember, NULL, &FieldValue) && 
		BinaryOperator (EBinOp_Assign, FieldValue, Value);
}

bool
COperatorMgr::GetClassVTable (
	const CValue& OpValue,
	CClassType* pClassType,
	CValue* pResultValue
	)
{
	int32_t LlvmIndexArray [] = 
	{
		0, // class.iface*
		0, // class.iface.hdr*
		0, // class.vtbl**
	};

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (
		OpValue, 
		LlvmIndexArray, 
		countof (LlvmIndexArray),
		NULL, 
		&PtrValue
		);

	// class.vtbl*

	CDataPtrType* pResultType = pClassType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe);
	m_pModule->m_LlvmBuilder.CreateLoad (PtrValue, pResultType, pResultValue);
	return true;
}

bool
COperatorMgr::GetVirtualMethod (
	CFunction* pFunction,
	CClosure* pClosure,
	CValue* pResultValue
	)
{
	ASSERT (pFunction->IsVirtual ());

	if (!pClosure || !pClosure->IsMemberClosure ())
	{
		err::SetFormatStringError ("virtual method requires an object pointer");
		return false;
	}
	
	CValue Value = *pClosure->GetArgList ()->GetHead ();
	CClassType* pClassType = ((CClassPtrType*) Value.GetType ())->GetTargetType ();
	CClassType* pVTableType = pFunction->GetVirtualOriginClassType ();
	size_t VTableIndex = pFunction->GetClassVTableIndex ();
	
	CBaseTypeCoord Coord;
	pClassType->FindBaseTypeTraverse (pVTableType, &Coord);
	VTableIndex += Coord.m_VTableIndex;
	
	// class.vtbl*

	CValue PtrValue;
	GetClassVTable (Value, pClassType, &PtrValue);

	// pf*

	m_pModule->m_LlvmBuilder.CreateGep2 (
		PtrValue, 
		VTableIndex,
		NULL, 
		&PtrValue
		);

	// pf

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
COperatorMgr::GetVirtualProperty (
	CProperty* pProperty,
	CClosure* pClosure,
	CValue* pResultValue
	)
{
	ASSERT (pProperty->IsVirtual ());
	
	if (!pClosure || !pClosure->IsMemberClosure ())
	{
		err::SetFormatStringError ("virtual method requires an object pointer");
		return false;
	}

	CValue Value = *pClosure->GetArgList ()->GetHead ();
	CClassType* pClassType = ((CClassPtrType*) Value.GetType ())->GetTargetType ();
	size_t VTableIndex = pProperty->GetParentClassVTableIndex ();

	CBaseTypeCoord Coord;
	pClassType->FindBaseTypeTraverse (pProperty->GetParentType (), &Coord);
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

CType*
COperatorMgr::GetStdFieldType (
	const CValue& RawOpValue,
	EStdField Field
	)
{
	CValue OpValue;
	PrepareOperandType (RawOpValue, &OpValue);

	CType* pOpType = OpValue.GetType ();
	EType OpTypeKind = pOpType->GetTypeKind ();

	CStructField* pField;
	bool IsUnsafe;

	switch (OpTypeKind)
	{
	case EType_ClassPtr:
		pField = ((CClassPtrType*) pOpType)->GetTargetType ()->GetStdField (Field);
		IsUnsafe = ((CClassPtrType*) pOpType)->GetPtrTypeKind () == EClassPtrType_Unsafe;
		break;

	case EType_PropertyPtr:
	case EType_PropertyRef:
		pField = ((CPropertyPtrType*) pOpType)->GetTargetType ()->GetStdField (Field);
		IsUnsafe = ((CPropertyPtrType*) pOpType)->GetPtrTypeKind () == EPropertyPtrType_Unsafe;
		break;

	default:
		err::SetFormatStringError (
			"'%s' has no field '%s'", 
			OpValue.GetType ()->GetTypeString ().cc (), 
			GetStdFieldString (Field)
			);
		return NULL;
	}

	if (!pField)
		return NULL;

	return pField->GetType ()->GetDataPtrType (
		EType_DataRef,
		IsUnsafe ? EDataPtrType_Unsafe : EDataPtrType_Thin,
		pOpType->GetFlags ()
		);
}

bool
COperatorMgr::GetStdFieldType (
	const CValue& OpValue,
	EStdField Field,
	CValue* pResultValue
	)
{
	CType* pResultType = GetStdFieldType (OpValue, Field);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
	return true;
}

bool
COperatorMgr::GetStdField (
	const CValue& RawOpValue,
	EStdField Field, 
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	CType* pOpType = OpValue.GetType ();
	EType OpTypeKind = pOpType->GetTypeKind ();

	switch (OpTypeKind)
	{
	case EType_ClassPtr:
		return GetClassStdField (OpValue, Field, pResultValue);

	case EType_PropertyPtr:
	case EType_PropertyRef:
		return GetPropertyStdField (OpValue, Field, pResultValue);

	default:
		err::SetFormatStringError (
			"'%s' has no field '%s'", 
			OpValue.GetType ()->GetTypeString ().cc (), 
			GetStdFieldString (Field)
			);
		return false;
	}
}

bool
COperatorMgr::GetClassStdField (
	const CValue& OpValue,
	EStdField Field, 
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_ClassPtr);

	CClassPtrType* pPtrType = (CClassPtrType*) OpValue.GetType ();
	CClassType* pClassType = pPtrType->GetTargetType ();
	CStructField* pField = pClassType->GetStdField (Field);
	if (!pField)
		return false;

	return GetClassField (OpValue, pField, NULL, pResultValue);
}

bool
COperatorMgr::GetPropertyStdField (
	const CValue& OpValue,
	EStdField Field, 
	CValue* pResultValue
	)
{
	ASSERT (
		OpValue.GetType ()->GetTypeKind () == EType_PropertyPtr ||
		OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);

	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->GetTargetType ();
	CStructField* pField = pPropertyType->GetStdField (Field);
	if (!pField)
		return false;

	CValue DataValue;

	if (OpValue.GetValueKind () != EValue_Property)
	{
		err::SetFormatStringError ("'%s' field for property pointers is not implemented yet", pField->GetName ().cc ());
		return false;
	}

	CProperty* pProperty = OpValue.GetProperty ();
	if (pProperty->GetStorageKind () == EStorage_Static)
	{
		CVariable* pStaticVariable = pProperty->GetStaticDataVariable ();
		ASSERT (pStaticVariable);
		DataValue = pStaticVariable;
	}
	else
	{
		bool Result = GetField (pProperty->GetParentTypeField (), NULL, &DataValue);
		if (!Result)
			return false;
	}

	CBaseTypeCoord Coord;
	Coord.m_LlvmIndexArray = 0; // augmented fields go to the base type of field struct
	return GetStructField (DataValue, pField, &Coord, pResultValue);
}

bool
COperatorMgr::GetAutoEvData (
	CAutoEv* pAutoEv,
	CValue* pResultValue
	)
{
	CValue DataValue;

	if (pAutoEv->GetStorageKind () == EStorage_Static)
	{
		CVariable* pStaticVariable = pAutoEv->GetStaticDataVariable ();
		ASSERT (pStaticVariable);		
		pResultValue->SetVariable (pStaticVariable);
		return true;
	}
	else
	{
		return GetField (pAutoEv->GetParentClassField (), NULL, pResultValue);
	}
}

bool
COperatorMgr::GetAutoEvField (
	CAutoEv* pAutoEv,
	EAutoEvField Field,
	CValue* pResultValue
	)
{
	CStructField* pMember = pAutoEv->GetField (Field);
	ASSERT (pMember);

	CValue AutoEvDataValue;
	
	return 
		GetAutoEvData (pAutoEv, &AutoEvDataValue) &&
		GetStructField (AutoEvDataValue, pMember, NULL, pResultValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
