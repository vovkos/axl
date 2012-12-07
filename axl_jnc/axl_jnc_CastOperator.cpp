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
			pResultValue->CreateConst (NULL, pType) &&
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
		TempValue.CreateConst (NULL, m_pIntermediateType) &&
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

	if (Value.GetType ()->GetLlvmType () == pType->GetLlvmType ())
	{
		pResultValue->OverrideType (Value, pType);
		return true;
	}

	m_pModule->m_LlvmBuilder.CreateBitCast (Value, pType, pResultValue);
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
	CFunctionType* pGetterType = ((CPropertyType*) pSrcType)->GetGetter ()->GetType ();
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
	ASSERT (Value.GetType ()->GetTypeKind () == EType_Property);

	CValue TmpValue;
	return 
		m_pModule->m_OperatorMgr.GetPropertyOperator (Value, &TmpValue) &&
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
	m_pModule->m_LlvmBuilder.CreateTrunc_i (Value, pType, pResultValue);
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
	m_pModule->m_LlvmBuilder.CreateExt_i (Value, pType, pResultValue);
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
	m_pModule->m_LlvmBuilder.CreateExt_u (Value, pType, pResultValue);
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

	CValue SwapFunctionValue;
	SwapFunctionValue.SetLlvmValue (pLlvmSwap, NULL);
	m_pModule->m_LlvmBuilder.CreateCall (SwapFunctionValue, Value, pType, pResultValue);
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
	m_pModule->m_LlvmBuilder.CreateTrunc_f (Value, pType, pResultValue);
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
	m_pModule->m_LlvmBuilder.CreateExt_f (Value, pType, pResultValue);
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
	m_pModule->m_LlvmBuilder.CreateIntToFp (Value, pType, pResultValue);
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
	m_pModule->m_LlvmBuilder.CreateIntToFp_u (Value, pType, pResultValue);
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
	m_pModule->m_LlvmBuilder.CreateFpToInt (Value, pType, pResultValue);
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

ECast
CCast_getbf::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	CType* pSrcBaseType = ((CBitFieldType*) pSrcType)->GetBaseType ();
	return m_pModule->m_OperatorMgr.GetCastKind (pSrcBaseType, pDstType);
}

bool
CCast_getbf::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CType* pDstType = DstValue.GetType ();
		
	CValue TmpValue;
	
	bool Result = SrcValue.GetType ()->GetSize () <= 4 ? 
		CastImpl <uint32_t, EType_Int32_u> (SrcValue, pDstType, &TmpValue) : 
		CastImpl <uint64_t, EType_Int64_u> (SrcValue, pDstType, &TmpValue);

	if (!Result)
		return false;

	memcpy (DstValue.GetConstData (), TmpValue.GetConstData (), pDstType->GetSize ());
	return true;
}

bool
CCast_getbf::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	return Value.GetType ()->GetSize () <= 4 ? 
		CastImpl <uint32_t, EType_Int32_u> (Value, pType, pResultValue) : 
		CastImpl <uint64_t, EType_Int64_u> (Value, pType, pResultValue);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ECast
CCast_setbf::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	CType* pDstBaseType = ((CBitFieldType*) pDstType)->GetBaseType ();
	return m_pModule->m_OperatorMgr.GetCastKind (pSrcType, pDstBaseType);
}

bool
CCast_setbf::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CBitFieldType* pDstType = (CBitFieldType*) DstValue.GetType ();
		
	CValue TmpValue;
	
	bool Result = SrcValue.GetType ()->GetSize () <= 4 ? 
		CastImpl <uint32_t, EType_Int32_u> (SrcValue, pDstType, &TmpValue) : 
		CastImpl <uint64_t, EType_Int64_u> (SrcValue, pDstType, &TmpValue);

	if (!Result)
		return false;

	memcpy (DstValue.GetConstData (), TmpValue.GetConstData (), pDstType->GetSize ());
	return true;
}

bool
CCast_setbf::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	return Value.GetType ()->GetSize () <= 4 ? 
		CastImpl <uint32_t, EType_Int32_u> (Value, (CBitFieldType*) pType, pResultValue) : 
		CastImpl <uint64_t, EType_Int64_u> (Value, (CBitFieldType*) pType, pResultValue);
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

void
GetPointerCastOffset (
	CType* pSrcType,
	CType* pDstType,
	intptr_t* pOffset,
	rtl::CArrayT <size_t>* pLlvmIndexArray
	)
{
	AssertPointerCastTypeValid (pSrcType, pDstType);
		
	CType* pSrcBaseType = ((CPointerType*) pSrcType)->GetBaseType ();
	CType* pDstBaseType = ((CPointerType*) pDstType)->GetBaseType ();

	if (pSrcBaseType->GetTypeKind () != EType_Struct ||
		pDstBaseType->GetTypeKind () != EType_Struct)
	{
		*pOffset = 0;
		return;
	}

	CStructType* pSrcStructType = (CStructType*) pSrcBaseType;
	CStructType* pDstStructType = (CStructType*) pDstBaseType;

	size_t Offset;
	
	*pOffset = 
		pSrcStructType->FindBaseType (pDstStructType, &Offset, pLlvmIndexArray) ? Offset :
		pDstStructType->FindBaseType (pSrcStructType, &Offset, NULL) ? -(intptr_t) Offset : 0;
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
		
	intptr_t Offset;
	GetPointerCastOffset (SrcValue.GetType (), DstValue.GetType (), &Offset, NULL);
	
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

	intptr_t Offset;

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	GetPointerCastOffset (Value.GetType (), pType, &Offset, &LlvmIndexArray);
	
	return Value.GetType ()->GetTypeKind () == EType_Pointer_u ? 
		LlvmCast_ptr_u (Value, Offset, &LlvmIndexArray, pPointerType, pResultValue) :
		LlvmCast_ptr (Value, Offset, &LlvmIndexArray, pPointerType, pResultValue);
}

bool
CCast_ptr::LlvmCast_ptr (
	const CValue& Value,
	intptr_t Offset,
	rtl::CArrayT <size_t>* pLlvmIndexArray,
	CPointerType* pPointerType,
	CValue* pResultValue
	)
{
	if (Value.GetValueKind () == EValue_Variable)
		return LlvmCast_ptr_u (Value, Offset, pLlvmIndexArray, pPointerType, pResultValue);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, &PtrValue);

	if (pPointerType->GetTypeKind () == EType_Pointer_u)
		return LlvmCast_ptr_u (PtrValue, Offset, pLlvmIndexArray, pPointerType, pResultValue);

	ASSERT (pPointerType->GetTypeKind () == EType_Pointer);

	CValue ValidatorValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 1, NULL, &ValidatorValue);

	LlvmCast_ptr_u (PtrValue, Offset, pLlvmIndexArray, pPointerType, &PtrValue);

	CValue SafePtrValue = pPointerType->GetUndefValue ();
	m_pModule->m_LlvmBuilder.CreateInsertValue (SafePtrValue, PtrValue, 0, NULL, &SafePtrValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (SafePtrValue, ValidatorValue, 1, pPointerType, pResultValue);
	return true;
}

bool
CCast_ptr::LlvmCast_ptr_u (
	const CValue& Value,
	intptr_t Offset,
	rtl::CArrayT <size_t>* pLlvmIndexArray,
	CPointerType* pPointerType,
	CValue* pResultValue
	)
{
	CValue PtrValue = Value;

	if (!pLlvmIndexArray->IsEmpty ())
	{
		pLlvmIndexArray->Insert (0, 0);
		m_pModule->m_LlvmBuilder.CreateGep (
			PtrValue, 
			*pLlvmIndexArray,
			pLlvmIndexArray->GetCount (),
			NULL, 
			&PtrValue
			);
	}
	else if (Offset)
	{
		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);
		m_pModule->m_LlvmBuilder.CreateGep (PtrValue, Offset, NULL, &PtrValue);
	}

	// make sure type is unsafe pointer (cause we cakk LlvmCast_ptr_u from LlvmCast_ptr also)

	pPointerType = pPointerType->GetBaseType ()->GetPointerType (EType_Pointer_u);
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPointerType, &PtrValue);

	if (Value.GetValueKind () == EValue_Variable)
		pResultValue->SetVariable (
			Value.GetVariable (), 
			PtrValue.GetLlvmValue (), 
			pPointerType, 
			false,
			(Value.GetFlags () & EValueFlag_IsVariableOffset) != 0 || Offset < 0
			);
	else
		*pResultValue = PtrValue;

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
		pPtr->m_Validator.m_pRegionBegin = p;
		pPtr->m_Validator.m_pRegionEnd = (char*) p + SrcValue.GetType ()->GetSize ();
		pPtr->m_Validator.m_ScopeLevel = 0;
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

	EType TypeKind = pType->GetTypeKind ();
	if (TypeKind == EType_Pointer && Value.GetValueKind () == EValue_Variable)
	{
		pResultValue->SetVariable (
			Value.GetVariable (), 
			Value.GetLlvmValue (), 
			pType, 
			true, 
			(Value.GetFlags () & EValueFlag_IsVariableOffset) != 0
			);
		return true;
	}

	CArrayType* pArrayType = (CArrayType*) ((CPointerType*) Value.GetType ())->GetBaseType ();
	ASSERT (pArrayType->GetTypeKind () == EType_Array);

	if (TypeKind == EType_Pointer_u)
	{
		m_pModule->m_LlvmBuilder.CreateGep2 (Value, 0, 0, pType, pResultValue);
		return true;
	}

	SetCastError (Value.GetType (), pType);
	return false;
}

//.............................................................................

ECast
CCast_class::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	CClassType* pSrcClassType = (CClassType*) pSrcType;
	CClassType* pDstClassType = (CClassType*) pDstType;

	ASSERT (pSrcClassType->IsClassType ());
	ASSERT (pDstClassType->IsClassType ());

	return pSrcClassType->FindBaseType (pDstClassType) ? ECast_Lossless : ECast_Lossy;
}

bool
CCast_class::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	CClassType* pSrcClassType = (CClassType*) SrcValue.GetType ();
	CClassType* pDstClassType = (CClassType*) DstValue.GetType ();

	ASSERT (pSrcClassType->IsClassType ());
	ASSERT (pDstClassType->IsClassType ());

	err::SetFormatStringError (_T("CCast_class::ConstCast is not yet implemented"));
	return false;
}

bool
CCast_class::LlvmCast (
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CClassType* pSrcClassType = (CClassType*) Value.GetType ();
	CClassType* pDstClassType = (CClassType*) pType;

	ASSERT (pSrcClassType->IsClassType ());
	ASSERT (pDstClassType->IsClassType ());

	size_t Offset;

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	bool Result = pSrcClassType->FindBaseType (
		pDstClassType, 
		&Offset, 
		&LlvmIndexArray
		);

	if (!Result)
		return m_pModule->m_LlvmBuilder.DynamicCastInterface (Value, pDstClassType, pResultValue);

	CValue PtrValue;
	CValue ScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 1, NULL, &ScopeLevelValue);

	CValue SrcNullValue = pSrcClassType->GetInterfaceStructType ()->GetPointerType (EType_Pointer_u)->GetZeroValue ();
	CValue DstNullValue = pDstClassType->GetInterfaceStructType ()->GetPointerType (EType_Pointer_u)->GetZeroValue ();

	CBasicBlock* pCmpBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	CBasicBlock* pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_phi"));
	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_nonull"));

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (PtrValue, SrcNullValue, &CmpValue);
	m_pModule->m_LlvmBuilder.CreateCondBr (CmpValue, pPhiBlock, pNoNullBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pNoNullBlock);
	
	LlvmIndexArray.Insert (0, 0);
	m_pModule->m_LlvmBuilder.CreateGep (
		PtrValue, 
		LlvmIndexArray,
		LlvmIndexArray.GetCount (),
		NULL, 
		&PtrValue
		);		

	m_pModule->m_LlvmBuilder.CreateBr (pPhiBlock);
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPhiBlock);

	CValue PhiValue;
	m_pModule->m_LlvmBuilder.CreatePhi (PtrValue, pNoNullBlock, DstNullValue, pCmpBlock, &PhiValue);
	m_pModule->m_LlvmBuilder.CreateInterface (PhiValue, ScopeLevelValue, pDstClassType, pResultValue);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
