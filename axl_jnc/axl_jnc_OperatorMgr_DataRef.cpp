#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::LoadReferenceOperator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pType = (CDataPtrType*) OpValue.GetType ();

	EType TypeKind = pType->GetTypeKind ();
	CType* pTargetType = pType->GetDataType ();

	CValue PtrValue;
	bool Result = PrepareReference (OpValue, EPrepareReferenceFlag_Load, &PtrValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateLoad (
		PtrValue, 
		pTargetType, 
		pResultValue, 
		(pType->GetFlags () & EPtrTypeFlag_Volatile) != 0
		);

	return true;
}

bool
COperatorMgr::PrepareReference (
	const CValue& Value,
	int Flags,
	CValue* pPtrValue
	)
{
	ASSERT (Value.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pType = (CDataPtrType*) Value.GetType ();

	if (pType->GetPtrTypeKind () == EDataPtrType_Unsafe)
	{
		// no need to do a range check
		*pPtrValue = Value;
		return true;
	}

	CValue ValidatorValue;
	if (Value.GetValueKind () == EValue_Variable)
	{
		*pPtrValue = Value;

		if (!(Value.GetFlags () & EValueFlag_VariableOffset) || (Flags & EPrepareReferenceFlag_NoRangeCheck))
			return true;

		m_pModule->m_LlvmBuilder.CreateDataPtrValidator (Value.GetVariable (), &ValidatorValue);
	}
	else
	{
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, pPtrValue);

		if (Flags & EPrepareReferenceFlag_NoRangeCheck)
			return true;

		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 1, NULL, &ValidatorValue);
	}

	ERuntimeError Error = (Flags & EPrepareReferenceFlag_Store) ? ERuntimeError_StoreOutOfRange : ERuntimeError_LoadOutOfRange;

	return m_pModule->m_LlvmBuilder.CheckDataPtrRange (
		*pPtrValue, 
		pType->GetDataType ()->GetSize (), 
		ValidatorValue, 
		Error
		);
}

bool
COperatorMgr::StoreReferenceOperator (
	const CValue& RawSrcValue,
	const CValue& RawDstValue,
	bool KeepProperty
	)
{
	bool Result;

	CValue DstValue = RawDstValue;

	ASSERT (DstValue.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pDstType = (CDataPtrType*) DstValue.GetType ();

	CType* pTargetType = pDstType->GetDataType ();

	if (!KeepProperty && pTargetType->GetTypeKind () == EType_PropertyRef)
	{
		return 
			LoadReferenceOperator (&DstValue) &&
			SetPropertyOperator (RawSrcValue, DstValue);
	} 

	if (pTargetType->GetTypeKind () == EType_Event)
		return EventOperator (DstValue, RawSrcValue, EEventOp_SetHandler);

	CValue SrcValue;

	Result = 
		CheckCastKind (RawSrcValue, pTargetType) &&
		CastOperator (RawSrcValue, pTargetType, &SrcValue);

	if (!Result)
		return false;

	int PrepareReferenceFlags = EPrepareReferenceFlag_Store;

	EType TargetTypeKind = pTargetType->GetTypeKind ();
	switch (TargetTypeKind)
	{
	case EType_DataPtr:
		Result = m_pModule->m_LlvmBuilder.CheckDataPtrScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		if (SrcValue.GetValueKind () == EValue_Variable)
			m_pModule->m_LlvmBuilder.CreateDataPtr (
				SrcValue, 
				SrcValue.GetVariable (),
				(CDataPtrType*) pTargetType,
				&SrcValue
				);

		break;

	case EType_ClassPtr:
		Result = m_pModule->m_LlvmBuilder.CheckInterfaceScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		break;

	case EType_FunctionPtr:
		Result = m_pModule->m_LlvmBuilder.CheckFunctionPointerScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		break;

	case EType_PropertyPtr:
		Result = m_pModule->m_LlvmBuilder.CheckPropertyPointerScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		break;

	case EType_BitField:
		Result = MergeBitField (&SrcValue, DstValue);
		if (!Result)
			return false;

		PrepareReferenceFlags |= EPrepareReferenceFlag_NoRangeCheck; // mergebitfield checks ptr range
		break;
	}
	
	CValue PtrValue;
	Result = PrepareReference (DstValue, PrepareReferenceFlags, &PtrValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateStore (
		SrcValue, 
		PtrValue, 
		(pDstType->GetFlags () & EPtrTypeFlag_Volatile) != 0
		);

	return true;
}

bool
COperatorMgr::MergeBitField (
	const CValue& RawSrcValue,
	const CValue& RawDstValue,
	CValue* pResultValue
	)
{
	ASSERT (RawSrcValue.GetType ()->GetTypeKind () == EType_BitField);
	ASSERT (RawDstValue.GetType ()->GetTypeKind () == EType_DataRef);

	CBitFieldType* pSrcType = (CBitFieldType*) RawSrcValue.GetType ();
	CType* pBaseType = pSrcType->GetBaseType ();
	size_t BitOffset = pSrcType->GetBitOffset ();
	size_t BitCount = pSrcType->GetBitCount ();

	CValue MaskValue;

	if (pSrcType->GetSize () <= 4)
	{
		uint32_t Mask = ~((((uint32_t) 1 << BitCount) - 1) << BitOffset);	
		MaskValue.SetConstInt32 (Mask, EType_Int32_u);
	}
	else
	{
		uint64_t Mask = ~((((uint64_t) 1 << BitCount) - 1) << BitOffset);	
		MaskValue.SetConstInt64 (Mask, EType_Int64_u);
	}

	CValue DstValue;
	bool Result = LoadReferenceOperator (RawDstValue, &DstValue);
	if (!Result)
		return false;

	DstValue.OverrideType (pBaseType);

	CValue SrcValue (RawSrcValue, pBaseType);
	return 
		BinaryOperator (EBinOp_BwAnd, &DstValue, MaskValue) &&
		BinaryOperator (EBinOp_BwOr, SrcValue, DstValue, pResultValue);
}

bool
COperatorMgr::MergeBitField (
	CValue* pValue,
	const CValue& DstValue
	)
{
	CValue ResultValue;

	bool Result = MergeBitField (*pValue, DstValue, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
