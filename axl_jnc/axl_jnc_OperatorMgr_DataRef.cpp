#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
COperatorMgr::GetClassPtrScopeLevel (
	const CValue& Value,
	CValue* pResultValue
	)
{
	ASSERT (Value.GetType ()->GetTypeKind () == EType_ClassPtr);

	CValue ObjPtrValue;
	
	static size_t LlvmIndexArray [] = 
	{
		0, // iface* 
		0, // iface.hdr*
		1, // TObject**
	};

	m_pModule->m_LlvmBuilder.CreateGep (
		Value, 
		LlvmIndexArray, 
		countof (LlvmIndexArray), 
		NULL, 
		&ObjPtrValue
		);  

	m_pModule->m_LlvmBuilder.CreateLoad (ObjPtrValue, NULL, &ObjPtrValue); // TObject* 

	CValue ScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, NULL, &ScopeLevelValue);  // size_t* m_pScopeLevel
	m_pModule->m_LlvmBuilder.CreateLoad (
		ScopeLevelValue, 
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), 
		pResultValue
		); 
}

void
COperatorMgr::GetThinDataPtrScopeLevel (
	const CValue& Value,
	CValue* pResultValue
	)
{
	ASSERT (Value.GetType ()->IsDataPtrType ());

	EValue ValueKind = Value.GetValueKind ();
	if (ValueKind == EValue_Variable)
	{	
		CalcScopeLevelValue (Value.GetVariable ()->GetScope (), pResultValue);
		return;
	}

	ASSERT (Value.GetThinDataPtrValidator ());
	CValue ScopeValidatorValue = Value.GetThinDataPtrValidator ()->GetScopeValidator ();

	if (ScopeValidatorValue.GetValueKind () == EValue_Variable)
	{
		CalcScopeLevelValue (ScopeValidatorValue.GetVariable ()->GetScope (), pResultValue);
		return;
	}
	
	EType TypeKind = ScopeValidatorValue.GetType ()->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_SizeT:
		*pResultValue = ScopeValidatorValue;
		break;

	case EType_DataPtr:
		ASSERT (((CDataPtrType*) ScopeValidatorValue.GetType ())->GetPtrTypeKind () == EDataPtrType_Normal);
		m_pModule->m_LlvmBuilder.CreateExtractValue (ScopeValidatorValue, 3, CGetType (m_pModule, EType_SizeT), pResultValue);
		break;

	case EType_ClassPtr:
		GetClassPtrScopeLevel (Value, pResultValue);
		break;

	default:
		ASSERT (false);
	}
}

void
COperatorMgr::GetThinDataPtrRange (
	const CValue& Value,
	CValue* pRangeBeginValue,
	CValue* pRangeEndValue
	)
{
	ASSERT (Value.GetType ()->IsDataPtrType ());

	CType* pBytePtrType = CGetType (m_pModule, EStdType_BytePtr);

	EValue ValueKind = Value.GetValueKind ();
	if (ValueKind == EValue_Variable)
	{	
		size_t Size =  Value.GetVariable ()->GetType ()->GetSize ();
		m_pModule->m_LlvmBuilder.CreateBitCast (Value, pBytePtrType, pRangeBeginValue);
		m_pModule->m_LlvmBuilder.CreateGep (*pRangeBeginValue, Size, pBytePtrType, pRangeEndValue);
		return;
	}

	CThinDataPtrValidator* pValidator = Value.GetThinDataPtrValidator ();
	ASSERT (pValidator);

	if (pValidator->GetValidatorKind () == EThinDataPtrValidator_Complex)
	{
		m_pModule->m_LlvmBuilder.CreateBitCast (pValidator->GetRangeBegin (), pBytePtrType, pRangeBeginValue);
		m_pModule->m_LlvmBuilder.CreateGep (*pRangeBeginValue, pValidator->GetSize (), pBytePtrType, pRangeEndValue);
		return;
	}

	CValue ValidatorValue = pValidator->GetScopeValidator ();

	if (ValidatorValue.GetValueKind () == EValue_Variable)
	{
		size_t Size = ValidatorValue.GetVariable ()->GetType ()->GetSize ();
		m_pModule->m_LlvmBuilder.CreateBitCast (ValidatorValue, pBytePtrType, pRangeBeginValue);
		m_pModule->m_LlvmBuilder.CreateGep (*pRangeBeginValue, Size, pBytePtrType, pRangeEndValue);
		return;
	}

	ASSERT (
		ValidatorValue.GetType ()->GetTypeKind () == EType_DataPtr &&
		((CDataPtrType*) ValidatorValue.GetType ())->GetPtrTypeKind () == EDataPtrType_Normal);

	m_pModule->m_LlvmBuilder.CreateExtractValue (ValidatorValue, 1, pBytePtrType, pRangeBeginValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (ValidatorValue, 2, pBytePtrType, pRangeEndValue);		
}

bool
COperatorMgr::PrepareDataPtr (
	const CValue& Value,
	ERuntimeError Error,
	CValue* pResultValue
	)
{
	ASSERT (Value.GetType ()->GetTypeKind () == EType_DataPtr || Value.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pType = (CDataPtrType*) Value.GetType ();
	EDataPtrType PtrTypeKind = pType->GetPtrTypeKind ();

	CDataPtrType* pResultType = pType->GetTargetType ()->GetDataPtrType (EDataPtrType_Unsafe);

	CValue PtrValue;
	CValue RangeBeginValue;	
	CValue RangeEndValue;	

	switch (PtrTypeKind)
	{
	case EDataPtrType_Unsafe:
		pResultValue->OverrideType (Value, pResultType);
		return true;

	case EDataPtrType_Thin:
		if (!(pType->GetFlags () & EPtrTypeFlag_Nullable))
		{
			pResultValue->OverrideType (Value, pResultType);
			return true;
		}

		PtrValue.OverrideType (Value, pResultType);
		GetThinDataPtrRange (Value, &RangeBeginValue, &RangeEndValue);
		break;

	case EDataPtrType_Normal:
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, pResultType, &PtrValue);

		if (!(pType->GetFlags () & EPtrTypeFlag_Nullable))
		{
			*pResultValue = PtrValue;
			return true;
		}

		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 1, NULL, &RangeBeginValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 2, NULL, &RangeEndValue);
		break;

	default:
		ASSERT (false);
	}

	CheckDataPtrRange (PtrValue, pType->GetTargetType ()->GetSize (), RangeBeginValue, RangeEndValue, Error);
	*pResultValue = PtrValue;
	return true;
}

bool
COperatorMgr::LoadDataRef (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_DataRef);
	
	bool Result;
	
	CDataPtrType* pType = (CDataPtrType*) OpValue.GetType ();

	CType* pTargetType = pType->GetTargetType ();

	CValue PtrValue;
	Result = PrepareDataPtr (OpValue, ERuntimeError_LoadOutOfRange, &PtrValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateLoad (
		PtrValue, 
		pTargetType, 
		pResultValue, 
		(pType->GetFlags () & EPtrTypeFlag_Volatile) != 0
		);

	if (pTargetType->GetTypeKind () == EType_BitField)
	{
		Result = ExtractBitField (
			*pResultValue, 
			(CBitFieldType*) pTargetType,
			pResultValue
			);

		if (!Result)
			return false;
	}

	return true;
}

bool
COperatorMgr::StoreDataRef (
	const CValue& DstValue,
	const CValue& RawSrcValue
	)
{
	ASSERT (DstValue.GetType ()->GetTypeKind () == EType_DataRef);

	bool Result;

	CDataPtrType* pDstType = (CDataPtrType*) DstValue.GetType ();
	
	if (pDstType->GetFlags () & EPtrTypeFlag_Const)
	{
		err::SetFormatStringError (_T("cannot store into const location"));
		return false;
	}

	CType* pTargetType = pDstType->GetTargetType ();
	EType TargetTypeKind = pTargetType->GetTypeKind ();

	CType* pCastType = (TargetTypeKind == EType_BitField) ? 
		((CBitFieldType*) pTargetType)->GetBaseType () : 
		pTargetType;

	CValue PtrValue;
	CValue SrcValue;
	CValue BfShadowValue;

	Result = 
		CheckCastKind (RawSrcValue, pCastType) &&
		CastOperator (RawSrcValue, pCastType, &SrcValue) &&
		PrepareDataPtr (DstValue, ERuntimeError_StoreOutOfRange, &PtrValue);

	if (!Result)
		return false;

	switch (TargetTypeKind)
	{
	case EType_DataPtr:
		Result = CheckDataPtrScopeLevel (SrcValue, DstValue);
		if (!Result)
			return false;

		break;

	case EType_ClassPtr:
		CheckClassPtrScopeLevel (SrcValue, DstValue);
		break;

	case EType_FunctionPtr:
		CheckFunctionPtrScopeLevel (SrcValue, DstValue);
		break;

	case EType_PropertyPtr:
		CheckPropertyPtrScopeLevel (SrcValue, DstValue);
		break;

	case EType_BitField:
		m_pModule->m_LlvmBuilder.CreateLoad (
			PtrValue, 
			pCastType,
			&BfShadowValue,
			(pDstType->GetFlags () & EPtrTypeFlag_Volatile) != 0
			);

		Result = MergeBitField (
			SrcValue,
			BfShadowValue, 
			(CBitFieldType*) pTargetType,
			&SrcValue
			);

		if (!Result)
			return false;
	}

	m_pModule->m_LlvmBuilder.CreateStore (
		SrcValue, 
		PtrValue, 
		(pDstType->GetFlags () & EPtrTypeFlag_Volatile) != 0
		);

	return true;
}

bool
COperatorMgr::ExtractBitField (
	const CValue& RawValue,
	CBitFieldType* pBitFieldType,
	CValue* pResultValue
	)
{
	bool Result;

	CType* pBaseType = pBitFieldType->GetBaseType ();
	size_t BitOffset = pBitFieldType->GetBitOffset ();
	size_t BitCount = pBitFieldType->GetBitCount ();

	EType TypeKind = pBaseType->GetSize () <= 4 ? EType_Int32_u : EType_Int64_u;
	int64_t Mask = ((int64_t) 1 << BitCount) - 1;

	CValue Value (RawValue, pBaseType);
	CValue MaskValue (Mask, TypeKind);
	CValue OffsetValue (BitOffset, TypeKind);

	Result = 
		BinaryOperator (EBinOp_Shr, &Value, OffsetValue) &&
		BinaryOperator (EBinOp_BwAnd, &Value, MaskValue);

	if (!Result)
		return false;

	if (pBaseType->IsSignedIntegerType ()) // extend with sign bit
	{
		int64_t SignBit = (int64_t) 1 << (BitCount - 1);

		CValue SignBitValue (SignBit, TypeKind);
		CValue OneValue (1, TypeKind);

		CValue SignExtValue;
		Result = 
			BinaryOperator (EBinOp_BwAnd, &SignBitValue, Value) &&
			BinaryOperator (EBinOp_Sub, SignBitValue, OneValue, &SignExtValue) &&
			UnaryOperator (EUnOp_BwNot, &SignExtValue) &&
			BinaryOperator (EBinOp_BwOr, &Value, SignExtValue);

		if (!Result)
			return false;
	}

	return CastOperator (Value, pBaseType, pResultValue);
}
	
bool
COperatorMgr::MergeBitField (
	const CValue& RawValue,
	const CValue& RawShadowValue,
	CBitFieldType* pBitFieldType,
	CValue* pResultValue
	)
{
	bool Result;

	CType* pBaseType = pBitFieldType->GetBaseType ();
	size_t BitOffset = pBitFieldType->GetBitOffset ();
	size_t BitCount = pBitFieldType->GetBitCount ();

	EType TypeKind = pBaseType->GetSize () <= 4 ? EType_Int32_u : EType_Int64_u;
	int64_t Mask = (((int64_t) 1 << BitCount) - 1) << BitOffset;

	CValue Value (RawValue, pBaseType);
	CValue ShadowValue (RawShadowValue, pBaseType);
	CValue MaskValue (Mask, TypeKind);
	CValue OffsetValue (BitOffset, TypeKind);

	Result = 
		BinaryOperator (EBinOp_Shl, &Value, OffsetValue) &&
		BinaryOperator (EBinOp_BwAnd, Value, MaskValue, pResultValue);

	if (!Result)
		return false;

	Mask = ~((((uint64_t) 1 << BitCount) - 1) << BitOffset);	
	MaskValue.SetConstInt64 (Mask, TypeKind);

	return 
		BinaryOperator (EBinOp_BwAnd, &ShadowValue, MaskValue) &&
		BinaryOperator (EBinOp_BwOr, &Value, ShadowValue) &&
		CastOperator (Value, pBaseType, pResultValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
