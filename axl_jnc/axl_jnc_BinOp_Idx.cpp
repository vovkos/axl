#include "stdafx.h"
#include "axl_jnc_BinOp_Idx.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CBinOp_Idx::Operator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	bool Result;

	CValue OpValue1;
	CValue OpValue2;

	Result = m_pModule->m_OperatorMgr.CastOperator (RawOpValue2, EType_Int_p, &OpValue2);
	if (!Result)
		return false;

	CType* pOpType1 = RawOpValue1.GetType ();
	if (pOpType1->GetTypeKind () == EType_DataRef)
	{
		CType* pBaseType = ((CDataPtrType*) pOpType1)->GetTargetType ();

		if (pBaseType->GetTypeKind () == EType_Array)
			return ArrayIndexOperator (RawOpValue1, (CArrayType*) pBaseType, OpValue2, pResultValue);

		Result = m_pModule->m_OperatorMgr.LoadDataRef (RawOpValue1, &OpValue1);
		if (!Result)
			return false;

		pOpType1 = OpValue1.GetType ();
	}

	EType TypeKind = pOpType1->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_DataPtr:
		return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Add, OpValue1, OpValue2, pResultValue);

	case EType_Array:
		return ArrayIndexOperator (OpValue1, (CArrayType*) pOpType1, OpValue2, pResultValue);

	case EType_PropertyRef:
	case EType_PropertyPtr:
		return PropertyIndexOperator (OpValue1, OpValue2, pResultValue);

	default:
		err::SetFormatStringError (_T("cannot index '%s'"), pOpType1->GetTypeString ());
		return false;
	}
}

bool
CBinOp_Idx::ArrayIndexOperator (
	const CValue& OpValue1,
	CArrayType* pArrayType,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	CType* pElementType = pArrayType->GetElementType ();

	if (OpValue1.GetValueKind () == EValue_Const && OpValue2.GetValueKind ())
	{
		void* p = (char*) OpValue1.GetConstData () + OpValue2.GetSizeT () * pElementType->GetSize ();
		pResultValue->CreateConst (p, pElementType);
		return true;
	}

	llvm::Value* pLlvmValue = OpValue1.GetLlvmValue ();

	EType OpTypeKind1 = OpValue1.GetType ()->GetTypeKind ();

	if (OpTypeKind1 != EType_DataRef)
	{
		ASSERT (OpTypeKind1 == EType_Array);
		err::SetFormatStringError (_T("indexing register-based arrays is not supported yet"));
		return false;
	}

	CDataPtrType* pOpType1 = (CDataPtrType*) OpValue1.GetType ();

	if (pOpType1->GetPtrTypeKind () == EDataPtrType_Unsafe)
	{
		CType* pResultType = pElementType->GetDataPtrType (
			EType_DataRef, 
			EDataPtrType_Unsafe,
			pOpType1->GetFlags ()
			);

		m_pModule->m_LlvmBuilder.CreateGep2 (OpValue1, OpValue2, pResultType, pResultValue);
		return true;
	}

	if (OpValue1.GetValueKind () == EValue_Variable)
	{
		CValue GepValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (OpValue1, OpValue2, NULL, &GepValue);
		pResultValue->SetVariable (
			OpValue1.GetVariable (), 
			GepValue.GetLlvmValue (), 
			pElementType,
			true,
			true
			);
	}
	else
	{
		CValue SizeValue;
		SizeValue.SetConstSizeT (pElementType->GetSize ());
		
		CValue IncrementValue;
		bool Result = m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Mul, OpValue2, SizeValue, &IncrementValue);
		if (!Result)
			return false;

		CType* pResultType = pElementType->GetDataPtrType (EType_DataRef);

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue1, 0, NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateGep (PtrValue, IncrementValue, NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateInsertValue (OpValue1, PtrValue, 0, pResultType, pResultValue);
	}

	return true;
}

bool
CBinOp_Idx::PropertyIndexOperator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	*pResultValue = RawOpValue1;
	
	CClosure* pClosure = pResultValue->GetClosure ();
	if (!pClosure)
		pClosure = pResultValue->CreateClosure ();

	ASSERT (pClosure->GetClosureKind () == EClosure_Property); 
	pClosure->GetArgList ()->InsertTail (RawOpValue2);
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
