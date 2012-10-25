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

	case EUnOp_Cast:
		return _T("cast-operator");

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
		m_pOperatorLo->LlvmOperator (OpValue, pReturnValue);
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
	ECanMove BestCanMove = ECanMove_None;
	size_t BestCastPrice = -1;

	rtl::CIteratorT <CUnaryOperator> Operator = m_List.GetHead ();
	for (; Operator; Operator++)
	{
		CUnaryOperator* pOperator = *Operator;
		
		CType* pCastOpType = pOperator->GetOpType ();
		CMoveOperator* pMoveOperator = pModule->m_OperatorMgr.FindMoveOperator (pOpType, pCastOpType);
		if (!pMoveOperator)
			continue;

		size_t CastPrice = pMoveOperator->GetPrice ();
		ECanMove CanMove = pMoveOperator->GetCanMove ();

		if (!pBestOperator || BestCanMove < CanMove || BestCastPrice > CastPrice)
		{
			pBestOperator = pOperator;
			BestCanMove = CanMove;
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

} // namespace axl {
} // namespace jnc {
