#include "stdafx.h"
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
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_ClassPtr && pType->GetTypeKind () == EType_ClassPtr);

	CClassPtrType* pSrcType = (CClassPtrType*) OpValue.GetType ();
	CClassPtrType* pDstType = (CClassPtrType*) pType;

	if (pSrcType->GetPtrTypeKind () == EClassPtrType_Unsafe && 
		pDstType->GetPtrTypeKind () != EClassPtrType_Unsafe)
		return ECast_None;

	if ((pSrcType->GetFlags () & EPtrTypeFlag_Const) != 0 && 
		(pDstType->GetFlags () & EPtrTypeFlag_Const) == 0 &&
		pDstType->GetPtrTypeKind () != EDataPtrType_Unsafe)
		return ECast_None;

	CClassType* pSrcClassType = pSrcType->GetTargetType ();
	CClassType* pDstClassType = pDstType->GetTargetType ();

	return pSrcClassType->Cmp (pDstClassType) == 0 || pSrcClassType->FindBaseType (pDstClassType) ? 
		ECast_Implicit : 
		ECast_Explicit;
}

bool
CCast_ClassPtr::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_ClassPtr && pType->GetTypeKind () == EType_ClassPtr);

	CClassPtrType* pSrcType = (CClassPtrType*) OpValue.GetType ();
	CClassPtrType* pDstType = (CClassPtrType*) pType;

	CClassType* pSrcClassType = pSrcType->GetTargetType ();
	CClassType* pDstClassType = pDstType->GetTargetType ();

	if (pSrcClassType->Cmp (pDstClassType) == 0)
	{
		pResultValue->OverrideType (OpValue, pType);
		return true;
	}

	CBaseTypeCoord Coord;
	bool Result = pSrcClassType->FindBaseType (pDstClassType, &Coord);
	if (!Result)
		return m_pModule->m_LlvmBuilder.DynamicCastInterface (OpValue, pDstClassType, pResultValue);

	CValue SrcNullValue = pSrcType->GetZeroValue ();
	CValue DstNullValue = pDstType->GetZeroValue ();

	CBasicBlock* pCmpBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	CBasicBlock* pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_phi"));
	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_nonull"));

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
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
