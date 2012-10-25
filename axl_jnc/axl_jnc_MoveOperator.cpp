#include "stdafx.h"
#include "axl_jnc_MoveOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CMoveOperator::CMoveOperator ()
{
	m_Price = 1;
	m_CanMove = ECanMove_Implicit;
	m_pOperatorLo = NULL;
	m_pDstType = NULL;
	m_pSrcType = NULL;
}

bool
CMoveOperator::Move (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	EValue SrcValueKind = SrcValue.GetValueKind ();
	EValue DstValueKind = DstValue.GetValueKind ();

	return DstValueKind == EValue_Const && SrcValueKind == EValue_Const ? 
		m_pOperatorLo->ConstMove (SrcValue, DstValue) :
		m_pOperatorLo->LlvmMove (SrcValue, DstValue);
}

//.............................................................................

bool
CSuperMove::ConstMove (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CValue TmpValue;
	return 
		TmpValue.CreateConst (m_pFirst->GetDstType ()) && 
		m_pFirst->Move (SrcValue, TmpValue) &&
		m_pSecond->Move (TmpValue, DstValue);
}

bool
CSuperMove::LlvmMove (
	const CValue& DstValue,
	const CValue& SrcValue
	)
{
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
