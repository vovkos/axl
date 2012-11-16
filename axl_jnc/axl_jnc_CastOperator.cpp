#include "stdafx.h"
#include "axl_jnc_CastOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

err::CError
SetCastError (
	CType* pSrcType,
	CType* pDstType
	)
{
	return err::SetFormatStringError (
		_T("cannot convert from '%s' to '%s'"),
		pSrcType->GetTypeString (),
		pDstType->GetTypeString ()
		);
}

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

	llvm::Value* pLlvmBitCast = m_pModule->m_LlvmBuilder.CreateBitCast (Value.GetLlvmValue (), pLlvmToType, "cast_cpy");
	pResultValue->SetLlvmRegister (pLlvmBitCast, pType);
	return true;
}

//.............................................................................

ECast
CCast_load::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	ASSERT (pSrcType->IsReferenceType ());
	return m_pModule->m_OperatorMgr.GetCastKind (((CPointerType*) pSrcType)->GetBaseType (), pDstType);
}

bool
CCast_load::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	err::SetFormatStringError (_T("can't load constant reference"));
	return false;
}

bool
CCast_load::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue TmpValue;
	return 
		m_pModule->m_OperatorMgr.LoadReferenceOperator (Value, &TmpValue) &&
		m_pModule->m_OperatorMgr.CastOperator (TmpValue, pType, pResultValue);
}

//.............................................................................

// get property

ECast
CCast_getp::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	ASSERT (pSrcType->GetTypeKind () == EType_Property);
	CFunctionType* pGetterType = ((CPropertyType*) pSrcType)->GetGetterType ();
	return m_pModule->m_OperatorMgr.GetCastKind (pGetterType->GetReturnType (), pDstType);
}

bool
CCast_getp::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	err::SetFormatStringError (_T("can't get constant property"));
	return false;
}

bool
CCast_getp::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (Value.GetValueKind () == EValue_Property);

	CValue TmpValue;
	return 
		m_pModule->m_OperatorMgr.GetPropertyOperator (Value.GetProperty (), &TmpValue) &&
		m_pModule->m_OperatorMgr.CastOperator (TmpValue, pType, pResultValue);
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

//.............................................................................

void
AssertPointerCastTypeValid (
	CType* pSrcType,
	CType* pDstType
	)
{
	EType SrcTypeKind = pSrcType->GetTypeKind ();
	EType DstTypeKind = pDstType->GetTypeKind ();

	ASSERT (
		SrcTypeKind == EType_Pointer && DstTypeKind == EType_Pointer ||
		SrcTypeKind == EType_Pointer && DstTypeKind == EType_Pointer_u ||
		SrcTypeKind == EType_Pointer_u && DstTypeKind == EType_Pointer_u
		);
}

intptr_t
GetPointerCastOffset (
	CType* pSrcType,
	CType* pDstType
	)
{
	AssertPointerCastTypeValid (pSrcType, pDstType);
		
	CType* pSrcBaseType = ((CPointerType*) pSrcType)->GetBaseType ();
	CType* pDstBaseType = ((CPointerType*) pDstType)->GetBaseType ();

	if (pSrcBaseType->GetTypeKind () != EType_Struct ||
		pDstBaseType->GetTypeKind () != EType_Struct)
		return 0;

	CStructType* pSrcStructType = (CStructType*) pSrcBaseType;
	CStructType* pDstStructType = (CStructType*) pDstBaseType;

	#pragma AXL_TODO ("implement struct inheritance check and offset calculation")

	return 0;
}

//.............................................................................

ECast
CCast_ptr::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	AssertPointerCastTypeValid (pSrcType, pDstType);
		
	CType* pSrcBaseType = ((CPointerType*) pSrcType)->GetBaseType ();
	CType* pDstBaseType = ((CPointerType*) pDstType)->GetBaseType ();

	if (pDstType->GetTypeKind () == EType_Pointer)
	{
		return 
			(pSrcBaseType->GetFlags () & ETypeFlag_IsPod) &&
			(pDstBaseType->GetFlags () & ETypeFlag_IsPod) ? ECast_Lossy : ECast_None;
	}
	else
	{
		return ECast_Lossy;
	}
}

bool
CCast_ptr::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	AssertPointerCastTypeValid (SrcValue.GetType (), DstValue.GetType ());
		
	intptr_t Offset = GetPointerCastOffset (SrcValue.GetType (), DstValue.GetType ());

	if (DstValue.GetType ()->GetTypeKind () == EType_Pointer_u)
	{
		*(char**) DstValue.GetConstData () = *(char**) SrcValue.GetConstData () + Offset;
	}
	else
	{
		TSafePtr Ptr = *(TSafePtr*) SrcValue.GetConstData ();
		Ptr.m_p = (char*) Ptr.m_p + Offset;
		*(TSafePtr*) DstValue.GetConstData () = Ptr;
	}

	return true;
}

bool
CCast_ptr::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	AssertPointerCastTypeValid (Value.GetType (), pType);
	
	CPointerType* pPointerType = (CPointerType*) pType;
	intptr_t Offset = GetPointerCastOffset (Value.GetType (), pType);

	return 
		pType->GetTypeKind () == EType_Pointer ? LlvmCast_ptr (Value, pPointerType, Offset, pResultValue) :
		Value.GetType ()->GetTypeKind () == EType_Pointer_u ? LlvmCast_ptr_u (Value, pPointerType, Offset, pResultValue) :
		LlvmCast_ptr_ptr_u (Value, pPointerType, Offset, pResultValue);
}

bool
CCast_ptr::LlvmCast_ptr (
	const CValue& Value,
	CPointerType* pType,
	size_t Offset,
	CValue* pResultValue
	)
{
	if (!Offset)
	{
		pResultValue->OverrideType (Value, pType);
		return true;
	}

	CValue OffsetValue;
	OffsetValue.SetConstSizeT (Offset, EType_Int_p);

	llvm::Value* pLlvmSafePtr = Value.GetLlvmValue ();
	llvm::Value* pLlvmPtr = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmSafePtr, 0, "sptr_p");
	pLlvmPtr = m_pModule->m_LlvmBuilder.CreateGEP (pLlvmPtr, OffsetValue.GetLlvmValue (), "sptr_p_inc");		
	pLlvmSafePtr = m_pModule->m_LlvmBuilder.CreateInsertValue (pLlvmSafePtr, pLlvmPtr, 0, "sptr");

	pResultValue->SetLlvmRegister (pLlvmSafePtr, pType);
	return true;
}

bool
CCast_ptr::LlvmCast_ptr_u (
	const CValue& Value,
	CPointerType* pType,
	size_t Offset,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmPtr = Value.GetLlvmValue ();

	if (Offset)
	{
		CValue OffsetValue;
		OffsetValue.SetConstSizeT (Offset, EType_Int_p);

		CType* pBytePtrType = m_pModule->m_TypeMgr.GetBytePtrType ();

		pLlvmPtr = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtr, pBytePtrType->GetLlvmType (), "p_cast");
		pLlvmPtr = m_pModule->m_LlvmBuilder.CreateGEP (pLlvmPtr, OffsetValue.GetLlvmValue (), "p_inc");
	}

	pLlvmPtr = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtr, pType->GetLlvmType (), "p_cast");
	pResultValue->SetLlvmRegister (pLlvmPtr, pType);
	return true;
}

bool
CCast_ptr::LlvmCast_ptr_ptr_u (
	const CValue& Value,
	CPointerType* pType,
	size_t Offset,
	CValue* pResultValue
	)
{
	llvm::Value* pLlvmSafePtr = Value.GetLlvmValue ();
	llvm::Value* pLlvmPtr = m_pModule->m_LlvmBuilder.CreateExtractValue (pLlvmSafePtr, 0, "p");

	if (Offset)
	{
		CValue OffsetValue;
		OffsetValue.SetConstSizeT (Offset, EType_Int_p);
		pLlvmPtr = m_pModule->m_LlvmBuilder.CreateGEP (pLlvmPtr, OffsetValue.GetLlvmValue (), "p_inc");		
	}

	pLlvmPtr = m_pModule->m_LlvmBuilder.CreateBitCast (pLlvmPtr, pType->GetLlvmType (), "p_cast");
	pResultValue->SetLlvmRegister (pLlvmPtr, pType);
	return true;
}

//.............................................................................

ECast
CCast_arr::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	return ECast_Lossy;
}

bool
CCast_arr::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	err::SetFormatStringError (_T("CCast_arr::ConstCast is not yet implemented"));
	return false;
}

bool
CCast_arr::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("CCast_arr::LlvmCast is not yet implemented"));
	return false;
}

//.............................................................................

ECast
CCast_arr_ptr::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	CArrayType* pArrayType = pSrcType->IsReferenceType () ? 
		(CArrayType*) ((CPointerType*) pSrcType)->GetBaseType () :
		(CArrayType*) pSrcType;

	CPointerType* pPointerType = (CPointerType*) pDstType;

	ASSERT (pArrayType->GetTypeKind () == EType_Array);
	ASSERT (pPointerType->IsPointerType ());

	CType* pArrayBaseType = pArrayType->GetBaseType ();
	CType* pPointerBaseType = pPointerType->GetBaseType ();

	return 
		pArrayBaseType->Cmp (pPointerBaseType) == 0 ? ECast_Lossless : 
		(pArrayBaseType->GetFlags () & ETypeFlag_IsPod) &&
		(pPointerBaseType->GetFlags () & ETypeFlag_IsPod) ? ECast_Lossy : ECast_None;
}

bool
CCast_arr_ptr::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	ASSERT (SrcValue.GetType ()->GetTypeKind () == EType_Array);
	ASSERT (DstValue.GetType ()->IsPointerType ());

	const CValue& SavedSrcValue = m_pModule->m_ConstMgr.SaveValue (SrcValue);
	void* p = SavedSrcValue.GetConstData ();

	if (DstValue.GetType ()->GetTypeKind () == EType_Pointer_u)
	{
		*(void**) DstValue.GetConstData () = p;
	}
	else
	{
		TSafePtr* pPtr = (TSafePtr*) DstValue.GetConstData ();
		pPtr->m_p = p;
		pPtr->m_pRegionBegin = p;
		pPtr->m_pRegionEnd = (char*) p + SrcValue.GetType ()->GetSize ();
		pPtr->m_ScopeLevel = 0;
	}

	return true;
}

bool
CCast_arr_ptr::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	if (!Value.GetType ()->IsReferenceType ())
	{
		SetCastError (Value.GetType (), pType);
		return false;
	}

	if (Value.GetValueKind () == EValue_Variable)
	{
		pResultValue->SetVariable (Value.GetVariable (), Value.GetLlvmValue (), pType);
		return true;
	}

	CArrayType* pArrayType = (CArrayType*) ((CPointerType*) Value.GetType ())->GetBaseType ();
	CPointerType* pPointerType = (CPointerType*) pType;

	ASSERT (pArrayType->GetTypeKind () == EType_Array);
	ASSERT (pPointerType->IsPointerType ());

	CValue Zero;
	Zero.SetConstSizeT (0);

	llvm::Value* LlvmIndexArray [] =
	{
		Zero.GetLlvmValue (),
		Zero.GetLlvmValue (),
	};

	llvm::Value* pLlvmValue = Value.GetLlvmValue ();
	llvm::Value* pLlvmPtr = m_pModule->m_LlvmBuilder.CreateGEP (
		pLlvmValue, 
		llvm::ArrayRef <llvm::Value*> (LlvmIndexArray, 2)
		);

	if (pType->GetTypeKind () == EType_Pointer)
		pLlvmPtr = m_pModule->m_OperatorMgr.CreateLlvmSafePtr (pLlvmPtr, pLlvmValue, pArrayType, 0);

	pResultValue->SetLlvmRegister (pLlvmPtr, pType);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
