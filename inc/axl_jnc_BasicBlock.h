// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

namespace axl {
namespace jnc {

//.............................................................................

class CBasicBlock: public rtl::TListLink
{
protected:
	friend class CControlFlowMgr;

	rtl::CString m_Name;
	llvm::BasicBlock* m_pLlvmBlock;

public:
	CBasicBlock ()
	{
		m_pLlvmBlock = NULL;
	}

	rtl::CString 
	GetName ()
	{
		return m_Name;
	}

	llvm::BasicBlock*
	GetLlvmBlock ()
	{
		return m_pLlvmBlock;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
