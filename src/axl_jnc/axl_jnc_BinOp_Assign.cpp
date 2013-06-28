#include "pch.h"
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
	*pResultValue = OpValue1;

	EType DstTypeKind = OpValue1.GetType ()->GetTypeKind ();
	
	switch (DstTypeKind)
	{
	case EType_DataRef:
		return m_pModule->m_OperatorMgr.StoreDataRef (OpValue1, OpValue2);

	case EType_ClassRef:
		err::SetFormatStringError ("assigning to class not yet implemented");
		return false;

	case EType_PropertyRef:
		return m_pModule->m_OperatorMgr.SetProperty (OpValue1, OpValue2);

	default:
		err::SetFormatStringError ("left operand must be l-value");
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

	*pResultValue = OpValue1;

	EBinOp OpKind = (EBinOp) (m_OpKind - EBinOp__OpAssignDelta);

	CValue RValue;
	return 
		m_pModule->m_OperatorMgr.BinaryOperator (OpKind, OpValue1, OpValue2, &RValue) &&
		m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, OpValue1, RValue);
}
	
//.............................................................................

} // namespace jnc {
} // namespace axl {


/*		CDataPtrType* pPtrType = (CDataPtrType*) OpValue1.GetType ();

		if (pPtrType->GetTargetType ()->GetTypeKind () == EType_Multicast)
		{
			CMulticastClassType* pMulticastType = (CMulticastClassType*) pPtrType->GetTargetType ();
			CFunction* pMethod = pMulticastType->GetMethod (EMulticastMethod_Set, pPtrType->GetPtrTypeKind ());
			
			CValue MulticastPtrValue;
			return 
				m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, OpValue1, &MulticastPtrValue) &&
				m_pModule->m_OperatorMgr.CallOperator2 (pMethod, MulticastPtrValue, OpValue2);
		}

//.............................................................................

	if ((m_OpKind == EBinOp_AddAssign || m_OpKind == EBinOp_SubAssign) && 
		OpValue1.GetType ()->GetTypeKind () == EType_DataRef)
	{
		CDataPtrType* pPtrType = (CDataPtrType*) OpValue1.GetType ();
		if (pPtrType->GetTargetType ()->GetTypeKind () == EType_Multicast)
		{
			CMulticastClassType* pMulticastType = (CMulticastClassType*) pPtrType->GetTargetType ();
		
			CFunction* pMethod = pMulticastType->GetMethod (
				m_OpKind == EBinOp_AddAssign ? EMulticastMethod_Add : EMulticastMethod_Remove,
				pPtrType->GetPtrTypeKind ()
				);

			CValue MulticastPtrValue;
			return 
				m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, OpValue1, &MulticastPtrValue) &&
				m_pModule->m_OperatorMgr.CallOperator2 (pMethod, MulticastPtrValue, OpValue2, pResultValue);
		}
	}
*/
