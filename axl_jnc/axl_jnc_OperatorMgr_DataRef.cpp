#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
COperatorMgr::GetThinDataPtrScopeLevel (
	const CValue& Value,
	CValue* pResultValue
	)
{
	EValue ValueKind = Value.GetValueKind ();
	if (ValueKind == EValue_Variable)
	{	
		CalcScopeLevelValue (Value.GetVariable ()->GetScope (), pResultValue);
		return;
	}

	ASSERT (ValueKind == EValue_Field && Value.GetClosure ());

	CValue ClassPtrValue = *Value.GetClosure ()->GetArgList ()->GetHead ();

	CValue ObjPtrValue;
	
	size_t ObjPtrIndexArray [] = 
	{
		0, // iface* 
		0, // iface.hdr*
		1, // TObject**
	};

	m_pModule->m_LlvmBuilder.CreateGep (
		ClassPtrValue, 
		ObjPtrIndexArray, 
		countof (ObjPtrIndexArray), 
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
COperatorMgr::GetThinDataPtrValidator (
	const CValue& Value,
	CValue* pResultValue
	)
{
	EValue ValueKind = Value.GetValueKind ();
	if (ValueKind == EValue_Variable)
	{
		m_pModule->m_LlvmBuilder.CreateDataPtrValidator (Value.GetVariable (), pResultValue);
		return;
	}
	
	ASSERT (ValueKind == EValue_Field);

	CClosure* pClosure = Value.GetClosure ();
	ASSERT (pClosure && pClosure->GetArgList ()->GetCount () == 2);

	rtl::CBoxIteratorT <CValue> Arg = pClosure->GetArgList ()->GetHead ();
	CValue ClassPtrValue = *Arg++;
	CValue RegionBegionValue = *Arg++;

	CValue ObjPtrValue;
	
	size_t ObjPtrIndexArray [] = 
	{
		0, // iface* 
		0, // iface.hdr*
		1, // TObject**
	};

	m_pModule->m_LlvmBuilder.CreateGep (
		ClassPtrValue, 
		ObjPtrIndexArray, 
		countof (ObjPtrIndexArray), 
		NULL, 
		&ObjPtrValue
		);  

	m_pModule->m_LlvmBuilder.CreateLoad (ObjPtrValue, NULL, &ObjPtrValue); // TObject* 

	CValue ScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, NULL, &ScopeLevelValue);  // size_t* m_pScopeLevel
	m_pModule->m_LlvmBuilder.CreateLoad (ScopeLevelValue, NULL, &ScopeLevelValue); // size_t m_ScopeLevel

	m_pModule->m_LlvmBuilder.CreateDataPtrValidator (
		RegionBegionValue, 
		Value.GetField ()->GetType ()->GetSize (),
		ScopeLevelValue,
		pResultValue
		);
}

bool
COperatorMgr::PrepareDataRef (
	const CValue& Value,
	ERuntimeError Error,
	CValue* pPtrValue
	)
{
	ASSERT (Value.GetType ()->GetTypeKind () == EType_DataRef);
	CDataPtrType* pType = (CDataPtrType*) Value.GetType ();
	EDataPtrType PtrTypeKind = pType->GetPtrTypeKind ();

	CValue ValidatorValue;

	switch (PtrTypeKind)
	{
	case EDataPtrType_Unsafe:
		*pPtrValue = Value;
		return true;

	case EDataPtrType_Thin:
		*pPtrValue = Value;
		if (Value.GetFlags () & EValueFlag_NoDataPtrRangeCheck)
			return true;

		GetThinDataPtrValidator (Value, &ValidatorValue);
		break;

	case EDataPtrType_Normal:
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, pPtrValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 1, NULL, &ValidatorValue);
		break;

	default:
		ASSERT (false);
	}

	CheckDataPtrRange (*pPtrValue, pType->GetTargetType ()->GetSize (), ValidatorValue, Error);
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
	Result = PrepareDataRef (OpValue, ERuntimeError_LoadOutOfRange, &PtrValue);
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
		PrepareDataRef (DstValue, ERuntimeError_StoreOutOfRange, &PtrValue);

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
