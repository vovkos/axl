#include "stdafx.h"
#include "axl_jnc_CastOp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

err::CError
SetCastError (
	const CValue& OpValue,
	CType* pDstType
	)
{
	CType* pSrcType = OpValue.GetType ();
	if (!pSrcType)
	{
		ASSERT (OpValue.GetValueKind () == EValue_Function);
		CFunction* pFunction = (CFunction*) OpValue.GetFunction ();

		return err::SetFormatStringError (
			_T("not enough information to select one of %d overloads of '%s'"),
			pFunction->GetOverloadCount (),
			pFunction->m_Tag
			);
	}

	return err::SetFormatStringError (
		_T("cannot convert from '%s' to '%s'"),
		OpValue.GetType ()->GetTypeString (),
		pDstType->GetTypeString ()
		);
}

//.............................................................................

ICastOperator::ICastOperator()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_OpFlags = 0;
}

bool
ICastOperator::Cast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	EValue OpValueKind = OpValue.GetValueKind ();
	if (OpValueKind == EValue_Const)
	{
		return 
			pResultValue->CreateConst (NULL, pType) &&
			ConstCast (OpValue, pType, pResultValue->GetConstData ());
	}

	return LlvmCast (AllocKind, OpValue, pType, pResultValue);
}

bool
ICastOperator::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	// fail by default; if const-cast is supported then override and implement

	err::SetFormatStringError (
		_T("cannot convert constant from '%s' to '%s'"),
		OpValue.GetType ()->GetTypeString (),
		pType->GetTypeString ()
		);

	return false;
}

//.............................................................................

bool
CCast_Copy::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	size_t SrcSize = OpValue.GetType ()->GetSize ();
	size_t DstSize = pType->GetSize ();

	ASSERT (SrcSize == DstSize);

	memcpy (pDst, OpValue.GetConstData (), DstSize);
	return true;
}

bool
CCast_Copy::LlvmCast (
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

ECast
CCast_Master::GetCastKind (
	const CValue& RawOpValue,
	CType* pType
	)
{
	if (!RawOpValue.GetType ())
		return ECast_None;

	ICastOperator* pOperator = GetCastOperator (RawOpValue, pType);
	if (!pOperator)
		return ECast_None;

	CValue OpValue = RawOpValue;

	int OpFlags = pOperator->GetOpFlags ();
	if (OpFlags != m_OpFlags)
		m_pModule->m_OperatorMgr.PrepareOperandType (&OpValue, OpFlags);

	return pOperator->GetCastKind (OpValue, pType);
}

bool
CCast_Master::ConstCast (
	const CValue& RawOpValue,
	CType* pType,
	void* pDst
	)
{
	ICastOperator* pOperator = GetCastOperator (RawOpValue, pType);
	if (!pOperator)
	{
		SetCastError (RawOpValue, pType);
		return false;
	}

	CValue OpValue = RawOpValue;

	int OpFlags = pOperator->GetOpFlags ();
	if (OpFlags != m_OpFlags)
	{
		bool Result = m_pModule->m_OperatorMgr.PrepareOperand (&OpValue, OpFlags);
		if (!Result)
			return false;
	}

	return pOperator->ConstCast (OpValue, pType, pDst);
}

bool
CCast_Master::LlvmCast (
	EAlloc AllocKind,
	const CValue& RawOpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ICastOperator* pOperator = GetCastOperator (RawOpValue, pType);
	if (!pOperator)
	{
		SetCastError (RawOpValue, pType);
		return false;
	}

	CValue OpValue = RawOpValue;

	int OpFlags = pOperator->GetOpFlags ();
	if (OpFlags != m_OpFlags)
	{
		bool Result = m_pModule->m_OperatorMgr.PrepareOperand (&OpValue, OpFlags);
		if (!Result)
			return false;
	}
		
	return pOperator->LlvmCast (AllocKind, OpValue, pType, pResultValue);
}

//.............................................................................

ECast
CCast_SuperMaster::GetCastKind (
	const CValue& RawOpValue,
	CType* pType
	)
{
	if (!RawOpValue.GetType ())
		return ECast_None;

	ICastOperator* pOperator1 = NULL;
	ICastOperator* pOperator2 = NULL;
	CType* pIntermediateType = NULL;

	bool Result = GetCastOperators (
		RawOpValue, 
		pType,
		&pOperator1, 
		&pOperator2, 
		&pIntermediateType
		);

	if (!Result)
		return ECast_None;

	ASSERT (pOperator1);

	CValue OpValue = RawOpValue;

	int OpFlags1 = pOperator1->GetOpFlags ();
	if (OpFlags1 != m_OpFlags)
		m_pModule->m_OperatorMgr.PrepareOperandType (&OpValue, OpFlags1);

	if (!pOperator2) 
		return pOperator1->GetCastKind (OpValue, pType);
	
	ECast CastKind1 = pOperator1->GetCastKind (OpValue, pIntermediateType);
	ECast CastKind2 = pOperator2->GetCastKind (pIntermediateType, pType);
	return min (CastKind1, CastKind2);
}

bool
CCast_SuperMaster::ConstCast (
	const CValue& RawOpValue,
	CType* pType,
	void* pDst
	)
{
	ICastOperator* pOperator1 = NULL;
	ICastOperator* pOperator2 = NULL;
	CType* pIntermediateType = NULL;

	bool Result = GetCastOperators (
		RawOpValue, 
		pType,
		&pOperator1, 
		&pOperator2, 
		&pIntermediateType
		);

	if (!Result)
	{
		SetCastError (RawOpValue, pType);
		return false;
	}

	ASSERT (pOperator1);

	CValue SrcValue = RawOpValue;

	int OpFlags1 = pOperator1->GetOpFlags ();
	if (OpFlags1 != m_OpFlags)
	{
		bool Result = m_pModule->m_OperatorMgr.PrepareOperand (&SrcValue, OpFlags1);
		if (!Result)
			return false;
	}

	if (!pOperator2) 
		return pOperator1->ConstCast (SrcValue, pType, pDst);

	CValue TmpValue;
	return 
		TmpValue.CreateConst (NULL, pIntermediateType) &&
		pOperator1->ConstCast (SrcValue, pIntermediateType, TmpValue.GetConstData ()) &&
		pOperator2->ConstCast (TmpValue, pType, pDst);
}

bool
CCast_SuperMaster::LlvmCast (
	EAlloc AllocKind,
	const CValue& RawOpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ICastOperator* pOperator1 = NULL;
	ICastOperator* pOperator2 = NULL;
	CType* pIntermediateType = NULL;

	bool Result = GetCastOperators (
		RawOpValue, 
		pType,
		&pOperator1, 
		&pOperator2, 
		&pIntermediateType
		);

	if (!Result)
	{
		SetCastError (RawOpValue, pType);
		return false;
	}

	ASSERT (pOperator1);

	CValue OpValue = RawOpValue;

	int OpFlags1 = pOperator1->GetOpFlags ();
	if (OpFlags1 != m_OpFlags)
	{
		bool Result = m_pModule->m_OperatorMgr.PrepareOperand (&OpValue, OpFlags1);
		if (!Result)
			return false;
	}

	if (!pOperator2) 
		return pOperator1->LlvmCast (AllocKind, OpValue, pType, pResultValue);

	CValue TmpValue;
	return 
		pOperator1->LlvmCast (AllocKind, OpValue, pIntermediateType, &TmpValue) &&
		pOperator2->LlvmCast (AllocKind, TmpValue, pType, pResultValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
