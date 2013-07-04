#include "pch.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::GetNamespaceMember (
	CNamespace* pNamespace,
	const char* pName,
	CValue* pResultValue
	)
{
	CModuleItem* pItem = pNamespace->FindItem (pName);
	if (!pItem)
	{
		err::SetFormatStringError ("'%s' is not a member of '%s'", pName, pNamespace->GetQualifiedName ().cc ());
		return false;
	}

	bool Result = true;

	EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Namespace:
		pResultValue->SetNamespace ((CGlobalNamespace*) pItem);
		break;

	case EModuleItem_Typedef:
		pItem = ((CTypedef*) pItem)->GetType ();
		// and fall through

	case EModuleItem_Type:	
		if (!(((CType*) pItem)->GetTypeKindFlags () & ETypeKindFlag_Named))
		{
			err::SetFormatStringError ("'%s' cannot be used as expression", ((CType*) pItem)->GetTypeString ().cc ());
			return false;
		}

		pResultValue->SetNamespace ((CNamedType*) pItem);
		break;

	case EModuleItem_Alias:
		return m_pModule->m_OperatorMgr.EvaluateAlias (((CAlias*) pItem)->GetInitializer (), pResultValue);

	case EModuleItem_Variable:
		pResultValue->SetVariable ((CVariable*) pItem);
		break;

	case EModuleItem_Function:
		pResultValue->SetFunction ((CFunction*) pItem);

		if (((CFunction*) pItem)->IsMember ())
			Result = false;

		break;

	case EModuleItem_Property:
		pResultValue->SetProperty ((CProperty*) pItem);

		if (((CProperty*) pItem)->IsMember ())
			Result = false;

		break;

	case EModuleItem_EnumConst:
		pResultValue->SetConstInt64 (
			((CEnumConst*) pItem)->GetValue (),
			((CEnumConst*) pItem)->GetParentEnumType ()->GetBaseType ()
			);
		break;

	default:
		Result = false;
	};

	if (!Result)
	{
		err::SetFormatStringError ("'%s.%s' cannot be used as expression", pNamespace->GetQualifiedName ().cc (), pName);
		return false;
	}

	return true;
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
		return 
			(OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_Ptr) ?
				((CStructField*) pMember)->GetType ()->GetDataPtrType (
					EType_DataRef, 
					EDataPtrType_Thin, 
					OpValue.GetType ()->GetFlags ()
					) :
				((CStructField*) pMember)->GetType ();
		
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
		return GetField (OpValue, (CStructField*) pMember, &Coord, pResultValue);
	
	case EModuleItem_Function:
		pResultValue->SetFunction ((CFunction*) pMember);
		break;

	case EModuleItem_Property:
		pResultValue->SetProperty ((CProperty*) pMember);
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

	pResultValue->InsertToClosureHead (ThisArgValue);
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

	case EType_ClassPtr:
		PrepareOperandType (&OpValue);
		return GetNamedTypeMemberType (OpValue, ((CClassPtrType*) pType)->GetTargetType (), pName);

	default:
		err::SetFormatStringError ("member operator cannot be applied to '%s'", pType->GetTypeString ().cc ());
		return NULL;
	}
}

bool
COperatorMgr::MemberOperator (
	const CValue& RawOpValue,
	size_t Index,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepDataRef | EOpFlag_KeepClassRef);
	if (!Result)
		return false;

	CType* pType = OpValue.GetType ();
	EType TypeKind = pType->GetTypeKind ();

	CStructField* pField;

	switch (TypeKind)
	{
	case EType_DataRef:
		pType = ((CDataPtrType*) pType)->GetTargetType ();
		TypeKind = pType->GetTypeKind ();
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

	case EType_ClassRef:
		pType = ((CClassPtrType*) pType)->GetTargetType ();
		pField = ((CClassType*) pType)->GetFieldByIndex (Index);		
		return pField && GetClassField (OpValue, pField, NULL, pResultValue);

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
	if (RawOpValue.GetValueKind () == EValue_Namespace)
		return GetNamespaceMember (RawOpValue.GetNamespace (), pName, pResultValue);

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
	case EType_Union:
		return GetNamedTypeMember (OpValue, (CNamedType*) pType, pName, pResultValue);

	case EType_ClassPtr:
		return 
			PrepareOperand (&OpValue) &&
			GetNamedTypeMember (OpValue, ((CClassPtrType*) pType)->GetTargetType (), pName, pResultValue);

	default:
		err::SetFormatStringError ("member operator cannot be applied to '%s'", pType->GetTypeString ().cc ());
		return false;
	}
}

CType*
COperatorMgr::GetWeakenedType (const CValue& OpValue)
{
	err::SetFormatStringError ("weakening is not implemented yet");
	return NULL;
}

bool
COperatorMgr::GetWeakenedType (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pType = GetWeakenedType (OpValue);
	if (!pType)
		return false;

	pResultValue->SetType (pType);
	return true;
}

bool
COperatorMgr::Weaken (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError ("weakening is not implemented yet");
	return false;
}


#if 0

CType*
COperatorMgr::GetStdPropertyFieldType (
	const CValue& RawOpValue,
	EStdPropertyField Field
	)
{
	CValue OpValue;
	PrepareOperandType (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);

	if (OpValue.GetValueKind () != EValue_Property)
	{
		err::SetFormatStringError (
			"'%s' has no field '%s'", 
			OpValue.GetType ()->GetTypeString ().cc (), 
			GetStdPropertyFieldString (Field)
			);
		return NULL;
	}

	CType* pOpType = OpValue.GetType ();
	EType OpTypeKind = pOpType->GetTypeKind ();

	CStructField* pField = NULL;

/*
	switch (OpTypeKind)
	{
	case EType_PropertyPtr:
	case EType_PropertyRef:
		pField = ((CPropertyPtrType*) pOpType)->GetTargetType ()->GetStdPropertyField (Field);
		IsUnsafe = ((CPropertyPtrType*) pOpType)->GetPtrTypeKind () == EPropertyPtrType_Unsafe;
		break;

	default:
	}
*/
	if (!pField)
		return NULL;

	return pField->GetType ()->GetDataPtrType (
		EType_DataRef,
		EDataPtrType_Thin,
		pOpType->GetFlags ()
		);
}

bool
COperatorMgr::GetStdPropertyFieldType (
	const CValue& OpValue,
	EStdPropertyField Field,
	CValue* pResultValue
	)
{
	CType* pResultType = GetStdPropertyFieldType (OpValue, Field);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
	return true;
}

bool
COperatorMgr::GetStdPropertyField (
	const CValue& RawOpValue,
	EStdPropertyField Field, 
	CValue* pResultValue
	)
{
	err::SetFormatStringError ("std property fields got to go");
	return false;
/*
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);
	if (!Result)
		return false;

	CType* pOpType = OpValue.GetType ();
	if (!pOpType->IsPropertyPtrType ())
	{
		err::SetFormatStringError (
			"'%s' has no field '%s'", 
			pOpType->GetTypeString ().cc (), 
			GetStdPropertyFieldString (Field)
			);
		return false;
	}

	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->GetTargetType ();
	CStructField* pField = pPropertyType->GetStdPropertyField (Field);
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

*/
}

#endif

//.............................................................................

} // namespace jnc {
} // namespace axl {
