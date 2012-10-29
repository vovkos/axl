// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EVariable
{
	EVariable_Global,
	EVariable_Local,
	EVariable_Arg,
	EVariable_Temp
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
class CVariable:
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CVariableMgr;

	EVariable m_VariableKind;
	CType* m_pType;

	union
	{
		llvm::Value* m_pLlvmValue;
		llvm::GlobalVariable* m_pLlvmGlobalVariable;
		llvm::AllocaInst* m_pLlvmAlloca;
	};

public:
	CVariable ();

	~CVariable ();

	EVariable
	GetVariableKind ()
	{
		return m_VariableKind;
	}

	CType* 
	GetType ()
	{
		return m_pType;
	}

	llvm::Value* 
	GetLlvmValue ()
	{
		return m_pLlvmValue;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
