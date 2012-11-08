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

	llvm::Value* pLlvmTrunc = m_pModule->m_LlvmBuilder.CreateBitCast (Value.GetLlvmValue (), pLlvmToType);
	pResultValue->SetLlvmRegister (pLlvmTrunc, pType);
	return true;
}

//.............................................................................

bool
CCast_load::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	err::SetFormatStringError (_T("cannot load a constant"));
	return false;
}

bool
CCast_load::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (Value.GetType ()->IsReferenceType ());
	CPointerType* pValueType = (CPointerType*) Value.GetType ();	
	CType* pTargetType = pValueType->GetBaseType ();

	EType TypeKind = pValueType->GetTypeKind ();
	if (TypeKind == EType_Reference_d)
	{
		err::SetFormatStringError (_T("loading from a dynamic reference is not supported yet"));
		return false;
	}

	llvm::Value* pLlvmPtr;

	if (TypeKind == EType_Reference_u)
	{
		pLlvmPtr = Value.GetLlvmValue ();
	}
	else if (Value.GetValueKind () == EValue_Variable)
	{
		pLlvmPtr = Value.GetLlvmValue ();
	}
	else
	{		
		llvm::Value* pLlvmPtrInt8 = m_pModule->m_LlvmBuilder.CreateExtractValue (Value.GetLlvmValue (), 0);
		llvm::Type* pLlvmPtrType = pTargetType->GetPointerType (EType_Pointer_u)->GetLlvmType ();
		pLlvmPtr = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtrInt8, pLlvmPtrType);

		#pragma AXL_TODO ("check pointer range")
	}

	llvm::Value* pLlvmLoad = m_pModule->m_LlvmBuilder.CreateLoad (pLlvmPtr);	

	CValue LoadValue;
	LoadValue.SetLlvmRegister (pLlvmLoad, pTargetType);
	return m_pModule->m_OperatorMgr.CastOperator (LoadValue, pType, pResultValue);
}

//.............................................................................

bool
CCast_getp::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	err::SetFormatStringError (_T("cannot get a property of a constant"));
	return false;
}

bool
CCast_getp::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("get property is not implemented yet"));
	return false;
}

//.............................................................................

bool
CCast_ptr::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	err::SetFormatStringError (_T("constant pointer casts are not implemented yet"));
	return false;
}

bool
CCast_ptr::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CType* pValueType = Value.GetType ();
	ASSERT (pValueType->IsPointerType ());

	CType* pTargetType = ((CPointerType*) pValueType)->GetBaseType ();

	if (pValueType->GetTypeKind () == EType_Pointer)
	{
		if (Value.GetValueKind () == EValue_Variable)
		{
			ASSERT (pTargetType->Cmp (Value.GetVariable ()->GetType ()) == 0);
		}
		else
		{
			#pragma AXL_TODO ("check pointer data range and actual target type")
		}
	}

	pResultValue->OverrideType (Value, pType);
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
	llvm::Value* pLlvmTrunc = m_pModule->m_LlvmBuilder.CreateTrunc (Value.GetLlvmValue (), pType->GetLlvmType ());
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateSExt (Value.GetLlvmValue (), pType->GetLlvmType ());
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateZExt (Value.GetLlvmValue (), pType->GetLlvmType ());
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

	llvm::Value* pLlvmResult = m_pModule->m_LlvmBuilder.CreateCall (pLlvmSwap, Value.GetLlvmValue ());
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateFPTrunc (Value.GetLlvmValue (), pType->GetLlvmType ());
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateFPExt (Value.GetLlvmValue (), pType->GetLlvmType ());
	pResultValue->SetLlvmRegister (pLlvmExt, pType);
	return true;
}

//.............................................................................

bool
CCast_int_bool::ConstCast (
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
CCast_int_bool::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	llvm::Constant* pLlvmZero = llvm::ConstantInt::get (Value.GetType ()->GetLlvmType (), 0);
	llvm::Value* pLlvmCmp = m_pModule->m_LlvmBuilder.CreateICmpNE (Value.GetLlvmValue (), pLlvmZero);
	pResultValue->SetLlvmRegister (pLlvmCmp, pType);
	return true;
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
	err::SetFormatStringError (_T("CCast_bool_int::ConstCast NOT IMPLEMENTED"));
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateSIToFP (Value.GetLlvmValue (), pType->GetLlvmType ());
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateUIToFP (Value.GetLlvmValue (), pType->GetLlvmType ());
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
	llvm::Value* pLlvmExt = m_pModule->m_LlvmBuilder.CreateFPToSI (Value.GetLlvmValue (), pType->GetLlvmType ());
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
	CValue Zero;
	Zero.SetConstInt32 (0, EType_Int32);
	
	llvm::Value* pLlvmValue = Value.GetLlvmValue ();
	llvm::Value* pLlvmZero = Zero.GetLlvmValue ();

	llvm::Value* LlvmIndexArray [] =
	{
		pLlvmZero,
		pLlvmZero,
	};

	llvm::Value* pLlvmGep = m_pModule->m_LlvmBuilder.CreateGEP (
		pLlvmValue, 
		llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, 2)
		);
	pResultValue->SetLlvmRegister (pLlvmGep, pType);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
