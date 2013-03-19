#include "stdafx.h"
#include "axl_jnc_ControlFlowMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CControlFlowMgr::IfStmt_Create (TIfStmt* pStmt)
{
	pStmt->m_pThenBlock = CreateBlock (_T("if_then"));
	pStmt->m_pElseBlock = CreateBlock (_T("if_else"));
	pStmt->m_pFollowBlock = pStmt->m_pElseBlock;
}

bool
CControlFlowMgr::IfStmt_Condition (
	TIfStmt* pStmt,
	const CValue& Value,
	const CToken::CPos& Pos
	)
{
	bool Result = ConditionalJump (Value, pStmt->m_pThenBlock, pStmt->m_pElseBlock);
	if (!Result)
		return false;

	m_pModule->m_NamespaceMgr.OpenScope (Pos);
	return true;
}

void
CControlFlowMgr::IfStmt_Else (
	TIfStmt* pStmt,
	const CToken::CPos& Pos
	)
{
	m_pModule->m_NamespaceMgr.CloseScope (Pos);
	pStmt->m_pFollowBlock = CreateBlock (_T("if_follow"));
	Jump (pStmt->m_pFollowBlock, pStmt->m_pElseBlock);
	m_pModule->m_NamespaceMgr.OpenScope (Pos);
}

void
CControlFlowMgr::IfStmt_Follow (
	TIfStmt* pStmt,
	const CToken::CPos& Pos
	)
{
	m_pModule->m_NamespaceMgr.CloseScope (Pos);
	Follow (pStmt->m_pFollowBlock);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CControlFlowMgr::SwitchStmt_Create (TSwitchStmt* pStmt)
{
	pStmt->m_pSwitchBlock = NULL;
	pStmt->m_pDefaultBlock = NULL;
	pStmt->m_pFollowBlock = CreateBlock (_T("switch_follow"));
}

bool
CControlFlowMgr::SwitchStmt_Condition (
	TSwitchStmt* pStmt,
	const CValue& Value,
	const CToken::CPos& Pos
	)
{	
	bool Result = m_pModule->m_OperatorMgr.CastOperator (Value, EType_Int, &pStmt->m_Value);
	if (!Result)
		return false;

	pStmt->m_pSwitchBlock = GetCurrentBlock ();

	CBasicBlock* pBodyBlock = CreateBlock (_T("switch_body"));
	SetCurrentBlock (pBodyBlock);
	MarkUnreachable (pBodyBlock);

	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos);
	pScope->m_pBreakBlock = pStmt->m_pFollowBlock;
	return true;
}

bool
CControlFlowMgr::SwitchStmt_Case (
	TSwitchStmt* pStmt,
	const CValue& Value,
	const CToken::CPos& Pos1,
	const CToken::CPos& Pos2
	)
{
	rtl::CHashTableMapIteratorT <intptr_t, CBasicBlock*> It = pStmt->m_CaseMap.Goto (Value);
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("redefinition of label (%d) of switch statement"), Value);
		return false;
	}

	m_pModule->m_NamespaceMgr.CloseScope (Pos1);

	CBasicBlock* pBlock = CreateBlock (_T("switch_case"));
	Follow (pBlock);
	It->m_Value = pBlock;

	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos2);
	pScope->m_pBreakBlock = pStmt->m_pFollowBlock;
	return true;
}

bool
CControlFlowMgr::SwitchStmt_Default (
	TSwitchStmt* pStmt,
	const CToken::CPos& Pos1,
	const CToken::CPos& Pos2
	)
{
	if (pStmt->m_pDefaultBlock)
	{
		err::SetFormatStringError (_T("redefinition of 'default' label of switch statement"));
		return false;
	}

	m_pModule->m_NamespaceMgr.CloseScope (Pos1);

	CBasicBlock* pBlock = CreateBlock (_T("switch_default"));
	Follow (pBlock);
	pStmt->m_pDefaultBlock = pBlock;

	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos2);
	pScope->m_pBreakBlock = pStmt->m_pFollowBlock;
	return true;
}

void
CControlFlowMgr::SwitchStmt_Follow (
	TSwitchStmt* pStmt,
	const CToken::CPos& Pos
	)
{
	m_pModule->m_NamespaceMgr.CloseScope (Pos);
	Follow (pStmt->m_pFollowBlock);

	SetCurrentBlock (pStmt->m_pSwitchBlock);

	CBasicBlock* pDefaultBlock = pStmt->m_pDefaultBlock ? pStmt->m_pDefaultBlock : pStmt->m_pFollowBlock;

	m_pModule->m_LlvmBuilder.CreateSwitch (
		pStmt->m_Value, 
		pDefaultBlock,
		pStmt->m_CaseMap.GetHead (),
		pStmt->m_CaseMap.GetCount ()
		);

	SetCurrentBlock (pStmt->m_pFollowBlock);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CControlFlowMgr::WhileStmt_Create (TWhileStmt* pStmt)
{
	pStmt->m_pConditionBlock = CreateBlock (_T("while_condition"));
	pStmt->m_pBodyBlock = CreateBlock (_T("while_body"));
	pStmt->m_pFollowBlock = CreateBlock (_T("while_follow"));
	Follow (pStmt->m_pConditionBlock);
}

bool
CControlFlowMgr::WhileStmt_Condition (
	TWhileStmt* pStmt,
	const CValue& Value,
	const CToken::CPos& Pos
	)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos);
	pScope->m_pBreakBlock = pStmt->m_pFollowBlock;
	pScope->m_pContinueBlock = pStmt->m_pConditionBlock;
	return ConditionalJump (Value, pStmt->m_pBodyBlock, pStmt->m_pFollowBlock);
}

void
CControlFlowMgr::WhileStmt_Follow (
	TWhileStmt* pStmt,
	const CToken::CPos& Pos
	)
{
	m_pModule->m_NamespaceMgr.CloseScope (Pos);
	Jump (pStmt->m_pConditionBlock, pStmt->m_pFollowBlock);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CControlFlowMgr::DoStmt_Create (TDoStmt* pStmt)
{
	pStmt->m_pConditionBlock = CreateBlock (_T("do_condition"));
	pStmt->m_pBodyBlock = CreateBlock (_T("do_body"));
	pStmt->m_pFollowBlock = CreateBlock (_T("do_follow"));
	Follow (pStmt->m_pBodyBlock);
}

void
CControlFlowMgr::DoStmt_PreBody (
	TDoStmt* pStmt,
	const CToken::CPos& Pos
	)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos);
	pScope->m_pBreakBlock = pStmt->m_pFollowBlock;
	pScope->m_pContinueBlock = pStmt->m_pConditionBlock;
}

void
CControlFlowMgr::DoStmt_PostBody (
	TDoStmt* pStmt,
	const CToken::CPos& Pos
	)
{
	m_pModule->m_NamespaceMgr.CloseScope (Pos);
	Follow (pStmt->m_pConditionBlock);
}

bool
CControlFlowMgr::DoStmt_Condition (
	TDoStmt* pStmt,
	const CValue& Value
	)
{
	return ConditionalJump (Value, pStmt->m_pBodyBlock, pStmt->m_pFollowBlock, pStmt->m_pFollowBlock);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CControlFlowMgr::ForStmt_Create (TForStmt* pStmt)
{
	pStmt->m_pBodyBlock = CreateBlock (_T("for_body"));
	pStmt->m_pFollowBlock = CreateBlock (_T("for_follow"));
	pStmt->m_pConditionBlock = pStmt->m_pBodyBlock;
	pStmt->m_pLoopBlock = pStmt->m_pBodyBlock;
}

void
CControlFlowMgr::ForStmt_PreInit (
	TForStmt* pStmt,
	const CToken::CPos& Pos
	)
{
	pStmt->m_pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos);
}

void
CControlFlowMgr::ForStmt_PreCondition (TForStmt* pStmt)
{
	pStmt->m_pConditionBlock = CreateBlock (_T("for_condition"));
	pStmt->m_pLoopBlock = pStmt->m_pConditionBlock;
	Follow (pStmt->m_pConditionBlock);
}

bool
CControlFlowMgr::ForStmt_PostCondition (
	TForStmt* pStmt,
	const CValue& Value
	)
{
	return ConditionalJump (Value, pStmt->m_pBodyBlock, pStmt->m_pFollowBlock);
}

void
CControlFlowMgr::ForStmt_PreLoop (TForStmt* pStmt)
{
	pStmt->m_pLoopBlock = CreateBlock (_T("for_loop"));
	SetCurrentBlock (pStmt->m_pLoopBlock);
}

void
CControlFlowMgr::ForStmt_PostLoop (TForStmt* pStmt)
{
	Jump (pStmt->m_pConditionBlock, pStmt->m_pBodyBlock);
}

void
CControlFlowMgr::ForStmt_PreBody (TForStmt* pStmt)
{
	pStmt->m_pScope->m_pBreakBlock = pStmt->m_pFollowBlock;
	pStmt->m_pScope->m_pContinueBlock = pStmt->m_pConditionBlock;
}

void
CControlFlowMgr::ForStmt_PostBody (
	TForStmt* pStmt, 
	const CToken::CPos& Pos
	)
{
	m_pModule->m_NamespaceMgr.CloseScope (Pos);
	Jump (pStmt->m_pLoopBlock, pStmt->m_pFollowBlock);

	if (!(pStmt->m_pFollowBlock->GetFlags () & EBasicBlockFlag_Jumped))
		MarkUnreachable (pStmt->m_pFollowBlock);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CControlFlowMgr::OnceStmt_Create (TOnceStmt* pStmt)
{
	pStmt->m_pFlagVariable = m_pModule->m_VariableMgr.CreateVariable (
		EStorage_Static, 
		_T("once_flag"), 
		_T("once_flag"), 
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32),
		EPtrTypeFlag_Volatile
		);

	pStmt->m_pPreBodyBlock = CreateBlock (_T("once_prebody"));
	pStmt->m_pBodyBlock = CreateBlock (_T("once_body"));
	pStmt->m_pLoopBlock = CreateBlock (_T("once_loop"));
	pStmt->m_pFollowBlock = CreateBlock (_T("once_follow"));
}

bool
CControlFlowMgr::OnceStmt_PreBody (
	TOnceStmt* pStmt, 
	const CToken::CPos& Pos
	)
{
	bool Result;

	CType* pType = pStmt->m_pFlagVariable->GetType ();

	CValue Value;

	Result = 
		m_pModule->m_VariableMgr.AllocateVariable (pStmt->m_pFlagVariable) &&
		m_pModule->m_OperatorMgr.LoadDataRef (pStmt->m_pFlagVariable, &Value);
	
	if (!Result)
		return false;

	intptr_t ConstArray [2] = { 0, 1 };
	CBasicBlock* BlockArray [2] = { pStmt->m_pPreBodyBlock, pStmt->m_pLoopBlock };

	m_pModule->m_LlvmBuilder.CreateSwitch (Value, pStmt->m_pFollowBlock, ConstArray, BlockArray, 2);

	// loop

	SetCurrentBlock (pStmt->m_pLoopBlock);

	Result = 
		m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Eq, pStmt->m_pFlagVariable, CValue (2, pType), &Value) &&
		ConditionalJump (Value, pStmt->m_pFollowBlock, pStmt->m_pLoopBlock, pStmt->m_pPreBodyBlock);

	if (!Result)
		return false;

	// pre body

	m_pModule->m_LlvmBuilder.CreateCmpXchg (
		pStmt->m_pFlagVariable, 
		CValue ((int64_t) 0, pType), 
		CValue (1, pType), 
		llvm::Acquire,
		llvm::CrossThread,
		&Value
		);

	Result = 
		m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Eq, Value, CValue ((int64_t) 0, pType), &Value) &&
		ConditionalJump (Value, pStmt->m_pBodyBlock, pStmt->m_pLoopBlock);

	if (!Result)
		return false;

	m_pModule->m_NamespaceMgr.OpenScope (Pos);
	return true;
}

void
CControlFlowMgr::OnceStmt_PostBody (
	TOnceStmt* pStmt, 
	const CToken::CPos& Pos
	)
{
	m_pModule->m_NamespaceMgr.CloseScope (Pos);

	CValue TmpValue;
	m_pModule->m_LlvmBuilder.CreateRmw (
		llvm::AtomicRMWInst::Xchg,
		pStmt->m_pFlagVariable, 
		CValue ((int64_t) 2, EType_Int32), 
		llvm::Release,
		llvm::CrossThread,
		&TmpValue
		);

	Follow (pStmt->m_pFollowBlock);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
