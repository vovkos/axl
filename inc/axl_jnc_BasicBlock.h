// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

namespace axl {
namespace jnc {

class CFunction;

//.............................................................................

enum EHasReturn
{
	EHasReturn_Undefined,
	EHasReturn_None,
	EHasReturn_Some,
	EHasReturn_All,
	EHasReturn_Explicit,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EBasicBlockFlag
{
	EBasicBlockFlag_IsHasReturnReady  = 0x01,
	EBasicBlockFlag_IsHasReturnCalc   = 0x02,
	EBasicBlockFlag_IsUnreachable     = 0x10,
	EBasicBlockFlag_IsJumped          = 0x20,
};		

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBasicBlock: public rtl::TListLink
{
protected:
	friend class CControlFlowMgr;

	rtl::CString m_Name;
	CFunction* m_pFunction;
	llvm::BasicBlock* m_pLlvmBlock;

	rtl::CArrayT <CBasicBlock*> m_JumpArray;

	EHasReturn m_HasReturn;
	int m_Flags;

public:
	CBasicBlock ();

	EHasReturn
	HasReturn ();
	
	int 
	GetFlags ()
	{
		return m_Flags;
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
