// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_rtl_List.h"
#include "axl_rtl_String.h"

namespace axl {
namespace jnc {

class CFunction;

//.............................................................................

enum EBasicBlockFlag
{
	EBasicBlockFlag_Unreachable = 0x10,
	EBasicBlockFlag_Jumped      = 0x20,
	EBasicBlockFlag_Entry       = 0x40,
};		

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBasicBlock: public rtl::TListLink
{
	friend class CControlFlowMgr;
	friend class CLlvmBuilder;

protected:
	rtl::CString m_Name;
	rtl::CString m_LeadingComment;
	CFunction* m_pFunction;
	llvm::BasicBlock* m_pLlvmBlock;

	uint_t m_Flags;

public:
	CBasicBlock ();

	int 
	GetFlags ()
	{
		return m_Flags;
	}

	bool
	HasTerminator ()
	{
		return m_pLlvmBlock->getTerminator () != NULL;
	}

	bool
	HasReturn ()
	{
		llvm::TerminatorInst* pInst = m_pLlvmBlock->getTerminator ();
		return pInst && pInst->getOpcode () == llvm::Instruction::Ret;
	}

	rtl::CString 
	GetName ()
	{
		return m_Name;
	}

	rtl::CString 
	GetLeadingComment ()
	{
		return m_LeadingComment;
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

	void
	MarkEntry ()
	{
		m_Flags |= EBasicBlockFlag_Entry;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
