#include "stdafx.h"
#include "axl_jnc_CastOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

ICastOperator::ICastOperator()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_Price = 1;
}

//.............................................................................

CSuperCast::CSuperCast ()
{
	m_pIntermediateType = NULL;
	m_pFirst = NULL;
	m_pSecond = NULL;
}

ECast
CSuperCast::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	ECast FirstCastKind = m_pFirst->GetCastKind (pSrcType, m_pIntermediateType);
	ECast SecondCastKind = m_pSecond->GetCastKind (m_pIntermediateType, pDstType);
	 
	return FirstCastKind && SecondCastKind ? max (FirstCastKind,SecondCastKind) : ECast_None;
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
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue TempValue;
	return 
		m_pFirst->LlvmCast (Value, m_pIntermediateType, &TempValue) &&
		m_pSecond->LlvmCast (TempValue, pType, pResultValue);
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

	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmTrunc = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateBitCast (pLlvmValue, pType->GetLlvmType ());
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
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmTrunc = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateTrunc (pLlvmValue, pType->GetLlvmType ());
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
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateSExt (pLlvmValue, pType->GetLlvmType ());
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
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateZExt (pLlvmValue, pType->GetLlvmType ());
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
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Type* pLlvmType = pType->GetLlvmType ();
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);

	llvm::Function* pLlvmSwap = llvm::Intrinsic::getDeclaration (
		m_pModule->m_pLlvmModule,
		llvm::Intrinsic::bswap,
		llvm::ArrayRef <llvm::Type*> (pLlvmType)
		);

	llvm::Value* pLlvmResult = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateCall (pLlvmSwap, pLlvmValue);
	pResultValue->SetLlvmRegister (pLlvmResult, pType);
	return true;
}

//.............................................................................

bool
CCast_f64_f32::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFPTrunc (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_f32_f64::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFPExt (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_int_fp::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateSIToFP (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_uint_fp::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateUIToFP (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_fp_int::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmValue = m_pModule->m_OperatorMgr.LoadValue (Value);
	llvm::Value* pLlvmExt = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFPToSI (pLlvmValue, pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_arr_ptr_c::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	const CValue& SavedSrcValue = m_pModule->m_ConstMgr.SaveValue (SrcValue);
	*(void**) DstValue.GetConstData () = SavedSrcValue.GetConstData ();
	return true;
}

bool
CCast_arr_ptr_c::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue Index;
	Index.SetConstInt32 (0);
	
	llvm::Value* pLlvmValue = Value.GetLlvmValue ();
	llvm::Value* pLlvmIndex = Index.GetLlvmValue ();

	llvm::Value* LlvmIndexArray [] =
	{
		pLlvmIndex,
		pLlvmIndex,
	};

	llvm::Value* pLlvmGep = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateGEP (
		pLlvmValue, 
		llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, 2)
		);
	pResultValue->SetLlvmRegister (pLlvmGep, pType);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
