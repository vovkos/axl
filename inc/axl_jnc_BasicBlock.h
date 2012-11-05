// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

namespace axl {
namespace jnc {

class CFunction;

//.............................................................................

class CBasicBlock: public rtl::TListLink
{
protected:
	friend class CControlFlowMgr;

	rtl::CString m_Name;
	CFunction* m_pFunction;
	llvm::BasicBlock* m_pLlvmBlock;

public:
	CBasicBlock ()
	{
		m_pLlvmBlock = NULL;
		m_pFunction = NULL;
	}

	rtl::CString 
	GetName ()
	{
		return m_Name;
	}

	CFunction* 
	GetFunction ()
	{
		return m_pFunction;
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
