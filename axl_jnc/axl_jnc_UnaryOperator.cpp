#include "stdafx.h"
#include "axl_jnc_UnaryOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetUnOpString (EUnOp OpKind)
{
	switch (OpKind)
	{
	case EUnOp_Minus:
		return _T("-");

	case EUnOp_BitwiseNot:
		return _T("~");

	case EUnOp_Addr:
		return _T("&");

	case EUnOp_Indir:
		return _T("*");

	case EUnOp_LogicalNot:
		return _T("!");

	case EUnOp_Inc:
		return _T("++");

	case EUnOp_Dec:
		return _T("--");

	default:
		return _T("undefined-unary-operator");
	};
}

//.............................................................................

CUnaryOperator::CUnaryOperator ()
{
	m_OpKind = EUnOp_None;
	m_pOperatorLo = NULL;
	m_pOpType = NULL;
	m_pReturnType = NULL;
}

bool
CUnaryOperator::Operator (
	const CValue& OpValueOrig,
	CValue* pReturnValue
	)
{
	bool Result;

	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pOpType = OpValueOrig.GetType ();
	CValue OpValue;

	Result = pModule->m_OperatorMgr.CastOperator (OpValueOrig, m_pOpType, &OpValue);
	if (!Result)
		return false;
	
	EValue OpValueKind = OpValue.GetValueKind ();
	return OpValueKind == EValue_Const ?
		m_pOperatorLo->ConstOperator (OpValue, pReturnValue) :
		m_pOperatorLo->LlvmOperator (pModule, OpValue, pReturnValue);
}

bool
CUnaryOperator::Operator (CValue* pValue)
{
	bool Result;

	CValue ResultValue;

	Result = Operator (*pValue, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

//.............................................................................

void
CUnaryOperatorOverload::Clear ()
{
	m_List.Clear ();
	m_DirectMap.Clear ();
	m_ImplicitMap.Clear ();
}

CUnaryOperator*
CUnaryOperatorOverload::FindOperator (CType* pOpType)
{
	rtl::CStringA Signature = pOpType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, CUnaryOperator*> It = m_DirectMap.Find (Signature);
	if (It)
		return It->m_Value;

	It = m_ImplicitMap.Find (Signature);
	if (It)
		return It->m_Value;

	// ok, we need to enumerate all overloads

	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CUnaryOperator* pBestOperator = NULL;
	ECast BestCastKind = ECast_Explicit;
	size_t BestCastPrice = -1;

	rtl::CIteratorT <CUnaryOperator> Operator = m_List.GetHead ();
	for (; Operator; Operator++)
	{
		CUnaryOperator* pOperator = *Operator;
		
		CType* pCastOpType = pOperator->GetOpType ();
		CCastOperator* pMoveOperator = pModule->m_OperatorMgr.FindCastOperator (pOpType, pCastOpType);
		if (!pMoveOperator)
			continue;

		ECast CastKind = pMoveOperator->GetCastKind ();
		size_t CastPrice = pMoveOperator->GetPrice ();

		if (!pBestOperator || CastKind < BestCastKind || CastPrice < BestCastPrice)
		{
			pBestOperator = pOperator;
			BestCastKind = CastKind;
			BestCastPrice = CastPrice;
		}
	}

	It = m_ImplicitMap.Goto (Signature); 
	It->m_Value = pBestOperator;
	return pBestOperator;
}

CUnaryOperator*
CUnaryOperatorOverload::AddOperator (
	CType* pReturnType,
	CType* pOpType,
	IUnaryOperatorLo* pOperatorLo
	)
{
	rtl::CStringA Signature = pOpType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, CUnaryOperator*> It = m_DirectMap.Goto (Signature);
	if (It->m_Value)
	{
		CUnaryOperator* pOperator = It->m_Value;		
		pOperator->m_pOperatorLo = pOperatorLo;
		return pOperator;
	}

	CUnaryOperator* pOperator = AXL_MEM_NEW (CUnaryOperator);
	pOperator->m_OpKind = m_OpKind;
	pOperator->m_pReturnType = pReturnType;
	pOperator->m_pOpType = pOpType;
	pOperator->m_pOperatorLo = pOperatorLo;
	m_List.InsertTail (pOperator);
	It->m_Value = pOperator;
	return pOperator;
}

//.............................................................................

llvm::Value*
CUnOp_Minus::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateNeg (pOpValue);
}

llvm::Value*
CUnOp_Minus::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFNeg (pOpValue);
}

//.............................................................................

llvm::Value*
CUnOp_BitwiseNot::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateNot (pOpValue);
}

//.............................................................................

bool
CUnOp_addr::ConstOperator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("cannot apply unary '&' to a constant"));
	return false;
}

bool
CUnOp_addr::LlvmOperator (
	CModule* pModule,
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pType = OpValue.GetType ();
	pType = pType->ModifyType (ETypeModifier_Pointer);

	EValue OpKind = OpValue.GetValueKind ();
	if (OpKind != EValue_Variable)
	{
		err::SetFormatStringError (_T("unary '&' can only be applied to variables"));
		return false;
	}

	CVariable* pVariable = OpValue.GetVariable ();
	llvm::Value* pLlvmValue = pVariable->GetLlvmValue ();
	llvm::Value* pLlvmTypeValue = NULL;

	llvm::Function* pLlvmCreateFatPointer = NULL;
	
	llvm::Value* ArgArray [] =
	{
		pLlvmValue,
		pLlvmValue,
		pLlvmTypeValue,
		pLlvmTypeValue,
	};

	llvm::Value* pLlvmResult = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateCall (
		pLlvmCreateFatPointer, 
		llvm::ArrayRef <llvm::Value*> (ArgArray, 4)
		);

	pResultValue->SetLlvmRegister (pLlvmResult, pType);	
	return false;
}

//.............................................................................

bool
CUnOp_indir::ConstOperator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("cannot apply unary '*' to a constant"));
	return false;
}

bool
CUnOp_indir::LlvmOperator (
	CModule* pModule,
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pType = OpValue.GetType ();
	pType = pType->ModifyType (ETypeModifier_RemovePointer);
	pType = pType->ModifyType (ETypeModifier_Reference);

	err::SetFormatStringError (_T("unary '*' not implemented yet"));
	return false;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
