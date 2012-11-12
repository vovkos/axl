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

bool
ICastOperator::Cast (
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	EValue OpValueKind = OpValue.GetValueKind ();
	if (OpValueKind == EValue_Const)
	{
		return 
			pResultValue->CreateConst (pType) &&
			ConstCast (OpValue, *pResultValue);
	}

	return LlvmCast (OpValue, pType, pResultValue);
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
		TempValue.CreateConst (m_pIntermediateType) &&
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

	llvm::Type* pLlvmFromType = Value.GetType ()->GetLlvmType ();
	llvm::Type* pLlvmToType = pType->GetLlvmType ();

	if (pLlvmFromType == pLlvmToType)
	{
		pResultValue->OverrideType (Value, pType);
		return true;
	}

	llvm::Value* pLlvmTrunc = m_pModule->m_LlvmBuilder.CreateBitCast (Value.GetLlvmValue (), pLlvmToType, "cast_cpy");
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
	llvm::Value* pLlvmTrunc = m_pModule->m_LlvmBuilder.CreateTrunc (Value.GetLlvmValue (), pType->GetLlvmType (), "trunc_i");
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateSExt (Value.GetLlvmValue (), pType->GetLlvmType (), "ext_i");
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateZExt (Value.GetLlvmValue (), pType->GetLlvmType (), "ext_u");
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

	llvm::Function* pLlvmSwap = llvm::Intrinsic::getDeclaration (
		m_pModule->m_pLlvmModule,
		llvm::Intrinsic::bswap,
		llvm::ArrayRef <llvm::Type*> (pLlvmType)
		);

	llvm::Value* pLlvmResult = m_pModule->m_LlvmBuilder.CreateCall (pLlvmSwap, Value.GetLlvmValue (), "bswap");
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateFPTrunc (Value.GetLlvmValue (), pType->GetLlvmType (), "trunc_f");
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateFPExt (Value.GetLlvmValue (), pType->GetLlvmType (), "ext_f");
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_num_bool::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	const char* p = (const char*) SrcValue.GetConstData ();
	const char* pEnd = p + SrcValue.GetType ()->GetSize ();
	
	bool Bool = false;

	for (; p < pEnd; p++)
	{
		if (*p)
		{
			Bool = true;
			break;
		}
	}

	*(bool*) DstValue.GetConstData () = Bool;
	return true;
}

bool
CCast_num_bool::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue Zero (Value.GetType (), NULL);
	return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Ne, Value, Zero, pResultValue);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CCast_bool_int::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	err::SetFormatStringError (_T("CCast_bool_int::ConstCast NOT IMPLEMENTED"));
	return false;
}

bool
CCast_bool_int::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("CCast_bool_int::LlvmCast NOT IMPLEMENTED"));
	return false;
}

//.............................................................................

bool
CCast_int_fp::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateSIToFP (Value.GetLlvmValue (), pType->GetLlvmType (), "cast_i_f");
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateUIToFP (Value.GetLlvmValue (), pType->GetLlvmType (), "cast_u_f");
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateFPToSI (Value.GetLlvmValue (), pType->GetLlvmType (), "cast_f_i");
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
