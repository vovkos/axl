#include "stdafx.h"
#include "axl_jnc_BinOp_Assign.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................
	
bool
CBinOp_Assign::Operator (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	EType DstTypeKind = OpValue1.GetType ()->GetTypeKind ();
	
	switch (DstTypeKind)
	{
	case EType_DataRef:
		return m_pModule->m_OperatorMgr.StoreDataRef (OpValue2, OpValue1);

	case EType_PropertyRef:
		return m_pModule->m_OperatorMgr.SetPropertyOperator (OpValue2, OpValue1);

	default:
		err::SetFormatStringError (_T("left operand must be l-value"));
		return false;
	}
}

//.............................................................................

bool
CBinOp_OpAssign::Operator (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	ASSERT (m_OpKind >= EBinOp_AddAssign && m_OpKind <= EBinOp_AtAssign);

	EBinOp OpKind = (EBinOp) (m_OpKind - EBinOp__OpAssignDelta);

	CValue RValue;
	return 
		m_pModule->m_OperatorMgr.BinaryOperator (OpKind, OpValue1, OpValue2, &RValue) &&
		m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, OpValue1, RValue);
}
	
//.............................................................................

} // namespace jnc {
} // namespace axl {
