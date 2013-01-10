// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

class CControlFlowMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	rtl::CStdListT <CBasicBlock> m_BlockList;
	CBasicBlock* m_pCurrentBlock;

public:
	CControlFlowMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

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

	bool
	ConditionalJump (
		const CValue& Value,
		CBasicBlock* pThenBlock,
		CBasicBlock* pElseBlock,
		CBasicBlock* pFollowBlock = NULL // if NULL then follow with pThenBlock
		);

	void
	MarkUnreachable (CBasicBlock* pBlock);

	void
	Follow (CBasicBlock* pBlock)
	{
		Jump (pBlock, pBlock);
	}

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
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
