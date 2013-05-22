#include "pch.h"
#include "axl_jnc_BinOp_At.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CType*
CBinOp_At::GetResultType (
	const CValue& OpValue1,
	const CValue& OpValue2
	)
{
	return OpValue1.GetType ();
}

bool
CBinOp_At::Operator (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	bool Result;

	CValue SchedulerValue;
	Result = m_pModule->m_OperatorMgr.CastOperator (
		OpValue2, 
		m_pModule->m_TypeMgr.GetStdType (EStdType_ISchedulerPtr), 
		&SchedulerValue
		);

	if (!Result)
		return false;
	
	EType OpType1 = OpValue1.GetType ()->GetTypeKind ();
	if (OpType1 != EType_FunctionPtr && OpType1 != EType_FunctionRef)
	{
		SetOperatorError (OpValue1, OpValue2);
		return false;
	}

	CFunctionPtrType* pTargetPtrType = (CFunctionPtrType*) OpValue1.GetClosureAwareType ();
	CFunction* pTargetFunction = OpValue1.GetValueKind () == EValue_Function ? OpValue1.GetFunction () : NULL;

	CFunction* pLauncher = m_pModule->m_FunctionMgr.GetScheduleLauncherFunction (
		pTargetPtrType,
		pTargetFunction,
		(CClassPtrType*) SchedulerValue.GetType ()
		);

	if (!pLauncher)
		return false;
		
	pResultValue->SetFunction (pLauncher);
	CClosure* pClosure = pResultValue->CreateClosure ();

	if (!pTargetFunction)
		pClosure->GetArgList ()->InsertTail (OpValue1);

	pClosure->GetArgList ()->InsertTail (SchedulerValue);

	if (OpValue1.HasClosure ())
		pClosure->Append (*OpValue1.GetClosure ()->GetArgList ());

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
