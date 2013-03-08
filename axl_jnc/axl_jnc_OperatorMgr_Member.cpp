#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::GetFieldMember (
	CStructField* pMember,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	bool Result;

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();

	CNamespace* pParentNamespace = pMember->GetParentType ()->GetParentNamespace ();
	ASSERT (pParentNamespace);

	if (pParentNamespace->GetNamespaceKind () == ENamespace_Property)
	{
		CProperty* pProperty = (CProperty*) pParentNamespace;
		if (pMember->GetStorageKind () == EStorage_Static)
		{
			CVariable* pStaticVariable = pProperty->GetStaticDataVariable ();
			ASSERT (pStaticVariable);

			return GetStructFieldMember (
				pStaticVariable, 
				pMember,
				pCoord,
				pResultValue
				);
		}

		CClassType* pParentClassType = pProperty->GetParentClassType ();
		ASSERT (pParentClassType);

		if (!ThisValue)
		{
			err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag);
			return false;
		}

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
	else if (pParentNamespace->GetNamespaceKind () == ENamespace_AutoEv)
	{
		CAutoEv* pAutoEv = (CAutoEv*) pParentNamespace;
		if (pMember->GetStorageKind () == EStorage_Static)
		{
			CVariable* pStaticVariable = pAutoEv->GetStaticDataVariable ();
			ASSERT (pStaticVariable);

			return GetStructFieldMember (
				pStaticVariable, 
				pMember,
				pCoord,
				pResultValue
				);
		}

		CClassType* pParentClassType = pAutoEv->GetClassType ();
		ASSERT (pParentClassType);

		if (!ThisValue)
		{
			err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag);
			return false;
		}

		CBaseTypeCoord Coord;

		CValue FieldValue;
		Result = GetClassFieldMember (
			ThisValue, 
			pAutoEv->GetClassFieldMember (),
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

		if (!ThisValue)
		{
			err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag);
			return false;
		}

		return GetClassFieldMember (
			ThisValue, 
			pMember,
			pCoord,
			pResultValue
			);	
	}
}

CType*
COperatorMgr::GetMemberOperatorResultType (
	const CValue& RawOpValue,
	const tchar_t* pName
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
			return false;
	}

	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Struct:
		return GetStructMemberType (OpValue, (CStructType*) pType, pName);

	case EType_Union:
		return GetUnionMemberType (OpValue, (CUnionType*) pType, pName);

	case EType_Multicast:
		return GetMulticastMemberType (OpValue, (CMulticastType*) pType, pName);

	case EType_AutoEv:
		return GetAutoEvMemberType (OpValue, (CAutoEvType*) pType, pName);

	case EType_ClassPtr:
		PrepareOperandType (&OpValue);
		return GetClassMemberType (OpValue, ((CClassPtrType*) pType)->GetTargetType (), pName);

	default:
		err::SetFormatStringError (_T("member operator cannot be applied to '%s'"), pType->GetTypeString ());
		return false;
	}
}

bool
COperatorMgr::GetMemberOperatorResultType (
	const CValue& RawOpValue,
	const tchar_t* pName,
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
	else if (
		pType->GetTypeKind () == EType_AutoEvPtr ||
		pType->GetTypeKind () == EType_AutoEvRef
		)
		pType = ((CAutoEvPtrType*) pType)->GetTargetType ();

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

	case EType_AutoEv:
		return GetAutoEvMember (OpValue, (CAutoEvType*) pType, pName, pResultValue);

	case EType_ClassPtr:
		return 
			PrepareOperand (&OpValue) &&
			GetClassMember (OpValue, ((CClassPtrType*) pType)->GetTargetType (), pName, pResultValue);

	default:
		err::SetFormatStringError (_T("member operator cannot be applied to '%s'"), pType->GetTypeString ());
		return false;
	}
}

CType*
COperatorMgr::GetWeakMemberOperatorResultType (
	const CValue& RawOpValue,
	const tchar_t* pName
	)
{
	err::SetFormatStringError (_T("weak member operator is not implemented yet"));
	return NULL;
}

bool
COperatorMgr::GetWeakMemberOperatorResultType (
	const CValue& RawOpValue,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	CType* pResultType = GetWeakMemberOperatorResultType (RawOpValue, pName);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
	return true;
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

CType*
COperatorMgr::GetFieldMemberType (
	const CValue& OpValue,
	CStructField* pMember
	)	
{
	if (OpValue.GetType ()->GetTypeKind () != EType_DataRef)
		return pMember->GetType ();

	CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
	return ((CStructField*) pMember)->GetType ()->GetDataPtrType (
		EType_DataRef,
		pPtrType->GetPtrTypeKind (),
		pPtrType->GetFlags ()
		);
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
COperatorMgr::GetStructMemberType (
	const CValue& OpValue,
	CStructType* pStructType,
	const tchar_t* pName
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
		return GetFieldMemberType (OpValue, (CStructField*) pMember);

	default:
		err::SetFormatStringError (_T("non-field members structs are not supported yet"));
		return false;
	}
}

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
		err::SetFormatStringError (_T("non-field members in structs are not supported yet"));
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
	else if (OpValue.GetValueKind () == EValue_Field)
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
			OpValue.GetField (), 
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

CType*
COperatorMgr::GetUnionMemberType (
	const CValue& OpValue,
	CUnionType* pUnionType,
	const tchar_t* pName
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
		return GetFieldMemberType (OpValue, (CStructField*) pMember);

	default:
		err::SetFormatStringError (_T("non-field members in unions are not supported yet"));
		return false;
	}
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
		err::SetFormatStringError (_T("non-field members in unions are not supported yet"));
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

CType*
COperatorMgr::GetMulticastMemberType (
	const CValue& OpValue,
	CMulticastType* pMulticastType,
	const tchar_t* pName
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
	
	rtl::CStringHashTableMapIteratorT <EMulticastMethod> Method = m_MulticastMethodMap.Find (pName);
	if (!Method)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pMulticastType->GetTypeString ());
		return false;
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
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
	
	rtl::CStringHashTableMapIteratorT <EMulticastMethod> Method = m_MulticastMethodMap.Find (pName);
	if (!Method)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pMulticastType->GetTypeString ());
		return false;
	}

	CFunction* pFunction = pMulticastType->GetMethod (Method->m_Value, pPtrType->GetPtrTypeKind ());
	
	CValue PtrValue;
	return 
		UnaryOperator (EUnOp_Addr, OpValue, &PtrValue) &&
		ClosureOperator (pFunction, PtrValue, pResultValue);
}

CType*
COperatorMgr::GetAutoEvMemberType (
	const CValue& OpValue,
	CAutoEvType* pAutoEvType,
	const tchar_t* pName
	)
{
	if (OpValue.GetValueKind () != EValue_AutoEv)
	{
		err::SetFormatStringError (_T("autoev pointers are not supported yet"));
		return false;
	}

	CAutoEv* pAutoEv = OpValue.GetAutoEv ();
	
	CFunction* pFunction = 
		_tcsicmp (pName, _T("Start")) == 0 ? pAutoEv->GetStarter () :
		_tcsicmp (pName, _T("Stop")) == 0 ? pAutoEv->GetStopper () : NULL;
	
	if (!pFunction)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pAutoEvType->GetTypeString ());
		return false;
	}

	return GetFunctionType (OpValue, pFunction->GetType ());
}

bool
COperatorMgr::GetAutoEvMember (
	const CValue& OpValue,
	CAutoEvType* pAutoEvType,
	const tchar_t* pName,
	CValue* pResultValue
	)
{
	if (OpValue.GetValueKind () != EValue_AutoEv)
	{
		err::SetFormatStringError (_T("autoev pointers are not supported yet"));
		return false;
	}

	CAutoEv* pAutoEv = OpValue.GetAutoEv ();
	
	CFunction* pFunction = 
		_tcsicmp (pName, _T("Start")) == 0 ? pAutoEv->GetStarter () :
		_tcsicmp (pName, _T("Stop")) == 0 ? pAutoEv->GetStopper () : NULL;
	
	if (!pFunction)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pAutoEvType->GetTypeString ());
		return false;
	}

	pResultValue->SetFunction (pFunction);
	pResultValue->SetClosure (OpValue.GetClosure ());
	return true;
}

CType*
COperatorMgr::GetClassMemberType (
	const CValue& OpValue,
	CClassType* pClassType,
	const tchar_t* pName
	)
{
	CBaseTypeCoord Coord;
	CModuleItem* pMember = pClassType->FindItemTraverse (pName, &Coord, ETraverse_NoParentNamespace);	
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' is not a member of '%s'"), pName, pClassType->GetTypeString ());
		return false;
	}

	EModuleItem MemberKind = pMember->GetItemKind ();
	switch (MemberKind)
	{
	case EModuleItem_StructField:
		return GetFieldMemberType (OpValue, (CStructField*) pMember	);
		
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
		err::SetFormatStringError (_T("invalid class member kind"));
		return false;
	}
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

	case EModuleItem_AutoEv:
		pResultValue->SetAutoEv ((CAutoEv*) pMember);
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
	
	if ((OpValue.GetType ()->GetFlags () & EPtrTypeFlag_Const) && !(PtrTypeFlags & EPtrTypeFlag_Mutable) ||
		(PtrTypeFlags & EPtrTypeFlag_ReadOnly) && m_pModule->m_NamespaceMgr.GetAccessKind (pCoord->m_pType) == EAccess_Public)
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
	pClassType->FindBaseTypeTraverse (pVTableType, &Coord);
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
	pClassType->FindBaseTypeTraverse (pProperty->GetParentClassType (), &Coord);
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
COperatorMgr::GetAuPropertyFieldMemberType (
	const CValue& OpValue,
	EAuPropertyField Field
	)
{
	ASSERT (
		OpValue.GetType ()->GetTypeKind () == EType_PropertyPtr ||
		OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);

	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->GetTargetType ();
	CStructField* pMember = pPropertyType->GetAuField (Field);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' has no '%s' field"), pPropertyType->GetTypeString (), GetAuPropertyFieldString (Field));
		return false;
	}

	return GetFieldMemberType (pPtrType->GetAuDataPtrType (EType_DataRef), pMember);
}

bool
COperatorMgr::GetAuPropertyFieldMemberType (
	const CValue& OpValue,
	EAuPropertyField Field,
	CValue* pResultValue
	)
{
	CType* pResultType = GetAuPropertyFieldMemberType (OpValue, Field);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
	return true;
}

bool
COperatorMgr::GetAuPropertyFieldMember (
	const CValue& OpValue,
	EAuPropertyField Field, 
	CValue* pResultValue
	)
{
	ASSERT (
		OpValue.GetType ()->GetTypeKind () == EType_PropertyPtr ||
		OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);

	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->GetTargetType ();
	CStructField* pMember = pPropertyType->GetAuField (Field);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' has no '%s' field"), pPropertyType->GetTypeString (), GetAuPropertyFieldString (Field));
		return false;
	}

	CValue DataValue;

	if (OpValue.GetValueKind () != EValue_Property)
	{
		err::SetFormatStringError (_T("'%s' field for property pointers is not implemented yet"), pMember->GetName ());
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
		CBaseTypeCoord Coord;
		return GetFieldMember (
			pProperty->GetParentClassFieldMember (),
			&Coord, 
			pResultValue
			);
	}

	CBaseTypeCoord Coord;
	Coord.m_LlvmIndexArray = 0; // augmented fields go to the base type of field struct
	return GetStructFieldMember (DataValue, pMember, &Coord, pResultValue);
}

bool
COperatorMgr::GetAutoEvData (
	CAutoEv* pAutoEv,
	CValue* pResultValue
	)
{
	if (pAutoEv->GetStorageKind () == EStorage_Static)
	{
		err::SetFormatStringError (_T("non-static 'onchange' is not implemented yet"));
		return false;
	}

	if (pAutoEv->GetStorageKind () == EStorage_Static)
	{
		CVariable* pStaticVariable = pAutoEv->GetStaticDataVariable ();
		ASSERT (pStaticVariable);

		pResultValue->SetVariable (pStaticVariable);
		return true;
	}

	CBaseTypeCoord Coord;
	return GetFieldMember (
		pAutoEv->GetClassFieldMember (),
		&Coord, 
		pResultValue
		);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
