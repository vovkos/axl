#include "stdafx.h"
#include "axl_jnc_CastOp_Fp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CCast_FpTrunc::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	m_pModule->m_LlvmBuilder.CreateTrunc_f (OpValue, pType, pResultValue);
	return true;
}

//.............................................................................

bool
CCast_FpExt::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	m_pModule->m_LlvmBuilder.CreateExt_f (OpValue, pType, pResultValue);
	return true;
}

//.............................................................................

bool
CCast_FpFromInt::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	m_pModule->m_LlvmBuilder.CreateIntToFp (OpValue, pType, pResultValue);
	return true;
}

bool
CCast_FpFromInt::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	EType DstTypeKind = pType->GetTypeKind ();
	switch (DstTypeKind)
	{
	case EType_Float:
		ConstCast_Fp32 (OpValue, (float*) pDst);
		break;

	case EType_Double:
		ConstCast_Fp64 (OpValue, (double*) pDst);
		break;

	default:
		ASSERT (false);
	}

	return true;
}

void
CCast_FpFromInt::ConstCast_Fp32 (
	const CValue& OpValue,
	float* pFp32
	)
{
	const void* pSrc = OpValue.GetConstData ();
	
	size_t SrcSize = OpValue.GetType ()->GetSize ();
	switch (SrcSize)
	{
	case 1:
		*pFp32 = *(char*) pSrc;
		break;

	case 2:
		*pFp32 = *(short*) pSrc;
		break;

	case 4:
		*pFp32 = (float) *(long*) pSrc;
		break;

	case 8:
		*pFp32 = (float) *(__int64*) pSrc;
		break;

	default:
		ASSERT (false);
	}
};

void
CCast_FpFromInt::ConstCast_Fp64 (
	const CValue& OpValue,
	double* pFp64
	)
{
	const void* pSrc = OpValue.GetConstData ();
	
	size_t SrcSize = OpValue.GetType ()->GetSize ();
	switch (SrcSize)
	{
	case 1:
		*pFp64 = *(char*) pSrc;
		break;

	case 2:
		*pFp64 = *(short*) pSrc;
		break;

	case 4:
		*pFp64 = *(long*) pSrc;
		break;

	case 8:
		*pFp64 = (double) *(__int64*) pSrc;
		break;

	default:
		ASSERT (false);
	}
};

//.............................................................................

bool
CCast_FpFromInt_u::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	m_pModule->m_LlvmBuilder.CreateIntToFp_u (OpValue, pType, pResultValue);
	return true;
}

bool
CCast_FpFromInt_u::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	EType DstTypeKind = pType->GetTypeKind ();
	switch (DstTypeKind)
	{
	case EType_Float:
		ConstCast_Fp32 (OpValue, (float*) pDst);
		break;

	case EType_Double:
		ConstCast_Fp64 (OpValue, (double*) pDst);
		break;

	default:
		ASSERT (false);
	}

	return true;
}

void
CCast_FpFromInt_u::ConstCast_Fp32 (
	const CValue& OpValue,
	float* pFp32
	)
{
	const void* pSrc = OpValue.GetConstData ();
	
	size_t SrcSize = OpValue.GetType ()->GetSize ();
	switch (SrcSize)
	{
	case 1:
		*pFp32 = *(uchar_t*) pSrc;
		break;

	case 2:
		*pFp32 = *(ushort_t*) pSrc;
		break;

	case 4:
		*pFp32 = (float) *(ulong_t*) pSrc;
		break;

	case 8:
		*pFp32 = (float) *(uint64_t*) pSrc;
		break;

	default:
		ASSERT (false);
	}
};

void
CCast_FpFromInt_u::ConstCast_Fp64 (
	const CValue& OpValue,
	double* pFp64
	)
{
	const void* pSrc = OpValue.GetConstData ();
	
	size_t SrcSize = OpValue.GetType ()->GetSize ();
	switch (SrcSize)
	{
	case 1:
		*pFp64 = *(uchar_t*) pSrc;
		break;

	case 2:
		*pFp64 = *(ushort_t*) pSrc;
		break;

	case 4:
		*pFp64 = *(ulong_t*) pSrc;
		break;

	case 8:
		*pFp64 = (double) *(uint64_t*) pSrc;
		break;

	default:
		ASSERT (false);
	}
};

//.............................................................................

bool
CCast_FpFromBeInt::GetCastOperators (
	const CValue& OpValue,
	CType* pType,
	ICastOperator** ppFirstOperator,
	ICastOperator** ppSecondOperator,
	CType** ppIntermediateType
	)
{
	ASSERT (IsBigEndianIntegerTypeKind (OpValue.GetType ()->GetTypeKind ()));

	EType IntermediateTypeKind = GetLittleEndianIntegerTypeKind (OpValue.GetType ()->GetTypeKind ());

	*ppFirstOperator = m_pModule->m_OperatorMgr.GetStdCastOperator (EStdCast_SwapByteOrder);
	*ppSecondOperator = m_pModule->m_OperatorMgr.GetStdCastOperator (EStdCast_Fp);
	*ppIntermediateType = m_pModule->m_TypeMgr.GetPrimitiveType (IntermediateTypeKind);
	return true;
}

//.............................................................................

bool
CCast_FpFromEnum::GetCastOperators (
	const CValue& OpValue,
	CType* pType,
	ICastOperator** ppFirstOperator,
	ICastOperator** ppSecondOperator,
	CType** ppIntermediateType
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_Enum);

	CType* pIntermediateType = ((CEnumType*) OpValue.GetType ())->GetBaseType ();

	*ppFirstOperator = m_pModule->m_OperatorMgr.GetStdCastOperator (EStdCast_Copy);
	*ppSecondOperator = m_pModule->m_OperatorMgr.GetStdCastOperator (EStdCast_Fp);
	*ppIntermediateType = pIntermediateType;
	return true;
}

//.............................................................................

ICastOperator*
CCast_Fp::GetCastOperator (
	const CValue& OpValue,
	CType* pType
	)
{
	CType* pSrcType = OpValue.GetType ();

	EType SrcTypeKind = pSrcType->GetTypeKind ();
	EType DstTypeKind = pType->GetTypeKind ();

	size_t SrcSize = pSrcType->GetSize ();
	size_t DstSize = pType->GetSize ();

	ASSERT (DstTypeKind == EType_Float || DstTypeKind == EType_Double);

	switch (SrcTypeKind)
	{
	case EType_Int8:
	case EType_Int16:
	case EType_Int32:
	case EType_Int64:
		return &m_FromInt;

	case EType_Bool:
	case EType_Int8_u:
	case EType_Int16_u:
	case EType_Int32_u:
	case EType_Int64_u:
		return &m_FromInt_u;

	case EType_Int16_be:
	case EType_Int16_beu:
	case EType_Int32_be:
	case EType_Int32_beu:
	case EType_Int64_be:
	case EType_Int64_beu:
		return &m_FromBeInt;

	case EType_Float:
	case EType_Double:
		return 
			SrcSize == DstSize ? m_pModule->m_OperatorMgr.GetStdCastOperator (EStdCast_Copy) : 
			SrcSize > DstSize ? (ICastOperator*) &m_Trunc : 
			(ICastOperator*) &m_Ext;

	case EType_Enum:
		return &m_FromEnum;

	default:
		return NULL;
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
