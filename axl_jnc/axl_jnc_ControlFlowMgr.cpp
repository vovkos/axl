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

	m_Flags = 0;
	m_pCurrentBlock = NULL;
	m_pReturnBlock = NULL;
	m_pSilentReturnBlock = NULL;
	m_pUnreachableBlock = NULL;
}

void
CControlFlowMgr::Clear ()
{
	m_BlockList.Clear ();
	m_pCurrentBlock = NULL;
	m_pReturnBlock = NULL;
	m_pSilentReturnBlock = NULL;
	m_pUnreachableBlock = NULL;
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
	if (!pBlock)
		return pPrevCurrentBlock;

	m_pModule->m_LlvmBuilder.SetInsertPoint (pBlock);

	if (pBlock->m_pFunction)
		return pPrevCurrentBlock;
	
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	pFunction->GetLlvmFunction ()->getBasicBlockList ().push_back (pBlock->m_pLlvmBlock);
	pBlock->m_pFunction = pFunction;
	return pPrevCurrentBlock;
}

CBasicBlock*
CControlFlowMgr::GetUnreachableBlock ()
{
	if (m_pUnreachableBlock)
		return m_pUnreachableBlock;

	m_pUnreachableBlock = CreateBlock (_T("unreachable_block"));
	MarkUnreachable (m_pUnreachableBlock);
	return m_pUnreachableBlock;
}

void
CControlFlowMgr::MarkUnreachable (CBasicBlock* pBlock)
{
	ASSERT (!(pBlock->m_Flags & EBasicBlockFlag_Jumped));
	pBlock->m_Flags |= EBasicBlockFlag_Unreachable;

	if (!pBlock->HasTerminator ())
	{
		CBasicBlock* pPrevCurrentBlock = SetCurrentBlock (pBlock);
		m_pModule->m_LlvmBuilder.CreateUnreachable ();
		SetCurrentBlock (pPrevCurrentBlock);
	}
}

void
CControlFlowMgr::Jump (
	CBasicBlock* pBlock,
	CBasicBlock* pFollowBlock
	)
{
	m_Flags |= EControlFlowFlag_HasJump;
	pBlock->m_Flags |= EBasicBlockFlag_Jumped;

	m_pModule->m_LlvmBuilder.CreateBr (pBlock);

	if (!pFollowBlock)
		pFollowBlock = GetUnreachableBlock ();

	SetCurrentBlock (pFollowBlock);
}

void
CControlFlowMgr::Follow (CBasicBlock* pBlock)
{
	if (!m_pCurrentBlock->HasTerminator ())
	{
		m_pModule->m_LlvmBuilder.CreateBr (pBlock);
		pBlock->m_Flags |= EBasicBlockFlag_Jumped;
	}

	SetCurrentBlock (pBlock);
}

bool
CControlFlowMgr::ConditionalJump (
	const CValue& Value,
	CBasicBlock* pThenBlock,
	CBasicBlock* pElseBlock,
	CBasicBlock* pFollowBlock
	)
{
	CValue BoolValue;
	bool Result = m_pModule->m_OperatorMgr.CastOperator (Value, EType_Bool, &BoolValue);
	if (!Result)
		return false;

	m_Flags |= EControlFlowFlag_HasJump;
	pThenBlock->m_Flags |= EBasicBlockFlag_Jumped;
	pElseBlock->m_Flags |= EBasicBlockFlag_Jumped;

	m_pModule->m_LlvmBuilder.CreateCondBr (BoolValue, pThenBlock, pElseBlock);

	if (!pFollowBlock)
		pFollowBlock = pThenBlock;

	SetCurrentBlock (pFollowBlock);
	return true;
}

bool
CControlFlowMgr::Break (size_t Level)
{
	CScope* pTargetScope = m_pModule->m_NamespaceMgr.FindBreakScope (Level);
	if (!pTargetScope)
	{
		err::SetFormatStringError (_T("illegal break"));
		return false;
	}

	ProcessDestructList (pTargetScope);
	Jump (pTargetScope->m_pBreakBlock);
	return true;
}

bool
CControlFlowMgr::Continue (size_t Level)
{
	CScope* pTargetScope = m_pModule->m_NamespaceMgr.FindBreakScope (Level);
	if (!pTargetScope)
	{
		err::SetFormatStringError (_T("illegal continue"));
		return false;
	}

	ProcessDestructList (pTargetScope);
	Jump (pTargetScope->m_pContinueBlock);
	return true;
}

void
CControlFlowMgr::ProcessDestructList (CScope* pTargetScope)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	while (pScope && pScope != pTargetScope && pScope->GetFunction () == pFunction)
	{	
		m_pModule->m_OperatorMgr.ProcessDestructList (pScope->GetDestructList ());
		pScope = pScope->GetParentScope ();
	}
}

void
CControlFlowMgr::RestoreScopeLevel ()
{
	CValue ScopeLevelValue = m_pModule->m_FunctionMgr.GetScopeLevelValue ();
	if (!ScopeLevelValue)
		return;

	m_pModule->m_LlvmBuilder.CreateComment (_T("restore scope level before return"));
	m_pModule->m_LlvmBuilder.CreateStore (ScopeLevelValue, m_pModule->m_VariableMgr.GetScopeLevelVariable ());
}

bool
CControlFlowMgr::Return (
	const CValue& Value,
	bool IsSilent
	)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	CFunctionType* pFunctionType = pFunction->GetType ();
	CType* pReturnType = pFunctionType->GetReturnType ();
	
	if (IsSilent && !m_pSilentReturnBlock)
	{
		err::SetFormatStringError (_T("cannot 'silent return' from '%s'"), pFunction->m_Tag);
		return false;
	}

	if (!Value)
	{
		if (pFunction->GetType ()->GetReturnType ()->GetTypeKind () != EType_Void)
		{
			err::SetFormatStringError (_T("function '%s' must return a '%s' value"), pFunction->m_Tag, pReturnType->GetTypeString ());
			return false;
		}
		
		ProcessDestructList ();
		RestoreScopeLevel ();

		if (m_pReturnBlock)
			Jump (IsSilent ? m_pSilentReturnBlock : m_pReturnBlock);
		else
			m_pModule->m_LlvmBuilder.CreateRet ();
	}
	else
	{
		CValue ReturnValue;
		bool Result = m_pModule->m_OperatorMgr.CastOperator (Value, pReturnType, &ReturnValue);
		if (!Result)
			return false;

		ProcessDestructList ();
		RestoreScopeLevel ();
		m_pModule->m_LlvmBuilder.CreateRet (ReturnValue);
	}

	m_Flags |= EControlFlowFlag_HasReturn;

	SetCurrentBlock (GetUnreachableBlock ());
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
