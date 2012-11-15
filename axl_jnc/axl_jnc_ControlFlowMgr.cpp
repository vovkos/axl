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

void
CControlFlowMgr::SetCurrentBlock (CBasicBlock* pBlock)
{
	m_pCurrentBlock = pBlock;
	m_pModule->m_LlvmBuilder.SetInsertPoint (pBlock->GetLlvmBlock ());

	if (pBlock->m_pFunction)
		return;
	
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	pFunction->GetLlvmFunction ()->getBasicBlockList ().push_back (pBlock->m_pLlvmBlock);
	pBlock->m_pFunction = pFunction;
}

void
CControlFlowMgr::Jump (
	CBasicBlock* pBlock,
	CBasicBlock* pFollowBlock
	)
{
	m_pModule->m_LlvmBuilder.CreateBr (pBlock->m_pLlvmBlock);

	bool IsUnreachable = pFollowBlock == NULL;

	if (IsUnreachable) 
		pFollowBlock = CreateBlock (_T("jmp_follow"));

	SetCurrentBlock (pFollowBlock);

	if (IsUnreachable)
		m_pModule->m_LlvmBuilder.CreateUnreachable ();
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

	m_pModule->m_LlvmBuilder.CreateCondBr (
		BoolValue.GetLlvmValue (), 
		pThenBlock->GetLlvmBlock (), 
		pElseBlock->GetLlvmBlock ()
		);

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

	m_pModule->m_ControlFlowMgr.Jump (pScope->GetBreakBlock (), NULL);
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

	m_pModule->m_ControlFlowMgr.Jump (pScope->GetContinueBlock (), NULL);
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

		m_pModule->m_LlvmBuilder.CreateRetVoid ();
	}
	else
	{
		CValue ReturnValue;
		bool Result = m_pModule->m_OperatorMgr.CastOperator (Value, pReturnType, &ReturnValue);
		if (!Result)
			return false;		

		llvm::Value* pLlvmValue = ReturnValue.GetLlvmValue ();

		if (pReturnType->GetTypeKind () == EType_Pointer)
		{
			if (ReturnValue.GetValueKind () == EValue_Variable)
			{
				CVariable* pVariable = ReturnValue.GetVariable ();
				CScope* pScope = pVariable->GetScope ();

				pLlvmValue = m_pModule->m_OperatorMgr.CreateLlvmSafePtr (
					pLlvmValue, 
					pVariable->GetLlvmValue (), 
					pVariable->GetType (),
					pScope ? pScope->GetLevel () : 0
					);
			}

			Result = m_pModule->m_OperatorMgr.CheckLlvmSafePtrScope (pLlvmValue, ReturnValue, 0);
			if (!Result)
				return false;
		}

		m_pModule->m_LlvmBuilder.CreateRet (pLlvmValue);
	}

	CBasicBlock* pFollowBlock = CreateBlock (_T("ret_follow"));
	SetCurrentBlock (pFollowBlock);
	m_pModule->m_LlvmBuilder.CreateUnreachable ();
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
