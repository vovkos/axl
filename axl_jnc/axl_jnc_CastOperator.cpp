#include "stdafx.h"
#include "axl_jnc_CastOperator.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Closure.h"

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
			ConstCast (OpValue, *pResultValue);
	}

	return LlvmCast (AllocKind, OpValue, pType, pResultValue);
}

bool
ICastOperator::ConstCast (
	const CValue& SrcValue,
	const CValue& DstValue
	)
{
	// fail by default; if const-cast is supported then override and implement

	err::SetFormatStringError (
		_T("cannot convert constant from '%s' to '%s'"),
		SrcValue.GetType ()->GetTypeString (),
		DstValue.GetType ()->GetTypeString ()
		);
	return false;
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
	 
	return min (FirstCastKind, SecondCastKind);
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
	EAlloc AllocKind,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue TempValue;
	return 
		m_pFirst->LlvmCast (AllocKind, Value, m_pIntermediateType, &TempValue) &&
		m_pSecond->LlvmCast (AllocKind, TempValue, pType, pResultValue);
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
	EAlloc AllocKind,
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
CCast_load::LlvmCast (
	EAlloc AllocKind,
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
	ASSERT (pSrcType->IsPropertyType ());

	CPropertyType* pPropertyType = pSrcType->GetTypeKind () == EType_Property ? 
		(CPropertyType*) pSrcType :
		((CPropertyPointerType*) pSrcType)->GetPropertyType ();

	CFunctionType* pGetterType = pPropertyType->GetGetter ()->GetType ();
	return m_pModule->m_OperatorMgr.GetCastKind (pGetterType->GetReturnType (), pDstType);
}

bool
CCast_getp::LlvmCast (
	EAlloc AllocKind,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (Value.GetType ()->IsPropertyType ());

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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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
	m_pModule->m_LlvmBuilder.CreateCall (
		SwapFunctionValue, 
		ECallConv_Default,
		&Value, 1,
		pType, 
		pResultValue
		);

	return true;
}

//.............................................................................

bool
CCast_f64_f32::LlvmCast (
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue ZeroValue = Value.GetType ()->GetZeroValue ();
	return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Ne, Value, ZeroValue, pResultValue);
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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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

intptr_t
GetPointerCastOffset (
	CType* pSrcType,
	CType* pDstType,
	CStructBaseTypeCoord* pCoord
	)
{
	AssertPointerCastTypeValid (pSrcType, pDstType);
		
	CType* pSrcBaseType = ((CPointerType*) pSrcType)->GetBaseType ();
	CType* pDstBaseType = ((CPointerType*) pDstType)->GetBaseType ();

	if (pSrcBaseType->GetTypeKind () != EType_Struct ||
		pDstBaseType->GetTypeKind () != EType_Struct)
	{
		return 0;
	}

	CStructType* pSrcStructType = (CStructType*) pSrcBaseType;
	CStructType* pDstStructType = (CStructType*) pDstBaseType;

	if (pSrcStructType->FindBaseType (pDstStructType, pCoord))
		return pCoord->m_Offset;

	CStructBaseTypeCoord Coord;
	if (pDstStructType->FindBaseType (pSrcStructType, &Coord))
		return -(intptr_t) Coord.m_Offset;

	return 0;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		
	CStructBaseTypeCoord Coord;
	intptr_t Offset = GetPointerCastOffset (SrcValue.GetType (), DstValue.GetType (), &Coord);
	
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
	EAlloc AllocKind,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	AssertPointerCastTypeValid (Value.GetType (), pType);
	
	CPointerType* pPointerType = (CPointerType*) pType;

	CStructBaseTypeCoord Coord;
	intptr_t Offset = GetPointerCastOffset (Value.GetType (), pType, &Coord);
	
	return Value.GetType ()->GetTypeKind () == EType_Pointer_u ? 
		LlvmCast_ptr_u (Value, Offset, &Coord.m_LlvmIndexArray, pPointerType, pResultValue) :
		LlvmCast_ptr (Value, Offset, &Coord.m_LlvmIndexArray, pPointerType, pResultValue);
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

	// make sure type is unsafe pointer (cause we call LlvmCast_ptr_u from LlvmCast_ptr also)

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
	EAlloc AllocKind,
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
	EAlloc AllocKind,
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
CCast_fn::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	ASSERT (pSrcType->IsFunctionType () && pDstType->IsFunctionType ());

	return 
		pSrcType->GetTypeKind () == EType_FunctionPointer && pDstType->IsUnsafeFunctionPointerType () ? ECast_None :
		ECast_Lossy;
}

bool
CCast_fn::LlvmCast (
	EAlloc AllocKind,
	const CValue& RawOpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue OpValue = RawOpValue;
	CType* pOpType = OpValue.GetType ();

	ASSERT (pOpType->IsFunctionType () && pType->IsFunctionType ());

	if (pType->IsUnsafeFunctionPointerType ())
	{
		if (!pOpType->IsUnsafeFunctionPointerType ())
		{
			err::SetFormatStringError (_T("safe function pointer cannot be cast to unsafe function pointer"));
			return false;
		}

		m_pModule->m_LlvmBuilder.CreateBitCast (OpValue, pType, pResultValue);
		return true;
	}

	if (pOpType->GetTypeKind () == EType_FunctionPointer)
	{
		m_pModule->m_OperatorMgr.NormalizeFunctionPointer (&OpValue);
		pOpType = OpValue.GetType ();
	}

	ASSERT (pOpType->IsUnsafeFunctionPointerType ());
	ASSERT (pType->GetTypeKind () == EType_FunctionPointer);

	CFunctionPointerType* pDstFunctionPtrType = (CFunctionPointerType*) pType;
	CFunctionType* pSrcFunctionType = pOpType->GetFunctionType ();
	CFunctionType* pSrcShortFunctionType = pSrcFunctionType;
	CFunctionType* pDstShortFunctionType = pDstFunctionPtrType->GetShortFunctionType ();

	CClosure* pClosure = OpValue.GetClosure ();
	if (pClosure)
		pSrcShortFunctionType = pClosure->GetFunctionClosureType (pSrcFunctionType);
	
	ECast CastKind = m_pModule->m_OperatorMgr.GetFunctionCastKind (
		pSrcShortFunctionType,
		pDstShortFunctionType
		);

	if (!CastKind)
	{
		SetCastError (pType, pType);
		return false;
	}

	// case 1: no conversion required, no closure object needs to be created

	CValue IfaceValue;

	if (pSrcShortFunctionType->Cmp (pDstShortFunctionType) == 0 &&
		pClosure && 
		pClosure->IsSimpleClassMemberClosure (&IfaceValue))
	{
		return LlvmCast_case1 (
			OpValue,
			IfaceValue,
			pSrcFunctionType,
			pDstFunctionPtrType,
			pResultValue
			);
	}

	// case 2: conversion is required, but no closure object needs to be created

	if (OpValue.GetValueKind () == EValue_Function)
	{
		CFunction* pFunction = OpValue.GetFunction ();

		if (!pClosure) 			
			return LlvmCast_case2 (
				pFunction,
				m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr)->GetZeroValue (),
				pDstFunctionPtrType,
				pResultValue
				);

		if (pClosure->IsSimpleClassMemberClosure (&IfaceValue))
			return LlvmCast_case2 (
				pFunction,
				IfaceValue,
				pDstFunctionPtrType,
				pResultValue
				);
	}

	// case 3: closure object needs to be created (so conversion is required even if function signatures match)

	// detach closure from OpValue

	ref::CPtrT <CClosure> Closure = OpValue.GetClosure (); 
	OpValue.SetClosure (NULL);

	return LlvmCast_case3 (
		AllocKind,
		OpValue,
		pClosure,
		pSrcFunctionType,
		pDstFunctionPtrType,
		pResultValue
		);
}

bool
CCast_fn::LlvmCast_case1 (
	const CValue& PfnValue,
	const CValue& RawIfaceValue,
	CFunctionType* pSrcFunctionType,
	CFunctionPointerType* pFunctionPtrType,
	CValue* pResultValue
	)
{
	rtl::CArrayT <CType*> ArgTypeArray = pSrcFunctionType->GetArgTypeArray ();
	ASSERT (!ArgTypeArray.IsEmpty ());

	CType* pIfaceArgType = ArgTypeArray [0];	
	ASSERT (pIfaceArgType->IsClassType ());

	CValue IfaceValue;
	bool Result = m_pModule->m_OperatorMgr.CastOperator (RawIfaceValue, pIfaceArgType, &IfaceValue);
	if (!Result)
		return false;

	m_pModule->m_LlvmBuilder.CreateFunctionPointer (PfnValue, IfaceValue, pFunctionPtrType, pResultValue);
	return true;
}

bool
CCast_fn::LlvmCast_case2 (
	CFunction* pFunction,
	const CValue& IfaceValue,
	CFunctionPointerType* pFunctionPtrType,
	CValue* pResultValue
	)
{
	CFunction* pDstFunction = m_pModule->m_FunctionMgr.GetThunkFunction (
		pFunction->GetType (), 
		pFunction,
		pFunctionPtrType
		);

	if (!pDstFunction)
		return false;

	m_pModule->m_LlvmBuilder.CreateFunctionPointer (pDstFunction, IfaceValue, pFunctionPtrType, pResultValue);
	return true;
}

bool
CCast_fn::LlvmCast_case3 (
	EAlloc AllocKind,
	const CValue& PfnValue,
	CClosure* pClosure,
	CFunctionType* pSrcFunctionType,
	CFunctionPointerType* pFunctionPtrType,
	CValue* pResultValue
	)
{
	bool Result;

	CClassType* pClosureType = m_pModule->m_TypeMgr.CreateUnnamedClassType (EType_Class);

	CFunction* pFunction = NULL;
	
	if (PfnValue.GetValueKind () == EValue_Function)
	{
		pFunction = PfnValue.GetFunction ();
	}
	else 
	{
		ASSERT (PfnValue.GetType ()->IsUnsafeFunctionPointerType ());
		pClosureType->CreateFieldMember (pSrcFunctionType->GetPointerType (EType_Pointer_u));
	}

	char Buffer [256];
	rtl::CArrayT <size_t> ClosureMap (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	const rtl::CBoxListT <CValue>* pClosureArgList = NULL;
	
	if (pClosure)
	{
		pClosureArgList = pClosure->GetArgList ();
		ClosureMap.Reserve (pClosureArgList->GetCount ());

		rtl::CBoxIteratorT <CValue> ClosureArg = pClosureArgList->GetHead ();
		rtl::CArrayT <CType*> SrcArgTypeArray = pSrcFunctionType->GetArgTypeArray ();
	
		for (size_t i = 0; ClosureArg; ClosureArg++, i++)
		{
			if (ClosureArg->IsEmpty ())
				continue;

			ASSERT (i < SrcArgTypeArray.GetCount ());
			ClosureMap.Append (i);
			pClosureType->CreateFieldMember (SrcArgTypeArray [i]);
		}
	}

	Result = pClosureType->CalcLayout ();
	if (!Result)
		return false;

	CFunction* pDstFunction = m_pModule->m_FunctionMgr.GetThunkFunction (
		pSrcFunctionType,
		pFunction, 
		pClosureType,
		ClosureMap,
		pFunctionPtrType
		);

	if (!pDstFunction)
		return false;
	
	CValue ClosureValue;
	Result = m_pModule->m_OperatorMgr.NewOperator (AllocKind, pClosureType, &ClosureValue);
	if (!Result)
		return false;

	// save pfn & arguments in the closure

	rtl::CIteratorT <CClassFieldMember> ClosureMember = pClosureType->GetFirstFieldMember ();	
	if (!pFunction)
	{
		Result = m_pModule->m_OperatorMgr.SetClassFieldMemberValue (
			ClosureValue, 
			*ClosureMember, 
			PfnValue
			);

		if (!Result)
			return false;

		ClosureMember++;
	}

	if (pClosure)
	{
		rtl::CBoxIteratorT <CValue> ClosureArg = pClosureArgList->GetHead ();
		for (; ClosureArg; ClosureArg++)
		{
			if (ClosureArg->IsEmpty ())
				continue;

			ASSERT (ClosureMember);

			Result = m_pModule->m_OperatorMgr.SetClassFieldMemberValue (
				ClosureValue, 
				*ClosureMember, 
				*ClosureArg
				);

			if (!Result)
				return false;

			ClosureMember++;
		}
	}

	m_pModule->m_LlvmBuilder.CreateFunctionPointer (pDstFunction, ClosureValue, pFunctionPtrType, pResultValue);
	return true;
}

//.............................................................................

ECast
CCast_prop::GetCastKind (
	CType* pSrcType,
	CType* pDstType
	)
{
	ASSERT (pDstType->GetTypeKind () == EType_PropertyPointer);

	return ECast_Lossy;
}

bool
CCast_prop::LlvmCast (
	EAlloc AllocKind,
	const CValue& RawValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (RawValue.GetType ()->IsPropertyType () && pType->GetTypeKind () == EType_PropertyPointer);

	CPropertyPointerType* pPropertyPointerType = (CPropertyPointerType*) pType;
	CPropertyType* pDstPropertyType = pPropertyPointerType->GetPropertyType ();

	CValue Value = RawValue;
	if (Value.GetValueKind () != EValue_Property)
	{
		SetCastError (Value.GetType (), pType);
		return false;
	}

	ASSERT (Value.GetType ()->GetTypeKind () == EType_Property);
	CPropertyType* pSrcPropertyType = (CPropertyType*) Value.GetType ();

	CClosure* pClosure = Value.GetClosure ();
	if (!pClosure)
	{
		if (pDstPropertyType->CmpAccessorTypes (pSrcPropertyType) != 0)
		{
			SetCastError (pSrcPropertyType, pType);
			return false;
		}

		m_pModule->m_LlvmBuilder.CreatePropertyPointer (
			Value,
			m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr)->GetZeroValue (),
			pPropertyPointerType, 
			pResultValue
			);

		return true;	
	}

	CValue ClosureArgValue = *pClosure->GetArgList ()->GetHead ();
	if (!ClosureArgValue.GetType ()->IsClassType ())
	{
		err::SetFormatStringError (_T("complex closures are not supported yet"));
		return false;
	}

	if (pDstPropertyType->CmpShortAccessorTypes (pSrcPropertyType) != 0)
	{
		SetCastError (pSrcPropertyType, pType);
		return false;
	}

	CFunction* pGetter = pSrcPropertyType->GetGetter ();
	if (pGetter->GetFunctionKind () == EFunction_Method)
	{
		bool Result = m_pModule->m_OperatorMgr.GetMemberProperty (pSrcPropertyType, pClosure, &Value);
		if (!Result)
			return false;
	}

	CClassType* pClosureClassType = (CClassType*) ClosureArgValue.GetType ();
	CClassType* pMethodClassType = pSrcPropertyType->GetParentClassType ();

	CValue PtrValue;
	CValue ScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ClosureArgValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (ClosureArgValue, 1, NULL, &ScopeLevelValue);

	CClassBaseTypeCoord Coord;
	pClosureClassType->FindBaseType (pMethodClassType, &Coord);

	rtl::CArrayT <size_t> LlvmIndexArray = Coord.m_FieldCoord.m_LlvmIndexArray;
	if (!LlvmIndexArray.IsEmpty ())
	{
		LlvmIndexArray.Insert (0, 0);
	
		m_pModule->m_LlvmBuilder.CreateGep (
			PtrValue, 
			LlvmIndexArray, 
			LlvmIndexArray.GetCount (),
			NULL, 
			&PtrValue
			);
	}
	
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);
	
	CValue InterfaceValue = m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr)->GetUndefValue ();
	m_pModule->m_LlvmBuilder.CreateInsertValue (InterfaceValue, PtrValue, 0, NULL, &InterfaceValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (InterfaceValue, ScopeLevelValue, 1, NULL, &InterfaceValue);

	CValue PropertyPointerValue = pType->GetUndefValue ();
	m_pModule->m_LlvmBuilder.CreateBitCast (Value, pDstPropertyType->GetVTableStructType ()->GetPointerType (EType_Pointer_u), &PtrValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (PropertyPointerValue, PtrValue, 0, NULL, &PropertyPointerValue);
	m_pModule->m_LlvmBuilder.CreateInsertValue (PropertyPointerValue, InterfaceValue, 1, pType, pResultValue);
	return true;
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
CCast_class::LlvmCast (
	EAlloc AllocKind,
	const CValue& Value,
	CType* pType,
	CValue* pResultValue
	)
{
	CClassType* pSrcClassType = (CClassType*) Value.GetType ();
	CClassType* pDstClassType = (CClassType*) pType;

	ASSERT (pSrcClassType->IsClassType ());
	ASSERT (pDstClassType->IsClassType ());

	CClassBaseTypeCoord Coord;
	bool Result = pSrcClassType->FindBaseType (pDstClassType, &Coord);
	if (!Result)
		return m_pModule->m_LlvmBuilder.DynamicCastInterface (Value, pDstClassType, pResultValue);

	CValue SrcNullValue = pSrcClassType->GetZeroValue ();
	CValue DstNullValue = pDstClassType->GetZeroValue ();

	CBasicBlock* pCmpBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	CBasicBlock* pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_phi"));
	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_nonull"));

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (Value, SrcNullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pPhiBlock, pNoNullBlock, pNoNullBlock);
	
	Coord.m_FieldCoord.m_LlvmIndexArray.Insert (0, 0);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (
		Value, 
		Coord.m_FieldCoord.m_LlvmIndexArray,
		Coord.m_FieldCoord.m_LlvmIndexArray.GetCount (),
		NULL, 
		&PtrValue
		);		

	m_pModule->m_ControlFlowMgr.Follow (pPhiBlock);

	m_pModule->m_LlvmBuilder.CreatePhi (PtrValue, pNoNullBlock, DstNullValue, pCmpBlock, pResultValue);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
