#include "stdafx.h"
#include "axl_jnc_BinaryOperator.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetBinOpString (EBinOp OpKind)
{
	switch (OpKind)
	{
	case EBinOp_Add:
		return _T("+");

	case EBinOp_Sub:
		return _T("-");

	case EBinOp_Mul:
		return _T("*");

	case EBinOp_Div:
		return _T("/");

	case EBinOp_Mod:
		return _T("%");

	case EBinOp_Shl:
		return _T("<<");

	case EBinOp_Shr:
		return _T(">>");

	case EBinOp_BitwiseAnd:
		return _T("&");

	case EBinOp_BitwiseOr:
		return _T("|");

	case EBinOp_BitwiseXor:
		return _T("^");

	case EBinOp_Eq:
		return _T("==");

	case EBinOp_Ne:
		return _T("!=");

	case EBinOp_Lt:
		return _T("<");

	case EBinOp_Le:
		return _T("<=");

	case EBinOp_Gt:
		return _T(">");

	case EBinOp_Ge:
		return _T(">=");

	case EBinOp_LogicalAnd:
		return _T("&&");

	case EBinOp_LogicalOr:
		return _T("||");

	case EBinOp_Idx:
		return _T("index-operator");

	default:
		return _T("undefined-binary-operator");
	};
}

//.............................................................................

IBinaryOperator::IBinaryOperator()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_OpKind = EBinOp_None;
}

//.............................................................................

void
CBinaryOperatorOverload::Clear ()
{
	m_SignatureCache.Clear ();
	m_DirectMap.Clear ();
	m_ImplicitMap.Clear ();
}

IBinaryOperator*
CBinaryOperatorOverload::GetOperator (
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	)
{
	bool Result;

	rtl::CStringA Signature = pOpType1->GetSignature () + pOpType2->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, IBinaryOperator*> It = m_DirectMap.Find (Signature);
	if (!It)
		It = m_ImplicitMap.Find (Signature);
	
	if (It)
	{
		IBinaryOperator* pOperator = It->m_Value;
		if (!pOperator)
			return NULL;

		Result = pOperator->GetTypeInfo (pOpType1, pOpType2, pTypeInfo);
		ASSERT (Result);
		return pOperator;
	}

	// ok, we need to enumerate all overloads

	IBinaryOperator* pBestOperator = NULL;
	TBinaryOperatorTypeInfo BestTypeInfo;

	for (It = m_DirectMap.GetHead (); It; It++)
	{
		IBinaryOperator* pOperator = It->m_Value;
		
		TBinaryOperatorTypeInfo TypeInfo;
		bool Result = pOperator->GetTypeInfo (pOpType1, pOpType2, &TypeInfo);
		if (!Result)
			continue;

		if (!pBestOperator || TypeInfo.m_CastKind < BestTypeInfo.m_CastKind)
		{
			pBestOperator = pOperator;
			BestTypeInfo = TypeInfo;
		}	
	}

	m_SignatureCache.InsertTail (Signature);
	It = m_ImplicitMap.Goto (Signature); 
	It->m_Value = pBestOperator;

	if (pBestOperator)
		*pTypeInfo = BestTypeInfo;

	return pBestOperator;
}

IBinaryOperator*
CBinaryOperatorOverload::AddOperator (
	CType* pOpType1,
	CType* pOpType2,
	IBinaryOperator* pOperator
	)
{
	rtl::CStringA Signature = pOpType1->GetSignature () + pOpType2->GetSignature ();
	rtl::CHashTableMapIteratorT <const tchar_t*, IBinaryOperator*> It = m_DirectMap.Goto (Signature);

	IBinaryOperator* pPrevOperator = It->m_Value;
	if (!pPrevOperator)
		m_SignatureCache.InsertTail (Signature);

	It->m_Value = pOperator;
	return pPrevOperator;
}

//.............................................................................

bool
GetArithmeticBinaryOperatorTypeInfo (
	CModule* pModule,
	CType* pType,
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	)
{
	ECast CastKind1 = pModule->m_OperatorMgr.GetCastKind (pOpType1, pType);
	ECast CastKind2 = pModule->m_OperatorMgr.GetCastKind (pOpType2, pType);

	if (!CastKind1 || !CastKind2)
		return false;

	pTypeInfo->m_CastKind = min (CastKind1, CastKind2);
	pTypeInfo->m_pOpType1 = pType;
	pTypeInfo->m_pOpType2 = pType;
	pTypeInfo->m_pReturnType = pType;
	return true;
}

bool
GetArithmeticBinaryOperatorTypeInfo (
	CModule* pModule,
	EType TypeKind,
	CType* pOpType1,
	CType* pOpType2,
	TBinaryOperatorTypeInfo* pTypeInfo
	)
{
	CType* pType = pModule->m_TypeMgr.GetBasicType (TypeKind);
	return GetArithmeticBinaryOperatorTypeInfo (pModule, pType, pOpType1, pOpType2, pTypeInfo);
}

//.............................................................................
	
llvm::Value*
CBinOp_Add::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateAdd (pOpValue1, pOpValue2);
}

	
llvm::Value*
CBinOp_Add::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFAdd (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_Sub::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateSub (pOpValue1, pOpValue2);
}

	
llvm::Value*
CBinOp_Sub::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFSub (pOpValue1, pOpValue2);
}

//.............................................................................
	
llvm::Value*
CBinOp_Mul::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateMul (pOpValue1, pOpValue2);
}

	
llvm::Value*
CBinOp_Mul::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFMul (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_Div::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateSDiv (pOpValue1, pOpValue2);
}
	
llvm::Value*
CBinOp_Div::LlvmOpFp (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateFDiv (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_Mod::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateSRem (pOpValue1, pOpValue2);
}

//.............................................................................
	
llvm::Value*
CBinOp_DivU::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateUDiv (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_ModU::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateURem (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_Shl::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateShl (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_Shr::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateLShr (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseAnd::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateAnd (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseOr::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateOr (pOpValue1, pOpValue2);
}

//.............................................................................

llvm::Value*
CBinOp_BitwiseXor::LlvmOpInt (
	CModule* pModule,
	llvm::Value* pOpValue1,
	llvm::Value* pOpValue2
	)
{
	return pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateXor (pOpValue1, pOpValue2);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
