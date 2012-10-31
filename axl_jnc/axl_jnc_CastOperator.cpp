#include "stdafx.h"
#include "axl_jnc_CastOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CCastOperator::CCastOperator ()
{
	m_Price = 1;
	m_CastKind = ECast_Implicit;
	m_pOperatorLo = NULL;
	m_pDstType = NULL;
	m_pSrcType = NULL;
}

bool
CCastOperator::Cast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	EValue ValueKind = Value.GetValueKind ();
	if (ValueKind == EValue_Const)
	{
		return 
			pResultValue->CreateConst (pType) &&
			m_pOperatorLo->ConstCast (Value, *pResultValue);
	}

	ASSERT (ValueKind == EValue_Const || ValueKind == EValue_Variable || ValueKind == EValue_LlvmRegister);
	return m_pOperatorLo->LlvmCast (pModule, Value, pType, pResultValue);
}

//.............................................................................

CSuperCast::CSuperCast ()
{
	m_pIntermediateType = NULL;
	m_pFirst = NULL;
	m_pSecond = NULL;
}

bool
CSuperCast::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	ASSERT (SrcValue.GetValueKind () == EValue_Const && DstValue.GetValueKind () == EValue_Const);

	CValue TempValue;
	return 
		TempValue.CreateConst (m_pIntermediateType);
		m_pFirst->ConstCast (SrcValue, TempValue) &&
		m_pSecond->ConstCast (TempValue, DstValue);
}

bool
CSuperCast::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue TempValue;
	return 
		m_pFirst->LlvmCast (pModule, Value, m_pIntermediateType, &TempValue) &&
		m_pSecond->LlvmCast (pModule, TempValue, pType, pResultValue);
}

//.............................................................................

bool
CCast_cpy::ConstCast (
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
CCast_cpy::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	if (pType->Cmp (Value.GetType ()) == 0)
	{
		*pResultValue = Value;
		return true;
	}

	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmTrunc = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateBitCast (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmTrunc, pType);
	return true;
}

//.............................................................................

bool
CCast_int_trunc::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	size_t SrcSize = SrcValue.GetType ()->GetSize ();
	size_t DstSize = DstValue.GetType ()->GetSize ();

	ASSERT (SrcSize > DstSize);

	memcpy (DstValue.GetConstData (), SrcValue.GetConstData (), DstSize);
	return true;
}

bool
CCast_int_trunc::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmTrunc = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateTrunc (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmTrunc, pType);
	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CCast_int_ext::ConstCast (
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
	return true;
}

bool
CCast_int_ext::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateSExt (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CCast_int_ext_u::ConstCast (
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
	return true;
}

bool
CCast_int_ext_u::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateZExt (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_int_swp::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	size_t SrcSize = SrcValue.GetType ()->GetSize ();
	size_t DstSize = SrcValue.GetType ()->GetSize ();

	ASSERT (SrcSize == DstSize);

	char* pSrc = (char*) SrcValue.GetConstData ();
	char* pDst = (char*) DstValue.GetConstData ();

	rtl::SwapByteOrder (pDst, pSrc, SrcSize);
	return true;
}

bool
CCast_int_swp::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Type* pLlvmType = pType->GetLlvmType ();
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);

	llvm::Function* pLlvmSwap = llvm::Intrinsic::getDeclaration (
		pModule->m_pLlvmModule,
		llvm::Intrinsic::bswap,
		llvm::ArrayRef <llvm::Type*> (pLlvmType)
		);

	llvm::Value* pLlvmResult = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateCall (pLlvmSwap, pLlvmValue);
	pResultValue->SetLlvmRegister (pLlvmResult, pType);
	return true;
}

//.............................................................................

bool
CCast_f64_f32::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFPTrunc (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_f32_f64::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFPExt (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_int_fp::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateSIToFP (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_uint_fp::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateUIToFP (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_fp_int::LlvmCast (
	CModule* pModule,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFPToSI (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
