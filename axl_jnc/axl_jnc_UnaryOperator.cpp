#include "stdafx.h"
#include "axl_jnc_UnaryOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetUnOpString (EUnOp OpKind)
{
	switch (OpKind)
	{
	case EUnOp_Minus:
		return _T("-");

	case EUnOp_BitwiseNot:
		return _T("~");

	case EUnOp_Addr:
		return _T("&");

	case EUnOp_Indir:
		return _T("*");

	case EUnOp_LogicalNot:
		return _T("!");

	case EUnOp_PreInc:
		return _T("pre-increment");

	case EUnOp_PreDec:
		return _T("pre-decrement");

	case EUnOp_PostInc:
		return _T("post-increment");

	case EUnOp_PostDec:
		return _T("post-decrement");

	default:
		return _T("undefined-unary-operator");
	};
}

//.............................................................................

IUnaryOperator::IUnaryOperator ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_OpKind = EUnOp_None;
}

//.............................................................................

void
CUnaryOperatorOverload::Clear ()
{
	m_DirectMap.Clear ();
	m_ImplicitMap.Clear ();
}

IUnaryOperator*
CUnaryOperatorOverload::GetOperator (
	CType* pOpType,
	TUnaryOperatorTypeInfo* pTypeInfo
	)
{
	bool Result;

	rtl::CStringA Signature = pOpType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, IUnaryOperator*> It = m_DirectMap.Find (Signature);
	if (!It)
		It = m_ImplicitMap.Find (Signature);

	if (It)
	{
		IUnaryOperator* pOperator = It->m_Value;
		if (!pOperator)
			return NULL;

		Result = pOperator->GetTypeInfo (pOpType, pTypeInfo);
		ASSERT (Result);
		return pOperator;
	}

	// ok, we need to enumerate all overloads

	IUnaryOperator* pBestOperator = NULL;
	TUnaryOperatorTypeInfo BestTypeInfo;

	for (It = m_DirectMap.GetHead (); It; It++)
	{
		IUnaryOperator* pOperator = It->m_Value;
		
		TUnaryOperatorTypeInfo TypeInfo;
		bool Result = pOperator->GetTypeInfo (pOpType, &TypeInfo);
		if (!Result)
			continue;

		if (!pBestOperator || TypeInfo.m_CastKind < BestTypeInfo.m_CastKind)
		{
			pBestOperator = pOperator;
			BestTypeInfo = TypeInfo;
		}
	}

	It = m_ImplicitMap.Goto (Signature); 
	It->m_Value = pBestOperator;

	if (pBestOperator)
		*pTypeInfo = BestTypeInfo;

	return pBestOperator;
}

IUnaryOperator*
CUnaryOperatorOverload::AddOperator (
	CType* pOpType,
	IUnaryOperator* pOperator
	)
{
	rtl::CStringA Signature = pOpType->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, IUnaryOperator*> It = m_DirectMap.Goto (Signature);

	IUnaryOperator* pPrevOperator = It->m_Value;
	It->m_Value = pOperator;
	return pPrevOperator;
}

//.............................................................................

bool
GetArithmeticUnaryOperatorTypeInfo (
	CModule* pModule,
	CType* pType,
	CType* pOpType,
	TUnaryOperatorTypeInfo* pTypeInfo
	)
{
	ECast CastKind = pModule->m_OperatorMgr.GetCastKind (pOpType, pType);
	if (!CastKind)
		return false;

	pTypeInfo->m_CastKind = CastKind;
	pTypeInfo->m_pOpType = pType;
	pTypeInfo->m_pReturnType = pType;
	return true;
}

bool
GetArithmeticUnaryOperatorTypeInfo (
	CModule* pModule,
	EType TypeKind,
	CType* pOpType,
	TUnaryOperatorTypeInfo* pTypeInfo
	)
{
	CType* pType = pModule->m_TypeMgr.GetBasicType (TypeKind);
	return GetArithmeticUnaryOperatorTypeInfo (pModule, pType, pOpType, pTypeInfo);
}

//.............................................................................

llvm::Value*
CUnOp_Minus::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue
	)
{
	return pModule->m_LlvmBuilder.CreateNeg (pOpValue);
}

llvm::Value*
CUnOp_Minus::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue
	)
{
	return pModule->m_LlvmBuilder.CreateFNeg (pOpValue);
}

//.............................................................................

llvm::Value*
CUnOp_BitwiseNot::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue
	)
{
	return pModule->m_LlvmBuilder.CreateNot (pOpValue);
}

//.............................................................................

bool
CUnOp_addr::GetTypeInfo (
	CType* pOpType,
	TUnaryOperatorTypeInfo* pTypeInfo
	)
{
	if (!pOpType->IsReferenceType ())
	{
		err::SetFormatStringError (_T("can only apply unary '&' to a l-value"));
		return false;
	}

	CType* pOriginalType = ((CPointerType*) pOpType)->GetBaseType ();
	if (pOriginalType->IsReferenceType ())
		return GetTypeInfo (pOriginalType, pTypeInfo);

	pTypeInfo->m_CastKind = ECast_Implicit;
	pTypeInfo->m_pOpType = pOpType;
	pTypeInfo->m_pReturnType = pOpType->GetTypeKind () == EType_Reference_u ? 
		pOriginalType->GetPointerType (EType_Pointer_u) :
		pOriginalType->GetPointerType (EType_Pointer);
	return true;
}

bool
CUnOp_addr::ConstOperator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("can only apply unary '&' to a l-value"));
	return false;
}

bool
CUnOp_addr::LlvmOperator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pOpType = OpValue.GetType ();
	if (!pOpType->IsReferenceType ())
	{
		err::SetFormatStringError (_T("can only apply unary '&' to a l-value"));
		return false;
	}

	CType* pOriginalType = ((CPointerType*) pOpType)->GetBaseType ();
	ASSERT (!pOriginalType->IsReferenceType ());

	CType* pReturnType = pOpType->GetTypeKind () == EType_Reference_u ? 
		pOriginalType->GetPointerType (EType_Pointer_u) :
		pOriginalType->GetPointerType (EType_Pointer);

	pResultValue->OverrideType (OpValue, pReturnType);
	return true;
}

//.............................................................................

bool
CUnOp_indir::GetTypeInfo (
	CType* pOpType,
	TUnaryOperatorTypeInfo* pTypeInfo
	)
{
	while (pOpType->IsReferenceType ())
		pOpType = ((CPointerType*) pOpType)->GetBaseType ();

	if (!pOpType->IsPointerType ())
	{
		err::SetFormatStringError (_T("can only apply unary '*' to a pointer"));
		return false;
	}

	CType* pOriginalType = ((CPointerType*) pOpType)->GetBaseType ();
	if (pOriginalType->IsReferenceType ())
		return GetTypeInfo (pOriginalType, pTypeInfo);

	pTypeInfo->m_CastKind = ECast_Implicit;
	pTypeInfo->m_pOpType = pOpType;
	pTypeInfo->m_pReturnType = pOpType->GetTypeKind () == EType_Pointer_u ? 
		pOriginalType->GetPointerType (EType_Reference_u) :
		pOriginalType->GetPointerType (EType_Reference);

	return true;
}

bool
CUnOp_indir::ConstOperator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("cannot apply unary '*' to a constant"));
	return false;
}

bool
CUnOp_indir::LlvmOperator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pOpType = OpValue.GetType ();
	if (!pOpType->IsPointerType ())
	{
		err::SetFormatStringError (_T("can only apply unary '*' to a pointer"));
		return false;
	}

	CType* pOriginalType = ((CPointerType*) pOpType)->GetBaseType ();
	ASSERT (!pOriginalType->IsReferenceType ());

	CType* pReturnType = pOpType->GetTypeKind () == EType_Pointer_u ? 
		pOriginalType->GetPointerType (EType_Reference_u) :
		pOriginalType->GetPointerType (EType_Reference);

	pResultValue->OverrideType (OpValue, pReturnType);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
