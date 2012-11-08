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

	void
	SetCurrentBlock (CBasicBlock* pBlock);

	void
	Jump (
		CBasicBlock* pBlock,
		CBasicBlock* pFollowBlock
		);

	bool
	ConditionalJump (
		const CValue& Value,
		CBasicBlock* pThenBlock,
		CBasicBlock* pElseBlock
		);

	void
	Follow (CBasicBlock* pBlock)
	{
		Jump (pBlock, pBlock);
	}

	bool
	Return (const CValue& Value);

	bool
	Break (size_t Level);

	bool
	Continue (size_t Level);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
