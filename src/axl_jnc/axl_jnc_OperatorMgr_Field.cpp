#include "pch.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::GetField (
	const CValue& OpValue,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	CNamespace* pNamespace = pField->GetParentNamespace ();
	ASSERT (pNamespace->GetNamespaceKind () == ENamespace_Type);

	CNamedType* pType = (CNamedType*) pNamespace;
	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Struct:
		return GetStructField (OpValue, pField, pCoord,	pResultValue);

	case EType_Union:
		return GetUnionField (OpValue, pField, pResultValue);

	case EType_Class:
		return GetClassField (OpValue, pField, pCoord, pResultValue);

	default:
		err::SetFormatStringError ("cannot get a field '%s' of '%s'", pField->GetName ().cc (), pType->GetTypeString ().cc ());
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
		m_pModule->m_LlvmIrBuilder.CreateExtractValue (
			OpValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (),
			pField->GetType (),
			pResultValue
			);

		return true;
	}

	ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_DataPtr);
	CDataPtrType* pOpType = (CDataPtrType*) OpValue.GetType ();

	pCoord->m_LlvmIndexArray.Insert (0, 0);

	uint_t PtrTypeFlags = pOpType->GetFlags () | pField->GetPtrTypeFlags ();
	if (pField->GetStorageKind () == EStorage_Mutable)
		PtrTypeFlags &= ~EPtrTypeFlag_Const;
	
	CDataPtrType* pPtrType;
	CValue PtrValue;

	EDataPtrType PtrTypeKind = pOpType->GetPtrTypeKind ();
	if (PtrTypeKind == EDataPtrType_Thin)
	{
		pPtrType = pField->GetType ()->GetDataPtrType (
			pField->GetParentNamespace (), 
			EType_DataRef, 
			EDataPtrType_Thin, 
			PtrTypeFlags
			);

		m_pModule->m_LlvmIrBuilder.CreateGep (
			OpValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (), 
			pPtrType, 
			pResultValue
			);

		if (OpValue.GetValueKind () == EValue_Variable)
			pResultValue->SetThinDataPtrValidator (OpValue);
		else
			pResultValue->SetThinDataPtrValidator (OpValue.GetThinDataPtrValidator ());
	}
	else
	{
		m_pModule->m_LlvmIrBuilder.CreateExtractValue (OpValue, 0, NULL, &PtrValue);

		pPtrType = pField->GetType ()->GetDataPtrType (
			pField->GetParentNamespace (), 
			EType_DataRef, 
			EDataPtrType_Thin, 
			PtrTypeFlags
			);

		m_pModule->m_LlvmIrBuilder.CreateGep (
			PtrValue, 
			pCoord->m_LlvmIndexArray, 
			pCoord->m_LlvmIndexArray.GetCount (), 
			pPtrType, 
			pResultValue
			);

		pResultValue->SetThinDataPtrValidator (OpValue);
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

	uint_t PtrTypeFlags = pOpType->GetFlags () | pField->GetPtrTypeFlags ();
	if (pField->GetStorageKind () == EStorage_Mutable)
		PtrTypeFlags &= ~EPtrTypeFlag_Const;
	
	CDataPtrType* pPtrType;
	CValue PtrValue;

	EDataPtrType PtrTypeKind = pOpType->GetPtrTypeKind ();
	if (PtrTypeKind == EDataPtrType_Thin)
	{
		pPtrType = pField->GetType ()->GetDataPtrType (
			pField->GetParentNamespace (), 
			EType_DataRef, 
			EDataPtrType_Thin, 
			PtrTypeFlags
			);

		m_pModule->m_LlvmIrBuilder.CreateBitCast (OpValue, pPtrType, pResultValue);

		if (OpValue.GetValueKind () == EValue_Variable)
			pResultValue->SetThinDataPtrValidator (OpValue);
		else
			pResultValue->SetThinDataPtrValidator (OpValue.GetThinDataPtrValidator ());
	}
	else
	{
		m_pModule->m_LlvmIrBuilder.CreateExtractValue (OpValue, 0, NULL, &PtrValue);

		pPtrType = pField->GetType ()->GetDataPtrType_c ();

		m_pModule->m_LlvmIrBuilder.CreateBitCast (OpValue, pPtrType, &PtrValue);

		pPtrType = pField->GetType ()->GetDataPtrType (
			pField->GetParentNamespace (), 
			EType_DataRef, 
			EDataPtrType_Thin, 
			PtrTypeFlags
			);

		pResultValue->SetThinDataPtr (
			PtrValue.GetLlvmValue (), 
			pPtrType,
			OpValue
			);
	}

	return true;
}

bool
COperatorMgr::GetClassField (
	const CValue& RawOpValue,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_ClassPtr);
	CClassPtrType* pOpType = (CClassPtrType*) OpValue.GetType ();

	CheckClassPtrNull (OpValue);

	CClassType* pClassType = (CClassType*) pField->GetParentNamespace ();

	CBaseTypeCoord Coord;
	if (!pCoord)
		pCoord = &Coord;

	pCoord->m_LlvmIndexArray.Insert (0, 0);
	pCoord->m_LlvmIndexArray.Append (pField->GetLlvmIndex ());

	if (pField->GetType ()->GetTypeKind () == EType_Class)
		pCoord->m_LlvmIndexArray.Append (1);

	CValue PtrValue;
	m_pModule->m_LlvmIrBuilder.CreateGep (
		OpValue, 
		pCoord->m_LlvmIndexArray, 
		pCoord->m_LlvmIndexArray.GetCount (), 
		NULL,
		&PtrValue
		);

	uint_t PtrTypeFlags = pOpType->GetFlags () | pField->GetPtrTypeFlags () | EPtrTypeFlag_Checked;
	if (pField->GetStorageKind () == EStorage_Mutable)
		PtrTypeFlags &= ~EPtrTypeFlag_Const;

	// TODO: handle dual types 
	// (PtrTypeFlags & EPtrTypeFlag_ReadOnly) && m_pModule->m_NamespaceMgr.GetAccessKind (pCoord->m_pType) == EAccess_Public)

	if (pField->GetType ()->GetTypeKind () == EType_Class)
	{
		CClassPtrType* pPtrType = ((CClassType*) pField->GetType ())->GetClassPtrType (
			pField->GetParentNamespace (), 
			EType_ClassRef, 
			EClassPtrType_Normal, 
			PtrTypeFlags
			);

		pResultValue->SetLlvmValue (PtrValue.GetLlvmValue (), pPtrType);
	}
	else
	{
		CDataPtrType* pPtrType = pField->GetType ()->GetDataPtrType (
			pField->GetParentNamespace (), 
			EType_DataRef, 
			EDataPtrType_Thin, 
			PtrTypeFlags
			);

		pResultValue->SetThinDataPtr (
			PtrValue.GetLlvmValue (), 
			pPtrType,
			OpValue,
			PtrValue,
			pField->GetType ()->GetSize ()
			);
	}

	return true;
}

bool
COperatorMgr::GetPropertyField (
	const CValue& OpValue,
	CModuleItem* pMember,
	CValue* pResultValue
	)
{
	if (pMember->GetItemKind () == EModuleItem_Variable)
	{
		pResultValue->SetVariable ((CVariable*) pMember);
		return true;
	}

	ASSERT (pMember->GetItemKind () == EModuleItem_StructField);
	CClosure* pClosure = OpValue.GetClosure ();
	ASSERT (pClosure);

	CValue ParentValue = *pClosure->GetArgList ()->GetHead ();
	return GetField (ParentValue, (CStructField*) pMember, pResultValue);
}

/*

bool
COperatorMgr::GetPropertyField (
	const CValue& OpValue,
	CProperty* pProperty,
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	if (!pProperty->GetParentType ())
		return GetClassField (
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
		GetClassField (ThisValue, pProperty->GetParentTypeField (),  NULL, &FieldValue) &&
		GetClassField (FieldValue, pField, pCoord, pResultValue);
}

bool
COperatorMgr::GetPropertyField (
	CProperty* pProperty,
	CClosure* pClosure, 
	CStructField* pField,
	CBaseTypeCoord* pCoord,
	CValue* pResultValue
	)
{
	if (!pProperty->GetParentType ())
		return GetClassField (
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
		GetClassField (ThisValue, pProperty->GetParentTypeField (),  NULL, &FieldValue) &&
		GetClassField (FieldValue, pField, pCoord, pResultValue);
}

*/

//.............................................................................

} // namespace jnc {
} // namespace axl {
