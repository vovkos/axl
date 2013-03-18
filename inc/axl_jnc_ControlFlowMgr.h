// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EControlFlowFlag
{
	EControlFlowFlag_HasReturn = 1,
	EControlFlowFlag_HasJump   = 2,
};

//.............................................................................

class CControlFlowMgr
{
protected:
	friend class CModule;
	friend class CFunctionMgr;

protected:
	CModule* m_pModule;

	rtl::CStdListT <CBasicBlock> m_BlockList;
	CBasicBlock* m_pUnreachableBlock;
	CBasicBlock* m_pCurrentBlock;
	CBasicBlock* m_pReturnBlock; // bindable setters & destructors return here

	int m_Flags;

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

	void
	MarkUnreachable (CBasicBlock* pBlock);

	bool
	Return (const CValue& Value);

	bool
	Return ()
	{
		return Return (CValue ());
	}

	bool
	Break (size_t Level);
	 
	bool
	Continue (size_t Level);
	
protected:
	void
	RestoreScopeLevel ();

	void
	ProcessDestructList (CScope* pTargetScope = NULL);

	CBasicBlock*
	GetUnreachableBlock ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
