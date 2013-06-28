#include "pch.h"
#include "axl_jnc_ClosureClassType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CClosureClassType::CClosureClassType ()
{
	m_ClassTypeKind = EClassType_Closure;
	m_pThunkMethod = NULL;
}

bool
CClosureClassType::CompileThunkMethod ()
{
/*

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


*/

	return true;
}

/*

bool
COperatorMgr::CreateClosureObject (
	EStorage StorageKind,
	const CValue& OpValue, // function or property thin ptr
	rtl::CArrayT <size_t>* pClosureMap,
	CValue* pResultValue
	)
{
	bool Result;

	CFunctionType* pSrcFunctionType;
	if (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_FunctionPtr)
	{
		pSrcFunctionType = ((CFunctionPtrType*) OpValue.GetType ())->GetTargetType ();
	}
	else
	{
		ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);
		pSrcFunctionType = ((CPropertyPtrType*) OpValue.GetType ())->GetTargetType ()->GetGetterType ();
	}

	CClassType* pClosureType = m_pModule->m_TypeMgr.CreateUnnamedClassType ();

	bool IsIndirect = 
		OpValue.GetValueKind () != EValue_Function &&
		OpValue.GetValueKind () != EValue_Property;
	
	if (IsIndirect)
		pClosureType->CreateField (OpValue.GetType ());

	CClosure* pClosure = OpValue.GetClosure ();

	const rtl::CBoxListT <CValue>* pClosureArgList = NULL;
	
	if (pClosure)
	{
		pClosureArgList = pClosure->GetArgList ();
		rtl::CArrayT <CFunctionArg*> SrcArgArray = pSrcFunctionType->GetArgArray ();

		size_t ClosureArgCount = pClosureArgList->GetCount ();
		size_t SrcArgCount = SrcArgArray.GetCount ();

		pClosureMap->Reserve (ClosureArgCount);

		rtl::CBoxIteratorT <CValue> ClosureArg = pClosureArgList->GetHead ();

		if (ClosureArgCount > SrcArgCount)
		{
			err::SetFormatStringError ("closure is too big for '%s'", pSrcFunctionType->GetTypeString ().cc ());
			return false;
		}

		for (size_t i = 0; ClosureArg; ClosureArg++, i++)
		{
			if (ClosureArg->IsEmpty ())
				continue;

			ASSERT (i < SrcArgCount);
			pClosureMap->Append (i);
			pClosureType->CreateField (SrcArgArray [i]->GetType ());
		}
	}

	Result = pClosureType->EnsureLayout ();
	if (!Result)
		return false;
	
	CValue ClosureValue;
	Result = m_pModule->m_OperatorMgr.NewOperator (StorageKind, pClosureType, NULL, &ClosureValue);
	if (!Result)
		return false;

	// save pf & arguments in the closure
	
	CStructType* pClosureFieldStructType = pClosureType->GetIfaceStructType ();
	ASSERT (pClosureFieldStructType);

	rtl::CIteratorT <CStructField> ClosureMember = pClosureFieldStructType->GetFieldList ().GetHead ();
	if (IsIndirect)
	{
		CValue PtrValue = OpValue;
		PtrValue.SetClosure (NULL); // remove closure

		CValue FieldValue;
		Result = 
			GetClassField (ClosureValue, *ClosureMember, NULL, &FieldValue) &&
			BinaryOperator (EBinOp_Assign, FieldValue, PtrValue);

		if (!Result)
			return false;
		
		ClosureMember++;
	}

	if (pClosure)
	{
		rtl::CBoxIteratorT <CValue> ClosureArg = pClosureArgList->GetHead ();
		for (; ClosureArg; ClosureArg++)
		{
			if (ClosureArg->IsEmpty ())
				continue;

			ASSERT (ClosureMember);

			CValue FieldValue;
			Result = 
				GetClassField (ClosureValue, *ClosureMember, NULL, &FieldValue) &&
				BinaryOperator (EBinOp_Assign, FieldValue, *ClosureArg);

			if (!Result)
				return false;

			ClosureMember++;
		}
	}

	*pResultValue = ClosureValue;
	return true;
}

*/

//.............................................................................

} // namespace jnc {
} // namespace axl {
