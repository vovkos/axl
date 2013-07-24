#include "pch.h"
#include "axl_jnc_CastOp_ClassPtr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

ECast
CCast_ClassPtr::GetCastKind (
	const CValue& OpValue,
	CType* pType
	)
{
	ASSERT (pType->GetTypeKind () == EType_ClassPtr);

	if (OpValue.GetType ()->GetTypeKind () != EType_ClassPtr)
		return ECast_None; // TODO: user conversions later via constructors

	CClassPtrType* pSrcType = (CClassPtrType*) OpValue.GetType ();
	CClassPtrType* pDstType = (CClassPtrType*) pType;

	if ((pSrcType->GetFlags () & EPtrTypeFlag_Const) != 0 && 
		(pDstType->GetFlags () & (EPtrTypeFlag_Const | EPtrTypeFlag_Unsafe)) == 0)
		return ECast_None;

	CClassType* pSrcClassType = pSrcType->GetTargetType ();
	CClassType* pDstClassType = pDstType->GetTargetType ();

	return 
		(pSrcClassType->GetClassTypeKind () == EClassType_StdObject) ||
		(pDstClassType->GetClassTypeKind () == EClassType_StdObject) ||
		pSrcClassType->Cmp (pDstClassType) == 0 || 
		pSrcClassType->FindBaseType (pDstClassType) ? 
		ECast_Implicit : 
		ECast_Explicit;
}

bool
CCast_ClassPtr::LlvmCast (
	EStorage StorageKind,
	const CValue& RawOpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (pType->GetTypeKind () == EType_ClassPtr);

	bool Result;

	if (RawOpValue.GetType ()->GetTypeKind () != EType_ClassPtr)
	{
		SetCastError (RawOpValue, pType);
		return false; // TODO: user conversions via constructors -- only if target ptr is EPtrTypeFlag_Const
	}

	CValue OpValue = RawOpValue;

	CClassPtrType* pSrcType = (CClassPtrType*) RawOpValue.GetType ();
	CClassPtrType* pDstType = (CClassPtrType*) pType;

	if (pSrcType->GetPtrTypeKind () == EClassPtrType_Weak &&
		pDstType->GetPtrTypeKind () != EClassPtrType_Weak)
	{
		CLlvmScopeComment Comment (&m_pModule->m_LlvmBuilder, "strengthen class pointer");

		CFunction* pStrengthen = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_StrengthenClassPtr);

		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, m_pModule->GetSimpleType (EStdType_ObjectPtr), &OpValue);
		m_pModule->m_LlvmBuilder.CreateCall (
			pStrengthen,
			pStrengthen->GetType (),
			OpValue,
			&OpValue
			);

		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pSrcType, &OpValue);
	}

	CClassType* pSrcClassType = pSrcType->GetTargetType ();
	CClassType* pDstClassType = pDstType->GetTargetType ();

	if (pDstType->GetFlags () & EPtrTypeFlag_Checked)
		m_pModule->m_OperatorMgr.CheckClassPtrNull (OpValue);

	if (pDstClassType->GetClassTypeKind () == EClassType_StdObject)
	{
		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pDstType, pResultValue);
		return true;
	}

	if (pSrcClassType->Cmp (pDstClassType) == 0)
	{
		pResultValue->OverrideType (OpValue, pType);
		return true;
	}

	CBaseTypeCoord Coord;
	Result = pSrcClassType->FindBaseTypeTraverse (pDstClassType, &Coord);
	if (!Result)
	{
		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &PtrValue);

		CValue TypeValue (&pDstClassType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));

		CFunction* pDynamicCastClassPtr = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_DynamicCastClassPtr);
		m_pModule->m_LlvmBuilder.CreateCall2 (
			pDynamicCastClassPtr,
			pDynamicCastClassPtr->GetType (),
			PtrValue,
			TypeValue,
			&PtrValue
			);

		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pDstType, pResultValue);
		return true;
	}

	CValue SrcNullValue = pSrcType->GetZeroValue ();
	CValue DstNullValue = pDstType->GetZeroValue ();

	CBasicBlock* pCmpBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	CBasicBlock* pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("iface_phi");
	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("iface_nonull");

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (OpValue, SrcNullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pPhiBlock, pNoNullBlock, pNoNullBlock);
	
	Coord.m_LlvmIndexArray.Insert (0, 0);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (
		OpValue, 
		Coord.m_LlvmIndexArray,
		Coord.m_LlvmIndexArray.GetCount (),
		NULL, 
		&PtrValue
		);		

	m_pModule->m_ControlFlowMgr.Follow (pPhiBlock);

	m_pModule->m_LlvmBuilder.CreatePhi (PtrValue, pNoNullBlock, DstNullValue, pCmpBlock, pResultValue);
	pResultValue->OverrideType (pDstType);
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
