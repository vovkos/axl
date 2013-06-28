#include "pch.h"
#include "axl_jnc_ThunkFunction.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CThunkFunction::CThunkFunction ()
{
	m_FunctionKind = EFunction_Thunk;
	m_ThunkKind = EThunk_Undefined;
	m_pTargetFunction = NULL;
	m_pTargetFunctionType = NULL;
	m_pClosureType = NULL;
}

bool
CThunkFunction::CompileDirectThunk ()
{
	ASSERT (m_pTargetFunction);

	bool Result;

	rtl::CArrayT <CFunctionArg*> TargetArgArray = m_pTargetFunction->GetType ()->GetArgArray ();
	rtl::CArrayT <CFunctionArg*> ThunkArgArray = m_pType->GetArgArray ();

	size_t TargetArgCount = TargetArgArray.GetCount ();
	size_t ThunkArgCount = ThunkArgArray.GetCount ();

	char Buffer [256];
	rtl::CArrayT <CValue> ArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgArray.SetCount (TargetArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (this);

	llvm::Function::arg_iterator LlvmArg = GetLlvmFunction ()->arg_begin();

	// handle the first thunk argument (it's either NULL or actual interface)

	size_t i = 0;

	if (TargetArgCount == ThunkArgCount)
	{
		CValue IfaceValue (LlvmArg, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr));
		m_pModule->m_LlvmBuilder.CreateBitCast (IfaceValue, TargetArgArray [0]->GetType (), &IfaceValue);
		ArgArray [0] = IfaceValue;
		i++;
	}
	else
	{
		ASSERT (ThunkArgCount == TargetArgCount + 1);
		ThunkArgArray.Remove (0); 
		LlvmArg++;
	}

	for (; i < TargetArgCount; i++, LlvmArg++)
	{
		CValue ArgValue (LlvmArg, ThunkArgArray [i]->GetType ());
		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgArray [i]->GetType ());
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		m_pTargetFunction, 
		m_pTargetFunction->GetType (),
		ArgArray,
		ArgArray.GetCount (),
		&ReturnValue
		);

	if (m_pType->GetReturnType ()->GetTypeKind () != EType_Void)
	{
		Result = m_pModule->m_ControlFlowMgr.Return (ReturnValue);
		if (!Result)
			return false;
	}

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool
CThunkFunction::CompileClosureThunk ()
{
	ASSERT (m_pClosureType);

	bool Result;

	rtl::CArrayT <CFunctionArg*> TargetArgArray = m_pTargetFunctionType->GetArgArray ();
	rtl::CArrayT <CFunctionArg*> ThunkArgArray = m_pType->GetArgArray ();
	rtl::CArrayT <size_t> ClosureMap = m_ClosureMap;

	size_t TargetArgCount = TargetArgArray.GetCount ();
	size_t ThunkArgCount = ThunkArgArray.GetCount ();
	size_t ClosureArgCount = m_ClosureMap.GetCount ();

	char Buffer [256];
	rtl::CArrayT <CValue> ArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgArray.SetCount (TargetArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (this);

	llvm::Function::arg_iterator LlvmArg = GetLlvmFunction ()->arg_begin();

	CValue ClosureValue (LlvmArg++, m_pClosureType->GetClassPtrType ());

	CStructType* pClosureFieldStructType = m_pClosureType->GetIfaceStructType ();
	ASSERT (pClosureFieldStructType);

	rtl::CIteratorT <CStructField> ClosureMember = pClosureFieldStructType->GetFieldList ().GetHead ();

	CValue PfnValue;

	if (m_pTargetFunction)
	{
		PfnValue = m_pTargetFunction;
	}
	else
	{
		Result = m_pModule->m_OperatorMgr.GetClassField (ClosureValue, *ClosureMember, NULL, &PfnValue);
		if (!Result)
			return false;

		ClosureMember++;
	}
	
	size_t i = 0;
	size_t iThunk = 1; // skip ClosureValue arg
	size_t iClosure = 0;

	for (; i < TargetArgCount && iClosure < ClosureArgCount; i++)
	{
		CValue ArgValue;

		if (i == m_ClosureMap [iClosure])
		{		
			Result = m_pModule->m_OperatorMgr.GetClassField (ClosureValue, *ClosureMember, NULL, &ArgValue);

			if (!Result)
				return false;

			ClosureMember++;
			iClosure++;
		}
		else
		{
			ArgValue = CValue (LlvmArg, ThunkArgArray [iThunk]->GetType ());
			LlvmArg++;
			iThunk++;
		}

		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgArray [i]->GetType ());
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	for (; i < TargetArgCount; i++, iThunk, LlvmArg++)
	{
		CValue ArgValue (LlvmArg, ThunkArgArray [iThunk]->GetType ());

		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgArray [i]->GetType ());
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	CValue ReturnValue;

	m_pModule->m_LlvmBuilder.CreateCall (
		PfnValue, 
		m_pTargetFunctionType,
		ArgArray,
		ArgArray.GetCount (),
		&ReturnValue
		);

	if (m_pType->GetReturnType ()->GetTypeKind () != EType_Void)
	{
		Result = m_pModule->m_ControlFlowMgr.Return (ReturnValue);
		if (!Result)
			return false;
	}

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
