// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EControlFlowMgrFlag
{
	EControlFlowMgrFlag_HasReturn = 1,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CControlFlowMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	rtl::CStdListT <CBasicBlock> m_BlockList;
	CBasicBlock* m_pCurrentBlock;
	CBasicBlock* m_pUnreachableBlock;

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

	void
	ResetHasReturn ()
	{
		m_Flags &= ~EControlFlowMgrFlag_HasReturn;
	}

	int 
	GetFlags ()
	{
		return m_Flags;
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
		CBasicBlock* pFollowBlock
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
	RestoreScopeLevel (CFunction* pFunction);

	void
	ProcessDestructList (CScope* pTargetScope = NULL);

	CBasicBlock*
	GetUnreachableBlock ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
