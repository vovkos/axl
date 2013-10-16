// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

struct TIfStmt
{
	CBasicBlock* m_pThenBlock;
	CBasicBlock* m_pElseBlock;
	CBasicBlock* m_pFollowBlock;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSwitchStmt
{
	CValue m_Value;
	CBasicBlock* m_pSwitchBlock;
	CBasicBlock* m_pDefaultBlock;
	CBasicBlock* m_pFollowBlock;
	rtl::CHashTableMapT <intptr_t, CBasicBlock*, axl::rtl::CHashIdT <intptr_t> > m_CaseMap;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TWhileStmt
{
	CBasicBlock* m_pConditionBlock;
	CBasicBlock* m_pBodyBlock;
	CBasicBlock* m_pFollowBlock;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TDoStmt
{
	CBasicBlock* m_pConditionBlock;
	CBasicBlock* m_pBodyBlock;
	CBasicBlock* m_pFollowBlock;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TForStmt
{
	CScope* m_pScope;
	CBasicBlock* m_pConditionBlock;
	CBasicBlock* m_pBodyBlock;
	CBasicBlock* m_pLoopBlock;
	CBasicBlock* m_pFollowBlock;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TOnceStmt
{
	CVariable* m_pFlagVariable;
	CBasicBlock* m_pPreBodyBlock;
	CBasicBlock* m_pBodyBlock;
	CBasicBlock* m_pLoopBlock;
	CBasicBlock* m_pFollowBlock;
};

//.............................................................................

enum EControlFlowFlag
{
	EControlFlowFlag_HasReturn = 1,
	EControlFlowFlag_HasJump   = 2,
};

//.............................................................................

class CControlFlowMgr
{
	friend class CModule;
	friend class CFunctionMgr;

protected:
	CModule* m_pModule;

	rtl::CStdListT <CBasicBlock> m_BlockList;
	CBasicBlock* m_pCurrentBlock;
	CBasicBlock* m_pReturnBlock; // bindable setters & destructors return here
	CBasicBlock* m_pSilentReturnBlock; // bindable setters
	CBasicBlock* m_pUnreachableBlock;

	uint_t m_Flags;

public:
	CControlFlowMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	int
	GetFlags ()
	{
		return m_Flags;
	}

	void
	ResetJumpFlag ()
	{
		m_Flags &= ~EControlFlowFlag_HasJump;
	}

	CBasicBlock* 
	CreateBlock (const rtl::CString& Name);

	CBasicBlock* 
	GetCurrentBlock ()
	{
		return m_pCurrentBlock;
	}

	CBasicBlock* 
	SetCurrentBlock (CBasicBlock* pBlock); // returns prev

	void
	MarkUnreachable (CBasicBlock* pBlock);

	// jumps

	void
	Jump (
		CBasicBlock* pBlock,
		CBasicBlock* pFollowBlock = NULL
		);

	void
	Follow (CBasicBlock* pBlock);

	bool
	ConditionalJump (
		const CValue& Value,
		CBasicBlock* pThenBlock,
		CBasicBlock* pElseBlock,
		CBasicBlock* pFollowBlock = NULL // if NULL then follow with pThenBlock
		);

	bool
	Break (size_t Level);
	 
	bool
	Continue (size_t Level);

	bool
	Return (
		const CValue& Value,
		bool IsSilent = false
		);

	bool
	Return ()
	{
		return Return (CValue (), false);
	}

	bool
	Unwind (const CValue& IndicatorValue);

	bool
	Catch ();

	bool
	Finally ();

	bool
	EndTry ();

	bool
	EndFinally ();

	// if stmt

	void
	IfStmt_Create (TIfStmt* pStmt);

	bool
	IfStmt_Condition (
		TIfStmt* pStmt,
		const CValue& Value,
		const CToken::CPos& Pos
		);

	void
	IfStmt_Else (
		TIfStmt* pStmt,
		const CToken::CPos& Pos
		);

	void
	IfStmt_Follow (
		TIfStmt* pStmt,
		const CToken::CPos& Pos
		);

	// switch stmt

	void
	SwitchStmt_Create (TSwitchStmt* pStmt);

	bool
	SwitchStmt_Condition (
		TSwitchStmt* pStmt,
		const CValue& Value,
		const CToken::CPos& Pos
		);

	bool
	SwitchStmt_Case (
		TSwitchStmt* pStmt,
		intptr_t Value,
		const CToken::CPos& Pos1,
		const CToken::CPos& Pos2
		);

	bool
	SwitchStmt_Default (
		TSwitchStmt* pStmt,
		const CToken::CPos& Pos1,
		const CToken::CPos& Pos2
		);

	void
	SwitchStmt_Follow (
		TSwitchStmt* pStmt,
		const CToken::CPos& Pos
		);

	// while stmt

	void
	WhileStmt_Create (TWhileStmt* pStmt);

	bool
	WhileStmt_Condition (
		TWhileStmt* pStmt,
		const CValue& Value,
		const CToken::CPos& Pos
		);

	void
	WhileStmt_Follow (
		TWhileStmt* pStmt,
		const CToken::CPos& Pos
		);

	// do stmt

	void
	DoStmt_Create (TDoStmt* pStmt);

	void
	DoStmt_PreBody (
		TDoStmt* pStmt,
		const CToken::CPos& Pos
		);

	void
	DoStmt_PostBody (
		TDoStmt* pStmt,
		const CToken::CPos& Pos
		);

	bool
	DoStmt_Condition (
		TDoStmt* pStmt,
		const CValue& Value
		);

	// for stmt

	void
	ForStmt_Create (TForStmt* pStmt);

	void
	ForStmt_PreInit (
		TForStmt* pStmt,
		const CToken::CPos& Pos
		);

	void
	ForStmt_PreCondition (TForStmt* pStmt);

	bool
	ForStmt_PostCondition (
		TForStmt* pStmt,
		const CValue& Value
		);

	void
	ForStmt_PreLoop (TForStmt* pStmt);

	void
	ForStmt_PostLoop (TForStmt* pStmt);

	void
	ForStmt_PreBody (TForStmt* pStmt);

	void
	ForStmt_PostBody (
		TForStmt* pStmt, 
		const CToken::CPos& Pos
		);

	// once stmt

	bool
	OnceStmt_Create (
		TOnceStmt* pStmt,
		EStorage StorageKind = EStorage_Static
		);

	void
	OnceStmt_Create (
		TOnceStmt* pStmt,
		CVariable* pFlagVariable
		);

	bool
	OnceStmt_PreBody (
		TOnceStmt* pStmt, 
		const CToken::CPos& Pos
		);

	void
	OnceStmt_PostBody (
		TOnceStmt* pStmt, 
		const CToken::CPos& Pos
		);

protected:
	void
	AddBlock (CBasicBlock* pBlock);

	void
	RestoreScopeLevel ();

	void
	OnLeaveScope (CScope* pTargetScope = NULL);

	CBasicBlock*
	GetUnreachableBlock ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
