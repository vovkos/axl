#include "stdafx.h"
#include "axl_jnc_BinOp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetBinOpKindString (EBinOp OpKind)
{
	switch (OpKind)
	{
	case EBinOp_Add:
		return _T("+");

	case EBinOp_Sub:
		return _T("-");

	case EBinOp_Mul:
		return _T("*");

	case EBinOp_Div:
		return _T("/");

	case EBinOp_Mod:
		return _T("%");

	case EBinOp_Shl:
		return _T("<<");

	case EBinOp_Shr:
		return _T(">>");

	case EBinOp_BwAnd:
		return _T("&");

	case EBinOp_BwXor:
		return _T("^");

	case EBinOp_BwOr:
		return _T("|");

	case EBinOp_Eq:
		return _T("==");

	case EBinOp_Ne:
		return _T("!=");

	case EBinOp_Lt:
		return _T("<");

	case EBinOp_Le:
		return _T("<=");

	case EBinOp_Gt:
		return _T(">");

	case EBinOp_Ge:
		return _T(">=");

	case EBinOp_Assign:
		return _T("=");

	case EBinOp_AddAssign:
		return _T("+=");

	case EBinOp_SubAssign:
		return _T("-=");

	case EBinOp_MulAssign:
		return _T("*=");

	case EBinOp_DivAssign:
		return _T("/=");

	case EBinOp_ModAssign:
		return _T("%=");

	case EBinOp_ShlAssign:
		return _T("<<=");

	case EBinOp_ShrAssign:
		return _T(">>=");

	case EBinOp_AndAssign:
		return _T("&=");

	case EBinOp_XorAssign:
		return _T("^=");

	case EBinOp_OrAssign:
		return _T("|=");

	case EBinOp_AtAssign:
		return _T("@=");

	case EBinOp_ClassAssign:
		return _T(":=");

	case EBinOp_LogAnd:
		return _T("&&");

	case EBinOp_LogOr:
		return _T("||");

	case EBinOp_Idx:
		return _T("[]");

	case EBinOp_At:
		return _T("@");

	default:
		return _T("undefined-binary-operator");
	};
}

//.............................................................................

IBinaryOperator::IBinaryOperator()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_OpKind = EBinOp_Undefined;
	m_OpFlags1 = m_OpFlags2 = 0;
}

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
		CType* pBaseType = ((CDataPtrType*) pOpType1)->GetDataType ();

		if (pBaseType->GetTypeKind () == EType_Array)
			return ArrayIndexOperator (RawOpValue1, (CArrayType*) pBaseType, OpValue2, pResultValue);

		Result = m_pModule->m_OperatorMgr.LoadReferenceOperator (RawOpValue1, &OpValue1);
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

	case EType_Property:
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
		pClosure = pResultValue->CreateClosure (EClosure_Property);

	ASSERT (pClosure->GetClosureKind () == EClosure_Property); 
	pClosure->GetArgList ()->InsertTail (RawOpValue2);
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
