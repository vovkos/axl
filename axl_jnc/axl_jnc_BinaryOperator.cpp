#include "stdafx.h"
#include "axl_jnc_BinaryOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetBinOpString (EBinOp OpKind)
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

	case EBinOp_BitwiseAnd:
		return _T("&");

	case EBinOp_BitwiseOr:
		return _T("|");

	case EBinOp_BitwiseXor:
		return _T("^");

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

	case EBinOp_LogicalAnd:
		return _T("&&");

	case EBinOp_LogicalOr:
		return _T("||");

	case EBinOp_Idx:
		return _T("index-operator");

	default:
		return _T("undefined-binary-operator");
	};
}

//.............................................................................

CBinaryOperator::CBinaryOperator ()
{
	m_OpKind = EBinOp_None;
	m_pOperatorLo = NULL;
	m_pOpType1 = NULL;
	m_pOpType2 = NULL;
	m_pReturnType = NULL;
}

bool
CBinaryOperator::Operator (
	const CValue& OpValueOrig1,
	const CValue& OpValueOrig2,
	CValue* pReturnValue
	)
{
	bool Result;

	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CType* pOpType1 = OpValueOrig1.GetType ();
	CType* pOpType2 = OpValueOrig2.GetType ();

	CValue OpValue1;
	CValue OpValue2;

	Result = 
		pModule->m_OperatorMgr.CastOperator (OpValueOrig1, m_pOpType1, &OpValue1) &&
		pModule->m_OperatorMgr.CastOperator (OpValueOrig2, m_pOpType2, &OpValue2);

	if (!Result)
		return false;
	
	EValue OpValueKind1 = OpValue1.GetValueKind ();
	EValue OpValueKind2 = OpValue2.GetValueKind ();
	
	return OpValueKind1 == EValue_Const && OpValueKind2 == EValue_Const ?
		m_pOperatorLo->ConstOperator (OpValue1, OpValue2, pReturnValue) :
		m_pOperatorLo->LlvmOperator (OpValue1, OpValue2, pReturnValue);
}

bool
CBinaryOperator::Operator (
	CValue* pValue,
	const CValue& OpValue2
	)
{
	bool Result;

	CValue ResultValue;

	Result = Operator (*pValue, OpValue2, &ResultValue);
	if (!Result)
		return false;

	*pValue = ResultValue;
	return true;
}

//.............................................................................

void
CBinaryOperatorOverload::Clear ()
{
	m_List.Clear ();
	m_SignatureCache.Clear ();
	m_DirectMap.Clear ();
	m_ImplicitMap.Clear ();
}

CBinaryOperator*
CBinaryOperatorOverload::FindOperator (
	CType* pOpType1,
	CType* pOpType2
	)
{
	rtl::CStringA Signature = pOpType1->GetSignature () + pOpType2->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, CBinaryOperator*> It = m_DirectMap.Find (Signature);
	if (It)
		return It->m_Value;

	It = m_ImplicitMap.Find (Signature);
	if (It)
		return It->m_Value;

	// ok, we need to enumerate all overloads

	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	CBinaryOperator* pBestOperator = NULL;
	ECanMove BestCanMove = ECanMove_None;
	size_t BestCastPrice = -1;

	rtl::CIteratorT <CBinaryOperator> Operator = m_List.GetHead ();
	for (; Operator; Operator++)
	{
		CBinaryOperator* pOperator = *Operator;
		
		CType* pCastOpType1 = pOperator->GetOpType1 ();
		CMoveOperator* pMoveOperator1 = pModule->m_OperatorMgr.FindMoveOperator (pOpType1, pCastOpType1);
		if (!pMoveOperator1)
			continue;

		CType* pCastOpType2 = pOperator->GetOpType2 ();
		CMoveOperator* pMoveOperator2 = pModule->m_OperatorMgr.FindMoveOperator (pOpType2, pCastOpType2);
		if (!pMoveOperator2)
			continue;

		size_t CastPrice = pMoveOperator1->GetPrice () + pMoveOperator2->GetPrice ();
		ECanMove CanMove = max (pMoveOperator1->GetCanMove (), pMoveOperator2->GetCanMove ());

		if (!pBestOperator || BestCanMove < CanMove || BestCastPrice > CastPrice)
		{
			pBestOperator = pOperator;
			BestCanMove = CanMove;
			BestCastPrice = CastPrice;
		}
	}

	m_SignatureCache.InsertTail (Signature);
	It = m_ImplicitMap.Goto (Signature); 
	It->m_Value = pBestOperator;
	return pBestOperator;
}

CBinaryOperator*
CBinaryOperatorOverload::AddOperator (
	CType* pReturnType,
	CType* pOpType1,
	CType* pOpType2,
	IBinaryOperatorLo* pOperatorLo
	)
{
	rtl::CStringA Signature = pOpType1->GetSignature () + pOpType2->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, CBinaryOperator*> It = m_DirectMap.Goto (Signature);
	if (It->m_Value)
	{
		CBinaryOperator* pOperator = It->m_Value;
		pOperator->m_pOperatorLo = pOperatorLo;
		return pOperator;
	}

	CBinaryOperator* pOperator = AXL_MEM_NEW (CBinaryOperator);
	pOperator->m_OpKind = m_OpKind;
	pOperator->m_pReturnType = pReturnType;
	pOperator->m_pOpType1 = pOpType1;
	pOperator->m_pOpType2 = pOpType2;
	pOperator->m_pOperatorLo = pOperatorLo;
	m_List.InsertTail (pOperator);
	m_SignatureCache.InsertTail (Signature);
	It->m_Value = pOperator;
	return pOperator;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
