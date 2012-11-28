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

CType*
GetArithmeticOperatorResultTypeKind (CType* pOpType)
{
	EType TypeKind = pOpType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_Int8:
	case EType_Int16:
	case EType_Int16_be:
	case EType_Int32_be:
		TypeKind = EType_Int32;
		break;

	case EType_Int8_u:
	case EType_Int16_u:
	case EType_Int16_beu:
	case EType_Int32_beu:
		TypeKind = EType_Int32_u;
		break;
	
	case EType_Int64_be:
		TypeKind = EType_Int64;
		break;

	case EType_Int64_beu:
		TypeKind = EType_Int64_u;
		break;

	case EType_Int32:
	case EType_Int32_u:
	case EType_Int64:
	case EType_Int64_u:
	case EType_Float:
	case EType_Double:
		// no change
		break;

	default:
		return NULL;
	}

	return pOpType->GetModule ()->m_TypeMgr.GetPrimitiveType (TypeKind);
}

//.............................................................................

IUnaryOperator::IUnaryOperator ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_OpKind = EUnOp_None;
	m_Flags = 0;
}

//.............................................................................

llvm::Value*
CUnOp_Minus::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateNeg_i (OpValue, pResultType, pResultValue);
}

llvm::Value*
CUnOp_Minus::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateNeg_f (OpValue, pResultType, pResultValue);
}

//.............................................................................

llvm::Value*
CUnOp_BitwiseNot::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue,
	CType* pResultType,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateNot (OpValue, pResultType, pResultValue);
}

//.............................................................................

bool
CUnOp_Addr::Operator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pOpType = OpValue.GetType ();
	if (!pOpType->IsReferenceType ())
	{
		err::SetFormatStringError (_T("can only apply unary '&' to a reference"));
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
CUnOp_Indir::Operator (
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
