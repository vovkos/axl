#include "stdafx.h"
#include "axl_jnc_CastOp_PropertyPtr.h"
#include "axl_jnc_Closure.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

ECast
CCast_PropertyPtr_Base::GetCastKind (
	const CValue& OpValue,
	CType* pType
	)
{
	ASSERT (OpValue.GetType ()->IsPropertyPtrType () && pType->GetTypeKind () == EType_PropertyPtr);

	CPropertyPtrType* pSrcPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyPtrType* pDstPtrType = (CPropertyPtrType*) pType;

	CClosure* pClosure = OpValue.GetClosure ();
	if (pClosure)
		pSrcPtrType = pClosure->GetPropertyClosureType (pSrcPtrType);

	CPropertyType* pSrcPropertyType = pSrcPtrType->GetTargetType ();
	CPropertyType* pDstPropertyType = pDstPtrType->GetTargetType ();
	
	return m_pModule->m_OperatorMgr.GetPropertyCastKind (pSrcPropertyType, pDstPropertyType);
}

//.............................................................................

bool
CCast_PropertyPtr_FromNormal::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsPropertyPtrType () && pType->GetTypeKind () == EType_PropertyPtr);

	CPropertyPtrType* pSrcPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pSrcPropertyType = pSrcPtrType->GetTargetType ();
	
	CPropertyPtrType* pThinPtrType = pSrcPropertyType->GetAbstractPropertyMemberType ()->GetPropertyPtrType (EPropertyPtrType_Thin);

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

	return m_pModule->m_OperatorMgr.CastOperator (StorageKind, PfnValue, pType, pResultValue);
}

//.............................................................................

bool
CCast_PropertyPtr_Thin2Normal::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsPropertyPtrType () && pType->GetTypeKind () == EType_PropertyPtr);
	
	CPropertyPtrType* pSrcPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyPtrType* pDstPtrType = (CPropertyPtrType*) pType;

	CPropertyType* pSrcPropertyType = pSrcPtrType->GetTargetType ();	
	CPropertyType* pDstPropertyType = pDstPtrType->GetTargetType ();

	CClosure* pClosure = OpValue.GetClosure ();
	
	CValue SimpleClosureObjValue;
	
	bool IsSimpleClosure = pClosure && pClosure->IsSimpleClosure ();
	if (IsSimpleClosure)
		SimpleClosureObjValue = *pClosure->GetArgList ()->GetHead ();

	// case 1: no conversion required, no closure object needs to be created

	if (IsSimpleClosure && 
		pSrcPropertyType->IsPropertyMemberType () &&
		pSrcPropertyType->GetShortType ()->Cmp (pDstPropertyType) == 0)
	{
		return LlvmCast_NoThunkSimpleClosure (
			OpValue,
			SimpleClosureObjValue,
			pSrcPropertyType, 
			pDstPtrType,
			pResultValue
			);
	}

	if (OpValue.GetValueKind () == EValue_Property)
	{
		CProperty* pProperty = OpValue.GetProperty ();

		// case 2.1: conversion is required, but no closure object needs to be created (closure arg is null)
		
		if (!pClosure) 			
			return LlvmCast_DirectThunkNoClosure (
				pProperty,
				pDstPtrType,
				pResultValue
				);

		// case 2.2: same as above, but simple closure is passed as closure arg

		if (IsSimpleClosure && pProperty->GetType ()->IsPropertyMemberType ())
			return LlvmCast_DirectThunkSimpleClosure (
				pProperty,
				SimpleClosureObjValue,
				pDstPtrType,
				pResultValue
				);
	}

	// case 3: closure object needs to be created (so conversion is required even if Property signatures match)

	return LlvmCast_FullClosure (
		StorageKind,
		OpValue,
		pSrcPropertyType,
		pDstPtrType,
		pResultValue
		); 
}

bool
CCast_PropertyPtr_Thin2Normal::LlvmCast_NoThunkSimpleClosure (
	const CValue& OpValue,
	const CValue& SimpleClosureObjValue,
	CPropertyType* pSrcPropertyType,
	CPropertyPtrType* pDstPtrType,
	CValue* pResultValue
	)
{
	CClassPtrType* pThisArgType = pSrcPropertyType->GetThisArgType ();

	CValue ThisArgValue;
	bool Result = m_pModule->m_OperatorMgr.CastOperator (SimpleClosureObjValue, pThisArgType, &ThisArgValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateClosurePropertyPtr (OpValue, ThisArgValue, pDstPtrType, pResultValue);
	return true;
}

bool
CCast_PropertyPtr_Thin2Normal::LlvmCast_DirectThunkNoClosure (
	CProperty* pProperty,
	CPropertyPtrType* pDstPtrType,
	CValue* pResultValue
	)
{
	CProperty* pThunkProperty = m_pModule->m_FunctionMgr.GetDirectThunkProperty (
		pProperty, 
		((CPropertyPtrType*) pDstPtrType)->GetTargetType (),
		true
		);

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr)->GetZeroValue ();
	
	return CreateClosurePropertyPtr (pThunkProperty, NullValue, pDstPtrType, pResultValue);
}

bool
CCast_PropertyPtr_Thin2Normal::LlvmCast_DirectThunkSimpleClosure (
	CProperty* pProperty,
	const CValue& SimpleClosureObjValue,
	CPropertyPtrType* pDstPtrType,
	CValue* pResultValue
	)
{
	CClassPtrType* pThisArgType = pProperty->GetType ()->GetThisArgType ();

	CValue ThisArgValue;
	bool Result = m_pModule->m_OperatorMgr.CastOperator (SimpleClosureObjValue, pThisArgType, &ThisArgValue);
	if (!Result)
		return false;

	CProperty* pThunkProperty = m_pModule->m_FunctionMgr.GetDirectThunkProperty (
		pProperty, 
		pThisArgType->GetTargetType ()->GetPropertyMemberType (pDstPtrType->GetTargetType ())
		);

	return CreateClosurePropertyPtr (pThunkProperty, ThisArgValue, pDstPtrType, pResultValue);
}

bool
CCast_PropertyPtr_Thin2Normal::LlvmCast_FullClosure (
	EStorage StorageKind,
	const CValue& OpValue,
	CPropertyType* pSrcPropertyType,
	CPropertyPtrType* pDstPtrType,
	CValue* pResultValue
	)
{
	char Buffer [256];
	rtl::CArrayT <size_t> ClosureMap (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	CValue ClosureObjValue;
	bool Result = m_pModule->m_OperatorMgr.CreateClosureObject (StorageKind, OpValue, &ClosureMap, &ClosureObjValue);
	if (!Result)
		return false;

	ASSERT (ClosureObjValue.GetType ()->GetTypeKind () == EType_ClassPtr);
	CClassType* pClosureType = ((CClassPtrType*) ClosureObjValue.GetType ())->GetTargetType ();

	CProperty* pThunkProperty = m_pModule->m_FunctionMgr.GetClosureThunkProperty (
		pSrcPropertyType,
		OpValue.GetProperty (), 
		pClosureType,
		ClosureMap,
		pDstPtrType->GetTargetType ()
		);

	return CreateClosurePropertyPtr (pThunkProperty, ClosureObjValue, pDstPtrType, pResultValue);
}

bool
CCast_PropertyPtr_Thin2Normal::CreateClosurePropertyPtr (
	CProperty* pProperty,
	const CValue& ClosureValue,
	CPropertyPtrType* pPtrType,
	CValue* pResultValue
	)
{
	CValue ThinPtrValue;
	bool Result = m_pModule->m_OperatorMgr.GetPropertyThinPtr (pProperty, NULL, &ThinPtrValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateClosurePropertyPtr (ThinPtrValue, ClosureValue, pPtrType, pResultValue);
	return true;
}

//.............................................................................

bool
CCast_PropertyPtr_Weak2Normal::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsPropertyPtrType () && pType->GetTypeKind () == EType_PropertyPtr);

	err::SetFormatStringError ("CCast_PropertyPtr_Weak2Normal::LlvmCast not yet implemented");
	return false;
}

//.............................................................................

bool
CCast_PropertyPtr_Thin2Thin::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsPropertyPtrType () && pType->GetTypeKind () == EType_PropertyPtr);
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) pType;

	if (OpValue.GetClosure ())
	{
		err::SetFormatStringError ("cannot create thin property pointer to a closure");
		return false;
	}

	if (OpValue.GetValueKind () != EValue_Property)
	{
		err::SetFormatStringError ("can only create thin pointer thunk to a property, not a property pointer");
		return false;
	}

	CProperty* pProperty = OpValue.GetProperty ();

	if (pPtrType->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Augmented)
	{
		err::SetFormatStringError ("augmented properties are not supported yet");
		return false;
	}

	CProperty* pThunkProperty = m_pModule->m_FunctionMgr.GetDirectThunkProperty (pProperty, pPtrType->GetTargetType ());
	return m_pModule->m_OperatorMgr.GetPropertyThinPtr (pThunkProperty, NULL, pPtrType, pResultValue);
}

//.............................................................................

bool
CCast_PropertyPtr_Thin2Weak::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->IsPropertyPtrType () && pType->GetTypeKind () == EType_PropertyPtr);

	CPropertyPtrType* pIntermediateType = ((CPropertyPtrType*) pType)->GetTargetType ()->GetPropertyPtrType (EPropertyPtrType_Normal);

	CValue TmpValue;
	return 
		m_pModule->m_OperatorMgr.CastOperator (StorageKind, OpValue, pIntermediateType, pResultValue) &&
		m_pModule->m_OperatorMgr.CastOperator (StorageKind, TmpValue, pType, pResultValue);
}

//.............................................................................

bool
CCast_PropertyPtr_Unsafe2Unsafe::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pType, pResultValue);
	return true;
}

//.............................................................................

CCast_PropertyPtr::CCast_PropertyPtr ()
{
	memset (m_OperatorTable, 0, sizeof (m_OperatorTable));

	m_OperatorTable [EPropertyPtrType_Normal] [EPropertyPtrType_Normal] = &m_FromNormal;
	m_OperatorTable [EPropertyPtrType_Normal] [EPropertyPtrType_Weak]   = &m_FromNormal;
	m_OperatorTable [EPropertyPtrType_Weak] [EPropertyPtrType_Normal]   = &m_Weak2Normal;
	m_OperatorTable [EPropertyPtrType_Thin] [EPropertyPtrType_Normal]   = &m_Thin2Normal;
	m_OperatorTable [EPropertyPtrType_Thin] [EPropertyPtrType_Weak]     = &m_Thin2Weak;
	m_OperatorTable [EPropertyPtrType_Thin] [EPropertyPtrType_Thin]     = &m_Thin2Thin;
	m_OperatorTable [EPropertyPtrType_Thin] [EPropertyPtrType_Unsafe]   = &m_Unsafe2Unsafe;
	m_OperatorTable [EPropertyPtrType_Unsafe] [EPropertyPtrType_Unsafe] = &m_Unsafe2Unsafe;
}

ICastOperator*
CCast_PropertyPtr::GetCastOperator (
	const CValue& OpValue,
	CType* pType
	)
{
	ASSERT (pType->GetTypeKind () == EType_PropertyPtr);	

	CPropertyPtrType* pDstPtrType = (CPropertyPtrType*) pType;
	EPropertyPtrType DstPtrTypeKind = pDstPtrType->GetPtrTypeKind ();

	CType* pSrcType = OpValue.GetType ();
	
	if (pSrcType->IsIntegerType ())
	{
		return DstPtrTypeKind == EPropertyPtrType_Unsafe ? 
			m_pModule->m_OperatorMgr.GetStdCastOperator (EStdCast_PtrFromInt) : 
			NULL;
	}
	else if (!pSrcType->IsPropertyPtrType ())
	{
		return NULL;
	}

	CPropertyPtrType* pSrcPtrType = (CPropertyPtrType*) pSrcType;
	EPropertyPtrType SrcPtrTypeKind = pSrcPtrType->GetPtrTypeKind ();

	ASSERT ((size_t) SrcPtrTypeKind < EPropertyPtrType__Count);
	ASSERT ((size_t) DstPtrTypeKind < EPropertyPtrType__Count);

	return m_OperatorTable [SrcPtrTypeKind] [DstPtrTypeKind];
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
