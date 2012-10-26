#include "stdafx.h"
#include "axl_jnc_MoveOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CMoveOperator::CMoveOperator ()
{
	m_Price = 1;
	m_CanMove = ECanMove_Implicit;
	m_pOperatorLo = NULL;
	m_pDstType = NULL;
	m_pSrcType = NULL;
}

bool
CMoveOperator::Move (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	EValue SrcValueKind = SrcValue.GetValueKind ();
	EValue DstValueKind = DstValue.GetValueKind ();

	if (SrcValueKind == EValue_Const)
	{
		if (DstValueKind == EValue_Const)
			return m_pOperatorLo->ConstMove (SrcValue, DstValue);

		CValue CastValue;
		return 
			pModule->m_OperatorMgr.CastOperator (SrcValue, DstValue.GetType (), &CastValue);
			pModule->m_OperatorMgr.MoveOperator (CastValue, DstValue);
	}

	return m_pOperatorLo->LlvmMove (pModule, SrcValue, DstValue);
}

//.............................................................................

bool
CSuperMove::ConstMove (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CValue TempValue;
	return 
		TempValue.CreateConst (m_pFirst->GetDstType ()) && 
		m_pFirst->Move (SrcValue, TempValue) &&
		m_pSecond->Move (TempValue, DstValue);
}

bool
CSuperMove::LlvmMove (
	CModule* pModule,
	const CValue& DstValue,
	const CValue& SrcValue
	)
{
	CVariable* pTemp = pModule->m_VariableMgr.CreateTempVariable (m_pFirst->GetDstType ());

	CValue TempValue;
	TempValue.SetVariable (pTemp);

	return 
		m_pFirst->Move (SrcValue, TempValue) &&
		m_pSecond->Move (TempValue, DstValue);
}

//.............................................................................

bool
CMove_cpy::ConstMove (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	size_t SrcSize = SrcValue.GetType ()->GetSize ();
	size_t DstSize = DstValue.GetType ()->GetSize ();

	ASSERT (SrcSize == DstSize);

	memcpy (DstValue.GetConstData (), SrcValue.GetConstData (), DstSize);
	return true;
}

bool
CMove_cpy::LlvmMove (
	CModule* pModule,
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	llvm::Value* pLlvmSrcValue = SrcValue.GetLlvmValue ();
	llvm::Value* pLlvmDstValue = DstValue.GetLlvmValue ();
	llvm::LoadInst* pLlvmLoad = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLoad (pLlvmSrcValue);
	pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateStore (pLlvmLoad, pLlvmDstValue);
	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CMove_int_trunc::ConstMove (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	size_t SrcSize = SrcValue.GetType ()->GetSize ();
	size_t DstSize = DstValue.GetType ()->GetSize ();

	ASSERT (SrcSize >= DstSize);

	memcpy (DstValue.GetConstData (), SrcValue.GetConstData (), DstSize);
	return true;
}

bool
CMove_int_trunc::LlvmMove (
	CModule* pModule,
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	llvm::Value* pLlvmSrcValue = SrcValue.GetLlvmValue ();
	llvm::Value* pLlvmDstValue = DstValue.GetLlvmValue ();
	llvm::Type* pLlvmDstType = DstValue.GetType ()->GetLlvmType ();
	llvm::LoadInst* pLlvmLoad = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLoad (pLlvmSrcValue);
	llvm::Value* pLlvmTrunc = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateTrunc (pLlvmLoad, pLlvmDstType);
	pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateStore (pLlvmTrunc, pLlvmDstValue);
	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CMove_int_ext::ConstMove (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	size_t SrcSize = SrcValue.GetType ()->GetSize ();
	size_t DstSize = DstValue.GetType ()->GetSize ();

	ASSERT (SrcSize < DstSize);

	char* pSrc = (char*) SrcValue.GetConstData ();
	char* pDst = (char*) DstValue.GetConstData ();

	if (pSrc [SrcSize - 1] < 0)
		memset (pDst, -1, DstSize);
	else
		memset (pDst, 0, DstSize);

	memcpy (pDst, pSrc, SrcSize);

	*(int16_t*) DstValue.GetConstData () = *(int8_t*) SrcValue.GetConstData ();
	return true;
}

bool
CMove_int_ext::LlvmMove (
	CModule* pModule,
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	llvm::Value* pLlvmSrcValue = SrcValue.GetLlvmValue ();
	llvm::Value* pLlvmDstValue = DstValue.GetLlvmValue ();
	llvm::Type* pLlvmDstType = DstValue.GetType ()->GetLlvmType ();
	llvm::LoadInst* pLlvmLoad = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLoad (pLlvmSrcValue);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateSExt (pLlvmLoad, pLlvmDstType);
	pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateStore (pLlvmExt, pLlvmDstValue);
	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CMove_int_ext_u::ConstMove (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	size_t SrcSize = SrcValue.GetType ()->GetSize ();
	size_t DstSize = DstValue.GetType ()->GetSize ();

	ASSERT (SrcSize < DstSize);

	char* pSrc = (char*) SrcValue.GetConstData ();
	char* pDst = (char*) DstValue.GetConstData ();

	memset (pDst, 0, DstSize);
	memcpy (pDst, pSrc, SrcSize);

	*(int16_t*) DstValue.GetConstData () = *(int8_t*) SrcValue.GetConstData ();
	return true;
}

bool
CMove_int_ext_u::LlvmMove (
	CModule* pModule,
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	llvm::Value* pLlvmSrcValue = SrcValue.GetLlvmValue ();
	llvm::Value* pLlvmDstValue = DstValue.GetLlvmValue ();
	llvm::Type* pLlvmDstType = DstValue.GetType ()->GetLlvmType ();
	llvm::LoadInst* pLlvmLoad = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLoad (pLlvmSrcValue);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateZExt (pLlvmLoad, pLlvmDstType);
	pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateStore (pLlvmExt, pLlvmDstValue);
	return true;
}

//.............................................................................

bool
CMove_f32_f64::LlvmMove (
	CModule* pModule,
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	llvm::Value* pLlvmSrcValue = SrcValue.GetLlvmValue ();
	llvm::Value* pLlvmDstValue = DstValue.GetLlvmValue ();
	llvm::Type* pLlvmDstType = DstValue.GetType ()->GetLlvmType ();
	llvm::LoadInst* pLlvmLoad = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLoad (pLlvmSrcValue);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFPExt (pLlvmLoad, pLlvmDstType);
	pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateStore (pLlvmExt, pLlvmDstValue);
	return true;
}

//.............................................................................

bool
CMove_f64_f32::LlvmMove (
	CModule* pModule,
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	llvm::Value* pLlvmSrcValue = SrcValue.GetLlvmValue ();
	llvm::Value* pLlvmDstValue = DstValue.GetLlvmValue ();
	llvm::Type* pLlvmDstType = DstValue.GetType ()->GetLlvmType ();
	llvm::LoadInst* pLlvmLoad = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLoad (pLlvmSrcValue);
	llvm::Value* pLlvmTrunc = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFPTrunc (pLlvmLoad, pLlvmDstType);
	pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateStore (pLlvmTrunc, pLlvmDstValue);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
