// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BasicBlock.h"

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
	CControlFlowMgr ()
	{
		m_pModule = NULL;
	}

	void
	Clear ();

	CBasicBlock* 
	CreateBlock ();

	CBasicBlock* 
	GetCurrentBlock ()
	{
		return m_pCurrentBlock;
	}

	void
	SetCurrentBlock (
		CBasicBlock* pBlock,
		int Flags = 0
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
