#include "stdafx.h"
#include "axl_jnc_ControlFlowMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CControlFlowMgr::CControlFlowMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
}

void
CControlFlowMgr::Clear ()
{
	m_BlockList.Clear ();
	m_pCurrentBlock = NULL;
}

CBasicBlock* 
CControlFlowMgr::CreateBlock (const rtl::CString& Name)
{
	CBasicBlock* pBlock = AXL_MEM_NEW (CBasicBlock);
	pBlock->m_Name = Name;
	pBlock->m_pLlvmBlock = llvm::BasicBlock::Create (
		llvm::getGlobalContext (), 
		(const tchar_t*) Name,
		NULL
		);

	m_BlockList.InsertTail (pBlock);
	return pBlock;
}

CBasicBlock*
CControlFlowMgr::SetCurrentBlock (CBasicBlock* pBlock)
{
	if (m_pCurrentBlock == pBlock)
		return pBlock;

	CBasicBlock* pPrevCurrentBlock = m_pCurrentBlock;

	m_pCurrentBlock = pBlock;
	m_pModule->m_LlvmBuilder.SetInsertPoint (pBlock);

	if (pBlock->m_pFunction)
		return pPrevCurrentBlock;
	
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	pFunction->GetLlvmFunction ()->getBasicBlockList ().push_back (pBlock->m_pLlvmBlock);
	pBlock->m_pFunction = pFunction;
	return pPrevCurrentBlock;
}

void
CControlFlowMgr::MarkUnreachable (CBasicBlock* pBlock)
{
	if (pBlock->m_Flags & EBasicBlockFlag_IsUnreachable)
		return;

	pBlock->m_Flags |= EBasicBlockFlag_IsUnreachable;

	CBasicBlock* pPrevCurrentBlock = SetCurrentBlock (pBlock);
	m_pModule->m_LlvmBuilder.CreateUnreachable ();
	SetCurrentBlock (pPrevCurrentBlock);
}

void
CControlFlowMgr::Jump (
	CBasicBlock* pBlock,
	CBasicBlock* pFollowBlock
	)
{
	m_pCurrentBlock->m_JumpArray.Append (pBlock);

	m_pModule->m_LlvmBuilder.CreateBr (pBlock);
	pBlock->m_Flags |= EBasicBlockFlag_IsJumped;

	bool IsUnreachable = pFollowBlock == NULL;

	if (IsUnreachable) 
		pFollowBlock = CreateBlock (_T("jmp_follow"));

	SetCurrentBlock (pFollowBlock);

	if (IsUnreachable)
		MarkUnreachable (pFollowBlock);
}

bool
CControlFlowMgr::ConditionalJump (
	const CValue& Value,
	CBasicBlock* pThenBlock,
	CBasicBlock* pElseBlock
	)
{
	CValue BoolValue;
	bool Result = m_pModule->m_OperatorMgr.CastOperator (Value, EType_Bool, &BoolValue);
	if (!Result)
		return false;

	m_pCurrentBlock->m_JumpArray.Append (pThenBlock);
	m_pCurrentBlock->m_JumpArray.Append (pElseBlock);

	pThenBlock->m_Flags |= EBasicBlockFlag_IsJumped;
	pElseBlock->m_Flags |= EBasicBlockFlag_IsJumped;

	m_pModule->m_LlvmBuilder.CreateCondBr (BoolValue, pThenBlock, pElseBlock);
	SetCurrentBlock (pThenBlock);
	return true;
}

bool
CControlFlowMgr::Break (size_t Level)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.FindBreakScope (Level);
	if (!pScope)
	{
		err::SetFormatStringError (_T("illegal break"));
		return false;
	}

	Jump (pScope->m_pBreakBlock, NULL);
	return true;
}

bool
CControlFlowMgr::Continue (size_t Level)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.FindBreakScope (Level);
	if (!pScope)
	{
		err::SetFormatStringError (_T("illegal continue"));
		return false;
	}

	Jump (pScope->m_pContinueBlock, NULL);
	return true;
}

bool
CControlFlowMgr::Return (const CValue& Value)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	CFunctionType* pFunctionType = pFunction->GetType ();
	CType* pReturnType = pFunctionType->GetReturnType ();

	if (Value.GetValueKind () == EValue_Void)
	{
		if (pFunction->GetType ()->GetReturnType ()->GetTypeKind () != EType_Void)
		{
			err::SetFormatStringError (_T("function '%s' must return a '%s' value"), pFunction->GetTag (), pReturnType->GetTypeString ());
			return false;
		}

		m_pModule->m_LlvmBuilder.CreateRet ();
	}
	else
	{
		CValue ReturnValue;
		bool Result = m_pModule->m_OperatorMgr.CastOperator (Value, pReturnType, &ReturnValue);
		if (!Result)
			return false;		

		if (pReturnType->GetTypeKind () == EType_Pointer)
		{
			Result = m_pModule->m_LlvmBuilder.CheckSafePtrScope (ReturnValue, NULL);
			if (!Result)
				return false;

			if (ReturnValue.GetValueKind () == EValue_Variable)
				m_pModule->m_LlvmBuilder.CreateSafePtr (
					ReturnValue, 
					ReturnValue.GetVariable (),
					(CPointerType*) pReturnType,
					&ReturnValue
					);
		}

		m_pModule->m_LlvmBuilder.CreateRet (ReturnValue);
	}

	m_pCurrentBlock->m_Flags |= EBasicBlockFlag_IsHasReturnReady;
	m_pCurrentBlock->m_HasReturn = EHasReturn_Explicit;

	CBasicBlock* pFollowBlock = CreateBlock (_T("ret_follow"));
	SetCurrentBlock (pFollowBlock);
	MarkUnreachable (pFollowBlock);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
