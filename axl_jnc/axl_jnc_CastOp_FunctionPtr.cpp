#include "stdafx.h"
#include "axl_jnc_CastOp_FunctionPtr.h"
#include "axl_jnc_Closure.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

ECast
CCast_FunctionPtr_Base::GetCastKind (
	const CValue& OpValue,
	CType* pType
	)
{
	if (!OpValue.GetType ()) 
	{
		ASSERT (OpValue.GetValueKind () == EValue_Function && OpValue.GetFunction ()->IsOverloaded ());
		return ECast_None; // choosing overload is not yet implemented
	}

	ASSERT (OpValue.GetType ()->IsFunctionPtrType () && pType->GetTypeKind () == EType_FunctionPtr);

	CFunctionPtrType* pSrcPtrType = (CFunctionPtrType*) OpValue.GetType ();
	CFunctionPtrType* pDstPtrType = (CFunctionPtrType*) pType;

	CClosure* pClosure = OpValue.GetClosure ();
	if (pClosure)
		pSrcPtrType = pClosure->GetFunctionClosureType (pSrcPtrType);

	CFunctionType* pSrcFunctionType = pSrcPtrType->GetTargetType ();
	CFunctionType* pDstFunctionType = pDstPtrType->GetTargetType ();
	
	return m_pModule->m_OperatorMgr.GetFunctionCastKind (pSrcFunctionType, pDstFunctionType);
}

//.............................................................................

bool
CCast_FunctionPtr_FromNormal::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsFunctionPtrType () && pType->GetTypeKind () == EType_FunctionPtr);

	CFunctionPtrType* pSrcPtrType = (CFunctionPtrType*) OpValue.GetType ();
	CFunctionType* pSrcFunctionType = pSrcPtrType->GetTargetType ();
	
	CFunctionPtrType* pThinPtrType = pSrcFunctionType->GetAbstractMethodMemberType ()->GetFunctionPtrType (EFunctionPtrType_Thin);

	CValue PfnValue;
	CValue ClosureObjValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, pThinPtrType, &PfnValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &ClosureObjValue);

	ref::CPtrT <CClosure> OldClosure = OpValue.GetClosure ();

	CClosure* pClosure;

	if (OldClosure)
	{
		PfnValue.SetClosure (OldClosure);
		pClosure = OldClosure;
	}
	else
	{
		pClosure = PfnValue.CreateClosure ();
	}

	pClosure->GetArgList ()->InsertHead (ClosureObjValue);

	return m_pModule->m_OperatorMgr.CastOperator (AllocKind, PfnValue, pType, pResultValue);
}

//.............................................................................

bool
CCast_FunctionPtr_Thin2Normal::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsFunctionPtrType () && pType->GetTypeKind () == EType_FunctionPtr);
	
	CFunctionPtrType* pSrcPtrType = (CFunctionPtrType*) OpValue.GetType ();
	CFunctionPtrType* pDstPtrType = (CFunctionPtrType*) pType;

	CFunctionType* pSrcFunctionType = pSrcPtrType->GetTargetType ();	
	CFunctionType* pDstFunctionType = pDstPtrType->GetTargetType ();

	CClosure* pClosure = OpValue.GetClosure ();
	
	CValue SimpleClosureObjValue;
	
	bool IsSimpleClosure = pClosure && pClosure->IsSimpleClosure ();
	if (IsSimpleClosure)
		SimpleClosureObjValue = *pClosure->GetArgList ()->GetHead ();

	// case 1: no conversion required, no closure object needs to be created

	if (IsSimpleClosure && 
		pSrcFunctionType->IsMethodMemberType () &&
		pSrcFunctionType->GetShortType ()->Cmp (pDstFunctionType) == 0)
	{
		return LlvmCast_NoThunkSimpleClosure (
			OpValue,
			SimpleClosureObjValue,
			pSrcFunctionType, 
			pDstPtrType,
			pResultValue
			);
	}

	if (OpValue.GetValueKind () == EValue_Function)
	{
		CFunction* pFunction = OpValue.GetFunction ();

		// case 2.1: conversion is required, but no closure object needs to be created (closure arg is null)
		
		if (!pClosure) 			
			return LlvmCast_DirectThunkNoClosure (
				pFunction,
				pDstPtrType,
				pResultValue
				);

		// case 2.2: same as above, but simple closure is passed as closure arg

		if (IsSimpleClosure && pFunction->GetType ()->IsMethodMemberType ())
			return LlvmCast_DirectThunkSimpleClosure (
				pFunction,
				SimpleClosureObjValue,
				pDstPtrType,
				pResultValue
				);
	}

	// case 3: closure object needs to be created (so conversion is required even if function signatures match)

	return LlvmCast_FullClosure (
		AllocKind,
		OpValue,
		pSrcFunctionType,
		pDstPtrType,
		pResultValue
		); 
}

bool
CCast_FunctionPtr_Thin2Normal::LlvmCast_NoThunkSimpleClosure (
	const CValue& OpValue,
	const CValue& SimpleClosureObjValue,
	CFunctionType* pSrcFunctionType,
	CFunctionPtrType* pDstPtrType,
	CValue* pResultValue
	)
{
	CClassPtrType* pThisArgType = pSrcFunctionType->GetThisArgType ();

	CValue ThisArgValue;
	bool Result = m_pModule->m_OperatorMgr.CastOperator (SimpleClosureObjValue, pThisArgType, &ThisArgValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateClosureFunctionPtr (OpValue, ThisArgValue, pDstPtrType, pResultValue);
	return true;
}

bool
CCast_FunctionPtr_Thin2Normal::LlvmCast_DirectThunkNoClosure (
	CFunction* pFunction,
	CFunctionPtrType* pDstPtrType,
	CValue* pResultValue
	)
{
	CFunction* pThunkFunction = m_pModule->m_FunctionMgr.GetDirectThunkFunction (
		pFunction, 
		((CFunctionPtrType*) pDstPtrType)->GetTargetType (),
		true
		);

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr)->GetZeroValue ();
	m_pModule->m_LlvmBuilder.CreateClosureFunctionPtr (pThunkFunction, NullValue, pDstPtrType, pResultValue);
	return true;
}

bool
CCast_FunctionPtr_Thin2Normal::LlvmCast_DirectThunkSimpleClosure (
	CFunction* pFunction,
	const CValue& SimpleClosureObjValue,
	CFunctionPtrType* pDstPtrType,
	CValue* pResultValue
	)
{
	CClassPtrType* pThisArgType = pFunction->GetType ()->GetThisArgType ();

	CValue ThisArgValue;
	bool Result = m_pModule->m_OperatorMgr.CastOperator (SimpleClosureObjValue, pThisArgType, &ThisArgValue);
	if (!Result)
		return false;

	CFunction* pThunkFunction = m_pModule->m_FunctionMgr.GetDirectThunkFunction (
		pFunction, 
		pThisArgType->GetTargetType ()->GetMethodMemberType (pDstPtrType->GetTargetType ())
		);

	m_pModule->m_LlvmBuilder.CreateClosureFunctionPtr (pThunkFunction, ThisArgValue, pDstPtrType, pResultValue);
	return true;
}

bool
CCast_FunctionPtr_Thin2Normal::LlvmCast_FullClosure (
	EAlloc AllocKind,
	const CValue& OpValue,
	CFunctionType* pSrcFunctionType,
	CFunctionPtrType* pDstPtrType,
	CValue* pResultValue
	)
{
	char Buffer [256];
	rtl::CArrayT <size_t> ClosureMap (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	CValue ClosureObjValue;
	bool Result = m_pModule->m_OperatorMgr.CreateClosureObject (AllocKind, OpValue, &ClosureMap, &ClosureObjValue);
	if (!Result)
		return false;

	ASSERT (ClosureObjValue.GetType ()->GetTypeKind () == EType_ClassPtr);
	CClassType* pClosureType = ((CClassPtrType*) ClosureObjValue.GetType ())->GetTargetType ();

	CFunction* pThunkFunction = m_pModule->m_FunctionMgr.GetClosureThunkFunction (
		pSrcFunctionType,
		OpValue.GetFunction (), 
		pClosureType,
		ClosureMap,
		pDstPtrType->GetTargetType ()
		);

	m_pModule->m_LlvmBuilder.CreateClosureFunctionPtr (pThunkFunction, ClosureObjValue, pDstPtrType, pResultValue);
	return true;
}

//.............................................................................

bool
CCast_FunctionPtr_Weak2Normal::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsFunctionPtrType () && pType->GetTypeKind () == EType_FunctionPtr);

	err::SetFormatStringError ("CCast_FunctionPtr_Weak2Normal::LlvmCast not yet implemented");
	return false;
}

//.............................................................................

bool
CCast_FunctionPtr_Thin2Thin::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsFunctionPtrType () && pType->GetTypeKind () == EType_FunctionPtr);

	CClosure* pClosure = OpValue.GetClosure ();
	if (pClosure)
	{
		err::SetFormatStringError ("cannot create thin function pointer to a closure");
		return false;
	}

	if (OpValue.GetValueKind () != EValue_Function)
	{
		err::SetFormatStringError ("can only create thin pointer thunk to a function, not a function pointer");
		return false;
	}

	CFunction* pThunkFunction = m_pModule->m_FunctionMgr.GetDirectThunkFunction (
		OpValue.GetFunction (), 
		((CFunctionPtrType*) pType)->GetTargetType ()
		);

	pResultValue->SetFunction (pThunkFunction);
	pResultValue->OverrideType (pType);
	return true;
}

//.............................................................................

bool
CCast_FunctionPtr_Thin2Weak::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsFunctionPtrType () && pType->GetTypeKind () == EType_FunctionPtr);

	CFunctionPtrType* pIntermediateType = ((CFunctionPtrType*) pType)->GetTargetType ()->GetFunctionPtrType (EFunctionPtrType_Normal);

	CValue TmpValue;
	return 
		m_pModule->m_OperatorMgr.CastOperator (AllocKind, OpValue, pIntermediateType, pResultValue) &&
		m_pModule->m_OperatorMgr.CastOperator (AllocKind, TmpValue, pType, pResultValue);
}

//.............................................................................

bool
CCast_FunctionPtr_Unsafe2Unsafe::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pType, pResultValue);
	return true;
}

//.............................................................................

CCast_FunctionPtr::CCast_FunctionPtr ()
{
	memset (m_OperatorTable, 0, sizeof (m_OperatorTable));

	m_OperatorTable [EFunctionPtrType_Normal] [EFunctionPtrType_Normal] = &m_FromNormal;
	m_OperatorTable [EFunctionPtrType_Normal] [EFunctionPtrType_Weak]   = &m_FromNormal;
	m_OperatorTable [EFunctionPtrType_Weak] [EFunctionPtrType_Normal]   = &m_Weak2Normal;
	m_OperatorTable [EFunctionPtrType_Thin] [EFunctionPtrType_Normal]   = &m_Thin2Normal;
	m_OperatorTable [EFunctionPtrType_Thin] [EFunctionPtrType_Weak]     = &m_Thin2Weak;
	m_OperatorTable [EFunctionPtrType_Thin] [EFunctionPtrType_Thin]     = &m_Thin2Thin;
	m_OperatorTable [EFunctionPtrType_Thin] [EFunctionPtrType_Unsafe]   = &m_Unsafe2Unsafe;
	m_OperatorTable [EFunctionPtrType_Unsafe] [EFunctionPtrType_Unsafe] = &m_Unsafe2Unsafe;
}

ICastOperator*
CCast_FunctionPtr::GetCastOperator (
	const CValue& OpValue,
	CType* pType
	)
{
	ASSERT (pType->GetTypeKind () == EType_FunctionPtr);	

	CFunctionPtrType* pDstPtrType = (CFunctionPtrType*) pType;
	EFunctionPtrType DstPtrTypeKind = pDstPtrType->GetPtrTypeKind ();

	CType* pSrcType = OpValue.GetType ();
	if (!pSrcType)
	{
		ASSERT (OpValue.GetValueKind () == EValue_Function && OpValue.GetFunction ()->IsOverloaded ());
		ASSERT (DstPtrTypeKind >= 0 && DstPtrTypeKind < 2);

		return m_OperatorTable [EFunctionPtrType_Thin] [DstPtrTypeKind];
	}
	
	if (pSrcType->IsIntegerType ())
	{
		return DstPtrTypeKind == EFunctionPtrType_Unsafe ? 
			m_pModule->m_OperatorMgr.GetStdCastOperator (EStdCast_PtrFromInt) : 
			NULL;
	}
	else if (!pSrcType->IsFunctionPtrType ())
	{
		return NULL;
	}

	CFunctionPtrType* pSrcPtrType = (CFunctionPtrType*) pSrcType;
	EFunctionPtrType SrcPtrTypeKind = pSrcPtrType->GetPtrTypeKind ();

	ASSERT (SrcPtrTypeKind >= 0 && SrcPtrTypeKind < EFunctionPtrType__Count);
	ASSERT (DstPtrTypeKind >= 0 && DstPtrTypeKind < EFunctionPtrType__Count);

	return m_OperatorTable [SrcPtrTypeKind] [DstPtrTypeKind];
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
